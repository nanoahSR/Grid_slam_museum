#pragma once
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
void update_map( int itr, int numReading, Eigen::VectorXd reading, double pos[3]);
