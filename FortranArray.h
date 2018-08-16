// -*- C++ -*-

#ifndef FortranArray_h
#define FortranArray_h

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>

/*
    range synopsis

namespace fa { namespace detail_dim
{
struct range;
}

typedef detail_dim::range r;

namespace detail_dim
{
struct range
{
  // coded range type
  using code_t = implementation_dependent_type_;

  // signed index type
  using index_t = signed_implementation_dependent_type_;

  // constructors
  constexpr range(code_t);
  constexpr range(int, int);

  // static constructors
  static constexpr range _(int, unsigned int);
  static constexpr code_t _0(code_t);
  static constexpr code_t _1(code_t);

  // properties
  constexpr index_t front() const;
  constexpr index_t size() const;
  constexpr code_t code() const;

  // conversion
  constexpr operator code_t() const;
};

using index_t = range::index_t;
}} // namespace

    tensor, dimension, and fdms synopsis

namespace fa { namespace detail_dim
{
template<char FC, class T, range::code_t... NN>
struct fdms;
} // namespace

template<class T, range::code_t... NN>
class tensor : public detail_dim::fdms<'c', T, NN...> {};

template<class T, range::code_t... NN>
class dimension : public detail_dim::fdms<'f', T, NN...> {};
}

namespace fa { namespace detail_dim
{
template<char FC, class T, range::code_t... NN>
struct fdms
{
  // index
  template<class... SS> static index_t c_index(SS...);
  template<class... SS> static index_t fortran_index(SS...);

  // array type
  const type& array() const;
  type& array();

  // cast
  template<class V> const V& as() const;
  template<class V> V& as();

  // fill and zero
  void fill(T);
  void zero;

  // c/c++
  static constexpr index_t size();
  const T* data() const;
  T* data();
  template<class... SS> const T& c(SS...) const;
  template<class... SS> T& c(SS...);
  const base_t& operator[](index_t) const;
  base_t& operator[](index_t);

  // fortran
  template<class... SS> const T& operator()(SS...) const;
  template<class... SS> T& operator()(SS...);
};
}} // namespace

    allocatable synopsis

namespace fa
{
template<class T, int... BEGINS>
struct allocatable
{
  // constructors and destructor
  allocatable();
  ~allocatable();
  allocatable(const allocatable&) = delete;
  allocatable& operator=(const allocatable&) = delete;
  allocatable(allocatable&&) = delete;
  allocatable& operator=(allocatable&&) = delete;

  // index
  template<class... SS> index_t c_index(SS...) const;
  template<class... SS> index_t fortran_index(SS...) const;

  // cast
  template<class V> const V& as() const;
  template<class V> V& as();

  // fill and zero
  void fill(T);
  void zero();

  // c++
  index_t size() const;
  const T* data() const;
  T* data() const;
  void clear();
  template<class... SS> void reserve(SS...);
  template<class... SS> void resize(SS...);
  template<class... SS> const T& c(SS...) const;
  template<class... SS> T& c(SS...);
  const_base_t operator[](index_t) const;
  base_t operator[](index_t);

  // fortran
  bool allocated() const;
  void deallocate();
  template<class... SS> void allocate(SS...);
  template<class... SS> void reallocate(SS...);
  template<class... SS> const T& operator()(SS...) const;
  template<class... SS> T& operator()(SS...);
};
} // namespace

*/

#if __cplusplus < 201103L
#error "c++11 is required."
#endif

namespace fa {
namespace detail_dim {
/**
 * @brief coded range: [front, front + size)
 */
class range {
private:
  // "class T" is necessary to put partial specialization in the class scope
  template<class T, int TOTALBITS>
  struct type_def1;

  template<class T>
  struct type_def1<T, 32> {
    using stype = std::uint32_t;
    using itype = int;
    using utype = unsigned int;
    using idxtype = std::int32_t;

    static constexpr int e_bits = 1;
    static constexpr int f_bits = 3;
    static constexpr int start_bits = 4;
    static constexpr int size_bits = 28;
    static_assert(e_bits + f_bits == start_bits, "");

