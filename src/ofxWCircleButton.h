/*
 * ofxWCircleButton.h
 *
 *  Created on: 08/01/2010
 *      Author: arturo
 */

#ifndef OFXWCIRCLEBUTTON_H_
#define OFXWCIRCLEBUTTON_H_

#include "ofxWButton.h"

class ofxWCircleButton: public ofxWButton {
public:
	ofxWCircleButton(const string & name):ofxWButton(name){}
	virtual ~ofxWCircleButton(){}

	void render(ofxWidgetsStyle & style){
		ofPushMatrix();
		ofTranslate(style.position.x,style.position.y);

		//background
		ofFill();
		ofSetColor(style.background.color.r,style.background.color.g,style.background.color.b);
		ofCircle(0,0,style.background.width/4);

		//foreground
		ofSetColor(style.foreground.color.r,style.foreground.color.g,style.foreground.color.b);
		ofCircle(0,0,style.foreground.width/4);

		// border
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		ofSetLineWidth(style.border.lineWidth);
		ofCircle(0,0,style.border.width/4);

		ofSetColor(style.text.color.r,style.text.color.g,style.text.color.b);
		if(style.text.ttf){
			style.text.font->drawString(title,style.text.position.x,style.text.position.y);
		}else{
			ofDrawBitmapString(title,style.text.position.x,style.text.position.y);
		}

		ofPopMatrix();
	}

	ofRectangle getActiveArea(ofxWidgetsStyle & style){
		ofRectangle area;
		area.x=style.position.x - style.border.width/4;
		area.y=style.position.y - MAX(style.border.height, OF_BITMAP_CHAR_HEIGHT)/4;
		area.width=style.border.width/2;// + OF_BITMAP_CHAR_WIDTH * title.size();
		area.height=MAX(style.border.height/2, OF_BITMAP_CHAR_HEIGHT);
		return area;
	}

	ofRectangle getTotalArea(ofxWidgetsStyle & style){
		ofRectangle area = getActiveArea(style);
		area.width = style.text.position.x;
		area.width += style.text.font->getStringBoundingBox(title,0,0).width;
		return area;
	}
};

#endif /* OFXWCIRCLEBUTTON_H_ */
