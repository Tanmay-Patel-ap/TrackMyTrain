cmake_minimum_required(VERSION 3.16)
project(TrackMyTrain VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# cpp-httplib - header-only HTTP client
FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.18.5
  EXCLUDE_FROM_ALL
  SYSTEM
)
FetchContent_MakeAvailable(httplib)

# nlohmann/json - header-only JSON library
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  EXCLUDE_FROM_ALL
  SYSTEM
)
FetchContent_MakeAvailable(nlohmann_json)

set(SOURCES
  main.cpp
  config/AppConfig.cpp
  api/HttpClient.cpp
  api/RailwayApiClient.cpp
  utils/JsonParser.cpp
  utils/Logger.cpp
  services/TrainService.cpp
  services/PNRService.cpp
  services/StationService.cpp
  services/SeatService.cpp
  controllers/TrainController.cpp
  controllers/PNRController.cpp
  controllers/StationController.cpp
  controllers/SeatController.cpp
)

add_executable(TrackMyTrain ${SOURCES})

target_compile_definitions(TrackMyTrain PRIVATE
  TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_data"
)

target_include_directories(TrackMyTrain PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${httplib_SOURCE_DIR}
  ${nlohmann_json_SOURCE_DIR}/include
)

if(WIN32)
  target_link_libraries(TrackMyTrain PRIVATE ws2_32 crypt32)
endif()
