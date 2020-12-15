#pragma once

#include "ofMain.h"
#include "pixelClient.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
    void setupGui();
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
    ofxXmlSettings xmlSettings;
    void readSettings();
    void saveSettings();
    
    pixelClient client;
    ofTexture tex;
    
    bool pixelsLoaded;
    
    ofTrueTypeFont font;
    ofxDatGui* gui;
    ofxDatGuiTextInput* txtNetwork;
    ofxDatGuiButton* btnSetNetwork;
        
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 12;
        //font.file = "path/to/font.ttf";
        init();
    }
};


