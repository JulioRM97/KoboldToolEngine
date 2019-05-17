#include "kte/timer.h"

namespace kte
{

float Time::deltaTime = 0.0f;
float Time::fixedDeltaTime = 40.0f / 1000.0f;
float Time::elapsedTime = 0.0f;
float Time::remainingSimulationTime = 0.0f;

#ifdef WIN32

////////////////////////////////////////////////////////////////
void Timer::start()
{
  QueryPerformanceCounter(&start_time_);
}

////////////////////////////////////////////////////////////////
float Timer::elapsed(bool restart)
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);

  LARGE_INTEGER end_time;
  QueryPerformanceCounter(&end_time);

  LARGE_INTEGER original_start = start_time_;

  if (restart)
  {
    start_time_ = end_time;
  }

  return static_cast<float>((end_time.QuadPart - original_start.QuadPart) /
      (double) frequency.QuadPart);
}

#else // !WIN32

#define NS_TO_S 0.000000001
#define CLOCK_TYPE CLOCK_REALTIME

////////////////////////////////////////////////////////////////
void Timer::start()
{
  clock_gettime(CLOCK_TYPE, &start_time_);
}

////////////////////////////////////////////////////////////////
float Timer::elapsed(bool restart)
{
  struct timespec end_time;
  clock_gettime(CLOCK_TYPE, &end_time);

  struct timespec start_time_orig = start_time_;

  if (restart)
  {
    start_time_ = end_time;
  }

  return (end_time.tv_sec - start_time_orig.tv_sec)
      + static_cast<float>(NS_TO_S * (end_time.tv_nsec - start_time_orig.tv_nsec));
}

#endif // !WIN32

} // end namespace kte

