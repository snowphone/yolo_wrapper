#과제 형식이 hw[1-9][ab].cpp, *[ab].cpp, *[ab].h 형식일때 자동으로 컴파일

#정규식으로 hw[1-9][ab].cpp 추출 후 확장자 없이 이름만 추출
TARGET = image_detector 

#같은 방법으로 *[ab].cpp를 추출해 object 파일로 확장자 치환
OBJECT = $(patsubst %.cpp, %.o, $(wildcard Image_Detector/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))
darknet = ../darknet/darknet.so

#헤더파일
HEAD = $(wildcard *.h)
CXXFLAGS = -O2 -W -Wall -std=c++11 -lpthread

#확장자 규칙: 모든 cpp파일을 object 파일로 컴파일
.SUFFIXES : .o .cpp
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install : $(TARGET)


$(OBJECT): $(HEAD)

#각 object 파일들을 링크함
$(TARGET) : $(OBJECT)
	$(CXX) -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_core -o $@ $^  $(darknet)



clean:
	$(RM) $(TARGET) *.o Image_Detector/*.o

