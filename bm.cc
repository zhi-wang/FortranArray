#ifndef NDEBUG
#  ifndef DEBUG
#    define NDEBUG
#  elif DEBUG == 0
#    define NDEBUG
#  endif
#endif

#include "FortranArray.h"
#include <benchmark/benchmark.h>

#include <array>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using OBJECT = double;
namespace bm = ::benchmark;

constexpr auto cd3max = 8 << 10;
constexpr auto cd2 = 20;
constexpr auto cd1 = 3;
static volatile auto vd2 = cd2;
static volatile auto vd1 = cd1;
static OBJECT data[cd3max * cd2 * cd1];
static std::vector<OBJECT> globv(cd3max* cd2* cd1);

int main(int argc, char** argv)
{
  using namespace std;

  auto start_with = [](const string& str, const string& prefix) -> bool {
    return !str.compare(0, prefix.size(), prefix);
  };

  vector<string> vs(argc);
  for (auto i = 0; i < argc; ++i) {
    vs[i] = argv[i];
  }

  string filename = "";
  vector<string> vs2;
  vs2.emplace_back(vs[0]);

  string pref1 = "--";
  string pref2 = "file=";
  for (auto i = 1; i < argc; ++i) {
    if (start_with(vs[i], pref1)) {
      vs2.emplace_back(vs[i]);
    } else if (start_with(vs[i], pref2)) {
      filename = vs[i].substr(pref2.size());
    }
  }

  ifstream ifs(filename);
  if (ifs) {
    string line;
    while (std::getline(ifs, line)) {
      if (start_with(line, pref1)) {
        vs2.emplace_back(line);
      }
    }
  }

  vector<char*> vptr(vs2.size());
  auto argc2 = static_cast<int>(vs2.size());
  for (auto i = 0; i < argc2; ++i) {
    vptr[i] = &vs2[i][0];
  }
  char** argv2 = vptr.data();

  bm::Initialize(&argc2, argv2);
  bm::RunSpecifiedBenchmarks();
}

static void CXX_Vector_bm1(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using vec1 = std::vector<OBJECT>;
  using vec2 = std::vector<vec1>;
  using here_t = std::vector<vec2>;
  here_t here(d3, vec2(d2, vec1(d1)));
  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here[i3][i2][i1];
          ++count;
        }
  }
}

static void Alloc_1_Array_bm2(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using namespace fa;
  using here_t = allocatable<OBJECT, 1, 1, 1>;
  here_t here;
  here.reserve(d3, d2, d1);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here[i3][i2][i1];
          ++count;
        }
  }
}

static void Pure_C_Pointer_bm3(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);
  auto c_index
    = [&](int i3, int i2, int i1) { return d1 * d2 * i3 + d1 * i2 + i1; };

  OBJECT* here = (OBJECT*)std::malloc(sizeof(OBJECT) * d3 * d2 * d1);
  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here[c_index(i3, i2, i1)];
          assert(c_index(i3, i2, i1) == count);
          ++count;
        }
  }
  std::free(here);
}

static void Alloc_0_C_bm4(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using namespace fa;
  using here_t = allocatable<OBJECT, 0, 0, 0>;
  here_t here;
  here.reserve(d3, d2, d1);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here.c(i3, i2, i1);
          assert(here.c_index(i3, i2, i1) == count);
          ++count;
        }
  }
}

static void Alloc_1_C_bm5(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using namespace fa;
  using here_t = allocatable<OBJECT, 1, 1, 1>;
  here_t here;
  here.reserve(d3, d2, d1);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here.c(i3, i2, i1);
          assert(here.c_index(i3, i2, i1) == count);
          ++count;
        }
  }
}

static void Alloc_0_F_bm6(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using namespace fa;
  using here_t = allocatable<OBJECT, 0, 0, 0>;
  here_t here;
  here.allocate(d1, d2, d3);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          globv[count] = here(i1, i2, i3);
          assert(here.fortran_index(i1, i2, i3) == count);
          ++count;
        }
  }
}

