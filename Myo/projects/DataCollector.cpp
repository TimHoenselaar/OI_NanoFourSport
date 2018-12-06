
#include "DataCollector.h"
#include <cmath>

DataCollector::DataCollector()
{
	connectionStatus	= false;
	rotation_roll		= 0;
	rotation_pitch		= 0;
	rotation_yaw		= 0;
	batteryLevel		= 0;
	BluetoothRange		= 0;
	EMG.empty();
}


void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation)
{	
	rotation_r = rotation;
	rotation_r.fromAxisAngle;

	rotation_roll	= std::atan2(2.0f * (rotation.w() * rotation.x() + rotation.y() * rotation.z()),
						  1.0f - 2.0f * (rotation.x() * rotation.x() + rotation.y() * rotation.y()));
	rotation_yaw	= std::atan2(2.0f * (rotation.w() * rotation.z() + rotation.x() * rotation.y()),
						  1.0f - 2.0f * (rotation.y() * rotation.y() + rotation.z() * rotation.z()));
	rotation_pitch = std::asin(std::max(-1.0f, std::min(1.0f, 2.0f * 
										(rotation.w() * rotation.y() - rotation.z() * rotation.x()))));
}


void DataCollector::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
{
	accelerometer = accel;
}


void DataCollector::onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
{
	gyroscope = gyro;
}


void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
	for (int i = 0; i < 8; i++) 
	{
		EMG[i] = emg[i];
	}
}


void DataCollector::onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level)
{
	batteryLevel = level;
}


void DataCollector::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi)
{	
	BluetoothRange = rssi;
}


void DataCollector::onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
	connectionStatus = true;
}


void DataCollector::onDisconnect(myo::Myo* myo, uint64_t timestamp)
{
	connectionStatus = false;
}
