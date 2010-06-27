/*
 * ofxYAButton.h
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */
#pragma once

#include "ofxWTypes.h"
#include "ofxWidget.h"
#include "ofMain.h"
#include "ofxWStyleLoader.h"


class ofxWButton: public ofxWidget{
public:

	int *	itargetValue;
	bool *	btargetValue;
	int		value;

	ofxWButton(const string & name);

	virtual ~ofxWButton(){}

	void init(const string & _title, int * _value, string style="default");
	void init(const string & _title, bool * _value, string style="default");
	void init(const string & _title, bool _value, string style="default");
	void init(const string & _title, string style="default");

	void setAutoRepeat(unsigned _ms_first=600, unsigned _ms_autorepeat=30);

	int getValueI();
	float getValueF();
	bool getValueB();

	ofEvent<int>  intEvent;
	ofEvent<bool> boolEvent;

protected:
	void on();
	void off();
	virtual void update();
	virtual void render(ofxWidgetsStyle & style);

	virtual ofRectangle getActiveArea(ofxWidgetsStyle & style);
	virtual ofRectangle getTotalArea(ofxWidgetsStyle & style);


	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);

	void saveTo(ofxXmlSettings & xml, const string & tag);
	void loadFrom(ofxXmlSettings & xml, const string & tag);

	int 	last_repeat;
	int    	ms_first;
	int		ms_autorepeat;
	bool 	prevValue;
};

