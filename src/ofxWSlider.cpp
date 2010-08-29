/*
 * ofxWSlider.cpp
 *
 *  Created on: 27/06/2010
 *      Author: arturo
 */

#include "ofxWSlider.h"

ofxWSlider::ofxWSlider(const string & name):ofxWidget(name){
	value 			= 0;
	ftargetValue 	= 0;
	itargetValue 	= 0;
	title			= "";
	intOnly 		= false;
}

void ofxWSlider::init(const string & _title, float * value, float _min, float _max, const string & style){
	itargetValue	= 0;
	ftargetValue	= value;
	title			= _title;
	min				= _min;
	max				= _max;
	setStyles("slider",style);
	intOnly 		= false;
}

void ofxWSlider::init(const string & _title, int * value, int _min, int _max, const string & style){
	ftargetValue	= 0;
	itargetValue	= value;
	title			= _title;
	min				= _min;
	max				= _max;
	setStyles("slider",style);
	intOnly = true;

}
void ofxWSlider::init(const string & _title, float _value, float _min, float _max, const string & style){
	itargetValue	= 0;
	ftargetValue	= 0;
	title			= _title;
	min				= _min;
	max				= _max;
	value			= ofMap(_value,min,max,0,1);
	setStyles("slider",style);
	intOnly 		= false;
}

void ofxWSlider::init(const string & _title, int _value, int _min, int _max, const string & style){
	ftargetValue	= 0;
	itargetValue	= 0;
	title			= _title;
	min				= _min;
	max				= _max;
	value			= ofMap(_value,min,max,0,1);
	setStyles("slider",style);
	intOnly 		= true;

}
int ofxWSlider::getValueI(){
	return ofMap(value,0,1,min,max);
}

float ofxWSlider::getValueF(){
	return ofMap(value,0,1,min,max);
}

bool ofxWSlider::getValueB(){
	return value;
}

void ofxWSlider::update(){
	if(ftargetValue)
		value = ofClamp(ofMap(*ftargetValue,min,max,0,1),0,1);
	if(itargetValue)
		value = ofClamp(ofMap(*itargetValue,min,max,0,1),0,1);
}

void ofxWSlider::setValue(float _value){
	value = ofClamp(_value,0,1);

	float floatValue = ofMap(value,0,1,min,max);
	int intValue 	 = floatValue;

	if(intOnly) value=ofMap(intValue,min,max,0,1);

	ofNotifyEvent(intEvent,intValue, this);
	ofNotifyEvent(floatEvent,floatValue, this);

	if(ftargetValue)
		*ftargetValue = floatValue;

	if(itargetValue)
		*itargetValue = intValue;
}

void ofxWSlider::render(ofxWidgetsStyle & style){
	ofPushMatrix();
	ofTranslate(style.position.x,style.position.y);

	//background
	ofFill();
	ofSetColor(style.background.color.r,style.background.color.g,style.background.color.b);
	float backgroundX= (style.border.width-style.background.width)/2;
	float backgroundY= (style.border.height-style.background.height)/2;
	ofRect(backgroundX,backgroundY,style.background.width,style.background.height);

	//foreground
	ofSetColor(style.foreground.color.r,style.foreground.color.g,style.foreground.color.b);
	float foregroundX, foregroundY, foregroundW, foregroundH;

	if(max>0 && min<0){
		foregroundX = (style.background.width-style.foreground.width)/2+style.foreground.width*ofMap((max + min),min,max,1,0);
		foregroundY = (style.background.height-style.foreground.height)/2;
		foregroundW = (style.foreground.width*value-style.foreground.width*ofMap((max + min),min,max,1,0));
		foregroundH = style.foreground.height;
		ofRect(foregroundX,foregroundY,foregroundW,foregroundH);
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		ofSetLineWidth(style.border.lineWidth);
		ofLine(foregroundX,0,foregroundX,style.border.height);
	}else if(min>max || max <=0 ){
		foregroundX = (style.background.width-style.foreground.width)/2 + style.foreground.width;
		foregroundY = (style.background.height-style.foreground.height)/2;
		foregroundW = style.foreground.width*value-style.foreground.width;
		foregroundH = style.foreground.height;
		ofRect(foregroundX,foregroundY,foregroundW,foregroundH);
	}else{
		foregroundX = (style.background.width-style.foreground.width)/2;
		foregroundY = (style.background.height-style.foreground.height)/2;
		foregroundW = style.foreground.width*value;
		foregroundH = style.foreground.height;
		ofRect(foregroundX,foregroundY,foregroundW,foregroundH);
	}

	// border
	ofNoFill();
	ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
	ofSetLineWidth(style.border.lineWidth);
	ofRect(0,0,style.border.width,style.border.height);

	char msg[1024];
	if(itargetValue)
		sprintf(msg,"%i",*itargetValue);
	else if(ftargetValue)
		sprintf(msg,"%.2f",*ftargetValue);
	else if(intOnly)
		sprintf(msg,"%i",(int)ofMap(value,0,1,min,max));
	else
		sprintf(msg,"%.2f",ofMap(value,0,1,min,max));
	ofSetColor(style.text.color.r,style.text.color.g,style.text.color.b);
	if(style.text.ttf){
		style.text.font->drawString(title +" "+ msg,style.text.position.x,style.text.position.y);
	}else{
		ofDrawBitmapString(title +" "+ msg,style.text.position.x,style.text.position.y);
	}

	ofPopMatrix();
}

