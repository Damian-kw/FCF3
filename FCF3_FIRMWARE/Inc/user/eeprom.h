#ifndef _EEPROM_H
#define _EEPROM_H
typedef struct eeprom_struct *eeprom;
typedef void (*ptr_write_byte)(uint32_t address, uint8_t byte);
typedef  uint8_t (*ptr_read_byte)(uint8_t address);
eeprom eeprom_ctor(ptr_write_byte write_byte, ptr_read_byte read_byte);
uint8_t eeprom_read_byte(eeprom me, uint32_t address);
int32_t eeprom_read_int32(eeprom me, uint32_t address);
float eeprom_read_float(eeprom me, uint32_t address);
void eeprom_write_byte(eeprom me, uint32_t address, uint8_t byte);
void eeprom_write_int32(eeprom me, uint32_t address, int32_t value);
void eeprom_write_float(eeprom me, uint32_t address, float value);
#endif // _EEPROM_H
