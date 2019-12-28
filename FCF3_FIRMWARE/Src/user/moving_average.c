#include <stdio.h>
#include <stdlib.h>
#include "moving_average.h"
typedef struct moving_average_struct
{
    uint32_t samples;
    float *buffer;
} moving_average_t;
moving_average moving_average_ctor(uint32_t samples)
{
    moving_average me;
    me = malloc(sizeof(moving_average_t));
    if (!me)
    {
        return NULL;
    }
    me->samples = samples;
    me->buffer = malloc(sizeof(float) * samples);
    if (!me->buffer)
    {
        return NULL;
    }
    return me;
}
void moving_average_insert(moving_average me, float value)
{
    for (size_t i = 0; i < me->samples; i++)
    {
        me->buffer[i] = me->buffer[i + 1];
    }
    me->buffer[me->samples - 1] = value;
}
float moving_average_calculate(moving_average me)
{
    float sum = 0;
    for (size_t i = 0; i < me->samples; i++)
    {
        sum += me->buffer[i];
    }
    return sum / me->samples;
}
