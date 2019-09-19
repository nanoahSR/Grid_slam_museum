/*************************************************
// Method: occupied_grid.h
// Description: Calculation path
// Author: RSN
// Date: 2019/06/27
// History: 2019/09/19 add: <struct oGrid> initial data to zero
*************************************************/

#pragma once
struct oGrid
{
	double empty_output[300][2] = {{0.0}};
	double occupied_output[2] = {0.0};
};
oGrid occupied_grid(double start_index[2], double end_index[2], double x_index_seq[3], double y_index_seq[3]);
