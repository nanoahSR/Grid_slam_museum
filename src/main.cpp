/*************************************************
// Method: main.cpp
// Description:
// Author: RSN
// Date: 2019/09/19
// History: 2019/09/19  MODIFY: main();
*************************************************/

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <cmath>
#include <ctime>
#include "update_map.h"
#include "draw_illustration.h"
#include "update_particles.h"
#include "resampling.h"
#include "public.h"

using namespace Eigen;
using namespace std;

void initialize_particles(double x0[3], double R[3], int numReading, Eigen::VectorXd reading, int range[2]);

/*************************************************
// Method: main
// Description: Program Start Entry
// Author: RSN
// Date: 2019/09/19
// Returns: int
// Parameter:
// History: 2019/09/19 add: string variable < path >;
                       modify: " file.open " --> " file.open ( path + "/" + "txt" ); "
*************************************************/
int main()
{
	/*
	% load .txt
	% this is a test dataset from OpenSLAM collected from ACES3 building, UT
	*/
	srand((int)time(0));
	int count = 5522; // number of data set
	MatrixXd control(5522, 3);
	VectorXd numReadings(5522); // number of measurements;
	MatrixXd pose(5522, 3);
	MatrixXd readings(5522, 1079); // measurement data

	string path = "/home/rsn/Workplace/SLAM/Grid_slam_museum/data";

	ifstream file1;
	file1.open (path + "/" + "fakecontrol.txt" );
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			file1 >> control(i,j);
		}
	}
	file1.close();

	ifstream file3 ( path + "/" + "fakenumReadings.txt" );
	for (int j = 0; j < count; j++)
	{
		file3 >> numReadings(j);
	}
	file3.close();

	ifstream file5 ( path + "/" + "fakepose.txt" );
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			file5 >> pose(i, j);
		}
	}
	file5.close();

	ifstream file6 ( path + "/" + "fakereadings.txt" );
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 1079; j++)
		{
			file6 >> readings(i, j);
		}
	}
	file6.close();

	/*
	x0: set the initial state, motion model covariance matrix and number of particles
	R: covariance matrix
	*/
	double x0[3] = { pose(0, 0), 0, 0 };
	//double R[3] = { pow(0.1, 2), pow(0.1, 2), pow(1.0, 2) };
	double R[3] = { pow(0.1, 2), 0, 0 };
	
	// set parameters of the MAP
	int range[2] = { 640, 640 };
	
	cout <<"Start Grid-based Rao-Blackwellized particle filter SLAM" << endl;

	int numReading = numReadings(399);

	VectorXd reading(1079);
	reading = readings.block<1, 1079>(399, 0);
	
	initialize_particles(x0, R, numReading, reading, range);
	
	double Maxn = 0.0;
	int Maxi = 0;
	for (int i = 0; i < 30; i++)
		if (weight[i] > Maxn) {
			Maxn = weight[i];
			Maxi = i;
		}
	cout << "Max weight is " << Maxi << " " << Maxn << endl;


	//VectorXd reading_old(180);
	int gap = 1; // Step size
	int fac = 1;
	int max_step = 5520;

	for (int step = 10 + gap; step <= max_step; step += gap)
	{
		cout << "moving to step: " << step << endl;
		
		//if (step == 1190)
		//	step = step;

		numReading = numReadings(step);
		reading = readings.block<1, 1079>(step-1, 0);
		
		double motion[3];
		for (int i = 0; i < 3; i++)
		{
			motion[i] = 0;
			for (int j = step - gap; j < step; j++)
				motion[i] += control(j, i);
		}

		R[0] = pow(min(0.1, motion[0] / fac), 2);
		R[1] = pow(min(1.0, motion[1] / fac), 2);
		R[2] = pow(min(0.1, motion[2] / fac), 2);

		update_particles(numReading, reading, motion, R);
		
		printf("moving to step %d\n", step);
		Maxn = 0.0;
		for(int i = 0; i < 30; i++)
			if (weight[i] > Maxn)
			{
				Maxn = weight[i];
				Maxi = i;
			}
		cout << "Max weight is " << Maxi << " " << Maxn<< endl;

		resampling();
		draw_illustration(Maxi);
	}

	draw_illustration(Maxi);

	getchar();
	return 0;
}


/*************************************************
// Method: initialize_particles
// Description: Initialize particles
// Author: RSN
// Date: 2019/05/22
// Returns: void
// Parameter: x0: the initial position of the robot
// Parameter: R: the covariance matrix of the initial position;
// Parameter: numReading: number of times to be measurements;
// Parameter: reading: measurement data for each step;
// Parameter: range: Map's size.
// History:
*************************************************/
void initialize_particles(double x0[3], double R[3], int numReading, Eigen::VectorXd reading, int range[2])
{

	// get random position of particles;
	mvrand fmv;
	fmv = generateGaussianNoise(x0, R);

	/* initialize the MAP */
	for (int k = 0; k < count_K; k++)
		for (int i = 0; i < range[0]; i++)
			for (int j = 0; j < range[1]; j++)
				MAP[k][i][j] = 0.5;

	for (int itr = 0; itr < count_K; itr++)
	{
		for (int i = 0; i < 3; i++)
			particles[itr][0][i] = fmv.rand[itr][i];
		
		update_map (itr, numReading, reading, fmv.rand[itr]);
	}
	
	/* initialize the weight */
	for (int i = 0; i < count_K; i++)
		weight[i] = 1.0 / count_K;
}
