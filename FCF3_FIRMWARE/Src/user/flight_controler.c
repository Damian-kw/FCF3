/*
 * flight_controler.c
 *
 *  Created on: 9 lip 2019
 *      Author: damiank
 */
#include "uart.h"
#include "esc.h"
#include "imu.h"
#include "battery_monitor.h"
#include "pid_controler.h"
#include "buzzer.h"
#include "complementary_filter.h"
#include "MedianFilter.h"
#include "float_compare.h"
#include "moving_average.h"
#include "flight_controler.h"
#include "ibus_receiver.h"
#include "bmi088_gyroscope.h"
#include "interface_implementation.h"
#include "eeprom.h"
#include "salloc.h"
#include <stdio.h>
#include <stm32f3xx.h>
#define CHANNELS_NUMBER 6
#define MEDIAN_FILTER_ELEMENTS 7
#define CHANNEL_GAP_US 2050
typedef enum STATES_ENUM {
	FLYING = 0,
	GET_MPU_DATA = 1,
	UPDATE_IMU = 2,
	CALCULATE_PIDS = 3,
	WRITE_TO_ESC = 4,
	GET_IBUS_FRAME = 5
} STATES;
typedef enum CHANNELS_ENUM {
	THROTTLE_CHANNEL = 2,
	ROLL_CHANNEL = 0,
	PITCH_CHANNEL = 1,
	YAW_CHANNEL = 3,
	ARM_CHANNEL = 4,
	MODE_CHANNEL = 5,

} CHANNELS;
static const float gyro_x_offset = -0.659;
static const float gyro_y_offset = 0.654;
static const float gyro_z_offset = -0.192;
static const float accel_x_offset = 0.092;
static const float accel_y_offset = 0.014;
static const float accel_z_offset = 0.828;
static const uint16_t FLIGHT_LOOP_FREQUENCY_HZ = 1000;
//flight controler objects
static sMedianFilter_t median_filter;
static sMedianNode_t median_buffer[MEDIAN_FILTER_ELEMENTS];
volatile static ibus_receiver radio_receiver;
static uart debug_uart;
static bmi088_gyroscope gyroscope;
static esc esc1;
static esc esc2;
static esc esc3;
static esc esc4;
static eeprom data_eeprom;
static battery_monitor battery;
static buzzer system_buzzer;
static pid_controler pid_controler_pitch_acro;
static pid_controler pid_controler_roll_acro;
static pid_controler pid_controler_yaw_acro;
static moving_average battery_moving_average;
//internal vars
uint32_t channels[CHANNELS_NUMBER];
volatile uint32_t fc_state;
int32_t pitch_output;
int32_t roll_output;
int32_t yaw_output;
float dt;
static uint32_t receiver_throttle;
static uint32_t receiver_arm;
static uint32_t receiver_mode;
static uint32_t receiver_pitch;
static uint32_t receiver_roll;
static uint32_t receiver_yaw;
static float accel_x;
static float accel_y;
static float accel_z;
static int32_t gyro_x;
static int32_t gyro_y;
static int32_t gyro_z;
static const int16_t ROLL_MIN_DPS = -180;
static const int16_t ROLL_MAX_DPS = 180;
static const int16_t PITCH_MIN_DPS = -180;
static const int16_t PITCH_MAX_DPS = 180;
static const int16_t YAW_MIN_DPS = -180;
static const int16_t YAW_MAX_DPS = 180;
static const int16_t PITCH_MAX_ANGLE = 30;
static const int16_t PITCH_MIN_ANGLE = -30;
static const int16_t ROLL_MAX_ANGLE = 30;
static const int16_t ROLL_MIN_ANGLE = -30;
static const int16_t ESC_MIN_ADJUST = -20;
static const int16_t ESC_MAX_ADJUST = 20;
static const float minimum_battery_voltage = 11.5;
static float accelerometer_pitch_angle;
static float accelerometer_roll_angle;
static bool is_new_flight_loop_time();
static bool is_drone_disarmed();
static bool is_new_ibus_frame();
int32_t map_int32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max);
static void flight_controler_set_state(uint32_t state);
static uint32_t flight_controler_get_state();
static void flight_controler_get_mpu_data();
static void flight_controler_get_ibus_frame();
static void flight_controler_write_to_esc();
static void flight_controler_update_imu();
static void flight_controler_calculate_pids();
static void flight_controler_flying();
void flight_controler_execute_get_mpu_data() {
	flight_controler_get_mpu_data();
}
void flight_controler_execute_arm() {
	flight_controler_flying();
}
void flight_controler_write_to_uart(char *msg) {
	uart_write_string(debug_uart, msg);
}
static bool is_new_flight_loop_time() {
	static uint32_t counter_us = 0;
	counter_us += get_micros();
	if (counter_us >= (1000000 / FLIGHT_LOOP_FREQUENCY_HZ)) {
		counter_us = 0;
		return true;
	}
	return false;

}
static bool is_drone_disarmed() {
	if (receiver_arm < 1200) {
		return true;
	}
	return false;
}
static bool is_new_ibus_frame() {
	return ibus_receiver_is_new_frame_avaible(radio_receiver);
}
int32_t map_int32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static void flight_controler_set_state(uint32_t state) {
	fc_state = state;
}
static uint32_t flight_controler_get_state() {
	return fc_state;
}
uint32_t flight_controler_get_micros() {
	return get_micros();
}
void flight_controler_set_new_ibus_receiver_value(uint8_t value) {
	ibus_receiver_set_new_received_value(radio_receiver, value);
}
void flight_controler_init() {
	median_filter.numNodes = MEDIAN_FILTER_ELEMENTS;
	median_filter.medianBuffer = median_buffer;
	int init_result = MEDIANFILTER_Init(&median_filter);
	if (init_result == -1) {
		error_handler();
	}
	salloc_enable();
	gyroscope = bmi088_gyroscope_create(gyroscope_write_byte,
			gyroscope_read_byte,GYRO_RANGE_1000,GYRO_ODR_1000_BW_116,GYRO_NORMAL);
	if (!gyroscope) {
		error_handler();
	}
	debug_uart = uart_ctor(100, uart2_send_byte, uart2_get_byte);
	if (!debug_uart) {
		error_handler();
	}
	data_eeprom = eeprom_ctor(data_eeprom_write_byte, data_eeprom_read_byte);
	if (!data_eeprom) {
		error_handler();
	}
	esc1 = esc_ctor(125, 250, esc1_write_pulse);
	if (!esc1) {
		error_handler();
	}
	esc2 = esc_ctor(125, 250, esc2_write_pulse);
	if (!esc2) {
		error_handler();
	}
	esc3 = esc_ctor(125, 250, esc3_write_pulse);
	if (!esc3) {
		error_handler();
	}
	esc4 = esc_ctor(125, 250, esc4_write_pulse);
	if (!esc4) {
		error_handler();
	}
	battery = battery_monitor_ctor(3, 4096, battery_adc_get_voltage);
	if (!battery) {
		error_handler();
	}
	system_buzzer = buzzer_ctor(system_buzzer_on, system_buzzer_off);
	if (!system_buzzer) {
		error_handler();
	}
	pid_controler_pitch_acro = pid_controler_ctor(0, 0, 0);
	if (!pid_controler_pitch_acro) {
		error_handler();
	}
	pid_controler_roll_acro = pid_controler_ctor(0, 0, 0);
	if (!pid_controler_roll_acro) {
		error_handler();
	}
	pid_controler_yaw_acro = pid_controler_ctor(0, 0, 0);
	if (!pid_controler_yaw_acro) {
		error_handler();
	}
	battery_moving_average = moving_average_ctor(17);
	if (!battery_moving_average) {
		error_handler();
	}

	radio_receiver = ibus_receiver_create(6);
	if (!radio_receiver) {
		error_handler();
	}
	start_pwm();
	start_adc();
	flight_controler_set_state(FLYING);
	HAL_Delay(2000);
	uart_dma_get_byte();
}
void flight_controler_run() {
	switch (flight_controler_get_state()) {
	case FLYING:
		flight_controler_flying();
		break;
	case GET_IBUS_FRAME:
		flight_controler_get_ibus_frame();
		;
		break;
	case GET_MPU_DATA:
		flight_controler_get_mpu_data();
		break;
	case UPDATE_IMU:
		flight_controler_update_imu();
		break;
	case CALCULATE_PIDS:
		flight_controler_calculate_pids();
		break;
	case WRITE_TO_ESC:
		flight_controler_write_to_esc();
		break;
	}
	char *states_str[] = { "FLYING", "GET_MPU_DATA", "UPDATE_IMU",
			"CALCULATE_PIDS", "WRITE_TO_ESC", "GET_CPPM_FRAME",
			"COLLECT_CPPM_DATA" };
//	flight_controler_write_to_uart(states_str[flight_controler_get_state()]);
//	flight_controler_write_to_uart("\n");
//	HAL_Delay(5);

}

