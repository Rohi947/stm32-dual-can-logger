/*
 * LSM6DSL.h
 *
 *  Created on: Jul 6, 2025
 *      Author: mrohi
 */

#ifndef INC_LSM6DSL_H_
#define INC_LSM6DSL_H_

#include "main.h"

#define LSM6DSL_ADD														0b1101010 << 1

/****************
 * Registers
 ***************/

// Identification Register
#define WHO_AM_I															0x0F

// Interrupt Registers
#define DRDY_PULSE_CFG_G											0x0B
#define INT1_CTRL                												0x0D
#define INT2_CTRL                 											0x0E
#define MD1_CFG                  											0x5E
#define MD2_CFG                  											0x5F

// Control Registers
#define CTRL1_XL                  												0x10
#define CTRL2_G                   												0x11
#define CTRL3_C                   												0x12
#define CTRL4_C                   												0x13
#define CTRL5_C            	       											0x14
#define CTRL6_C                	   											0x15
#define CTRL7_G                   												0x16
#define CTRL8_XL                  												0x17
#define CTRL9_XL                  												0x18
#define CTRL10_C                  											0x19

// Status Register
#define STATUS_REG											                0x1E
#define FUNC_SRC1_REG 									                0x53
#define FUNC_SRC2_REG													0x54
#define WRIST_TILT_IA														0x55

// Temperature Output Registers
#define OUT_TEMP_L                										0x20
#define OUT_TEMP_H               											0x21

// Gyroscope Data Registers
#define OUTX_L_G                  											0x22
#define OUTX_H_G                 											0x23
#define OUTY_L_G                  											0x24
#define OUTY_H_G                  											0x25
#define OUTZ_L_G                  											0x26
#define OUTZ_H_G                  											0x27

// Accelerometer Data Registers
#define OUTX_L_XL                 											0x28
#define OUTX_H_XL                 											0x29
#define OUTY_L_XL                 											0x2A
#define OUTY_H_XL                 											0x2B
#define OUTZ_L_XL                 											0x2C
#define OUTZ_H_XL                 											0x2D

// Timestamp Registers
#define TIMESTAMP0_REG           										0x40
#define TIMESTAMP1_REG           										0x41
#define TIMESTAMP2_REG           										0x42

// Tap Detection Registers
#define TAP_CFG                  												0x58
#define TAP_THS_6D               											0x59
#define INT_DUR2                 												0x5A
#define TAP_SRC_REG									                    0x1C

// Function Configuration Access Register
#define FUNC_CFG_ACCESS          										0x01
#define EMB_FUNC_BANK_A          									0x80  // Bank A access
#define EMB_FUNC_BANK_B          									0x40  // Bank B access

// Wrist Tilt Detection Registers (Bank B)
#define A_WRIST_TILT_LAT         										0x50  // Bank B
#define A_WRIST_TILT_THS         										0x54  // Bank B
#define A_WRIST_TILT_MASK        									0x59  // Bank B

//Wake Up
#define WAKE_UP_THS_REG            									0x5B
#define WAKE_UP_DUR_REG             								0x5C

/********************
 * Important Register Bits
 ********************/

#define DEV_ID																	0x6Ah

#define FUNC_CFG_EN_POS												0x5
#define FUNC_CFG_EN_MSK												(0x1 << FUNC_CFG_EN_POS)
#define FUNC_CFG_EN_B_POS											0x7
#define FUNC_CFG_EN_B_MKS											(0x1 << FUNC_CFG_EN_B_POS)

#define DI_BANK_A_BANK_B												0x00
#define EN_BANK_A															FUNC_CFG_EN_MSK
#define EN_BANK_B															(FUNC_CFG_EN_B_MKS | FUNC_CFG_EN_MSK)

#define DRDY_PULSED_POS												(0x1 << 0x7)
#define INT2_WRIST_TILT_POS											(0x1 << 0x0)

#define INT1_STEP_DETECTOR											(0x1 << 7)
#define INT1_SIGN_MOT													(0x1 << 6)
#define INT1_FULL_FLAG													(0x1 << 5)
#define INT1_ FIFO_OVR													(0x1 << 4)
#define INT1_FTH																(0x1 << 3)
#define INT1_BOOT															(0x1 << 2)
#define INT1_DRDY_G														(0x1 << 1)
#define INT1_DRDY_XL														(0x1 << 0x0)

