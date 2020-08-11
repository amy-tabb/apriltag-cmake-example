/*
 * opencv-demo-read.hpp
 *
 *  Created on: Aug 10, 2020
 *      Author: atabb
 */

#ifndef OPENCV_DEMO_READ_HPP_
#define OPENCV_DEMO_READ_HPP_

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <functional>

extern "C" {
#include "apriltag.h"
#include "tag36h11.h"
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"
//#include "common/getopt.h"
}
using namespace std;
using namespace cv;

template<class T>
T FromString(const string& s)
{
    std::istringstream stream (s);
    T t;
    stream >> t;
    return t;
}

template<class T>
string ToString(T arg)
{
    std::ostringstream s;

    s << arg;

    return s.str();

}

void DetectAprilTagsInImages(Mat& frame, Mat& gray, apriltag_detector_t *td);



#endif /* OPENCV_DEMO_READ_HPP_ */
