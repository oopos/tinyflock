#ifndef FLOCK_H_
#define FLOCK_H_

#include "boid.h"
#include "configuration.h"

#define FLOCK_TYPE_RANDOM 0
#define FLOCK_TYPE_CENTERED 1

boid* create_flock(configuration* config);

void destroy_flock(boid* flock);

// Update the location and velocity of each boid in the flock
void flock_update(boid* flock, configuration* config);

// thread args type for flock_render
typedef struct{ int run; boid* flock; configuration* config; SDL_Surface* screen; } flock_render_data;

// Render the flock (independent thread)
int flock_render_thread(void* arg);

void flock_influence(vector* v, boid* flock, boid* b, configuration* config);

static inline float rand_range(float min, float max);

#endif