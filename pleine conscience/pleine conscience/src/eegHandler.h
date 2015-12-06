#pragma once

#define SIMULATION_MODE true

#include "ofMain.h"
#include "eegData.h"

/// <summary>
/// A handler class for the eeg headset abstracting the serial communication and parsing
/// </summary>
class eegHandler{
	public:
		eegHandler();
		~eegHandler();

		/// <summary>
		/// Get serial devices for user to select the eeg headset in a GUI
		/// </summary>
		/// <returns>devices list</returns>
		vector<ofSerialDeviceInfo> getAvailableDevices();
		/// <summary>
		/// Use to setup the arduino's serial communication on the given port
		/// </summary>
		/// <param name="portName"></param>
		/// <returns>success of the port opening</returns>
		bool startListeningDevice(string portName);
		/// <summary>
		/// 
		/// </summary>
		/// <returns>ready to read device</returns>
		bool isReady();
		/// <summary>
		/// Should be called every frame to read the data on the serial port
		/// </summary>
		/// <returns>whether the serial port listening went smoothly</returns>
		bool update();
		/// <summary>
		/// 
		/// </summary>
		/// <returns>data set in the eeg data queue</returns>
		bool dataAvailable();
		/// <summary>
		/// Return and pop data from the queue
		/// </summary>
		/// <returns>eeg data set, random if no eeg data is available</returns>
		eegData popData();

	private:
		bool _ready;
		std::queue<eegData> _eegDataSets;
		ofSerial _serial;
		const int SERIAL_BUID = 9600;
		string _buffer;
};

