#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE); //what does that mean?
    ofSetFrameRate(30);

    int wi = 620;
    int he = 480;

    background.allocate(wi, he, OF_IMAGE_COLOR);
    movement.allocate(wi, he, OF_IMAGE_COLOR_ALPHA);
//    streamImg.allocate(wi, he, OF_IMAGE_COLOR);
//    cout << streamImg.getHeight() << endl;
//    cout << streamImg.getWidth() << endl;
    cout << "---" << endl;
    
    
    loadCameras();
    
    IPCameraDef& cam = ipcam;
    
    auto c = std::make_shared<Video::IPVideoGrabber>(); //make_shared ???
    c->setURI(cam.getURL());
    c->connect(); // connect immediately

    the_stream = c;
    
 

}

void ofApp::loadCameras()
{
    
//    IPCameraDef def("http://107.1.228.34/axis-cgi/mjpg/video.cgi");
//    IPCameraDef def("http://155.33.204.133/mjpg/video.mjpg");  //BOSTON
//        stream_name = "Boston";
//    IPCameraDef def("http://91.201.117.136/mjpg/video.mjpg"); //RUSSIA
//    IPCameraDef def("http://68.181.140.38/mjpg/video.mjpg"); //LOS ANGELES
    
//    IPCameraDef def("http://73.43.57.142/mjpg/video.mjpg"); //fencers  /
//    stream_name = "Nellya Fencers";
//    
//    IPCameraDef def("http://96.92.37.226:84/mjpg/video.mjpg"); // ICE HOCKEY
//    stream_name = "Ice Hockey";
 
//    IPCameraDef def("http://69.63.114.69/mjpg/video.mjpg");  //RUNNING TRACK
//    stream_name = "Council Bluffs, Iowa";
    
    
// // ***
//    IPCameraDef def("http://192.133.84.223/mjpg/video.mjpg"); //TENNIS /
//    stream_name = "Cedar Rapids, Iowa";
    
//    IPCameraDef def("http://93.91.44.169:81/mjpg/video.mjpg"); //LONODN
    
//    IPCameraDef def("http://212.13.111.186/mjpg/video.mjpg"); //colored houses moscov, russio
//    stream_name = "Moscow, Russia";
//    
    
//    IPCameraDef def("http://83.234.97.117/mjpg/video.mjpg"); //fancy building
//    stream_name = "Moscow, Russia";
    
//    IPCameraDef def("http://195.113.240.144:8081/mjpg/video.mjpg");
//    stream_name = "Jihlava, Czech Republic";
    
//    IPCameraDef def(http://195.225.70.249/mjpg/video.mjpg");
//    stream_name = "Lodz, Poland";
    
    
//    IPCameraDef def("http://193.213.88.123/mjpg/video.mjpg");
//    stream_name = "Oslo, Norway";
    
//    IPCameraDef def("http://188.75.140.14/mjpg/video.mjpg");
//    stream_name = "Kolin, Czech Republic";
    
//    IPCameraDef def("http://149.156.124.49/mjpg/video.mjpg");
//    stream_name = "Krakow, Poland";
    
//    IPCameraDef def("http://195.55.223.100/mjpg/video.mjpg"); // GIJON, SPAIN
 
//    IPCameraDef def("http://174.78.247.218/mjpg/video.mjpg"); //SURFER
//    stream_name = "San Diego, California";
  
    
    
  // ***
//    IPCameraDef def("http://134.114.122.215/mjpg/video.mjpg"); //Flagstaff, Arizona
//    stream_name = "Flagstaff, Arizona";
////
////    
    
    
 // ***
//    IPCameraDef def("http://166.248.11.128/mjpg/video.mjpg"); // bolton
//    stream_name = "Bolton, Connecticut";

    
    IPCameraDef def("http://131.193.91.50/mjpg/video.mjpg"); //CHICAGO
    stream_name = "Chicago, Illinois";

//    IPCameraDef def("http://166.140.227.198/mjpg/video.mjpg"); //newjersey, newark
//    stream_name = "Newark,  New Jersey";
//    
//    IPCameraDef def("http://75.144.167.174/mjpg/video.mjpg");//north dakota new england
//    stream_name = "North Dakota, New England";
//    
//    IPCameraDef def("http://166.154.192.220/mjpg/video.mjpg"); //nmew jersey, newark
//    stream_name = "Newark,  New Jersey";
    
//    IPCameraDef def("http://207.140.49.147/mjpg/video.mjpg"); //new jersey, middletown
//    stream_name = "Middletown, New Jersey";
    
//    IPCameraDef def("http://166.141.41.145/mjpg/video.mjpg"); //torrance california
//    stream_name = "Torrance, California";
    
//    IPCameraDef def("http://166.248.6.133/mjpg/video.mjpg"); //east hartford connecticut
//    stream_name = "East Hartford, Connecticut";
//    
//    IPCameraDef def("http://166.241.180.138/mjpg/video.mjpg");//walnut creek california
//    stream_name = "Walnut Creek, California";
    
//    
//    IPCameraDef def("http://61.60.195.28/mjpg/video.mjpg");
//    stream_name = "Taipei City, Taiwan";
//    
    
    ipcam = def;
    
}