#define INT2_STEP_DELTA                  								(0x1 << 7)
#define INT2_STEP_COUNT_OV              							(0x1 << 6)
#define INT2_FULL_FLAG								                    (0x1 << 5)
#define INT2_FIFO_OVR                   									(0x1 << 4)
#define INT2_FTH                        										(0x1 << 3)
#define INT2_DRDY_TEMP                  								(0x1 << 2)
#define INT2_DRDY_G                     									(0x1 << 1)
#define INT2_DRDY_XL                   									(0x1 << 0)

// ODR_XL[3:0] – Output Data Rate
#define ODR_XL_POWER_DOWN		    				            (0x0 << 4)
#define ODR_XL_12_5HZ                 									(0x1 << 4)
#define ODR_XL_26HZ                   									(0x2 << 4)
#define ODR_XL_52HZ                   									(0x3 << 4)
#define ODR_XL_104HZ                  									(0x4 << 4)
#define ODR_XL_208HZ                  									(0x5 << 4)
#define ODR_XL_416HZ                  									(0x6 << 4)
#define ODR_XL_833HZ                  									(0x7 << 4)
#define ODR_XL_1_66KHZ                									(0x8 << 4)
#define ODR_XL_3_33KHZ                									(0x9 << 4)
#define ODR_XL_6_66KHZ                									(0xA << 4)

// FS_XL[1:0] – Full Scale
#define FS_XL_2G                      											(0x0 << 2)
#define FS_XL_16G                     										(0x1 << 2)
#define FS_XL_4G                      											(0x2 << 2)
#define FS_XL_8G                     											(0x3 << 2)

// LPF1_BW_SEL
#define LPF1_BW_SEL_MASK              								(0x1 << 1)

// BW0_XL
#define BW0_XL_1_5KHZ                 									(0x0)
#define BW0_XL_400HZ                  									(0x1)

// ODR_G[3:0] – Output Data Rate
#define ODR_G_POWER_DOWN           							(0x0 << 4)
#define ODR_G_12_5HZ                									(0x1 << 4)
#define ODR_G_26HZ                    									(0x2 << 4)
#define ODR_G_52HZ                    									(0x3 << 4)
#define ODR_G_104HZ                   									(0x4 << 4)
#define ODR_G_208HZ                   									(0x5 << 4)
#define ODR_G_416HZ                   									(0x6 << 4)
#define ODR_G_833HZ                   									(0x7 << 4)
#define ODR_G_1_66KHZ                 									(0x8 << 4)
#define ODR_G_3_33KHZ                 									(0x9 << 4)
#define ODR_G_6_66KHZ                 									(0xA << 4)

// FS_G[1:0] – Full Scale
#define FS_G_250DPS                   										(0x0 << 2)
#define FS_G_500DPS                   										(0x1 << 2)
#define FS_G_1000DPS                  									(0x2 << 2)
#define FS_G_2000DPS                  									(0x3 << 2)

// FS_125 – Set bit 0 to enable 125 dps full scale
#define FS_G_125DPS                   										(0x1 << 1)

// CTRL3_C (0x12) – Common Control
#define SW_RESET                        										(0x1 << 0)
#define BLE                             											(0x1 << 1)
#define IF_INC                          											(0x1 << 2)
#define SIM                             											(0x1 << 3)
#define PP_OD                           										(0x1 << 4)
#define H_LACTIVE                       										(0x1 << 5)
#define BDU                             											(0x1 << 6)
#define BOOT                           											(0x1 << 7)

// CTRL4_C (0x13) – Control Register 4
#define LPF1_SEL_G                    										(0x1 << 0)
#define I2C_DISABLE                     										(0x1 << 1)
#define DRDY_MASK                       									(0x1 << 2)
#define INT2_ON_INT1                    									(0x1 << 3)
#define DEN_DRDY_INT1                   								(0x1 << 4)
#define SLEEP_G                        											(0x1 << 5)
#define DEN_XL_EN                       									(0x1 << 6)

// CTRL5_C (0x14) – Control Register 5
// Rounding modes
#define ROUNDING_NONE                   								(0x0 << 5)
#define ROUNDING_ACC_ONLY              							(0x1 << 5)
#define ROUNDING_GYRO_ONLY             							(0x2 << 5)
#define ROUNDING_ACC_GYRO              							(0x3 << 5)
#define ROUNDING_SH1_6                 								(0x4 << 5)
#define ROUNDING_ACC_SH1_6             							(0x5 << 5)
#define ROUNDING_ALL_SH1_12            							(0x6 << 5)
#define ROUNDING_ACC_GYRO_SH1_6        					(0x7 << 5)

