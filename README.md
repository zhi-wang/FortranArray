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
## dimension
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

dimension<int,range(1,3),range(1,5),range(1,7)> af;
dimension<int,3,5,7> bf;
// or dimension<int,range(0,2),range(0,4),range(0,6)> bf;
// or dimension<int,range::_(0,3),range::_(0,5),range::_(0,7)> bf;
af(1,2,3) = 100;
bf(0,1,2) = 100;
af[2][1][0] = 100;
bf[2][1][0] = 100;
af.c(2,1,0) = 100;
bf.c(2,1,0) = 100;

tensor<int,range(1,7),range(1,5),range(1,3)> ac;
tensor<int,7,5,3> bc;
ac(1,2,3) = 100;
bc(0,1,2) = 100;
ac[2][1][0] = 100;
bc[2][1][0] = 100;
ac.c(2,1,0) = 100;
bc.c(2,1,0) = 100;
```

## allocatable
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
std::vector<std::vector<std::vector<int>>> a;
int d1 = 3;
int d2 = 5;
int d3 = 7;
if (!a.size()) {
  // int a[7][5][3]
  a.resize(d3);
  for (int i = 0; i < d3; ++i) {
    a[i].resize(d2);
    for (int j = 0; j < d2; ++j) {
      a[i][j].resize(d1);
    }
  }
}
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
