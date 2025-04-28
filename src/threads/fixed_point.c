#include "fixed_point.h"
#include <stdint.h>


int n_to_fp(int n)
{
  return n * F;
}

int x_to_int(int x)
{
  return x / F;
}

int x_to_int_round(int x)
{
  if(x >= 0)
      return (x + F / 2) / F;
  else
      return (x - F / 2) / F;   

}
int add_xy(int x, int y)
{
  return x + y;

}
int sub_xy(int x, int y)
{
  return x - y;

}
int add_xn(int x, int n)
{
  return x + n * F;

}
int sub_xn(int x, int n)
{
  return x - n * F;
}

int mul_xy(int x, int y)
{
  return ((int64_t)x) * y / F;
}
int mul_xn(int x, int n)
{
  return x * n;
}

int div_xy(int x, int y)
{
  return ((int64_t)x) * F / y;
}
int div_xn(int x, int n)
{
  return x / n;
}

