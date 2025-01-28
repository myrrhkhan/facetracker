#pragma once
#include <opencv2/videoio.hpp>
#include <vector>

class CamStitcher {
public:
  CamStitcher();
  void run();

private:
  void preview_device(int device_ID);
  std::vector<cv::Ptr<cv::VideoCapture>> m_caps;
  int m_API_ID = cv::CAP_ANY;
  std::vector<cv::Mat> m_frames;
};
