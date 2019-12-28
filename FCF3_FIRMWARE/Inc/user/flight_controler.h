/*
 * flight_controler.h
 *
 *  Created on: 9 lip 2019
 *      Author: damiank
 */

#ifndef INC_FLIGHT_CONTROLER_H_
#define INC_FLIGHT_CONTROLER_H_
#include <stdbool.h>
#include <stdint.h>
void flight_controler_execute_get_mpu_data();
void flight_controler_execute_arm();
void flight_controler_init();
void flight_controler_run();
void flight_controler_write_to_uart(char *msg);
void flight_controler_write_char_to_uart(char ch);
uint32_t flight_controler_get_micros();
void flight_controler_set_mpu_new_data_ready(bool state);
bool flight_controler_get_mpu_new_data_ready();
void flight_controler_set_new_ibus_receiver_value(uint8_t value);
bool flight_controler_get_cppm_new_timer_value();
void test_esc();
#endif /* INC_FLIGHT_CONTROLER_H_ */
