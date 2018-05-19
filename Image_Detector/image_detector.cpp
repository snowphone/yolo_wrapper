#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>

#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#include "../yolo_v2_class.hpp"
#include "../parser.h"
#else
#include "parser.h"
#include "../src/yolo_v2_class.hpp"
#endif

vector<string> read_image_name(const char* image_list)
{
    vector<string> image_names;
    ifstream in(image_list);
    while(in)
    {
        string s;
        getline(in, s);
        image_names.push_back(s);
    }
    return image_names;
}

int main(int argc, const char* argv[])
{
    const char* image_list = find_option(argc, argv, "-image");
    const char* weight_filename = find_option(argc, argv, "-weight");
    const char* data_filename = find_option(argc, argv, "-data");
    const char* cfg_filename = find_option(argc, argv, "-cfg");

    assert(image_list && "invalid text");
    assert(weight_filename && "invalid weight");
    assert(data_filename && "invalid .data");
    assert(cfg_filename && "invalid .cfg");

    vector<string> image_names = read_image_name(image_list);

    Detector detector(cfg_filename, weight_filename);
    
    for(string& image_name : image_names)
    {
        auto img = detector.load_image(image_name);
        auto b_boxes = detector.detect(img);

    }










}
