set(PROJECT_NAME WatchfulEye)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------
set(Header_Files
    "Yolo.h"
    "Logging.h"
    "Camera.h"
    "Server.h"
    "Queue.h"
    "Sqlite.h"
    "sqlite-amalgamation/sqlite3.h"
    "‏‏ConfigurationCamera.h"
    "‏‏ConfigurationServer.h"
    #if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #"ConfigurationCamera.h"
    #"ConfigurationServer.h"
    #endif()
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Yolo.cpp"
    "Logging.cpp"
    "Camera.cpp"
    "Server.cpp"
    "Queue.cpp"
    "Sqlite.cpp"
    "sqlite-amalgamation/sqlite3.c"
    "‏‏ConfigurationCamera.cpp"
    "‏‏ConfigurationServer.cpp"
   #if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   #"ConfigurationCamera.cpp"
   #"ConfigurationServer.cpp"
   #endif()
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

# ---------------------------------------------------------------------------------------
# Threads
# ---------------------------------------------------------------------------------------

find_package(Threads REQUIRED)

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
# spdlog
# ---------------------------------------------------------------------------------------

find_package(spdlog REQUIRED)

# ---------------------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------------------
add_library(${PROJECT_NAME} ${ALL_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads ${OpenCV_LIBS}  ${CMAKE_DL_LIBS} spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> myproto)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_link_libraries(${PROJECT_NAME} PUBLIC ncurses)
endif()

# ---------------------------------------------------------------------------------------


set(ROOT_NAMESPACE WatchfulEye)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

# ---------------------------------------------------------------------------------------
# Compile definitions
# ---------------------------------------------------------------------------------------
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()
# ---------------------------------------------------------------------------------------
# Compile and link options
# ---------------------------------------------------------------------------------------
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()