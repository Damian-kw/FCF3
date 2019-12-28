#ifndef _COMPLEMENTARY_FILTER_H
#define _COMPLEMENTARY_FILTER_H
typedef struct complementary_filter_struct * complementary_filter; 
complementary_filter complementary_filter_ctor();
void complementary_filter_set_alpha_gain(complementary_filter me,float alpha_gain);
void complementary_filter_set_beta_gain(complementary_filter me,float beta_gain);
float complementary_filter_get_alpha_gain(complementary_filter me);
float complementary_filter_get_beta_gain(complementary_filter me);
float complementary_filter_calculate_output(complementary_filter me,float alpha_variable,float beta_variable);
#endif // _COMPLEMENTARY_FILTER_H