// DEN active level
#define DEN_ACTIVE_HIGH                								(0x1 << 4)
#define DEN_ACTIVE_LOW                								(0x0 << 4)

// Gyroscope self-test
#define ST_G_NORMAL                   									(0x0 << 2)
#define ST_G_POSITIVE                 										(0x1 << 2)
#define ST_G_NEGATIVE                 									(0x3 << 2)

// Accelerometer self-test
#define ST_XL_NORMAL                  									(0x0)
#define ST_XL_POSITIVE                									(0x1)
#define ST_XL_NEGATIVE                									(0x2)

// CTRL6_C (0x15) – Angular Rate Sensor Control
// FTYPE[1:0] – Gyroscope LPF1 Bandwidth
#define GYRO_LPF1_245HZ                								(0x0 << 0)
#define GYRO_LPF1_195HZ                								(0x1 << 0)
#define GYRO_LPF1_155HZ                								(0x2 << 0)
#define GYRO_LPF1_293HZ_PLUS           							(0x3 << 0)

// USR_OFF_W
#define USR_OFF_WEIGHT_2_10G_LSB       						(0x0 << 2)
#define USR_OFF_WEIGHT_2_6G_LSB        						(0x1 << 2)

// XL_HM_MODE
#define ACC_HIGH_PERF_MODE             							(0x0 << 3)
#define ACC_LOW_POWER_MODE            						(0x1 << 3)

// DEN Trigger modes
#define DEN_TRIG_LVL2_EN               								(0x1 << 4)
#define DEN_TRIG_LVL_EN                								(0x1 << 5)
#define DEN_TRIG_EDGE_EN               								(0x1 << 6)

// CTRL7_G (0x16) – Gyroscope Control
// G_HM_MODE – Gyro High-Performance Mode
#define GYRO_HIGH_PERF_MODE           							(0x0 << 7)
#define GYRO_LOW_POWER_MODE           						(0x1 << 7)

// HP_EN_G – High-Pass Filter Enable
#define GYRO_HPF_DISABLE              								(0x0 << 6)
#define GYRO_HPF_ENABLE               								(0x1 << 6)

// HPM_G[1:0] – HP Filter Cutoff
#define GYRO_HPF_16mHz                								(0x0 << 4)
#define GYRO_HPF_65mHz                								(0x1 << 4)
#define GYRO_HPF_260mHz               								(0x2 << 4)
#define GYRO_HPF_1_04Hz               								(0x3 << 4)

// ROUNDING_STATUS – Rounding for SRC/STATUS regs
#define ROUNDING_STATUS_ENABLE        						(0x1 << 2)
#define ROUNDING_STATUS_DISABLE       						(0x0 << 2)

// CTRL8_XL (0x17) – Accelerometer Filter Control
// LPF2_XL_EN – Low-pass filter 2 enable
#define ACC_LPF2_ENABLE               									(0x1 << 7)
#define ACC_LPF2_DISABLE              									(0x0 << 7)

// HPCF_XL[1:0] – Filter cutoff configuration
#define ACC_HPCF_ODR_50               								(0x0 << 5)
#define ACC_HPCF_ODR_100              								(0x1 << 5)
#define ACC_HPCF_ODR_9                								(0x2 << 5)
#define ACC_HPCF_ODR_400              								(0x3 << 5)

// INPUT_COMPOSITE – Composite input filter source
#define ACC_INPUT_ODR_DIV_2             							(0x0 << 3)
#define ACC_INPUT_ODR_DIV_4             							(0x1 << 3)

// HP_SLOPE_XL_EN – High-pass or slope filter enable
#define ACC_SLOPE_FILTER_ENABLE         						(0x1 << 2)
#define ACC_SLOPE_FILTER_DISABLE        						(0x0 << 2)

// LOW_PASS_ON_6D – LPF2 enable on 6D function
#define ACC_LPF2_ON_6D_ENABLE           						(0x1 << 0)
#define ACC_LPF2_ON_6D_DISABLE          						(0x0 << 0)

// CTRL9_XL (0x18) – DEN and Soft-Iron Config
// DEN axis LSB stamping
#define DEN_STAMP_X                    									(0x1 << 7)
#define DEN_STAMP_Y                    									(0x1 << 6)
#define DEN_STAMP_Z                    									(0x1 << 5)

// DEN_XL_G – Stamp DEN to accelerometer (1) or gyro (0)
#define DEN_STAMP_ACCEL                								(0x1 << 4)
#define DEN_STAMP_GYRO                 								(0x0 << 4)

