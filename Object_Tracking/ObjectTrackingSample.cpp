//
//  ObjectTrackingSample.cpp
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 26/07/2012.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#include <iostream>
#include "ObjectTrackingClass.h"
#include "ObjectTrackingSample.h"
#include "Globals.h"

ObjectTrackingSample::ObjectTrackingSample(): m_algorithmName("LKT"), m_maxCorners(200){
    
    std::vector<std::string> algos;
    algos.push_back("LKT");
}

//! Sets the reference frame for latter processing
void ObjectTrackingSample::setReferenceFrame(const cv::Mat& reference)
{
    getGray(reference, imagePrev);
    computeObject = true;
}

// Reset object keypoints and descriptors
void ObjectTrackingSample::resetReferenceFrame() const{
    trackObject = false;
    computeObject = false;
}

//! Processes a frame and returns output image
bool ObjectTrackingSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame){
    
    // display the frame
    inputFrame.copyTo(outputFrame);
    
    // convert input frame to gray scale
    getGray(inputFrame, imageNext);
    
    // prepare the tracking class
    ObjectTrackingClass ot;
    ot.setMaxCorners(m_maxCorners);
    
    // begin tracking object
    if ( trackObject ) {
        ot.track(outputFrame,
                 imagePrev,
                 imageNext,
                 pointsPrev,
                 pointsNext,
                 status,
                 err);
        
        // check if the next points array isn't empty
        if ( pointsNext.empty() )
            trackObject = false;
    }
    
    // store the reference frame as the object to track
    if ( computeObject ) {
        ot.init(outputFrame, imagePrev, pointsNext);
        trackObject = true;
        computeObject = false;
    }
    
    // backup previous frame
    imageNext.copyTo(imagePrev);
    
    // backup points array
    std::swap(pointsNext, pointsPrev);
    
    return true;
}
