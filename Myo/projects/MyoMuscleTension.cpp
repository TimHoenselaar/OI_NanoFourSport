#include "MyoMuscleTension.h"
#include <math.h>
#include <array>





MyoMuscleTension::MyoMuscleTension(int movAvr)
{
	filter = MovingAverage(movAvr);

}

int MyoMuscleTension::calculate(std::array<int8_t, 8> data)
{
	//if (data.empty()) return 0;

	int Tension = 0;
	for (size_t i = 0; i < data.size()-1; i++)
	{
		int positiveTension = sqrt(pow(data[i],2));
		Tension += positiveTension;
	}

	filter.add(Tension);
	return filter.getCurrentAverage();
}


MyoMuscleTension::~MyoMuscleTension()
{
}
