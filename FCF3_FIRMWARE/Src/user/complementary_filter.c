#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "complementary_filter.h"
typedef struct complementary_filter_struct
{
    float alpha_gain;
    float beta_gain;
} complementary_filter_t;
static bool is_gain_greater_than_one(float gain);
static bool is_gain_negative(float gain);
static float compute_second_gain(float firts_gain);
complementary_filter complementary_filter_ctor()
{
    complementary_filter me;
    me = (complementary_filter) malloc(sizeof(complementary_filter_t));
    if (!me)
    {
       return NULL;
    }
    return me;
}
void complementary_filter_set_alpha_gain(complementary_filter me, float alpha_gain)
{
    if (is_gain_negative(alpha_gain))
    {
        alpha_gain *= -1;
    }
    if (is_gain_greater_than_one(alpha_gain))
    {
        alpha_gain = 1;
    }
    me->alpha_gain = alpha_gain;
    me->beta_gain = compute_second_gain(alpha_gain);
}
void complementary_filter_set_beta_gain(complementary_filter me, float beta_gain)
{
    if (is_gain_negative(beta_gain))
    {
        beta_gain *= -1;
    }
    if (is_gain_greater_than_one(beta_gain))
    {
        beta_gain = 1;
    }
    me->beta_gain = beta_gain;
    me->alpha_gain = compute_second_gain(beta_gain);
}
float complementary_filter_get_alpha_gain(complementary_filter me)
{
    return me->alpha_gain;
}
float complementary_filter_get_beta_gain(complementary_filter me)
{
    return me->beta_gain;
}

static bool is_gain_greater_than_one(float gain)
{
    return (gain * 100) >= 100;
}
static float compute_second_gain(float first_gain)
{
    return 1.0 - first_gain;
}
static bool is_gain_negative(float gain)
{
    return gain < 0;
}
float complementary_filter_calculate_output(complementary_filter me, float alpha_variable, float beta_variable)
{
    return (me->alpha_gain * alpha_variable) + (me->beta_gain * beta_variable);
}
