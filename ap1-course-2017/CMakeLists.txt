cmake_minimum_required(VERSION 3.6)
project(AP1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread -Wextra")

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)

set(BOOST_ROOT CACHE PATH "/usr/include/boost/")
find_package(Boost 1.58.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})

set(SERIALIZATION src/Util/Serialization/Serialization.h)

set(GRAPH
        src/Util/Graph/GraphAlgorithm.h src/Util/Graph/GraphNode.h
        src/Util/Graph/BFS.h src/Util/Graph/BFS.cpp)

set(UTILS ${SERIALIZATION}
        src/Util/Factories/DriverFactory.cpp src/Util/Factories/DriverFactory.h
        src/Util/Factories/TripInfoFactory.cpp src/Util/Factories/TripInfoFactory.h
        src/Util/Factories/TaxiFactory.cpp src/Util/Factories/TaxiFactory.h
        src/Util/InputParser.cpp src/Util/InputParser.h
        src/Util/DriverInput.h src/Util/easylogging++.h)

set(THREADING
        src/Core/Threading/ThreadPool.cpp src/Core/Threading/ThreadPool.h src/Core/Threading/Task.h
        src/Core/Threading/Thread.cpp src/Core/Threading/Thread.h
        src/Core/Threading/WorkQueue.cpp src/Core/Threading/WorkQueue.h
        src/Core/Threading/PoolWorkerThread.cpp src/Core/Threading/PoolWorkerThread.h)

set(CONNECTION
        src/Core/Connection/Communicable.cpp src/Core/Connection/Communicable.h
        src/Core/Connection/Command.h src/Core/Connection/Message.h)

set(CORE_FILES ${UTILS} ${GRAPH} ${THREADING} ${CONNECTION}
        src/Core/Point.cpp src/Core/Point.h src/Core/Map.cpp src/Core/Map.h src/Core/SynchronizedQueue.h
        src/Core/Trip/Trip.cpp src/Core/Trip/Trip.h src/Core/Trip/TripInfo.cpp src/Core/Trip/TripInfo.h
        src/Core/Entities/Driver.cpp src/Core/Entities/Driver.h
        src/Core/Vehicles/Vehicle.cpp src/Core/Vehicles/Vehicle.h src/Core/Vehicles/Taxi.cpp src/Core/Vehicles/Taxi.h
        src/Core/Trip/TripEndedListener.h src/Core/Trip/TripEndedNotifier.cpp src/Core/Trip/TripEndedNotifier.h)

set(SERVER_FILES
        src/Server/Server.cpp src/Server/Server.h
        src/Server/TaxiCenter/TaxiCenter.cpp src/Server/TaxiCenter/TaxiCenter.h src/Server/TaxiCenter/DriverInfo.h
        src/Server/TaxiCenter/DriverDatabase.cpp src/Server/TaxiCenter/DriverDatabase.h
        src/Server/Tasks/CalculatePathTask.cpp src/Server/Tasks/CalculatePathTask.h
        src/Server/TaxiCenter/PathCalculation.cpp src/Server/TaxiCenter/PathCalculation.h
        src/Server/Tasks/ClientTask.cpp src/Server/Tasks/ClientTask.h
        src/Server/DriverQueue.cpp src/Server/DriverQueue.h src/Server/MessageQueue.h)


set(CLIENT_FILES src/Client/Client.cpp src/Client/Client.h)


set(SOURCE_FILES ${CORE_FILES} ${SERVER_FILES} ${UTILS})

set(TEST_FILES
        Tests/PointTest.cpp Tests/BfsTest.cpp Tests/MapTest.cpp Tests/TripTest.cpp
        Tests/DriverTest.cpp Tests/TripInfoTest.cpp Tests/GraphNodeTest.cpp Tests/VehicleTest.cpp
        Tests/DriverFactoryTest.cpp Tests/TaxiFactoryTest.cpp Tests/TripInfoFactoryTest.cpp)

add_executable(server src/Server/server_main.cpp ${CORE_FILES} ${SERVER_FILES})
target_link_libraries(server ${Boost_LIBRARIES})

add_executable(client src/Client/client_main.cpp ${CORE_FILES} ${CLIENT_FILES})
target_link_libraries(client ${Boost_LIBRARIES})

add_executable(runUnitTests Tests/tests_main.cpp ${TEST_FILES} ${SOURCE_FILES})
target_link_libraries(runUnitTests gtest gtest_main ${Boost_LIBRARIES})