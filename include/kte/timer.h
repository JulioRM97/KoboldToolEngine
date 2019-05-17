#pragma once

#ifdef WIN32

#include <windows.h>

typedef LARGE_INTEGER START_TYPE;

#else // !WIN32

#include <ctime>

typedef struct timespec START_TYPE;

#endif // !WIN32

namespace kte
{

struct Time
{
  static float deltaTime;
  static float fixedDeltaTime;
  static float remainingSimulationTime;
  static float elapsedTime;
};

////////////////////////////////////////////////////////////////
/// \summary High precision timer
class Timer
{
 public:
  /// \summary Start the timer
  void start();

  /// \returns time elapsed since the start of the timer in seconds
  float elapsed(bool restart = false);

  /// Restarts the timer.
  void restart() { elapsed(true); };

 private:
  START_TYPE start_time_;
};

}

