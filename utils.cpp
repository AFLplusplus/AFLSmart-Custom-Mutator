#include "aflsmart.h"
#include <sys/time.h>

uint64_t get_cur_time() {

  struct timeval tv;
  struct timezone tz;

  gettimeofday(&tv, &tz);

  return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);

}
