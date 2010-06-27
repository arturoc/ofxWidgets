/*
 * ofxYAFrame.h
 *
 *  Created on: 20-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxWSlider.h"
#include "ofxWButton.h"
#include "ofxWToggle.h"
#include "ofxWFps.h"
#include "ofxWStyleLoader.h"
#include "ofxWSpinSlider.h"
#include "ofxXmlSettings.h"
#include "ofxWTextBox.h"

class ofxWFrame: public ofxWidget{
public:

	ofxWFrame():ofxWidget(""){
		setStyle("default");
		title = "frame";

	}

	void init(float x, float y, float width, float height, const string & title="", const string & name="", bool growOnHeight=true){
		frameStyle.width = width;
		frameStyle.border.width = width + frameStyle.hSpacing*2.0;
		frameStyle.height = height;
		frameStyle.border.height = height + frameStyle.vSpacing*2.0;
		frameStyle.position.x = x;
		frameStyle.position.y = y;
		frameStyle.growOnHeight = growOnHeight;
		this->title = title;
		this->name = name;
	}

	void init(const ofRectangle & shape, const string & title="", const string & name="", bool growOnHeight = true){
		init(shape.x, shape.y, shape.width, shape.height, title, name, growOnHeight);
	}

	void addSaveButton(const string & filename, const string & xml_root){
		ofxWButton * button = new ofxWButton("load_" + name);
		button->init("save to xml",style);

		button->setPosition(getNextPosition());

		controls.push_back(button);

		this->filename = filename;
		this->xml_root = xml_root;

		ofAddListener(button->intEvent,this,&ofxWFrame::onSave);
	}

	void addLoadButton(const string & filename, const string & xml_root){
		ofxWButton * button = new ofxWButton("save_" + name);
		button->init("load from xml",style);

		button->setPosition(getNextPosition());

		controls.push_back(button);

		this->filename = filename;
		this->xml_root = xml_root;

		ofAddListener(button->intEvent,this,&ofxWFrame::onLoad);
	}

	void onSave(int & pressed){
		if(pressed){
			saveTo(filename,xml_root);
		}
	}

	void onLoad(int & pressed){
		if(pressed){
			loadFrom(filename,xml_root);
		}
	}

	void saveTo(const string & filename, const string & xml_root){
		ofxXmlSettings xml;
		xml.addTag(xml_root);
		xml.pushTag(xml_root);
		map<string,ofxWidget*>::iterator it;
		for(it=controlsIndex.begin(); it!=controlsIndex.end();it++){
			it->second->saveTo(xml,it->first);
		}
		xml.popTag();
		xml.saveFile(filename);
	}

	void loadFrom(const string & filename, const string & xml_root){
		ofxXmlSettings xml;
		xml.loadFile(filename);
		xml.pushTag(xml_root);
		map<string,ofxWidget*>::iterator it;
		for(it=controlsIndex.begin(); it!=controlsIndex.end();it++){
			it->second->loadFrom(xml,it->first);
		}
		xml.popTag();
	}

	int getValueI(string controlName, int defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueI();
		}else{
			return defaultValue;
		}
	}

	float getValueF(string controlName, float defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueF();
		}else{
			return defaultValue;
		}
	}

	string getValueS(string controlName){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueS();
		}else{
			return "";
		}
	}

	bool getValueB(string controlName, bool defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueB();
		}else{
			return defaultValue;
		}
	}
	void setStyle(const string & _style){
		setDisabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setEnabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setFocusedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setPressedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setOutStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		frameStyle = ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style);
		style=_style;
	}

	virtual void enable(){
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->enable();
		}
		ofxWidget::enable();
	}

	virtual void disable(){
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->disable();
		}
		ofxWidget::disable();
	}


	virtual void setVisible(bool visible){
		ofxWidget::setVisible(visible);
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->setVisible(visible);
		}
	}

	virtual void render(ofxWidgetsStyle & style){
		ofPushMatrix();

		ofTranslate(frameStyle.position.x,frameStyle.position.y);
		ofSetRectMode(OF_RECTMODE_CORNER);

		//background
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(frameStyle.background.color.r,frameStyle.background.color.g,frameStyle.background.color.b, frameStyle.background.color.a);
		ofRect(0,frameStyle.decoration_h,frameStyle.border.width,frameStyle.border.height);
		ofDisableAlphaBlending();

		// border
		ofNoFill();
		ofSetColor(frameStyle.border.color.r,frameStyle.border.color.g,frameStyle.border.color.b);
		ofSetLineWidth(frameStyle.border.lineWidth);
		ofRect(0,frameStyle.decoration_h,frameStyle.border.width,frameStyle.border.height);

		// menu
		ofRect(0,0,frameStyle.border.width,frameStyle.decoration_h);
		if(style.text.ttf){
			style.text.font->drawString(title,frameStyle.hSpacing - frameStyle.border.lineWidth,(frameStyle.decoration_h - style.text.font->getLineHeight())*1.5);
		}else{
			ofDrawBitmapString(title, 10, 10);
		}

		ofPopMatrix();

		static ofEventArgs voidArgs;
		//if(!isVisible()){
			for(unsigned i=0; i<controls.size(); i++){
				controls[i]->draw(voidArgs);
			}
		//}
	}
	ofRectangle getActiveArea(ofxWidgetsStyle & style){
		ofRectangle area(frameStyle.position.x,frameStyle.position.y,frameStyle.width,20);
		return area;
	}

	ofRectangle getTotalArea(ofxWidgetsStyle & style){
		return getActiveArea(style);
	}

	ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		static ofPoint prevMousePos;
		ofxWidgetsState nextState = currentState;;

		switch(currentState){
		case OFX_WIDGET_UNFOCUSED:
			if(event == OFX_W_E_POINTER_OVER)
				nextState = OFX_WIDGET_FOCUSED;
		break;

		case OFX_WIDGET_FOCUSED:
			if(event == OFX_W_E_POINTER_PRESSED){
				prevMousePos.x = args.abs_x;
				prevMousePos.y = args.abs_y;
				nextState = OFX_WIDGET_PRESSED;
			}
			if(event == OFX_W_E_POINTER_OUT){
				nextState = OFX_WIDGET_UNFOCUSED;
			}
		break;

		case OFX_WIDGET_OUT:
			if(event == OFX_W_E_POINTER_OVER){
				nextState = OFX_WIDGET_PRESSED;
			}
		case OFX_WIDGET_PRESSED:
			if(event == OFX_W_E_POINTER_OVER || event == OFX_W_E_POINTER_OUT){
				ofPoint posIncrement = ofPoint(args.abs_x,args.abs_y) - prevMousePos;
				frameStyle.position += posIncrement;

				for(unsigned i=0; i<controls.size(); i++){
					controls[i]->setPosition(controls[i]->getControlPosition() + posIncrement);
				}

				prevMousePos.x = args.abs_x;
				prevMousePos.y = args.abs_y;
			}
			if(event == OFX_W_E_POINTER_OUT){
				nextState = OFX_WIDGET_OUT;
			}
			if(event == OFX_W_E_POINTER_RELEASED){
				nextState = OFX_WIDGET_FOCUSED;
			}
		break;

		default:
			nextState = currentState;
		}

		return nextState;
	}

	void update(){
		/*float borderHeight = 0;
		float maxWidth	 = 0;
		float maxX		 = 0;
		for(unsigned i=0; i<controls.size(); i++){
			float controlX = controls[i]->getControlArea().x;
			float totalWidth = controlX * 2 + controls[i]->getControlArea().width;

			if( controlX > maxX ) maxX = controlX;
			if( totalWidth > maxWidth ) maxWidth = totalWidth;

			borderHeight += frameStyle.vSpacing + controls[i]->getControlArea().height;
		}
		if(controls.size()>0){
			borderHeight += controls[0]->getControlArea().y * 2 + 20;
			frameStyle.border.width = maxWidth;
			frameStyle.border.height = borderHeight;
			frameStyle.width = maxWidth;
			frameStyle.height = borderHeight;
		}*/

	}

	ofxWSlider & addSlider(const string & title, int * value, int min, int max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider(controlName);
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);

		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, float * value, float min, float max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider(controlName);
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, int value, int min, int max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider(controlName);
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, float value, float min, float max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider(controlName);
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}


	ofxWSpinSlider & addSpinSlider(const string & title, int * value, int min, int max, int step=1, string controlName="", const string & _style=""){
		ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
		slider->init(title,value,min,max,step,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);

		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSpinSlider & addSpinSlider(const string & title, float * value, float min, float max, float step=1,  string controlName="", const string & _style=""){
		ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
		slider->init(title,value,min,max,step,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSpinSlider & addSpinSlider(const string & title, int value, int min, int max, int step=1, string controlName="", const string & _style=""){
		ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
		slider->init(title,value,min,max,step,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSpinSlider & addSpinSlider(const string & title, float value, float min, float max, float step=1,  string controlName="", const string & _style=""){
		ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
		slider->init(title,value,min,max,step,_style==""?style:_style);

		slider->setPosition(getNextPosition());


		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWButton & addButton(const string & title, int * value, string controlName="", string _style=""){

		ofxWButton * button = new ofxWButton(controlName);
		button->init(title,value,_style==""?style:_style);

		button->setPosition(getNextPosition());


		controls.push_back(button);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=button;
		return *button;
	}

	ofxWButton & addButton(const string & title, bool * value, string controlName="", string _style=""){

		ofxWButton * button = new ofxWButton(controlName);
		button->init(title,value,_style==""?style:_style);

		button->setPosition(getNextPosition());


		controls.push_back(button);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=button;
		return *button;
	}

	ofxWButton & addButton(const string & title, int value, string controlName="", string _style=""){
		cout << "adding " << controlName << " value: " << value << endl;
		ofxWButton * button = new ofxWButton(controlName);
		button->init(title,value,_style==""?style:_style);

		button->setPosition(getNextPosition());


		controls.push_back(button);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=button;
		return *button;
	}

	ofxWButton & addButton(const string & title, string controlName="", string _style=""){

		ofxWButton * button = new ofxWButton(controlName);
		button->init(title,_style==""?style:_style);

		button->setPosition(getNextPosition());


		controls.push_back(button);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=button;
		return *button;
	}


	ofxWToggle & addToggle(const string & title, int * value, string controlName="", string _style=""){
		ofxWToggle * toggle = new ofxWToggle(controlName);
		toggle->init(title,value,_style==""?style:_style);

		toggle->setPosition(getNextPosition());


		controls.push_back(toggle);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=toggle;
		return *toggle;
	}

	ofxWToggle & addToggle(const string & title, bool * value, string controlName="", string _style=""){
		ofxWToggle * toggle = new ofxWToggle(controlName);
		toggle->init(title,value,_style==""?style:_style);

		toggle->setPosition(getNextPosition());


		controls.push_back(toggle);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=toggle;
		return *toggle;
	}

	ofxWToggle & addToggle(const string & title, int value, string controlName="", string _style=""){
		ofxWToggle * toggle = new ofxWToggle(controlName);
		toggle->init(title,value,_style==""?style:_style);

		toggle->setPosition(getNextPosition());


		controls.push_back(toggle);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=toggle;
		return *toggle;
	}

	ofxWToggle & addToggle(const string & title, string controlName="", string _style=""){
		ofxWToggle * toggle = new ofxWToggle(controlName);
		toggle->init(title,_style==""?style:_style);

		toggle->setPosition(getNextPosition());


		controls.push_back(toggle);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=toggle;
		return *toggle;
	}

	void groupedPressed(const void * sender, bool & pressed){
		if(pressed){
			map<string,vector<ofxWToggle*> >::iterator group;
			for(group=groups.begin();group!=groups.end();group++){
				bool groupFound = false;
				for(unsigned i=0; i<group->second.size(); i++){
					if(group->second[i]==sender){
						groupFound=true;
						break;
					}
				}
				if(groupFound){
					for(unsigned i=0; i<group->second.size(); i++){
						if(group->second[i]!=sender){
							if(group->second[i]->btargetValue) *group->second[i]->btargetValue=false;
							else if(group->second[i]->itargetValue) *group->second[i]->itargetValue=0;
							else group->second[i]->value=0;
						}
					}
					break;
				}
			}
		}
	}

	ofxWToggle & addGroupedToggle(const string & title, int * value, string group, string controlName="", string _style=""){
		ofxWToggle & toggle = addToggle(title,value,controlName,style);
		groups[group].push_back(&toggle);
		ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
		return toggle;
	}

	ofxWToggle & addGroupedToggle(const string & title, bool * value, string group, string controlName="", string _style=""){
		ofxWToggle & toggle = addToggle(title,value,controlName,style);
		groups[group].push_back(&toggle);
		ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
		return toggle;
	}

	ofxWToggle & addGroupedToggle(const string & title, int value, string group, string controlName="", string _style=""){
		ofxWToggle & toggle = addToggle(title,value,controlName,style);
		groups[group].push_back(&toggle);
		ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
		return toggle;
	}

	ofxWToggle & addGroupedToggle(const string & title, string group, string controlName="", string _style=""){
		ofxWToggle & toggle = addToggle(title,controlName,style);
		groups[group].push_back(&toggle);
		ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
		return toggle;
	}

	ofxWTextBox & addTextBox(const string & title, string text="", string controlName="", string _style=""){
		ofxWTextBox * textBox = new ofxWTextBox(controlName);
		textBox->init(title,_style==""?style:_style);

		textBox->setPosition(getNextPosition());
		textBox->setText(text);


		controls.push_back(textBox);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=textBox;
		return *textBox;
	}

	ofxWidgetFps & addFps(string controlName=""){
		ofxWidgetFps * fps = new ofxWidgetFps(controlName);

		/*fps->setEnabledStyle(buttonStyle);
		fps->setDisabledStyle(buttonStyle);
		fps->setFocusedStyle(buttonFocusedStyle);
		fps->setPressedStyle(buttonPressedStyle);
		fps->setOutStyle(buttonPressedStyle);*/


		fps->setPosition(getNextPosition());


		controls.push_back(fps);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=fps;
		return *fps;
	}

	ofPoint getNextPosition(){
		float totalHeight = frameStyle.vSpacing + frameStyle.decoration_h;
		float totalWidth = frameStyle.hSpacing;
		//float frameWidth = frameStyle.width!=-1?frameStyle.width:ofGetWidth();
		float frameHeight = frameStyle.height!=-1?frameStyle.height:ofGetHeight();
		float maxControlWidth = 0;
		for(unsigned int i = 0; i<controls.size(); i++){
			float controlWidth=controls[i]->getControlTotalArea().width;
			float controlHeight=controls[i]->getControlTotalArea().height;
			totalHeight += controlHeight;
			totalHeight += frameStyle.vSpacing;
			if(controlWidth>maxControlWidth)
				maxControlWidth=controlWidth;
			if(totalHeight>frameHeight && !frameStyle.growOnHeight){
				totalHeight=frameStyle.vSpacing + frameStyle.decoration_h;
				totalWidth+=maxControlWidth+frameStyle.hSpacing;
				//maxControlWidth=0;
			}
		}

		// update size when adding new control
		// TODO: should this be a separate function called from addControls
		frameStyle.height = MAX(frameStyle.height,totalHeight);
		frameStyle.border.height = MAX(frameStyle.border.height,totalHeight+frameStyle.vSpacing);
		frameStyle.width = MAX(frameStyle.width, totalWidth + maxControlWidth);
		frameStyle.border.width = MAX(frameStyle.border.width, totalWidth + maxControlWidth + frameStyle.hSpacing*2);
		return ofPoint(frameStyle.position.x+totalWidth,frameStyle.position.y+totalHeight);
	}

	int getValueI(){
		return 0;
	}

	float getValueF(){
		return 0;
	}

	bool getValueB(){
		return 0;
	}
protected:
	vector<ofxWidget*> controls;
	map<string, ofxWidget*> controlsIndex;
	map<string, vector<ofxWToggle*> > groups;
	string style;
	ofxWFrameStyle		frameStyle;

	string				filename;
	string				xml_root;
};


