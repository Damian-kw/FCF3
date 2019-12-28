#ifndef _BUZZER_H
#define _BUZZER_H
typedef struct buzzer_struct *buzzer;
typedef void (*ptr_buzzer_on)();
typedef void (*ptr_buzzer_off)();
buzzer buzzer_ctor(ptr_buzzer_on buzzer_on,ptr_buzzer_off buzzer_off);
void buzzer_beep( buzzer me, uint32_t duration_ms);
void buzzer_decrement_duration(buzzer me);
#endif // _BUZZER_H
