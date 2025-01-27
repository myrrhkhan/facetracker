#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>
#include <unistd.h>
#include <vector>

cv::Ptr<cv::VideoCapture> preview_device(int device_ID) {
  cv::Mat frame;
  //--- INITIALIZE VIDEOCAPTURE
  cv::Ptr<cv::VideoCapture> cap = cv::makePtr<cv::VideoCapture>();
  // open the default camera using default API
  // cap.open(0);
  // OR advance usage: select any API backend
  int api_ID = cv::CAP_ANY; // 0 = autodetect default API
  // open selected camera using selected API
  cap->open(device_ID, api_ID);
  // check if we succeeded
  if (!cap->isOpened()) {
    std::cerr << "ERROR! Unable to open camera\n";
    return nullptr;
  }
  //--- GRAB AND WRITE LOOP
  std::cout << "Start grabbing from ID: " << device_ID << std::endl
            << "Press any key to terminate" << std::endl;
  for (;;) {
    // wait for a new frame from camera and store it into 'frame'
    cap->read(frame);
    // check if we succeeded
    if (frame.empty()) {
      std::cerr << "ERROR! blank frame grabbed\n";
      return NULL;
    }
    // show live and wait for a key with timeout long enough to show images
    namedWindow("Live", cv::WINDOW_NORMAL);
    imshow("Live", frame);
    // cv::resizeWindow("Live", 360, 240);
    if (cv::waitKey(5) >= 0)
      break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return cap;
}

std::vector<cv::Ptr<cv::VideoCapture>> get_cameras() {
  int index = 0;
  std::vector<cv::Ptr<cv::VideoCapture>> caps;
  for (int device_ID = 0; device_ID < 10; device_ID++) {
    cv::VideoCapture *cap = preview_device(device_ID);
    if (cap) {
      caps.push_back(cap);
      std::cout << index << std::endl;
      index++;
    }
  }
  for (int i = 0; i < index; i++) {
    std::cout << i << std::endl;
  }

  int device1_idx, device2_idx;
  std::cout << "Choose first camera" << std::endl;
  std::cin >> device1_idx;
  std::cout << "Choose second camera" << std::endl;
  std::cin >> device2_idx;

  std::vector<cv::Ptr<cv::VideoCapture>> final_caps;
  final_caps.push_back(caps[device1_idx]);
  final_caps.push_back(caps[device2_idx]);

  // https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
  return final_caps;
}
