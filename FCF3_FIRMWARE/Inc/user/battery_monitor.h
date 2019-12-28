#ifndef _BATTERY_MONITOR_H
#define _BATTERY_MONITOR_H
typedef struct battery_monitor_struct *battery_monitor;
typedef uint32_t (*ptr_get_adc_value)();
battery_monitor battery_monitor_ctor(float vref, uint32_t adc_resolution,ptr_get_adc_value get_adc_value);
float battery_monitor_get_voltage(battery_monitor me);
#endif // _BATTERY_MONITOR_H
