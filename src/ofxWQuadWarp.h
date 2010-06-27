/*
 * ofxWQuadWarp.h
 *
 *  Created on: 08/01/2010
 *      Author: arturo
 */

#ifndef OFXWQUADWARP_H_
#define OFXWQUADWARP_H_

#include "ofxWidget.h"
#include "ofxWCircleButton.h"

class ofxWQuadWarp: public ofxWButton {
public:
	ofxWQuadWarp()
		:ofxWButton("quad_warp")
	{

	}
	virtual ~ofxWQuadWarp(){

	}

	void init(ofRectangle origin){
		ofxWButton::init("");
		this->origin = origin;

		src[0].x = origin.x;
		src[0].y = origin.y;

		src[1].x = origin.x + origin.width;
		src[1].y = origin.y;

		src[2].x = origin.x + origin.width;
		src[2].y = origin.y + origin.height;

		src[3].x = origin.x;
		src[3].y = origin.y + origin.height;

		for(int i=0; i<4; i++){
			dst[i] = src[i];
			buttons[i] = new ofxWCircleButton("button" + ofToString(i));
			buttons[i]->init("");
			ofAddListener(buttons[i]->boolEvent,this,&ofxWQuadWarp::buttonPressed);
		}


	}

	virtual void render(ofxWidgetsStyle & style){
		ofPushStyle();
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);

		ofBeginShape();
		for(int i=0;i<4; i++){
			ofVertex(dst[i].x,dst[i].y);
		}
		ofEndShape(true);

		ofRect(origin.x,origin.y,origin.width,origin.height);


		ofPopStyle();
	}
	ofPoint src[4];
	ofPoint dst[4];

protected:

	virtual ofRectangle getActiveArea(ofxWidgetsStyle & style){
		return ofRectangle(dst[0].x,dst[0].y,dst[1].x - dst[0].x,dst[2].y - dst[1].y);
	}

	virtual ofRectangle getTotalArea(ofxWidgetsStyle & style){
		return getActiveArea(style);
	}

	void update(){
		ofxWButton::update();
		for(int i=0; i<4; i++){
			if(buttons[i]==activeButton){
				dst[i] = mousePos;
			}
			buttons[i]->setPosition(dst[i]);
		}
		if(getValueB() && !activeButton){
			//cout << "changing pos" << endl;
			ofPoint mouseDiff = mousePos - prevMousePos;
			for(int i=0; i<4; i++){
				//src[i]+=mouseDiff;
				dst[i]+=mouseDiff;
			}
			prevMousePos = mousePos;
		}

	}

	void buttonPressed(const void * sender, bool & pressed){
		if(pressed){
			activeButton = (ofxWCircleButton*)sender;
		}else{
			activeButton = 0;
		}
	}


	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){

		prevMousePos = mousePos;
		mousePos.x = mouse.x;
		mousePos.y = mouse.y;


		return ofxWButton::manageEvent(event,args,currentState);
	}


private:
	ofxWCircleButton * buttons[4];
	ofxWCircleButton * activeButton;
	ofRectangle origin;
	ofPoint mousePos;
	ofPoint prevMousePos;
};

#endif /* OFXWQUADWARP_H_ */
