#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <myo/myo.hpp>
#include <array>



/*!
 *  @brief     Contains all device information about a Myo Bracelet.
 *  @details   It inherits all save data functions from myo::DeviceListener, so 
 *			   to initalise the class from recieving data you have to put this 
 *			   line afer Myo initialisation: myo::hub.addListener(&datacollector); 
 *  @author    Ryan Vrosch
 *  @version   1.1
 *  @date      2018-11-30
 *  @warning   Some params dont update themselves, and need to be called via myo::Myo.
 *  @copyright GNU Public License.
 */
class DataCollector : public myo::DeviceListener
{

private:

	// raw data
	myo::Quaternion<float> rotation_r;
	myo::Vector3<float> gyroscope;
	myo::Vector3<float> accelerometer;
	std::array<int8_t, 8> EMG;

	// calculated data
	float	rotation_roll;
	float	rotation_pitch;
	float	rotation_yaw;

	// device information
	uint8_t batteryLevel;
	int8_t BluetoothRange;
	bool connectionStatus;

public:

	DataCollector();

	// Sensor			(inherited from DeviceListener): needed for updating values
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation);
	void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel);
	void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro);
	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg);

	// device status	(inherited from DeviceListener): needed for updating values
	void onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level);
	void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi);
	void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion);
	void onDisconnect(myo::Myo* myo, uint64_t timestamp);

	// Raw SensorValues
	/// @brief Gets rotation values. (updates automatically if class is conneced to myo::Hub)
	/// @return myo::Quaternion<float> use raw value: .x() .y() .z() .w().
	myo::Quaternion<float>	getRotation()		{ return rotation_r; }
	/// @brief Gets the Gyroscope values. (updates automatically if class is conneced to myo::Hub)
	/// @return accel myo::Vector3<float> use raw value : .x() .y() .z().
	myo::Vector3<float>		getGyroscope()		{ return gyroscope; }
	/// @brief Gets all 8 EMG sensor Values. (updates automatically if class is conneced to myo::Hub)
	/// @return accel myo::Vector3<float> use raw value : .x() .y() .z().
	myo::Vector3<float>		getAccelerometer()	{ return accelerometer; }
	/// @brief Gets all 8 EMG sensor Values. (updates automatically if class is conneced to myo::Hub)
	/// @param[in,out] data  returns emg data if *data != null.
	void					getEMG( std::array<int8_t, 8> *data)
												{ if (data != NULL)	*data = EMG; }

	// calculated data
	/// @brief  Calculated Roll from rotation data
	/// @return Roll in radial.
	float getRotation_roll()	{ return rotation_roll; }
	/// @brief  Calculated pitch from rotation data
	/// @return Pitch in radial.
	float getRotation_pitch()	{ return rotation_pitch; }
	/// @brief  Calculated yaw from rotation data
	/// @return Yaw in radial.
	float getRotation_yaw()		{ return rotation_yaw; }

	//device status
	/// @brief Battery: This value wont update periodically. update via MYO::requestBatteryLevel()
	/// @return Battery Level in procentage.
	uint8_t	getBatteryLevel()	{ return batteryLevel; }
	/// @brief  Bluetooth: This value wont update periodically. update via update via MYO::requestRssi();
	/// @return bluetooth range 0-127.
	int8_t	getBluetoothRange()	{ return BluetoothRange; }
	/// @brief This value is automaticly updated.
	/// @return connection status (bool) true = Connected | false = disconnected.
	bool getConnectionStatus()  { return connectionStatus; }
};

#endif //DATACOLLECTOR_H