// SOFT_EN – Soft-iron correction (magnetometer)
#define SOFT_IRON_CORRECTION_EN        						(0x1 << 2)
#define SOFT_IRON_CORRECTION_DIS       						(0x0 << 2)

// CTRL10_C (0x19) – Embedded Functions Control
#define WRIST_TILT_EN                    									(0x1 << 7)
#define TIMER_EN                         										(0x1 << 5)
#define PEDO_EN                          										(0x1 << 4)
#define TILT_EN                          										(0x1 << 3)
#define FUNC_EN                          										(0x1 << 2)
#define PEDO_RST_STEP                    								(0x1 << 1)
#define SIGN_MOTION_EN                   								(0x1 << 0)

// TAP_SRC (0x1C) – Tap Status Register
#define TAP_IA                         											(0x1 << 6)  // General tap event occurred
#define SINGLE_TAP                     										(0x1 << 5)  // Single tap detected
#define DOUBLE_TAP                     									(0x1 << 4)  // Double tap detected
#define TAP_SIGN                       										(0x1 << 3)  // 0 = positive, 1 = negative
#define X_TAP                          											(0x1 << 2)  // Tap on X-axis
#define Y_TAP                          											(0x1 << 1)  // Tap on Y-axis
#define Z_TAP                          											(0x1 << 0)  // Tap on Z-axis

// STATUS_REG (0x1E) – Data Status Flags
#define TDA                             											(0x1 << 2)  // Temp data available
#define GDA                              											(0x1 << 1)  // Gyro data available
#define XLDA                             											(0x1 << 0)  // Accel data available

//FUNC_SRC1
#define FUNC_STEP_DELTA_IA             								(0x1 << 7)
#define FUNC_SIGN_MOTION_IA          							(0x1 << 6)
#define FUNC_TILT_IA                 										(0x1 << 5)
#define FUNC_STEP_DETECTED           								(0x1 << 4)
#define FUNC_STEP_OVERFLOW           							(0x1 << 3)
#define FUNC_HI_FAIL                 										(0x1 << 2)
#define FUNC_SI_END_OP               									(0x1 << 1)
#define FUNC_SENSORHUB_END_OP        						(0x1 << 0)

//FUNC_SR2
#define FUNC_WRIST_TILT_IA											(0x1 << 0)

//WRIST_TILT_IA
#define WRIST_TILT_IA_XPOS           									(0x1 << 5)
#define WRIST_TILT_IA_XNEG           								(0x1 << 4)
#define WRIST_TILT_IA_YPOS           									(0x1 << 3)
#define WRIST_TILT_IA_YNEG           								(0x1 << 2)
#define WRIST_TILT_IA_ZPOS           									(0x1 << 1)
#define WRIST_TILT_IA_ZNEG           								(0x1 << 0)

//TAP_CFG
#define TAP_CFG_INT_ENABLE           								(0x1 << 7)
#define TAP_CFG_INACT_EN1            								(0x1 << 6)
#define TAP_CFG_INACT_EN0            								(0x1 << 5)
#define TAP_CFG_SLOPE_FDS            								(0x1 << 4)
#define TAP_CFG_TAP_X_EN             									(0x1 << 3)
#define TAP_CFG_TAP_Y_EN             									(0x1 << 2)
#define TAP_CFG_TAP_Z_EN             									(0x1 << 1)
#define TAP_CFG_LIR                  										(0x1 << 0)

//TAP_THS_6D_REG
#define TAP_THS_D4D_EN              									(0x1 << 7)
#define TAP_THS_SIXD_THS1           									(0x1 << 6)
#define TAP_THS_SIXD_THS0           									(0x1 << 5)
#define TAP_THS_THS4                										(0x1 << 4)
#define TAP_THS_THS3                										(0x1 << 3)
#define TAP_THS_THS2                										(0x1 << 2)
#define TAP_THS_THS1                										(0x1 << 1)
#define TAP_THS_THS0                										(0x1 << 0)

//INT_DUR2_REG
#define INT_DUR2_DUR3              										(0x1 << 7)
#define INT_DUR2_DUR2              										(0x1 << 6)
#define INT_DUR2_DUR1              										(0x1 << 5)
#define INT_DUR2_DUR0              										(0x1 << 4)
#define INT_DUR2_QUIET1            									(0x1 << 3)
#define INT_DUR2_QUIET0            									(0x1 << 2)
#define INT_DUR2_SHOCK1            									(0x1 << 1)
#define INT_DUR2_SHOCK0            									(0x1 << 0)

