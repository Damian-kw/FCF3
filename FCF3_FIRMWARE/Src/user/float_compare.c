#include "float_compare.h"
#include <math.h>
static uint32_t scale_float_to_int(float value, uint32_t scaling_precision);
bool is_float_greater(float tested_value, float reference_value, uint32_t precision)
{
    bool is_greater = scale_float_to_int(tested_value,precision) > scale_float_to_int(reference_value,precision);
    return is_greater;
}
static uint32_t scale_float_to_int(float value, uint32_t scaling_precision)
{
    return(uint32_t) value * powf(10, scaling_precision);
}