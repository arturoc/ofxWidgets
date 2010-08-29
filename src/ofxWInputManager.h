/*
 * ofxWInputManager.h
 *
 *  Created on: 29/08/2010
 *      Author: arturo
 */

#ifndef OFXWINPUTMANAGER_H_
#define OFXWINPUTMANAGER_H_

#include "ofMain.h"
#include "ofxWidget.h"
#include <list>

class ofxWInputManager {
public:
	ofxWInputManager();
	virtual ~ofxWInputManager();


	void keyPressed(ofKeyEventArgs & key);
	void keyReleased(ofKeyEventArgs & key);

	void registerWidget(ofxWidget * widget, int order=-1);

private:
	list<ofxWidget*> widgets;
	list<ofxWidget*>::iterator selectedWidget;
};

#endif /* OFXWINPUTMANAGER_H_ */
