#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <vector>
#include <thread>

// global variables
cv::VideoCapture inputVideo;
bool isTerminated = false;

// main process thread
void arucoThread() {
  // Prepare 6x6 aruco dictionary
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  std::cout << "---------------------------------------" << std::endl;
  std::cout << "marker detection ready." << std::endl;
  std::cout << "input 'exit' to terminate this program." << std::endl;
  std::cout << "---------------------------------------" << std::endl;

  // main loop
  while (inputVideo.grab() && !isTerminated) {
    cv::Mat originalImage;
    inputVideo.retrieve(originalImage);

    // marker detection
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;
    cv::aruco::detectMarkers(originalImage, dictionary, corners, ids);

    // if markers were detected
    if (ids.size() > 0) {
      // show ids in ascending order
      std::sort(ids.begin(), ids.end());
      std::cout << "detected ids: ";
      for (auto &&id : ids) {
        std::cout << id << " ";
      }
      std::cout << std::endl;
    }
  }
  return;
}

// key waiting thread
void keyThread() {
  std::string input;
  while (!isTerminated) {
    std::cin >> input;
    if (input == "exit") {
      isTerminated = true;
    }
  }
  return;
}

int main() {
  // Open camera device
  inputVideo.open(0);
  if (!inputVideo.isOpened()) {
    std::cout << "Error: Failed to open input device" << std::endl;
    return -1;
  }

  // create threads (for terminating program by input "exit")
  std::thread t1(arucoThread);
  std::thread t2(keyThread);
  t1.join();
  t2.join();

  return 0;
}
