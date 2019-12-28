#include <stdint.h>
#include <stdlib.h>
#include "buzzer.h"
typedef struct buzzer_struct
{
    uint32_t duration_ms;
    ptr_buzzer_on buzzer_on;
    ptr_buzzer_off buzzer_off;
} buzzer_t;
static void buzzer_on(buzzer me);
static void buzzer_off(buzzer me);
buzzer buzzer_ctor(ptr_buzzer_on buzzer_on,ptr_buzzer_off buzzer_off)
{
    buzzer me;
    me = malloc(sizeof(buzzer_t));
    if (!me)
    {
        return NULL;
    }
    me->buzzer_on = buzzer_on;
    me->buzzer_off = buzzer_off;
    buzzer_off(me);
    return me;
}
void buzzer_beep(buzzer me, uint32_t duration_ms)
{
    me->duration_ms = duration_ms;
    buzzer_on(me);
}
void buzzer_decrement_duration(buzzer me)
{
    me->duration_ms--;
    if (me->duration_ms <= 0)
    {
        me->duration_ms = 0;
        buzzer_off(me);
    }
}
static void buzzer_on(buzzer me)
{
    me->buzzer_on();
}
static void buzzer_off(buzzer me)
{
    me->buzzer_off();
}
