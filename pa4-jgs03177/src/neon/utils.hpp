#ifndef __UTILS_H_
#define __UTILS_H_

#include "neon/scene.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>

namespace ne {

namespace utils {

// This is just copy of https://github.com/andremaravilha/cxxtimer
class Timer {

public:
  /**
   * Constructor.
   *
   * @param   start
   *          If true, the timer is started just after construction.
   *          Otherwise, it will not be automatically started.
   */
  Timer(bool start = false);

  /**
   * Copy constructor.
   *
   * @param   other
   *          The object to be copied.
   */
  Timer(const Timer &other) = default;

  /**
   * Transfer constructor.
   *
   * @param   other
   *          The object to be transfered.
   */
  Timer(Timer &&other) = default;

  /**
   * Destructor.
   */
  virtual ~Timer() = default;

  /**
   * Assignment operator by copy.
   *
   * @param   other
   *          The object to be copied.
   *
   * @return  A reference to this object.
   */
  Timer &operator=(const Timer &other) = default;

  /**
   * Assignment operator by transfer.
   *
   * @param   other
   *          The object to be transferred.
   *
   * @return  A reference to this object.
   */
  Timer &operator=(Timer &&other) = default;

  /**
   * Start/resume the timer.
   */
  void start();

  /**
   * Stop/pause the timer.
   */
  void stop();

  /**
   * Reset the timer.
   */
  void reset();

  /**
   * Return the elapsed time.
   *
   * @param   duration_t
   *          The duration type used to return the time elapsed. If not
   *          specified, it returns the time as represented by
   *          std::chrono::milliseconds.
   *
   * @return  The elapsed time.
   */
  template <class duration_t = std::chrono::milliseconds>
  typename duration_t::rep count() const;

private:
  bool started_;
  bool paused_;
  std::chrono::steady_clock::time_point reference_;
  std::chrono::duration<long double> accumulated_;
};

inline Timer::Timer(bool start)
    : started_(false), paused_(false),
      reference_(std::chrono::steady_clock::now()),
      accumulated_(std::chrono::duration<long double>(0)) {
  if (start) {
    this->start();
  }
}

inline void Timer::start() {
  if (!started_) {
    started_ = true;
    paused_ = false;
    accumulated_ = std::chrono::duration<long double>(0);
    reference_ = std::chrono::steady_clock::now();
  } else if (paused_) {
    reference_ = std::chrono::steady_clock::now();
    paused_ = false;
  }
}

inline void Timer::stop() {
  if (started_ && !paused_) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
    accumulated_ =
        accumulated_ +
        std::chrono::duration_cast<std::chrono::duration<long double>>(
            now - reference_);
    paused_ = true;
  }
}

inline void Timer::reset() {
  if (started_) {
    started_ = false;
    paused_ = false;
    reference_ = std::chrono::steady_clock::now();
    accumulated_ = std::chrono::duration<long double>(0);
  }
}

template <class duration_t> typename duration_t::rep Timer::count() const {
  if (started_) {
    if (paused_) {
      return std::chrono::duration_cast<duration_t>(accumulated_).count();
    } else {
      return std::chrono::duration_cast<duration_t>(
                 accumulated_ + (std::chrono::steady_clock::now() - reference_))
          .count();
    }
  } else {
    return duration_t(0).count();
  }
}

// based on https://github.com/prakhar1989/progress-cpp but add multithread
// support
class Progressbar {
public:
  explicit Progressbar(unsigned int total, unsigned int width = 60)
      : total_ticks_(total), bar_width_(width) {}

  unsigned int operator++() { return ticks_++; }

  void display() const {
    static std::mutex displayMutex;
    std::lock_guard<std::mutex> lock(displayMutex);

    float progress = float(ticks_) / float(total_ticks_);
    int pos = int(bar_width_ * progress);

    auto time_elapsed = timer_.count();

    std::cout << "[";

    for (int i = 0; i < bar_width_; ++i) {
      if (i < pos)
        std::cout << complete_char;
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << incomplete_char;
    }
    std::cout << "] " << int(progress * 100.0) << "% "
              << float(time_elapsed) / 1000.0 << "s\r";
    std::cout.flush();
  }

  unsigned int increase(unsigned int t) {
    ticks_ += t;
    return ticks_;
  }

  void end() const {
    display();
    std::cout << std::endl;
  }

  void reset() {
    ticks_ = 0;
    timer_.reset();
  }

  void start() { timer_.start(); }

private:
  Timer timer_;
  std::atomic_uint32_t bar_width_ = 60;
  std::atomic_uint32_t total_ticks_ = 0;
  std::atomic_uint32_t ticks_ = 0;
  inline static constexpr char incomplete_char = ' ';
  inline static constexpr char complete_char = '=';
};

} // namespace utils

} // namespace ne

#endif // __UTILS_H_
