#ifndef _SALLOC_H
#define _SALLOC_H
#include <stdint.h>
#include <stdbool.h>
#define BUFFER_SIZE_IN_BYTES 2500
void *salloc(uint32_t size);
void salloc_enable();
void salloc_disable();
uint32_t salloc_get_used_memory();
void salloc_clear_buffer();
#endif // _SALLOC_H
