#include "stdafx.h"
#include "fig_2_20.h"

 void ch02::fig_2_20::fig_2_20_run(std::vector<double>& x, const std::vector<double>& a, std::vector<double>& b) {
    const int N = x.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            b[i] -= a[j + i * N] * x[j];
        }
        x[i] = b[i] / a[i + i * N];
    }
}

std::vector<double> ch02::fig_2_20::initForwardSubstitution(std::vector<double>& x, std::vector<double>& a, std::vector<double>& b) {
     const int N = x.size();
     for (int i = 0; i < N; ++i) {
         x[i] = 0;
         b[i] = i * i;
         for (int j = 0; j <= i; ++j) {
             a[j + i * N] = 1 + j * i;
         }
     }

     std::vector<double> b_tmp = b;
     std::vector<double> x_gold = x;
     for (int i = 0; i < N; ++i) {
         for (int j = 0; j < i; ++j) {
             b_tmp[i] -= a[j + i * N] * x_gold[j];
         }
         x_gold[i] = b_tmp[i] / a[i + i * N];
     }
     return x_gold;
 }

int ch02::fig_2_20::main() {
      const int N = 32768;

      std::vector<double> a(N * N);
      std::vector<double> b(N);
      std::vector<double> x(N);

      auto x_gold = initForwardSubstitution(x, a, b);

      double serial_time = 0.0;
      {
          tbb::tick_count t0 = tbb::tick_count::now();
          fig_2_20_run(x, a, b);
          serial_time = (tbb::tick_count::now() - t0).seconds();
      }
      if (x_gold != x) {
          std::cerr << "Error: results do not match gold version" << std::endl;
      }
      std::cout << "serial_time == " << serial_time << " seconds" << std::endl;
      return 0;
  }
