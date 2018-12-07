#define _USE_MATH_DEFINES 
#include <iostream> 
#include <iomanip> 
#include <stdexcept> 
#include <string> 
#include <string.h> 
#include <fstream> 

#include <myo/myo.hpp>			// The only file that needs to be included to use the Myo C++ SDK is myo.hpp. 
#include "UDPClient.h"			// warning ! needs to be the first file that includes <windows.h> 
#include "DataCollector.h" 
#include "Communicator.h" 
#include "PeakDetector.h" 

#include "windows.h."


//  
constexpr auto Connected = true;
const char filename[] = "test.txt ";
std::ofstream file;

int display = 0;
int relativeTime = 0;


void commandLineUpdate(int updatefreq, DataCollector data);
void WriteToFile(DataCollector data, bool StepDetected, int muscleTension);
void UDPStringBuilder(DataCollector data, bool StepDetected, int muscleTension, std::string &messageToSend);

int main(int argc, char** argv)
{
	// We catch any exceptions that might occur below -- see the catch statement for more details. 
	try {
		myo::Hub hub("com.example.hello-myo");

		std::cout << "Attempting to find a Myo..." << std::endl;

		// connecting to Myo bracelet. 
		myo::Myo* myo = hub.waitForMyo(10000);
		if (!myo) throw std::runtime_error("Unable to find a Myo!");

		// adding a data listner to the bracelet. 
		DataCollector collector;
		hub.addListener(&collector);

		// connecting to udp visualised app 
		UDPClient UDP_visualApp = UDPClient("127.0.0.1", 1111);

		//open file to collect sample data. 
		file.open(filename);
		file << "roll,pitch,yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, EMG0, EMG1, EMG2, EMG3, EMG4, EMG5, EMG6, EMG7, stepDetect, muscleTension, time\n";

		// initialise baterylevel and bluetoothRange for the collector  
		myo->requestBatteryLevel();
		myo->requestRssi();

		// turn on emg data transfer 
		myo->setStreamEmg(myo->streamEmgEnabled);

		// vibrate once to let the user know initalisation is complete. 
		myo->vibrate(myo::Myo::vibrationShort);

		// initalisation complete  
		//-----------------------------------------------------------------------------// 

		// Add the Stepcalculator 
		PeakDetector<float> WaveDetector = PeakDetector<float>(8, 5, 40, 0);
		//PeakDetector<float> WaveDetector = PeakDetector<float>(6, (float)0.11, (float)0.4, 0);

		// Add Arm muscle tension detection 
		// 
		// 
		// 


		// main program loop 
		while (1)
		{
			Sleep(10);
			// Sample time setting 
			hub.run(1000 / 60);
			relativeTime = std::clock();
			
			// Runner StepDetection  
			WaveDetector.Calculate(collector.getGyroscope().z());
			bool StepDetected = false;
			if (WaveDetector.GetPeak() == PeakType::positive) StepDetected = true;
			if (WaveDetector.GetPeak() == PeakType::negative) StepDetected = true;

			// Arm muscle tension detection 
			int armTension = 0;
			// 
			// 
			// 
			// 
			// ---------------- 


			// saving, sending and updating data. 
			commandLineUpdate(20, collector);
			WriteToFile(collector, StepDetected, armTension);

			std::string UDP_sendMessage;
			UDPStringBuilder(collector, StepDetected, armTension, UDP_sendMessage);
			UDP_visualApp.Write(UDP_sendMessage.c_str(), UDP_sendMessage.length());

		}
	}
	// If a standard exception occurred, we print out its message and exit. 
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Did you try starting the Myo application?";
		std::cin.ignore();
		return 1;
	}
}





