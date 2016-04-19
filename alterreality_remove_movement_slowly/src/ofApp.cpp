#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE); //what does that mean?
    ofSetFrameRate(30);

    int wi = 620;
    int he = 480;

    background.allocate(wi, he, OF_IMAGE_COLOR);
    movement.allocate(wi, he, OF_IMAGE_COLOR_ALPHA);
    streamImg.allocate(wi, he, OF_IMAGE_COLOR);
    cout << streamImg.getHeight() << endl;
    cout << streamImg.getWidth() << endl;
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
    IPCameraDef def("http://155.33.204.133/mjpg/video.mjpg");
//    IPCameraDef def("http://91.201.117.136/mjpg/video.mjpg");
//    IPCameraDef def("http://68.181.140.38/mjpg/video.mjpg");
//    IPCameraDef def("http://73.43.57.142/mjpg/video.mjpg"); //fencers
//    IPCameraDef def("http://96.92.37.226:84/mjpg/video.mjpg");
//    IPCameraDef def("http://192.133.84.223/mjpg/video.mjpg");
    
    ipcam = def;
    
}


void ofApp::update()
{
    
    the_stream -> update();
    
    
    if(the_stream->isFrameNew()){  //what was the difference between "." and "->" again?
        auto prep_stream_pixels = the_stream->getPixels().getData();
        int prep_stream_w = the_stream->getWidth();
        int prep_stream_h = the_stream->getHeight();
        streamImg.resize(prep_stream_w, prep_stream_h);
        background.resize(prep_stream_w, prep_stream_h);
        movement.resize(prep_stream_w, prep_stream_h);
        
        int prep_stream_i = 0;
        for(int y = 0; y < prep_stream_h; y++){  //row though loops
            for(int x = 0; x < prep_stream_w; x++){     // loop though columns
                ofColor prep_stream_c;
                prep_stream_c.set(prep_stream_pixels[prep_stream_i+0], prep_stream_pixels[prep_stream_i+1], prep_stream_pixels[prep_stream_i+2]);
                streamImg.setColor(x, y, prep_stream_c);
                
                
                float r = prep_stream_pixels[prep_stream_i+0];
                float g = prep_stream_pixels[prep_stream_i+1];
                float b = prep_stream_pixels[prep_stream_i+2];

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
                    if ( colCountPerPixel[prep_stream_i+0].find(r) == colCountPerPixel[prep_stream_i+0].end()) {
                        // not found
                        colCountPerPixel[prep_stream_i+0][r] = 0;
                    }
                    colCountPerPixel[prep_stream_i+0][r] += 1;

                    if ( colCountPerPixel[prep_stream_i+1].find(g) == colCountPerPixel[prep_stream_i+1].end()) {
                        // not found
                        colCountPerPixel[prep_stream_i+1][g] = 0;
                    }
                    colCountPerPixel[prep_stream_i+1][g] += 1;
                    
                    if ( colCountPerPixel[prep_stream_i+2].find(b) == colCountPerPixel[prep_stream_i+2].end()) {
                        // not found
                        colCountPerPixel[prep_stream_i+2][b] = 0;
                    }
                    colCountPerPixel[prep_stream_i+2][b] += 1;
                    
                }
                
                //RED:
                int most_frequent_r = 0;
                int highest_count_r = 0;
                for(auto& ri : colCountPerPixel[prep_stream_i+0]){
                    if (ri.second > highest_count_r) {
                        highest_count_r = ri.second;
                        most_frequent_r = ri.first;
                    }
                }
//                r = most_frequent_r;

                //GREEN:
                int most_frequent_g = 0;
                int highest_count_g = 0;
                for(auto& gi : colCountPerPixel[prep_stream_i+1]){
                    if (gi.second > highest_count_g) {
                        highest_count_g = gi.second;
                        most_frequent_g = gi.first;
                    }
                }
//                g = most_frequent_g;

                //BLUE:
                int most_frequent_b = 0;
                int highest_count_b = 0;
                for(auto& bi : colCountPerPixel[prep_stream_i+2]){
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

                
              


                //DISSECTING MOVEMENT:
                int threshold = 30;

                bool redStill = r > most_frequent_r - threshold && r < most_frequent_r + threshold;
                bool greenStill = g > most_frequent_g - threshold && g < most_frequent_g + threshold;
                bool blueStill = b > most_frequent_b - threshold && b < most_frequent_b + threshold;
                
                if(redStill && greenStill && blueStill){

                    float r_mv = 255;
                    float g_mv = 255;
                    float b_mv = 255;

                    ofColor c_mv;
                    c_mv.set(r_mv, g_mv, b_mv, 0);

                    movement.setColor(x, y, c_mv);
                }else{
                    //JUST MOVEMENT
                    float r_mv = prep_stream_pixels[prep_stream_i+0];
                    float g_mv = prep_stream_pixels[prep_stream_i+1];
                    float b_mv = prep_stream_pixels[prep_stream_i+2];
                    
                    ofColor c_mv;
                    c_mv.set(r_mv, g_mv, b_mv, 255);
                    
                    movement.setColor(x, y, c_mv);
                }

                prep_stream_i+=3;
            }
        }
        first = false;
        streamImg.update();
        background.update();
        movement.update();
    }
    
        
//        int ww = grabbers[0]->getWidth();
//        int hh = grabbers[0]->getHeight();
//        std::cout << ww << std::endl;
//        std::cout << hh << std::endl;
////        std::cout << "---" << std::endl;
//        
//        
//        auto pixels = grabbers[0]->getPixels().getData();  //getData returns a pointer
////        auto& alteredRealityPixels = alteredReality.getPixels();
////        auto& backgroundPixels = alteredReality.getPixels();
//        int i = 0;
//        for(int y = 0; y < hh; y++){  //row though loops
//            for(int x = 0; x < ww; x++){     // loop though columns
//                float r = pixels[i+0];
//                float g = pixels[i+1];
//                float b = pixels[i+2];
//                
//                if(first == true){
//                    std::map<int,long> mapR;
//                    mapR[r] = 1;
//                    colCountPerPixel.push_back(mapR);
//                    
//                    std::map<int,long> mapG;
//                    mapG[g] = 1;
//                    colCountPerPixel.push_back(mapG);
//                    
//                    std::map<int,long> mapB;
//                    mapB[b] = 1;
//                    colCountPerPixel.push_back(mapB);
//                }else{
//                    if ( colCountPerPixel[i+0].find(r) == colCountPerPixel[i+0].end()) {
//                        // not found
//                        colCountPerPixel[i+0][r] = 0;
//                    }
//                    colCountPerPixel[i+0][r] += 1;
//                    
//                    if ( colCountPerPixel[i+1].find(g) == colCountPerPixel[i+1].end()) {
//                        // not found
//                        colCountPerPixel[i+1][g] = 0;
//                    }
//                    colCountPerPixel[i+1][g] += 1;
//                    
//                    if ( colCountPerPixel[i+2].find(b) == colCountPerPixel[i+2].end()) {
//                        // not found
//                        colCountPerPixel[i+2][b] = 0;
//                    }
//                    colCountPerPixel[i+2][b] += 1;
//                    
//                }
//                
//                
//                //RED:
//                int most_frequent_r = 0;
//                int highest_count_r = 0;
//                for(auto& ri : colCountPerPixel[i+0]){
//                    if (ri.second > highest_count_r) {
//                        highest_count_r = ri.second;
//                        most_frequent_r = ri.first;
//                    }
//                }
////                r = most_frequent_r;
//                
//                //GREEN:
//                int most_frequent_g = 0;
//                int highest_count_g = 0;
//                for(auto& gi : colCountPerPixel[i+1]){
//                    if (gi.second > highest_count_g) {
//                        highest_count_g = gi.second;
//                        most_frequent_g = gi.first;
//                    }
//                }
////                g = most_frequent_g;
//                
//                //BLUE:
//                int most_frequent_b = 0;
//                int highest_count_b = 0;
//                for(auto& bi : colCountPerPixel[i+2]){
//                    if (bi.second > highest_count_b) {
//                        highest_count_b = bi.second;
//                        most_frequent_b = bi.first;
//                    }
//                }
////                b = most_frequent_b;
//                
//                // JUST BACKGROUND
//                float r_bg = most_frequent_r;
//                float g_bg = most_frequent_g;
//                float b_bg = most_frequent_b;
//                ofColor c_bg;
//                c_bg.set(r_bg, g_bg, b_bg);
//                
//                background.setColor(x, y, c_bg);
//                
//                //JUST MOVEMENT
//                float r_mv = pixels[i+0];
//                float g_mv = pixels[i+1];
//                float b_mv = pixels[i+2];
//                
//                
//                //DISSECTING MOVEMENT:
//                int threshold = 30;
//                
//                bool redStill = r > most_frequent_r - threshold && r < most_frequent_r + threshold;
//                bool greenStill = g > most_frequent_g - threshold && g < most_frequent_g + threshold;
//                bool blueStill = b > most_frequent_b - threshold && b < most_frequent_b + threshold;
//                
//               
//                
//                if(redStill && greenStill && blueStill){
//                    r = most_frequent_r;
//                    g = most_frequent_g;
//                    b = most_frequent_b;
//                
//                    r_mv = 255;
//                    g_mv = 255;
//                    b_mv = 255;
////                    r = alteredRealityPixels[i+0];
////                    g = alteredRealityPixels[i+1];
////                    b = alteredRealityPixels[i+2];
//                    
//                    
//                    //                    r = avColors[i+0].first;
//                    //                    g = avColors[i+1].first;
//                    //                    b = avColors[i+2].first;
//                    ofColor c_mv;
//                    c_mv.set(r_mv, g_mv, b_mv, 0);
//                    
//                    movement.setColor(x, y, c_mv);
//                }else{
//                    ofColor c_mv;
//                    c_mv.set(r_mv, g_mv, b_mv, 255);
//                    
//                    movement.setColor(x, y, c_mv);
//                }
//
//
//                i+=3; //what about the alpha again?
//                
//                ofColor c;
//                c.set(r, g, b);
//                
//                alteredReality.setColor(x, y, c);
//                
//                
//                //ONLY MOVEMENT
//                
//            }
//        }
//    alteredReality.update();
//    background.update();
//    movement.update();
//    first = false;
//    }
}


