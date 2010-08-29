/*
 * ofxWButton.cpp
 *
 *  Created on: 27/06/2010
 *      Author: arturo
 */

#include "ofxWButton.h"

ofxWButton::ofxWButton(const string & name):ofxWidget(name){
	itargetValue=NULL;
	btargetValue=NULL;
	value=0;
	prevValue=0;
	ms_autorepeat=false;
}

int ofxWButton::getValueI(){
	return value;
}

float ofxWButton::getValueF(){
	return value;
}

bool ofxWButton::getValueB(){
	return value;
}

void ofxWButton::init(const string & _title, int * _value, string style){
	itargetValue = _value;
	if(_value){
		value	= *_value;
		state	= *_value?OFX_WIDGET_PRESSED:OFX_WIDGET_UNFOCUSED;
	}else{
		value	= 0;
		state	= OFX_WIDGET_UNFOCUSED;
	}
	title 		= _title;

	setStyles("button",style);
}

void ofxWButton::init(const string & _title, bool * _value, string style){
	btargetValue = _value;
	if(_value && *_value){
		value	= 1;
		state	= OFX_WIDGET_PRESSED;
	}else{
		value	= 0;
		state	= OFX_WIDGET_UNFOCUSED;
	}
	title 		= _title;

	setStyles("button",style);

}

void ofxWButton::init(const string & _title, bool _value, string style){
	if(_value){
		on();
		state	= OFX_WIDGET_PRESSED;
	}else{
		off();
		state	= OFX_WIDGET_UNFOCUSED;
	}
	title 		= _title;
	setStyles("button",style);
}

void ofxWButton::init(const string & _title, string style){
	btargetValue = NULL;
	itargetValue = NULL;
	value	= 0;
	state	= OFX_WIDGET_UNFOCUSED;
	title 		= _title;

	setStyles("button",style);

}

void ofxWButton::setAutoRepeat(unsigned _ms_first, unsigned _ms_autorepeat){
	ms_first = _ms_first;
	ms_autorepeat = _ms_autorepeat;
}

void ofxWButton::on(){
	if(itargetValue)
		*itargetValue=1;
	if(btargetValue)
		*btargetValue=true;
	value = 1;
	prevValue = 1;
	bool boolValue = true;
	ofNotifyEvent(intEvent , value, this);
	ofNotifyEvent(boolEvent, boolValue, this);
}

void ofxWButton::off(){
	if(itargetValue)
		*itargetValue=0;
	if(btargetValue)
		*btargetValue=false;
	value = 0;
	prevValue = 0;
	bool boolValue = false;
	ofNotifyEvent(intEvent , value, this);
	ofNotifyEvent(boolEvent, boolValue, this);
}

void ofxWButton::update(){
	if(itargetValue ){
		if(value!=*itargetValue){
			value = *itargetValue;
			ofxWidgetEventArgs args;
			ofxWidget::newEvent(OFX_W_E_VALUE_CHANGED,args);
		}
	}else
	if(btargetValue){
		if(value!=*btargetValue){
			value = *btargetValue;
			ofxWidgetEventArgs args;
			ofxWidget::newEvent(OFX_W_E_VALUE_CHANGED,args);
		}
	}else
	if(prevValue!=value){
		ofxWidgetEventArgs args;
		ofxWidget::newEvent(OFX_W_E_VALUE_CHANGED,args);
	}
	if(state == OFX_WIDGET_PRESSED && ms_autorepeat && ofGetElapsedTimeMillis()-last_repeat>ms_autorepeat){
		on();
		off();
		last_repeat = ofGetElapsedTimeMillis();
	}
	prevValue = value;
}

