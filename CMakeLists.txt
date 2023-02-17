cmake_minimum_required(VERSION 3.22)

# Specify generator

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
message("==================================================================================================")
idf_component_register( SRC_DIRS        "./src"
                        INCLUDE_DIRS    "./include/" )
message("==================================================================================================")
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# idf_component_register(SRCS "./src/lwmem/lwmem.c"
#                       INCLUDE_DIRS "./src/include/lwmem/")

