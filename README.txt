To build a static library
============-============
make -f Makefile.static

To build a shared/dynamic library
=================================
make -f Makefile.shared

To compile and run the test cases
=================================
cpp-argparser uses Google Test framework (http://code.google.com/p/googletest/)
Modify the GTEST_HOME in the Makefile to point to your Google Test location