    static constexpr int min_start = -4; // FFFFFFF 1 100
    static constexpr int max_start = 3;  // 0000000 0 011
    // max(idxtype) must >= max_size
    static constexpr unsigned int max_size = 0x0FFFFFFF;
  };

  template<class T>
  struct type_def1<T, 64> {
    using stype = std::uint64_t;
    using itype = int;
    using utype = unsigned int;
    using idxtype = std::int64_t;

    static constexpr int e_bits = 1;
    static constexpr int f_bits = 31;
    static constexpr int start_bits = 32;
    static constexpr int size_bits = 32;
    static_assert(e_bits + f_bits == start_bits, "");

    static constexpr int min_start = 0xC0000000; // 1 100 0000000
    static constexpr int max_start = 0x3FFFFFFF; // 0 011 FFFFFFF
    // max(idxtype) must >= max_size
    static constexpr unsigned int max_size = 0x7FFFFFFF;
  };

  template<int TOTALBITS>
  struct type_defs : public type_def1<int, TOTALBITS> {
    using base_t = type_def1<int, TOTALBITS>;

    using stype = typename base_t::stype;
    using itype = typename base_t::itype;
    using utype = typename base_t::utype;

    unsigned int explicit_ : base_t::e_bits;
    itype start_ : base_t::f_bits;
    utype size_ : base_t::size_bits;

    static constexpr itype check_front_(int ifront)
    {
#ifdef NDEBUG
      return ifront;
#else
      return (base_t::min_start <= ifront && ifront <= base_t::max_start)
        ? ifront
        : throw std::out_of_range("ifront is out of range.");
#endif
    }

    static constexpr utype check_size_(int isize)
    {
#ifdef NDEBUG
      return isize;
#else
      return (0 <= isize && isize <= base_t::max_size)
        ? isize
        : throw std::out_of_range("isize is out of range.");
#endif
    }

    static constexpr unsigned int code_if_explicit_(stype icode)
    {
      return icode >> (base_t::f_bits + base_t::size_bits);
    }

    static constexpr itype code_to_front_(stype icode)
    {
      return icode << base_t::e_bits >> (base_t::e_bits + base_t::size_bits);
    }

    static constexpr utype code_to_size_(stype icode)
    {
      return icode << base_t::start_bits >> base_t::start_bits;
    }

    static constexpr stype to_code_(stype iexplicit, stype ifront, utype isize)
    {
      return (iexplicit << (base_t::f_bits + base_t::size_bits))
        + (ifront << (base_t::e_bits + base_t::size_bits) >> base_t::e_bits)
        + isize;
    }

    constexpr stype to_code_() const
    {
      return to_code_(explicit_, start_, size_);
    }

    template<itype N>
    static constexpr stype n_view_(stype icode)
    {
      return code_if_explicit_(icode)
        ? icode
        : type_defs(1, N, code_to_size_(icode)).to_code_();
    }

    constexpr type_defs(stype icode)
      : explicit_(code_if_explicit_(icode))
      , start_(code_to_front_(icode))
      , size_(code_to_size_(icode))
    {}

    constexpr type_defs(int ifront, int iback)
      : explicit_(1)
      , start_(check_front_(ifront))
      , size_(check_size_(iback - ifront + 1))
    {}

    constexpr type_defs(int iexplicit, int ifront, int isize)
      : explicit_(iexplicit)
      , start_(ifront)
      , size_(isize)
    {}
  };

#ifdef EIGEN_DEFAULT_DENSE_INDEX_TYPE
  static constexpr size_t nbit_ = 8 * sizeof(EIGEN_DEFAULT_DENSE_INDEX_TYPE);
  static_assert(
    sizeof(type_defs<nbit_>) == sizeof(EIGEN_DEFAULT_DENSE_INDEX_TYPE), "");
#else
  // default platform information
  static constexpr size_t nbit_ = 8 * sizeof(size_t);
  static_assert(sizeof(type_defs<nbit_>) == sizeof(size_t), "");
#endif

  using type = type_defs<nbit_>;
  static_assert(std::is_signed<type::idxtype>::value, "");

  type m_;

public:
  /**
   * @brief type of the coded range
   */
  using code_t = type::stype;

