/* { dg-do compile { target i?86*-*-* } } */
/* { dg-options "-O2 -march=pentium4 -mtune=prescott -mfpmath=387" } */
double foo (double x) {
  return x + 1.75;
}
/* { dg-final { scan-assembler "flds" } } */
