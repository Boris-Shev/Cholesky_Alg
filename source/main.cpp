#include "header.hpp"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    printf("Недостаточное количество аргументов\n");
    return -1;
  }
  for (int i = 1; i < 4; i++) {
    std::string s(argv[i]);
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    if (!(!s.empty() && it == s.end())) {
          printf("Некорректные аргументы\n");
          return -2;
    }
  }
  int n = std::stoi(argv[1]);
  int m = std::stoi(argv[2]);
  int k = std::stoi(argv[3]);
  if (n <= 0) {
    printf("Некорректный размер матрицы\n");
    return -3;
  }
  double* matrx = new double[n*n];
  int err;
  if (k == 0)
    err = InMat(n, k, matrx, argv[4]);
  else
    err = InMat(n, k, matrx, nullptr);
  switch (err) {
    case -1:
      std::cout << "Ошибка открытия файла" << std::endl;
      delete[] matrx;
      return -4;
    case -2:
      std::cout << "Неверный формат данных" << std::endl;
      delete[] matrx;
      return -5;
    case -3:
      std::cout << "Ошибка ввода-вывода при чтении" << std::endl;
      delete[] matrx;
      return -6;
    case -4:
      std::cout << "Недостаточное количество элементов" << std::endl;
      delete[] matrx;
      return -7;
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

  err = 0;
  double time = (double)clock();
  err = HolecAlg (n, matrx, b, x, extra_mem);
  time = (double)(clock() - time) / CLOCKS_PER_SEC;
  if (err == -1) {
    printf("Матрица вырождена или некорректна\n");
    delete[] matrx;
    delete[] b;
    delete[] x;
    delete[] extra_mem;
    return -8;
  }
  PrintMat(x, n, 1, m);
  printf("Время алгоритма: %.3lf\n", time);
  printf("Норма невязки: %lf\n", Residual(matrx, n, b, x));
  printf("Норма погрешности: %lf\n", Inaccuracy(x, n));

  delete[] matrx;
  delete[] b;
  delete[] x;
  delete[] extra_mem;
  return 0;
}
