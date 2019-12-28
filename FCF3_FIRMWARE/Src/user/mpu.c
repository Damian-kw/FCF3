#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "mpu.h"
#define SHIFTED_DATA_BUFFER_SIZE 6
#define RAW_DATA_BUFFER_SIZE 14
#define MPU_ACCELEROMETER_MEASUREMENT_REGISTERS_START 0x3B
#define MPU_BITS 16
#define ACCELEROMETER_X_AXIS_INDEX 0
#define ACCELEROMETER_Y_AXIS_INDEX 1
#define ACCELEROMETER_Z_AXIS_INDEX 2
#define GYROSCOPE_X_AXIS_INDEX 3
#define GYROSCOPE_Y_AXIS_INDEX 4
#define GYROSCOPE_Z_AXIS_INDEX 5
typedef struct mpu_struct
{
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float gyro_x_offset;
	float gyro_y_offset;
	float gyro_z_offset;
	float accel_x_offset;
	float accel_y_offset;
	float accel_z_offset;
	float gyro_lsb;
	float accel_lsb;
	int16_t *shifted_data_buffer;
	uint8_t *raw_data;
	ptr_mpu_write_byte mpu_write_byte;
	ptr_mpu_read_byte mpu_read_byte;
} mpu_t;
static int16_t get_int16_from_u8(uint8_t high_byte, uint8_t low_byte);
static void convert_data(mpu me);
mpu mpu_ctor(ptr_mpu_write_byte mpu_write_byte, ptr_mpu_read_byte mpu_read_byte)
{
	mpu me;
	me = malloc(sizeof(mpu_t));
	if (!me)
	{
		return NULL;
	}
	me->shifted_data_buffer = malloc(
		sizeof(uint16_t) * SHIFTED_DATA_BUFFER_SIZE);
	if (!me->shifted_data_buffer)
	{
		return NULL;
	}
	me->raw_data = malloc(sizeof(uint8_t) * RAW_DATA_BUFFER_SIZE);
	if (!me->raw_data)
	{
		return NULL;
	}
	me->mpu_write_byte = mpu_write_byte;
	me->mpu_read_byte = mpu_read_byte;
	//disable sleep mode
	mpu_clear_bit(me, MPU_PWR_MGMT_1_REGISTER, 6);
	//enable interrupt
//	mpu_set_bit(me, MPU_INT_ENABLE_REGISTER, 0);
	return me;
}
float mpu_get_accel_x(mpu me)
{
	me->accel_x = (me->shifted_data_buffer[ACCELEROMETER_X_AXIS_INDEX] / me->accel_lsb) - me->accel_x_offset;
	return me->accel_x;
}
float mpu_get_accel_y(mpu me)
{
	me->accel_y = (me->shifted_data_buffer[ACCELEROMETER_Y_AXIS_INDEX] / me->accel_lsb) - me->accel_y_offset;
	return me->accel_y;
}
float mpu_get_accel_z(mpu me)
{
	me->accel_z = (me->shifted_data_buffer[ACCELEROMETER_Z_AXIS_INDEX] / me->accel_lsb) - me->accel_z_offset;
	return me->accel_z;
}
float mpu_get_gyro_x(mpu me)
{
	me->gyro_x = (me->shifted_data_buffer[GYROSCOPE_X_AXIS_INDEX] / me->gyro_lsb) - me->gyro_x_offset;
	return me->gyro_x;
}
float mpu_get_gyro_y(mpu me)
{
	me->gyro_y = (me->shifted_data_buffer[GYROSCOPE_Y_AXIS_INDEX] / me->gyro_lsb) - me->gyro_y_offset;
	return me->gyro_y;
}
float mpu_get_gyro_z(mpu me)
{
	me->gyro_z = (me->shifted_data_buffer[GYROSCOPE_Z_AXIS_INDEX] / me->gyro_lsb) - me->gyro_z_offset;
	return me->gyro_z;
}
void mpu_collect_data(mpu me)
{
	
	for (size_t i = 0; i < 14; i++)
	{
		me->mpu_read_byte(MPU_ACCELEROMETER_MEASUREMENT_REGISTERS_START + i,
						  me->raw_data + i);
	}
	convert_data(me);
}
void mpu_set_bit(mpu me, uint16_t address, uint8_t bit)
{
	uint8_t register_value = 0;
	me->mpu_read_byte(address, &register_value);
	register_value |= (1 << bit);
	me->mpu_write_byte(address, &register_value);
}
void mpu_clear_bit(mpu me, uint16_t address, uint8_t bit)
{
	uint8_t register_value = 0;
	me->mpu_read_byte(address, &register_value);
	register_value &= ~(1 << bit);
	me->mpu_write_byte(address, &register_value);
}

