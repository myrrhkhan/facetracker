#include "get_cameras.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>
#include <unistd.h>
#include <vector>

void CamStitcher::preview_device(int device_ID) {
  cv::Mat frame;
  //--- INITIALIZE VIDEOCAPTURE
  cv::Ptr<cv::VideoCapture> cap = cv::makePtr<cv::VideoCapture>();
  // open selected camera using selected API
  cap->open(device_ID, m_API_ID);
  // check if we succeeded
  if (!cap->isOpened()) {
    std::cerr << "ERROR! Unable to open camera\n";
    return;
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
      return;
    }
    // show live and wait for a key with timeout long enough to show images
    namedWindow("Live", cv::WINDOW_NORMAL);
    imshow("Live", frame);
    // cv::resizeWindow("Live", 360, 240);
    if (cv::waitKey(5) >= 0)
      break;
  }
  m_caps.push_back(cap);
}

CamStitcher::CamStitcher() {
  for (int device_ID = 0; device_ID < 10; device_ID++) {
    preview_device(device_ID);
  }
  for (int i = 0; i < m_caps.size(); i++) {
    std::cout << i << std::endl;
  }

  int device1_idx, device2_idx;
  std::cout << "Choose first camera" << std::endl;
  std::cin >> device1_idx;
  std::cout << "Choose second camera" << std::endl;
  std::cin >> device2_idx;

  std::vector<cv::Ptr<cv::VideoCapture>> final_caps;
  final_caps.push_back(m_caps[device1_idx]);
  final_caps.push_back(m_caps[device2_idx]);

  m_caps = final_caps;

  // initialize frames:
  for (int i = 0; i < m_caps.size(); i++) {
    cv::Mat frame;
    m_frames.push_back(frame);
  }

  // https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
}

void CamStitcher::run() {
  for (;;) {
    // step 1: get frame
    for (int i = 0; i < m_caps.size(); i++) {
      m_caps[i]->read(m_frames[i]);
      if (m_frames[i].empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
        return;
      }
      // for (;;) {
      //   namedWindow("Live", cv::WINDOW_NORMAL);
      //   imshow("Live", m_frames[i]);
      //   // cv::resizeWindow("Live", 360, 240);
      //
      //   if (cv::waitKey(5) >= 0)
      //     break;
      // }
    }
    // step 2: stitch together
    cv::Mat pano;
    cv::Ptr<cv::Stitcher> stitcher =
        cv::Stitcher::create(cv::Stitcher::PANORAMA); // TODO why Ptr?
    cv::Stitcher::Status status = stitcher->stitch(m_frames, pano);
    std::cerr << "frames, pano" << std::endl;

    if (status != cv::Stitcher::OK) {
      std::cout << "Can't stitch images, error code = " << int(status)
                << std::endl;
      imshow("Cam 1", m_frames[0]);
      imshow("Cam 2", m_frames[1]);
      if (cv::waitKey(5) >= 0)
        continue;
      break;
    }
    // step 3: show
    imshow("Live", pano);
    if (cv::waitKey(5) >= 0)
      break;
  }
}

int main() {
  CamStitcher stitcher = CamStitcher();
  stitcher.run();
}
