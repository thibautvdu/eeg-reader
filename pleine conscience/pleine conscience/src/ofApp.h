#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "eegHandler.h"
#include "ofxMultiPlot.h"

class ofApp : public ofBaseApp{

	public:
		// OF ROUNTINES //
		void setup();
		void update();
		void draw();
		void exit();
	
		// GUI //
		/// <summary>
		/// Display the available serial ports
		/// </summary>
		void listDevices();
		/// <summary>
		/// Callback, when the user select a serial port
		/// </summary>
		/// <param name="sender"></param>
		void onDeviceSelected(const void * sender);

		enum interfaceStep {NONE, DEVICE_SELECTION, DRAWING_GRAPH};
		interfaceStep _interfaceStep = NONE;
		ofxPanel _deviceSelectPanel;
		vector<ofxButton> _devicesButtons;

		// STYLE //
		ofxButton _buttonStyle;

		// DEVICE //
		eegHandler _eegHeadset;

		// DATA //
		stringstream _eegSessionBuffer;
		ofxMultiPlot _multiPlot;
};
