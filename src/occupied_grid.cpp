/*************************************************
// Method: occupied_grid.cpp
// Description: Calculate the occupied grid
// Author: RSN
// Date: 2019/09/19
// History: 2019/09/19 NEW function: void Storage(double Tx, double Ty);
                       NEW global variables: int ino;
                                             double output[300][2];
                       MODIFY: occupied_grid();
*************************************************/

#include "occupied_grid.h"
#include "public.h"

using namespace std;

int ino = 0; // Number of statistical grids

/*************************************************
// Method: Storage
// Description: Storage coordinates
// Author: RSN
// Date: 2019/09/19
// Returns: void
// Parameter: Tx
// Parameter: Ty
// History:
*************************************************/
void* Storage(double *output, double Tx, double Ty)
{
    output[0] = Tx;
    output[1] = Ty;
    ino ++;
};

/*************************************************
// Method: occupied_grid
// Description: Calculate the occupied grid
// Author: RSN
// Date: 2019/06/17
// Returns: oGrid
// Parameter: start_index: start position;
// Parameter: end_index: end position;
// Parameter: x_index_seq: Occupies the x coordinate of the grid;
// Parameter: y_index_seq: Occupies the y coordinate of the grid.
// History: 2019/09/19 MOVE: "int ino = 0; double output[300][2] = { 0.0 };" --> global variables;
                             all "output[ino][0] = ...; output[ino++][1] = ..." --> new function "void Storage()";
                       MODIFY: "if (y_index_seq[0] < 0)" --> "if (y_index_seq[0] == 0";
                               "else if (x_index_seq[0] < 0)" --> " else if (x_index_seq[0] == 0)";
*************************************************/
oGrid occupied_grid(double start_index[2], double end_index[2], double x_index_seq[3], double y_index_seq[3])
{
	double start_index_x, start_index_y, end_index_x, end_index_y;
	start_index_x = start_index[0];
	start_index_y = start_index[1];
	end_index_x = end_index[0];
	end_index_y = end_index[1];

    double output[300][2] = {{0.0}}; // Storage occupied grid
	ino = 0;
    Storage(output[ino], start_index_x, start_index_y);

	int x_step, y_step;
	x_step = sign(end_index_x - start_index_x);
	y_step = sign(end_index_y - start_index_y);

	/* Storage occupied grid */
	// If the value equals zero, it means that the laser is vertical or horizontal in grid coordinates.
	if (y_index_seq[0] == 0)
		if (y_step < 0)
			for (int nu = ceil(start_index_y)+y_step; nu >= end_index_y; nu += y_step)
                Storage(output[ino], start_index_x, nu);
		else if (y_step > 0)
			for (int nu = floor(start_index_y)+y_step + y_step; nu <= end_index_y; nu += y_step)
			    Storage(output[ino], start_index_x, nu);
		else ;
	else if ( x_index_seq[0] == 0 )
		if ( x_step > 0 )
			for (int nu = ceil(start_index_x)+x_step; nu <= end_index_x; nu += x_step)
			    Storage(output[ino], nu, start_index_y);
		else if( x_step < 0 )
			for (int nu = floor(start_index_x)+x_step; nu >= end_index_x; nu += x_step)
			    Storage(output[ino], nu, start_index_y);
		else ;
	else
	{
		int x_itr = 0, y_itr = 0, itr = 0;
		int temp[2];
		
		while ((x_index_seq[x_itr] > 0) || (y_index_seq[y_itr] > 0))
		{
			temp[0] = ceil(output[itr][0]);
			temp[1] = ceil(output[itr][1]);

			if (x_itr < 640 && temp[0] == x_index_seq[x_itr])
			{
			    Storage(output[ino], temp[0], temp[1]+y_step);
				x_itr++;
			}
			else if (y_itr < 640 && temp[1] == y_index_seq[y_itr])
			{
			    Storage(output[ino], temp[0]+x_step, temp[1]);
				y_itr++;
			}
			else if (y_itr < 640 && temp[1] == y_index_seq[y_itr+1])
			{
                Storage(output[ino], temp[0]+x_step, temp[1]);
				y_itr++;
			}
			else if (x_itr < 640 && temp[0] == x_index_seq[x_itr+1])
			{
                Storage(output[ino], temp[0], temp[1]+y_step);
				x_itr++;
			}
			else
				break;
			itr = itr + 1;
		}
	}

	oGrid og;
	if (output[1][0] == 0 && output[0][0] > 0)
	{
		memcpy(og.empty_output, output, sizeof(output));
		og.occupied_output[0] = output[0][0];
		og.occupied_output[1] = output[0][1];
	}
	else
	{
		if (ino == 2)
		{
			og.empty_output[0][0] = output[0][0];
			og.empty_output[0][1] = output[0][1];
		}
		else
			memcpy(og.empty_output, output, sizeof(double)*(ino-1) * 2);
		og.occupied_output[0] = output[ino-1][0];
		og.occupied_output[1] = output[ino-1][1];
	}
	return og;
}
