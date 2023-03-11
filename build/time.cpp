#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define num_tests 10

int main(){
  for (int dim = 1170; dim <= 1190; dim+=20) {
    std::vector<double> v;
    std::string n(std::to_string(dim));
    for (int i = 0; i < num_tests; i++) {
      system(("./holec_alg " + n + " 0 4 1").c_str());
      system(("./holec_alg " + n + " 0 1 1").c_str());
      std::ifstream threads1("1 time");
      std::ifstream threads4("4 time");

      std::string tmp;

      threads1 >> tmp;
      double time1 = std::stod(tmp);

      threads4 >> tmp;
      double time4 = std::stod(tmp);

      v.push_back(time1 / time4);
    }

    double sum = 0;
    for (int i = 0; i < num_tests; i++) {
      sum += v[i];
    }
    printf("\n%d  Ratio: %lf\n", dim, sum / num_tests);
  }


  return 0;
}
