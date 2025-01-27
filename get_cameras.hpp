#pragma once
#include <opencv2/videoio.hpp>
#include <vector>

std::vector<cv::Ptr<cv::VideoCapture>> get_cameras();
