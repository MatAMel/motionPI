# motionPI

A program like motionEye but faster and built in c++.
Supports:
- Webserver
- 30 fps videos and saving of said images
- config file in json format

### Install
To install use the provided Makefile. It may not be configured to your specific openCV path. You could therefore use the compiled binary in the release section of this GitHub page.
Just run to install:
```
make
```



The webserver used is [this](https://raw.githubusercontent.com/nadjieb/cpp-mjpeg-streamer/master/single_include/nadjieb/mjpeg_streamer.hpp) nice webserver from nadjieb.

The JSON parser used is [this](https://github.com/nlohmann/json) from nlohmann. 