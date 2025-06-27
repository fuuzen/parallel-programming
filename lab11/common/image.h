#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

inline cv::Mat load_image(const char* image_path) {
  cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
  image.convertTo(image, CV_32FC3);
  cv::normalize(
    image,
    image,
    0,
    1,
    cv::NORM_MINMAX
  );
  return image;
}

inline void save_image(const char* output_filename, cv::Mat output_image) {
  // Make negative values zero.
  cv::threshold(
    output_image,
    output_image,
    /*threshold=*/0,
    /*maxval=*/0,
    cv::THRESH_TOZERO);
  cv::normalize(output_image, output_image, 0.0, 255.0, cv::NORM_MINMAX);
  output_image.convertTo(output_image, CV_8UC3);
  cv::imwrite(output_filename, output_image);
}