#include "ofApp.h"
#include <sstream>

//--------------------------------------------------------------
void ofApp::setup(){
	// OPENGL //
	ofSetFrameRate(30);

	// DATA //
	ofDirectory sessionDataDir = ofDirectory(ofToDataPath("eeg sessions"));
	if(!sessionDataDir.exists()){
		sessionDataDir.create();
	}

	// GUI //
	ofSetBackgroundColor(ofColor::white);
	_deviceSelectPanel.setup();
	_deviceSelectPanel.setDefaultBackgroundColor(ofColor::black);
	_deviceSelectPanel.setDefaultHeaderBackgroundColor(ofColor::black);
	_deviceSelectPanel.setDefaultBorderColor(ofColor::black);
	_deviceSelectPanel.setDefaultTextColor(ofColor::white);
	_deviceSelectPanel.setDefaultFillColor(ofColor(237, 202, 56));
	_multiPlot.setup(20, ofColor::black);
	_multiPlot.addGraph("low alpha", ofColor::white, ofxMultiPlot::lineStyle::DASH);
	_multiPlot.addGraph("high alpha", ofColor::white, ofxMultiPlot::lineStyle::CROSS);
	_multiPlot.addGraph("meditation", ofColor::white, ofxMultiPlot::lineStyle::PLAIN);
	_multiPlot.addGraph("low alpha", ofColor::white, ofxMultiPlot::lineStyle::DASH);
	_multiPlot.addGraph("high alpha", ofColor::white, ofxMultiPlot::lineStyle::CROSS);
	_multiPlot.addGraph("meditation", ofColor::white, ofxMultiPlot::lineStyle::PLAIN);
	//_multiPlot.addEntry(ofColor::white);

	_interfaceStep = DEVICE_SELECTION;
	listDevices();
}

void ofApp::listDevices(){
	_deviceSelectPanel.clear();

	vector<ofSerialDeviceInfo> devices = _eegHeadset.getAvailableDevices();
	_devicesButtons = vector<ofxButton>(devices.size(), _buttonStyle);

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
			_multiPlot.addGraphData(0, (int)data.LowAlpha);
			_multiPlot.addGraphData(1, (int)data.HighAlpha);
			_multiPlot.addGraphData(2, (int)data.Meditation);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(_interfaceStep == DEVICE_SELECTION){
		_deviceSelectPanel.draw();
	}else if(_interfaceStep == DRAWING_GRAPH){
		_multiPlot.draw(0, 0, ofGetWidth(), ofGetHeight() * 0.2);
	}
}

void ofApp::exit(){
	ofGetWindowPtr()->setFullscreen(false);

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