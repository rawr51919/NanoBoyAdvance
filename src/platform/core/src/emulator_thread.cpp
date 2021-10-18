/*
 * Copyright (C) 2021 fleroviux
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#include <platform/emulator_thread.hpp>

namespace nba {

EmulatorThread::EmulatorThread(
  std::shared_ptr<CoreBase> core
)   : core(core) {
  frame_limiter.Reset(59.7275);
}

EmulatorThread::~EmulatorThread() {
  if (IsRunning()) Stop();
}

bool EmulatorThread::IsRunning() const {
  return running;
}

void EmulatorThread::SetFastForward(bool enabled) {
  frame_limiter.SetFastForward(enabled);
}

void EmulatorThread::Start() {
  if (!running) {
    running = true;
    thread = std::thread{[this]() {
      frame_limiter.Reset();

      while (running) {
        frame_limiter.Run([this]() {
          core->RunForOneFrame();
        }, [this](float fps) {
          // ...
        });
      }
    }};
  }
}

} // namespace nba
