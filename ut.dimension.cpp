#include "FortranArray.h"
#include <catch2/catch.hpp>
using namespace fa;

TEST_CASE("range tests", "[range]")
{
  SECTION("explicitly set ranges")
  {
    constexpr auto ra0 = r(0, 0);
    constexpr auto ra1 = r(0, 1);
    constexpr auto ra2 = r(3, 42);
    constexpr auto ra3 = r(-2, 100);

    REQUIRE(ra0.front() == 0);
    REQUIRE(ra1.front() == 0);
    REQUIRE(ra2.front() == 3);
    REQUIRE(ra3.front() == -2);

    REQUIRE(ra0.size() == 1);
    REQUIRE(ra1.size() == 2);
    REQUIRE(ra2.size() == 40);
    REQUIRE(ra3.size() == 103);

    constexpr auto rc0 = r(ra0.code());
    constexpr auto rc1 = r(ra1.code());
    constexpr auto rc2 = r(ra2.code());
    constexpr auto rc3 = r(ra3.code());

    REQUIRE(rc0.front() == 0);
    REQUIRE(rc1.front() == 0);
    REQUIRE(rc2.front() == 3);
    REQUIRE(rc3.front() == -2);

    REQUIRE(rc0.size() == 1);
    REQUIRE(rc1.size() == 2);
    REQUIRE(rc2.size() == 40);
    REQUIRE(rc3.size() == 103);

    constexpr auto rb0 = r(1, 0);
    constexpr auto rb1 = r(-1, 1);
    constexpr auto rb2 = r(1, 42);
    constexpr auto rb3 = r(-3, 100);

    constexpr auto rf0 = r(rb0.code());
    constexpr auto rf1 = r(rb1.code());
    constexpr auto rf2 = r(rb2.code());
    constexpr auto rf3 = r(rb3.code());

    REQUIRE(rf0.front() == 1);
    REQUIRE(rf1.front() == -1);
    REQUIRE(rf2.front() == 1);
    REQUIRE(rf3.front() == -3);

    REQUIRE(rf0.size() == 0);
    REQUIRE(rf1.size() == 3);
    REQUIRE(rf2.size() == 42);
    REQUIRE(rf3.size() == 104);
  }

  SECTION("C/C++ stype default ranges")
  {
    constexpr r rc0 = r::_0(0);
    constexpr r rc1 = r::_0(1);
    constexpr r rc2 = r::_0(42);
    constexpr r rc3 = r::_0(100);

    REQUIRE(rc0.front() == 0);
    REQUIRE(rc1.front() == 0);
    REQUIRE(rc2.front() == 0);
    REQUIRE(rc3.front() == 0);

    REQUIRE(rc0.size() == 0);
    REQUIRE(rc1.size() == 1);
    REQUIRE(rc2.size() == 42);
    REQUIRE(rc3.size() == 100);

    constexpr auto rr0 = r(rc0);
    constexpr auto rr1 = r(rc1);
    constexpr auto rr2 = r(rc2);
    constexpr auto rr3 = r(rc3);

    REQUIRE(rr0.front() == rc0.front());
    REQUIRE(rr1.front() == rc1.front());
    REQUIRE(rr2.front() == rc2.front());
    REQUIRE(rr3.front() == rc3.front());

    REQUIRE(rr0.size() == rc0.size());
    REQUIRE(rr1.size() == rc1.size());
    REQUIRE(rr2.size() == rc2.size());
    REQUIRE(rr3.size() == rc3.size());
  }

  SECTION("Fortran style default ranges")
  {
    constexpr r rf0 = r::_1(0);
    constexpr r rf1 = r::_1(1);
    constexpr r rf2 = r::_1(42);
    constexpr r rf3 = r::_1(100);

    REQUIRE(rf0.front() == 1);
    REQUIRE(rf1.front() == 1);
    REQUIRE(rf2.front() == 1);
    REQUIRE(rf3.front() == 1);

    REQUIRE(rf0.size() == 0);
    REQUIRE(rf1.size() == 1);
    REQUIRE(rf2.size() == 42);
    REQUIRE(rf3.size() == 100);

    constexpr auto rr0 = r(rf0);
    constexpr auto rr1 = r(rf1);
    constexpr auto rr2 = r(rf2);
    constexpr auto rr3 = r(rf3);

    REQUIRE(rr0.front() == rf0.front());
    REQUIRE(rr1.front() == rf1.front());
    REQUIRE(rr2.front() == rf2.front());
    REQUIRE(rr3.front() == rf3.front());

    REQUIRE(rr0.size() == rf0.size());
    REQUIRE(rr1.size() == rf1.size());
    REQUIRE(rr2.size() == rf2.size());
    REQUIRE(rr3.size() == rf3.size());
  }

  SECTION("[front, back] and size")
  {
    constexpr auto size = 42;
    constexpr auto st1 = 0;
    constexpr auto rf1 = r::_(st1, size);
    constexpr auto st2 = 7;
    constexpr auto rf2 = r::_(st2, size);
    constexpr auto st3 = -4;
    constexpr auto rf3 = r::_(st3, size);

    REQUIRE(rf1.front() == st1);
    REQUIRE(rf2.front() == st2);
    REQUIRE(rf3.front() == st3);

    REQUIRE(rf1.size() == size);
    REQUIRE(rf2.size() == size);
    REQUIRE(rf3.size() == size);
  }
}

TEST_CASE("dimension with different initial index", "[dimension]")
{
  SECTION("1 dimensional starts from 0")
  {
    constexpr auto start = 0;
    constexpr auto rf1 = r::_(start, 2);
    REQUIRE(rf1.front() == start);

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

  SECTION("1 dimensional starts from 3")
  {
    constexpr auto start = 3;
    constexpr auto rf1 = r::_(start, 2);

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

  SECTION("1 dimensional starts from -3")
  {
    constexpr auto start = -3;
    constexpr auto rf1 = r::_(start, 2);

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
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);

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

  SECTION("2 dimensional starts from 3")
  {
    constexpr auto start = 3;
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);

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
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);

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
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);
    constexpr auto rf3 = r::_(start, 4);
    constexpr auto rf4 = r::_(start, 5);

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

  SECTION("4 dimensional starts from 2")
  {
    constexpr auto start = 2;
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);
    constexpr auto rf3 = r::_(start, 4);
    constexpr auto rf4 = r::_(start, 5);

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
    constexpr auto rf1 = r::_(start, 2);
    constexpr auto rf2 = r::_(start, 3);
    constexpr auto rf3 = r::_(start, 4);
    constexpr auto rf4 = r::_(start, 5);

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

  SECTION("1 dimensional starting from 3")
  {
    constexpr int start = 3;
    constexpr int size = 2;
    tensor<int, r::_(start, 2)> cc;

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
    tensor<int, r::_(start, 2), r(start, -2)> cc;
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

  SECTION("4 dimensional starting from 2")
  {
    constexpr int start = 2;
    constexpr int size = 120;

    tensor<int, r::_(start, 2), r::_(start, 3), r(2, 5), r(2, 6)> cc;
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
