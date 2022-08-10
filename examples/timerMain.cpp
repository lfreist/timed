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
#define SLEEP(x) nanosleep(x*1000*1000*1000)
#endif


int main(int argc, char** argv) {
  timed::WallTimer wall_timer;
  timed::CPUTimer cpu_timer;
  std::cout << "Sleeping for 5 seconds..." << std::endl;
  wall_timer.start();
  cpu_timer.start();
  SLEEP(5);
  cpu_timer.stop();
  wall_timer.stop();
  std::cout << "Wall Time: " << wall_timer.getTime() << std::endl;
  std::cout << "CPU Time : " << cpu_timer.getTime() << std::endl;
}