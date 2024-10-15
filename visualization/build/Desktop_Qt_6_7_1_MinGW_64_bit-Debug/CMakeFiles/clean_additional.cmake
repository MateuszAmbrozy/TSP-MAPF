# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\visualization_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\visualization_autogen.dir\\ParseCache.txt"
  "visualization_autogen"
  )
endif()
