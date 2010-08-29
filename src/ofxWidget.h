/*
 * ofxYAGuiControl.h
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */

#ifndef OFXYAGUICONTROL_H_
#define OFXYAGUICONTROL_H_

#include "ofxWTypes.h"
#include "ofxWStyleLoader.h"
#include "ofEvents.h"
#include "ofMain.h"
#include <set>
#include "ofxXmlSettings.h"

//#define OFXWIDGETS_USING_TUIO
#define OFXWIDGETS_SEND_OSC

#ifdef OFXWIDGETS_USING_TUIO
#include "ofxTuioClient.h"

#endif

#ifdef OFXWIDGETS_SEND_OSC
#include "ofxOsc.h"
#endif

class ofxWidget {

protected:
	virtual void render(ofxWidgetsStyle & style)=0;
	virtual ofRectangle getActiveArea(ofxWidgetsStyle & style)=0;
	virtual ofRectangle getTotalArea(ofxWidgetsStyle & style)=0;
	//virtual ofRectangle getActiveArea(ofxYAGControlStyle & style)=0;
	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);
	virtual void update(){};


	virtual void onEnable(){}
	virtual void onDisable(){}
	virtual void onVisible(ofPoint point){}
	virtual void onInvisible(ofPoint point){}


	ofxWidgetsStyle getStyle(string controlType, string state, string style="default");

	void setStyles(string controlType, const string & style);

	void newEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args);

	bool mouseIn();


	ofMouseEventArgs mouse;
	ofxWidgetsState 	state;

	string name;

#ifdef OFXWIDGETS_USING_TUIO
	static ofxTuioClient * tuioClient;
#endif


public:

	string title;

	ofEvent<bool> focusedEvent;

#ifdef OFXWIDGETS_USING_TUIO
	static void setTuio(ofxTuioClient & _tuioClient);
#endif

	virtual int getValueI()=0;
	virtual float getValueF()=0;
	virtual bool getValueB()=0;
	virtual string getValueS(){ return ""; };

	string getName() const{
		return name;
	}

	virtual void enable();
	virtual void disable();
	virtual void setVisible(bool _visible);


	ofRectangle getControlActiveArea();

	ofPoint getControlActiveSize();

	ofRectangle getControlTotalArea();

	ofPoint getControlPosition();

	void update(ofEventArgs & args);

	ofxWidget(const string & name);

	virtual ~ofxWidget();


	void setPosition(ofPoint _position);

	virtual void setSize(ofPoint _size);

	void setEnabledStyle(ofxWidgetsStyle style);
	void setDisabledStyle(ofxWidgetsStyle style);
	void setPressedStyle(ofxWidgetsStyle style);
	void setFocusedStyle(ofxWidgetsStyle style);
	void setOutStyle(ofxWidgetsStyle style);


	void draw(ofEventArgs & args);


	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseMoved(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);

	void keyPressed(ofKeyEventArgs & key);

	void keyReleased(ofKeyEventArgs & key);

	void setFocused(bool isFocused);


#ifdef OFXWIDGETS_USING_TUIO
	void addTuioCursor(TUIO::TuioCursor &tuioCursor);

	void updateTuioCursor(TUIO::TuioCursor &tuioCursor);

	void removeTuioCursor(TUIO::TuioCursor &tuioCursor);


	/*void tuioAdded(ofxTuioCursor & tuioCursor){
		if(cursorIn(tuioCursor)){
			yargs.x=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).x;
			yargs.y=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).y;
			newEvent(OFX_W_E_POINTER_PRESSED, yargs);
		}
	}
	void tuioRemoved(ofxTuioCursor & tuioCursor){
		yargs.x=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).x;
		yargs.y=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).y;
		newEvent(OFX_W_E_POINTER_RELEASED, yargs);
	}
	void tuioUpdated(ofxTuioCursor & tuioCursor){
		if(cursorIn(tuioCursor)){
			yargs.x=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).x;
			yargs.y=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).y;
			newEvent(OFX_W_E_POINTER_OVER, yargs);
		}else{
			yargs.x=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).x;
			yargs.y=getRelativePosition(tuioCursor.getX() * ofGetWidth(),tuioCursor.getY() * ofGetHeight()).y;
			newEvent(OFX_W_E_POINTER_OUT, yargs);
		}
	}*/

	bool cursorIn(TUIO::TuioCursor & tuioCursor);
#endif


	virtual void saveTo(ofxXmlSettings & xml, const string & tag){};
	virtual void loadFrom(ofxXmlSettings & xml, const string & tag){};


	ofxWidgetsStyle & getCurrentStyle();

private:

	ofPoint getRelativePosition(float x, float y);

	const ofxWStyleLoader * styleLoader;

	ofxWidgetsStyle 	styleEnabled;
	ofxWidgetsStyle 	styleFocused;
	ofxWidgetsStyle 	styleDisabled;
	ofxWidgetsStyle 	stylePressed;
	ofxWidgetsStyle 	styleOut;

	ofxWidgetsStyle  	currentStyle;

	bool				enabled;
	bool				visible;

	ofPoint				position;


	ofxWidgetEventArgs 		yargs;
	set<int> cursorsIn;

};

#endif /* OFXYAGUICONTROL_H_ */
