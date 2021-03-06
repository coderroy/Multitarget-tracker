# ----------------------------------------------------------------------------
# ����������� ������ CMake, ��������� ���������� ���� ����
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
project(MultitargetTracker)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors --std=gnu++14" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native --fast-math -ffast-math -funroll-loops -Wall -DNDEBUG -DBOOST_DISABLE_ASSERTS" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -march=native -mtune=native -Wall -DDEBUG" CACHE STRING COMPILE_FLAGS FORCE)
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4" CACHE STRING COMPILE_FLAGS FORCE)
endif()


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# ---------------------------------------------------------------------------- 
# ������������, ��� FindOpenCV.cmake ���������� �� ������ CMAKE_MODULE_PATH.
# ----------------------------------------------------------------------------
  FIND_PACKAGE(OpenCV)
# ----------------------------------------------------------------------------  
# ���� ��� .cpp � .h ����� � ��������� �� � ��� ������ 
# ----------------------------------------------------------------------------
  set(folder_source  main.cpp
		     Detector/Detector.cpp
             HungarianAlg/HungarianAlg.cpp
             vibe_src/BackgroundSubtract.cpp
             vibe_src/VIBE.cpp
             Tracker/Ctracker.cpp
             Tracker/Kalman.cpp)

  set(folder_header  Detector/Detector.h
		     HungarianAlg/HungarianAlg.h
             vibe_src/BackgroundSubtract.h
             vibe_src/VIBE.h
             Tracker/Ctracker.h
             Tracker/Kalman.h
             Tracker/defines.h)

  SOURCE_GROUP("Source Files" FILES ${folder_source})
  SOURCE_GROUP("Header Files" FILES ${folder_header})
# ----------------------------------------------------------------------------  
# ������� ������
# ----------------------------------------------------------------------------
  ADD_EXECUTABLE(MultitargetTracker ${folder_source} ${folder_header})
# ----------------------------------------------------------------------------   
# ��������� include ����������
# ----------------------------------------------------------------------------
  INCLUDE_DIRECTORIES(
		      ${PROJECT_SOURCE_DIR}/Detector
		      ${PROJECT_SOURCE_DIR}/HungarianAlg
              ${PROJECT_SOURCE_DIR}/Tracker
		      ${PROJECT_SOURCE_DIR}/vibe_src)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
  INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
# ----------------------------------------------------------------------------
# � Lib-� opencv 
# ----------------------------------------------------------------------------
  TARGET_LINK_LIBRARIES(MultitargetTracker ${OpenCV_LIBS})
