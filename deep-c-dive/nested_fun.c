#include <stdio.h>
double foo (double a, double b)
{
  double square (double z) { return z * z; }

  return square (a) + square (b);
}

int main() {
		printf("%d", foo(2,3));
}
