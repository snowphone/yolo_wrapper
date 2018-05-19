#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#include "../parser.h"
#include "../yolo_v2_class.hpp"
#else
#include "parser.h"
#include "../src/yolo_v2_class.hpp"
#endif
using namespace std;

int main(int argc, const char* argv[])
{
	if(argc < 5)
	{
		std::cerr << "Usage: ./sports_analyser -cfg cfg_file -weight weight_file" << endl;
		return 1;
	}
	const char* cfg_filename = find_option(argc, argv, "-cfg");
	const char* weight_filename = find_option(argc, argv, "-weight");
	const char* thresh_str = find_option(argc, argv, "-thresh");
	float thresh = stof(thresh_str);

	assert(cfg_filename);
	assert(weight_filename);

	Detector detector(cfg_filename, weight_filename);
}
