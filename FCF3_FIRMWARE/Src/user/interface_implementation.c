/*
 * interface_implemnetation.c
 *
 *  Created on: 24 cze 2019
 *      Author: damiank
 */
#include <stdbool.h>
#include "usart.h"
#include "tim.h"
#include "i2c.h"
#include "adc.h"
#include "interface_implementation.h"
#include "flight_controler.h"
static const uint8_t EEPROM_ADDRESS = 0xA0;
static const uint8_t GYROSCOPE_ADDRESS = 0x69 << 1;
static const uint8_t ACCELEROMETER_ADDRESS = 0x19 << 1;
void start_pwm() {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}
void start_adc() {
	HAL_ADC_Start(&hadc1);
}

void uart2_send_byte(uint8_t byte) {
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY) {

	}
	HAL_StatusTypeDef st = HAL_UART_Transmit(&huart2, &byte, sizeof(uint8_t),
	HAL_MAX_DELAY);
	if (st != HAL_OK) {
		Error_Handler();
	}
}
uint8_t uart2_get_byte() {
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY) {

	}
	uint8_t byte = 0;
	HAL_StatusTypeDef st = HAL_UART_Receive(&huart2, &byte, sizeof(uint8_t),
	HAL_MAX_DELAY);
	if (st != HAL_OK) {
		Error_Handler();
	}
	return byte;
}
void esc1_write_pulse(uint32_t pulse) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);
}
void esc2_write_pulse(uint32_t pulse) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);
}
void esc3_write_pulse(uint32_t pulse) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
}
void esc4_write_pulse(uint32_t pulse) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse);
}
void data_eeprom_write_byte(uint32_t address, uint8_t byte) {
	HAL_StatusTypeDef st = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address,
			sizeof(uint8_t), &byte, sizeof(uint8_t), HAL_MAX_DELAY);
	if (st != HAL_OK) {
		Error_Handler();
	}
	HAL_Delay(150);
}

uint8_t data_eeprom_read_byte(uint8_t address) {
	uint8_t byte = 0;
	HAL_StatusTypeDef st;
	st = HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, sizeof(uint8_t),
			&byte, sizeof(uint8_t), HAL_MAX_DELAY);
	if (st != HAL_OK) {
		Error_Handler();
	}
	HAL_Delay(150);
	return byte;
}
uint32_t battery_adc_get_voltage() {
	uint32_t value = HAL_ADC_GetValue(&hadc1);
	return value;
}
void gyroscope_write_byte(uint8_t address, uint8_t *byte) {
	HAL_StatusTypeDef st = HAL_I2C_Mem_Write(&hi2c1, GYROSCOPE_ADDRESS, address,
			1, byte, 1, HAL_MAX_DELAY);
	if (st != HAL_OK) {
		error_handler();
	}
}
void gyroscope_read_byte(uint8_t address, uint8_t *byte) {
	HAL_StatusTypeDef st = HAL_I2C_Mem_Read(&hi2c1, GYROSCOPE_ADDRESS, address,
			1, byte, 1, HAL_MAX_DELAY);
	if (st != HAL_OK) {
		error_handler();
	}
}
void accelerometer_write_byte(uint8_t address, uint8_t *byte) {
	HAL_StatusTypeDef st = HAL_I2C_Mem_Write(&hi2c1, ACCELEROMETER_ADDRESS, address, 1, byte,
			1, HAL_MAX_DELAY);
	if (st != HAL_OK) {
		error_handler();
	}
}
void accelerometer_read_byte(uint8_t address, uint8_t *byte) {
	HAL_StatusTypeDef st = HAL_I2C_Mem_Read(&hi2c1, ACCELEROMETER_ADDRESS, address,
			1, byte, 1, HAL_MAX_DELAY);
	if (st != HAL_OK) {
		error_handler();
	}
}
void system_buzzer_on() {
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

}
void system_buzzer_off() {
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
}
void uart_dma_get_byte() {
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) {

	}
	uint8_t byte = 0;
	HAL_StatusTypeDef st = HAL_UART_Receive_DMA(&huart1, &byte,
			sizeof(uint8_t));
	if (st != HAL_OK) {
		Error_Handler();
	}
	flight_controler_set_new_ibus_receiver_value(byte);
}
void pasue_uart_dma() {
//	HAL_UART_DMAPause(&huart2);
}
uint32_t get_micros() {
	uint32_t micros = HAL_GetTick() * 1000 + 1000 - SysTick->VAL / 72;
	return micros;
}
void error_led_blink() {
	HAL_GPIO_TogglePin(error_led_GPIO_Port, error_led_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(error_led_GPIO_Port, error_led_Pin);
	HAL_Delay(1000);
}
