#include "header.hpp"

using namespace std;

void* HolecAlgParallel(void* arg) {
  ARGS* args = (ARGS*)arg;

  int n = args->n;
  //args->time = currentTime();
  args->err = HolecAlg(args->n, args->id, args->total_threads,
                      args->A, args->b, args->x, args->R, args->R + n*n, args->R + 2*n*n, args->time);
  //args->time = currentTime() - args->time;

  pthread_exit(nullptr);
}

int HolecAlg (int n, int id, int total_threads, double* A,
              double* b, double* x, double* R, double* RR, double* d, double* time) {
time=time;
  double sum;
  double eps = 1e-12;
  int left = (n*n / total_threads) * (id - 1);
  int right = (n*n / total_threads) * id;
  if (id == total_threads)
    right = n*n;

  for(int i = left; i < right; i++) {
    R[i] = 0;
    RR[i] = 0;
  }
  for (int i = 0; i < n; i++) {
    d[i] = 0;
    RR[i*n + i] = 1;
  }
  synchronize(total_threads);
  ///////////////////////////#Построение Холецкого#////////////////////////////
  if(id == 1){

    for (int i = 0; i < n; i++) {
      sum = 0;
      for (int k = 0; k < i; k++) {
        sum += R[k*n + i] * R[k*n + i] * d[k];
      }

      if (fabs(A[i*n + i] - sum) < eps){
        d[0] = 0;
        goto M1;
      }
      d[i] = Sgn(A[i*n + i] - sum);
      R[i*n + i] = sqrt(fabs(A[i*n + i] - sum));

      for (int j = i + 1; j < n; j++) {
        sum = 0;
        for (int k = 0; k < i; k++) {
          sum += R[k*n + i] * d[k] * R[k*n + j];
        }
        R[i*n + j] = (A[i*n + j] - sum) / (R[i*n + i] * d[i]);
      }
    }
  // PrintMat(A,n,n);
  // printf("\n" );
  // PrintMat(R,n,n);
  // printf("\n" );
  // PrintMat(d,1,n);
  // printf("\n" );
  }
M1:
  synchronize(total_threads);
  if (fabs(d[0]) < eps)
    return -1;
  //////////////////////////#Обращение матрицы R#//////////////////////////////

  for (int i = n - 1; i >= 0; i--) {
    if(id == 1) {
      for (int j = i; j < n; j++) {
        RR[i*n + j] = RR[i*n + j] / R[i*n + i];
      }
    }

    synchronize(total_threads);

    // left = ((n-i) / total_threads) * (id - 1) + i;
    // right = ((n-i) / total_threads) * id + i;
    // if (id == total_threads)
    //   right = n;
    // //printf("%d: %d, %d\n", id, left, right);
    //
    // for (int k = 0; k < i; k++) {
    //   for (int j = left; j < right; j++) {
    //     RR[k*n + j] = RR[k*n + j] - R[k*n + i] * RR[i*n + j];
    //   }
    // }
    left = (i / total_threads) * (id - 1);
    right = (i / total_threads) * id;
    if (id == total_threads)
      right = i;

    for (int k = left; k < right; k++) {
      for (int j = i; j < n; j++) {
        RR[k*n + j] = RR[k*n + j] - R[k*n + i] * RR[i*n + j];
      }
    }
    synchronize(total_threads);
  }
  // if (id == 1) {
  //   printf("\n" );
  //   PrintMat(RR,n,n);
  //   printf("\n" );
  // }
  ////////////////////////#Перемножение матриц#///////////////////////////////

  left = (n / total_threads) * (id - 1);
  right = (n / total_threads) * id;
  if (id == total_threads)
    right = n;


  for (int i = left; i < right; i++) {
    x[i] = 0;
    for (int k = 0; k < n; k++) {
      for (int q = 0; q < n; q++) {
        x[i] += RR[i*n + k] * d[k] * RR[q*n + k] * b[q];
      }
    }
  }
  {// for (int i = left; i < right; i++) { // Начало рабочего куска
  //   sum = 0;
  //   for (int j = 0; j < n; j++) {
  //     sum = sum + RR[j*n + i] * b[j];
  //   }
  //   x[i] = sum;
  // }
  // synchronize(total_threads);
  //
  // if (id == 1) {
  //   for (int i = 0; i < n; i++) {
  //     x[i] = x[i] * d[i];
  //     d[i] = x[i];
  //   }
  // }
  //
  // synchronize(total_threads);
  //
  // for (int i = left; i < right; i++) {
  //   sum = 0;
  //   for (int j = 0; j < n; j++) {
  //     sum = sum + RR[i*n + j] * d[j];
  //   }
  //   x[i] = sum;
  // }
  //
  // synchronize(total_threads); // Конец рабочего куска
  // // if (id == 1) {
  // //   printf("\n" );
  // //   PrintMat(x,1,n);
  // //   printf("\n" );
  // // }
  }

   return 0;
}


template <typename T>
int Sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
