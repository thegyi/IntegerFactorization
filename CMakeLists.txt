cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

project(TaskServer)
add_executable(TaskServer main.cpp ServerConfig.cpp ServerConfig.h IntegerFactorizationTask.cpp IntegerFactorizationTask.h uuid.h)
target_include_directories(TaskServer PUBLIC external/)
target_include_directories(TaskServer PUBLIC external/Simple-Web-Server)
target_include_directories(TaskServer PUBLIC /home/thegyi/projects/cryptopp)
target_link_libraries(TaskServer PUBLIC -lpthread)
target_link_libraries(TaskServer PRIVATE /home/thegyi/projects/CLionProjects/oled/libcryptopp_x64.a)

project(IntegerFactorizationClient)
add_executable(IntegerFactorizationClient FactorizationClient.cpp FactorizationClient.h)
target_include_directories(IntegerFactorizationClient PUBLIC external/)
target_include_directories(IntegerFactorizationClient PUBLIC external/Simple-Web-Server)
target_include_directories(IntegerFactorizationClient PUBLIC /home/thegyi/projects/cryptopp)
target_link_libraries(IntegerFactorizationClient PUBLIC -lpthread)
target_link_libraries(IntegerFactorizationClient PRIVATE /home/thegyi/projects/CLionProjects/oled/libcryptopp_x64.a)