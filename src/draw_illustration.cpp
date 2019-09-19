#include <iostream>
#include <opencv2/opencv.hpp>
#include "draw_illustration.h"
#include "public.h"

using namespace cv;
using namespace std;

static double A[640][640] = { 0 };


void draw_illustration(int mi)
{	

	for (int i = 0; i < 640; i++)
	{
		for (int j = 0; j < 640; j++)
			A[i][j] = MAP[mi][640-i][j];
	}
	Mat frame = Mat(640, 640, CV_64FC1, A);
	Mat bigImage;

	resize(frame, bigImage, Size(frame.cols, frame.rows));
	imshow("Map", bigImage);
	waitKey(1);
	//destroyWindow("Map");
}
