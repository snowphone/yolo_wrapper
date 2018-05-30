#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#endif

#include "../misc.h"
#include "../parser.h"
#include "../yolo_v2_class.hpp"



using namespace std;

int main(int argc, const char* argv[])
{
	const char* image_list = find_option(argc, argv, "-image", nullptr);
	const char* weight_filename = find_option(argc, argv, "-weight", nullptr);
	const char* data_filename = find_option(argc, argv, "-data", nullptr);
	const char* cfg_filename = find_option(argc, argv, "-cfg", nullptr);
	bool bStore = (bool)find(argc, argv, "-store", "true");

	if (!image_list || !weight_filename || !data_filename || !cfg_filename)
	{
		throw runtime_error(string("Usage: ") + argv[0] + " -data <data> -cfg <cfg> -weight <weight> -image <image folder>");
	}

    vector<string> image_names = get_image_list(image_list);

	Data data = read_data(data_filename);
    Detector detector(cfg_filename, weight_filename);


	size_t box_counter = 0;
    for(string& image_name : image_names)
    {
        vector<bbox_t> b_boxes = detector.detect(image_name);
		box_counter += b_boxes.size();
		cv::Mat image = load_Mat(image_name);

        draw_boxes(image, b_boxes, objects_names_from_file(data.names), 0, true);
        
		if (bStore)
		{
			string name = get_new_name(image_name, weight_filename);
            cout << name  << ",\t box: " << b_boxes.size() << endl;
			cv::imwrite(name, image);
		}
    }
	cout << box_counter << " boxes detected" << endl;
	cout << "Done!";
}