void commandLineUpdate(int updatefreq, DataCollector data)
{
	display++;
	if (display > updatefreq)
	{
		display = 0;
		system("CLS");

		// header 
		std::cout << "\n				Nano4Sports - Fontys OI\n\n";
		if (data.getConnectionStatus()) { std::cout << "					Connected\n\n"; }
		else { std::cout << "					Disconnected \n\n"; }
		std::cout << "		Battery: " << (int)data.getBatteryLevel() << "		"
			<< "	Bluetooth Signal Strength: " << (int)data.getBluetoothRange() << "\n \n";

		std::array<int8_t, 8> EMG_Data;
		data.getEMG(&EMG_Data);

		// Measurements  
		std::cout
			<< "			Rotation_roll		: " << data.getRotation_roll() << "\n"
			<< "			Rotation_pitch		: " << data.getRotation_pitch() << "\n"
			<< "			Rotation_yaw		: " << data.getRotation_yaw() << "\n"
			<< "			Gyroscope x		: " << data.getGyroscope().x() << "\n"
			<< "			Gyroscope y		: " << data.getGyroscope().y() << "\n"
			<< "			Gyroscope z		: " << data.getGyroscope().z() << "\n"
			<< "			Accelerometer x		: " << data.getAccelerometer().x() << "\n"
			<< "			Accelerometer y		: " << data.getAccelerometer().y() << "\n"
			<< "			Accelerometer z		: " << data.getAccelerometer().z() << "\n"
			<< "			Relative Time		: " << relativeTime << "\n\n"
			<< "			EMG_DATA[0]		: " << (int)EMG_Data[0] << "\n"
			<< "			EMG_DATA[1]		: " << (int)EMG_Data[1] << "\n"
			<< "			EMG_DATA[2]		: " << (int)EMG_Data[2] << "\n"
			<< "			EMG_DATA[3]		: " << (int)EMG_Data[3] << "\n"
			<< "			EMG_DATA[4]		: " << (int)EMG_Data[4] << "\n"
			<< "			EMG_DATA[5]		: " << (int)EMG_Data[5] << "\n"
			<< "			EMG_DATA[6]		: " << (int)EMG_Data[6] << "\n"
			<< "			EMG_DATA[7]		: " << (int)EMG_Data[7] << "\n\n\n";

		
		// footer 
		std::cout << "		Writing to " << filename << "\n";
	}
}

void WriteToFile(DataCollector data, bool StepDetected, int muscleTension)
{
	std::array<int8_t, 8> EMG_Data;
	data.getEMG(&EMG_Data);

	int stepdata = 0;
	if (StepDetected) stepdata = 1;

	file << data.getRotation_roll() << ","
		<< data.getRotation_pitch() << ","
		<< data.getRotation_yaw() << ","
		<< data.getGyroscope().x() << ","
		<< data.getGyroscope().y() << ","
		<< data.getGyroscope().z() << ","
		<< data.getAccelerometer().x() << ","
		<< data.getAccelerometer().y() << ","
		<< data.getAccelerometer().z() << ","
		<< (int)EMG_Data[0] << ","
		<< (int)EMG_Data[1] << ","
		<< (int)EMG_Data[2] << ","
		<< (int)EMG_Data[3] << ","
		<< (int)EMG_Data[4] << ","
		<< (int)EMG_Data[5] << ","
		<< (int)EMG_Data[6] << ","
		<< (int)EMG_Data[7] << ","
		<< stepdata << ","				// calculated 
		<< muscleTension << ","			// calculated 
		<< relativeTime << "\n";

}

void UDPStringBuilder(DataCollector data, bool StepDetected, int muscleTension , std::string &messageToSend)
{
	std::array<int8_t, 8> EMG_Data;
	data.getEMG(&EMG_Data);
	std::string measurement;

	//Rotation (from gyroscope) 
	measurement.append(std::to_string(data.getRotation_roll())); measurement.append(" ");
	measurement.append(std::to_string(data.getRotation_pitch())); measurement.append(" ");
	measurement.append(std::to_string(data.getRotation_yaw())); measurement.append(" ");

	// Gyroscope 
	measurement.append(std::to_string(data.getGyroscope().x())); measurement.append(" ");
	measurement.append(std::to_string(data.getGyroscope().y())); measurement.append(" ");
	measurement.append(std::to_string(data.getGyroscope().z())); measurement.append(" ");

	//Accelerometer 
	measurement.append(std::to_string(data.getAccelerometer().x())); measurement.append(" ");
	measurement.append(std::to_string(data.getAccelerometer().y())); measurement.append(" ");
	measurement.append(std::to_string(data.getAccelerometer().z())); measurement.append(" ");

	//  some EMG data 
	measurement.append(std::to_string((int)EMG_Data[0])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[1])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[2])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[3])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[4])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[5])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[6])); measurement.append(" ");
	measurement.append(std::to_string((int)EMG_Data[7])); measurement.append(" ");

	// stepdetect 
	int stepdata = 0;
	if (StepDetected) stepdata = 10;
	measurement.append(std::to_string(stepdata)); measurement.append(" ");

	// muscleTension 
	measurement.append(std::to_string(muscleTension)); measurement.append(" ");

	// time  
	measurement.append(std::to_string(relativeTime)); measurement.append(" \n");
	
	// sending.
	messageToSend = measurement;

	std::cout << "* ";
	
}