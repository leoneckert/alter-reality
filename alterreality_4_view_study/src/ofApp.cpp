#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE); //what does that mean?
    ofSetFrameRate(30);
//    int wi = 800;
//    int he = 600;
    int wi = 620;
    int he = 480;
    alteredReality.allocate(wi, he, OF_IMAGE_COLOR);
    background.allocate(wi, he, OF_IMAGE_COLOR);
    movement.allocate(wi, he, OF_IMAGE_COLOR_ALPHA);
    
        
    loadCameras();
    
    IPCameraDef& cam = ipcams[0];
    
    auto c = std::make_shared<Video::IPVideoGrabber>(); //make_shared ???
    

    c->setURI(cam.getURL());
    c->connect(); // connect immediately


    grabbers.push_back(c);

}

void ofApp::loadCameras()
{
    
//    size of windmill: 800x600
//    IPCameraDef def("http://107.1.228.34/axis-cgi/mjpg/video.cgi");
    IPCameraDef def("http://155.33.204.133/mjpg/video.mjpg");
//    IPCameraDef def("http://91.201.117.136/mjpg/video.mjpg");
//    http://209.194.208.53/mjpg/video.mjpg

    ipcams.push_back(def);
    
    nextCamera = ipcams.size();
}


void ofApp::update()
{
    grabbers[0]->update();

    if(grabbers[0]->isFrameNew()){  //what was the difference between "." and "->" again?
        
        int ww = grabbers[0]->getWidth();
        int hh = grabbers[0]->getHeight();
//        std::cout << ww << std::endl;
//        std::cout << hh << std::endl;
//        std::cout << "---" << std::endl;
        
        
        auto pixels = grabbers[0]->getPixels().getData();  //getData returns a pointer
//        auto& alteredRealityPixels = alteredReality.getPixels();
//        auto& backgroundPixels = alteredReality.getPixels();
        int i = 0;
        for(int y = 0; y < hh; y++){  //row though loops
            for(int x = 0; x < ww; x++){     // loop though columns
                float r = pixels[i+0];
                float g = pixels[i+1];
                float b = pixels[i+2];
                
                if(first == true){
                    std::map<int,long> mapR;
                    mapR[r] = 1;
                    colCountPerPixel.push_back(mapR);
                    
                    std::map<int,long> mapG;
                    mapG[g] = 1;
                    colCountPerPixel.push_back(mapG);
                    
                    std::map<int,long> mapB;
                    mapB[b] = 1;
                    colCountPerPixel.push_back(mapB);
                }else{
                    if ( colCountPerPixel[i+0].find(r) == colCountPerPixel[i+0].end()) {
                        // not found
                        colCountPerPixel[i+0][r] = 0;
                    }
                    colCountPerPixel[i+0][r] += 1;
                    
                    if ( colCountPerPixel[i+1].find(g) == colCountPerPixel[i+1].end()) {
                        // not found
                        colCountPerPixel[i+1][g] = 0;
                    }
                    colCountPerPixel[i+1][g] += 1;
                    
                    if ( colCountPerPixel[i+2].find(b) == colCountPerPixel[i+2].end()) {
                        // not found
                        colCountPerPixel[i+2][b] = 0;
                    }
                    colCountPerPixel[i+2][b] += 1;
                    
                }
                
                
                //RED:
                int most_frequent_r = 0;
                int highest_count_r = 0;
                for(auto& ri : colCountPerPixel[i+0]){
                    if (ri.second > highest_count_r) {
                        highest_count_r = ri.second;
                        most_frequent_r = ri.first;
                    }
                }
//                r = most_frequent_r;
                
                //GREEN:
                int most_frequent_g = 0;
                int highest_count_g = 0;
                for(auto& gi : colCountPerPixel[i+1]){
                    if (gi.second > highest_count_g) {
                        highest_count_g = gi.second;
                        most_frequent_g = gi.first;
                    }
                }
//                g = most_frequent_g;
                
                //BLUE:
                int most_frequent_b = 0;
                int highest_count_b = 0;
                for(auto& bi : colCountPerPixel[i+2]){
                    if (bi.second > highest_count_b) {
                        highest_count_b = bi.second;
                        most_frequent_b = bi.first;
                    }
                }
//                b = most_frequent_b;
                
                // JUST BACKGROUND
                float r_bg = most_frequent_r;
                float g_bg = most_frequent_g;
                float b_bg = most_frequent_b;
                ofColor c_bg;
                c_bg.set(r_bg, g_bg, b_bg);
                
                background.setColor(x, y, c_bg);
                
                //JUST MOVEMENT
                float r_mv = pixels[i+0];
                float g_mv = pixels[i+1];
                float b_mv = pixels[i+2];
                
                
                //DISSECTING MOVEMENT:
                int threshold = 30;
                
                bool redStill = r > most_frequent_r - threshold && r < most_frequent_r + threshold;
                bool greenStill = g > most_frequent_g - threshold && g < most_frequent_g + threshold;
                bool blueStill = b > most_frequent_b - threshold && b < most_frequent_b + threshold;
                
               
                
                if(redStill && greenStill && blueStill){
                    r = most_frequent_r;
                    g = most_frequent_g;
                    b = most_frequent_b;
                
                    r_mv = 255;
                    g_mv = 255;
                    b_mv = 255;
//                    r = alteredRealityPixels[i+0];
//                    g = alteredRealityPixels[i+1];
//                    b = alteredRealityPixels[i+2];
                    
                    
                    //                    r = avColors[i+0].first;
                    //                    g = avColors[i+1].first;
                    //                    b = avColors[i+2].first;
                    ofColor c_mv;
                    c_mv.set(r_mv, g_mv, b_mv, 0);
                    
                    movement.setColor(x, y, c_mv);
                }else{
                    ofColor c_mv;
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    
                    movement.setColor(x, y, c_mv);
                }


                i+=3; //what about the alpha again?
                
                ofColor c;
                c.set(r, g, b);
                
                alteredReality.setColor(x, y, c);
                
                
                //ONLY MOVEMENT
                
            }
        }
    alteredReality.update();
    background.update();
    movement.update();
    first = false;
    }
}


void ofApp::draw()
{
    ofBackground(255);
    int w = ofGetWidth();
    int h = ofGetHeight();
    grabbers[0]->draw(0,0,w/2,h/2);
//    alteredReality.draw(w/2,h/2);
//    background.draw(w/2,0);
//    background.draw(w/2,h/2);
//    movement.draw(0,h/2);
//    movement.draw(w/2,h/2);
//    int offset = 60;
//    background.draw( w/2 - w/4 - offset, h/2 - h/4 - offset);
//    movement.draw(w/2 - w/4 + offset, h/2 - h/4 + offset);
//    alteredReality.d
  
    
}



