#ifndef _BMI0088_GYROSCOPE
#define _BMI0088_GYROSCOPE
#include <stdint.h>
#define BMI088_GYRO_RANGE 0x0F
#define BMI088_GYRO_BAND_WIDTH 0x10
#define BMI088_GYRO_LPM_1 0x11
#define BMI088_GYRO_RATE_X_LSB 0x02
typedef enum gyro_scale_enum {
	GYRO_RANGE_2000 = 0x00,
	GYRO_RANGE_1000 = 0x01,
	GYRO_RANGE_500 = 0x02,
	GYRO_RANGE_250 = 0x03,
	GYRO_RANGE_125 = 0x04,
} gyro_scale_t;
typedef enum gyro_data_rate_enum {
	GYRO_ODR_2000_BW_532 = 0x00,
	GYRO_ODR_2000_BW_230 = 0x01,
	GYRO_ODR_1000_BW_116 = 0x02,
	GYRO_ODR_400_BW_47 = 0x03,
	GYRO_ODR_200_BW_23 = 0x04,
	GYRO_ODR_100_BW_12 = 0x05,
	GYRO_ODR_200_BW_64 = 0x06,
	GYRO_ODR_100_BW_32 = 0x07,
} gyro_data_rate_t;
typedef enum gyro_power_mode_enum {
	GYRO_NORMAL = 0x00, GYRO_SUSPEND = 0x80, GYRO_DEEP_SUSPEND = 0x20,
} gyro_power_mode_t;

typedef struct bmi088_gyroscope_struct *bmi088_gyroscope;
typedef void (*ptr_bmi088_gyroscope_write_byte)(uint8_t address,
		uint8_t *buffer);
typedef void (*ptr_bmi088_gyroscope_read_byte)(uint8_t address, uint8_t *buffer);
bmi088_gyroscope bmi088_gyroscope_create(
		ptr_bmi088_gyroscope_write_byte write_byte,
		ptr_bmi088_gyroscope_read_byte read_byte,gyro_scale_t range,gyro_data_rate_t rate, gyro_power_mode_t mode);
void bmi088_gyroscope_write_byte(bmi088_gyroscope me, uint16_t address,
		uint8_t *value);
void bmi088_gyroscope_read_byte(bmi088_gyroscope me, uint16_t address,
		uint8_t *value);
void bmi088_gyroscope_collect_data(bmi088_gyroscope me);
float bmi088_gyroscope_get_x(bmi088_gyroscope me);
float bmi088_gyroscope_get_y(bmi088_gyroscope me);
float bmi088_gyroscope_get_z(bmi088_gyroscope me);
void bmi088_gyroscope_calibrate(bmi088_gyroscope me);
#endif
