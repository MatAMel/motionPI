# Define the OpenCV installation root directory
OPENCV_ROOT := /home/pi/opencv

# Automatically find all relevant include directories
INCLUDE_DIRS := $(wildcard $(OPENCV_ROOT)/modules/*/include)

# Add the OpenCV "include" directory
INCLUDE_DIRS += $(OPENCV_ROOT)/include

# Specify the library paths
LIB_DIRS := -L$(OPENCV_ROOT)/build/lib

# Specify the OpenCV library names
LIBS := -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lstdc++fs

# Compile your code with the found include directories and library paths
default:
	g++ ../main/main.cpp $(addprefix -I,$(INCLUDE_DIRS)) $(LIB_DIRS) $(LIBS) -o ../bin/motionPI -std=c++17 -pthread -O3 -Wno-psabi