  /**
   * @brief type of the index
   */
  using index_t = type::idxtype;

  /**
   * @brief constructs range object from the coded range
   */
  constexpr range(code_t icode)
    : m_(icode)
  {}

  /**
   * @brief constructs range object from the inclusive [front, back] range
   */
  constexpr range(int ifront, int iback)
    : m_(ifront, iback)
  {}

  /**
   * @brief constructs range object from the [front, front + size) range
   */
  static constexpr range _(int ifront, unsigned int isize)
  {
    return range(ifront, ifront + isize - 1);
  }

  /**
   * @brief returns this range code; or the range code for [0, icode - 1]
   */
  static constexpr code_t _0(code_t icode)
  {
    return type::n_view_<0>(icode);
  }

  /**
   * @brief returns this range code; or the range code for [1, icode]
   */
  static constexpr code_t _1(code_t icode)
  {
    return type::n_view_<1>(icode);
  }

  /**
   * @brief returns the front index
   */
  constexpr index_t front() const
  {
    return m_.start_;
  }

  /**
   * @brief returns the size/length of the range
   */
  constexpr index_t size() const
  {
    return m_.size_;
  }

  /**
   * @brief returns the coded [front, front + size) range
   */
  constexpr code_t code() const
  {
    return m_.to_code_();
  }

  /**
   * @brief implicit conversion to code_t type
   */
  constexpr operator code_t() const
  {
    return code();
  }
};
} // namespace detail_dim
} // namespace fa

namespace fa {
using r = detail_dim::range;
using index_t = r::index_t;

namespace detail_dim {
/**
 * @brief compile-time sanity check
 */
///@{
template<char FC>
struct sanity {
  static_assert(FC == 'F' || FC == 'f' || FC == 'C' || FC == 'c', "");
};

template<>
struct sanity<'F'> {
  static constexpr char fc = 'f';
};

template<>
struct sanity<'f'> {
  static constexpr char fc = 'f';
};

template<>
struct sanity<'C'> {
  static constexpr char fc = 'c';
};

template<>
struct sanity<'c'> {
  static constexpr char fc = 'c';
};
///@}

/**
 * @brief compile-time product of the coded range size
 */
///@{
template<range::code_t... NN>
struct P;

template<range::code_t N>
struct P<N> {
  static constexpr index_t prod = range(N).size();
};

template<range::code_t N, range::code_t... NN>
struct P<N, NN...> {
  static constexpr index_t prod = range(N).size() * P<NN...>::prod;
};
///@}

/**
 * @brief c/c++ style index
 */
template<char FC, range::code_t... NN>
struct C1;

/**
 * @brief [A][B][C] or dimension(C, B, A)
 *        via c(x, y, z) -> xBC + yC + z
 */
///@{
template<range::code_t N>
struct C1<'c', N> {
  template<class S>
  static index_t index(S s)
  {
    return s;
  }
};

template<range::code_t N, range::code_t... NN>
struct C1<'c', N, NN...> {
  template<class S, class... SS>
  static index_t index(S s, SS... ss)
  {
    return s * P<NN...>::prod + C1<'c', NN...>::index(ss...);
  }
};
///@}

/**
 * @brief dimension(A, B, C) or [C][B][A]
 *        via c(x, y, z) -> xAB + yA + z
 */
///@{
template<range::code_t... NN>
struct C1<'f', NN...> {
private:
  template<index_t S, range::code_t... MM>
  struct E;

  template<range::code_t... MM>
  struct E<1, MM...> {
    static constexpr index_t prod = 1;
  };

  template<range::code_t M, range::code_t... MM>
  struct E<1, M, MM...> {
    static constexpr index_t prod = 1;
  };

  template<index_t S, range::code_t M, range::code_t... MM>
  struct E<S, M, MM...> {
    static constexpr index_t prod = range(M).size() * E<S - 1, MM...>::prod;
  };

public:
  template<class S>
  static index_t index(S s)
  {
    return s;
  }

