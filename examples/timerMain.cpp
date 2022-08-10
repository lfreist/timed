// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "timed/Timer.h"

#ifdef _WIN32
#define SLEEP(x) Sleep(x * 1000)
#else
#define SLEEP(x) sleep(x)
#endif

int moduloCounter(int i, int  j, int k) {
  int counter = 0;
  for (;i > 0; --i) {
    for (; j > 0; --j) {
      for (; k> 0; --k) {
        if (i % j == k) {
          counter++;
        }
      }
    }
  }
  return counter;
}


int main(int argc, char** argv) {
  timed::WallTimer wall_timer;
  timed::CPUTimer cpu_timer;
  std::cout << wall_timer.getTime() << std::endl;
  // just sleep for 5 seconds
  std::cout << "Sleeping for 2 seconds..." << std::endl;
  wall_timer.start();
  cpu_timer.start();
  SLEEP(2);
  cpu_timer.stop();
  wall_timer.stop();
  std::cout << "Wall Time: " << wall_timer.getTime() << std::endl;
  std::cout << "CPU Time : " << cpu_timer.getTime() << std::endl;

  // do some computations
  std::cout << "Computing number of occurrences of i % j == k for i, j, k from 0 to 100" << std::endl;
  wall_timer.start();
  cpu_timer.start();
  moduloCounter(100, 100, 100);
  cpu_timer.stop();
  wall_timer.stop();
  std::cout << "Wall Time: " << wall_timer.getTime() << std::endl;
  std::cout << "CPU Time : " << cpu_timer.getTime() << std::endl;
}