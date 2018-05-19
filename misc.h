#pragma once
#include <string>
#include <opencv2/opencv.hpp>	// C++

#include "yolo_v2_class.hpp"


std::vector<std::string> objects_names_from_file(std::string const filename);
void show_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names);
void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, unsigned int wait_msec = 0, bool dont_show = false);

cv::Mat load_Mat(const std::string& image_filename);

cv::Mat image_t_to_mat(const image_t& img);

struct Data {
	int classes;
	std::string train,
		valid,
		names,
		backup;
};

Data read_data(const char* data_filename);