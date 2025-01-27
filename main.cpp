#include "get_cameras.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/videoio.hpp>
#include <unistd.h>
#include <vector>

int API_ID = cv::CAP_ANY;

int main() {
  // global vars:
  // get camera IDs
  std::vector<cv::Ptr<cv::VideoCapture>> captures_ptrs = get_cameras();

  // frames to read into
  std::vector<cv::Mat> frames;

  for (int i = 0; i < captures_ptrs.size(); i++) {
    // add to frames array

    // step 1: initialize frame
    cv::Mat frame;

    // step 2: add to above
    frames.push_back(frame);
    // step 3: profit
  }

  // loop
  for (;;) {
    // step 1: read into frames
    for (int i = 0; i < captures_ptrs.size(); i++) {
      cv::VideoCapture cap = *captures_ptrs[i];
      std::cout << "segfault accessing cap?" << std::endl;
      std::cout << frames[i].size << std::endl;
      captures_ptrs[i]->read(frames[i]);
      std::cout << "segfault accessing frame?" << std::endl;
      if (frames[i].empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
        return -1;
      }
      std::cout << "displaying" << std::endl;
      for (;;) {
        namedWindow("Live", cv::WINDOW_NORMAL);
        imshow("Live", frames[i]);
        // cv::resizeWindow("Live", 360, 240);

        if (cv::waitKey(5) >= 0)
          break;
      }
    }
    // step 2: stitch together
    cv::Mat pano;
    cv::Ptr<cv::Stitcher> stitcher =
        cv::Stitcher::create(cv::Stitcher::PANORAMA); // TODO why Ptr?
    cv::Stitcher::Status status = stitcher->stitch(frames, pano);
    std::cerr << "frames, pano" << std::endl;

    if (status != cv::Stitcher::OK) {
      std::cout << "Can't stitch images, error code = " << int(status)
                << std::endl;
      return EXIT_FAILURE;
    }
    // step 3: show
    imshow("Live", pano);
    if (cv::waitKey(5) >= 0)
      break;
  }
  return 0;
}
