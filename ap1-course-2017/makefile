CXX = g++
CXXFLAGS = -std=c++0x
RM = rm -f
LIBS = -pthread -lboost_serialization -I.

all: core.o utils.o client.o server.o

core.o: connection.o entities.o threading.o trip.o vehicles.o
	$(CXX) $(CXXFLAGS) -c src/Core/*.cpp

connection.o:
	$(CXX) $(CXXFLAGS) -c src/Core/Connection/*.cpp

threading.o:
	$(CXX) $(CXXFLAGS) -c src/Core/Threading/*.cpp

entities.o:
	$(CXX) $(CXXFLAGS) -c src/Core/Entities/*.cpp

vehicles.o:
	$(CXX) $(CXXFLAGS) -c src/Core/Vehicles/*.cpp

trip.o:
	$(CXX) $(CXXFLAGS) -c src/Core/Trip/*.cpp


utils.o: graph.o factories.o
	$(CXX) $(CXXFLAGS) -c src/Util/*.cpp


graph.o:
	$(CXX) $(CXXFLAGS) -c src/Util/Graph/*.cpp

factories.o:
	$(CXX) $(CXXFLAGS) -c src/Util/Factories/*.cpp

SERVER_FILES = src/Server/Tasks/*.cpp src/Server/TaxiCenter/*.cpp src/Server/*.cpp

server.o:
	$(CXX) $(CXXFLAGS) *.o $(SERVER_FILES) $(LIBS) -o server.out

client.o:
	$(CXX) $(CXXFLAGS) *.o src/Client/*.cpp $(LIBS) -o client.out

clean:
	$(RM) *.o
