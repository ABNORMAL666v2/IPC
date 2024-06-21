CXX = g++
CXXFLAGS = -std=c++11 -Wall -m64
INCLUDES = -IG:/repos/ipcv3/vcpkg_installed/x64-windows/x64-windows/include \
           -IG:/repos/ipcv3/vcpkg_installed/x64-windows/x64-windows/include/rapidjson
LIBDIR = -LG:/repos/ipcv3/vcpkg_installed/x64-windows/x64-windows/lib
LIBS = -lcurl

SRCS = ipcv3.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = ipc.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBDIR) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