void ofApp::update()
{
    
    the_stream -> update();
    
    
    if(the_stream->isFrameNew()){  //what was the difference between "." and "->" again?
        
        int prep_stream_w = the_stream->getWidth();
        int prep_stream_h = the_stream->getHeight();
        
//        streamImg.resize(prep_stream_w, prep_stream_h);
        background.resize(prep_stream_w, prep_stream_h);
        movement.resize(prep_stream_w, prep_stream_h);
        
        auto prep_stream_pixels = the_stream->getPixels().getData();
        int prep_stream_i = 0;
        
        
//         auto& alteredRealityPixels = alteredReality.getPixels();
        
        for(int y = 0; y < prep_stream_h; y++){  //row though loops
            for(int x = 0; x < prep_stream_w; x++){     // loop though columns
//                ofColor prep_stream_c;
//                prep_stream_c.set(prep_stream_pixels[prep_stream_i+0], prep_stream_pixels[prep_stream_i+1], prep_stream_pixels[prep_stream_i+2]);
//                streamImg.setColor(x, y, prep_stream_c);
                
                
                float r = prep_stream_pixels[prep_stream_i+0];
                float g = prep_stream_pixels[prep_stream_i+1];
                float b = prep_stream_pixels[prep_stream_i+2];

                if(first == true){
                    avColors.push_back(std::make_pair(r,1));
                    avColors.push_back(std::make_pair(g,1));
                    avColors.push_back(std::make_pair(b,1));
                }else{
                    std::pair <float, long> OLDp1 = avColors[prep_stream_i+0];
                    //                        if (i == 0) {
                    //                            std::cout << OLDp1.first << " " << OLDp1.second << std::endl;
                    //                        }
                    std::pair <float,long> NEWp1 (((OLDp1.first * OLDp1.second)+ r)/(OLDp1.second+1) , OLDp1.second + 1);
                    avColors[prep_stream_i+0] = NEWp1;
                    
                    std::pair <float,long> OLDp2 = avColors[prep_stream_i+1];
                    std::pair <float,long> NEWp2 (((OLDp2.first * OLDp2.second) + g)/(OLDp2.second+1),OLDp2.second + 1);
                    avColors[prep_stream_i+1] = NEWp2;
                    
                    std::pair <float,long> OLDp3 = avColors[prep_stream_i+2];
                    std::pair <float,long> NEWp3 (((OLDp3.first * OLDp3.second) + b)/(OLDp3.second+1),OLDp3.second + 1);
                    avColors[prep_stream_i+2] = NEWp3;
                    
                }
                
                

                // JUST BACKGROUND
                float r_bg = avColors[prep_stream_i+0].first;
                float g_bg = avColors[prep_stream_i+1].first;
                float b_bg = avColors[prep_stream_i+2].first;
                ofColor c_bg;
                c_bg.set(r_bg, g_bg, b_bg);
                background.setColor(x, y, c_bg);
                
                //DISSECTING MOVEMENT:
                
                
                bool r_moved = r > avColors[prep_stream_i+0].first + threshold || r < avColors[prep_stream_i+0].first - threshold;
                bool g_moved = g > avColors[prep_stream_i+1].first + threshold || g < avColors[prep_stream_i+1].first - threshold;
                bool b_moved = b > avColors[prep_stream_i+2].first + threshold || b < avColors[prep_stream_i+2].first - threshold;
                
                
                float r_mv = r;
                float g_mv = g;
                float b_mv = b;
                ofColor c_mv;

                if(r_moved && g_moved && b_moved && !drawing){
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    movement.setColor(x, y, c_mv);
                }else if(!drawing){
                    c_mv.set(r_mv, g_mv, b_mv, 0);
                    movement.setColor(x, y, c_mv);
                }
                
                if(drawing && first){
                    r_mv = 255;
                    g_mv = 255;
                    b_mv = 255;
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    movement.setColor(x, y, c_mv);
                }
                
                if(r_moved && g_moved && b_moved && drawing){
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    movement.setColor(x, y, c_mv);
                }
                

                
                


                prep_stream_i+=3;
            }
        }
        first = false;
//        streamImg.update();
        background.update();
        movement.update();
    }

    
}


