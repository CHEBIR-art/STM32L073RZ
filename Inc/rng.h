/*
 * rng.h
 *
 *  Created on: Feb 21, 2025
 *      Author: user
 */

#ifndef RNG_H_
#define RNG_H_

void rng_init(void) ;
uint32_t rng_get_random(void) ;
uint32_t rng_get_random_in_range(uint32_t min, uint32_t max);
int32_t rng_get_signed_random_in_range(int32_t min, int32_t max) ;
void test_rng(void);
#endif /* RNG_H_ */
