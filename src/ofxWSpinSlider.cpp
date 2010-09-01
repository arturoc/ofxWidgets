/*
 * ofxWSpiSlider.cpp
 *
 *  Created on: 27/06/2010
 *      Author: arturo
 */
#include "ofxWSpinSlider.h"

ofxWSpinSlider::ofxWSpinSlider(const string & name)
:ofxWSlider(name)
,down(name+"_down",false)
,up(name+"_up",true){
	up.init("");
	down.init("");
	up.setAutoRepeat();
	down.setAutoRepeat();
	ofAddListener(up.boolEvent,this,&ofxWSpinSlider::upPressed);
	ofAddListener(down.boolEvent,this,&ofxWSpinSlider::downPressed);
}

ofxWSpinSlider::~ofxWSpinSlider(){

}
void ofxWSpinSlider::init(const string & _title, float * value, float _min, float _max, float _step, const string & style){
	ofxWSlider::init(_title,value,_min,_max,style);
	step = _step;
}

void ofxWSpinSlider::init(const string & _title, int * value, int _min, int _max, int _step, const string & style){
	ofxWSlider::init(_title,value,_min,_max,style);
	step = _step;
}
void ofxWSpinSlider::init(const string & _title, float _value, float _min, float _max, float _step, const string & style){
	ofxWSlider::init(_title,_value,_min,_max,style);
	step = _step;
}

void ofxWSpinSlider::init(const string & _title, int _value, int _min, int _max, int _step, const string & style){
	ofxWSlider::init(_title,_value,_min,_max,style);
	step = _step;
}

void ofxWSpinSlider::render(ofxWidgetsStyle & style){
	ofxWidgetsStyle newStyle = style;
	newStyle.background.width -= up.getControlActiveSize().x + down.getControlActiveSize().x;
	newStyle.foreground.width -= up.getControlActiveSize().x + down.getControlActiveSize().x;
	newStyle.border.width -= up.getControlActiveSize().x + down.getControlActiveSize().x;
	ofxWSlider::render(newStyle);
}

void ofxWSpinSlider::upPressed(bool & value){
	if(value){
		if(intOnly)
			setValue(this->value+ofMap(step,0,max-min,0,1)+ofMap(0.25,0,max-min,0,1));
		else
			setValue(this->value+ofMap(step,0,max-min,0,1));
	}
}

void ofxWSpinSlider::downPressed(bool & value){
	if(value)setValue(this->value-ofMap(step,0,max-min,0,1));
}

ofxWidgetsState ofxWSpinSlider::manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
	if(event==OFX_W_E_POS_CHANGED){
		ofPoint pos = getControlPosition();
		pos.x += getControlActiveSize().x;
		down.setPosition(pos);
		pos.x += down.getControlActiveSize().x;
		up.setPosition(pos);
		return currentState;
	}else{
		return ofxWSlider::manageEvent(event,args,currentState);
	}
}

ofRectangle ofxWSpinSlider::getActiveArea(ofxWidgetsStyle & style){
	ofRectangle area = ofxWSlider::getActiveArea(style);
	area.width -= down.getControlActiveSize().x + up.getControlActiveSize().x;
	return area;
}

void ofxWSpinSlider::enable(){
	ofxWidget::enable();
	up.enable();
	down.enable();
}

void ofxWSpinSlider::disable(){
	ofxWidget::disable();
	up.disable();
	down.disable();
}

void ofxWSpinSlider::setVisible(bool visible){
	ofxWidget::setVisible(visible);
	up.setVisible(visible);
	down.setVisible(visible);

}
