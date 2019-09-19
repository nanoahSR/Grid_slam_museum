//#include "stdafx.h"
#include <iostream>
#include <Eigen/Dense>
#include "public.h"
#include <ctime>
#include <random>

using namespace Eigen;
using namespace std;

int origin[2] = { -100, -70 };
double mesh_dimen[2] = { 0.25, 0.25 };
double MAP[30][640][640] = { 0.0 };
double weight[30] = { 0.0 };
double particles[30][5530][3] = { 0.0 };


mvrand generateGaussianNoise(double mu[3], double sigma[3])
{
	mvrand MV;
	double r;

	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 30; i++)
			if (sigma[j] == 0)
			{
				MV.rand[i][j] = mu[j];
				continue;
			}
			else
				while (true)
				{

					std::default_random_engine  e(time(NULL));
					std::normal_distribution<double> distribution(mu[j], sigma[j]);
					r = distribution(e);
					if (!isinf(r) && !isnan(r))
					{
						MV.rand[i][j] = r;
						break;
					}
				}
	return MV;
};


double rad2ang(double rad)
{
	double ang;
	double flag = (rad < 0) ? -1.0 : 1.0;
	if(rad < 0)
		rad = rad * (-1.0);
	double result = (rad*180) / p_PI;
	ang = flag * result;
	return ang;
};

double ang2rad(double degree)
{
	double flag = (degree < 0) ? -1.0 : 1.0;			//判断正负
	if (degree < 0)
		degree = degree * (-1.0);
	double result = flag * (degree*p_PI) / 180;
	
	return result;
}

int sign(double num)
{
	if (num > 0)
		return 1;
	else if (num < 0)
		return -1;
	else return 0;
};
