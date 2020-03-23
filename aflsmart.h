#ifndef AFLSMART_CUSTOM_MUTATOR
#define AFLSMART_CUSTOM_MUTATOR

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <map>

using namespace std;

#define UR(limit) (rand() % (limit))

struct SmartInput {
  int validity;
  struct chunk* chunk;
  struct chunk* cached_chunk;
};

void update_input_structure(const string& filename, SmartInput* si);

// Utils
uint64_t get_cur_time();

#endif
