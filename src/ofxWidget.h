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
	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		return currentState;
	}

	virtual void update(){};


	virtual void onEnable(){}
	virtual void onDisable(){}
	virtual void onVisible(ofPoint point){}
	virtual void onInvisible(ofPoint point){}


	ofxWidgetsStyle getStyle(string controlType, string state, string style="default"){
		return ofxWStyleLoader::getLoader().getStyle(controlType,state,style);
	}

	void setStyles(string controlType, const string & style){
		setDisabledStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"disabled",style));
		setEnabledStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"enabled",style));
		setFocusedStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"focused",style));
		setPressedStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"pressed",style));
		setOutStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"pressed",style));
	}

	void newEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args){
		//ofLog(OF_LOG_VERBOSE,name  + " newEvent: " + toString(event));
		//ofLog(OF_LOG_VERBOSE,name  + " newEvent: from " + toString(state));
		ofxWidgetsState prevState = state;
		state = manageEvent(event,args,state);
		if(state!=prevState){
			if(state==OFX_WIDGET_FOCUSED){
				bool focused = true;
				ofNotifyEvent(focusedEvent,focused, this);
			}
			if(state==OFX_WIDGET_UNFOCUSED){
				bool focused = false;
				ofNotifyEvent(focusedEvent,focused, this);
			}
		}
		//ofLog(OF_LOG_VERBOSE,name  + " newEvent: to " + toString(state));
	}

	bool mouseIn(){
		ofRectangle area = getActiveArea(getCurrentStyle());
		bool mouseIn= mouse.x >= area.x &&
		mouse.x <= area.x + area.width &&
		mouse.y >= area.y &&
		mouse.y <= area.y + area.height ;

		//ofLog(OF_LOG_VERBOSE,(title + " mousein: %i").c_str(),mouseIn?1:0);
		return (mouseIn);
	}


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
	static void setTuio(ofxTuioClient & _tuioClient){
		tuioClient = &_tuioClient;
	}
