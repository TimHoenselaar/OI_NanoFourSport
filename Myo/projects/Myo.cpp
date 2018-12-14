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
#include "MovingAverage.h"
#include "Serialport.h"

#include "windows.h."


//  
constexpr auto Connected = true;
const char filename[] = "test.txt ";
std::ofstream file;
bool running;

int display = 0;
int relativeTime = 0;
int EMG_Data[8];


void commandLineUpdate(int updatefreq, DataCollector data);
void WriteToFile(DataCollector data, bool StepDetected, int muscleTension);
void UDPStringBuilder(DataCollector data, bool StepDetected, int muscleTension, std::string &messageToSend);
void cls();

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
		running = UDP_visualApp.Start();

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

		// clear EMG_data_holder
		for (size_t i = 0; i < 8; i++) { EMG_Data[i] = 0; }

		// initalisation complete  
		// initalisation complete  
		//-----------------------------------------------------------------------------// 

		// Add the Stepcalculator 
		PeakDetector<float> WaveDetector = PeakDetector<float>(8, 5, 100, 0);
		//PeakDetector<float> WaveDetector = PeakDetector<float>(6, (float)0.11, (float)0.4, 0);

		// Arm muscle tension detection
		MovingAverage averageFilter = MovingAverage(20);

		//accel testttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
		char incomingData[MAX_DATA_LENGTH];
		const char *port_name = "\\\\.\\COM7";
		SerialPort arduino(port_name);
		if (arduino.isConnected()) std::cout << "Connection Established" << std::endl;
		else std::cout << "ERROR, check port name";
		char send = '1';

		std::vector<std::string> accelData; //x = 0, y = 1, z = 2

		
		
		// main program loop 
		while (1)
		{
			Sleep(10);

			//accellllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
			arduino.writeSerialPort(&send, 1);
			int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
			if (read_result != -1)
			{
				accelData = arduino.Split(incomingData, ',');
				for (size_t i = 0; i < 3; i++)
				{
					std::cout << accelData[i] << std::endl;

				}
			}

			// Sample time setting 
			hub.run(1000 / 60);
			relativeTime = std::clock();
			
			// Runner StepDetection  
			WaveDetector.Calculate(collector.getGyroscope().z());
			bool StepDetected = false;
			if (WaveDetector.GetPeak() == PeakType::positive) StepDetected = true;
			if (WaveDetector.GetPeak() == PeakType::negative) StepDetected = true;

			// Arm muscle tension detection 
		    collector.getEMG(EMG_Data);
			double armTension = 0;
			for (int i = 0; i < 8; i++)
			{
				double positiveTension = sqrt(EMG_Data[i]* EMG_Data[i]);
				armTension = armTension + positiveTension;
			}
			averageFilter.add(armTension);
			armTension = averageFilter.getCurrentAverage();

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
		//system("CLS");
		cls();

		// header 
		std::cout << "\n				Nano4Sports - Fontys OI\n\n";
		if (data.getConnectionStatus()) { std::cout << "					Connected\n\n"; }
		else { std::cout << "					Disconnected \n\n"; }
		std::cout << "		Battery: " << (int)data.getBatteryLevel() << "		"
			<< "	Bluetooth Signal Strength: " << (int)data.getBluetoothRange() << "\n \n";


		data.getEMG(EMG_Data);

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
			<< "			EMG_DATA[7]		: " << (int)EMG_Data[7] << "\n\n"
			<< "			UDP running		: " << running << "\n\n\n";


		
		// footer 
		std::cout << "		Writing to " << filename << "\n";
	}
}

void WriteToFile(DataCollector data, bool StepDetected, int muscleTension)
{
	data.getEMG(EMG_Data);

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
		<< EMG_Data[0] << ","
		<< EMG_Data[1] << ","
		<< EMG_Data[2] << ","
		<< EMG_Data[3] << ","
		<< EMG_Data[4] << ","
		<< EMG_Data[5] << ","
		<< EMG_Data[6] << ","
		<< EMG_Data[7] << ","
		<< stepdata << ","				// calculated 
		<< muscleTension << ","			// calculated 
		<< relativeTime << "\n";

}

void UDPStringBuilder(DataCollector data, bool StepDetected, int muscleTension , std::string &messageToSend)
{
	data.getEMG(EMG_Data);
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
	measurement.append(std::to_string(EMG_Data[0])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[1])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[2])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[3])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[4])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[5])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[6])); measurement.append(" ");
	measurement.append(std::to_string(EMG_Data[7])); measurement.append(" ");

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

	std::cout << "*";
	
}

void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}