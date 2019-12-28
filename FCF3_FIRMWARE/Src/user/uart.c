#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "conversion.h"
#include "uart.h"
typedef struct uart_struct
{
	uint8_t *buffer_image;
	uint32_t buffer_size;
	ptr_send_byte send_byte;
	 ptr_get_byte get_byte;
} uart_t;
static char new_line[] = "\r\n";
static void
	clear_buffer(uart me);
uart uart_ctor(uint32_t buffer_size, ptr_send_byte send_byte, ptr_get_byte get_byte)
{
	uart me;
	me = malloc(sizeof(uart_t));
	if (!me)
	{
		return NULL;
	}
	me->buffer_image = malloc(sizeof(uint8_t) * buffer_size);
	if (!me->buffer_image)
	{
		return NULL;
	}
	me->buffer_size = buffer_size;
	me->send_byte = send_byte;
	me->get_byte = get_byte;
	return me;
}
void uart_write_int32(uart me, int32_t value)
{
	clear_buffer(me);
	uint32_t integer_lenght = get_int32_lenght(value);
	uint32_t string_lenght = integer_to_ascii(me->buffer_image, value);
	for (size_t i = 0; i < string_lenght; i++)
	{
		me->send_byte(me->buffer_image[i]);
	}
}

void uart_write_float(uart me, float value, uint32_t accuracy)
{
	clear_buffer(me);
	uint32_t float_lenght = get_float_lenght(value, accuracy);
	check_for_buffer_overflow(me, float_lenght);
	uint32_t string_lenght = float_to_ascii(me->buffer_image, value,
											accuracy);
	for (size_t i = 0; i < string_lenght; i++)
	{
		me->send_byte(me->buffer_image[i]);
	}
}
void uart_write_char(uart me, char c)
{
	clear_buffer(me);
	me->buffer_image[0] = c;
	me->send_byte(me->buffer_image[0]);
}
void uart_write_string(uart me, const char *str)
{
	clear_buffer(me);
	uint32_t string_lenght = strlen(str);
	memcpy(me->buffer_image, str, string_lenght);
	for (size_t i = 0; i < string_lenght; i++)
	{
		me->send_byte(me->buffer_image[i]);
	}
}
void uart_write_new_line(uart me)
{
	clear_buffer(me);
	uart_write_string(me, new_line);
}
char uart_read_char(uart me)
{
	clear_buffer(me);
	char c = me->get_byte();
	return c;
}
void uart_read_string(uart me, uint8_t *buffer)
{
	char c = 1;
	uint32_t str_lenght = 0;
	while (c != '\0')
	{
		c = me->get_byte();
		me->buffer_image[str_lenght] = c;
		str_lenght++;
	}
	me->buffer_image[str_lenght] = '\0';
	memcpy(buffer, me->buffer_image, str_lenght);
}
void uart_get_buffer(uart me, uint8_t *destination_buffer, uint32_t size)
{
	memcpy(destination_buffer, me->buffer_image, size);
}
static void clear_buffer(uart me)
{
	memset(me->buffer_image, 0, me->buffer_size);
}
