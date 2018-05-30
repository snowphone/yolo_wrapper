#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <dirent.h>

#define OPENCV
#include "opencv2/core/version.hpp"
#include "misc.h"

using std::string;  using std::vector;
using std::cout; 
using std::endl;
using std::ifstream;
using std::runtime_error;

string get_epoch(const string& weight_filename)
{
    string::const_iterator number_begin = weight_filename.begin() +  weight_filename.find_first_of(string("0123456789")),
        number_end = weight_filename.begin() + weight_filename.find_last_of(string("0123456789")) + 1;
    return string(number_begin, number_end);
}

void draw_boxes(cv::Mat mat_img, std::vector<bbox_t>& result_vec, const std::vector<std::string>& obj_names, unsigned int wait_msec , bool dont_show) {
	for (auto &i : result_vec) {
		cv::Scalar color = obj_id_to_color(i.obj_id);
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 3);
		if (obj_names.size() > i.obj_id)
			putText(mat_img, obj_names[i.obj_id], cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
		if (i.track_id > 0)
			putText(mat_img, std::to_string(i.track_id), cv::Point2f(i.x + 5, i.y + 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
	}
	if (!dont_show)
	{
		cv::namedWindow("window", cv::WINDOW_GUI_NORMAL);
		cv::resizeWindow("window", 800, 800);
		cv::imshow("window", mat_img);
		cv::waitKey(wait_msec);
		cv::destroyWindow("window");
	}
}

cv::Mat load_Mat(const std::string & image_filename)
{
	return cv::imread(image_filename);
}

cv::Mat image_t_to_mat(const image_t & img)
{
	cv::Mat mat(img.h, img.w, CV_32FC3, img.data);
	return mat;
}

Data read_data(const char * data_filename)
{
	Data data;
	std::ifstream in(data_filename);
	while (in)
	{
		std::string line;
		std::getline(in, line);
		if (line.empty())
			continue;
		std::string option = line.substr(0, line.find_first_of("=")),
			payload = line.substr(line.find("=") + 1);

		payload = payload.substr(payload.find_first_not_of(" "));
		

		if (option.find("classes") != std::string::npos)
		{
			data.classes = std::stoi(payload);
		}
		else if (option.find("train") != std::string::npos)
		{
			data.train = payload;
		}
		else if (option.find("valid") != std::string::npos)
		{
			data.valid = payload;
		}
		else if (option.find("names") != std::string::npos)
		{
			data.names = payload;
		}
		else if (option.find("backup") != std::string::npos)
		{
			data.backup = payload;
		}
	}
	return data;
}


void show_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names) {
	for (auto &i : result_vec) {
		if (obj_names.size() > i.obj_id) std::cout << obj_names[i.obj_id] << " - ";
		std::cout << "obj_id = " << i.obj_id << ",  x = " << i.x << ", y = " << i.y
			<< ", w = " << i.w << ", h = " << i.h
			<< std::setprecision(3) << ", prob = " << i.prob << std::endl;
	}
}

std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) return file_lines;
	for (std::string line; file >> line;) file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}

vector<string> get_image_list(const char* image_folder)
{
    DIR* directory = opendir(image_folder);
    string folder_name(image_folder);
    struct dirent* file_info;

    if(!directory)
        throw runtime_error("invalid image folder");

    vector<string> ret;
    while( (file_info = readdir(directory)) )
    {
        string name(file_info->d_name);
        if(name.rfind(".jpg") != string::npos)
        {
            name = folder_name + '/' + name;
            cout << "load\t" << name << endl;
            ret.push_back(name);
        }

    }
    closedir(directory);
    return ret;
}


string get_new_name(const string& image_name, const string& weight_filename)
{
    string::const_iterator basename_begin = image_name.begin() + image_name.rfind('/') + 1,
        basename_end = image_name.begin() + image_name.rfind('.');
	string basename(basename_begin, basename_end);
    string extension; 
    if(image_name.find(".jpg") != string::npos)
        extension = ".jpg";
    else
        extension = ".mkv";

	return basename + "_" + get_epoch(weight_filename) + "_" + extension;
}

vector<string> read_image_name(const char* image_list)
{
    vector<string> image_names;
    ifstream in(image_list);
    while(in)
    {
        string s;
        getline(in, s);
		if (s.empty())
			continue;
        image_names.push_back(s);
    }
    return image_names;
}


