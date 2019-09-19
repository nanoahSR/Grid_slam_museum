
#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include "resampling.h"
#include "public.h"
#include "length.h"

using namespace std;
void low_variance_resampling( );

static double particles_new[30][735][3];
static double map_new[30][640][640];
static double weight_new[30];

void resampling( )
{
	/*
		% calculate the resampling coefficient n_eff, if n_eff is smaller than
		% certen number epslion, do resampling; otherwise, leave the particle set
		% as it is.
		%
		% written by He Sun on Oct. 26, 2015
	*/
	
	double eps, n_eff, sum_pow = 0.0;

	eps = ( sqrt(count_K) * 1.0 ) / 2;
	for (int i = 0; i < 30; i++)
		sum_pow += pow( weight[i], 2 );
	n_eff = 1.0 / sum_pow ;

	if ( n_eff < eps )
		low_variance_resampling( );
	
	else if ( n_eff >= eps )
	{
		;
	}

	else
	{
		for (int i = 0; i < 30; i++)
			weight[i] = 1.0 / count_K;
	}
	
}

void low_variance_resampling( )
{
	// use a Roulette wheel to do stochastic universal resampling

	double r, c, U;
	int i;

	default_random_engine e (time (0));
	uniform_real_distribution<double> u (0, 1.0 / count_K);
	r = u(e);
	c = weight[0];
	i = 1;
	
	for ( int j = 1; j <= count_K; j++ )
	{
		//if ( j == 30 )
		//	j = j;
		U = r +  ( j - 1 ) * 1.0 / count_K;
		
		while ( U > c )
		{
			i = i + 1;
			c = c + weight[i-1];
		}

		for( int k = 0; particles[i-1][k][0] != 0; k++)
			for( int m = 0; m < 3; m++)
				particles_new[j-1][k][m] = particles[i-1][k][m];
	
		for ( int k = 0; k < 640; k++ )
			for ( int m = 0; m < 640; m++ )
				map_new[j-1][k][m] = MAP[i-1][k][m];
	}
	
	for ( int i = 0; i < 30; i++ )
		weight_new[i] = 1.0 / count_K;

	memcpy ( particles, particles_new, sizeof(particles));
	memcpy ( weight, weight_new, sizeof(weight));
	memcpy ( MAP, map_new, sizeof(MAP));
}
