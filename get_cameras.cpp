#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

int main(int, char **) {
  cv::Mat cam_frame1;
  cv::Mat cam_frame2;
  //--- INITIALIZE VIDEOCAPTURE
  cv::VideoCapture cap1;
  cv::VideoCapture cap2;
  // open the default camera using default API
  // cap.open(0);
  // OR advance usage: select any API backend
  int cam1ID = 1; // 0 = open default camera
  int cam2ID = 2;
  int apiID = cv::CAP_ANY; // 0 = autodetect default API
  // open selected camera using selected API
  cap1.open(cam1ID, apiID);
  cap2.open(cam2ID, apiID);
  // check if we succeeded
  if (!cap1.isOpened() || !cap2.isOpened()) {
    std::cerr << "ERROR! Unable to open camera\n";
    return -1;
  }
  //--- GRAB AND WRITE LOOP
  std::cout << "Start grabbing" << std::endl
            << "Press any key to terminate" << std::endl;
  for (;;) {
    // wait for a new frame from camera and store it into 'frame'
    cap1.read(cam_frame1);
    cap2.read(cam_frame2);
    // check if we succeeded
    if (cam_frame1.empty() || cam_frame2.empty()) {
      std::cerr << "ERROR! blank frame grabbed\n";
      break;
    }
    // show live and wait for a key with timeout long enough to show images
    imshow("Camera 1", cam_frame1);
    imshow("Camera 2", cam_frame2);
    if (cv::waitKey(5) >= 0)
      break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