  template<class S, class... SS>
  static index_t index(S s, SS... ss)
  {
    constexpr index_t sz = 1 + sizeof...(SS);
    return s * E<sz, NN...>::prod + index(ss...);
  }
};
///@}

/**
 * fortran style index
 */
template<range::code_t... NN>
struct D1;

/**
 * @brief [A][B][C] or dimension(C, B, A)
 *        via f(x, y, z) -> (x - 1) + (y - 1)C + (z - 1)BC
 */
///@{
template<range::code_t N>
struct D1<'c', N> {
  template<class S>
  static index_t index(S s)
  {
    return s - range(N).front();
  }
};

template<range::code_t N, range::code_t... NN>
struct D1<'c', N, NN...> {
private:
  static constexpr index_t NUMER_ = P<N, NN...>::prod;

  template<index_t S, range::code_t... MM>
  struct E;

  template<range::code_t... MM>
  struct E<0, MM...> {
    static constexpr index_t denom = 1;
  };

  template<range::code_t M, range::code_t... MM>
  struct E<0, M, MM...> {
    static constexpr index_t denom = 1;
  };

  template<index_t S, range::code_t M, range::code_t... MM>
  struct E<S, M, MM...> {
    static constexpr index_t denom = range(M).size() * E<S - 1, MM...>::denom;
  };

public:
  template<class S>
  static index_t index(S s)
  {
    constexpr index_t prod = NUMER_ / E<1, N, NN...>::denom;
    return (s - range(N).front()) * prod;
  }

  template<class S, class... SS>
  static index_t index(S s, SS... ss)
  {
    constexpr index_t prod = NUMER_ / E<1 + sizeof...(SS), N, NN...>::denom;
    return (s - range(N).front()) * prod + index(ss...);
  }
};
///@}

/**
 * @brief dimension(A, B, C) or [C][B][A]
 *        via f(x, y, z) -> (x - 1) + A((y - 1) + B(z - 1))
 */
///@{
template<range::code_t N>
struct D1<'f', N> {
  template<class S>
  static index_t index(S s)
  {
    return s - range(N).front();
  }
};

template<range::code_t N, range::code_t... NN>
struct D1<'f', N, NN...> {
  template<class S, class... SS>
  static index_t index(S s, SS... ss)
  {
    return s - range(N).front()
      + range(N).size() * D1<'f', NN...>::index(ss...);
  }
};
///@}

/**
 * @brief type traits of the dimension array
 */
///@{
template<char FC, class T, range::code_t... NN>
struct traits;

template<class T, range::code_t N>
struct traits<'c', T, N> {
  using type = T[range(N).size()];
  using decay = typename std::decay<type>::type;
  using base = typename std::remove_pointer<decay>::type;
};

template<class T, range::code_t N, range::code_t... NN>
struct traits<'c', T, N, NN...> {
  using type = typename traits<'c', T, NN...>::type[range(N).size()];
  using decay = typename std::decay<type>::type;
  using base = typename std::remove_pointer<decay>::type;
};

template<class T, range::code_t N>
struct traits<'f', T, N> {
  using type = T[range(N).size()];
  using decay = typename std::decay<type>::type;
  using base = typename std::remove_pointer<decay>::type;
};

template<class T, range::code_t N, range::code_t... NN>
struct traits<'f', T, N, NN...> {
  using type =
    typename traits<'f', typename traits<'f', T, N>::type, NN...>::type;
  using decay = typename std::decay<type>::type;
  using base = typename std::remove_pointer<decay>::type;
};
///@}

/**
 * @brief analog of fortran dimension and c/c++ array
 * @details Example: 24 elements in 3 dimensions declared in c/c++ or fortran
 * @code
 * int arr1[2][3][4];                 // c/c++
 * integer, dimension(4,3,2) :: arr2 !// fortran
 *
 * fdms<'c', int, 2, 3, 4> arr_c; // c/c++ style
 * fdms<'f', int, 4, 3, 2> arr_f; // fortran style
 * @endcode
 *
 * @tparam FC  using fortran style ('f' or 'F') or
 *             c/c++ style ('c' or 'C') declaration
 * @tparam T   type of the element
 * @tparam NN  coded ranges for each dimension; if directly giving an integer,
 *             using 0-based numbering by default
 *
 * @see range
 */
template<char FC, class T, range::code_t... NN>
class fdms {
private:
  static constexpr char fc_ = sanity<FC>::fc;

