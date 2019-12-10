#ifndef DLOG_H
#define DLOG_H

#include <iostream>

struct X {
  ~X() { std::cout << std::endl; }
};
#ifndef NDEBUG 
#define debug_disabled 0
#else 
#define debug_disabled 1
#endif

#define DLOG \
    if (debug_disabled) {} \
    else std::cerr << "  [Dbug] "

using namespace std;

#endif