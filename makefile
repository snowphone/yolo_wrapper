#과제 형식이 hw[1-9][ab].cpp, *[ab].cpp, *[ab].h 형식일때 자동으로 컴파일

#정규식으로 hw[1-9][ab].cpp 추출 후 확장자 없이 이름만 추출
TARGET = image_detector sports_analyser

#같은 방법으로 *[ab].cpp를 추출해 object 파일로 확장자 치환
#OBJECT = $(patsubst %.cpp, %.o, $(wildcard Image_Detector/*.cpp))  $(patsubst %.cpp, %.o, $(wildcard Sports_Network/*.cpp))  $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJECT = misc.o parser.o 
darknet = ../darknet/darknet.so

#헤더파일
CXXFLAGS = -O2 -W -Wall -std=c++11 
CXX=clang++

LINK = -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_core

#확장자 규칙: 모든 cpp파일을 object 파일로 컴파일
.SUFFIXES : .o .cpp
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install : image_detector sports_analyser




image_detector : Image_Detector/image_detector.o $(OBJECT) 
	$(CXX) $(LINK) -o $@ $^  $(darknet)

sports_analyser : Sports_Network/sports_analyser.o $(OBJECT)
	$(CXX) $(LINK)  -o $@ $^  $(darknet)

clean:
	$(RM) $(TARGET) *.o Image_Detector/*.o