  // underlying type of c array; e.g. int [2][3][4]
  using type = typename traits<fc_, T, NN...>::type;

  // underlying base type of c array; e.g. int [3][4]
  using base_t = typename traits<fc_, T, NN...>::base;

  type data_;

public:
  /**
   * @brief 0-based array index following c/c++ style
   */
  template<class... SS>
  static index_t c_index(SS... ss)
  {
    static_assert(sizeof...(SS) == sizeof...(NN), "");
    return C1<FC, NN...>::index(ss...);
  }

  /**
   * @brief x-based array index following fortran style;
   *        x must be specified explicitly unless it equals 0
   */
  template<class... SS>
  static index_t fortran_index(SS... ss)
  {
    static_assert(sizeof...(SS) == sizeof...(NN), "");
    return D1<FC, NN...>::index(ss...);
  }

  /**
   * @brief returns the (const) reference to the underlying c/c++ array
   */
  ///@{
  const type& array() const
  {
    return data_;
  }

  type& array()
  {
    return data_;
  }
  ///@}

  /**
   * @brief cast to another type
   */
  ///@{
  template<class V>
  const V& as() const
  {
    static_assert(sizeof(*this) == sizeof(V), "");
    return *reinterpret_cast<const V*>(this);
  }

  template<class V>
  V& as()
  {
    static_assert(sizeof(*this) == sizeof(V), "");
    return *reinterpret_cast<V*>(this);
  }
  ///@}

  /**
   * @brief fill all the elements with the same value
   */
  ///@{
  void fill(T t)
  {
    std::uninitialized_fill_n(data(), size(), t);
  }

  void zero()
  {
    fill((T)0);
  }
  ///@}

  // c/c++ style

  /**
   * @brief returns total number of elements
   */
  static constexpr index_t size()
  {
    return P<NN...>::prod;
  }

  /**
   * @brief returns the (const) pointer to the first element
   */
  ///@{
  const T* data() const
  {
    return reinterpret_cast<const T*>(&data_);
  }

  T* data()
  {
    return reinterpret_cast<T*>(&data_);
  }
  ///@}

  /**
   * @brief returns the (const) reference to the element following the
   *        c/c++ style index
   */
  ///@{
  template<class... SS>
  const T& c(SS... ss) const
  {
    return data()[c_index(ss...)];
  }

  template<class... SS>
  T& c(SS... ss)
  {
    return data()[c_index(ss...)];
  }
  ///@}

  /**
   * @brief does the same job as the c/c++ operator[] of arrays
   */
  ///@{
  const base_t& operator[](index_t index) const
  {
    return data_[index];
  }

  base_t& operator[](index_t index)
  {
    return data_[index];
  }
  ///@}

  // fortran style

  /**
   * @brief returns the (const) reference to the element following the
   *        fortran style index
   */
  ///@{
  template<class... SS>
  const T& operator()(SS... ss) const
  {
    return data()[fortran_index(ss...)];
  }

