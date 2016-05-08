#CXX = $(shell fltk-config --cxx)
DEBUG = -g

CXXFLAGS = -std=c++11 $(shell fltk-config --use-gl --use-images --cxxflags ) -I. -I/usr/include/FL -D_GLIBCXX_DEBUG -g
LDFLAGS = $(shell fltk-config --use-gl --use-images --ldflags ) 
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK = $(CXX)

TARGET = main 
OBJS = visualize.o prefix.o tree.o prefix_lex.o main.o 
SRCS = visualize.cpp prefix.cpp tree.cpp prefix_lex.cpp main.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o 
	rm -f $(TARGET) 
