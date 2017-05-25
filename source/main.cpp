#include <windows.h>
#include <stdio.h>
#include <math.h>

#include <emmintrin.h>
#include <immintrin.h>
#include <intrin.h>

int ops_scalar(float* a, float* b, float* c, float* d, int N)
{
  for (int i = 0; i<N; i++)
  {
    d[i] = a[i] * b[i] + sqrt(c[i]);
  }
  return 0;
}

int ops_SSE(float* a, float* b, float* c, float* d, int N)
{
  for (int i = 0; i<N; i += 4)
  {
    __m128 A = _mm_load_ps(&(a[i])); //Za³adowanie 4 float
    __m128 B = _mm_load_ps(&(b[i]));
    __m128 C = _mm_load_ps(&(c[i]));

    __m128 mul = _mm_mul_ps(A, B); //A*B
    __m128 sqrt = _mm_sqrt_ps(C); //Sqrt(C)

    __m128 sum = _mm_add_ps(mul, sqrt); //A*B+Sqrt(C)

    _mm_store_ps(&(d[i]), sum); //Zapis wyniku do pamiêci
  }
  return 0;
}

int ops_AVX(float* a, float* b, float* c, float* d, int N)
{
  for (int i = 0; i<N; i += 8)
  {
    __m256 A = _mm256_load_ps(&(a[i]));
    __m256 B = _mm256_load_ps(&(a[i]));
    __m256 C = _mm256_load_ps(&(a[i]));

    __m256 mul = _mm256_mul_ps(A, B);

    __m256 sqrt = _mm256_sqrt_ps(C);

    __m256 sum = _mm256_add_ps(mul, sqrt);

    _mm256_store_ps(&(d[i]), sum);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  fprintf(stdout, "START\n");

  int N = 100000000;

  unsigned __int64 ticks;

  //Alokacja pamieci z wyrównaniem 
  float* a = (float*)_aligned_malloc(sizeof(float)*N, 32);
  float* b = (float*)_aligned_malloc(sizeof(float)*N, 32);
  float* c = (float*)_aligned_malloc(sizeof(float)*N, 32);

  float* d = (float*)_aligned_malloc(sizeof(float)*N, 32);

  //Za³adowanie losowych danych do przetworzenia
  for (int i = 0; i<N; i++)
  {
    a[i] = rand();
    b[i] = rand();
    c[i] = rand();
  }

  ticks = __rdtsc(); //Pomar czasu w taktach 
  ops_scalar(a, b, c, d, N);
  ticks = __rdtsc() - ticks;
  printf("Scalar: %I64d\n", ticks);

  ticks = __rdtsc();
  ops_SSE(a, b, c, d, N);
  ticks = __rdtsc() - ticks;
  printf("SSE:    %I64d\n", ticks);

  ticks = __rdtsc();
  ops_AVX(a, b, c, d, N);
  ticks = __rdtsc() - ticks;
  printf("AVX:    %I64d\n", ticks);

  _aligned_free(a);
  _aligned_free(b);
  _aligned_free(c);

  _aligned_free(d);
  system("pause");
  return 0;
}