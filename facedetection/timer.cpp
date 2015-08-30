#include "timer.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>  

/*!
 * \brief Start measuring execution time 
 *        Win32/64 timer version (essentially calls to __asm rdtsc).
 * \param TM_STATE *state - state to be set to signaled when the specified due time arrives
 * \param TM_COUNTER *start - get start time
 * \return void
 */

void start_timer (TM_STATE *state, TM_COUNTER *start)
{
  SetThreadAffinityMask (GetCurrentThread(), 1);
  QueryPerformanceFrequency ((PLARGE_INTEGER)&state->freq);
  QueryPerformanceCounter ((PLARGE_INTEGER)&start->counter);
}

/*!
 * \brief Stop measuring time
 *        Win32/64 timer version (essentially calls to __asm rdtsc).
 * \param TM_COUNTER *end - get end time
 * \return void
 */

void stop_timer (TM_COUNTER *end)
{
  QueryPerformanceCounter ((PLARGE_INTEGER)&end->counter);
}

/*!
 * \brief Compute elapsed time
 *        Win32/64 timer version (essentially calls to __asm rdtsc).
 * \param TM_STATE *state - state to be set to signaled when the specified due time arrives
 * \param TM_COUNTER *start - get start time
 * \param TM_COUNTER *end - get end time
 * \return void
 */

double elapsed_time (TM_STATE *state, TM_COUNTER *start, TM_COUNTER *end)
{
  return (double) (end->counter - start->counter) / (double) state->freq;
}

#else // !windows

#include <time.h>
#include <unistd.h>

/*!
 * \brief Start measuring execution time 
 *        Linux version, relying on POSIX clock_gettime() function
 * \param TM_STATE *state - state to be set to signaled when the specified due time arrives
 * \param TM_COUNTER *start - get start time
 * \return void
 */

void start_timer (TM_STATE *state, TM_COUNTER *start)
{
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, (struct timespec*)&start->tmspec);
}

/*!
 * \brief End measuring execution time 
 *        Linux version, relying on POSIX clock_gettime() function
 * \param TM_COUNTER *end - get end time
 * \return void
 */
void stop_timer (TM_COUNTER *end)
{
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, (struct timespec*)&end->tmspec);
}

/*!
 * \brief Measure execution/elapsed time 
 *        Linux version, relying on POSIX clock_gettime() function
 * \param TM_STATE *state - state to be set to signaled when the specified due time arrives
 * \param TM_COUNTER *start - get start time
 * \param TM_COUNTER *end - get end time
 * \return void
 */
double elapsed_time (TM_STATE *state, TM_COUNTER *start, TM_COUNTER *end)
{
  time_t sec  = end->tmspec.tv_sec - start->tmspec.tv_sec;
  long   nsec = end->tmspec.tv_nsec - start->tmspec.tv_nsec;
  if (nsec < 0) {sec --; nsec += 1000000000;} 
  return (double)sec + (double)nsec / 1000000000.;
}

#endif
