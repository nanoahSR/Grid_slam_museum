//#include "stdafx.h"
#include <iostream>
#include <Eigen/Dense>
#include <fstream>
#include "update_particles.h"
#include "laser_point_prob.h"
#include "update_map.h"
#include "public.h"
#include "length.h"

using namespace std;
using namespace Eigen;

void normalize();

/*************************************************
// Method: update_particles
// Description: update particles with new state
// Author: RSN
// Date: 2019/06/27
// Returns: void
// Parameter: numReading: number of times to be measurements;
// Parameter: reading: current particles set
// Parameter: control: control parameter, odometry here
// Parameter: R: the covariance matrix of the control input
// History:
*************************************************/
void update_particles(int numReading, Eigen::VectorXd reading, double control[3], double R[3])
{

	mvrand upfmv;
	upfmv = generateGaussianNoise(control, R);
	
	for (int itr = 0; itr < count_K; itr++)
	{
		/*if (itr == 177)
			itr = itr;*/

		double  pose_new[3] = { 0 };
		int cp = 0; // Statistical particle observations

		/* Take the last updated position of particles */
		int index = 0;
		while ( particles[itr][index][0] != 0 )
		{
			cp++;
			index++;
		}

		for (int i = 0; i < 3; i++)
			pose_new[i] = particles[itr][cp-1][i] + upfmv.rand[itr][i];

		weight[itr] *= laser_point_prob(itr, pose_new, numReading, reading);

		for (int j = 0; j < 3; j++)
			particles[itr][cp][j] = pose_new[j];
		
		update_map(itr, numReading, reading, pose_new);
	}

	normalize();
}


void normalize()
{
	double weight_sum = 0.0;

	for(int i = 0; i < 30; i++)
		weight_sum += weight[i];

	for (int i = 0; i < 30; i++)
		weight[i] = weight[i] * 1.0 / weight_sum;
}
