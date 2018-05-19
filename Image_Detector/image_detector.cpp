#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>

#include "../misc.h"
#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#include "../yolo_v2_class.hpp"
#include "../parser.h"
#else
#include "parser.h"
#include "../src/yolo_v2_class.hpp"
#endif

string new_name(const string& image_name, const string& weight);
vector<string> read_image_name(const char* image_list);


int main(int argc, const char* argv[])
{
	const char* image_list = find_option(argc, argv, "-image", nullptr);
	const char* weight_filename = find_option(argc, argv, "-weight", nullptr);
	const char* data_filename = find_option(argc, argv, "-data", nullptr);
	const char* cfg_filename = find_option(argc, argv, "-cfg", nullptr);
	bool bStore = (bool)find(argc, argv, "-store", nullptr);

	if (!image_list || !weight_filename || !data_filename || !cfg_filename)
	{
		throw exception("invalid input");
	}

    vector<string> image_names = read_image_name(image_list);
	Data data = read_data(data_filename);

    Detector detector(cfg_filename, weight_filename);


	size_t box_counter = 0;
    for(string& image_name : image_names)
    {
        vector<bbox_t> b_boxes = detector.detect(image_name);
		box_counter += b_boxes.size();
		cv::Mat image = load_Mat(image_name);
		draw_boxes(image, b_boxes, objects_names_from_file(data.names));
		if (bStore)
		{
			string name = new_name(image_name, weight_filename);
			cv::imwrite(name, image);
		}
    }
	cout << box_counter << " boxes detected" << endl;
	cout << "Done!";
}

string new_name(const string& image_name, const string& weight)
{
	int index = image_name.rfind('.');
	string new_name = image_name.substr(0, index),
		ext = new_name.substr(index);

	return new_name + "_labeled_" + ext;
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


