//#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "update_map.h"
#include "calc_path.h"
#include "occupied_grid.h"
#include "public.h"

using namespace Eigen;
using namespace std;

/*************************************************
// Method: update_map
// Description:
// Author: RSN
// Date: 2019/06/27
// Returns: void
// Parameter: itr: current particle;
// Parameter: numReading: number of times to be measurements;
// Parameter: reading: measurement data for each step;
// Parameter: pos: current particle position.
// History:
*************************************************/
void update_map(int itr, int numReading, Eigen::VectorXd reading, double pos[3])
{
	//int MAXR = 8; // Maximum distance
	int MAXR = 30; // Maximum distance
	double x_pose, y_pose, angle_pose, angle_pose_deg;
	double prob_empty, prob_occupied, factor_empty, factor_occupied;
	double start_point[2], end_point[2];

	double dist;
	double x_point, y_point;
	double degree;

	Result fcp;
	oGrid fog;

	x_pose = pos[0];
	y_pose = pos[1];
	angle_pose = pos[2];
	angle_pose_deg = rad2ang(angle_pose);
	
	start_point[0] = x_pose;
	start_point[1] = y_pose;
	
	prob_empty = 0.4;
	prob_occupied = 0.75;
	factor_empty = (1 - prob_empty) / prob_empty;
	factor_occupied = (1 - prob_occupied) / prob_occupied;

	for (int nu_readings = 1; nu_readings <= 1079; nu_readings++)
	{
		dist = reading(nu_readings-1);

		if (dist < MAXR)
		{
			//degree = angle_pose_deg + 90 - nu_readings;
			degree = angle_pose_deg - 134.75 + nu_readings * rad2ang(0.004363);
			x_point = x_pose + dist * cos(ang2rad(degree));
			y_point = y_pose + dist * sin(ang2rad(degree));
			end_point[0] = x_point;
			end_point[1] = y_point;

			fcp = calc_path(start_point, end_point);
			fog = occupied_grid(fcp.start_index, fcp.end_index, fcp.x_index_seq, fcp.y_index_seq);

			/* put in MAP */
			int temp_coord[2], x_coord, y_coord;
			for (int ntr = 0; fog.empty_output[ntr][0] > 0; ntr++)
			{
				temp_coord[0] = ceil(fog.empty_output[ntr][0]);
				temp_coord[1] = ceil(fog.empty_output[ntr][1]);
				x_coord = temp_coord[0] - 1;
				y_coord = temp_coord[1] - 1;
				//printf("Before: y_coord = %d,x_coord = %d,output = %lf\n", x_coord, y_coord, MAP[itr][y_coord][x_coord]);
				// <<Probabilistic Robotics>> Page 217. formulas 9.5-9.6.
				MAP[itr][y_coord][x_coord] =
					1.0 / (1+factor_empty*((1-MAP[itr][y_coord][x_coord]) / MAP[itr][y_coord][x_coord]));
				//printf("After: y_coord = %d,x_coord = %d,output = %lf\n", x_coord, y_coord, MAP[itr][y_coord][x_coord]);
			}

			temp_coord[0] = ceil(fog.occupied_output[0]);
			temp_coord[1] = ceil(fog.occupied_output[1]);
			x_coord = temp_coord[0] - 1;
			y_coord = temp_coord[1] - 1;
			//printf("Before: y_coord = %d,x_coord = %d,output = %lf\n", x_coord, y_coord, MAP[itr][y_coord][x_coord]);
			MAP[itr][y_coord][x_coord] =
				1.0 / (1+factor_occupied*((1-MAP[itr][y_coord][x_coord]) / MAP[itr][y_coord][x_coord]));
			//printf("After: y_coord = %d,x_coord = %d,output = %lf\n", x_coord, y_coord, MAP[itr][y_coord][x_coord]);
		}
	}
}