static void Alloc_1_F_bm7(bm::State& state)
{
  auto d1 = vd1;
  auto d2 = vd2;
  auto d3 = state.range(0);

  using namespace fa;
  using here_t = allocatable<OBJECT, 1, 1, 1>;
  here_t here;
  here.allocate(d1, d2, d3);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 1; i3 < d3 + 1; ++i3)
      for (auto i2 = 1; i2 < d2 + 1; ++i2)
        for (auto i1 = 1; i1 < d1 + 1; ++i1) {
          globv[count] = here(i1, i2, i3);
          assert(here.fortran_index(i1, i2, i3) == count);
          ++count;
        }
  }
}

template<int cd3>
void CXX_Array_d1(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  using here_t = std::array<std::array<std::array<OBJECT, d1>, d2>, d3>;
  here_t& here = *reinterpret_cast<here_t*>(&data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          here[i3][i2][i1] = count;
          ++count;
        }
  }
}

template<int cd3>
void Pure_C_Array_d2(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  OBJECT(*here)[d3][d2][d1] = reinterpret_cast<OBJECT(*)[d3][d2][d1]>(data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          (*here)[i3][i2][i1] = count;
          ++count;
        }
  }
}

template<int cd3>
void Tensor_Op_d3(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  using namespace fa;
  using here_t = tensor<OBJECT, d3, d2, d1>;
  here_t& here = *reinterpret_cast<here_t*>(&data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          here[i3][i2][i1] = count;
          ++count;
        }
  }
}

template<int cd3>
void Tensor_C_d4(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  using namespace fa;
  using here_t = tensor<OBJECT, d3, d2, d1>;
  here_t& here = *reinterpret_cast<here_t*>(&data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          here.c(i3, i2, i1) = count;
          assert(here.c_index(i3, i2, i1) == count);
          ++count;
        }
  }
}

template<int cd3>
void Dim0_d5(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  using namespace fa;
  using here_t = dimension<OBJECT, r(0, d1 - 1), r(0, d2 - 1), r(0, d3 - 1)>;
  here_t& here = *reinterpret_cast<here_t*>(&data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 0; i3 < d3; ++i3)
      for (auto i2 = 0; i2 < d2; ++i2)
        for (auto i1 = 0; i1 < d1; ++i1) {
          here(i1, i2, i3) = count;
          assert(here.fortran_index(i1, i2, i3) == count);
          ++count;
        }
  }
}

template<int cd3>
void Dim1_d6(bm::State& state)
{
  constexpr auto d1 = cd1;
  constexpr auto d2 = cd2;
  constexpr auto d3 = cd3;

  using namespace fa;
  using here_t = dimension<OBJECT, d1, d2, d3>;
  here_t& here = *reinterpret_cast<here_t*>(&data);

  for (auto _ : state) {
    auto count = 0;
    for (auto i3 = 1; i3 < d3 + 1; ++i3)
      for (auto i2 = 1; i2 < d2 + 1; ++i2)
        for (auto i1 = 1; i1 < d1 + 1; ++i1) {
          here(i1, i2, i3) = count;
          assert(here.fortran_index(i1, i2, i3) == count);
          ++count;
        }
  }
}

BENCHMARK(CXX_Vector_bm1)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Alloc_1_Array_bm2)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Pure_C_Pointer_bm3)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Alloc_0_C_bm4)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Alloc_1_C_bm5)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Alloc_0_F_bm6)->RangeMultiplier(2)->Range(8, cd3max);
BENCHMARK(Alloc_1_F_bm7)->RangeMultiplier(2)->Range(8, cd3max);

#define xpand(TPL)               \
  BENCHMARK_TEMPLATE(TPL, 8);    \
  BENCHMARK_TEMPLATE(TPL, 16);   \
  BENCHMARK_TEMPLATE(TPL, 32);   \
  BENCHMARK_TEMPLATE(TPL, 64);   \
  BENCHMARK_TEMPLATE(TPL, 128);  \
  BENCHMARK_TEMPLATE(TPL, 256);  \
  BENCHMARK_TEMPLATE(TPL, 512);  \
  BENCHMARK_TEMPLATE(TPL, 1024); \
  BENCHMARK_TEMPLATE(TPL, 2048); \
  BENCHMARK_TEMPLATE(TPL, 8096);

xpand(CXX_Array_d1);
xpand(Pure_C_Array_d2);
xpand(Tensor_Op_d3);
xpand(Tensor_C_d4);
xpand(Dim0_d5);
xpand(Dim1_d6);

#undef xpand
