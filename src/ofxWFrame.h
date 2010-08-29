/*
 * ofxYAFrame.h
 *
 *  Created on: 20-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxWSlider.h"
#include "ofxWButton.h"
#include "ofxWToggle.h"
#include "ofxWFps.h"
#include "ofxWStyleLoader.h"
#include "ofxWSpinSlider.h"
#include "ofxXmlSettings.h"
#include "ofxWTextBox.h"

class ofxWFrame: public ofxWidget{
public:

	ofxWFrame();

	void init(const string & title="", const string & name="", bool growOnHeight = true);
	void init(float x, float y, float width, float height, const string & title="", const string & name="", bool growOnHeight=true);
	void init(const ofRectangle & shape, const string & title="", const string & name="", bool growOnHeight = true);

	void addSaveButton(const string & filename, const string & xml_root);
	void addLoadButton(const string & filename, const string & xml_root);

	void saveTo(const string & filename, const string & xml_root);
	void loadFrom(const string & filename, const string & xml_root);

	int getValueI(string controlName, int defaultValue=0);
	float getValueF(string controlName, float defaultValue=0);
	string getValueS(string controlName);
	bool getValueB(string controlName, bool defaultValue=0);

	void setStyle(const string & _style);

	virtual void enable();
	virtual void disable();


	virtual void setVisible(bool visible);

	void update();
	virtual void render(ofxWidgetsStyle & style);

	ofRectangle getActiveArea(ofxWidgetsStyle & style);
	ofRectangle getTotalArea(ofxWidgetsStyle & style);

	ofxWSlider & addSlider(const string & title, int * value, int min, int max, string controlName="", const string & _style="");
	ofxWSlider & addSlider(const string & title, float * value, float min, float max, string controlName="", const string & _style="");
	ofxWSlider & addSlider(const string & title, int value, int min, int max, string controlName="", const string & _style="");
	ofxWSlider & addSlider(const string & title, float value, float min, float max, string controlName="", const string & _style="");


	ofxWSpinSlider & addSpinSlider(const string & title, int * value, int min, int max, int step=1, string controlName="", const string & _style="");
	ofxWSpinSlider & addSpinSlider(const string & title, float * value, float min, float max, float step=1,  string controlName="", const string & _style="");
	ofxWSpinSlider & addSpinSlider(const string & title, int value, int min, int max, int step=1, string controlName="", const string & _style="");
	ofxWSpinSlider & addSpinSlider(const string & title, float value, float min, float max, float step=1,  string controlName="", const string & _style="");

	ofxWButton & addButton(const string & title, int * value, string controlName="", string _style="");
	ofxWButton & addButton(const string & title, bool * value, string controlName="", string _style="");
	ofxWButton & addButton(const string & title, int value, string controlName="", string _style="");
	ofxWButton & addButton(const string & title, string controlName="", string _style="");


	ofxWToggle & addToggle(const string & title, int * value, string controlName="", string _style="");
	ofxWToggle & addToggle(const string & title, bool * value, string controlName="", string _style="");
	ofxWToggle & addToggle(const string & title, int value, string controlName="", string _style="");
	ofxWToggle & addToggle(const string & title, string controlName="", string _style="");

	ofxWToggle & addGroupedToggle(const string & title, int * value, string group, string controlName="", string _style="");
	ofxWToggle & addGroupedToggle(const string & title, bool * value, string group, string controlName="", string _style="");
	ofxWToggle & addGroupedToggle(const string & title, int value, string group, string controlName="", string _style="");
	ofxWToggle & addGroupedToggle(const string & title, string group, string controlName="", string _style="");

	ofxWTextBox & addTextBox(const string & title, string text="", string controlName="", string _style="");
	ofxWidgetFps & addFps(string controlName="");

	ofPoint getNextPosition();

	int getValueI();
	float getValueF();
	bool getValueB();

	void onSave(int & pressed);
	void onLoad(int & pressed);

	void groupedPressed(const void * sender, bool & pressed);

	ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);

protected:
	void addWidget(ofxWidget * widget, string controlName);

	vector<ofxWidget*> controls;
	map<string, ofxWidget*> controlsIndex;
	map<string, vector<ofxWToggle*> > groups;
	string style;
	ofxWFrameStyle		frameStyle;

	string				filename;
	string				xml_root;
	static	ofPoint		next_pos;
	static float		max_frame_width;
	bool				auto_pos;
};


