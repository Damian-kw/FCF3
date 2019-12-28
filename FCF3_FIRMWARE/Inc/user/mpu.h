#ifndef _MPU_H
#define _MPU_H
#include <stdint.h>
#define MPU_PWR_MGMT_1_REGISTER 0x6B
#define MPU_GYRO_CONFIG_REGISTER 0x1B
#define MPU_ACCEL_CONFIG_REGISTER 0x1C
#define MPU_INT_ENABLE_REGISTER 0x38
#define MPU_CONFIG_REGISTER 0x1A
typedef struct mpu_struct *mpu;
typedef enum GYROSCOPE_FULL_SCALE_RANGES_ENUM
{
    RANGE_250DPS,
    RANGE_500DPS,
    RANGE_1000DPS,
    RANGE_2000DPS
} GYROSCOPE_FULL_SCALE_RANGES_t;
typedef enum ACCELEROMETER_FULL_SCALE_RANGE_ENUM
{
    RANGE_2G,
    RANGE_4G,
    RANGE_8G,
    RANGE_16G,
} ACCELEROMETER_FULL_SCALE_RANGE_t;
typedef enum DLPF_SETTING_ENUM
{
    DLPF_CFG_0,
    DLPF_CFG_1,
    DLPF_CFG_2,
    DLPF_CFG_3,
    DLPF_CFG_4,
    DLPF_CFG_5,
    DLPF_CFG_6
} DLPF_SETTING_t;
typedef void (*ptr_mpu_write_byte)(uint8_t address,uint8_t *buffer);
typedef void (*ptr_mpu_read_byte)(uint8_t address, uint8_t *buffer);
typedef struct mpu_struct *mpu;
mpu mpu_ctor(ptr_mpu_write_byte mpu_write_byte, ptr_mpu_read_byte mpu_read_byte);
void mpu_set_bit(mpu me, uint16_t address, uint8_t bit);
void mpu_clear_bit(mpu me, uint16_t address, uint8_t bit);
//void mpu_write_byte(mpu me, uint16_t address, uint8_t *value);
//uint8_t mpu_read_byte(mpu me, uint16_t address);
void mpu_set_gyroscope_full_scale_range(mpu me, uint8_t option);
void mpu_set_accelerometer_full_scale_range(mpu me, uint8_t option);
void mpu_set_dlpf(mpu me, uint8_t option);
void mpu_set_gyroscope_x_offset(mpu me, float offset);
void mpu_set_gyroscope_y_offset(mpu me, float offset);
void mpu_set_gyroscope_z_offset(mpu me, float offset);
void mpu_set_accelerometer_x_offset(mpu me, float offset);
void mpu_set_accelerometer_y_offset(mpu me, float offset);
void mpu_set_accelerometer_z_offset(mpu me, float offset);
float mpu_get_accel_x(mpu me);
float mpu_get_accel_y(mpu me);
float mpu_get_accel_z(mpu me);
float mpu_get_gyro_x(mpu me);
float mpu_get_gyro_y(mpu me);
float mpu_get_gyro_z(mpu me);
void mpu_collect_data(mpu me);
#endif // _MPU_H
