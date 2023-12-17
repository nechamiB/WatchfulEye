cmake_minimum_required(VERSION 3.16)

project(ServerProcess)
set(PROJECT_NAME ServerProcess)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------

set(Source_Files 
				 "main.cpp"
				 )
set(ALL_FILES ${Source_Files})

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
					  Threads::Threads
					  spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
					  ${OpenCV_LIBS}
					  WatchfulEye)