  template<class... SS>
  T& operator()(SS... ss)
  {
    return data()[fortran_index(ss...)];
  }
  ///@}
};
} // namespace detail_dim

/**
 * @brief c/c++ array analog
 */
template<class T, r::code_t... NN>
class tensor : public detail_dim::fdms<'c', T, r::_0(NN)...> {};

/**
 * @brief fortran array analog
 */
template<class T, r::code_t... NN>
class dimension : public detail_dim::fdms<'f', T, r::_1(NN)...> {};

namespace detail_alcb {
/**
 * @brief filling an array with the given variadic arguments
 */
///@{
template<index_t N, class... MM>
struct fill_dims_impl;

template<index_t N, class M>
struct fill_dims_impl<N, M> {
  static_assert(N == 1, "");
  static void exec(index_t* parr, M m)
  {
    parr[0] = m;
  }
};

template<index_t N, class M, class... MM>
struct fill_dims_impl<N, M, MM...> {
  static_assert(N == 1 + sizeof...(MM), "");
  static void exec(index_t* parr, M m, MM... mm)
  {
    parr[0] = m;
    fill_dims_impl<N - 1, MM...>::exec(parr + 1, mm...);
  }
};
///@}

/**
 * @brief reversely filling an array with the given variadic arguments;
 *        the pointer to the last element of the array is provided
 */
///@{
template<index_t N, class... MM>
struct rev_fill_dims_impl;

template<index_t N, class M>
struct rev_fill_dims_impl<N, M> {
  static_assert(N == 1, "");
  static void exec(index_t* pback, M m)
  {
    pback[0] = m;
  }
};

template<index_t N, class M, class... MM>
struct rev_fill_dims_impl<N, M, MM...> {
  static_assert(N == 1 + sizeof...(MM), "");
  static void exec(index_t* pback, M m, MM... mm)
  {
    pback[0] = m;
    rev_fill_dims_impl<N - 1, MM...>::exec(pback - 1, mm...);
  }
};
///@}

/**
 * @brief storing the dimensions provided by the variadic arguments following
 *        the fortran convention; or reversely storing them if following the
 *        c/c++ style
 */
///@{
template<char FC, index_t N, class... MM>
struct copy_dims;

template<index_t N, class... MM>
struct copy_dims<'c', N, MM...> {
  static void exec(std::array<index_t, N>& arr, MM... mm)
  {
    rev_fill_dims_impl<N, MM...>::exec(&arr[N - 1], mm...);
  }
};

template<index_t N, class... MM>
struct copy_dims<'f', N, MM...> {
  static void exec(std::array<index_t, N>& arr, MM... mm)
  {
    fill_dims_impl<N, MM...>::exec(&arr[0], mm...);
  }
};
///@}

/**
 * @brief dimension(A, B, C) via c(x, y, z) -> xAB + yA + z
 */
///@{
template<int... BB>
struct G {
private:
  template<index_t S, int... MM>
  struct E;

  template<int... MM>
  struct E<1, MM...> {
    static index_t prod(const index_t*)
    {
      return 1;
    }
  };

  template<int M, int... MM>
  struct E<1, M, MM...> {
    static index_t prod(const index_t*)
    {
      return 1;
    }
  };

  template<index_t S, int M, int... MM>
  struct E<S, M, MM...> {
    static index_t prod(const index_t* parr)
    {
      return parr[0] * E<S - 1, MM...>::prod(parr + 1);
    }
  };

public:
  template<class S>
  static index_t index(const index_t*, S s)
  {
    return s;
  }

  template<class S, class... SS>
  static index_t index(const index_t* parr, S s, SS... ss)
  {
    constexpr index_t sz = 1 + sizeof...(SS);
    return s * E<sz, BB...>::prod(parr) + index(parr, ss...);
  }
};
///@}

/**
 * @brief dimension(A, B, C) via f(x, y, z) -> (x - 1) + A((y - 1) + B(z - 1))
 */
///@{
template<int... BB>
struct H;

template<int B>
struct H<B> {
  template<class S>
  static index_t index(const index_t*, S s)
  {
    return s - B;
  }
};

template<int B, int... BB>
struct H<B, BB...> {
  template<class S, class... SS>
  static index_t index(const index_t* parr, S s, SS... ss)
  {
    return s - B + parr[0] * H<BB...>::index(parr + 1, ss...);
  }
};
///@}

/**
 * @brief      allocatable data
 * @tparam T   type of the element
 * @tparam BB  x value for x-based numbering in each dimension
 */
template<class T, int... BB>
struct ad {
  static constexpr index_t N_ = sizeof...(BB);
  T* data_;
  std::array<index_t, N_> dims_;

  ad()
    : data_(nullptr)
    , dims_()
  {
    dims_.fill(0);
  }

  ~ad()
  {
    deallocate();
  }

  void deallocate()
  {
    delete[] data_;
    data_ = nullptr;
    dims_.fill(0);
  }

  bool allocated() const
  {
    return data_ != nullptr;
  }

  index_t size() const
  {
    index_t prod = 1;
    for (index_t i = 0; i < N_; ++i) {
      prod *= dims_[i];
    }
    return prod;
  }

