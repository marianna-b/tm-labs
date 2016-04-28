#CXX = $(shell fltk-config --cxx)
DEBUG = -g

CXXFLAGS = -std=c++11 $(shell fltk-config --use-gl --use-images --cxxflags ) -I. -I/usr/include/FL -D_GLIBCXX_DEBUG -g
LDFLAGS = $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK = $(CXX)

TARGET = main 
OBJS = visualize.o parser.o tree.o lexer.o main.o 
SRCS = visualize.cpp parser.cpp tree.cpp lexer.cpp main.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o 
	rm -f $(TARGET) 
