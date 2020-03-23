#include "aflsmart.h"

#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

map<string, SmartInput*> smart_map;

/* Add input structure information to the queue entry */

extern "C" void afl_custom_init(unsigned int seed) {
  srand(seed);
}

extern "C" uint8_t afl_custom_queue_get(const char *filename) {

  string fname(filename);
  struct chunk *chunk = NULL;

  if (smart_map.find(fname) != smart_map.end())
    chunk = smart_map[fname]->chunk;

  // Deferred cracking
  if (!chunk && UR(100) < (get_cur_time() - last_path_time) / 50) {
    update_input_structure(filename);
  }

  return 1;

}

void afl_custom_queue_new_entry(const uint8_t *filename_new_queue,
                                const uint8_t *filename_orig_queue) {

  last_path_time = get_cur_time();

}

size_t afl_custom_fuzz(uint8_t **buf, size_t buf_size, uint8_t *add_buf,
                       size_t add_buf_size,  // add_buf can be NULL
                       size_t max_size) {

  

}

// Disable trimming
extern "C" uint32_t afl_custom_init_trim(uint8_t* buf, size_t buf_size) {
  return 0;
}
extern "C" void afl_custom_trim(uint8_t** out_buf, size_t* out_buf_size) {}
extern "C" uint32_t afl_custom_post_trim(uint8_t success) {
  return 0;
}

