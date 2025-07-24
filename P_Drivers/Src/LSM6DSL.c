/*
 * LSM6DSL.c
 *
 *  Created on: Jul 6, 2025
 *      Author: mrohi
 */

#include "LSM6DSL.h"

extern I2C_HandleTypeDef myi2c2;

//XL G sensitivity
double sensitivity_xl[4] = {0.061, 0.122, 0.244, 0.488};
double sensitivity_g[5] = {4.375, 8.75, 17.50, 35.0, 70.0};
uint8_t full_scale_xl;
uint8_t full_scale_g;
uint8_t timestamp_resolution;

//I2C Write
void lsm6ds3_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    HAL_I2C_Master_Transmit(&myi2c2, LSM6DSL_ADD, buf, 2, HAL_MAX_DELAY);
}

//I2C Read
void lsm6ds3_read(uint8_t reg, uint8_t* val, int count)
{
	uint8_t buf = reg;
    HAL_I2C_Master_Transmit(&myi2c2, LSM6DSL_ADD, &buf, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&myi2c2, LSM6DSL_ADD, val, count, HAL_MAX_DELAY);
}


//Initialization and Setup
void lsm6ds3_init(void)
{

	lsm6ds3_write(CTRL3_C, SW_RESET);
	uint8_t temp[2] = {0};
	do
	{
		temp[0] = CTRL3_C;
		HAL_I2C_Master_Transmit(&myi2c2, LSM6DSL_ADD, temp, 1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&myi2c2, LSM6DSL_ADD, temp+1, 1, HAL_MAX_DELAY);
	}while(temp[1] & (SW_RESET));


	lsm6ds3_write(CTRL3_C, (BDU) | (IF_INC));

	full_scale_xl = FS_XL_4G;
	lsm6ds3_enable_accel(ODR_XL_208HZ, FS_XL_4G);

	//Filter
	lsm6ds3_write(CTRL8_XL, ACC_LPF2_ENABLE | ACC_HPCF_ODR_100);
	lsm6ds3_write(CTRL7_G, GYRO_HPF_ENABLE | GYRO_HPF_65mHz);

	full_scale_g = FS_G_500DPS;
	lsm6ds3_enable_gyro(ODR_G_208HZ, FS_G_500DPS);

	lsm6ds3_write(CTRL10_C, (TIMER_EN) | (FUNC_EN) | (WRIST_TILT_EN));

	// WAKE_UP_DUR
	timestamp_resolution = WAKE_UP_DUR_TIMER_HR;
	lsm6ds3_write(WAKE_UP_DUR_REG, WAKE_UP_DUR_TIMER_HR);

	lsm6ds3_write(TAP_CFG, (TAP_CFG_TAP_X_EN) | (TAP_CFG_TAP_Y_EN) | (TAP_CFG_TAP_Z_EN) | (TAP_CFG_INT_ENABLE) | (TAP_CFG_LIR));

	//TAP_THS_6D
	lsm6ds3_write(TAP_THS_6D, 0x08);

	//INT_DUR2
	lsm6ds3_write(INT_DUR2, 0x53);

	//WAKE_UP_THS
	lsm6ds3_write(WAKE_UP_THS_REG, (0x1 << 7));

    // Route tap/wrist tilt interrupts to INT1 or INT2 (MD1_CFG / MD2_CFG)
	lsm6ds3_write(MD1_CFG, (INT1_SINGLE_TAP) | (INT1_DOUBLE_TAP) | (INT1_TILT));

}


void lsm6ds3_set_bank(uint8_t bank_flag)
{
	if(bank_flag == EN_BANK_A || bank_flag == EN_BANK_B)
		lsm6ds3_write(FUNC_CFG_ACCESS, bank_flag);
	else
		printf("Incorrect bank_flag.\n");
	return;
}


//Sensor Configuration
void lsm6ds3_enable_accel(uint8_t odr, uint8_t fs)
{
	if((odr >= ODR_XL_POWER_DOWN) && (odr <= ODR_XL_6_66KHZ) && (fs >= FS_XL_2G)  && (fs <= FS_XL_8G))
	{
		full_scale_xl = fs;
		lsm6ds3_write(CTRL1_XL, odr | fs);
	}
	else
		printf("Incorrect ODR and fs.\n");
	return;
}

void lsm6ds3_enable_gyro(uint8_t odr, uint8_t fs)
{
	if((odr >= ODR_G_POWER_DOWN) && (odr <= ODR_G_6_66KHZ) && (fs >= FS_G_250DPS)  && (fs <= FS_G_2000DPS))
	{
		full_scale_g = fs;
		lsm6ds3_write(CTRL2_G, odr | fs);
	}
	else
		printf("Incorrect ODR and fs.\n");
	return;
}


