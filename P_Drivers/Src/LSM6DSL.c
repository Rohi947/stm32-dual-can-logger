/*
 * LSM6DSL.c
 *
 *  Created on: Jul 6, 2025
 *      Author: mrohi
 */

#include "LSM6DSL.h"

extern I2C_HandleTypeDef myi2c2;

void lsm6ds3_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    HAL_I2C_Master_Transmit(&myi2c2, LSM6DSL_ADD, buf, 2, HAL_MAX_DELAY);
}


//Initialization and Setup
void lsm6ds3_init(void)
{
    // Perform software reset and wait until reset complete (CTRL3_C)
	lsm6ds3_write(CTRL3_C, SW_RESET);

	do
	{
		uint8_t temp[2] = {0};
		temp[0] = CTRL3_C;
		HAL_I2C_Master_Transmit(&myi2c2, LSM6DSL_ADD, temp, 1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&myi2c2, LSM6DSL_ADD, temp+1, 1, HAL_MAX_DELAY);
	}while(temp[1] & (SW_RESET));

    // Enable Block Data Update (BDU) and auto-increment register address (CTRL3_C)
    // Configure interrupt output type (active high/low, push-pull/open-drain) (CTRL3_C) //default
	lsm6ds3_write(CTRL3_C, (BDU) | (IF_INC));

    //  Configure accelerometer:
    // Set ODR, full scale, LPF1 bandwidth (CTRL1_XL)
	lsm6ds3_write(CTRL1_XL, (ODR_XL_208HZ) | (FS_XL_4G));

	//Filter
	lsm6ds3_write(CTRL8_XL, ACC_LPF2_ENABLE | ACC_HPCF_ODR_100);
	lsm6ds3_write(CTRL7_G, GYRO_HPF_ENABLE | GYRO_HPF_65mHz);


    // Configure gyroscope:
    // Set ODR, full scale (CTRL2_G)
    // Configure LPF1 (CTRL6_C)
    // Enable/disable HPF and set HPF cutoff (CTRL7_G)
	lsm6ds3_write(CTRL2_G, (ODR_G_208HZ) | (FS_G_500DPS));

    // Enable timestamp and set resolution (CTRL10_C)
	 //Enable embedded functionality block (FUNC_EN in CTRL10_C)
    // Enable wrist tilt detection (CTRL10_C)
	lsm6ds3_write(CTRL10_C, (TIMER_EN) | (FUNC_EN) | (WRIST_TILT_EN));

	// WAKE_UP_DUR
	lsm6ds3_write(WAKE_UP_DUR_REG, WAKE_UP_DUR_TIMER_HR);

    // Enable tap detection in X/Y/Z (TAP_CFG)
    // Set threshold, duration, quiet time, shock (TAP_THS_6D, INT_DUR2)
    // Enable single/double-tap mode (WAKE_UP_THS)
    // Optional – Enable latched interrupts (TAP_CFG)
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


void lsm6ds3_set_bank(uint8_t bank_flag);

