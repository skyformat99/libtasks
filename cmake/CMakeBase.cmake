# libtasks cmake base module

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(OS_NAME _OS_LINUX_)
  execute_process(COMMAND getconf LEVEL1_DCACHE_LINESIZE OUTPUT_VARIABLE CACHE_LINE_SIZE OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_definitions(-DCACHE_LINE_SIZE=${CACHE_LINE_SIZE})
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(OS_NAME _OS_DARWIN_)
  execute_process(COMMAND sysctl -n hw.cachelinesize OUTPUT_VARIABLE CACHE_LINE_SIZE OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_definitions(-DCACHE_LINE_SIZE=${CACHE_LINE_SIZE})
elseif(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
  set(OS_NAME _OS_FREEBSD_)
endif()

add_definitions(-D${OS_NAME})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_definitions(-O4)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
    add_definitions(-Wno-attributes)
  endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-ggdb)
  endif(CMAKE_BUILD_TYPE MATCHES "Debug")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-O3)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4.0")
    add_definitions(-Wno-attributes)
  endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4.0")
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-g)
  endif(CMAKE_BUILD_TYPE MATCHES "Debug")
else()
  message(WARNING "Unsupported compiler ${CMAKE_CXX_COMPILER} (${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")
  add_definitions(-O3)
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-g)
  endif(CMAKE_BUILD_TYPE MATCHES "Debug")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message(STATUS "Enabled debug messages")
  add_definitions(-D_DEBUG_OUTPUT)
  if(NOT DISABLE_LOGMUTEX MATCHES "Y" OR NOT DISABLE_LOGMUTEX MATCHES "y")
    add_definitions(-DLOGMUTEX)
  endif(NOT DISABLE_LOGMUTEX MATCHES "Y" OR NOT DISABLE_LOGMUTEX MATCHES "y")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

add_definitions(-Wall -Wextra -Wlong-long -Wmissing-braces -std=c++1y -pthread)

find_package(PkgConfig)
find_package(Boost 1.53.0 REQUIRED system thread REQUIRED)
find_package(LibEV REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBEV_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${LIBEV_LIBRARY_DIRS})