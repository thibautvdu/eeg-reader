#include "ofApp.h"
#include <sstream>

//--------------------------------------------------------------
void ofApp::setup(){
	// DATA //
	ofDirectory sessionDataDir = ofDirectory(ofToDataPath("eeg sessions"));
	if(!sessionDataDir.exists()){
		sessionDataDir.create();
	}

	// GUI //
	ofSetBackgroundColor(ofColor::white);
	_interfaceStep = DEVICE_SELECTION;
	listDevices();
}

void ofApp::listDevices(){
	_deviceSelectPanel.clear();
	_deviceSelectPanel.setup("Select EGG device");

	vector<ofSerialDeviceInfo> devices = _eegHeadset.getAvailableDevices();
	_devicesButtons = vector<ofxButton>(devices.size());

	for(int i = 0; i < _devicesButtons.size(); i++){
		_devicesButtons[i].addListener(this, &ofApp::onDeviceSelected);
		_deviceSelectPanel.add(_devicesButtons[i].setup(devices[i].getDeviceName()));
	}
}

void ofApp::onDeviceSelected(const void * sender){
	ofxButton * button = (ofxButton*)sender;
	bool listening = _eegHeadset.startListeningDevice(button->getName());
	
	if(!listening){
		listDevices();
	}else{
		_interfaceStep = DRAWING_GRAPH;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(_eegHeadset.isReady()){
		_eegHeadset.update();

		while(_eegHeadset.dataAvailable()){
			eegData data = _eegHeadset.popData();
			_eegSessionBuffer << data;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(_interfaceStep == DEVICE_SELECTION){
		_deviceSelectPanel.draw();
	}else if(_interfaceStep == DRAWING_GRAPH){
	}
}

void ofApp::exit(){
	ofFileDialogResult dialogResult =  ofSystemSaveDialog("EEG SESSION DATA", "select a path to save the eeg session data");
	if(dialogResult.bSuccess){
		ofFile sessionFile = ofFile(ofToDataPath(dialogResult.filePath), ofFile::WriteOnly);
		if(!sessionFile.exists()){
			sessionFile.create();
		}
		sessionFile << _eegSessionBuffer;
		sessionFile.close();
		ofLogNotice("ofApp::exit") << "wrote session to " + dialogResult.filePath;
	}
}