  template<char FC, class... SS>
  void reserve_impl(SS... ss)
  {
    assert(allocated() == false);
    copy_dims<detail_dim::sanity<FC>::fc, sizeof...(BB), SS...>::exec(
      dims_, ss...);
    data_ = new T[size()];
  }
};

/**
 * @brief      base type of allocatable data
 * @tparam T   type of the element
 * @tparam BB  x value for x-based numbering in each dimension
 */
///@{
template<class T, int... BB>
struct ad_base;

template<class T, int B>
struct ad_base<T, B> {
  using type = T&;
  using const_type = const T&;
};

template<class T, int B, int BB>
struct ad_base<T, B, BB> {
  struct type {
    T* data_;
    const index_t* back_;

    type() {}
    type(const ad<T, B, BB>& a, index_t index)
      : back_()
      , data_(a.data_ + index * a.dims_[0])
    {}

    const T& operator[](index_t index) const
    {
      return data_[index];
    }

    T& operator[](index_t index)
    {
      return data_[index];
    }
  };

  using const_type = type;
};

template<class T, int B, int... BB>
struct ad_base<T, B, BB...> {
  struct type {
    static constexpr index_t N_ = 1 + sizeof...(BB);
    T* data_;
    const index_t* back_;

    /**
     * @brief        returns the product of n index_t integers, starting
     *               reversely from arr[pback]
     * @details      Example: {5, 4, 3, 2} with *pback = 3 and n = 2, returns 12
     * @param pback  pointer to the last element of the array to be included
     * @param n      number of elements to be included in an array
     */
    static index_t rev_prod(const index_t* pback, index_t n)
    {
      index_t prod = 1;
      // must use signed type for i here
      static_assert(std::is_signed<index_t>::value, "");
      for (index_t i = 0; i < n; ++i) {
        prod *= pback[-i];
      }
      return prod;
    }

    type() {}
    type(const ad<T, B, BB...>& a, index_t index)
      : data_(a.data_ + index * rev_prod(&a.dims_[N_ - 2], N_ - 1))
      , back_(&a.dims_[N_ - 2])
    {}

    typename ad_base<T, BB...>::type operator[](index_t index) const
    {
      typename ad_base<T, BB...>::type dp;
      dp.back_ = back_ - 1;
      dp.data_ = data_ + index * rev_prod(dp.back_, N_ - 2);
      return dp;
    }
  };

  using const_type = type;
};
///@}

/**
 * @brief allocatable implementation
 */
///@{
template<class T, int... BB>
struct aimpl;

template<class T, int B>
struct aimpl<T, B> : public ad<T, B> {
  using base_t = typename ad_base<T, B>::type;
  using const_base_t = typename ad_base<T, B>::const_type;

  index_t c_index(index_t index) const
  {
    return index;
  }

  index_t fortran_index(index_t index) const
  {
    return index - B;
  }

  const_base_t operator[](index_t index) const
  {
    return ad<T, B>::data_[index];
  }

  base_t operator[](index_t index)
  {
    return ad<T, B>::data_[index];
  }
};

template<class T, int B, int... BB>
struct aimpl<T, B, BB...> : public ad<T, B, BB...> {
  using base_t = typename ad_base<T, B, BB...>::type;
  using const_base_t = typename ad_base<T, B, BB...>::const_type;

  template<class... SS>
  index_t c_index(SS... ss) const
  {
    return G<B, BB...>::index(&ad<T, B, BB...>::dims_[0], ss...);
  }

  template<class... SS>
  index_t fortran_index(SS... ss) const
  {
    return H<B, BB...>::index(&ad<T, B, BB...>::dims_[0], ss...);
  }

  const_base_t operator[](index_t index) const
  {
    return const_base_t(*this, index);
  }

  base_t operator[](index_t index)
  {
    return base_t(*this, index);
  }
};
///@}
} // namespace detail_alcb

/**
 * @brief fortran allocatable analog
 */
template<class T, int... BEGINS>
class allocatable : private detail_alcb::aimpl<T, BEGINS...> {
private:
  using impl_t = detail_alcb::aimpl<T, BEGINS...>;

public:
  allocatable()
    : impl_t()
  {}
  ~allocatable() {}
  allocatable(const allocatable&) = delete;
  allocatable& operator=(const allocatable&) = delete;
  allocatable(allocatable&&) = delete;
  allocatable& operator=(allocatable&&) = delete;