void lsm6ds3_set_accel_lpf(uint8_t lpf_cfg)
{
	lsm6ds3_write(CTRL8_XL, lpf_cfg);
}

void lsm6ds3_set_gyro_lpf(uint8_t ftype)
{
	lsm6ds3_write(CTRL6_C, ftype);
}

void lsm6ds3_route_interrupt_to_int1(uint8_t mask)
{
	lsm6ds3_write(MD1_CFG, mask);
}

void lsm6ds3_route_interrupt_to_int2(uint8_t mask)
{
	lsm6ds3_write(MD2_CFG, mask);
}

void lsm6ds3_read_accel(float* ax_g, float* ay_g, float* az_g)
{
	uint8_t buf[6] = {0};
	lsm6ds3_read(OUTX_L_XL, buf, 6);

	int16_t temp1 = (buf[1] << 8) | buf[0];
	int16_t temp2 = (buf[3] << 8) | buf[2];
	int16_t temp3 = (buf[5] << 8) | buf[4];

	double sensitivity = get_accel_sensitivity();
	if(sensitivity == 255)
	{
		printf("Full Scale for XL not configured yet.\n");
		return;
	}

	*ax_g = temp1 * sensitivity / 1000.0f;
	*ay_g = temp2 * sensitivity / 1000.0f;
	*az_g = temp3 * sensitivity / 1000.0f;
}

void lsm6ds3_read_gyro(float* gx_dps, float* gy_dps, float* gz_dps)
{
	uint8_t buf[6] = {0};
	lsm6ds3_read(OUTX_L_G, buf, 6);

	int16_t temp1 = (buf[1] << 8) | buf[0];
	int16_t temp2 = (buf[3] << 8) | buf[2];
	int16_t temp3 = (buf[5] << 8) | buf[4];

	uint8_t sensitivity = get_gyro_sensitivity();
	if(sensitivity == 255)
	{
		printf("Full Scale for G not configured yet.\n");
		return;
	}
	*gx_dps = temp1 * sensitivity / 1000.0f;
	*gx_dps = temp2 * sensitivity / 1000.0f;
	*gx_dps = temp3 * sensitivity / 1000.0f;
}

void lsm6ds3_read_temp(float* temp)
{
	uint8_t buf[2] = {0};
	lsm6ds3_read(OUT_TEMP_L, buf, 2);
	int16_t raw = (buf[1] << 8 ) | buf[0];
	*temp = 25.0f + (raw / 256.0f);
}

void lsm6ds3_read_timestamp(uint32_t* timestamp)
{
	uint8_t buf[3] = {0};
	lsm6ds3_read(TIMESTAMP0_REG, buf, 3);
	uint32_t temp = (buf[2] << 16) | (buf[1] << 8) | buf[0];
	*timestamp = temp * timestamp_resolution;
}

double get_accel_sensitivity(void)
{
	switch(full_scale_xl)
	{
		case FS_XL_2G:  return sensitivity_xl[0];
		case FS_XL_4G:  return sensitivity_xl[1];
		case FS_XL_8G:  return sensitivity_xl[2];
		case FS_XL_16G: return sensitivity_xl[3];
		default:        return 255;
	}
}

double get_gyro_sensitivity(void)
{
	switch(full_scale_g)
	{
		case FS_G_125DPS:  return sensitivity_g[0];
		case FS_G_250DPS:  return sensitivity_g[1];
		case FS_G_500DPS:  return sensitivity_g[2];
		case FS_G_1000DPS: return sensitivity_g[3];
		case FS_G_2000DPS: return sensitivity_g[4];
		default:        return 255;
	}
}

//Tap Detection
void lsm6ds3_enable_tap(void)
{
    lsm6ds3_write(TAP_CFG, TAP_CFG_TAP_X_EN | TAP_CFG_TAP_Y_EN | TAP_CFG_TAP_Z_EN | TAP_CFG_INT_ENABLE | TAP_CFG_LIR);
    lsm6ds3_write(TAP_THS_6D, 0x08);
    lsm6ds3_write(INT_DUR2, 0x53);
    lsm6ds3_write(WAKE_UP_THS_REG, (1 << 7));
}

void lsm6ds3_read_tap_status(uint8_t* tap_src)
{
    lsm6ds3_read(TAP_SRC_REG, tap_src, 1);
}

//Wrist Detection
void lsm6ds3_enable_wrist_tilt(void)
{
    lsm6ds3_write(CTRL10_C, TIMER_EN | FUNC_EN | WRIST_TILT_EN);
}

