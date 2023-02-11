#include "header.hpp"

using namespace std;

void* HolecAlgParallel(void* arg) {
  ARGS* args = (ARGS*)arg;

  args->time = currentTime();
  args->err = HolecAlg(args->n, args->id, args->total_threads,
                      args->A, args->b, args->x, args->R);
  args->time = currentTime() - args->time;

  pthread_exit(nullptr);
}

int HolecAlg (int n, int id, int total_threads, double* A,
              double* b, double* x, double* R) {

  double sum;
  double eps = std::numeric_limits<double>::epsilon();
  int left = (n*n / total_threads) * (id - 1);
  int right = (n*n / total_threads) * id;
  if (id == total_threads)
    right = n*n;

  for(int i = left; i < right; i++) {
    R[i] = 0;
    R[n*n + i] = 0;
  }
  for (int i = 0; i < n; i++) {
    R[2*n*n + i] = 0;
    R[n*n + i*n + i] = 1;
  }
  synchronize(total_threads);
  //////////////////////////////////////////////////
  if(id == 1){

  for (int i = 0; i < n; i++) {
    sum = 0;
    for (int k = 0; k < i; k++) {
      sum += R[k*n + i] * R[k*n + i] * R[2*n*n + k];
    }

    R[2*n*n + i] = Sgn(A[i*n + i] - sum);
    if(fabs(R[2*n*n + i]) < eps) {
      R[2*n*n + 0] = 0;
      goto M1;
    }
    R[i*n + i] = sqrt(fabs(A[i*n + i] - sum));

    for (int j = i + 1; j < n; j++) {
      sum = 0;
      for (int k = 0; k < i; k++) {
        sum += R[k*n + i] * R[2*n*n + k] * R[k*n + j];
      }
      R[i*n + j] = (A[i*n + j] - sum) / (R[i*n + i] * R[2*n*n + i]);
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
  if (fabs(R[2*n*n + 0]) < eps)
    return -1;
  ////////////////////////////////////////////////////////

  for (int i = n - 1; i >= 0; i--) {
    if(id == 1) {
      for (int j = i; j < n; j++) {
        R[n*n + i*n + j] = R[n*n + i*n + j] / R[i*n + i];
      }
    }
    synchronize(total_threads);

    left = (i / total_threads) * (id - 1);
    right = (i / total_threads) * id;
    if (id == total_threads)
      right = i;

    for (int k = left; k < right; k++) {
      for (int j = i; j < n; j++) {
        R[n*n + k*n + j] = R[n*n + k*n + j] - R[k*n + i] * R[n*n + i*n + j];
      }
    }
    synchronize(total_threads);
  }
  // if (id == 1) {
  //   printf("\n" );
  //   PrintMat(RR,n,n);
  //   printf("\n" );
  // }
  ///////////////////////////////////////////////////////

  left = (n / total_threads) * (id - 1);
  right = (n / total_threads) * id;
  if (id == total_threads)
    right = n;


  for (int i = left; i < right; i++) {
    x[i] = 0;
    for (int k = 0; k < n; k++) {
      for (int q = 0; q < n; q++) {
        x[i] += R[n*n + i*n + k] * R[2*n*n + k] * R[n*n + q*n + k] * b[q];
      }
    }
  }

  // for (int i = left; i < right; i++) { // Начало рабочего куска
  //   sum = 0;
  //   for (int j = 0; j < n; j++) {
  //     sum = sum + R[n*n + j*n + i] * b[j];
  //   }
  //   x[i] = sum;
  // }
  // synchronize(total_threads);
  //
  // if (id == 1) {
  //   for (int i = 0; i < n; i++) {
  //     x[i] = x[i] * R[2*n*n + i];
  //     R[2*n*n + i] = x[i];
  //   }
  // }
  //
  // synchronize(total_threads);
  //
  // for (int i = left; i < right; i++) {
  //   sum = 0;
  //   for (int j = 0; j < n; j++) {
  //     sum = sum + R[n*n + i*n + j] * R[2*n*n + j];
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

   return 0;
}


template <typename T>
int Sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