//WAKE_UP_THS
#define WAKE_UP_THS_SINGLE_DOUBLE   						(0x1 << 7)
#define WAKE_UP_THS_WK_THS5            							(0x1 << 5)
#define WAKE_UP_THS_WK_THS4            							(0x1 << 4)
#define WAKE_UP_THS_WK_THS3            							(0x1 << 3)
#define WAKE_UP_THS_WK_THS2            							(0x1 << 2)
#define WAKE_UP_THS_WK_THS1            							(0x1 << 1)
#define WAKE_UP_THS_WK_THS0            							(0x1 << 0)

//WAKE_UP_DUR
#define WAKE_UP_DUR_FF_DUR5            							(0x1 << 7)
#define WAKE_UP_DUR_WAKE_DUR1          						(0x1 << 6)
#define WAKE_UP_DUR_WAKE_DUR0          						(0x1 << 5)
#define WAKE_UP_DUR_TIMER_HR           						(0x1 << 4)
#define WAKE_UP_DUR_SLEEP_DUR3         						(0x1 << 3)
#define WAKE_UP_DUR_SLEEP_DUR2         						(0x1 << 2)
#define WAKE_UP_DUR_SLEEP_DUR1         						(0x1 << 1)
#define WAKE_UP_DUR_SLEEP_DUR0         						(0x1 << 0)

// MD1_CFG (0x5E)
#define INT1_INACT_STATE         										(1 << 7)
#define INT1_SINGLE_TAP           										(1 << 6)
#define INT1_WU                   											(1 << 5)
#define INT1_FF                   												(1 << 4)
#define INT1_DOUBLE_TAP           									(1 << 3)
#define INT1_6D                   												(1 << 2)
#define INT1_TILT                 												(1 << 1)
#define INT1_TIMER                											(1 << 0)

// MD2_CFG (0x5F)
#define INT2_INACT_STATE          										(1 << 7)
#define INT2_SINGLE_TAP           										(1 << 6)
#define INT2_WU                   											(1 << 5)
#define INT2_FF                   												(1 << 4)
#define INT2_DOUBLE_TAP           									(1 << 3)
#define INT2_6D                   												(1 << 2)
#define INT2_TILT                 												(1 << 1)
#define INT2_IRON                 											(1 << 0)

/*****************
 * API's
 *****************/

//Initialization and Setup
void lsm6ds3_init(void);
void lsm6ds3_set_bank(uint8_t bank_flag);
void lsm6ds3_write(uint8_t reg, uint8_t val);
void lsm6ds3_read(uint8_t reg, uint8_t* val, int count);

//Sensor Configuration
void lsm6ds3_enable_accel(uint8_t odr, uint8_t fs);
void lsm6ds3_enable_gyro(uint8_t odr, uint8_t fs);
void lsm6ds3_set_accel_lpf(uint8_t lpf_cfg);
void lsm6ds3_set_gyro_lpf(uint8_t ftype);

//Interrupt Routing
void lsm6ds3_route_interrupt_to_int1(uint8_t mask);
void lsm6ds3_route_interrupt_to_int2(uint8_t mask);

//Sensor Data Reading
void lsm6ds3_read_accel(float* ax_g, float* ay_g, float* az_g);
void lsm6ds3_read_gyro(float* gx_dps, float* gy_dps, float* gz_dps);
void lsm6ds3_read_temp(float* temp);
void lsm6ds3_read_timestamp(uint32_t* timestamp);
double get_accel_sensitivity(void);
double get_gyro_sensitivity(void);

//Tap Detection
void lsm6ds3_enable_tap(void);
void lsm6ds3_read_tap_status(uint8_t* tap_src);

//Wrist Detection
void lsm6ds3_enable_wrist_tilt(void);
void lsm6ds3_read_wrist_tilt(uint8_t* tilt_src);

//Status
void lsm6ds3_read_status(uint8_t* status_reg);
void lsm6ds3_read_func_src(uint8_t* func_src1);

//Control & Reboot
void lsm6ds3_reboot(void);
void lsm6ds3_software_reset(void);

//Additional API's
__weak void imu_init(void);
void IMU_alive(void);
void test_lsm6dsl_whoami(void);
void poll_imu(void);
void test_lis3mdl_whoami(void);
void test_lsm6dsl_whoami(void);

#endif /* INC_LSM6DSL_H_ */
