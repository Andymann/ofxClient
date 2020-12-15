#include "ofApp.h"
bool bActive = false;
string lblServer = "Server";
string lblSetNetwork = "Click to set Network";
static string sXmlFile = "settings.xml";
string sServer;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Client");
    //ofSetEscapeQuitsApp( false );
    ofBackground(60,60,70);
    pixelsLoaded = false;
    readSettings();
    setupGui();
    client.setup(sServer, 44999);
    
}

void ofApp::readSettings(){
    if (xmlSettings.loadFile(sXmlFile)) {
        ofLogVerbose()<<"XML loaded"<<endl;
        //addLog("XML loaded");
        sServer = xmlSettings.getValue("Server", "127.0.0.1");
    }else{
        ofLogVerbose("Could not load xml. Reverting to default values.");
        //addLog("Could not load xml. Reverting to default values.");
    }
}

void ofApp::saveSettings(){
    xmlSettings.setValue("Server",sServer);
    xmlSettings.save(sXmlFile);
}

void ofApp::setupGui(){
    font.load(OF_TTF_MONO, 23);
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );

        
        //cmbNic = gui->addDropdown(sNic, optsNic);
    txtNetwork = gui->addTextInput("Server");
    txtNetwork->setText(sServer);
    
    //btnSetNetwork = gui->addButton(lblSetNetwork);
        
    gui->setTheme(new myCustomTheme() );
    gui->setWidth(640);
   
    // once the gui has been assembled, register callbacks to listen for component specific events
      
    gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
}

//--------------------------------------------------------------
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e){
    sServer = e.target->getText();
    client.setup(sServer, 44999);
}

//--------------------------------------------------------------
void ofApp::update(){
    client.update(client.getMessageSize() /*91920*/);
        
    if( client.getState() == COMPLETE ){
        //tex.loadData(client.pixels, 750, 450, OF_IMAGE_FORMAT_JPEG);
        pixelsLoaded = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    string statusStr =  "status: " + client.getStateStr();
    statusStr += " -- loaded "+ofToString(client.getPctLoaded(), 2)+"% of" + ofToString(client.getImageSize() );
    
    ofSetColor(255, 0, 255);
    ofDrawBitmapString(statusStr, 10, 20);
    ofDrawBitmapString("client - launch server than hit c key to connect - r to reset the state", 10, ofGetHeight()-20);
    
    ofSetColor(255, 255, 255);
    if( pixelsLoaded ){
        ofImage img = client.getImage();
        if(img.isAllocated()){
            img.draw(0,0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'c'){
        client.setup("127.0.0.1", 44999);
    }
    if(key == 'r'){
        client.reset();
    }
    
    if(key == 's'){
        client.saveImage();
    }
    
    if(key == ' '){
        bActive = !bActive;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit(){
    saveSettings();
}
