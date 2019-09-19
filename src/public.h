#pragma once
#include <Eigen/Dense>
using namespace Eigen;
#define count_K 30
#define p_PI 3.1415926
extern int origin[2];
extern double mesh_dimen[2];
extern double MAP[30][640][640];
extern double weight[30];
extern double particles[30][5530][3];

struct mvrand {
	double rand[30][3] = {{0.0}};
};
mvrand generateGaussianNoise(double mu[3], double sigma[3]);

double rad2ang(double rad);
double ang2rad(double degree);
int sign(double num);