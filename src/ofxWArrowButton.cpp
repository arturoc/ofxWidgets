/*
 * ofxWArrowButton.cpp
 *
 *  Created on: 27/06/2010
 *      Author: arturo
 */

#include "ofxWArrowButton.h"


static struct {
  int 	 width;
  int 	 height;
  unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[10 * 9 * 4 + 1];
} arrow_down = {
  10, 9, 4,
  "\0\0\0\270\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0"
  "\0\0\377\0\0\0\377\0\0\0\270\0\0\0-\0\0\0\375\0\0\0\377\0\0\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\375\0\0\0-\377\377\377\0\0\0\0\234\0"
  "\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\234\377"
  "\377\377\0\377\377\377\0\0\0\0\31\0\0\0\364\0\0\0\377\0\0\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\364\0\0\0\31\377\377\377\0\377\377\377\0\377\377\377\0\0"
  "\0\0\177\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\200\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\13\0\0\0\346\0\0\0\377\0"
  "\0\0\377\0\0\0\346\0\0\0\13\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0c\0\0\0\377\0\0\0\377\0\0\0c\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0"
  "\0\0\2\0\0\0\322\0\0\0\322\0\0\0\2\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0G\0\0\0G"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0",
};

static struct {
  int 	 width;
  int 	 height;
  unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[10 * 9 * 4 + 1];
} arrow_up = {
  10, 9, 4,
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0G\0\0\0G\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\0\0\0\2\0\0\0\322\0\0\0\322\0\0\0\2\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0"
  "c\0\0\0\377\0\0\0\377\0\0\0c\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\13\0\0\0\346\0\0\0\377\0\0\0\377\0\0\0\346"
  "\0\0\0\13\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\177"
  "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\200\377\377\377\0\377\377"
  "\377\0\377\377\377\0\0\0\0\31\0\0\0\364\0\0\0\377\0\0\0\377\0\0\0\377\0\0"
  "\0\377\0\0\0\364\0\0\0\31\377\377\377\0\377\377\377\0\0\0\0\234\0\0\0\377"
  "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\234\377\377\377"
  "\0\0\0\0-\0\0\0\375\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0"
  "\0\377\0\0\0\375\0\0\0-\0\0\0\270\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\270",
};

ofxWArrowButton::ofxWArrowButton(const string & name, bool up):ofxWButton(name)
{
	if(up){
		arrow.allocate(arrow_up.width,arrow_up.height,GL_RGBA);
		arrow.loadData(arrow_up.pixel_data,arrow_up.width,arrow_up.height,GL_RGBA);
	}else{
		arrow.allocate(arrow_down.width,arrow_down.height,GL_RGBA);
		arrow.loadData(arrow_down.pixel_data,arrow_down.width,arrow_down.height,GL_RGBA);
	}
}

void ofxWArrowButton::render(ofxWidgetsStyle & style){
	ofPushStyle();
	ofxWButton::render(style);
	ofPopStyle();
	ofSetColor(255,255,255,255);
	ofEnableAlphaBlending();
	arrow.draw(style.position.x + ( style.background.width - arrow_up.width )/2,style.position.y + ( style.background.height - arrow_up.height )/2);
}
