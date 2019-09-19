
#include <Eigen/Dense>
#include <cmath>
#include "laser_point_prob.h"
#include "public.h"

using namespace Eigen;
using namespace std;

void* coord2index(int *index_end, double x, double y)
{
	index_end[0] = ceil ((x-origin[0]) / mesh_dimen[0]);
	index_end[1] = ceil ((y-origin[1]) / mesh_dimen[1]);
}

/*************************************************
// Method: laser_point_prob
// Description: given the start pose and data from laser range data, calculate the
				terminating points, and then calculation the probability that all the
				terminating points are occupied.
// Author: RSN
// Date: 2019/06/17
// Returns: double
// Parameter: itr: current particle;
// Parameter: pose: new position of current position;
// Parameter: numReading: number of times to be measurements;
// Parameter: reading: measurement data for each step.
// History:
*************************************************/
double laser_point_prob(int itr, double pose[3], int numReading, Eigen::VectorXd reading)
{
	int MAXR = 30; // Maximum distance

	double x_start = pose[0];
	double y_start = pose[1];
	double angle = pose[2];
	double angle_deg = rad2ang(angle);

	int step_size = 3;
	double w_output = 1.0;
	double amp_factor = 1.3;

	for ( int wtr = 1; wtr < 1079; wtr += step_size )
	{
		double dist = reading( wtr - 1 );
		if ( dist < MAXR )
		{
			//double degree = angle_deg + 90 - wtr;
			double degree = angle_deg - 134.75 + wtr * rad2ang(0.004363);
			double x_end = x_start + dist * cos(ang2rad(degree));
			double y_end = y_start + dist * sin(ang2rad(degree));
			int index_end[2];

			coord2index(index_end, x_end, y_end);

			if (index_end[0] < 640 && index_end[1] < 640)
				w_output = w_output * amp_factor * MAP[itr][index_end[1]-1][index_end[0]-1];

			if (w_output < 1e-10)
					w_output = 5e-11;

			else if (w_output > 1e10)
				w_output = 5e10;
		}
	}
	return w_output;
}
