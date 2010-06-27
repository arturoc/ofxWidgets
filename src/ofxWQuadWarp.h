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
	ofxWQuadWarp();
	virtual ~ofxWQuadWarp(){}

	void init(ofRectangle origin);
	void render(ofxWidgetsStyle & style);

	ofPoint src[4];
	ofPoint dst[4];

protected:

	ofRectangle getActiveArea(ofxWidgetsStyle & style);
	ofRectangle getTotalArea(ofxWidgetsStyle & style);

	void update();

	void buttonPressed(const void * sender, bool & pressed);

	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState);


private:
	ofxWCircleButton * buttons[4];
	ofxWCircleButton * activeButton;
	ofRectangle origin;
	ofPoint mousePos;
	ofPoint prevMousePos;
};

#endif /* OFXWQUADWARP_H_ */
