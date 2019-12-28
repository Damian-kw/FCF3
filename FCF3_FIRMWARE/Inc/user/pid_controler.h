#ifndef _PID_CONTROLER_H
#define _PID_CONTROLER_H
typedef struct pid_controler_struct* pid_controler;
pid_controler pid_controler_ctor(float kp, float ki, float kd);
float pid_controler_get_kp(pid_controler me);
float pid_controler_get_ki(pid_controler me);
float pid_controler_get_kd(pid_controler me);
void pid_controler_set_kp(pid_controler me, float kp);
void pid_controler_set_ki(pid_controler me, float ki);
void pid_controler_set_kd(pid_controler me, float kd);
void pid_controler_set_out_min(pid_controler me, float out_min);
void pid_controler_set_out_max(pid_controler me, float out_max);
float pid_controler_calculate_output(pid_controler me, float input, float setpoint,float d_time);
#endif // _PID_CONTROLER_H