#endif

	virtual int getValueI()=0;
	virtual float getValueF()=0;
	virtual bool getValueB()=0;
	virtual string getValueS(){ return ""; };

	string getName() const{
		return name;
	}

	virtual void enable(){

		ofAddListener(ofEvents.update,this,&ofxWidget::update);
		ofRegisterMouseEvents(this);
		ofRegisterKeyEvents(this);
		//ofAddListener(ofEvents.mousePressed,this,&ofxWidget::mousePressed);
		//ofAddListener(ofEvents.mouseReleased,this,&ofxWidget::mouseReleased);
		//ofAddListener(ofEvents.mouseMoved,this,&ofxWidget::mouseMoved);
		//ofAddListener(ofEvents.mouseDragged,this,&ofxWidget::mouseDragged);
		#ifdef OFXWIDGETS_USING_TUIO
			if(tuioClient){
				ofAddListener(tuioClient->cursorAdded, this, &ofxWidget::addTuioCursor);
				ofAddListener(tuioClient->cursorUpdated, this, &ofxWidget::updateTuioCursor);
				ofAddListener(tuioClient->cursorRemoved, this, &ofxWidget::removeTuioCursor);
			}
		#endif
		enabled=true;
		newEvent(OFX_W_E_ENABLED, yargs);
	}

	virtual void disable(){
		ofRemoveListener(ofEvents.update,this,&ofxWidget::update);

		//ofRemoveListener(ofEvents.mousePressed,this,&ofxWidget::mousePressed);
		//ofRemoveListener(ofEvents.mouseReleased,this,&ofxWidget::mouseReleased);
		//ofRemoveListener(ofEvents.mouseMoved,this,&ofxWidget::mouseMoved);
		//ofRemoveListener(ofEvents.mouseDragged,this,&ofxWidget::mouseDragged);
		ofUnregisterMouseEvents(this);
		ofUnregisterKeyEvents(this);
		#ifdef OFXWIDGETS_USING_TUIO
			if(tuioClient){
				ofRemoveListener(tuioClient->cursorAdded, this, &ofxWidget::addTuioCursor);
				ofRemoveListener(tuioClient->cursorUpdated, this, &ofxWidget::updateTuioCursor);
				ofRemoveListener(tuioClient->cursorRemoved, this, &ofxWidget::removeTuioCursor);
			}
		#endif
		enabled=false;
		newEvent(OFX_W_E_DISABLED, yargs);
	}


	virtual void setVisible(bool _visible){
		visible=_visible;
		if(_visible)
			ofAddListener(ofEvents.draw,this,&ofxWidget::draw);
		else
			ofRemoveListener(ofEvents.draw,this,&ofxWidget::draw);
	}


	ofRectangle getControlActiveArea(){
		return getActiveArea(styleEnabled);
	}

	ofPoint getControlActiveSize(){
		ofRectangle area = getActiveArea(styleEnabled);
		return ofPoint(area.width,area.height);
	}

	ofRectangle getControlTotalArea(){
		return getTotalArea(styleEnabled);
	}

	ofPoint getControlPosition(){
		return position;
	}

	void update(ofEventArgs & args){
		update();
	}

	ofxWidget(const string & name);

	virtual ~ofxWidget();


	void setPosition(ofPoint _position){
		position=_position;
		newEvent(OFX_W_E_POS_CHANGED,yargs);
	}

	virtual void setSize(ofPoint _size){
		/*styleEnabled.;
		styleFocused;
		styleDisabled;
		stylePressed;
		styleOut;*/
	}

	void setEnabledStyle(ofxWidgetsStyle style){
		styleEnabled=style;
		styleEnabled.styleState="enabled";
	}

	void setDisabledStyle(ofxWidgetsStyle style){
		styleDisabled=style;
		styleDisabled.styleState="disable";
	}

	void setPressedStyle(ofxWidgetsStyle style){
		stylePressed=style;
		stylePressed.styleState="pressed";
	}

	void setFocusedStyle(ofxWidgetsStyle style){
		styleFocused=style;
		styleFocused.styleState="focused";
	}

	void setOutStyle(ofxWidgetsStyle style){
		styleOut=style;
		styleOut.styleState="out";
	}


	void draw(ofEventArgs & args){
		//ofLog(OF_VERBOSE, "style for state: " + getCurrentStyle().styleState);
		ofPushStyle();
			render(getCurrentStyle());
		ofPopStyle();
	}


	void mousePressed(ofMouseEventArgs & mouse){
		this->mouse=mouse;
		if(mouseIn()){
			yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
			yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
			yargs.abs_x = mouse.x;
			yargs.abs_y = mouse.y;
			newEvent(OFX_W_E_POINTER_PRESSED, yargs);
		}
	}
	void mouseReleased(ofMouseEventArgs & mouse){
		this->mouse=mouse;
		yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
		yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
		yargs.abs_x = mouse.x;
		yargs.abs_y = mouse.y;
		newEvent(OFX_W_E_POINTER_RELEASED, yargs);
	}
	void mouseMoved(ofMouseEventArgs & mouse){
		this->mouse=mouse;
		if(mouseIn()){
			this->mouse=mouse;
			yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
			yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
			yargs.abs_x = mouse.x;
			yargs.abs_y = mouse.y;
			newEvent(OFX_W_E_POINTER_OVER, yargs);
		}else{
			yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
			yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
			yargs.abs_x = mouse.x;
			yargs.abs_y = mouse.y;
			newEvent(OFX_W_E_POINTER_OUT, yargs);
		}
	}
	void mouseDragged(ofMouseEventArgs & mouse){
		this->mouse=mouse;
		if(mouseIn()){
			yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
			yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
			yargs.abs_x = mouse.x;
			yargs.abs_y = mouse.y;
			newEvent(OFX_W_E_POINTER_OVER, yargs);
		}else{
			yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
			yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
			yargs.abs_x = mouse.x;
			yargs.abs_y = mouse.y;
			newEvent(OFX_W_E_POINTER_OUT, yargs);
		}

	}

	void keyPressed(ofKeyEventArgs & key){
		yargs.key = key.key;
		newEvent(OFX_W_E_KEY_PRESSED,yargs);
	}

	void keyReleased(ofKeyEventArgs & key){
		yargs.key = key.key;
		newEvent(OFX_W_E_KEY_RELEASED,yargs);
	}


