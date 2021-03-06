cmake_minimum_required(VERSION 2.8.4)
SET(CMAKE_AUTOMOC ON)

project("Clarus")
INCLUDE(../clews/Clews.cmake)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

clews_add_qt_resources()
clews_add_executable()

find_library(SOCK_LIBRARY NAMES sock libsock HINTS ${CMAKE_CURRENT_LIST_DIR}/lib /usr/local/lib/clews)
find_library(RTAUDIO_LIBRARY NAMES rtaudio librtaudio HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
MESSAGE(STATUS "SOCK_LIBRARY: ${SOCK_LIBRARY}")
MESSAGE(STATUS "RTAUDIO_LIBRARY: ${RTAUDIO_LIBRARY}")
clews_target_link_libraries(${SOCK_LIBRARY})
clews_target_link_libraries(${RTAUDIO_LIBRARY})

find_library(OPENCV_CORE_LIBRARY NAMES opencv_core libopencv_core opencv_core300 libopencv_core300 HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
find_library(OPENCV_HIGHGUI_LIBRARY NAMES opencv_highgui libopencv_highgui opencv_highgui300 libopencv_highgui300 HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
find_library(OPENCV_IMGPROC_LIBRARY NAMES opencv_imgproc libopencv_imgproc opencv_imgproc300 libopencv_imgproc300 HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
MESSAGE(STATUS "OPENCV_CORE_LIBRARY: ${OPENCV_CORE_LIBRARY}")
MESSAGE(STATUS "OPENCV_HIGHGUI_LIBRARY: ${OPENCV_HIGHGUI_LIBRARY}")
MESSAGE(STATUS "OPENCV_IMGPROC_LIBRARY ${OPENCV_IMGPROC_LIBRARY}")
clews_target_link_libraries(${OPENCV_CORE_LIBRARY})
clews_target_link_libraries(${OPENCV_HIGHGUI_LIBRARY})
clews_target_link_libraries(${OPENCV_IMGPROC_LIBRARY})

IF(DEFINED WIN32)
find_library(OPENCV_VIDEOIO_LIBRARY NAMES opencv_videoio libopencv_videoio opencv_videoio300 libopencv_videoio300 HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
find_library(OPENCV_IMGCODECS_LIBRARY NAMES opencv_imgcodecs libopencv_imgcodecs opencv_imgcodecs300 libopencv_imgcodecs300 HINTS ${CMAKE_CURRENT_LIST_DIR}/lib)
MESSAGE(STATUS "OPENCV_VIDEOIO_LIBRARY: ${OPENCV_VIDEOIO_LIBRARY}")
MESSAGE(STATUS "OPENCV_IMGCODECS_LIBRARY ${OPENCV_IMGCODECS_LIBRARY}")
clews_target_link_libraries(${OPENCV_VIDEOIO_LIBRARY})
clews_target_link_libraries(${OPENCV_IMGCODECS_LIBRARY})
ENDIF()

clews_target_link_libraries(Qt5::Core)
clews_target_link_libraries(Qt5::Widgets)
clews_target_link_libraries(Qt5::Network)

