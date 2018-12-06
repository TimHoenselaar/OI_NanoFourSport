
#include "PeakDetector.h"
#include <algorithm>
#include <vector>

///
/// @brief initialises all sample settings.
///
/// @param measureLength				Length of the sampling array, has to be an even number.
/// @param minimumSampleDifference		If The sample is lower then previous sampleDifference the sample is discarded.
/// @param minimumPeakThreshold			Minimum Threshold relative to Peakoffset. If Sample is lower than Peakthreshold 
///										no peak will be detected. this value is the same for positive and negative samples.
/// @param mimimumPeakOffset			Sets the baseline of the peakthreshold. 		
///
template <class T>
PeakDetector<T>::PeakDetector(int measureLength, T minimumSampleDifference, T minimumPeakThreshold, T mimimumPeakOffset)
{
    if (measureLength <= 4  || !(measureLength %2 == 0))	throw "measureLength";
    
    this->measureLength = measureLength;
    this->minimumPeakThreshold = minimumPeakThreshold;
    this->minimumSampleDifference = minimumSampleDifference;
	this->mimimumPeakOffset = mimimumPeakOffset;

    lastSample	= 0;
    currentDirection = 0;
    lastDetectedPeek = 0;
	
	peak = PeakType::noneDetected;
	Direction.resize(measureLength);	
	RawData.resize(measureLength);
}


template <class T>
PeakDetector<T>::~PeakDetector()
{
}

/// @brief Call Calculate(T sample) before Getting the peakvalue.
template <class T>
PeakType PeakDetector<T>::GetPeak()
{
	return peak;
}

/// @brief Call Calculate(T sample) before Getting the RawPeekValue.
template <class T>
T PeakDetector<T>::GetRawPeekValue()
{
	return RawData[measureLength / 2];
}


/// @brief Calculates the signal if a direction is detected. use GetPeak() to see if a peak is detected.
/// @param sample signal to calculate 	
template <class T>
void PeakDetector<T>::Calculate(T sample)
{
	peak = PeakType::noneDetected;
	lastDetectedPeek++;

    if      (lastSample < sample - minimumSampleDifference)		{ currentDirection = -1; }
    else if (lastSample > sample + minimumSampleDifference)		{ currentDirection =  1; }
    else														{ return; }

    lastSample = sample;
	
	std::rotate(Direction.begin(), Direction.begin() + 1, Direction.end());
	std::rotate(RawData.begin(), RawData.begin() + 1, RawData.end());

    Direction[measureLength - 1] = currentDirection;
	
    if (Total(Direction, measureLength) == 0 && lastDetectedPeek > measureLength)
    {
		if (sample >= mimimumPeakOffset + minimumPeakThreshold)
		{
			peak = PeakType::positive;
			lastDetectedPeek = 0;
		}
		else if (sample <= (mimimumPeakOffset + (minimumPeakThreshold - (minimumPeakThreshold *2))) )
		{
			peak = PeakType::negative;
			lastDetectedPeek = 0;
		}     
    }
}

/// @brief Adds all values of an Vector together
template <class T>
int PeakDetector<T>::Total(std::vector<int> array, size_t arrayLength)
{
	int average = 0;
	for (int i = 0; i < arrayLength; i++)
	{
		average += array[i];
	}
	return average;
}


/*
Code that invokes template functions must have matching
template function declarations. Declarations must include
the same template parameters as the definition. The following
sample generates LNK2019 on a user-defined operator, and shows
how to fix it.*/
template class PeakDetector<int>;
template class PeakDetector<unsigned int>;
template class PeakDetector<float>;
template class PeakDetector<double>;
template class PeakDetector<long>;