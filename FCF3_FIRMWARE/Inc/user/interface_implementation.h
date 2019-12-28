/*
 * interfaces_implementation.h
 *
 *  Created on: 24 cze 2019
 *      Author: damiank
 */

#ifndef INC_INTERFACE_IMPLEMENTATION_H_
#define INC_INTERFACE_IMPLEMENTATION_H_
#include <stdint.h> 
extern void error_handler();
void start_pwm();
void start_adc();
void uart2_send_byte(uint8_t byte);
uint8_t uart2_get_byte();
void ibus_receiver_wait_for_new_data();
void esc1_write_pulse(uint32_t pulse);
void esc2_write_pulse(uint32_t pulse);
void esc3_write_pulse(uint32_t pulse);
void esc4_write_pulse(uint32_t pulse);
void data_eeprom_write_byte(uint32_t address, uint8_t byte);
uint8_t data_eeprom_read_byte(uint8_t address);
uint32_t battery_adc_get_voltage();
void gyroscope_write_byte(uint8_t address, uint8_t *byte);
void gyroscope_read_byte(uint8_t address, uint8_t *byte);
void accelerometer_write_byte(uint8_t address, uint8_t *byte);
void accelerometer_read_byte(uint8_t address, uint8_t *byte);
void system_buzzer_on();
void system_buzzer_off();
void start_uart_dma();
void pasue_uart_dma();
void uart_dma_get_byte();
uint32_t get_micros();
void error_led_blink();
#endif /* INC_INTERFACE_IMPLEMENTATION_H_ */
