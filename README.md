# Introduction
This is a C++ wrapper that mimics the syntax of static and dynamic multidimensional Fortran array.

I felt it would cost me less time to write a correct wrapper than
+ to change the 1-based numbering Fortran index to 0-based numbering C index;
+ to change the parentheses to multiple pairs of square brackets;
+ and to change arrays from column-major to row-major.

I believe developers should be able to choose their favorite flavor from the above options in an expressive language like C++.