  /**
   * @brief 0-based array index following c/c++ convention
   */
  template<class... SS>
  index_t c_index(SS... ss) const
  {
    return impl_t::c_index(ss...);
  }

  /**
   * @brief x-based array index following fortran convention
   */
  template<class... SS>
  index_t fortran_index(SS... ss) const
  {
    return impl_t::fortran_index(ss...);
  }

  /**
   * @brief cast to another type
   */
  ///@{
  template<class V>
  const V& as() const
  {
    static_assert(sizeof(*this) == sizeof(V), "");
    return *reinterpret_cast<const V*>(this);
  }

  template<class V>
  V& as()
  {
    static_assert(sizeof(*this) == sizeof(V), "");
    return *reinterpret_cast<V*>(this);
  }
  ///@}

  /**
   * @brief fill all the elements with the same value
   */
  ///@{
  void fill(T t)
  {
    std::uninitialized_fill_n(data(), size(), t);
  }

  void zero()
  {
    fill((T)0);
  }
  ///@}

  // c++

  /**
   * @brief returns total number of elements
   */
  index_t size() const
  {
    return impl_t::size();
  }

  /**
   * @brief returns the (const) pointer to the first element
   */
  ///@{
  const T* data() const
  {
    return impl_t::data_;
  }

  T* data()
  {
    return impl_t::data_;
  }
  ///@}

  /**
   * @brief dynamic deallocation;
   *        should be safe to call even if the memory is unallocated
   */
  void clear()
  {
    impl_t::deallocate();
  }

  /**
   * @brief dynamic allocation following c/c++ convention, assuming unallocated
   */
  template<class... SS>
  void reserve(SS... ss)
  {
    impl_t::template reserve_impl<'c'>(ss...);
  }

  /**
   * @brief dynamic allocation following c/c++ convention, assuming allocated;
   *        should be safe to call even if the memory is unallocated
   */
  template<class... SS>
  void resize(SS... ss)
  {
    clear();
    reserve(ss...);
  }

  /**
   * @brief returns the (const) reference to the element following the
   *        c/c++ style index
   */
  ///@{
  template<class... SS>
  const T& c(SS... ss) const
  {
    return data()[c_index(ss...)];
  }

  template<class... SS>
  T& c(SS... ss)
  {
    return data()[c_index(ss...)];
  }
  ///@}

  /**
   * @brief does the same job as the c/c++ operator[] of arrays
   */
  ///@{
  using const_base_t = typename impl_t::const_base_t;
  using base_t = typename impl_t::base_t;

  const_base_t operator[](index_t index) const
  {
    return impl_t::operator[](index);
  }

  base_t operator[](index_t index)
  {
    return impl_t::operator[](index);
  }
  ///@}

  // fortran style

  /**
   * @brief works as the fortran 'allocated()' check
   */
  bool allocated() const
  {
    return impl_t::allocated();
  }

  /**
   * @brief dynamic deallocation;
   *        should be safe to call even if the memory is unallocated
   */
  void deallocate()
  {
    impl_t::deallocate();
  }

  /**
   * @brief dynamic allocation following fortran convention,
   *        assuming unallocated;
   */
  template<class... SS>
  void allocate(SS... ss)
  {
    impl_t::template reserve_impl<'f'>(ss...);
  }

  /**
   * @brief dynamic allocation following fortran convention,
   *        assuming allocated;
   *        should be safe to call even if the memory is unallocated
   */
  template<class... SS>
  void reallocate(SS... ss)
  {
    deallocate();
    allocate(ss...);
  }

  /**
   * @brief returns the (const) reference to the element following the
   *        fortran style index
   */
  ///@{
  template<class... SS>
  const T& operator()(SS... ss) const
  {
    return data()[fortran_index(ss...)];
  }

  template<class... SS>
  T& operator()(SS... ss)
  {
    return data()[fortran_index(ss...)];
  }
  ///@}
};
} // namespace fa

#endif /* FortranArray_h */
