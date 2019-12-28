#include <stdint.h>
#include <stdlib.h>
#include "battery_monitor.h"
typedef struct battery_monitor_struct
{
	float vref;
	uint32_t adc_resolution;
	ptr_get_adc_value get_adc_value;
} battery_monitor_t;
battery_monitor battery_monitor_ctor(float vref, uint32_t adc_resolution,ptr_get_adc_value get_adc_value)
{
	battery_monitor me;
	me = malloc(sizeof(battery_monitor_t));
	if (!me)
	{
		return NULL;
	}
	me->vref = vref;
	me->adc_resolution = adc_resolution;
	me->get_adc_value = get_adc_value;
	return me;
}
float battery_monitor_get_voltage(battery_monitor me)
{
	uint32_t adc_value = me->get_adc_value();
	float voltage = (adc_value * me->vref) / me->adc_resolution;
	return voltage;
}
