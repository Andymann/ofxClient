#include "ofApp.h"
bool bActive = false;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Client");
    ofBackground(60,60,70);
    //tex.allocate(640, 480, GL_RGB);
    pixelsLoaded = false;
    client.setup("127.0.0.1", 44999);
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
