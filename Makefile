# Make file to build Router exe

CXX=g++
CFLAGS=-c -Wall -std=c++11
INCLUDES=-IC:/MinGW14/MinGW/include

Router.exe: main.o router.o Settings.o 
	g++  main.o router.o Settings.o -o Router.exe -lboost_system -lwsock32 -lws2_32
	
main.o: main.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) main.cpp

router.o: router.cpp router.hpp
	$(CXX) $(CFLAGS) -D_WIN32_WINNT=0x0501 $(INCLUDES) router.cpp

Settings.o: Settings.cpp Settings.hpp
	$(CXX) $(CFLAGS) $(INCLUDES) Settings.cpp
	
clean:
	del *.o *.exe
	