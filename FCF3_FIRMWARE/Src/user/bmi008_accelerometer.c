#include "bmi088_accelerometer.h"
#include <stdlib.h>
#include <stdio.h>
static const float RANGE_3G_LSB = 10920;
static const float RANGE_6G_LSB = 5460;
static const float RANGE_12G_LSB = 2730;
static const float RANGE_24G_LSB = 1365;
typedef struct bmi088_accelerometer_struct {
	ptr_bmi088_aceelerometer_write_byte write_byte;
	ptr_bmi088_accelerometer_read_byte read_byte;
	uint8_t *buffer;
	float lsb;
	float offset_x;
	float offset_y;
	float offset_z;
} bmi088_accelerometer_t;
static void set_scale_range(bmi088_accelerometer me, accel_scale_t range);
static void set_output_data_rate(bmi088_accelerometer me,
		accel_data_rate_t rate);
static void set_power_mode(bmi088_accelerometer me, accel_power_mode_t mode);
static float calculate_lsb(accel_scale_t range);
static void set_scale_range(bmi088_accelerometer me, accel_scale_t range) {
	uint8_t accel_range = range;
	me->write_byte(BMI088_ACC_RANGE, &accel_range);
}
static void set_output_data_rate(bmi088_accelerometer me,
		accel_data_rate_t rate) {
	uint8_t accel_rate = rate;
	me->write_byte(BMI088_ACC_CONF, &accel_rate);
}
static void set_power_mode(bmi088_accelerometer me, accel_power_mode_t mode) {
	uint8_t accel_active_register_value = 0x04;
	uint8_t accel_suspend_register_value = 0x03;
	uint8_t zero = 0;
	switch (mode) {
	case ACC_ACTIVE:
		me->write_byte(BMI088_ACC_PWR_CTRl, &accel_active_register_value);
		me->write_byte(BMI088_ACC_PWR_CONF, &zero);
		break;
	case ACC_SUSPEND:
		me->write_byte(BMI088_ACC_PWR_CONF, &accel_suspend_register_value);
		me->write_byte(BMI088_ACC_PWR_CTRl, &zero);
		break;
	}

}
static float calculate_lsb(accel_scale_t range) {
	float lsb = 0;
	switch (range) {
	case ACCEL_RANGE_3G:
		lsb = RANGE_3G_LSB;
		break;
	case ACCEL_RANGE_6G:
		lsb = RANGE_6G_LSB;
		break;
	case ACCEL_RANGE_12G:
		lsb = RANGE_12G_LSB;
		break;
	case ACCEL_RANGE_24G:
		lsb = RANGE_24G_LSB;
		break;
	}
	return lsb;
}
bmi088_accelerometer bmi088_accelerometer_create(
		ptr_bmi088_aceelerometer_write_byte write_byte,
		ptr_bmi088_accelerometer_read_byte read_byte, accel_scale_t range,
		accel_data_rate_t rate, accel_power_mode_t mode) {
	bmi088_accelerometer me;
	me = malloc(sizeof(bmi088_accelerometer_t));
	if (!me) {
		return NULL;
	}
	me->buffer = malloc(sizeof(uint8_t) * 6);
	if (!me->buffer) {
			return NULL;
	}
	me->lsb = calculate_lsb(range);
	me->read_byte = read_byte;
	me->write_byte = write_byte;
	set_scale_range(me, range);
	set_output_data_rate(me, rate);
	set_power_mode(me, mode);
	return me;

}
void bmi088_accelerometer_collect_data(bmi088_accelerometer me) {
	for (size_t i = 0; i < 6; i++) {
		me->read_byte(BMI088_ACC_X_LSB + i, me->buffer + i);
	}
}
float bmi088_accelerometer_get_x(bmi088_accelerometer me) {
	int16_t value = me->buffer[1] * 256 + me->buffer[0];
	float x = value / me->lsb;
	x -= me->offset_x;
	return x;
}
float bmi088_accelerometer_get_y(bmi088_accelerometer me) {
	int16_t value = me->buffer[3] * 256 + me->buffer[2];
	float y = value / me->lsb;
	y -= me->offset_y;
	return y;
}
float bmi088_accelerometer_get_z(bmi088_accelerometer me) {
	int16_t value = me->buffer[5] * 256 + me->buffer[4];
	float z = value / me->lsb;
	z -= me->offset_z;
	return z;
}
void bmi088_accelerometer_calibrate(bmi088_accelerometer me) {
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	uint32_t samples = 1000;
	for (size_t i = 0; i < samples; i++) {
		bmi088_accelerometer_collect_data(me);
		sum_x += bmi088_accelerometer_get_x(me);
		sum_y += bmi088_accelerometer_get_y(me);
		sum_z += bmi088_accelerometer_get_z(me);
	}
	me->offset_x = sum_x / samples;
	me->offset_y = sum_y / samples;
	me->offset_z = sum_z / samples;
}
