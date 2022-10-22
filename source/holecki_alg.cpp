#include "header.hpp"

using namespace std;

int HolecAlg (int n, double* A, double* b, double* x, double* ExtraMem) {
  double sum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
      sum = 0;
      for (int k = 0; k < j; k++){
        sum += ExtraMem[i*n + k] * ExtraMem[j*n + k];
      }
      if (i == j) {
        ExtraMem[i*n + j] = sqrt(fabs(A[i*n + i] - sum));
      }
      else {
        if (fabs(ExtraMem[j*n + j]) < std::numeric_limits<double>::epsilon())
          return -1; // Матрица вырождена
        ExtraMem[i*n + j] = (1.0 / ExtraMem[j*n + j] * (A[i*n + j] - sum));
      }
    }
  }
   //PrintMat(ExtraMem,n,n,n);
   //printf("\n");



  for (int k = 0; k < n; k++) {
    for (int i = 0; i < k; i++) {
      ExtraMem[k*n + i] /= ExtraMem[k*n + k];
    }
    ExtraMem[k*n + k] = 1 / ExtraMem[k*n + k];
    for(int i = k + 1; i < n; i++){
      for (int j = 0; j < k; j++) {
        ExtraMem[i*n + j] += - ExtraMem[k*n + j] * ExtraMem[i*n + k];
      }
    }
    for (int i = k + 1; i < n; i++) {
      ExtraMem[i*n + k] = - ExtraMem[k*n + k] * ExtraMem[i*n + k];
    }
  }
  // PrintMat(ExtraMem, n, n, n);
  // printf("\n");

  for (int m = 0; m < n; m++) {
    double sum2 = 0;
    for (int z = 0; z < m; z++) {
      sum = 0;
      for (int k = m; k < n; k++) {
        sum += ExtraMem[k*n + z] * ExtraMem[k*n + m];
      }
      sum2 += sum * b[z];
    }
    for (int z = m; z < n; z++) {
      sum = 0;
      for (int k = z; k < n; k++) {
        sum += ExtraMem[k*n + z] * ExtraMem[k*n + m];
      }
      sum2 += sum * b[z];
    }
    x[m] = sum2;
  }
  // PrintMat(x, n, 1, n);
  // printf("\n");
  return 0;
}


template <typename T>
int Sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
