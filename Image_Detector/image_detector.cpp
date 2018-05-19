#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>

#include "../misc.h"
#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#else
#include <dirent.h>
#endif

#include "../yolo_v2_class.hpp"
#include "../parser.h"

string get_new_name(const string& image_name);
vector<string> read_image_name(const char* image_list);

#ifdef _MSC_VER
#else
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
#endif


int main(int argc, const char* argv[])
{
	const char* image_list = find_option(argc, argv, "-image", nullptr);
	const char* weight_filename = find_option(argc, argv, "-weight", nullptr);
	const char* data_filename = find_option(argc, argv, "-data", nullptr);
	const char* cfg_filename = find_option(argc, argv, "-cfg", nullptr);
	bool bStore = (bool)find(argc, argv, "-store", nullptr);
	bool bDont_show = (bool)find(argc, argv, "-dont_show", nullptr);

	if (!image_list || !weight_filename || !data_filename || !cfg_filename)
	{
		throw runtime_error("invalid input");
	}

#ifdef _MSC_VER 
    vector<string> image_names = read_image_name(image_list);
#else
    vector<string> image_names = get_image_list(image_list);
#endif

	Data data = read_data(data_filename);
    Detector detector(cfg_filename, weight_filename);


	size_t box_counter = 0;
    for(string& image_name : image_names)
    {
        vector<bbox_t> b_boxes = detector.detect(image_name);
		box_counter += b_boxes.size();
		cv::Mat image = load_Mat(image_name);

        draw_boxes(image, b_boxes, objects_names_from_file(data.names), 0, bDont_show);
        
		if (bStore)
		{
			string name = get_new_name(image_name);
            cout << name << endl;
			cv::imwrite(name, image);
		}
    }
	cout << box_counter << " boxes detected" << endl;
	cout << "Done!";
}

string get_new_name(const string& image_name)
{
    string::const_iterator basename_begin = image_name.begin() + image_name.rfind('/') + 1,
        basename_end = image_name.begin() + image_name.rfind('.');
	string basename(basename_begin, basename_end);

	return basename + "_labeled_.jpg";
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


