#include "ofApp.h"


void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE); //what does that mean?
    ofSetFrameRate(30);
//    ofHideCursor();

    loadNextCamera();
}

void ofApp::update()
{
    
    the_stream -> update();
    
    if(the_stream->isFrameNew()){
        
        // I resize the images to the stream's orginal size to process them,
        // then later in the draw mode I can change to any desired size.
        
        int the_stream_w = the_stream->getWidth();
        int the_stream_h = the_stream->getHeight();
        
        background.resize(the_stream_w, the_stream_h);
        movement.resize(the_stream_w, the_stream_h);
        
        auto the_stream_pixels = the_stream->getPixels().getData();
        int i = 0;
        
        for(int y = 0; y < the_stream_h; y++){  //loop though rows
            for(int x = 0; x < the_stream_w; x++){     // loop though columns
                
                float r = the_stream_pixels[i+0];
                float g = the_stream_pixels[i+1];
                float b = the_stream_pixels[i+2];

                if(first == true){
                    avColors.push_back(r);
                    avColors.push_back(g);
                    avColors.push_back(b);
                }else{
                    float OLD_r = avColors[i+0];
                    float NEW_r = ((OLD_r * num_measurements)+ r)/(num_measurements+1);
                    avColors[i+0] = NEW_r;
                    
                    float OLD_g = avColors[i+1];
                    float NEW_g = ((OLD_g * num_measurements) + g)/(num_measurements+1);
                    avColors[i+1] = NEW_g;
                    
                    float OLD_b = avColors[i+2];
                    float NEW_b = ((OLD_b * num_measurements) + b)/(num_measurements+1);
                    avColors[i+2] = NEW_b;
                    
                }
                
                
                // JUST BACKGROUND
                float r_bg = avColors[i+0];
                float g_bg = avColors[i+1];
                float b_bg = avColors[i+2];
                ofColor c_bg;
                c_bg.set(r_bg, g_bg, b_bg);
                background.setColor(x, y, c_bg);
                
                //DISSECTING MOVEMENT:
                
                
                bool r_moved = r > avColors[i+0] + threshold || r < avColors[i+0] - threshold;
                bool g_moved = g > avColors[i+1] + threshold || g < avColors[i+1] - threshold;
                bool b_moved = b > avColors[i+2] + threshold || b < avColors[i+2] - threshold;
                
                
                float r_mv = r;
                float g_mv = g;
                float b_mv = b;
                ofColor c_mv;

//
//                if(r_moved && g_moved && b_moved && !drawing){
//                    c_mv.set(r_mv, g_mv, b_mv, 255);
//                    movement.setColor(x, y, c_mv);
//                }else if(!drawing){
//                    c_mv.set(r_mv, g_mv, b_mv, 0);
//                    movement.setColor(x, y, c_mv);
//                }
                
                if(!drawing){
                    r_mv = 255;
                    g_mv = 255;
                    b_mv = 255;
                    c_mv.set(r_mv, g_mv, b_mv, 0);
                    movement.setColor(x, y, c_mv);
                }
                
                if(r_moved && g_moved && b_moved && drawing){
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    movement.setColor(x, y, c_mv);
                }
                


                i+=3;
            }
        }
        first = false;
        background.update();
        movement.update();
        num_measurements += 1;
    }

    
}

void drawPlus(int x, int y){
    ofDrawRectangle(x - 7, y-25, 14, 50);
    ofDrawRectangle(x - 25, y-7, 50, 14);
}
void drawMinus(int x, int y){
    ofDrawRectangle(x - 25, y-7, 50, 14);
}



void ofApp::draw()
{
    ofBackground(255);
    
    
    float w = ofGetWidth();
    float h = ofGetHeight();
    

    float orig_width = background.getWidth();
    float orig_height = background.getHeight();
    float factor = (w/2.1) / orig_width;
    
    ofApp::img_x = w/2 - (orig_width * factor)/2;
    ofApp::img_y = h/3 - (orig_height * factor)/2;
    ofApp::img_w = orig_width * factor;
    ofApp::img_h = orig_height * factor;
    
    if(!hideMovement && !hideBackgroundWhileDrawing){
        the_stream->draw(img_x, img_y, img_w, img_h);
    }else if(!hideBackgroundWhileDrawing){
        background.draw(img_x, img_y, img_w, img_h);
    }
    
    if(drawing){
        movement.draw(img_x, img_y, img_w, img_h);
        
    }
    
    
    ofPushStyle();
    
        ofSetColor(0, 0, 0);
        string str = "STREAMING LIVE FROM: " + stream_name;
        ofDrawBitmapString(str, img_x, img_y + img_h + 20);
        ofDrawBitmapString(threshold, img_x, img_y + img_h + 35);
        
        if (!hideMovement && drawing){
            ofSetColor(250);
            ofDrawRectangle(img_x, img_y + img_h + 75, img_w, 80);
            ofSetColor(240);
            drawMinus(img_x + (img_w/2), img_y + img_h + 75 + 40);
        }else if (!hideMovement){
            ofSetColor(240);
            ofDrawRectangle(img_x, img_y + img_h + 75, img_w, 80);
            ofSetColor(180);
            drawMinus(img_x + (img_w/2), img_y + img_h + 75 + 40);
        }else{
            ofSetColor(230);
            ofDrawRectangle(img_x, img_y + img_h + 75, img_w, 80);
            ofSetColor(200);
            drawMinus(img_x + (img_w/2), img_y + img_h + 75 + 40);
        }
        
        if (!drawing){
            ofSetColor(240);
            ofDrawRectangle(img_x, img_y + img_h + 185, img_w, 80);
            ofSetColor(180);
            drawPlus(img_x + (img_w/2),img_y + img_h + 185 + 40);
        }else{
            ofSetColor(210);
            ofDrawRectangle(img_x, img_y + img_h + 185, img_w, 80);
            ofSetColor(180);
            drawPlus(img_x + (img_w/2),img_y + img_h + 185 + 40);
        }
    
//        --------
    
        //hidebackgroundWhile Drawing
        ofSetColor(252);
        if (drawing){
            ofDrawRectangle(img_x, img_y + img_h + 295, img_w, 80);
        }
    
        //next stream
    
        ofDrawRectangle(img_x + img_w + 30, img_y, 80, img_h);
        //reset current stream:
        ofDrawRectangle(img_x - 30 - 80, img_y, 80, img_h);
    
    
        //treshold -
        ofDrawRectangle(20, 20, 80, 80);
        //treshold +
        ofDrawRectangle(120, 20, 80, 80);
    
    
    ofPopStyle();

    
}

