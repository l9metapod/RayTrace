#pragma once

#include "vec3.h"

#define M 0x100000000LL  
#define C 0xB16  
#define A 0x5DEECE66DLL  

double drand48();
static unsigned long long seed = 1;
vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.00*vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0);
	return p;
}

double drand48(void)
{
	seed = (A * seed + C) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return  ((double)x / (double)M);
}