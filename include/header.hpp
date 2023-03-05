#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <limits>
#include <ctype.h>
#include <string>
#include <pthread.h>
#include <sys/time.h>

struct ARGS
{
  int n;
	double* A;
  double* b;
	double* x;
  double* R;
	int id;
	int total_threads;
  int err = 0;
  double time = 0;
};

void synchronize(int total_threads);
int TestInitArg (int argc, char* argv[], int* n, int* m, int* p, int* k);
int InMat (int size, int formula, double* matrx, char* file);
double HelperInMat (int formula, int size, int i, int j);
void PrintMat (double* matrx, int numRow, int numCol, int limiter);
void PrintMat (double* matrx, int numRow, int numCol);
void* HolecAlgParallel(void* arg);
int HolecAlg (int n, int id, int total_threads, double* A,
              double* b, double* x, double* R, double* RR, double* d);
template <typename T>
int Sgn(T);
double Residual (double* A, int n, double* b, double* x);
double Inaccuracy (double* x, int size);
double currentTime();


#endif // _HEADER_HPP_