bool doesContain(int mouseX, int mouseY, float x, float y, float w, float h){
    if(mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h){
        return true;
    }else{
        return false;
    }
    
}


void ofApp::keyPressed(int key){

    if(key == OF_KEY_DOWN){
        threshold -= 2;
    }
    if(key == OF_KEY_UP){
       threshold += 2;
    }
    
}



void ofApp::mousePressed(int x, int y, int button){

    if(doesContain(x,y,img_x, img_y + img_h + 75, img_w, 80) && !drawing){
        hideMovement = true;
    }
    
    if(doesContain(x,y,img_x, img_y + img_h + 185, img_w, 80) && !drawing){
        drawing = true;
        hideMovement = false;
    }else if(doesContain(x,y,img_x, img_y + img_h + 185, img_w, 80) && drawing){
        drawing = false;
        hideBackgroundWhileDrawing = false;
    }
    
    //hidebackgroundWhile Drawing:
    if(doesContain(x,y,img_x, img_y + img_h + 295, img_w, 80) && drawing){
        if(!hideBackgroundWhileDrawing){
            hideBackgroundWhileDrawing = true;
        }else{
            hideBackgroundWhileDrawing = false;
        }
    }
    
    
    //next cam:
    if(doesContain(x,y,img_x + img_w + 30, img_y, 80, img_h)){
        loadNextCamera();
    }
    //reset current cam:
    if(doesContain(x,y,img_x - 30 - 80, img_y, 80, img_h)){
        resetStream();
    }
    
    //threshold -
    if(doesContain(x,y,20, 20, 80, 80)){
        threshold -= 2;
    }
    //threshold +
    if(doesContain(x,y,120, 20, 80, 80)){
        threshold += 2;
    }
    
}

void ofApp::mouseReleased(int x, int y, int button){
    
    if(doesContain(x,y,img_x, img_y + img_h + 75, img_w, 80) && !drawing){
        hideMovement = false;
    }

}


// _                     _ _
//| |__   __ _ _ __   __| | | ___    ___ __ _ _ __ ___   ___ _ __ __ _ ___
//| '_ \ / _` | '_ \ / _` | |/ _ \  / __/ _` | '_ ` _ \ / _ \ '__/ _` / __|
//| | | | (_| | | | | (_| | |  __/ | (_| (_| | | | | | |  __/ | | (_| \__ \
//|_| |_|\__,_|_| |_|\__,_|_|\___|  \___\__,_|_| |_| |_|\___|_|  \__,_|___/

void ofApp::getNextCamera(){
    ofxXmlSettings XML;
    urlTitle.first = "";
    urlTitle.second = "";
    
    
    
    if (XML.loadFile("streams.xml")){
        
        XML.pushTag("streams");
        std::string tag = "stream";
        
        std::size_t nCams = static_cast<std::size_t>(XML.getNumTags(tag));
        if(nCams > 0){
            
            if(currentCamera == -1){
                currentCamera = 0;
            }else if(nCams - 1 > currentCamera){
                currentCamera += 1;
            }else{
                currentCamera = 0;
            }

            urlTitle.first = XML.getAttribute(tag, "url", "", currentCamera);
            urlTitle.second = XML.getAttribute(tag, "name", "", currentCamera);
            cout << "new Camera: " << urlTitle.first << urlTitle.second << endl;
            
            XML.popTag();
        }else{
            ofLog(OF_LOG_ERROR, "Unable to load streams.xml.");
        }
        
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Unable to load streams.xml.");
    }
    

}


void ofApp::loadCameras()
{
    
    getNextCamera();
    IPCameraDef def(urlTitle.first); //newjersey, newark
    stream_name = urlTitle.second;
    ipcam = def;
    
}

void ofApp::loadNextCamera(){
    
    int wi = 620;
    int he = 480;
    num_measurements = 1;
    
    avColors.clear(); //might now even need
    first = true;
    background.allocate(wi, he, OF_IMAGE_COLOR);
    movement.allocate(wi, he, OF_IMAGE_COLOR_ALPHA);
    drawing = false;
    hideMovement = false;
    
    loadCameras();
    IPCameraDef& cam = ipcam;
    
    auto c = std::make_shared<Video::IPVideoGrabber>(); //make_shared ???
    c->setURI(cam.getURL());
    c->connect(); // connect immediately
    
    the_stream = c;
    
}

void ofApp::resetStream(){
    
    currentCamera -= 1;
    loadNextCamera();
    
}
