#include "ofxWFrame.h"

ofPoint		ofxWFrame::next_pos;
float		ofxWFrame::max_frame_width=0;

ofxWFrame::ofxWFrame():ofxWidget(""){
	setStyle("default");
	title = "frame";

}

void ofxWFrame::init(const string & title, const string & name, bool growOnHeight){
	if(next_pos.y + frameStyle.interFrameVSpace + frameStyle.decoration_h*2>ofGetHeight()){
		next_pos.y = 0;
		next_pos.x += max_frame_width + frameStyle.interFrameHSpace;
		max_frame_width = 0;
	}
	auto_pos = true;
	init(next_pos.x+frameStyle.interFrameHSpace,next_pos.y+frameStyle.interFrameVSpace,0,0,title,name,growOnHeight);
	next_pos.y += frameStyle.decoration_h*2+frameStyle.interFrameVSpace;
	ofLog(OF_LOG_NOTICE,"frame " + title + " at " + ofToString(frameStyle.position.x) + "," + ofToString(frameStyle.position.y));
}

void ofxWFrame::init(float x, float y, float width, float height, const string & title, const string & name, bool growOnHeight){
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

void ofxWFrame::init(const ofRectangle & shape, const string & title, const string & name, bool growOnHeight){
	init(shape.x, shape.y, shape.width, shape.height, title, name, growOnHeight);
}

void ofxWFrame::addSaveButton(const string & filename, const string & xml_root){
	ofxWButton * button = new ofxWButton("load_" + name);
	button->init("save to xml",style);

	button->setPosition(getNextPosition());

	controls.push_back(button);

	this->filename = filename;
	this->xml_root = xml_root;

	ofAddListener(button->intEvent,this,&ofxWFrame::onSave);
}

void ofxWFrame::addLoadButton(const string & filename, const string & xml_root){
	ofxWButton * button = new ofxWButton("save_" + name);
	button->init("load from xml",style);

	button->setPosition(getNextPosition());

	controls.push_back(button);

	this->filename = filename;
	this->xml_root = xml_root;

	ofAddListener(button->intEvent,this,&ofxWFrame::onLoad);
}

void ofxWFrame::onSave(int & pressed){
	if(pressed){
		saveTo(filename,xml_root);
	}
}

void ofxWFrame::onLoad(int & pressed){
	if(pressed){
		loadFrom(filename,xml_root);
	}
}

void ofxWFrame::saveTo(const string & filename, const string & xml_root){
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

void ofxWFrame::loadFrom(const string & filename, const string & xml_root){
	ofxXmlSettings xml;
	xml.loadFile(filename);
	xml.pushTag(xml_root);
	map<string,ofxWidget*>::iterator it;
	for(it=controlsIndex.begin(); it!=controlsIndex.end();it++){
		it->second->loadFrom(xml,it->first);
	}
	xml.popTag();
}

int ofxWFrame::getValueI(string controlName, int defaultValue){
	if(controlsIndex.find(controlName)!=controlsIndex.end()){
		ofxWidget * control = controlsIndex[controlName];
		return control->getValueI();
	}else{
		return defaultValue;
	}
}

float ofxWFrame::getValueF(string controlName, float defaultValue){
	if(controlsIndex.find(controlName)!=controlsIndex.end()){
		ofxWidget * control = controlsIndex[controlName];
		return control->getValueF();
	}else{
		return defaultValue;
	}
}

string ofxWFrame::getValueS(string controlName){
	if(controlsIndex.find(controlName)!=controlsIndex.end()){
		ofxWidget * control = controlsIndex[controlName];
		return control->getValueS();
	}else{
		return "";
	}
}

bool ofxWFrame::getValueB(string controlName, bool defaultValue){
	if(controlsIndex.find(controlName)!=controlsIndex.end()){
		ofxWidget * control = controlsIndex[controlName];
		return control->getValueB();
	}else{
		return defaultValue;
	}
}

void ofxWFrame::setStyle(const string & _style){
	setDisabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
	setEnabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
	setFocusedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
	setPressedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
	setOutStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
	frameStyle = ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style);
	style=_style;
}

void ofxWFrame::enable(){
	for(unsigned int i=0; i< controls.size(); i++){
		controls[i]->enable();
	}
	ofxWidget::enable();
}

void ofxWFrame::disable(){
	for(unsigned int i=0; i< controls.size(); i++){
		controls[i]->disable();
	}
	ofxWidget::disable();
}


void ofxWFrame::setVisible(bool visible){
	ofxWidget::setVisible(visible);
	for(unsigned int i=0; i< controls.size(); i++){
		controls[i]->setVisible(visible);
	}
}

void ofxWFrame::render(ofxWidgetsStyle & style){
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

	//tabs
	for(unsigned i=0;i<tabs.size();i++){
		ofRect(i*frameStyle.border.width/tabs.size(),frameStyle.decoration_h,frameStyle.border.width/tabs.size(),frameStyle.tabs_h);

		/*if(i!=tabs.size()-1)
			ofLine((i+1)*frameStyle.border.width/tabs.size(),frameStyle.decoration_h,(i+1)*frameStyle.border.width/tabs.size(),frameStyle.decoration_h+frameStyle.tabs_h);*/
		style.text.font->drawString(tabTitles[i],i*frameStyle.border.width/tabs.size(),frameStyle.decoration_h + (frameStyle.tabs_h - style.text.font->getLineHeight())*1.5);
	}

	/*static ofEventArgs voidArgs;
	//if(!isVisible()){
		for(unsigned i=0; i<controls.size(); i++){
			controls[i]->draw(voidArgs);
		}
	//}*/
	ofPopMatrix();
}
ofRectangle ofxWFrame::getActiveArea(ofxWidgetsStyle & style){
	ofRectangle area(frameStyle.position.x,frameStyle.position.y,frameStyle.border.width,tabs.size()?frameStyle.decoration_h+frameStyle.tabs_h:frameStyle.decoration_h);
	return area;
}

ofRectangle ofxWFrame::getTotalArea(ofxWidgetsStyle & style){
	return ofRectangle(frameStyle.position.x,frameStyle.position.y,frameStyle.border.width,frameStyle.border.height);
}

ofxWidgetsState ofxWFrame::manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
	static ofPoint prevMousePos;
	ofxWidgetsState nextState = currentState;;

	switch(currentState){
	case OFX_WIDGET_UNFOCUSED:
		if(event == OFX_W_E_POINTER_OVER)
			nextState = OFX_WIDGET_FOCUSED;
	break;

	case OFX_WIDGET_FOCUSED:
		if(event == OFX_W_E_POINTER_PRESSED){
			if(tabs.size() && args.relative_y>frameStyle.decoration_h/(frameStyle.decoration_h+frameStyle.tabs_h)){

				pressedState = OFX_W_FRAME_SELECTING_TAB;
				pressedTab = args.relative_x * tabs.size();
				ofLog(OF_LOG_VERBOSE,"frame selecting tab %i in relative pos y %f",pressedTab,args.relative_y);
			}else{
				ofLog(OF_LOG_VERBOSE,"frame moving");
				pressedState = OFX_W_FRAME_MOVING;
			}
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
		if((event == OFX_W_E_POINTER_OVER || event == OFX_W_E_POINTER_OUT) && pressedState==OFX_W_FRAME_MOVING){
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
			ofLog(OF_LOG_VERBOSE,"frame release, pressed tab %i, current mouse tab %f, relative x %f ", pressedTab, args.relative_x * tabs.size(),args.relative_x );
			if(pressedState==OFX_W_FRAME_SELECTING_TAB && pressedTab == (int)(args.relative_x * tabs.size())){
				selectTab(pressedTab);
			}
			nextState = OFX_WIDGET_FOCUSED;
		}
	break;

	default:
		nextState = currentState;
	}

	return nextState;
}

void ofxWFrame::update(){
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

void ofxWFrame::addWidget(ofxWidget * widget, string controlName){


	controls.push_back(widget);
	if(tabs.size()){
		tabs[currentTab].push_back(widget);
	}

	if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

	controlsIndex[controlName]=widget;
}


ofxWSlider & ofxWFrame::addSlider(const string & title, int * value, int min, int max, string controlName, const string & _style){
	ofxWSlider * slider = new ofxWSlider(controlName);
	slider->init(title,value,min,max,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSlider & ofxWFrame::addSlider(const string & title, float * value, float min, float max, string controlName, const string & _style){
	ofxWSlider * slider = new ofxWSlider(controlName);
	slider->init(title,value,min,max,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSlider & ofxWFrame::addSlider(const string & title, int value, int min, int max, string controlName, const string & _style){
	ofxWSlider * slider = new ofxWSlider(controlName);
	slider->init(title,value,min,max,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSlider & ofxWFrame::addSlider(const string & title, float value, float min, float max, string controlName, const string & _style){
	ofxWSlider * slider = new ofxWSlider(controlName);
	slider->init(title,value,min,max,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}


ofxWSpinSlider & ofxWFrame::addSpinSlider(const string & title, int * value, int min, int max, int step, string controlName, const string & _style){
	ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
	slider->init(title,value,min,max,step,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSpinSlider & ofxWFrame::addSpinSlider(const string & title, float * value, float min, float max, float step,  string controlName, const string & _style){
	ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
	slider->init(title,value,min,max,step,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSpinSlider & ofxWFrame::addSpinSlider(const string & title, int value, int min, int max, int step, string controlName, const string & _style){
	ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
	slider->init(title,value,min,max,step,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWSpinSlider & ofxWFrame::addSpinSlider(const string & title, float value, float min, float max, float step,  string controlName, const string & _style){
	ofxWSpinSlider * slider = new ofxWSpinSlider(controlName);
	slider->init(title,value,min,max,step,_style==""?style:_style);

	slider->setPosition(getNextPosition());


	addWidget(slider,controlName);
	return *slider;
}

ofxWButton & ofxWFrame::addButton(const string & title, int * value, string controlName, string _style){

	ofxWButton * button = new ofxWButton(controlName);
	button->init(title,value,_style==""?style:_style);

	button->setPosition(getNextPosition());


	addWidget(button,controlName);
	return *button;
}

ofxWButton & ofxWFrame::addButton(const string & title, bool * value, string controlName, string _style){

	ofxWButton * button = new ofxWButton(controlName);
	button->init(title,value,_style==""?style:_style);

	button->setPosition(getNextPosition());


	addWidget(button,controlName);
	return *button;
}

ofxWButton & ofxWFrame::addButton(const string & title, int value, string controlName, string _style){
	cout << "adding " << controlName << " value: " << value << endl;
	ofxWButton * button = new ofxWButton(controlName);
	button->init(title,value,_style==""?style:_style);

	button->setPosition(getNextPosition());


	addWidget(button,controlName);
	return *button;
}

ofxWButton & ofxWFrame::addButton(const string & title, string controlName, string _style){

	ofxWButton * button = new ofxWButton(controlName);
	button->init(title,_style==""?style:_style);

	button->setPosition(getNextPosition());


	addWidget(button,controlName);
	return *button;
}


ofxWToggle & ofxWFrame::addToggle(const string & title, int * value, string controlName, string _style){
	ofxWToggle * toggle = new ofxWToggle(controlName);
	toggle->init(title,value,_style==""?style:_style);

	toggle->setPosition(getNextPosition());


	addWidget(toggle,controlName);
	return *toggle;
}

ofxWToggle & ofxWFrame::addToggle(const string & title, bool * value, string controlName, string _style){
	ofxWToggle * toggle = new ofxWToggle(controlName);
	toggle->init(title,value,_style==""?style:_style);

	toggle->setPosition(getNextPosition());


	addWidget(toggle,controlName);
	return *toggle;
}

ofxWToggle & ofxWFrame::addToggle(const string & title, int value, string controlName, string _style){
	ofxWToggle * toggle = new ofxWToggle(controlName);
	toggle->init(title,value,_style==""?style:_style);

	toggle->setPosition(getNextPosition());


	addWidget(toggle,controlName);
	return *toggle;
}

ofxWToggle & ofxWFrame::addToggle(const string & title, string controlName, string _style){
	ofxWToggle * toggle = new ofxWToggle(controlName);
	toggle->init(title,_style==""?style:_style);

	toggle->setPosition(getNextPosition());


	addWidget(toggle,controlName);
	return *toggle;
}

void ofxWFrame::groupedPressed(const void * sender, bool & pressed){
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

ofxWToggle & ofxWFrame::addGroupedToggle(const string & title, int * value, string group, string controlName, string _style){
	ofxWToggle & toggle = addToggle(title,value,controlName,style);
	groups[group].push_back(&toggle);
	ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
	return toggle;
}

ofxWToggle & ofxWFrame::addGroupedToggle(const string & title, bool * value, string group, string controlName, string _style){
	ofxWToggle & toggle = addToggle(title,value,controlName,style);
	groups[group].push_back(&toggle);
	ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
	return toggle;
}

ofxWToggle & ofxWFrame::addGroupedToggle(const string & title, int value, string group, string controlName, string _style){
	ofxWToggle & toggle = addToggle(title,value,controlName,style);
	groups[group].push_back(&toggle);
	ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
	return toggle;
}

ofxWToggle & ofxWFrame::addGroupedToggle(const string & title, string group, string controlName, string _style){
	ofxWToggle & toggle = addToggle(title,controlName,style);
	groups[group].push_back(&toggle);
	ofAddListener(toggle.boolEvent,this,&ofxWFrame::groupedPressed);
	return toggle;
}

ofxWTextBox & ofxWFrame::addTextBox(const string & title, string text, string controlName, string _style){
	ofxWTextBox * textBox = new ofxWTextBox(controlName);
	textBox->init(title,_style==""?style:_style);

	textBox->setPosition(getNextPosition());
	textBox->setText(text);


	addWidget(textBox,controlName);
	return *textBox;
}

ofxWidgetFps & ofxWFrame::addFps(string controlName){
	ofxWidgetFps * fps = new ofxWidgetFps(controlName);

	/*fps->setEnabledStyle(buttonStyle);
	fps->setDisabledStyle(buttonStyle);
	fps->setFocusedStyle(buttonFocusedStyle);
	fps->setPressedStyle(buttonPressedStyle);
	fps->setOutStyle(buttonPressedStyle);*/


	fps->setPosition(getNextPosition());


	addWidget(fps,controlName);
	return *fps;
}

int ofxWFrame::addTab(const string & title){
	tabTitles.push_back(title);
	tabs[tabs.size()];//,vector<ofxWidget*>());
	selectTab(tabs.size()-1);
	return tabs.size()-1;
}

void ofxWFrame::selectTab(int tab){
	ofLog(OF_LOG_VERBOSE,"selecting tab %i", tab);
	if(tab<(int)tabs.size() && tab>=0){
		currentTab = tab;
		map<int,vector<ofxWidget*> >::iterator it;
		for(it=tabs.begin();it!=tabs.end();it++){
			for(unsigned i=0;i<it->second.size();i++){
				if(it->first==currentTab){
					it->second[i]->setVisible(true);
					it->second[i]->enable();
				}else{
					it->second[i]->setVisible(false);
					it->second[i]->disable();
				}
			}
		}
	}
}

int ofxWFrame::getTabCount(){
	return tabs.size();
}

ofPoint ofxWFrame::getNextPosition(){
	float totalHeight = frameStyle.vSpacing + frameStyle.decoration_h + (tabs.size()?frameStyle.tabs_h:0);
	float totalWidth = frameStyle.hSpacing;
	//float frameWidth = frameStyle.width!=-1?frameStyle.width:ofGetWidth();
	float frameHeight = frameStyle.height!=-1?frameStyle.height:ofGetHeight();
	float maxControlWidth = 0;
	for(unsigned int i = 0; i<(tabs.size()?tabs[currentTab].size():controls.size()); i++){
		float controlWidth;
		float controlHeight;
		if(tabs.size()){
			controlWidth=tabs[currentTab][i]->getControlTotalArea().width;
			controlHeight=tabs[currentTab][i]->getControlTotalArea().height;
		}else{
			controlWidth=controls[i]->getControlTotalArea().width;
			controlHeight=controls[i]->getControlTotalArea().height;
		}
		totalHeight += controlHeight;
		totalHeight += frameStyle.vSpacing;
		if(controlWidth>maxControlWidth)
			maxControlWidth=controlWidth;
		if(totalHeight>frameHeight && !frameStyle.growOnHeight){
			totalHeight=frameStyle.vSpacing + frameStyle.decoration_h + (tabs.size()?frameStyle.tabs_h:0);
			totalWidth+=maxControlWidth+frameStyle.hSpacing;
			//maxControlWidth=0;
		}
	}

	// update size when adding new control
	// TODO: should this be a separate function called from addControls
	float currentHeigth = frameStyle.border.height ;

	frameStyle.height = MAX(frameStyle.height,totalHeight);
	frameStyle.border.height = MAX(frameStyle.border.height,totalHeight+frameStyle.vSpacing);
	frameStyle.width = MAX(frameStyle.width, totalWidth + maxControlWidth);
	frameStyle.border.width = MAX(frameStyle.border.width, totalWidth + maxControlWidth + frameStyle.hSpacing*2);

	if(auto_pos){
		if(frameStyle.border.width>max_frame_width)
			max_frame_width = frameStyle.border.width;
		next_pos.y += frameStyle.border.height - currentHeigth;

		if(frameStyle.border.height + frameStyle.position.y>ofGetHeight()){
			for(unsigned i=0;i<controls.size();i++){
				controls[i]->setPosition(controls[i]->getControlPosition()-ofPoint(frameStyle.position.x,frameStyle.position.y));
			}
			frameStyle.position.y = frameStyle.interFrameVSpace;
			frameStyle.position.x = next_pos.x + max_frame_width + frameStyle.interFrameHSpace*2;

			for(unsigned i=0;i<controls.size();i++){
				controls[i]->setPosition(controls[i]->getControlPosition()+ofPoint(frameStyle.position.x,frameStyle.position.y));
			}
			next_pos.y = frameStyle.position.y + frameStyle.border.height + frameStyle.decoration_h;
			next_pos.x += max_frame_width + frameStyle.interFrameHSpace;
			max_frame_width = frameStyle.border.width;
		}
	}
	return ofPoint(totalWidth + frameStyle.position.x,totalHeight + frameStyle.position.y);
}

int ofxWFrame::getValueI(){
	return 0;
}

float ofxWFrame::getValueF(){
	return 0;
}

bool ofxWFrame::getValueB(){
	return 0;
}