static void flight_controler_get_mpu_data() {

	flight_controler_set_state(UPDATE_IMU);
}
static void flight_controler_get_ibus_frame() {
	receiver_throttle = ibus_receiver_get_channel_value(radio_receiver,
			THROTTLE_CHANNEL);
	receiver_throttle = MEDIANFILTER_Insert(&median_filter, receiver_throttle);
	receiver_pitch = ibus_receiver_get_channel_value(radio_receiver,
			PITCH_CHANNEL);
	receiver_roll = ibus_receiver_get_channel_value(radio_receiver,
			ROLL_CHANNEL);
	receiver_yaw = ibus_receiver_get_channel_value(radio_receiver, YAW_CHANNEL);
	receiver_yaw = map_int32(receiver_yaw, 1000, 2000, 2000, 1000);
	receiver_arm = ibus_receiver_get_channel_value(radio_receiver, ARM_CHANNEL);
	receiver_mode = ibus_receiver_get_channel_value(radio_receiver,
			MODE_CHANNEL);
	flight_controler_set_state(FLYING);

}
static void flight_controler_write_to_esc() {
	uint32_t roll_adjust = map_int32(roll_output, ROLL_MIN_DPS, ROLL_MAX_DPS,
			ESC_MIN_ADJUST, ESC_MAX_ADJUST);
	uint32_t pitch_adjust = map_int32(pitch_output, PITCH_MIN_DPS,
			PITCH_MAX_DPS, ESC_MIN_ADJUST, ESC_MAX_ADJUST);
	uint32_t yaw_adjust = map_int32(yaw_output, YAW_MIN_DPS, YAW_MAX_DPS,
			ESC_MIN_ADJUST, ESC_MAX_ADJUST);
	uint32_t esc1_value = receiver_throttle;
	uint32_t esc2_value = receiver_throttle;
	uint32_t esc3_value = receiver_throttle;
	uint32_t esc4_value = receiver_throttle;
	esc_write_pulse(esc1, esc1_value);
	esc_write_pulse(esc2, esc2_value);
	esc_write_pulse(esc3, esc3_value);
	esc_write_pulse(esc4, esc4_value);
	flight_controler_set_state(FLYING);
}
static void flight_controler_update_imu() {
	accelerometer_pitch_angle = imu_get_pitch_angle(accel_x, accel_y, accel_z);
	accelerometer_roll_angle = imu_get_roll_angle(accel_x, accel_y, accel_z);
	flight_controler_set_state(CALCULATE_PIDS);
}
static void flight_controler_calculate_pids() {
//map_int32 to degrees per sec
	int32_t receiver_pitch_dps = map_int32(receiver_pitch, 1000, 2000,
			PITCH_MIN_DPS, PITCH_MAX_DPS);
	int32_t receiver_roll_dps = map_int32(receiver_roll, 1000, 2000,
			ROLL_MIN_DPS, ROLL_MAX_DPS);
	int32_t receiver_yaw_dps = map_int32(receiver_yaw, 1000, 2000, YAW_MIN_DPS,
			YAW_MAX_DPS) * (-1);
//calculate output
	pitch_output = pid_controler_calculate_output(pid_controler_pitch_acro,
			gyro_y, receiver_pitch_dps, dt);
	roll_output = pid_controler_calculate_output(pid_controler_roll_acro,
			gyro_x, receiver_roll_dps, dt);
	yaw_output = pid_controler_calculate_output(pid_controler_yaw_acro, gyro_z,
			receiver_yaw_dps, dt);
	flight_controler_set_state(WRITE_TO_ESC);
}
static void flight_controler_flying() {
	float battery_voltage = battery_monitor_get_voltage(battery) * 4.2f + 1.0f;
	moving_average_insert(battery_moving_average, battery_voltage);
	float filtered_voltage = moving_average_calculate(battery_moving_average);
	bool is_greater = is_float_greater(minimum_battery_voltage - 3.0,
			minimum_battery_voltage, 2);
	if (!is_greater) {
		buzzer_beep(system_buzzer, 500);
	}
	if (is_drone_disarmed()) {
		esc_write_pulse(esc1, 125);
		esc_write_pulse(esc2, 125);
		esc_write_pulse(esc3, 125);
		esc_write_pulse(esc4, 125);
	}
	if (is_new_ibus_frame()) {
		flight_controler_set_state(GET_IBUS_FRAME);
	}
	if (is_new_flight_loop_time()) {
		flight_controler_set_state(GET_MPU_DATA);
		return;
	}
}
void error_handler() {
	while (true) {
		error_led_blink();
	}
}
