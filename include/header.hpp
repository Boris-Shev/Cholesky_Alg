#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

int InMat (int size, int formula, double* matrx, char* file);
double HelperInMat (int formula, int size, int i, int j);
void PrintMat (double* matrx, int numRow, int numCol, int limiter);
void HolecAlg (int n, double* A, double* b, double* x, double* ExtraMem);
template <typename T>
int Sgn(T);
double Residual (double* A, int n, double* b, double* x);


#endif // _HEADER_HPP_
