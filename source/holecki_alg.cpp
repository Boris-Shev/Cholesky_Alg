#include "header.hpp"

using namespace std;

void* HolecAlgParallel(void* arg) {
  ARGS* args = (ARGS*)arg;

  args[0].err = HolecAlg(args->n, args->id, args->total_threads,
                      args->A, args->b, args->x, args->R, args->RR, args->d);

  pthread_exit(nullptr);
}

int HolecAlg (int n, int id, int total_threads, double* A,
              double* b, double* x, double* R, double* RR, double* d) { // В конце RR, d запихнуть в R
id=id;total_threads=total_threads;R=R;b=b;x=x;A=A;n=n;RR=RR;d=d;

  double sum;
  double eps = std::numeric_limits<double>::epsilon();
  int err = 0;
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
  //////////////////////////////////////////////////
  if(id == 1){

  for (int i = 0; i < n; i++) {
    sum = 0;
    for (int k = 0; k < i; k++) {
      sum += R[k*n + i] * R[k*n + i] * d[k];
    }

    d[i] = Sgn(A[i*n + i] - sum);
    if(fabs(d[i]) < eps) {
      err = -1;
      goto M1;
    }
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
  if (err)
    return err;
  ////////////////////////////////////////////////////////

  for (int i = n - 1; i >= 0; i--) {
    if(id == 1) {
      for (int j = i; j < n; j++) {
        RR[i*n + j] = RR[i*n + j] / R[i*n + i];
      }
    }
    synchronize(total_threads);

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
  ///////////////////////////////////////////////////////

  left = (n / total_threads) * (id - 1);
  right = (n / total_threads) * id;
  if (id == total_threads)
    right = n;

  synchronize(total_threads);

  for (int i = left; i < right; i++) {
    sum = 0;
    for (int j = 0; j < n; j++) {
      sum = sum + RR[j*n + i] * b[j];
    }
    x[i] = sum;
  }
  synchronize(total_threads);

  if (id == 1) {
    for (int i = 0; i < n; i++) {
      x[i] = x[i] * d[i];
      d[i] = x[i];
    }
  }

  synchronize(total_threads);

  for (int i = left; i < right; i++) {
    sum = 0;
    for (int j = 0; j < n; j++) {
      sum = sum + RR[i*n + j] * d[j];
    }
    x[i] = sum;
  }

  synchronize(total_threads);
  // if (id == 1) {
  //   printf("\n" );
  //   PrintMat(x,1,n);
  //   printf("\n" );
  // }

   return 0;
}


template <typename T>
int Sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
