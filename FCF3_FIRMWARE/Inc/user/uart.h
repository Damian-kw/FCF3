#ifndef _UART_H
#define _UART_H
#include <stdint.h>
typedef struct uart_struct *uart;
typedef void (*ptr_send_byte)(uint8_t byte);
typedef  uint8_t (*ptr_get_byte)();
uart uart_ctor(uint32_t buffer_size, ptr_send_byte send_byte, ptr_get_byte get_byte);
void uart_write_int32(uart me, int32_t value);
void uart_write_float(uart me, float value, uint32_t accuracy);
void uart_write_char(uart me, char c);
void uart_write_string(uart me, const char *str);
void uart_write_new_line(uart me);
char uart_read_char(uart me);
void uart_read_string(uart me, uint8_t *buffer);
void uart_get_buffer(uart me, uint8_t *destination_buffer, uint32_t size);
#endif // _UART_H
