
#include <iostream>
#include <Eigen/Dense>
#include "calc_path.h"
#include "public.h"

using namespace Eigen;
using namespace std;

double y_fun(double x, double start[2], double end[2])
{
	double result = start[1] + (x-start[0]) * (end[1]-start[1]) / (end[0]-start[0]);
	return result;
}

double x_fun(double y, double start[2], double end[2])
{
	double result = start[0] + (y-start[1]) * (end[0]-start[0]) / (end[1]-start[1]);
	return result;
}


/*************************************************
// Method: calc_path
// Description: Calculation path
// Author: RSN
// Date: 2019/06/27
// Returns: Result
// Parameter: start_point: start position;
// Parameter: end_point: end position.
// History:
*************************************************/
Result calc_path(double start_point[2], double end_point[2])
{
	Result rep;

	double x_dimen, y_dimen;
	x_dimen = mesh_dimen[0];
	y_dimen = mesh_dimen[1];

	double mol1, mol2;
	double den1, den2;
	mol1 = start_point[0] - origin[0];
	mol2 = start_point[1] - origin[1];
	den1 = x_dimen;
	den2 = y_dimen;

	rep.start_index[0] = ceil ( mol1 / den1 );
	rep.start_index[1] = ceil ( mol2 / den2 );
	
	//cout << "calc_path rep.start_index[0]: " <<rep.start_index[0] << endl;
	mol1 = end_point[0] - origin[0];
	mol2 = end_point[1] - origin[1];
	rep.end_index[0] = ceil ( mol1 / den1 );
	rep.end_index[1] = ceil ( mol2 / den2 );

	int x_step, y_step;
	x_step = sign ( rep.end_index[0] - rep.start_index[0] );
	y_step = sign ( rep.end_index[1] - rep.start_index[1] );

	/* */
	int j = 0, k = 0;
	if ( x_step > 0 )
		for ( int i = ceil(rep.start_index[0]); i <= rep.end_index[0]; i++, j++ )
			rep.y_index_seq[j] = y_fun ( i * x_dimen + origin[0], start_point, end_point );
	else if ( x_step < 0 )
		for ( int i = floor(rep.start_index[0]-1); i > rep.end_index[0]; i--, j++)
			rep.y_index_seq[j] = y_fun ( i * x_dimen + origin[0], start_point, end_point );
	else ;


	if ( y_step > 0 )
		for ( int i = ceil(rep.start_index[1]); i <= rep.end_index[1]; i++, k++ )
			rep.x_index_seq[k] = x_fun ( i * y_dimen + origin[1], start_point, end_point );
	else if ( y_step < 0)
		for ( int i = floor(rep.start_index[1]-1); i > rep.end_index[1]; i--, k++ )
			rep.x_index_seq[k] = x_fun ( i * y_dimen + origin[1], start_point, end_point );
	else ;
	
	for ( int i = 0; i < k; i++ )
		rep.x_index_seq[i] = ceil(( rep.x_index_seq[i] - origin[0]) / x_dimen);
	for ( int i = 0; i < j; i++ )
		rep.y_index_seq[i] = ceil(( rep.y_index_seq[i] - origin[1]) / y_dimen);

	return rep;
}
