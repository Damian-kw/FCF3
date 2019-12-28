#include <stdlib.h>
#include <stdio.h>
#include "esc.h"

typedef struct esc_struct
{
	uint32_t pulse;
	uint32_t min_pulse;
	uint32_t max_pulse;
	ptr_write_to_esc write_to_esc;
} esc_t;
esc esc_ctor(uint32_t min_pulse, uint32_t max_pulse, ptr_write_to_esc write_to_esc)
{
	esc me;
	me = malloc(sizeof(esc_t));
	if (!me)
	{
		return NULL;
	}
	me->min_pulse = min_pulse;
	me->max_pulse = max_pulse;
	me->write_to_esc = write_to_esc;
	return me;
}
void esc_write_pulse(esc me, uint32_t pulse)
{
	if (pulse < me->min_pulse)
	{
		pulse = me->min_pulse;
	}
	else if (pulse > me->max_pulse)
	{
		pulse = me->max_pulse;
	}
	me->pulse = pulse;
	me->write_to_esc(me->pulse);
}
uint32_t esc_get_pulse(esc me)
{
	return me->pulse;
}
void esc_stop(esc me)
{
	me->pulse = me->min_pulse;
	esc_write_pulse(me, me->pulse);
}
