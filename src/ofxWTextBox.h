/*
 * ofxWTextBox.h
 *
 *  Created on: 26/06/2010
 *      Author: arturo
 */

#ifndef OFXWTEXTBOX_H_
#define OFXWTEXTBOX_H_

#include "ofxWidget.h"

class ofxWTextBox: public ofxWidget{
public:
	ofxWTextBox(const string & name):ofxWidget(name){
		cursorPosition = 0;
	}

	void init(const string & title, const string & style){
		this->title = title;
		setStyles("textbox",style);
	}

	void render(ofxWidgetsStyle & style){

		// border
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		ofSetLineWidth(style.border.lineWidth);
		ofRect(style.position.x,style.position.y,style.border.width,style.border.height);

		if(state==OFX_WIDGET_PRESSED){
			style.text.font->drawString(text.substr(0,cursorPosition) + "|" + text.substr(cursorPosition),style.position.x,style.position.y+style.text.font->getLineHeight());

		}else{
			style.text.font->drawString(text,style.position.x,style.position.y+style.text.font->getLineHeight());
		}
	}
	ofRectangle getActiveArea(ofxWidgetsStyle & style){
		return ofRectangle(style.position.x,style.position.y,style.border.width,style.border.height);
	}
	ofRectangle getTotalArea(ofxWidgetsStyle & style){
		return ofRectangle(style.position.x,style.position.y,style.border.width,style.border.height);
	}

	ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		switch(event){
		case OFX_W_E_KEY_RELEASED:
			if(currentState==OFX_WIDGET_PRESSED){
				if(args.key==OF_KEY_BACKSPACE){
					if(text.length() && cursorPosition){
						text = text.substr(0,cursorPosition-1)+text.substr(cursorPosition);
						cursorPosition--;
					}
				}else if(args.key==OF_KEY_LEFT){
					cursorPosition=ofClamp(cursorPosition-1,0,text.size());
				}else if(args.key==OF_KEY_RIGHT){
					cursorPosition=ofClamp(cursorPosition+1,0,text.size());
				}else if(args.key==OF_KEY_RETURN){
					text += '\n';
					cursorPosition++;
				}else{
					text = text.substr(0,cursorPosition) + (char)args.key + text.substr(cursorPosition);
					cursorPosition++;
				}
			}
			return currentState;
			break;
		case OFX_W_E_POINTER_OVER:
			if(currentState==OFX_WIDGET_UNFOCUSED)
				return OFX_WIDGET_FOCUSED;
			break;
		case OFX_W_E_POINTER_PRESSED:
			if(currentState==OFX_WIDGET_FOCUSED || currentState==OFX_WIDGET_PRESSED){
				cursorPosition=text.length();
				for(unsigned i=1; i<text.length(); i++){
					if(getCurrentStyle().text.font->getStringBoundingBox(text.substr(0,i),0,0).width>args.relative_x*getActiveArea(getCurrentStyle()).width){
						cursorPosition = i-1;
						break;
					}
				}
				return OFX_WIDGET_PRESSED;
			}
			break;
		case OFX_W_E_POINTER_RELEASED:
			if(currentState==OFX_WIDGET_PRESSED && !mouseIn())
				return OFX_WIDGET_UNFOCUSED;
			break;
		default:
			return currentState;
		}
		return currentState;
	}

	int getValueI(){ return 0; }
	float getValueF(){ return 0; }
	bool getValueB(){ return 0; }

	string getText(){ return text; }
	void setText(string text){this->text = text;}
	string getValueS(){ return getText(); }
private:
	string text;
	int cursorPosition;
};

#endif /* OFXWTEXTBOX_H_ */
