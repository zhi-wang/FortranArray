#include "FortranArray.h"
#include <catch2/catch.hpp>
using namespace fa;

TEST_CASE("range tests", "[range]")
{
  SECTION("if a range starts from 0, its code should equal its size")
  {
    constexpr range rf0(0, 0);
    constexpr range rf1(0, 1);
    constexpr range rf2(0, 2);
    constexpr range rf3(0, 42);
    constexpr range rf4(0, 100);
    REQUIRE(rf0.code() == rf0.size());
    REQUIRE(rf1.code() == rf1.size());
    REQUIRE(rf2.code() == rf2.size());
    REQUIRE(rf3.code() == rf3.size());
    REQUIRE(rf4.code() == rf4.size());
  }

  SECTION("initialize a range with one small natural number")
  {
    constexpr range rf0(0);
    constexpr range rf1(1);
    constexpr range rf2(2);
    constexpr range rf3(3);
    constexpr range rf4(100);
    REQUIRE(rf0.code() == rf0.size());
    REQUIRE(rf1.code() == rf1.size());
    REQUIRE(rf2.code() == rf2.size());
    REQUIRE(rf3.code() == rf3.size());
    REQUIRE(rf4.code() == rf4.size());
    REQUIRE(rf0.front() == 0);
    REQUIRE(rf1.front() == 0);
    REQUIRE(rf2.front() == 0);
    REQUIRE(rf3.front() == 0);
    REQUIRE(rf4.front() == 0);
  }

  SECTION("[front, back] and size")
  {
    constexpr auto size = 42;
    constexpr auto st1 = 0;
    constexpr auto rf1 = ext(st1, size);
    constexpr auto st2 = 7;
    constexpr auto rf2 = ext(st2, size);
    constexpr auto st3 = -4;
    constexpr auto rf3 = ext(st3, size);
    REQUIRE(rf1.front() == st1);
    REQUIRE(rf2.front() == st2);
    REQUIRE(rf3.front() == st3);
    REQUIRE(rf1.back() == -1 + st1 + size);
    REQUIRE(rf2.back() == -1 + st2 + size);
    REQUIRE(rf3.back() == -1 + st3 + size);
  }
}

TEST_CASE("dimension with different initial index", "[dimension]")
{
  SECTION("1 dimensional starts from 0")
  {
    constexpr auto start = 0;
    constexpr auto rf1 = ext(start, 2);
    REQUIRE(rf1.front() == start);
    REQUIRE(rf1.back() == -1 + start + 2);

    dimension<int, rf1> ff;
    const int size = 2;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a + start));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      ++count;
    }
  }

  SECTION("1 dimensional starts from 5")
  {
    constexpr auto start = 5;
    constexpr auto rf1 = ext(start, 2);

    dimension<int, rf1> ff;
    const int size = 2;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a + start));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      ++count;
    }
  }

  SECTION("1 dimensional starts from -4")
  {
    constexpr auto start = -4;
    constexpr auto rf1 = ext(start, 2);

    dimension<int, rf1> ff;
    const int size = 2;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == ff(a + start));
      REQUIRE(count == ff[a]);
      REQUIRE(count == ff.c(a));
      ++count;
    }
  }

  SECTION("2 dimensional starts from 0")
  {
    constexpr auto start = 0;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);

    dimension<int, rf2, rf1> ff;
    const int size = 6;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + start, a + start));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        ++count;
      }
    }
  }

  SECTION("2 dimensional starts from 5")
  {
    constexpr auto start = 5;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);

    dimension<int, rf2, rf1> ff;
    const int size = 6;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + start, a + start));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        ++count;
      }
    }
  }

  SECTION("2 dimensional starts from -4")
  {
    constexpr auto start = -4;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);

    dimension<int, rf2, rf1> ff;
    const int size = 6;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == ff(b + start, a + start));
        REQUIRE(count == ff[a][b]);
        REQUIRE(count == ff.c(a, b));
        ++count;
      }
    }
  }

  SECTION("4 dimensional starts from 0")
  {
    constexpr auto start = 0;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);
    constexpr auto rf3 = ext(start, 4);
    constexpr auto rf4 = ext(start, 5);

    dimension<int, rf4, rf3, rf2, rf1> ff;
    const int size = 120;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        for (int c = 0; c < 4; ++c) {
          for (int d = 0; d < 5; ++d) {
            REQUIRE(count == ff(d + start, c + start, b + start, a + start));
            REQUIRE(count == ff[a][b][c][d]);
            REQUIRE(count == ff.c(a, b, c, d));
            ++count;
          }
        }
      }
    }
  }

  SECTION("4 dimensional starts from 5")
  {
    constexpr auto start = 5;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);
    constexpr auto rf3 = ext(start, 4);
    constexpr auto rf4 = ext(start, 5);

    dimension<int, rf4, rf3, rf2, rf1> ff;
    const int size = 120;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        for (int c = 0; c < 4; ++c) {
          for (int d = 0; d < 5; ++d) {
            REQUIRE(count == ff(d + start, c + start, b + start, a + start));
            REQUIRE(count == ff[a][b][c][d]);
            REQUIRE(count == ff.c(a, b, c, d));
            ++count;
          }
        }
      }
    }
  }

  SECTION("4 dimensional starts from -4")
  {
    constexpr auto start = -4;
    constexpr auto rf1 = ext(start, 2);
    constexpr auto rf2 = ext(start, 3);
    constexpr auto rf3 = ext(start, 4);
    constexpr auto rf4 = ext(start, 5);

    dimension<int, rf4, rf3, rf2, rf1> ff;
    const int size = 120;
    REQUIRE(size == ff.size());

    int* pf = ff.data();
    for (int i = 0; i < size; ++i) {
      pf[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        for (int c = 0; c < 4; ++c) {
          for (int d = 0; d < 5; ++d) {
            REQUIRE(count == ff(d + start, c + start, b + start, a + start));
            REQUIRE(count == ff[a][b][c][d]);
            REQUIRE(count == ff.c(a, b, c, d));
            ++count;
          }
        }
      }
    }
  }
}