ofRectangle ofxWSlider::getActiveArea(ofxWidgetsStyle & style){
	ofRectangle area;
	area.x=style.position.x;
	area.y=style.position.y;
	area.width=style.foreground.width;
	area.height=style.foreground.height;
	return area;
}

ofRectangle ofxWSlider::getTotalArea(ofxWidgetsStyle & style){
	return getActiveArea(style);
}

// UNFOCUSED -> over -> FOCUSED -> pressed -> PRESSED -> out -> OUT -> released -> UNFOCUSED
// PRESSED -> released -> FOCUSSED
// FOCUSED -> out ->  UNFOCUSED
ofxWidgetsState ofxWSlider::manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
	switch(event){

	case OFX_W_E_POINTER_OVER:
		if(currentState==OFX_WIDGET_PRESSED){
			setValue(args.relative_x);
		}
		if(currentState==OFX_WIDGET_UNFOCUSED){
			return OFX_WIDGET_FOCUSED;
		}
		if(currentState==OFX_WIDGET_OUT){
			return OFX_WIDGET_PRESSED;
		}
		break;

	case OFX_W_E_POINTER_OUT:
		if(currentState==OFX_WIDGET_OUT){
			setValue(args.relative_x);
		}
		if(currentState==OFX_WIDGET_FOCUSED){
			return OFX_WIDGET_UNFOCUSED;
		}
		if(currentState==OFX_WIDGET_PRESSED){
			return OFX_WIDGET_OUT;
		}
		break;

	case OFX_W_E_POINTER_PRESSED:
		if(currentState==OFX_WIDGET_FOCUSED){
			setValue(args.relative_x);
			return OFX_WIDGET_PRESSED;
		}
		break;
	case OFX_W_E_POINTER_RELEASED:
		if(currentState==OFX_WIDGET_PRESSED){
			return OFX_WIDGET_FOCUSED;
		}
		if(currentState==OFX_WIDGET_OUT){
			return OFX_WIDGET_UNFOCUSED;
		}
		break;
	case OFX_W_E_ENABLED:
	case OFX_W_E_DISABLED:
	case OFX_W_E_VALUE_CHANGED:
	case OFX_W_E_POINTER_MOVED:
	case OFX_W_E_POINTER_DRAGGED:
	case OFX_W_E_KEY_PRESSED:
		break;
	case OFX_W_E_KEY_RELEASED:
		if(args.key==OF_KEY_LEFT){
			setValue(value-ofMap(1,min,max,0,1));
		}else if(args.key==OF_KEY_RIGHT){
			setValue(value+ofMap(1,min,max,0,1));
		}
		break;

	default:
	break;
	}

	return currentState;
}

void ofxWSlider::saveTo(ofxXmlSettings & xml, const string & tag){
	if(ftargetValue) xml.addValue(tag,*ftargetValue);
	else if(itargetValue) xml.addValue(tag, *itargetValue);
	else xml.addValue(tag,ofMap(value,0,1,min,max));
}

void ofxWSlider::loadFrom(ofxXmlSettings & xml, const string & tag){
	value = ofMap(xml.getValue(tag,min),min,max,0,1);
	if(ftargetValue) *ftargetValue = xml.getValue(tag,*ftargetValue);
	else if(itargetValue) *itargetValue = xml.getValue(tag,*itargetValue);
}
