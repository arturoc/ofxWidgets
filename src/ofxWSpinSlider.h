/*
 * ofxWFineSlider.h
 *
 *  Created on: 23/12/2009
 *      Author: arturo
 */

#ifndef OFXWSPINSLIDER_H_
#define OFXWSPINSLIDER_H_

#include "ofxWSlider.h"
#include "ofxWArrowButton.h"

class ofxWSpinSlider: public ofxWSlider {
public:
	ofxWSpinSlider(const string & name)
	:ofxWSlider(name)
	,up(name+"_up",true)
	,down(name+"_down",false){
		up.init("");
		down.init("");
		up.setAutoRepeat();
		down.setAutoRepeat();
		ofAddListener(up.boolEvent,this,&ofxWSpinSlider::upPressed);
		ofAddListener(down.boolEvent,this,&ofxWSpinSlider::downPressed);
	}

	virtual ~ofxWSpinSlider(){

	}
	void init(const string & _title, float * value, float _min, float _max, float _step, const string & style){
		ofxWSlider::init(_title,value,_min,_max,style);
		step = _step;
	}

	void init(const string & _title, int * value, int _min, int _max, int _step, const string & style){
		ofxWSlider::init(_title,value,_min,_max,style);
		step = _step;
	}
	void init(const string & _title, float _value, float _min, float _max, float _step, const string & style){
		ofxWSlider::init(_title,_value,_min,_max,style);
		step = _step;
	}

	void init(const string & _title, int _value, int _min, int _max, int _step, const string & style){
		ofxWSlider::init(_title,_value,_min,_max,style);
		step = _step;
	}


protected:
	void render(ofxWidgetsStyle & style){
		ofxWidgetsStyle newStyle = style;
		newStyle.background.width -= up.getControlSize().x + down.getControlSize().x;
		newStyle.foreground.width -= up.getControlSize().x + down.getControlSize().x;
		newStyle.border.width -= up.getControlSize().x + down.getControlSize().x;
		ofxWSlider::render(newStyle);
	}

	void upPressed(bool & value){
		if(value){
			if(intOnly)
				setValue(this->value+ofMap(step,0,max-min,0,1)+ofMap(0.25,0,max-min,0,1));
			else
				setValue(this->value+ofMap(step,0,max-min,0,1));
		}
	}

	void downPressed(bool & value){
		if(value)setValue(this->value-ofMap(step,0,max-min,0,1));
	}

	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		if(event==OFX_W_E_POS_CHANGED){
			ofPoint pos = getControlPosition();
			pos.x += getControlSize().x;
			down.setPosition(pos);
			pos.x += down.getControlSize().x;
			up.setPosition(pos);
			return currentState;
		}else{
			return ofxWSlider::manageEvent(event,args,currentState);
		}
	}

	ofRectangle getActiveArea(ofxWidgetsStyle & style){
		ofRectangle area = ofxWSlider::getActiveArea(style);
		area.width -= down.getControlSize().x + up.getControlSize().x;
		return area;
	}

private:
	ofxWArrowButton up, down;
	float step;
};

#endif /* OFXWSPINSLIDER_H_ */
