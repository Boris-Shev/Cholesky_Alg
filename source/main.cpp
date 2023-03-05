#include "header.hpp"

int main(int argc, char* argv[]) {
  // 1: размер матрицы; 2: количество выводимых значений матрицы
  // 3: количество потоков; 4: номер формулы; 5: имя файла
  int n, m, total_threads, k, err;
  err = TestInitArg(argc, argv, &n, &m, &total_threads, &k);
  switch (err) {
    case -1:
      printf("Недостаточное количество аргументов\n");
      return -1;

    case -2:
      printf("Некорректные аргументы\n");
      return -2;

    default:
      break;
  }

  if (n <= 0 || n > 1e4) {
    printf("Некорректный размер матрицы\n");
    return -3;
  }
  if (total_threads > n){
    total_threads = n;
    printf("Количество потоков изменено на размерность матрицы\n");
  }
  if (m > n){
    m = n;
    printf("Количество выводимых элементов изменено на размерность матрицы\n");
  }
/////////////////////////#Заполнение матрицы#////////////////////////////////
  double* matrx = new double[n*n];
  if (k == 0)
    err = InMat(n, k, matrx, argv[5]);
  else
    err = InMat(n, k, matrx, nullptr);
  switch (err) {
    case -1:
      std::cout << "Ошибка открытия файла" << std::endl;
      delete[] matrx;
      return -4;
    case -2:
      std::cout << "Неверный формат данных или неправильная размерность матрицы" << std::endl;
      delete[] matrx;
      return -5;
    case -3:
      std::cout << "Ошибка ввода-вывода при чтении" << std::endl;
      delete[] matrx;
      return -6;
    default:
      break;
  }
//////////////////////#Проверка матрицы#///////////////////////////////
  double eps = std::numeric_limits<double>::epsilon();
  double sum = 0;
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
      if (fabs(matrx[i*n + j] - matrx[j*n + i]) > eps) {
        printf("Матрица не симметрична\n");
        delete[] matrx;
        return -1;
      }

  sum = 0;
  for (int i = 0; i < n*n; i++){
    sum += fabs(matrx[i]);
  }
  if(sum < 1e-30){
    printf("Есть нулевые главные миноры или матрица некорректна\n");
    delete[] matrx;
    return -1;
  } else {
    for (int i = 0; i < n*n; i++)
      matrx[i] = matrx[i] / sum;
  }
///////////////////////////#Подготовка#////////////////////////////////
  double* b = new double[n];
  double* x = new double[n];
  for (int i = 0; i < n; i++) {
    sum = 0;
    for (int j = 0; j < (n + 1)/ 2; j++)
      sum += matrx[i*n + 2*j];
    b[i] = sum;
  }
  double* extra_mem = new double[n*n*2 + n]; // (2*n + 1) X (n)
  ARGS* args = new ARGS[total_threads];
  pthread_t* threads = new pthread_t[total_threads];
  for (int i = 0; i < total_threads; i++)
	{
		args[i].n = n;
		args[i].A = matrx;
    args[i].b = b;
		args[i].x = x;
		args[i].id = i + 1;
    args[i].R = extra_mem;
		args[i].total_threads = total_threads;
	}

  err = 0;
  double time = currentTime();
  /////////////////////////////#Решение системы#///////////////////////////////
  for (int i = 0; i < total_threads; i++)
		if (pthread_create(threads + i, nullptr, HolecAlgParallel, args + i)) {
			printf("Не получилось создать поток %d!\n", i);
      delete[] matrx;
      delete[] b;
      delete[] x;
      delete[] extra_mem;
      delete[] args;
      delete[] threads;

			return -10;
		}

	for (int i = 0; i < total_threads; i++)
		if (pthread_join(threads[i], nullptr)){
			printf("Не получилось подождать поток %d!\n", i);
      delete[] matrx;
      delete[] b;
      delete[] x;
      delete[] extra_mem;
      delete[] args;
      delete[] threads;
			return -11;
		}
  ///////////////////////////#Вывод результатов#///////////////////////////////
  time = currentTime() - time;
  err = args[0].err;
  if (err == -1) {
    printf("Есть нулевые главные миноры или матрица некорректна\n");
    delete[] matrx;
    delete[] b;
    delete[] x;
    delete[] extra_mem;
    delete[] args;
    delete[] threads;
    return -8;
  }

  double residual = Residual(matrx, n, b, x);
  PrintMat(x, n, 1, m);
  printf("Время алгоритма: %.3lf\n", time);
  // for (int i = 0; i < total_threads; i++)
  //   printf("Время потока %d: %3lf\n", args[i].id, args[i].time);

  printf("Норма невязки: %lf\n", residual);
  printf("Норма погрешности: %lf\n", Inaccuracy(x, n));
  printf("%s : residual = %e elapsed = %.2f s = %d n = %d m = %d p = %d\n",
        argv[0], residual, time, k, n, m, total_threads);

  delete[] matrx;
  delete[] b;
  delete[] x;
  delete[] extra_mem;
  delete[] args;
  delete[] threads;
  return 0;
}
