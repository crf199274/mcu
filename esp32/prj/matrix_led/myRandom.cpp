#include "myRandom.h"

#define RANDOM_SEED_SOURCE_GPIO   36

uint32_t getARandom(uint32_t start,uint32_t end)
{
  randomSeed(analogRead(RANDOM_SEED_SOURCE_GPIO));
  return random(start,end);
}
