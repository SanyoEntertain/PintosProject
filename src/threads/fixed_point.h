#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#define F (1 << 14)


int n_to_fp(int n);
int x_to_int(int x);
int x_to_int_round(int x);
int add_xy(int x, int y);
int sub_xy(int x, int y);
int add_xn(int x, int n);
int sub_xn(int x, int n);
int mul_xy(int x, int y);
int mul_xn(int x, int n);
int div_xy(int x, int y);
int div_xn(int x, int n);

#endif