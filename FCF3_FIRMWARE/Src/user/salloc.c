#include <stdio.h>
#include "salloc.h"
static bool salloc_enabled_flag = false;
static uint8_t buffer[BUFFER_SIZE_IN_BYTES];
static uint32_t offset_in_bytes;
static bool is_salloc_enabled();
static bool is_buffer_full(uint32_t size_in_bytes);
void *salloc(uint32_t size_in_bytes) {
	if (!is_salloc_enabled()) {
		return NULL;
	}
	if (is_buffer_full(size_in_bytes)) {
		return NULL;
	}
	void *next_block_of_memory = &buffer[offset_in_bytes];
	offset_in_bytes += size_in_bytes;
	return next_block_of_memory;
}
void salloc_enable() {
	salloc_enabled_flag = true;
}
void salloc_disable() {
	salloc_enabled_flag = false;
}
uint32_t salloc_get_used_memory() {
	return offset_in_bytes;
}
static bool is_salloc_enabled() {
	return salloc_enabled_flag;
}
static bool is_buffer_full(uint32_t size_in_bytes) {
	if ((offset_in_bytes + size_in_bytes) > BUFFER_SIZE_IN_BYTES) {
		return true;
	} else {
		return false;
	}
}