void ofApp::draw()
{
    ofBackground(255);
    
    float w = ofGetWidth();
    float h = ofGetHeight();
    
    
    
    float orig_width = streamImg.getWidth();
    float orig_height = streamImg.getHeight();
    float factor = (w/2) / orig_width;
    
//    streamImg.draw(0,0, orig_width * factor, orig_height * factor);
    
    background.draw(0,h/2 - (orig_height * factor)/2, orig_width * factor, orig_height * factor);
    movement.draw(w/2,h/2 - (orig_height * factor)/2, orig_width * factor, orig_height * factor);
//    int w = ofGetWidth();
//    int h = ofGetHeight();
//    grabbers[0]->draw(0,0,w/2,h/2);
//    alteredReality.draw(w/2,h/2);
//    background.draw(w/2,0);
//    background.draw(w/2,h/2);
//    movement.draw(0,h/2);
//    movement.draw(w/2,h/2);
//    int offset = mouseX;
//    the_stream->draw(0,0,w/2,h/2);
    
//    background.draw( w/2 - w/4 - offset, h/2 - h/4 - offset);
//    movement.draw(w/2 - w/4 + offset, h/2 - h/4 + offset);
//    alteredReality.d

//    background.draw( w/2 - w/4, h/2 - h/4 );

//    movement.draw(w/2 - w/4 + offset, h/2 - h/4);
//    movement.draw(w/2 - w/4 - offset, h/2 - h/4);
//    movement.draw(w/2 - w/4 - 2*offset, h/2 - h/4);
    
}



