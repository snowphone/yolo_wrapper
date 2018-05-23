#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "../yolo_cpp_dll.lib")
#endif

#define OPENCV
#define TRACK_OPTFLOW

#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

#include "../yolo_v2_class.hpp"
#include "../misc.h"
#include "../parser.h"

using namespace std;

int main(int argc, const char* argv[])
{

    const char* cfg_filename = find_option(argc, argv, "-cfg");
    const char* weight_filename = find_option(argc, argv, "-weight");
    const char* thresh_str = find_option(argc, argv, "-thresh", "0.2");
    const char* data_filename = find_option(argc, argv, "-data");
    const char* video_filename = find_option(argc, argv, "-video");
    float thresh = stof(thresh_str);

    if(!cfg_filename || !weight_filename || !data_filename || !video_filename)
    {
        cerr << "Usage: " << argv[0] << " -data <.data> -cfg <.cfg> -weight <.weight> -video <video file> (-thresh [0, 1))" << endl;
        return 1;
    }

    Data data = read_data(data_filename);
    vector<string> names = objects_names_from_file(data.names);

    Detector detector(cfg_filename, weight_filename);


    string new_name = get_new_name(video_filename, weight_filename);
    cout << "new name: " << new_name << endl;
    cv::VideoCapture capture;
    cout << video_filename << endl;
    capture.open(video_filename);
    cv::Size size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::Mat frame;
    float fps = capture.get(cv::CAP_PROP_FPS);

    auto codec = CV_FOURCC('M', 'P', '4', 'V');

    cv::VideoWriter writer(new_name , codec, 30, size, true);
    cout << " writer ready" << capture.isOpened() << endl;

    while(capture >> frame, capture.isOpened())
    {
        vector<bbox_t> result_vec = detector.detect(frame, thresh);
        result_vec = detector.tracking_id(result_vec);
        draw_boxes(frame, result_vec, names, 0, true);
        cout << "frame NO. " << result_vec[0].frames_counter << endl;

        writer << frame;
    }
}
