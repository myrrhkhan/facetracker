#include "opencv2/stitching.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

int preview_device(int device_ID) {
  cv::Mat frame;
  //--- INITIALIZE VIDEOCAPTURE
  cv::VideoCapture cap;
  // open the default camera using default API
  // cap.open(0);
  // OR advance usage: select any API backend
  int api_ID = cv::CAP_ANY; // 0 = autodetect default API
  // open selected camera using selected API
  cap.open(device_ID, api_ID);
  // check if we succeeded
  if (!cap.isOpened()) {
    std::cerr << "ERROR! Unable to open camera\n";
    return -1;
  }
  //--- GRAB AND WRITE LOOP
  std::cout << "Start grabbing" << std::endl
            << "Press any key to terminate" << std::endl;
  for (;;) {
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) {
      std::cerr << "ERROR! blank frame grabbed\n";
      return -1;
    }
    // show live and wait for a key with timeout long enough to show images
    imshow("Live", frame);
    if (cv::waitKey(5) >= 0)
      break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return device_ID;
}

int main(int, char **) {
  int ids[10];
  int idx;
  for (int device_ID = 0; device_ID < 10; device_ID++) {
    if (preview_device(device_ID) != -1) {
      ids[idx] = device_ID;
      idx++;
    }
  }
  for (int i = 0; i < idx; i++) {
    std::cout << ids[i] << std::endl;
  }

  int device_ID_1, device_ID_2;
  std::cout << "Choose first camera" << std::endl;
  std::cin >> device_ID_1;
  std::cout << "Choose second camera" << std::endl;
  std::cin >> device_ID_2;

  cv::Mat pano;
  cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
  // https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
}