#ifdef OFXWIDGETS_USING_TUIO
	void addTuioCursor(TUIO::TuioCursor &tuioCursor){
		if(cursorIn(tuioCursor)){
			yargs.relative_x=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).x;
			yargs.relative_y=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).y;
			yargs.abs_x=tuioCursor.getX();
			yargs.abs_y=tuioCursor.getY();
			newEvent(OFX_W_E_POINTER_OVER, yargs);
			newEvent(OFX_W_E_POINTER_PRESSED, yargs);
			cursorsIn.insert(tuioCursor.getSessionID());
		}
	}

	void updateTuioCursor(TUIO::TuioCursor &tuioCursor){
		if(cursorsIn.find(tuioCursor.getSessionID())==cursorsIn.end()) return;
		if(cursorIn(tuioCursor)){
			yargs.relative_x=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).x;
			yargs.relative_y=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).y;
			yargs.abs_x=tuioCursor.getX();
			yargs.abs_y=tuioCursor.getY();
			newEvent(OFX_W_E_POINTER_OVER, yargs);
		}else{
			yargs.relative_x=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).x;
			yargs.relative_y=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).y;
			yargs.abs_x=tuioCursor.getX();
			yargs.abs_y=tuioCursor.getY();
			newEvent(OFX_W_E_POINTER_OUT, yargs);
		};
	}

	void removeTuioCursor(TUIO::TuioCursor &tuioCursor){
		if(cursorsIn.find(tuioCursor.getSessionID())!=cursorsIn.end()){
			cursorsIn.erase(tuioCursor.getSessionID());
			yargs.relative_x=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).x;
			yargs.relative_y=getRelativePosition(tuioCursor.getX(),tuioCursor.getY()).y;
			yargs.abs_x=tuioCursor.getX();
			yargs.abs_y=tuioCursor.getY();
			newEvent(OFX_W_E_POINTER_RELEASED, yargs);
		}
	}


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

	bool cursorIn(TUIO::TuioCursor & tuioCursor){
		ofRectangle area = getActiveArea(getCurrentStyle());
		bool cursorIn= tuioCursor.getX()>= area.x &&
		tuioCursor.getX() <= area.x + area.width &&
		tuioCursor.getY() >= area.y &&
		tuioCursor.getY() <= area.y + area.height ;

		ofLog(OF_LOG_VERBOSE,"cursorIn: %i",cursorIn?1:0);
		return (cursorIn);
	}
#endif


	virtual void saveTo(ofxXmlSettings & xml, const string & tag){};
	virtual void loadFrom(ofxXmlSettings & xml, const string & tag){};




	ofxWidgetsStyle & getCurrentStyle(){
		if(!enabled)
			currentStyle = styleDisabled;

		switch(state){
			case OFX_WIDGET_FOCUSED:
				currentStyle = styleFocused;
				break;
			case OFX_WIDGET_PRESSED:
				currentStyle = stylePressed;
				break;
			case OFX_WIDGET_OUT:
				currentStyle = styleOut;
				break;
			case OFX_WIDGET_UNFOCUSED:
			default:
				currentStyle = styleEnabled;
				break;
		}
		currentStyle.position+=position;
		return currentStyle;
	}
private:

	const ofxWStyleLoader * styleLoader;

	ofPoint getRelativePosition(float x, float y){
		return ofPoint((x-getActiveArea(getCurrentStyle()).x)/getActiveArea(getCurrentStyle()).width
							,(y-getActiveArea(getCurrentStyle()).y)/getActiveArea(getCurrentStyle()).height);
	}

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
