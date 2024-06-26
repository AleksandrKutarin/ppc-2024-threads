// Copyright 2024 Bozin Dmitry
#include "tbb/bozin_d_trapez/include/ops_tbb.hpp"

#include <cmath>
#include <functional>
#include <iostream>

namespace Bozin_d_tbb {

double multiDimensionalIntegral(const std::function<double(double, double)>& func, double ax, double bx, double ay,
                                double by, int nx, int ny) {
  double stepx = (bx - ax) / nx;
  double stepy = (by - ay) / ny;
  double summ = 0;

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      double x1 = ax + i * stepx;
      double x2 = ax + (i + 1) * stepx;
      double y1 = ay + j * stepy;
      double y2 = ay + (j + 1) * stepy;

      double fy1 = func(x1, y1);
      double fy2 = func(x2, y1);
      double fy3 = func(x1, y2);
      double fy4 = func(x2, y2);

      double area = ((x2 - x1) * (y2 - y1));
      summ += area * (fy1 + fy2 + fy3 + fy4) / 4;
    }
  }

  return summ;
}

double multiDimensionalIntegralTBB(const std::function<double(double, double)>& func, double ax, double bx, double ay,
                                   double by, int nx, int ny) {
  double stepx = (bx - ax) / nx;
  double stepy = (by - ay) / ny;
  double reslt = 0;

  reslt = tbb::parallel_reduce(
      tbb::blocked_range2d<int>(0, nx, 0, ny), 0.0,
      [&](const tbb::blocked_range2d<int>& r, double summ) {
        for (int i = r.rows().begin(); i != r.rows().end(); ++i) {
          for (int j = r.cols().begin(); j != r.cols().end(); ++j) {
            double x1 = ax + i * stepx;
            double x2 = ax + (i + 1) * stepx;
            double y1 = ay + j * stepy;
            double y2 = ay + (j + 1) * stepy;

            double fy1 = func(x1, y1);
            double fy2 = func(x2, y1);
            double fy3 = func(x1, y2);
            double fy4 = func(x2, y2);

            double area = ((x2 - x1) * (y2 - y1));
            summ += area * (fy1 + fy2 + fy3 + fy4) / 4;
          }
        }
        return summ;
      },
      std::plus<>());

  return reslt;
}

bool BozinTaskSequential::pre_processing() {
  internal_order_test();
  // Init value for input and output
  ax = reinterpret_cast<double*>(taskData->inputs[0])[0];
  ay = reinterpret_cast<double*>(taskData->inputs[0])[1];
  bx = reinterpret_cast<double*>(taskData->inputs[0])[2];
  by = reinterpret_cast<double*>(taskData->inputs[0])[3];
  nx = reinterpret_cast<int*>(taskData->inputs[1])[0];
  ny = reinterpret_cast<int*>(taskData->inputs[1])[1];
  return true;
}

bool BozinTaskSequential::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool BozinTaskSequential::run() {
  internal_order_test();
  res = multiDimensionalIntegral(func, ax, bx, ay, by, nx, ny);
  return true;
}

bool BozinTaskSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool BozinTaskParralel::pre_processing() {
  internal_order_test();
  // Init vectors
  ax = reinterpret_cast<double*>(taskData->inputs[0])[0];
  ay = reinterpret_cast<double*>(taskData->inputs[0])[1];
  bx = reinterpret_cast<double*>(taskData->inputs[0])[2];
  by = reinterpret_cast<double*>(taskData->inputs[0])[3];
  nx = reinterpret_cast<int*>(taskData->inputs[1])[0];
  ny = reinterpret_cast<int*>(taskData->inputs[1])[1];
  return true;
}

bool BozinTaskParralel::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool BozinTaskParralel::run() {
  internal_order_test();
  res = multiDimensionalIntegral(func, ax, bx, ay, by, nx, ny);
  return true;
}

bool BozinTaskParralel::post_processing() {
  internal_order_test();
  reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  return true;
}
}  // namespace Bozin_d_tbb