void ofApp::draw()
{
    ofBackground(255);
    
    
    
    float w = ofGetWidth();
    float h = ofGetHeight();
    
//    float orig_width = streamImg.getWidth();
//    float orig_height = streamImg.getHeight();
    
    float orig_width = background.getWidth();
    float orig_height = background.getHeight();
    float factor = (w/1.1) / orig_width;
    
//    streamImg.draw(0,0, orig_width * factor, orig_height * factor);

    if(!drawing || showBackground){
        background.draw(w/2 - (orig_width * factor)/2, h/2 - (orig_height * factor)/2, orig_width * factor, orig_height * factor);
    }
    if(showMovement || drawing){
        movement.draw(w/2 - (orig_width * factor)/2,h/2 - (orig_height * factor)/2  - offset, orig_width * factor, orig_height * factor);

    }
//    ofDrawBitmapString("hello", w/2 - (orig_width * factor)/2, h/2 + (orig_height * factor)/2 + 10) ;
    ofPushStyle();
    ofSetColor(0, 0, 0);
    string str = "LIVE: " + stream_name;
    ofDrawBitmapString(str, w/2 - (orig_width * factor)/2 , h/2 + (orig_height * factor)/2 + 20);
    ofDrawBitmapString(threshold, w/2 - (orig_width * factor)/2 , h/2 + (orig_height * factor)/2 + 35);
    ofPopStyle();


}

void ofApp::keyPressed(int key){
    if (key == ' '){
        showMovement = true;
    }
    if (key == 'u'){
        offset += 6;
    }
    if (key == 'd'){
        offset -= 6;
    }
    if (key == 'r'){
        offset = 0;
    }
    
    if (key =='s' && drawing && !showBackground) {
        showBackground = true;
    }else if (key =='s' && drawing && showBackground) {
        showBackground = false;
    }
    
    if (key == 'm' && drawing){
        drawing = false;
    }else if (key == 'm'){
        drawing = true;
    }
    
    if(key == OF_KEY_DOWN){
        threshold -= 2;
    }
    if(key == OF_KEY_UP){
       threshold += 2;
    }
    
}

void ofApp::keyReleased(int key){

    if (key == ' '){
        showMovement = false;
        offset = 0;
    }
}
