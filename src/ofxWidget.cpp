/*
 * ofxYAGuiControl.cpp
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */

#include "ofxWidget.h"
#include "ofxWControlPanel.h"


#ifdef OFXWIDGETS_USING_TUIO
ofxTuioClient * ofxWidget::tuioClient = NULL;
#endif

ofxWidget::ofxWidget(const string & name) {
	//
	this->name = name;
	enabled = false;
	visible = false;
	state = OFX_WIDGET_UNFOCUSED;

	if(dynamic_cast<ofxWControlPanel*>(this)==NULL){
		//ofxWControlPanel::getControlPanel()->registerControl(this);
		enable();
	}
	setVisible(true);
	styleLoader = &(ofxWStyleLoader::getLoader());
	state		= OFX_WIDGET_UNFOCUSED;
}

ofxWidget::~ofxWidget() {

}

ofxWidgetsState ofxWidget::manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
	return currentState;
}

ofxWidgetsStyle ofxWidget::getStyle(string controlType, string state, string style){
	return ofxWStyleLoader::getLoader().getStyle(controlType,state,style);
}

void ofxWidget::setStyles(string controlType, const string & style){
	setDisabledStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"disabled",style));
	setEnabledStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"enabled",style));
	setFocusedStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"focused",style));
	setPressedStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"pressed",style));
	setOutStyle(ofxWStyleLoader::getLoader().getStyle(controlType,"pressed",style));
}

void ofxWidget::newEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args){
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

bool ofxWidget::mouseIn(){
	ofRectangle area = getActiveArea(getCurrentStyle());
	bool mouseIn= mouse.x >= area.x &&
	mouse.x <= area.x + area.width &&
	mouse.y >= area.y &&
	mouse.y <= area.y + area.height ;

	//ofLog(OF_LOG_VERBOSE,(title + " mousein: %i").c_str(),mouseIn?1:0);
	return (mouseIn);
}

#ifdef OFXWIDGETS_USING_TUIO
void ofxWidget::setTuio(ofxTuioClient & _tuioClient){
	tuioClient = &_tuioClient;
}
#endif


void ofxWidget::enable(){

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

void ofxWidget::disable(){
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

void ofxWidget::setVisible(bool _visible){
	visible=_visible;
	if(_visible)
		ofAddListener(ofEvents.draw,this,&ofxWidget::draw);
	else
		ofRemoveListener(ofEvents.draw,this,&ofxWidget::draw);
}


ofRectangle ofxWidget::getControlActiveArea(){
	return getActiveArea(styleEnabled);
}

ofPoint ofxWidget::getControlActiveSize(){
	ofRectangle area = getActiveArea(styleEnabled);
	return ofPoint(area.width,area.height);
}

ofRectangle ofxWidget::getControlTotalArea(){
	return getTotalArea(styleEnabled);
}

ofPoint ofxWidget::getControlPosition(){
	return position;
}

void ofxWidget::update(ofEventArgs & args){
	update();
}


void ofxWidget::setPosition(ofPoint _position){
	position=_position;
	newEvent(OFX_W_E_POS_CHANGED,yargs);
}

void ofxWidget::setSize(ofPoint _size){
	/*styleEnabled.;
	styleFocused;
	styleDisabled;
	stylePressed;
	styleOut;*/
}

void ofxWidget::setEnabledStyle(ofxWidgetsStyle style){
	styleEnabled=style;
	styleEnabled.styleState="enabled";
}

void ofxWidget::setDisabledStyle(ofxWidgetsStyle style){
	styleDisabled=style;
	styleDisabled.styleState="disable";
}

void ofxWidget::setPressedStyle(ofxWidgetsStyle style){
	stylePressed=style;
	stylePressed.styleState="pressed";
}

void ofxWidget::setFocusedStyle(ofxWidgetsStyle style){
	styleFocused=style;
	styleFocused.styleState="focused";
}

void ofxWidget::setOutStyle(ofxWidgetsStyle style){
	styleOut=style;
	styleOut.styleState="out";
}


void ofxWidget::draw(ofEventArgs & args){
	//ofLog(OF_VERBOSE, "style for state: " + getCurrentStyle().styleState);
	ofPushStyle();
		render(getCurrentStyle());
	ofPopStyle();
}


void ofxWidget::mousePressed(ofMouseEventArgs & mouse){
	this->mouse=mouse;
	if(mouseIn()){
		yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
		yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
		yargs.abs_x = mouse.x;
		yargs.abs_y = mouse.y;
		newEvent(OFX_W_E_POINTER_PRESSED, yargs);
	}
}
void ofxWidget::mouseReleased(ofMouseEventArgs & mouse){
	this->mouse=mouse;
	yargs.relative_x=getRelativePosition(mouse.x,mouse.y).x;
	yargs.relative_y=getRelativePosition(mouse.x,mouse.y).y;
	yargs.abs_x = mouse.x;
	yargs.abs_y = mouse.y;
	newEvent(OFX_W_E_POINTER_RELEASED, yargs);
}
void ofxWidget::mouseMoved(ofMouseEventArgs & mouse){
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
void ofxWidget::mouseDragged(ofMouseEventArgs & mouse){
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

void ofxWidget::keyPressed(ofKeyEventArgs & key){
	yargs.key = key.key;
	newEvent(OFX_W_E_KEY_PRESSED,yargs);
}

void ofxWidget::keyReleased(ofKeyEventArgs & key){
	yargs.key = key.key;
	newEvent(OFX_W_E_KEY_RELEASED,yargs);
}


#ifdef OFXWIDGETS_USING_TUIO
void ofxWidget::addTuioCursor(TUIO::TuioCursor &tuioCursor){
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

void ofxWidget::updateTuioCursor(TUIO::TuioCursor &tuioCursor){
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

void ofxWidget::removeTuioCursor(TUIO::TuioCursor &tuioCursor){
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

bool ofxWidget::cursorIn(TUIO::TuioCursor & tuioCursor){
	ofRectangle area = getActiveArea(getCurrentStyle());
	bool cursorIn= tuioCursor.getX()>= area.x &&
	tuioCursor.getX() <= area.x + area.width &&
	tuioCursor.getY() >= area.y &&
	tuioCursor.getY() <= area.y + area.height ;

	ofLog(OF_LOG_VERBOSE,"cursorIn: %i",cursorIn?1:0);
	return (cursorIn);
}
#endif




ofxWidgetsStyle & ofxWidget::getCurrentStyle(){
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

ofPoint ofxWidget::getRelativePosition(float x, float y){
	return ofPoint((x-getActiveArea(getCurrentStyle()).x)/getActiveArea(getCurrentStyle()).width
						,(y-getActiveArea(getCurrentStyle()).y)/getActiveArea(getCurrentStyle()).height);
}
