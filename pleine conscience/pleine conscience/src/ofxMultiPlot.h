#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxGui.h"

class ofxMultiPlot{
	public:
		ofxMultiPlot();
		ofxMultiPlot * setup(int windowSize, ofColor backgroundColor);
		~ofxMultiPlot();

		enum lineStyle{
			PLAIN, TIDY_DASH, DASH, SPACED_DASH, TIDY_DOT, DOT, SPACED_DOT, CROSS
		};

		/// <summary>
		/// Add a new collection, that is a new graph
		/// </summary>
		/// <param name="color"></param>
		/// <param name="lowBound">min value expected</param>
		/// <param name="highBound">max value expected</param>
		/// <returns>the index of the new graph in this instance</returns>
		int addGraph(string name, ofColor color, lineStyle linestyle);
		/// <summary>
		/// Add a value to the graph of given index
		/// </summary>
		/// <param name="entryIndex">the index of the graph in this instance</param>
		/// <param name="value"></param>
		/// <returns>success</returns>
		bool addGraphData(int entryIndex, float value);

		void draw(float x, float y, float width, float height);

	protected:
		/// <summary>
		/// Regenerate the mesh with current data
		/// </summary>
		void generateDraw();

		// STYLE //
		float _lineWidth = 1;
		ofColor _backgroundColor;
		const float GUI_PADDING = 3;

		// GUI //
		vector<ofPtr<ofxBaseGui>> _guiControls;

		// PARAMETERS //
		ofParameter<int> _windowSize;
		float _dataValueLowBound = 0;
		float _dataValueHighBound = 1;

		// DATA //
		/// <summary>
		/// Represent a graph
		/// </summary>
		class graph{
		public :
			ofColor color;
			vector<float> dataf;
			ofMesh mesh;
			lineStyle linestyle = PLAIN;
			ofParameter<bool> visible;

			void switchVisibility(const void* sender, bool & visibility){
				visible = visibility;
			}

			void constructMesh(int lowBound, int highBound, float dataValueHighBound){
				if(lowBound < 0){
					ofLogError("ofxMultiPlot::collection::constructMesh", "low bound is negative " + lowBound);
					return;
				}

				mesh.clear();
				for(int i = lowBound; i <= highBound &&  i < dataf.size(); i++){
					mesh.addVertex(ofVec3f((i - lowBound), dataValueHighBound - dataf[i]));
				}
			}
		};
		vector<graph*> _graphs;

		// SHADERS //
		ofShader _dashedLineShader;
		ofShader _dottedLineShader;
		ofShader _crossedLineShader;
};

