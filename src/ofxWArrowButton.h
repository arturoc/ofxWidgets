/*
 * ofxWArrowButton.h
 *
 *  Created on: 23/12/2009
 *      Author: arturo
 */

#ifndef OFXWARROWBUTTON_H_
#define OFXWARROWBUTTON_H_

#include "ofxWButton.h"
#include "ofMain.h"



class ofxWArrowButton: public ofxWButton {
public:
	ofxWArrowButton(const string & name, bool up);
	virtual ~ofxWArrowButton(){}

	void render(ofxWidgetsStyle & style);

private:
	ofTexture arrow;

};

#endif /* OFXWARROWBUTTON_H_ */