void lsm6ds3_read_wrist_tilt(uint8_t* tilt_src)
{
    lsm6ds3_read(WRIST_TILT_IA, tilt_src, 1);
}

//Status
void lsm6ds3_read_status(uint8_t* status_reg)
{
    lsm6ds3_read(STATUS_REG, status_reg, 1);
}

void lsm6ds3_read_func_src(uint8_t* func_src1)
{
    lsm6ds3_read(FUNC_SRC1_REG, func_src1, 1);
}


//Control & Reboot
void lsm6ds3_reboot(void)
{
    lsm6ds3_write(CTRL3_C, BOOT);
}

void lsm6ds3_software_reset(void)
{
    lsm6ds3_write(CTRL3_C, SW_RESET);
    HAL_Delay(1); // Optional: wait briefly
    while (1)
    {
        uint8_t val = 0;
        lsm6ds3_read(CTRL3_C, &val, 1);
        if (!(val & SW_RESET))
            break;
    }
}

/************************************************
 * User can modify these functions but calling lsm6ds3_init()
 * is necessary unless you have implemented a new version
 ************************************************/
__weak void imu_init(void)
{

	IMU_alive();
	test_lsm6dsl_whoami();
	test_lis3mdl_whoami();

	lsm6ds3_init();

	lsm6ds3_write(TAP_THS_6D, 0x03);
	lsm6ds3_write(INT_DUR2, 0x7F);
	lsm6ds3_write(WAKE_UP_THS_REG, (1 << 7));

	lsm6ds3_write(TAP_CFG, TAP_CFG_TAP_X_EN | TAP_CFG_TAP_Y_EN | TAP_CFG_TAP_Z_EN | TAP_CFG_INT_ENABLE);

	lsm6ds3_enable_accel(ODR_XL_104HZ, FS_XL_2G);

	lsm6ds3_write(CTRL10_C, TIMER_EN | TILT_EN |  FUNC_EN | WRIST_TILT_EN);
}

__weak void poll_imu(void)
{
	float ax, ay, az;
	float gx, gy, gz;
	float temp;
	uint32_t ts;

	lsm6ds3_read_accel(&ax, &ay, &az);
	printf("Accel: X=%.2f Y=%.2f Z=%.2f\r\n", ax, ay, az);

	lsm6ds3_read_gyro(&gx, &gy, &gz);
	printf("Gyro: X=%.2f dps Y=%.2f dps Z=%.2f dps\r\n", gx, gy, gz);

	lsm6ds3_read_temp(&temp);
	printf("Temperature: %.2f C\r\n", temp);

	lsm6ds3_read_timestamp(&ts);
	printf("Timestamp: %lu ms\r\n", ts);

	uint8_t tap_src = 0;
	uint8_t tilt_src = 0;

	lsm6ds3_read_tap_status(&tap_src);
	lsm6ds3_read_wrist_tilt(&tilt_src);

	if (tap_src & 0x20) printf("Single Tap Detected\r\n");
	if (tap_src & 0x10) printf("Double Tap Detected\r\n");
	if ((tilt_src & 0x80) || (tilt_src & 0x40)) printf("Wrist Tilt Detected\r\n");

	HAL_Delay(100);
}


void IMU_alive(void)
{
	for(uint8_t addr = 0x00; addr < 0x7F; addr++) {
		if (HAL_I2C_IsDeviceReady(&myi2c2, addr << 1, 1, 10) == HAL_OK) {
			printf("Found device at 0x%02X\r\n", addr);
		}
	}
}

void test_lsm6dsl_whoami(void)
{
    uint8_t id = 0;

//    HAL_I2C_Master_Transmit(&myi2c, LSM6DSL_ADD << 1, &reg, 1, HAL_MAX_DELAY);
//    HAL_I2C_Master_Receive(&myi2c, LSM6DSL_ADD << 1, &id, 1, HAL_MAX_DELAY);
    lsm6ds3_read(WHO_AM_I, &id, 1);

    if (id == 0x6A)
        printf("LSM6DSL detected. WHO_AM_I = 0x%02X\r\n", id);
    else
        printf("Unexpected WHO_AM_I from LSM6DSL: 0x%02X\r\n", id);
}

void test_lis3mdl_whoami(void)
{
    uint8_t reg = 0x0F;
    uint8_t id = 0;

    HAL_I2C_Master_Transmit(&myi2c2, 0x1C << 1, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&myi2c2, 0x1C << 1, &id, 1, HAL_MAX_DELAY);

    printf("LIS3MDL WHO_AM_I = 0x%02X\r\n", id);
}

