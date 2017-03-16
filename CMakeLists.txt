cmake_minimum_required(VERSION 3.5)
PROJECT(web_crawler)

FIND_PACKAGE(CURL)
IF(NOT CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)



add_executable(wc main.c)
target_include_directories(wc PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(wc ${CURL_LIBRARIES})

