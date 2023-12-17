cmake_minimum_required(VERSION 3.16)

project(WatchfulEyeTest)
set(PROJECT_NAME WatchfulEyeTest)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------

set(Source_Files "protectedQueue.cpp"
				 "calcAvg.cpp"
				 "cmpFrames.cpp"
				 "improveQueue.cpp"
				 )
set(ALL_FILES ${Source_Files})


# ---------------------------------------------------------------------------------------
# Catch2
# ---------------------------------------------------------------------------------------

find_package(Catch2 3 REQUIRED)

# ---------------------------------------------------------------------------------------
# Threads
# ---------------------------------------------------------------------------------------

find_package(Threads REQUIRED)

# ---------------------------------------------------------------------------------------
# spdlog
# ---------------------------------------------------------------------------------------

find_package(spdlog REQUIRED)

# ---------------------------------------------------------------------------------------
# OpenCV
# ---------------------------------------------------------------------------------------

list(APPEND CMAKE_PREFIX_PATH ${OpenCV_BIN_DIR})

# Set OpenCV_DIR to the OpenCV installation directory
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(OpenCV_DIR "C:/openCV/opencv/build")
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV libraries and directories
include_directories(${OpenCV_INCLUDE_DIRS})

# ---------------------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_link_libraries(${PROJECT_NAME}
					  PUBLIC
					  Catch2::Catch2WithMain
					  Threads::Threads
					  spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
					  ${OpenCV_LIBS}
					  WatchfulEye)
