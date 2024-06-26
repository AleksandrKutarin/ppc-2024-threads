// Copyright 2024 Vadim Belan
#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"
#include "tbb/belan_vadim_mat_fox_tbb/include/ops_tbb.hpp"

using namespace BelanTBB;

TEST(belan_fox_blocked_tbb, test_pipeline_run) {
  // Create data
  const int rows = 512;
  const int cols = 512;
  std::vector<double> matrix_a(rows * cols);
  std::vector<double> matrix_b(rows * cols);
  std::vector<double> matrix_c(rows * cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      matrix_a[i * cols + j] = i + j;
      matrix_b[i * cols + j] = i - j;
      matrix_c[i * cols + j] = 0;
    }
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_a.data()));
  taskDataSeq->inputs_count.emplace_back(rows);
  taskDataSeq->inputs_count.emplace_back(cols);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_b.data()));
  taskDataSeq->inputs_count.emplace_back(rows);
  taskDataSeq->inputs_count.emplace_back(cols);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_c.data()));
  taskDataSeq->outputs_count.emplace_back(rows);
  taskDataSeq->outputs_count.emplace_back(cols);

  // Create Task
  auto testTaskTBB = std::make_shared<FoxBlockedParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  // Compare results
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double expected = 0;
      for (int k = 0; k < cols; ++k) {
        expected += matrix_a[i * cols + k] * matrix_b[k * cols + j];
      }
      ASSERT_EQ(matrix_c[i * cols + j], expected);
    }
  }
}

TEST(belan_fox_blocked_tbb, test_task_run) {
  // Create data
  const int rows = 512;
  const int cols = 512;
  std::vector<double> matrix_a(rows * cols);
  std::vector<double> matrix_b(rows * cols);
  std::vector<double> matrix_c(rows * cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      matrix_a[i * cols + j] = i + j;
      matrix_b[i * cols + j] = i - j;
      matrix_c[i * cols + j] = 0;
    }
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_a.data()));
  taskDataSeq->inputs_count.emplace_back(rows);
  taskDataSeq->inputs_count.emplace_back(cols);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_b.data()));
  taskDataSeq->inputs_count.emplace_back(rows);
  taskDataSeq->inputs_count.emplace_back(cols);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(matrix_c.data()));
  taskDataSeq->outputs_count.emplace_back(rows);
  taskDataSeq->outputs_count.emplace_back(cols);

  // Create Task
  auto testTaskTBB = std::make_shared<FoxBlockedParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  // Compare results
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double expected = 0;
      for (int k = 0; k < cols; ++k) {
        expected += matrix_a[i * cols + k] * matrix_b[k * cols + j];
      }
      ASSERT_EQ(matrix_c[i * cols + j], expected);
    }
  }
}