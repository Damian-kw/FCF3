#ifndef _MOVING_AVERAGE_H
#define _MOVING_AVERAGE_H
#include <stdint.h>
typedef struct moving_average_struct *moving_average;
moving_average moving_average_ctor(uint32_t samples);
void moving_average_insert(moving_average me, float value);
float moving_average_calculate(moving_average me);
#endif // _MOVING_AVERAGE_H
