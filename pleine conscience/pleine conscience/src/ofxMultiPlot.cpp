#include "ofxMultiPlot.h"

ofxMultiPlot::ofxMultiPlot() {
	if(ofIsGLProgrammableRenderer()){
		_dashedLineShader.setGeometryInputType(GL_LINES);
		_dashedLineShader.setGeometryOutputType(GL_LINES);
		_dashedLineShader.load("ofxMultiPlot/dashedLine.vert", "ofxMultiPlot/dashedLine.frag", "ofxMultiPlot/dashedLine.geo");

		_dottedLineShader.setGeometryInputType(GL_LINES);
		_dottedLineShader.setGeometryOutputType(GL_POINT);
		_dottedLineShader.load("ofxMultiPlot/dottedLine.vert", "ofxMultiPlot/dottedLine.frag", "ofxMultiPlot/dottedLine.geo");

		_crossedLineShader.setGeometryInputType(GL_LINES);
		_crossedLineShader.setGeometryOutputType(GL_LINES);
		_crossedLineShader.load("ofxMultiPlot/crossedLine.vert", "ofxMultiPlot/crossedLine.frag", "ofxMultiPlot/crossedLine.geo");
	}
	else{
		ofLogError("ofxMultiPlot::ofxMultiPlot") << "Need programmable renderer";
	}
}

ofxMultiPlot * ofxMultiPlot::setup(int windowSize, ofColor backgroundColor){
	_windowSize = windowSize;
	_backgroundColor = backgroundColor;

	// GUI //
	ofPtr<ofxBaseGui> windowSizeSlider(new ofxIntSlider());
	((ofxIntSlider*)windowSizeSlider.get())->setup(_windowSize.set("window size", windowSize, 5, 100));
	_guiControls.push_back(windowSizeSlider);

	return this;
}

ofxMultiPlot::~ofxMultiPlot(){
	for(graph* gr : _graphs){
		free(gr);
	}
	_dashedLineShader.unload();
}

int ofxMultiPlot::addGraph(string name, ofColor color, lineStyle linestyle){
	graph*  newGraph = new graph();
	newGraph->color = color;
	newGraph->linestyle = linestyle;

	_graphs.push_back(newGraph);

	string icon;
	switch(linestyle){
		case PLAIN:
			icon = "____";
			break;
		case TIDY_DASH:
			icon = "----";
			break;
		case DASH:
			icon = "- - ";
			break;
		case SPACED_DASH:
			icon = "-  -";
			break;
		case TIDY_DOT:
			icon = "....";
			break;
		case DOT:
			icon = ". . ";
			break;
		case SPACED_DOT:
			icon = ".  .";
			break;
		case CROSS:
			icon = "++++";
	}
	ofPtr<ofxBaseGui> newToggle(new ofxToggle());
	((ofxToggle*)newToggle.get())->setup(newGraph->visible.set(" " + icon + " " + name, false));
	_guiControls.push_back(newToggle);

	return (_graphs.size() - 1);
}

bool ofxMultiPlot::addGraphData(int graphIndex, float value){
	if(graphIndex >= _graphs.size()){
		ofLogError("ofxMultiPlot::addData") << "Graph index is out of bound";
		return false;
	}

	if(value < _dataValueLowBound){
		_dataValueLowBound = value;
	} else if(value > _dataValueHighBound){
		_dataValueHighBound = value;
	}

	_graphs[graphIndex]->dataf.push_back(value);
	generateDraw();

	return true;
}

void ofxMultiPlot::generateDraw(){
	for(graph* gr : _graphs){
		int lowBound = max<int>(gr->dataf.size() - _windowSize - 1, 0);
		int highBound = gr->dataf.size() - 1;
		gr->constructMesh(lowBound, highBound, _dataValueHighBound);
	}
}

void ofxMultiPlot::draw(float x, float y, float width, float height){
	if(_graphs.size() == 0)
		return;

	// COMPUTE SPACE FOR GUI
	float totalLength = 0;
	for(ofPtr<ofxBaseGui> guiElem : _guiControls){
		totalLength += guiElem->getWidth() + GUI_PADDING;
	}
	int numLines = floor(totalLength / width);

	height -= (numLines * (_guiControls[0]->getHeight()+GUI_PADDING)); // keep space for controls

	ofPushMatrix();
		ofTranslate(x, y);

		// DRAW BACKGROUND
		ofSetColor(_backgroundColor);
		ofRect(0,0, width, height);

		ofSetLineWidth(_lineWidth);

		ofPushMatrix();
			float xScale = width / (float)_windowSize;
			float yScale = height / (_dataValueHighBound - _dataValueLowBound);
			ofScale(xScale, yScale);

			// DRAW DATA
			for(graph* gr : _graphs){
				ofSetColor(gr->color);
				if(gr->mesh.getNumVertices() > 0 && gr->visible){
					switch(gr->linestyle){
						case lineStyle::PLAIN:
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							break;
						case lineStyle::TIDY_DASH:
							_dashedLineShader.begin();
								_dashedLineShader.setUniform1i("dashLength", 4);
								gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
								gr->mesh.draw();
							_dashedLineShader.end();
							break;
						case lineStyle::DASH:
							_dashedLineShader.begin();
							_dashedLineShader.setUniform1i("dashLength", 8);
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_dashedLineShader.end();
							break;
						case lineStyle::SPACED_DASH:
							_dashedLineShader.begin();
							_dashedLineShader.setUniform1i("dashLength", 12);
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_dashedLineShader.end();
							break;
						case lineStyle::TIDY_DOT:
							_dottedLineShader.begin();
							_dottedLineShader.setUniform1i("spaceLength", 4);
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_dottedLineShader.end();
							break;
						case lineStyle::DOT:
							_dottedLineShader.begin();
							_dottedLineShader.setUniform1i("spaceLength", 8);
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_dottedLineShader.end();
							break;
						case lineStyle::SPACED_DOT:
							_dottedLineShader.begin();
							_dottedLineShader.setUniform1i("spaceLength", 12);
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_dottedLineShader.end();
							break;
						case lineStyle::CROSS:
							_crossedLineShader.begin();
							gr->mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
							gr->mesh.draw();
							_crossedLineShader.end();
							break;
					}

				}
			}
		ofPopMatrix();
	ofPopMatrix();

	// DRAW 
	int xCopy = x;
	xCopy += GUI_PADDING;
	height += GUI_PADDING;
	for(ofPtr<ofxBaseGui> guiElem : _guiControls){
		float elementWidth = guiElem->getWidth() + GUI_PADDING;

		if(xCopy + elementWidth > x + width){ // next line
			xCopy = x + GUI_PADDING;
			height += GUI_PADDING + guiElem->getHeight();
		}

		guiElem->setPosition(xCopy, y + height);
		guiElem->draw();
		xCopy += elementWidth;
	}
}