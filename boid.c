#include "config.h"
#include "boid.h"

void init_boid(boid* b, SDL_Surface* image, int loc_x, int loc_y, int vel_x, int vel_y)
{
	b->sprite = image;

	init_vector(&b->location, loc_x, loc_y, 0);
	init_vector(&b->velocity, vel_x, vel_y, 0);
	init_vector(&b->acceleration, 0, 0, 0);
}

void  flock_influence(vector* v, boid* flock, boid* b)
{
	init_vector_scalar(v, 0);

     /* influence[0] = alignment & cohesion,
	influence[2] = separation */
	vector influence[2];

     /* The first population is a total of the boids within the neighborhood of the target boid.
	The second population is a total of the boids infringing on the target boid's space.*/
	int population[2];

	int i;
	for(i = 0; i < 2; i++)
		init_vector_scalar(&influence[i], 0);

	for(i = 0; i < NUM_BOIDS; i++)
	{
		if(&flock[i] != b)
		{
			float distance = vector_distance_nosqrt(&flock[i].location, &b->location);

			if(distance < NEIGHBORHOOD_RADIUS_SQUARED)
			{
				vector_add(&influence[0], &flock[i].velocity);
				vector_add(&influence[0], &flock[i].location);

				population[0] += 2;

				if(distance < MIN_BOID_SEPARATION_SQUARED)
				{
					vector loc;
					copy_vector(&loc, &b->location);

					vector_sub(&loc, &flock[i].location);
					vector_normalize(&loc);
					vector_div_scalar(&loc, distance);

					vector_add(&influence[1], &loc);

					population[1]++;
				}
			}
		}
	}

	for(i = 0; i < 2; i++)
	{
		if(population[i] > 0)
			vector_div_scalar(&influence[i], population[i]);
		else
			init_vector_scalar(&influence[i], 0);

		if(vector_magnitude(&influence[i]) > 0)
		{
			vector_normalize(&influence[i]);
			vector_mul_scalar(&influence[i], MAX_BOID_VELOCITY);
			vector_sub(&influence[i], &b->velocity);
			vector_mul_scalar(&influence[i], MAX_BOID_STEERING_FORCE);
		}
	}

	for(i = 0; i < 2; i++)
		vector_add(v, &influence[i]);
}
