CXX = g++
CXXFLAGS = -O2 -g -Wall
OBJS = TraderSpi.o \
       testTraderApi.o

INCLUDES = -I.
LIB += -lthosttraderapi
TARGET = testTrader.exe

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIB)

all : $(TARGET)

clean:
	rm -f $(TARGET) *.o
