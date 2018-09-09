# Introduction
This is a C++ wrapper that mimics the syntax of the multidimensional
Fortran array.
I felt it would cost me less time to write this wrapper correct than
+ to change the 1-based numbering Fortran index to 0-based C index;
+ to change the parentheses to multiple pairs of square brackets;
+ and to reverse the order of index numbers.

# Data Layout
One and two dimensional arrays are guaranteed to have the same data
layout as the corresponding `Eigen` types, e.g.
```C
fa::allocatable<double,1,1> ff;
auto& ee = ff.as<Eigen::MatrixX2d>();
ee.resize(3,42); // ff.allocate(3,42);
                 // or ff.resize(42,3); or ff.reserve(42,3);
ee(1,15) = M_PI; // ff.c(15,1) = M_PI;
                 // or ff[15][1] = M_PI;
                 // or ff(2,16) = M_PI;
ee.resize(0,0);  // ff.deallocate(); or ff.free();
```

# Examples
## DIMENSION
In Fortran
```Fortran
integer, dimension(3,5,7) :: a
integer, dimension(0:2,0:4,0:6) :: b
a(1,2,3) = 100
b(0,1,2) = 100
```
vs. in C/C++
```C
int a[7][5][3];
int b[7][5][3];
a[2][1][0] = 100;
b[2][1][0] = 100;
```
vs. using this wrapper
```C
using namespace fa;

dimension<int,3,5,7> af;
dimension<int,r(0,2),r(0,4),r(0,6)> bf;
// or dimension<int,r::_(0,3),r::_(0,5),r::_(0,7)> bf;
af(1,2,3) = 100;
bf(0,1,2) = 100;
af[2][1][0] = 100;
bf[2][1][0] = 100;
af.c(2,1,0) = 100;
bf.c(2,1,0) = 100;

tensor<int,r(1,7),r(1,5),r(1,3)> ac;
// or tensor<int,r::_(1,7),r::_(1,5),r::_(1,3)> ac;
tensor<int,7,5,3> bc;
ac(1,2,3) = 100;
bc(0,1,2) = 100;
ac[2][1][0] = 100;
bc[2][1][0] = 100;
ac.c(2,1,0) = 100;
bc.c(2,1,0) = 100;
```

## ALLOCATABLE
### API
Declare the allocatable type as follows:

| FortranArray | Fortran |  C++ | Explanation |
|--------------|---------|------|-------------|
| `allocatable<int,1,1,...> foo;` | `integer, allocatable :: foo(:,:,...)` | `vector<...vector<int>> foo;` | declare an n-dimensional dynamic array; when using Fortran syntax, assuming index starts from **1** |
| `allocatable<int,0,0,...> bar;` | `integer, allocatable :: bar(:,:,...)` | `vector<...vector<int>> bar;` | declare an n-dimensional dynamic array; when using Fortran syntax, assuming index starts from **0** |
| `foo.allocated();` | `ALLOCATED(foo)` | `foo.size();` | check availability |
| `foo.size();` | `SIZE(foo)` | *ditto* | **total number of elements** |
| `foo.deallocate();` | `DEALLOCATE(foo)` | `foo.clear();` | release the dynamic allocation |
| `foo.clear();` | *ditto* | *ditto* | *ditto* |
| `foo.allocate(d1,d2,...);` | `ALLOCATE(foo(d1,d2,...))` | `foo.reserve(...,d2,d1);` | dynamic allocation |
| `bar.allocate(d1,d2,...);` | `ALLOCATE(bar(0:d1-1,0:d2-1,...))` | `bar.reserve(...,d2,d1);` | dynamic allocation |
| `foo.reserve(...,d2,d1);` | *ditto* | *ditto* | *ditto* |
| `foo.reallocate(...,d2,d1);` | *unavailable* | `foo.resize(...,d2,d1);` | reallocate dynamic memory |
| `foo.resize(...,d2,d1);` | *unavailable* | *ditto* | *ditto* |
| `foo.data();` | `foo` | `foo.data();` | pointer to the first element |
| `foo.fill(42);` | `foo = 42` | `std::fill(foo.data(), foo.data()+foo.size(), 42);` | assign value 42 to every element |
| `foo.zero();` | `foo = 0` | `std::fill(foo.data(), foo.data()+foo.size(), 0);` | zero out all the elements |
| `foo.c_index(...,i2,i1);` | *unavailable* | *unavailable* | **0-based** index of the element "foo[...][i2][i1]" |
| `foo.fortran_index(j1,j2,...)` | *unavailable* | *unavailable* | **0-based** index of the element "foo(j1,j2,...)"; **the effect of different "beginning indexes" is taken into account** |
| `foo(j1,j2,...);` | `foo(j1,j2,...)` | `foo[...][i2][i1];` | access element "foo(j1,j2,...)" |
| `foo.c(...,i2,i1);` | `foo(j1,j2,...)` | `foo[...][i2][i1];` | access element "foo[...][i2][i1]" |
| `foo[...][i2][i1];` | `foo(j1,j2,...)` | `foo[...][i2][i1];` | access element "foo[...][i2][i1]"; **slow implementation; not recommended** |

### Example
In Fortran
```Fortran
integer d1,d2,d3
integer, allocatable :: a(:,:,:)
d1 = 3
d2 = 5
d3 = 7
if (.not. allocated(a)) then
   allocate (a(d1,d2,d3))
end if
a(1,2,3) = 100
deallocate (a)
```
vs. C
```C
int d1,d2,d3;
int* a = NULL;
d1 = 3;
d2 = 5;
d3 = 7;
if (!a) {
  // int a[7][5][3];
  a = (int*) malloc(d3*d2*d1*sizeof(int));
}
// a[2][1][0]
a[d3*d2*2+d2*1+0] = 100;
free(a);
a = NULL;
```
vs. C++
```C
int d1 = 3;
int d2 = 5;
int d3 = 7;
typedef std::vector<int> vc1;
typedef std::vector<vc1> vc2;
typedef std::vector<vc2> vc3;
// int a[7][5][3]
vc3 a(d3, vc2(d2, vc1(d1)));
a.resize(d3);
a[2][1][0] = 100;
```
vs. this wrapper
```C
using namespace fa;

int d1 = 3;
int d2 = 5;
int d3 = 7;
allocatable<int,1,1,1> af;
if (!af.allocated()) {
  af.allocate(d1,d2,d3);
}
af(1,2,3) = 100;
af[2][1][0] = 100;
af.c(2,1,0) = 100;
af.deallocate();

allocatable<int,0,0,0> ac;
if (!ac.size()) {
  ac.resize(d3,d2,d1);
}
ac(0,1,2) = 100;
ac[2][1][0] = 100;
ac.c(2,1,0) = 100;
ac.clear();
```
