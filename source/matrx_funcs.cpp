#include "header.hpp"

int TestInitArg (int argc, char* argv[], int* n, int* m, int* p, int* k) {
   if (argc < 5)
     return -1;

   for (int i = 1; i < 5; i++) {
     int ind = 0;
     std::string s(argv[i]);
     std::string::const_iterator it = s.begin();

     while (it != s.end() && std::isdigit(*it)) {
       ++it;
       ++ind;
     }
     if (!(!s.empty() && it == s.end()) || ind > 5)
       return -2;
   }
   *n = std::stoi(argv[1]);
   *m = std::stoi(argv[2]);
   *p = std::stoi(argv[3]);
   *k = std::stoi(argv[4]);

   if(*p <= 0)
    return -2;

   if(*k > 4 || *k < 0)
    return -2;

   if (*m < 0)
    return -2;

   return 0;
}

int InMat (int size, int formula, double* matrx, char* file) {
  if (formula == 0) {
    std::ifstream fin(file);
    if(!fin.is_open()){ // Ошибка открытия файла
      return -1;
    }
    int i = 0;
    double tmp;
    for (i = 0; fin >> tmp && i < size*size; i++)
      matrx[i] = tmp;

    if (fin.eof() && i == size*size)
      {}
    else if (i != size*size) // Недостаточное количество элементов
      return -2;
    else if (!fin.eof()) // Файл больше чем размерность матрицы
      return -2;
    else if (fin.fail()) // Неверный формат данных
      return -2;
    else if (fin.bad()) // Ошибка ввода-вывода при чтении
      return -3;
  } else {
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
        matrx[i*size + j] = HelperInMat(formula, size, i, j);
  }
  return 0;
}

double HelperInMat (int formula, int size, int i, int j) {
  switch (formula) {
    case 1:
      return size - std::max(i + 1, j + 1) + 1;
    case 2:
      return std::max(i + 1, j + 1);
    case 3:
      return std::fabs(i - j);
    case 4:
      return 1 / double(i + j + 1);
    default:
      return 0;
  }
  return 0;
}

void PrintMat (double* matrx, int numRow, int numCol, int limiter) {
  if (limiter <= numRow) {
    for (int i = 0; i < limiter; i++) {
      for (int j = 0; j < numCol; j++) {
        printf(" %10.3e", matrx[i*numCol + j]);
      }
      printf("\n");
    }
  } else {
    for (int i = 0; i < numRow; i++) {
      for (int j = 0; j < numCol; j++) {
        printf(" %10.3e", matrx[i*numCol + j]);
      }
      printf("\n");
    }
  }
}

void PrintMat (double* matrx, int numRow, int numCol) {
  PrintMat(matrx, numRow, numCol, std::max(numCol, numRow));
}

double Residual (double* A, int n, double* b, double* x) {
  double norm_b = 0;
  double norm_num = 0;
  double sum;

  for (int i = 0; i < n; i++) {
    norm_b += b[i] * b[i];
  }

  for (int i = 0; i < n; i++) {
    sum = 0;
    for (int k = 0; k < n; k++) {
      sum += A[i*n + k] * x[k];
    }
    norm_num += (sum - b[i]) * (sum - b[i]);
  }
  return std::sqrt(std::fabs(norm_num)) / std::sqrt(std::fabs(norm_b));
}

double Inaccuracy (double* x, int size) {
  double sum = 0;
  for (int i = 0; i < size; i += 2) {
    sum += (x[i] - 1) * (x[i] - 1);
  }
  for (int i = 1; i < size; i += 2) {
    sum += x[i] * x[i];
  }
  return std::sqrt(std::fabs(sum));
}

double currentTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((double)time.tv_sec * 1000000. + (double)time.tv_usec)/1000000.;
}
