default:
	g++ diff_image.cpp -I /usr/local/include/opencv4/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc -std=c++17 -lstdc++fs -o diff_detector -pthread -O3 -Wno-psabi