void ofxWButton::render(ofxWidgetsStyle & style){
	ofPushMatrix();
	ofTranslate(style.position.x,style.position.y);
	ofSetRectMode(OF_RECTMODE_CORNER);

	//background
	ofFill();
	ofSetColor(style.background.color.r,style.background.color.g,style.background.color.b);
	float backgroundX= (style.border.width-style.background.width)/2;
	float backgroundY= (style.border.height-style.background.height)/2;
	ofRect(backgroundX,backgroundY,style.background.width,style.background.height);

	//foreground
	ofSetColor(style.foreground.color.r,style.foreground.color.g,style.foreground.color.b);
	float foregroundX= (style.background.width-style.foreground.width)/2;
	float foregroundY= (style.background.height-style.foreground.height)/2;
	ofRect(foregroundX,foregroundY,style.foreground.width,style.foreground.height);

	// border
	ofNoFill();
	ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
	ofSetLineWidth(style.border.lineWidth);
	ofRect(0,0,style.border.width,style.border.height);

	ofSetColor(style.text.color.r,style.text.color.g,style.text.color.b);
	if(style.text.ttf){
		style.text.font->drawString(title,style.text.position.x,style.text.position.y);
	}else{
		ofDrawBitmapString(title,style.text.position.x,style.text.position.y);
	}

	ofPopMatrix();
}

ofRectangle ofxWButton::getActiveArea(ofxWidgetsStyle & style){
	ofRectangle area;
	area.x=style.position.x;
	area.y=style.position.y;
	area.width=style.border.width;// + OF_BITMAP_CHAR_WIDTH * title.size();
	area.height=MAX(style.border.height, OF_BITMAP_CHAR_HEIGHT);
	return area;
}

ofRectangle ofxWButton::getTotalArea(ofxWidgetsStyle & style){
	ofRectangle area = getActiveArea(style);
	area.width = style.text.position.x;
	area.width += style.text.font->getStringBoundingBox(title,0,0).width;
	return area;
}


ofxWidgetsState ofxWButton::manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){

	switch(event){

	case OFX_W_E_POINTER_OVER:
		if(currentState==OFX_WIDGET_UNFOCUSED){
			return OFX_WIDGET_FOCUSED;
		}
		if(currentState==OFX_WIDGET_OUT){
			return OFX_WIDGET_PRESSED;
		}
		break;

	case OFX_W_E_POINTER_OUT:
		if(currentState==OFX_WIDGET_FOCUSED){
			return OFX_WIDGET_UNFOCUSED;
		}
		if(currentState==OFX_WIDGET_PRESSED){
			return OFX_WIDGET_OUT;
		}
		break;

	case OFX_W_E_POINTER_PRESSED:
		if(currentState==OFX_WIDGET_FOCUSED){
			last_repeat = ofGetElapsedTimeMillis()+ms_first;
			on();
			return OFX_WIDGET_PRESSED;
		}
		break;

	case OFX_W_E_POINTER_RELEASED:
		if(currentState==OFX_WIDGET_PRESSED){
			off();
			return OFX_WIDGET_FOCUSED;
		}
		if(currentState==OFX_WIDGET_OUT){
			off();
			return OFX_WIDGET_UNFOCUSED;
		}
		break;

	case OFX_W_E_VALUE_CHANGED:
		if(currentState==OFX_WIDGET_PRESSED){
			return OFX_WIDGET_FOCUSED;
		}else{
			return OFX_WIDGET_PRESSED;
		}
		break;
	case OFX_W_E_DISABLED:
		if(currentState==OFX_WIDGET_PRESSED){
			return OFX_WIDGET_UNFOCUSED;
		}
		break;
	case OFX_W_E_ENABLED:
		if(currentState==OFX_WIDGET_PRESSED){
			if(mouseIn())
				return OFX_WIDGET_FOCUSED;
			else
				return OFX_WIDGET_UNFOCUSED;
		}
		break;
	case OFX_W_E_POINTER_MOVED:
	case OFX_W_E_POINTER_DRAGGED:
	case OFX_W_E_KEY_PRESSED:
		break;
	case OFX_W_E_KEY_RELEASED:
		if(args.key==' ' || args.key==OF_KEY_RETURN){
			on();
			off();
		}
		break;
	default:
	break;
	}

	return currentState;
}

void ofxWButton::saveTo(ofxXmlSettings & xml, const string & tag){
	xml.addValue(tag,value);
}

void ofxWButton::loadFrom(ofxXmlSettings & xml, const string & tag){
	value = xml.getValue(tag,0);
}

