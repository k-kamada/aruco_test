main: main.o
	clang++ -o $@ $< -Wl, -rpath /usr/local/lib/ -lopencv_core -lopencv_imgcodecs -lopencv_aruco -lopencv_videoio -lopencv_highgui -pthread

.cpp.o:
	clang++ -o $@ -c $< -std=c++14 -Wall -I/usr/local/include/opencv4

clean:
	-rm main.o main
