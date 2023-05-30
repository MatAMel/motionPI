#pragma once
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;


struct Config {
    std::string path;
    double threshold;
    int pixel_treshold;
    bool webstream;
    int port;
    int capture_device;
};

Config parse_config() {
    Config config;
    std::ifstream file("config.json");

    if (!file) {
        std::cerr << "Unable to open file data.json";
        exit(-1);
    }

    // Parse the file into a json object
    json json_object;
    file >> json_object;

    file.close();

    // Access the values
    config.path = json_object["path"];
    config.threshold = json_object["threshold"];
    config.pixel_treshold = json_object["pixel_treshold"];
    config.webstream = json_object["webstream"];
    config.port = json_object["port"];
    config.capture_device = json_object["capture_device"];

    return config;
}