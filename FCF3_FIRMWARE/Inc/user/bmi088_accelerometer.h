#ifndef _BMI0088_ACCELEROMETER
#define _BMI0088_ACCELEROMETER
#include <stdint.h>
#define BMI088_ACC_RANGE 0x41
#define BMI088_ACC_CONF 0x40
#define BMI088_ACC_PWR_CTRl 0x7D
#define BMI088_ACC_PWR_CONF 0x7C
#define BMI088_ACC_X_LSB 0x12
typedef enum accelerometer_scale_enum {
	ACCEL_RANGE_3G = 0x00, ACCEL_RANGE_6G = 0x01, ACCEL_RANGE_12G = 0x02, ACCEL_RANGE_24G = 0x03,
} accel_scale_t;
typedef enum accel_data_rate_enum {
	ACCEL_ODR_12 = 0x05,
	ACCEL_ODR_25 = 0x06,
	ACCEL_ODR_50 = 0x07,
	ACCEL_ODR_100 = 0x08,
	ACCEL_ODR_200 = 0x09,
	ACCEL_ODR_400 = 0x0A,
	ACCEL_ODR_800 = 0x0B,
	ACCEL_ODR_1600 = 0x0C,
} accel_data_rate_t;
typedef enum accel_power_mode_enum {
	ACC_ACTIVE = 0x00, ACC_SUSPEND = 0x03,
} accel_power_mode_t;
typedef struct bmi088_accelerometer_struct *bmi088_accelerometer;
typedef void (*ptr_bmi088_accelerometer_read_byte)(uint8_t address,
		uint8_t *buffer);
typedef void (*ptr_bmi088_aceelerometer_write_byte)(uint8_t address,
		uint8_t *buffer);
bmi088_accelerometer bmi088_accelerometer_create(
		ptr_bmi088_aceelerometer_write_byte write_byte,
		ptr_bmi088_accelerometer_read_byte read_byte, accel_scale_t range,
		accel_data_rate_t rate, accel_power_mode_t mode);
void bmi088_accelerometer_collect_data();
float bmi088_accelerometer_get_x(bmi088_accelerometer me);
float bmi088_accelerometer_get_y(bmi088_accelerometer me);
float bmi088_accelerometer_get_z(bmi088_accelerometer me);
void bmi088_accelerometer_calibrate(bmi088_accelerometer me);
#endif
