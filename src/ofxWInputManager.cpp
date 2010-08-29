/*
 * ofxWInputManager.cpp
 *
 *  Created on: 29/08/2010
 *      Author: arturo
 */

#include "ofxWInputManager.h"

ofxWInputManager::ofxWInputManager() {
	ofRegisterKeyEvents(this);
}

ofxWInputManager::~ofxWInputManager() {
	ofUnregisterKeyEvents(this);
}

void ofxWInputManager::keyPressed(ofKeyEventArgs & key){
	(*selectedWidget)->keyPressed(key);
}

void ofxWInputManager::keyReleased(ofKeyEventArgs & key){
	if(key.key==OF_KEY_DOWN){
		(*selectedWidget)->setFocused(false);
		selectedWidget++;
		if(selectedWidget==widgets.end())
			selectedWidget=widgets.begin();
		(*selectedWidget)->setFocused(true);
	}else if(key.key==OF_KEY_UP){
		(*selectedWidget)->setFocused(false);
		if(selectedWidget!=widgets.begin())
			selectedWidget--;
		else
			selectedWidget = --widgets.end();
		(*selectedWidget)->setFocused(true);
	}

	(*selectedWidget)->keyReleased(key);

}

void ofxWInputManager::registerWidget(ofxWidget * widget, int order){
	if(order==-1 || order>=(int)widgets.size())
		widgets.push_back(widget);
	else{
		list<ofxWidget*>::iterator it = widgets.begin();
		for(int i=0; i<order; i++, it++);
		widgets.insert(it,widget);
	}

	selectedWidget = widgets.begin();
}
