#include "eegHandler.h"

#include <array>
#include "utilities/advancedString.h"

eegHandler::eegHandler(){
	_ready = false;
}

eegHandler::~eegHandler(){
	_serial.close();
}

vector<ofSerialDeviceInfo> eegHandler::getAvailableDevices(){
	#ifdef SIMULATION_MODE
		vector<ofSerialDeviceInfo> devices;
		devices = vector<ofSerialDeviceInfo>(1);
		devices[0] = ofSerialDeviceInfo("SIMULATION", "SIMULATION", 0);

		return devices;
	#endif

	vector<ofSerialDeviceInfo> devicesList;
	_serial.listDevices();
	devicesList = _serial.getDeviceList();

	return devicesList;
}

bool eegHandler::startListeningDevice(string portName){
	#ifdef SIMULATION_MODE
		return true;
	#endif

	bool success = _serial.setup(portName, SERIAL_BUID);

	if(!success){
		ofLogError("eegHandler::startListeningDevice") << "could not open the port " + portName;
		return false;
	}

	ofLogNotice("eegHandler::startListeningDevice") << "successfully started listening on port " + portName;

	return true;
}

bool eegHandler::isReady(){
	#ifdef SIMULATION_MODE
		return true;
	#endif
	return _serial.isInitialized();
}

bool eegHandler::update(){
	#ifdef SIMULATION_MODE
		_eegDataSets.push(eegData());
		return true;
	#endif

	if(!isReady()){
		ofLogError("eegHandler::update") << "no serial connection";
		return false;
	}

	while(_serial.available()){
		unsigned char byte = _serial.readByte();
		bool endOfLine = (byte == '\r' || byte == '\n');

		if(endOfLine &&  _buffer.size() > 0){
			ofLogNotice("eegHandler::update") << _buffer;
			vector<string> bufferValues = ofSplitString(_buffer, ",");
			_buffer.clear();

			if(bufferValues.size() == eegData::NUMBER_OF_TERMS){
				_eegDataSets.push(eegData(bufferValues));
			}
		}else if(!endOfLine){
			_buffer += (char)byte;
		}
	}

	return true;
}

bool eegHandler::dataAvailable(){
	return (_eegDataSets.size() > 0);
}

eegData eegHandler::popData(){
	if(_eegDataSets.size() == 0){
		ofLogError("eegHandler::popData") << "no EEG data available";
		return eegData();
	}

	eegData res = _eegDataSets.front();
	_eegDataSets.pop();

	return res;
}