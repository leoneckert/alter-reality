#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE); //what does that mean?
    ofSetFrameRate(30);
    alteredReality.allocate(640, 480, OF_IMAGE_COLOR);
    
//    //IF MOVEMENT DRAWING:
    auto& alteredRealityPixels = alteredReality.getPixels();
    ofColor co;
    co.set(255, 255, 255);
    for(int y = 0; y < 480; y++){  //row though loops
        for(int x = 0; x < 640; x++){
            alteredReality.setColor(x, y, co);
        }
    }
    
    
    loadCameras();
    
    IPCameraDef& cam = ipcams[0];
    
    auto c = std::make_shared<Video::IPVideoGrabber>(); //make_shared ???
    

    c->setURI(cam.getURL());
    c->connect(); // connect immediately


    grabbers.push_back(c);

}

void ofApp::loadCameras()
{
    
    
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
        std::cout << ww << std::endl;
        std::cout << hh << std::endl;
        std::cout << "---" << std::endl;
        
        
        auto pixels = grabbers[0]->getPixels().getData();  //getData returns a pointer
        auto& alteredRealityPixels = alteredReality.getPixels();
        int i = 0;
        for(int y = 0; y < hh; y++){  //row though loops
            for(int x = 0; x < ww; x++){     // loop though columns
                float r = pixels[i+0];
                float g = pixels[i+1];
                float b = pixels[i+2];
                
                if(first == true){
                    avColors.push_back(std::make_pair(r,1));
                    avColors.push_back(std::make_pair(g,1));
                    avColors.push_back(std::make_pair(b,1));
                }else{
                    std::pair <float, long> OLDp1 = avColors[i+0];
                    //                        if (i == 0) {
                    //                            std::cout << OLDp1.first << " " << OLDp1.second << std::endl;
                    //                        }
                    std::pair <float,long> NEWp1 (((OLDp1.first * OLDp1.second)+ r)/(OLDp1.second+1) , OLDp1.second + 1);
                    avColors[i+0] = NEWp1;
                    
                    std::pair <float,long> OLDp2 = avColors[i+1];
                    std::pair <float,long> NEWp2 (((OLDp2.first * OLDp2.second) + g)/(OLDp2.second+1),OLDp2.second + 1);
                    avColors[i+1] = NEWp2;
                    
                    std::pair <float,long> OLDp3 = avColors[i+2];
                    std::pair <float,long> NEWp3 (((OLDp3.first * OLDp3.second) + b)/(OLDp3.second+1),OLDp3.second + 1);
                    avColors[i+2] = NEWp3;
                    
                }
                
                
                //BASIC "GHOST CITY":
//                r = avColors[i+0].first;
//                g = avColors[i+1].first;
//                b = avColors[i+2].first;
                
                //DISSECTING MOVEMENT:
                int threshold = 90;

                bool redStill = r > avColors[i+0].first - threshold && r < avColors[i+0].first + threshold;
                bool greenStill = g > avColors[i+1].first - threshold && g < avColors[i+1].first + threshold;
                bool blueStill = b > avColors[i+2].first - threshold && b < avColors[i+2].first + threshold;
                
                if(redStill && greenStill && blueStill){
                    
                    r = alteredRealityPixels[i+0];
                    g = alteredRealityPixels[i+1];
                    b = alteredRealityPixels[i+2];
                    
                    
//                    r = avColors[i+0].first;
//                    g = avColors[i+1].first;
//                    b = avColors[i+2].first;
                }
                
//                r = 255;
//                g = 255;
//                b = 255;

                i+=3; //what about the alpha again?
                
                ofColor c;
                c.set(r, g, b);
                alteredReality.setColor(x, y, c);
            }
        }
    alteredReality.update();
    first = false;
    }
}


void ofApp::draw()
{
    ofBackground(0,0,0);
    int w = ofGetWidth();
    int h = ofGetHeight();
    grabbers[0]->draw(0,0,w,h/2);
    alteredReality.draw(0,h/2);
    
}



