#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "eeprom.h"
typedef struct eeprom_struct
{
	ptr_write_byte write_byte;
	ptr_read_byte read_byte;
} eeprom_t;
typedef union eeprom_float_union {
	uint8_t bytes[4];
	float value;
} eeprom_float_t;
typedef union eeprom_int32_union {
	uint8_t bytes[4];
	int32_t value;
} eeprom_int32_t;
static void eeprom_write_4_bytes(eeprom me, uint32_t address, void *value);
static void eeprom_read_4_bytes(eeprom me, uint32_t address, void *buffer);
eeprom eeprom_ctor(ptr_write_byte write_byte, ptr_read_byte read_byte)
{
	eeprom me;
	me = malloc(sizeof(eeprom_t));
	if (!me)
	{
		return NULL;
	}
	me->write_byte = write_byte;
	me->read_byte = read_byte;
	return me;
	
}
void eeprom_write_byte(eeprom me, uint32_t address, uint8_t byte)
{
	me->write_byte(address, byte);
}
uint8_t eeprom_read_byte(eeprom me, uint32_t address)
{
	return me->read_byte(address);
}
void eeprom_write_int32(eeprom me, uint32_t address, int32_t value)
{
	eeprom_int32_t ei;
	ei.value = value;
	for (size_t i = 0; i < 4; i++)
	{
		eeprom_write_byte(me, address + i, ei.bytes[i]);
	}
}
int32_t eeprom_read_int32(eeprom me, uint32_t address)
{
	eeprom_int32_t ei;
	for (size_t i = 0; i < 4; i++)
	{
		ei.bytes[i] = eeprom_read_byte(me, address + i);
	}
	return ei.value;
}
float eeprom_read_float(eeprom me, uint32_t address)
{
	float buffer = 0;
	eeprom_read_4_bytes(me, address, &buffer);
	return buffer;
}
void eeprom_write_float(eeprom me, uint32_t address, float value)
{
	eeprom_write_4_bytes(me, address, &value);
}
static void eeprom_write_4_bytes(eeprom me, uint32_t address, void *value)
{
	uint8_t *ptr = (uint8_t *)value;
	for (size_t i = 0; i < 4; i++)
	{
		eeprom_write_byte(me, address + i, ptr[i]);
	}
}
static void eeprom_read_4_bytes(eeprom me, uint32_t address, void *buffer)
{
	uint8_t *ptr = (uint8_t *)buffer;
	for (size_t i = 0; i < 4; i++)
	{
		ptr[i] = eeprom_read_byte(me, address + i);
	}
}
