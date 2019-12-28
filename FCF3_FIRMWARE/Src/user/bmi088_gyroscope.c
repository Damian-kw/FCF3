#include "bmi088_gyroscope.h"
#include <stdio.h>
#include <stdlib.h>
static const float RANGE_125_LSB = 262.144f;
static const float RANGE_250_LSB = 131.072f;
static const float RANGE_500_LSB = 65.536f;
static const float RANGE_1000_LSB = 32.758f;
static const float RANGE_2000_LSB = 16.384f;
typedef struct bmi088_gyroscope_struct {
	ptr_bmi088_gyroscope_write_byte write_byte;
	ptr_bmi088_gyroscope_read_byte read_byte;
	uint8_t *buffer;
	float lsb;
	float offset_x;
	float offset_y;
	float offset_z;
} bmi088_gyroscope_t;
static void set_scale_range(bmi088_gyroscope me, gyro_scale_t range);
static void set_output_data_rate(bmi088_gyroscope me, gyro_data_rate_t rate);
static void set_power_mode(bmi088_gyroscope me, gyro_power_mode_t mode);
static float calculate_lsb(gyro_scale_t range);
static void set_scale_range(bmi088_gyroscope me, gyro_scale_t range) {
	uint8_t gyro_range = range;
	me->write_byte(BMI088_GYRO_RANGE, &gyro_range);
}
static void set_output_data_rate(bmi088_gyroscope me, gyro_data_rate_t rate) {
	uint8_t gyro_data_rate = rate;
	me->write_byte(BMI088_GYRO_BAND_WIDTH, &gyro_data_rate);
}
static void set_power_mode(bmi088_gyroscope me, gyro_power_mode_t mode) {
	uint8_t gyro_power_mode = mode;
	me->write_byte(BMI088_GYRO_LPM_1, &gyro_power_mode);

}
static float calculate_lsb(gyro_scale_t range) {
	float lsb = 0;
	switch (range) {
	case GYRO_RANGE_125:
		lsb = RANGE_125_LSB;
		break;
	case GYRO_RANGE_250:
		lsb = RANGE_250_LSB;
		break;
	case GYRO_RANGE_500:
		lsb = RANGE_500_LSB;
		break;
	case GYRO_RANGE_1000:
		lsb = RANGE_1000_LSB;
		break;
	case GYRO_RANGE_2000:
		lsb = RANGE_2000_LSB;
		break;
	}
	return lsb;
}
bmi088_gyroscope bmi088_gyroscope_create(
		ptr_bmi088_gyroscope_write_byte write_byte,
		ptr_bmi088_gyroscope_read_byte read_byte, gyro_scale_t range,
		gyro_data_rate_t rate, gyro_power_mode_t mode) {
	bmi088_gyroscope me;
	me = malloc(sizeof(bmi088_gyroscope_t));
	if (!me) {
		return NULL;
	}
	me->buffer = malloc(sizeof(uint8_t) * 6);
	if (!me->buffer) {
		return NULL;
	}
	me->lsb = calculate_lsb(range);
	me->write_byte = write_byte;
	me->read_byte = read_byte;
	set_scale_range(me, range);
	set_output_data_rate(me, rate);
	set_power_mode(me, mode);
	return me;
}
void bmi088_gyroscope_write_byte(bmi088_gyroscope me, uint16_t address,
		uint8_t *value) {
	me->write_byte(address, value);
}
void bmi088_gyroscope_read_byte(bmi088_gyroscope me, uint16_t address,
		uint8_t *value) {

	me->read_byte(address, value);
}
void bmi088_gyroscope_collect_data(bmi088_gyroscope me) {
	for (size_t i = 0; i < 6; i++) {
		me->read_byte(BMI088_GYRO_RATE_X_LSB + i, me->buffer + i);
	}
}
float bmi088_gyroscope_get_x(bmi088_gyroscope me) {
	int16_t value = me->buffer[1] * 256 + me->buffer[0];
	float x = value / me->lsb;
	x -= me->offset_x;
	return x;
}
float bmi088_gyroscope_get_y(bmi088_gyroscope me) {
	int16_t value = me->buffer[3] * 256 + me->buffer[2];
	float y = value / me->lsb;
	y -= me->offset_y;
	return y;
}
float bmi088_gyroscope_get_z(bmi088_gyroscope me) {
	int16_t value = me->buffer[5] * 256 + me->buffer[4];
	float z = value / me->lsb;
	z -= me->offset_z;
	return z;
}
void bmi088_gyroscope_calibrate(bmi088_gyroscope me) {
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	uint32_t samples = 1000;
	for (size_t i = 0; i < samples; i++) {
		bmi088_gyroscope_collect_data(me);
		sum_x += bmi088_gyroscope_get_x(me);
		sum_y += bmi088_gyroscope_get_y(me);
		sum_z += bmi088_gyroscope_get_z(me);
	}
	me->offset_x = sum_x / samples;
	me->offset_y = sum_y / samples;
	me->offset_z = sum_z / samples;
}
