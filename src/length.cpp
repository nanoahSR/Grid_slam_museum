//#include "stdafx.h"
#include <iostream>
#include "length.h"
using namespace std;

template<class T>
int length(T& arr)
{
	return sizeof(arr) / sizeof(arr[0]);
};
