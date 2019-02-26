CXX = g++
CXXFLAGS = -O2 -g -Wall
OBJS = MdSpi.o \
       testMdApi.o

INCLUDES = -I.
LIB += -lthostmduserapi
TARGET = testCTP.exe

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIB)

all : $(TARGET)

clean:
	rm -f $(TARGET) *.o