void mpu_write_byte(mpu me, uint16_t address, uint8_t *value)
{
	me->mpu_write_byte(address, value);
}
uint8_t mpu_read_byte(mpu me, uint16_t address)
{
	uint8_t byte = 0;
	me->mpu_read_byte(address, &byte);
	return byte;
}
void mpu_set_gyroscope_full_scale_range(mpu me, uint8_t option)
{
	uint8_t byte = mpu_read_byte(me, MPU_GYRO_CONFIG_REGISTER);
	byte &= ~(1 << 3);
	byte &= ~(1 << 4);
	byte |= (option << 3);
	float base = 500;
	me->gyro_lsb = (powf(2, MPU_BITS) / 2) / ((base) * (option + 1));
	mpu_write_byte(me, MPU_GYRO_CONFIG_REGISTER, &byte);
}
void mpu_set_accelerometer_full_scale_range(mpu me, uint8_t option)
{
	uint8_t byte = mpu_read_byte(me, MPU_ACCEL_CONFIG_REGISTER);
	byte &= ~(1 << 3);
	byte &= ~(1 << 4);
	byte |= (option << 3);
	float base = 4;
	me->accel_lsb = (powf(2, MPU_BITS) / 2) / ((base) * (option + 1));
	mpu_write_byte(me, MPU_ACCEL_CONFIG_REGISTER, &byte);
}
void mpu_set_dlpf(mpu me, uint8_t option)
{
	uint8_t byte = mpu_read_byte(me, MPU_CONFIG_REGISTER);
	byte &= ~(1 << 0);
	byte &= ~(1 << 1);
	byte &= ~(1 << 2);
	byte |= option;
	mpu_write_byte(me, MPU_CONFIG_REGISTER, &byte);
}
void mpu_set_gyroscope_x_offset(mpu me, float offset)
{
	me->gyro_x_offset = offset;
}
void mpu_set_gyroscope_y_offset(mpu me, float offset)
{
	me->gyro_y_offset = offset;
}
void mpu_set_gyroscope_z_offset(mpu me, float offset)
{
	me->gyro_z_offset = offset;
}
void mpu_set_accelerometer_x_offset(mpu me, float offset)
{
	me->accel_x_offset = offset;
}
void mpu_set_accelerometer_y_offset(mpu me, float offset)
{
	me->accel_y_offset = offset;
}
void mpu_set_accelerometer_z_offset(mpu me, float offset)
{
	me->accel_z_offset = offset;
}

static int16_t get_int16_from_u8(uint8_t high_byte, uint8_t low_byte)
{
	return (high_byte << 8) | (low_byte & 0xFF);
}
static void convert_data(mpu me)
{
	//get_accel
	me->shifted_data_buffer[0] = get_int16_from_u8(me->raw_data[0],
												   me->raw_data[1]);
	me->shifted_data_buffer[1] = get_int16_from_u8(me->raw_data[2],
												   me->raw_data[3]);
	me->shifted_data_buffer[2] = get_int16_from_u8(me->raw_data[4],
												   me->raw_data[5]);
	//get gyro
	me->shifted_data_buffer[3] = get_int16_from_u8(me->raw_data[8],
												   me->raw_data[9]);
	me->shifted_data_buffer[4] = get_int16_from_u8(me->raw_data[10],
												   me->raw_data[11]);
	me->shifted_data_buffer[5] = get_int16_from_u8(me->raw_data[12],
												   me->raw_data[13]);
}
