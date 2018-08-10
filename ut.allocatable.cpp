#include "FortranArray.h"
#include <catch2/catch.hpp>
using namespace fa;

TEST_CASE("allocatable tests dimension equals 1", "[allocatable]")
{
  SECTION("starting from 0")
  {
    constexpr auto st = 0;
    constexpr auto size = 2;
    allocatable<int, st> cc;
    allocatable<int, st> ff;
    cc.reserve(size);
    ff.allocate(size);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      REQUIRE(count == cc(a));
      REQUIRE(count == cc[a]);
      REQUIRE(count == cc.c(a));
      ++count;
    }
  }

  SECTION("starting from -4")
  {
    constexpr auto st = -4;
    constexpr auto size = 2;
    allocatable<int, st> cc;
    allocatable<int, st> ff;
    cc.reserve(size);
    ff.allocate(size);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a + st));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      REQUIRE(count == cc(a + st));
      REQUIRE(count == cc[a]);
      REQUIRE(count == cc.c(a));
      ++count;
    }
  }

  SECTION("starting from 5")
  {
    constexpr auto st = 5;
    constexpr auto size = 2;
    allocatable<int, st> cc;
    allocatable<int, st> ff;
    cc.reserve(size);
    ff.allocate(size);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a + st));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      REQUIRE(count == cc(a + st));
      REQUIRE(count == cc[a]);
      REQUIRE(count == cc.c(a));
      ++count;
    }
  }
}

TEST_CASE("allocatable tests dimension equals 2", "[allocatable]")
{
  SECTION("starting from 0")
  {
    constexpr auto st = 0;
    constexpr auto size = 6;
    allocatable<int, st, st> cc;
    allocatable<int, st, st> ff;
    cc.reserve(2, 3);
    ff.allocate(3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + st, a + st));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        REQUIRE(count == cc(b + st, a + st));
        REQUIRE(count == cc[a][b]);
        REQUIRE(count == cc.c(a, b));
        ++count;
      }
  }

  SECTION("starting from -4")
  {
    constexpr auto st = -4;
    constexpr auto size = 6;
    allocatable<int, st, st> cc;
    allocatable<int, st, st> ff;
    cc.reserve(2, 3);
    ff.allocate(3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + st, a + st));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        REQUIRE(count == cc(b + st, a + st));
        REQUIRE(count == cc[a][b]);
        REQUIRE(count == cc.c(a, b));
        ++count;
      }
  }

  SECTION("starting from 5")
  {
    constexpr auto st = 5;
    constexpr auto size = 6;
    allocatable<int, st, st> cc;
    allocatable<int, st, st> ff;
    cc.reserve(2, 3);
    ff.allocate(3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + st, a + st));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        REQUIRE(count == cc(b + st, a + st));
        REQUIRE(count == cc[a][b]);
        REQUIRE(count == cc.c(a, b));
        ++count;
      }
  }
}

TEST_CASE("allocatable tests dimension equals 3", "[allocatable]")
{
  SECTION("starting from 0")
  {
    constexpr auto st = 0;
    constexpr auto size = 24;
    allocatable<int, st, st, st> cc;
    allocatable<int, st, st, st> ff;
    cc.reserve(2, 3, 4);
    ff.allocate(4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c) {
          REQUIRE(count == ff(c + st, b + st, a + st));
          REQUIRE(count == ff[a][b][c]);
          REQUIRE(count == ff.c(a, b, c));
          REQUIRE(count == cc(c + st, b + st, a + st));
          REQUIRE(count == cc[a][b][c]);
          REQUIRE(count == cc.c(a, b, c));
          ++count;
        }
  }

  SECTION("starting from -4")
  {
    constexpr auto st = -4;
    constexpr auto size = 24;
    allocatable<int, st, st, st> cc;
    allocatable<int, st, st, st> ff;
    cc.reserve(2, 3, 4);
    ff.allocate(4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c) {
          REQUIRE(count == ff(c + st, b + st, a + st));
          REQUIRE(count == ff[a][b][c]);
          REQUIRE(count == ff.c(a, b, c));
          REQUIRE(count == cc(c + st, b + st, a + st));
          REQUIRE(count == cc[a][b][c]);
          REQUIRE(count == cc.c(a, b, c));
          ++count;
        }
  }

  SECTION("starting from 5")
  {
    constexpr auto st = 5;
    constexpr auto size = 24;
    allocatable<int, st, st, st> cc;
    allocatable<int, st, st, st> ff;
    cc.reserve(2, 3, 4);
    ff.allocate(4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c) {
          REQUIRE(count == ff(c + st, b + st, a + st));
          REQUIRE(count == ff[a][b][c]);
          REQUIRE(count == ff.c(a, b, c));
          REQUIRE(count == cc(c + st, b + st, a + st));
          REQUIRE(count == cc[a][b][c]);
          REQUIRE(count == cc.c(a, b, c));
          ++count;
        }
  }
}

TEST_CASE("allocatable tests dimension equals 5", "[allocatable]")
{
  SECTION("starting from 0")
  {
    constexpr auto st = 0;
    constexpr auto size = 720;
    allocatable<int, st, st, st, st, st> cc;
    allocatable<int, st, st, st, st, st> ff;
    cc.reserve(2, 3, 4, 5, 6);
    ff.allocate(6, 5, 4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c)
          for (int d = 0; d < 5; ++d)
            for (int e = 0; e < 6; ++e) {
              REQUIRE(count == ff(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == ff[a][b][c][d][e]);
              REQUIRE(count == ff.c(a, b, c, d, e));
              REQUIRE(count == cc(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == cc[a][b][c][d][e]);
              REQUIRE(count == cc.c(a, b, c, d, e));
              ++count;
            }
  }

  SECTION("starting from -4")
  {
    constexpr auto st = -4;
    constexpr auto size = 720;
    allocatable<int, st, st, st, st, st> cc;
    allocatable<int, st, st, st, st, st> ff;
    cc.reserve(2, 3, 4, 5, 6);
    ff.allocate(6, 5, 4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c)
          for (int d = 0; d < 5; ++d)
            for (int e = 0; e < 6; ++e) {
              REQUIRE(count == ff(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == ff[a][b][c][d][e]);
              REQUIRE(count == ff.c(a, b, c, d, e));
              REQUIRE(count == cc(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == cc[a][b][c][d][e]);
              REQUIRE(count == cc.c(a, b, c, d, e));
              ++count;
            }
  }

  SECTION("starting from 5")
  {
    constexpr auto st = 5;
    constexpr auto size = 720;
    allocatable<int, st, st, st, st, st> cc;
    allocatable<int, st, st, st, st, st> ff;
    cc.reserve(2, 3, 4, 5, 6);
    ff.allocate(6, 5, 4, 3, 2);

    REQUIRE(size == cc.size());
    REQUIRE(size == ff.size());

    int* pc = cc.data();
    int* pf = ff.data();

    for (int i = 0; i < size; ++i) {
      pc[i] = i;
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a)
      for (int b = 0; b < 3; ++b)
        for (int c = 0; c < 4; ++c)
          for (int d = 0; d < 5; ++d)
            for (int e = 0; e < 6; ++e) {
              REQUIRE(count == ff(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == ff[a][b][c][d][e]);
              REQUIRE(count == ff.c(a, b, c, d, e));
              REQUIRE(count == cc(e + st, d + st, c + st, b + st, a + st));
              REQUIRE(count == cc[a][b][c][d][e]);
              REQUIRE(count == cc.c(a, b, c, d, e));
              ++count;
            }
  }
}
