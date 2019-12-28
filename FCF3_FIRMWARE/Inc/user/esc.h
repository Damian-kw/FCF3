#ifndef _ESC_H
#define _ESC_H
typedef struct esc_struct *esc;
typedef void (*ptr_write_to_esc)(uint32_t pulse);
esc esc_ctor(uint32_t min_pulse, uint32_t max_pulse, ptr_write_to_esc write_to_esc);
void esc_write_pulse(esc me, uint32_t pulse);
uint32_t esc_get_pulse(esc me);
void esc_stop(esc me);
#endif // _ESC_H
