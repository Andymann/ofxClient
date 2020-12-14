//
//  pixelClient.h
//  ofxClient
//
//  Created by Andy Fischer on 12.12.20.
//

//#ifndef pixelClient_h
//#define pixelClient_h
//#endif /* pixelClient_h */

#include "ofxNetwork.h"
#include "ofMain.h"

//#define NUM_BYTES_IN_IMG (320 * 240 * 3)

enum clientState{
    NONE, READY, RECEIVING, COMPLETE, FAULTY
};

class pixelClient{
    public:
    
    //-----------------------------------------------------
    pixelClient(){
        state                = NONE;
        //totalBytes            = NUM_BYTES_IN_IMG;
        bytesRecieved        = 0;
        //memset(pixels, 255, totalBytes);
    }
    
    //-----------------------------------------------------
    int getState(){
        return state;
    }

    //-----------------------------------------------------
    string getStateStr(){
        if(state == NONE)return "NONE";
        else
        if(state == READY)return "READY";
        else
        if(state == RECEIVING)return "RECEIVING";
        else
        if(state == COMPLETE)return "COMPLETE";
        
        return "ERR";
    }
    
    //-----------------------------------------------------
    float getPctLoaded(){
        return ( (float)bytesRecieved / totalBytes ) * 100;
    }
    
    int getImageSize(){
        return totalBytes;
    }
    
    ofImage getImage(){
        return imgReceived;
    }

    //-----------------------------------------------------
    void reset(){
        //ofLogNotice("pixelClient.reset()");
        state = READY;
        bytesRecieved = 0;
        totalBytes = 0;
        
    }

    //-----------------------------------------------------
    void setup(string ip, int port = 11999){
        tcpClient.setup(ip, port);
        bytesRecieved = 0;
        pixels.clear();
        state = READY;
    }


    //-----------------------------------------------------
    void update(int bytesToGet = 9192){
        
        char tmpBuffer[bytesToGet];
        
        if( bNextReset){
            bNextReset = false;
            reset();
        }
        if( state == READY){
            if(tcpClient.isConnected() ){
                processIncoming();
            }  else{
                //ofLogNotice("not connected");
            }
            
        }else if (state == RECEIVING){
            processReceiving(bytesToGet);
            
        }else if (state == FAULTY){
            processFaultyData(bytesToGet);
            
        }else{}
 
    }
    
    int getMessageSize(){
        return iMESSAGESIZE;
    }
    
    void processIncoming(){
        char tmpHeaderBuffer[16];
        int numReceived = tcpClient.receiveRawBytes(tmpHeaderBuffer, 16);
        
        if (numReceived == -1){
            //---Keine Daten am Hahn
        }else if(numReceived == 16){
            vector<unsigned char>bufHeader;
            for(int i=0; i<16; i++){
                bufHeader.push_back(tmpHeaderBuffer[i]);
            }
            if((bufHeader[1]==0xAF) && (bufHeader[2]==0xAF)){
                //ofLogNotice("received correct headerdata");
                
                iMESSAGESIZE = bufHeader[3]*1000;
                
                pixels.clear();
                totalBytes=(bufHeader[15]<<24) +
                (bufHeader[14]<<16) +
                (bufHeader[13]<<8) +
                bufHeader[12]-16;
                state = RECEIVING;
            }else{
                state = FAULTY;
            }
            
        }
    }
    
    void processReceiving(int bytesToGet){
        char tmpBuffer[bytesToGet];
        
        if( bytesToGet + bytesRecieved >= totalBytes ){
            bytesToGet -= ( ( bytesToGet + bytesRecieved ) - totalBytes );
        }
        int numRecieved = tcpClient.receiveRawBytes(tmpBuffer, bytesToGet);
        
        if( numRecieved > 0 ){
            //state = RECEIVING;
            for(int i=0; i<numRecieved; i++){
                pixels.push_back(tmpBuffer[i]);
            }
            bytesRecieved += numRecieved;
        }
        
        if( bytesRecieved >= totalBytes ){
            createImageFromPixels();
            state = COMPLETE;
            
        }
    }
    
    void processFaultyData(int bytesToGet){
        char tmpBuffer[bytesToGet];
        int numReceived = tcpClient.receiveRawBytes(tmpBuffer, bytesToGet);
        if(numReceived <= 0){
            //ofLogNotice("Queue empty");
            bNextReset = true;
            state = COMPLETE;
        }
    }
    
    void createImageFromPixels(){
        //char tmpBuff[ totalBytes ];
        //std::copy(&pixels[pixels.begin()], &pixels[pixels.end()], tmpBuff);
        
         //----also Works, but might be slower
        //for(int i=0; i<iImagesizeFromHeaderData; i++){
        //    tmpBuff[i] = *&bufImg[i+iStartOfImage];
        //}
        //if((char)pixels[totalBytes-1]=='\xD9'){
        if((char)pixels[1]=='\xD8'){
            
            char tmpBuff[ totalBytes ];
            std::copy(&pixels[0], &pixels[totalBytes], tmpBuff);
            
            //char* a = &pixels[0];
            
           ofBuffer buf;
            buf.set((tmpBuff), totalBytes);
           //buf.set(*(tmpBuff), totalBytes);
            
            imgReceived.allocate(640, 480, OF_IMAGE_COLOR);
           //imgReceived.load(buf);
            ofLoadImage(imgReceived, buf);
            
            
            //imgReceived.load(a);
            //bCopyingToBuffer=false;
            //tcpClient.send("IMAGE COMPLETE");
            //tcpClient.send("OK");
            bNextReset = true;
        }else{
            ofLogNotice("Not a valid JPEG Image");
            imgReceived.clear();
            
            bNextReset=true;
            //tcpClient.send("FAIL");
            /*
            ofstream fout("/Users/andyfischer/Documents/openFrameworks/apps/myApps/ofxTCPSyphonClient/bin/data/data.jpg", ios::out | ios::binary);
            size_t size = bufImg.size();
             fout.write((char*)&bufImg[16], bufImg.size() * sizeof(size));
             fout.close();
             */
        }
        
        //imgInThread.load(ofBuff);
        
        
    }
    
    
    void saveImage(){
        if(imgReceived.isAllocated()){
            ofSaveImage(imgReceived, "test.jpeg");
        }else{
            ofLogNotice("pixelClient.save(): Not allocated");
        }
    }

    clientState state;
    int bytesRecieved;
    int totalBytes;
    int iMESSAGESIZE = 10000;
    
    ofxTCPClient tcpClient;
    vector<unsigned char>pixels;
    ofImage imgReceived;
    bool bNextReset =false;
};
