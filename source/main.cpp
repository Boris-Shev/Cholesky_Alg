#include "header.hpp"

int main(int argc, char* argv[]) {
  int err;
  argc = argc;

  int n = std::stoi(argv[1]);
  int m = std::stoi(argv[2]);
  int k = std::stoi(argv[3]);
  m=m;
  double* matrx = new double[n*n];

  if (k == 0)
    err = InMat(n, k, matrx, argv[4]);
  else
    err = InMat(n, k, matrx, nullptr);
  switch (err) {
    case -1:
      std::cout << "Ошибка открытия файла" << std::endl;
      delete[] matrx;
      return err;
    case -2:
      std::cout << "Неверный формат данных" << std::endl;
      delete[] matrx;
      return err;
    case -3:
      std::cout << "Ошибка ввода-вывода при чтении" << std::endl;
      delete[] matrx;
      return err;
    case -4:
      std::cout << "Недостаточное количество элементов" << std::endl;
      delete[] matrx;
      return err;
    default:
      break;
  }

  double* b = new double[n];
  double* x = new double[n];
  for (int i = 0; i < n; i++) {
    double sum = 0;
    for (int j = 0; j < (n + 1)/ 2; j++)
      sum += matrx[i*n + 2*j];
    b[i] = sum;
  }
  double* extra_mem = new double[n*n];

  int time = clock();
  HolecAlg (n, matrx, b, x, extra_mem);
  time = clock() - time;
  PrintMat(x, n, 1, m);
  printf("Время алгоритма: %d\n", time);
  printf("Норма невязки: %lf\n", Residual(matrx, n, b, x));

  delete[] matrx;
  delete[] b;
  delete[] x;
  delete[] extra_mem;
  return 0;
}
