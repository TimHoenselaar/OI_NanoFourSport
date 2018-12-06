#ifndef PEAKDETECTOR_H
#define PEAKDETECTOR_H

#include <vector>

enum PeakType: uint8_t { positive, negative, noneDetected};

/*!
 *  @brief     Detects peaks in realtime signals by calculating the direction of a signal.
 *  @details   
 *			  
 *			   
 *  @author    Ryan Vrosch & Tim Hoenselaar
 *  @version   1.3
 *  @date      2018-11-30
 *  @copyright GNU Public License.
 */
template <class T>
class PeakDetector
{
	public:
	PeakDetector(int measureLength, T minimumSampleDifference, T minimumPeakThreshold, T mimimumPeakOffset);
	~PeakDetector();


	void Calculate(T Sample);
	PeakType GetPeak();
	T GetRawPeekValue();

private:
	int Total(std::vector<int> array, size_t arrayLength);

	int measureLength;
	T minimumSampleDifference;
	T minimumPeakThreshold;
	T mimimumPeakOffset;

	T lastSample;
	PeakType peak;


    int currentDirection;
    std::vector<int> Direction;
	std::vector<T> RawData;
    int lastDetectedPeek;
};

#endif
