/*************************************************
// Method: calc_path.h
// Description: Calculation path
// Author: RSN
// Date: 2019/06/27
// History: 2019/09/19 add: <struct Result> initial data to zero
*************************************************/

#pragma once
struct Result
{
	double start_index[2] = {0.0};
	double end_index[2] = {0.0};
	double x_index_seq[200] = {0.0};
	double y_index_seq[200] = {0.0};
};

Result calc_path(double start_point[2], double end_point[2]);