TEST_CASE("c/c++-like array", "[array]")
{
  SECTION("1 dimensional starting from 0")
  {
    constexpr int size = 2;
    tensor<int, 2> cc;
    REQUIRE(size == cc.size());

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == cc(a));
      REQUIRE(count == cc[a]);
      REQUIRE(count == cc.c(a));
      ++count;
    }
  }

  SECTION("2 dimensional starting from 0")
  {
    constexpr int size = 6;
    tensor<int, 2, 3> cc;
    REQUIRE(size == cc.size());

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == cc(b, a));
        REQUIRE(count == cc[a][b]);
        REQUIRE(count == cc.c(a, b));
        ++count;
      }
    }
  }

  SECTION("4 dimensional starting from 0")
  {
    constexpr int size = 120;
    tensor<int, 2, 3, 4, 5> cc;
    REQUIRE(size == cc.size());

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        for (int c = 0; c < 4; ++c) {
          for (int d = 0; d < 5; ++d) {
            REQUIRE(count == cc(d, c, b, a));
            REQUIRE(count == cc[a][b][c][d]);
            REQUIRE(count == cc.c(a, b, c, d));
            ++count;
          }
        }
      }
    }
  }

  SECTION("1 dimensional starting from 5")
  {
    constexpr int start = 5;
    constexpr int size = 2;
    tensor<int, ext(start, 2)> cc;

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      REQUIRE(count == cc(a + start));
      REQUIRE(count == cc[a]);
      REQUIRE(count == cc.c(a));
      ++count;
    }
  }

  SECTION("2 dimensional starting from -4")
  {
    constexpr int start = -4;
    constexpr int size = 6;
    tensor<int, ext(start, 2), ext(start, 3)> cc;
    REQUIRE(size == cc.size());

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        REQUIRE(count == cc(start + b, start + a));
        REQUIRE(count == cc[a][b]);
        REQUIRE(count == cc.c(a, b));
        ++count;
      }
    }
  }

  SECTION("4 dimensional starting from 7")
  {
    constexpr int start = 7;
    constexpr int size = 120;

    tensor<int, ext(start, 2), ext(start, 3), ext(start, 4), ext(start, 5)> cc;
    REQUIRE(size == cc.size());

    int* pc = cc.data();
    for (int i = 0; i < size; ++i) {
      pc[i] = i;
    }

    int count = 0;
    for (int a = 0; a < 2; ++a) {
      for (int b = 0; b < 3; ++b) {
        for (int c = 0; c < 4; ++c) {
          for (int d = 0; d < 5; ++d) {
            REQUIRE(count == cc(start + d, start + c, start + b, start + a));
            REQUIRE(count == cc[a][b][c][d]);
            REQUIRE(count == cc.c(a, b, c, d));
            ++count;
          }
        }
      }
    }
  }
}
