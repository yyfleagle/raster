/*
 * Copyright (C) 2017, Yeolar
 */

#pragma once

#include <vector>
#include <signal.h>
#include "raster/util/Function.h"

namespace rdd {

void setupSignal(int signo, void (*handler)(int));
void setupSignal(int signo, void (*handler)(int, siginfo_t*, void*));

void setupIgnoreSignal(int signo);
void setupShutdownSignal(int signo);
void setupSigsegvSignal(bool protect = false);

class Shutdown {
public:
  Shutdown() {}

  void addTask(const VoidFunc& callback) {
    callbacks_.push_back(callback);
  }

  void run() {
    for (auto& cb : callbacks_) {
      cb();
    }
    exit(0);
  }

private:
  std::vector<VoidFunc> callbacks_;
};

} // namespace rdd
