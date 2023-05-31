// TODO: Mark each image with the time in the corner
//       Create videos from images with ffmpeg?
//       DONE: Create config file with all configs and not hardcoded in this file
//       DONE Create git-repo
//       DONE: Create webserver to watch livestream

#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "color_output.hpp"
#include "json.hpp"
#include "mjpeg_streamer.hpp" // for the webserver
#include "config.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <csignal>

#define DATE 1
#define TIME 2
#define DATE_AND_TIME 3

typedef cv::Vec3b pixel;
using MJPEGStreamer = nadjieb::MJPEGStreamer;

// global variables
Config config = parse_config();
cv::VideoCapture cap(config.capture_device);
MJPEGStreamer streamer;
std::vector<int> params;

// function prototypes
int compare_images(cv::Mat& image1, cv::Mat& image2, double threshold, int pixel_treshold);
void get_video_stream();
std::string get_date(int date_format);
void save_image(cv::Mat& image, Config& config);
void signal_handler(int signum);


void get_video_stream()
{
    // if SIGTERM signal, go to signal_handler and clean up (systemctl restart ex.)
    signal(SIGTERM, signal_handler);
    
    if(!cap.isOpened())
    {
        std::cerr << "Could not open cv::VideoCapture cap" << std::endl;
        exit(-1);
    }

    cv::Mat frame;
    cv::Mat prev_frame;
    int diff;

    if(config.webstream) // if set in the config file, start the webstream
    {
        params = {cv::IMWRITE_JPEG_QUALITY, 90};
        streamer.start(config.port);
    }

    while(1)
    {
        cap >> frame;
        if(frame.empty())
        {
            std::cerr << "Frame is empty" << std::endl;
            exit(-1);
        }
 
        if(prev_frame.empty())
            prev_frame = frame.clone();
        
        diff = compare_images(frame, prev_frame, config.threshold, config.pixel_treshold);
        if(diff == 1) // images are different
            save_image(frame, config);

        if(config.webstream)
        {
            // http://localhost:PORT/video
            std::vector<uchar> buff_bgr;
            cv::imencode(".jpg", frame, buff_bgr, params);
            streamer.publish("/video", std::string(buff_bgr.begin(), buff_bgr.end()));
        }

        prev_frame = frame.clone();  
    }
    
}


// returns 0 if images are the same, 1 if they are different based on the threshold
int compare_images(cv::Mat& image1, cv::Mat& image2, double threshold, int pixel_treshold)
{
    double diff = 0;
    double total_pixels = image1.rows * image1.cols;
    if ( !(image1.rows == image2.rows && image1.cols == image2.cols) ) // images are not the same size
        return 0;

    for(int i = 0; i < image1.rows; i++)
    {
        pixel* p1 = image1.ptr<pixel>(i);
        pixel* p2 = image2.ptr<pixel>(i);
        for(int j = 0; j < image1.cols; j++)
        {
            if( abs(p1[j][0] - p2[j][0]) > pixel_treshold && abs(p1[j][1] - p2[j][1]) > pixel_treshold && abs(p1[j][2] - p2[j][2]) > pixel_treshold )
                    diff++;
        }
    }
    
    double percent_diff = (diff / total_pixels);
    if(percent_diff > threshold) // images are not the same
        return 1;

    return 0;
}


void save_image(cv::Mat& image, Config& config)
{
    std::string date = get_date(DATE);
    std::string time = get_date(TIME);
    std::string date_and_time = get_date(DATE_AND_TIME);

    std::string base_dir = config.path;
    std::string date_dir = base_dir + date;
    std::string time_dir = date_dir + "/" + time;
    std::string filename = time_dir + "/" + date_and_time + ".jpg";

    // Creating directories if they do not exist
    std::filesystem::create_directories(time_dir);

    // Mark image with date and time in bottom right
    mark_image(image, date_and_time);

    // Save image
    cv::imwrite(filename, image);
}

void mark_image(cv::Mat& image, std::string text)
{
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    cv::Scalar color = cv::Scalar(0, 0, 0); // white color
    int thickness = 2;
    int lineType = cv::LINE_8;
    bool bottomLeftOrigin = false;
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    cv::Point textOrg(image.cols - textSize.width, image.rows - baseline);
    cv::putText(image, text, textOrg, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
}



std::string get_date(int date_format)
{
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto fraction = now - seconds;

    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_r(&in_time_t, &buf);

    std::ostringstream ss;

    if( date_format == DATE )
        ss << std::put_time(&buf, "%Y-%m-%d");
    else if( date_format == TIME )
        ss << std::put_time(&buf, "%H-%M");
    else if ( date_format == DATE_AND_TIME )
    {
        ss << std::put_time(&buf, "%Y-%m-%d-%H-%M-%S");
        ss << '-' << std::setfill('0') << std::setw(3) << fraction.count() * 1000; // adding milliseconds
    }
    
    return ss.str();
}

// stops and releases the resources used by the service
void signal_handler(int signum) 
{
    streamer.stop();
    cap.release();
    exit(-1);
}