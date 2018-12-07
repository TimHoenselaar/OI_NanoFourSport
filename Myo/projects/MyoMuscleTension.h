#pragma once

#include <algorithm>
#include "MovingAverage.h"

class MyoMuscleTension
{



public:
	MyoMuscleTension(int movAvr);
	int calculate(std::array<int8_t, 8> data);
	MovingAverage filter;
	~MyoMuscleTension();


private:
};

