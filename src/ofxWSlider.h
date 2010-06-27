/*
 * ofxYAButton.h
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */
#pragma once

#include "ofxWTypes.h"
#include "ofxWidget.h"
#include "ofxWStyleLoader.h"
#include "ofMain.h"


class ofxWSlider: public ofxWidget{
public:

	float  value;

	ofxWSlider(const string & name);

	void init(const string & _title, float * value, float _min, float _max, const string & style);
	void init(const string & _title, int * value, int _min, int _max, const string & style);
	void init(const string & _title, float _value, float _min, float _max, const string & style);
	void init(const string & _title, int _value, int _min, int _max, const string & style);

	void setValue(float _value);

	int getValueI();
	float getValueF();
	bool getValueB();

	void update();

	virtual void render(ofxWidgetsStyle & style);

	virtual ofRectangle getActiveArea(ofxWidgetsStyle & style);
	virtual ofRectangle getTotalArea(ofxWidgetsStyle & style);

	// UNFOCUSED -> over -> FOCUSED -> pressed -> PRESSED -> out -> OUT -> released -> UNFOCUSED
	// PRESSED -> released -> FOCUSSED
	// FOCUSED -> out ->  UNFOCUSED
	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);

	void saveTo(ofxXmlSettings & xml, const string & tag);
	void loadFrom(ofxXmlSettings & xml, const string & tag);
	
	
	ofEvent<int> 	intEvent;
	ofEvent<float>  floatEvent;

protected:
	bool intOnly;
	float min;
	float max;

	float * ftargetValue;
	int   * itargetValue;
};

