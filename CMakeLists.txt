cmake_minimum_required(VERSION 3.23)
# Setting vars
set (SRC_DIRS       ${CMAKE_CURRENT_LIST_DIR}/src)
set (INCLUDE_DIRS   ${CMAKE_CURRENT_LIST_DIR}/include)

# Debug message
message(" ========== Entering ${CMAKE_CURRENT_LIST_DIR}/CMakelists.txt========== ")

idf_component_register(SRC_DIRS "./src"
                     INCLUDE_DIRS "./include"
                     REQUIRES osal)

if(TEST) # Not working yet

idf_component_register()
add_subdirectory(../actor-framework)
target_link_libraries(main PUBLIC actor-framework)
# project(LibActor)

# Building OSAL
add_subdirectory(osal)
add_library(osal_lib 
    ${CMAKE_CURRENT_LIST_DIR}/osal/targets/freertos/os_mutex.c
    ${CMAKE_CURRENT_LIST_DIR}/osal/targets/freertos/os_queue.c
    ${CMAKE_CURRENT_LIST_DIR}/osal/targets/freertos/os_sem.c   
    ${CMAKE_CURRENT_LIST_DIR}/osal/targets/freertos/os_task.c
    ${CMAKE_CURRENT_LIST_DIR}/osal/targets/freertos/os_timer.c
)
# Build library
target_include_directories(osal_lib PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/osal/include
    ${CMAKE_CURRENT_LIST_DIR}/targets/freertos/
)


add_executable(LibActor} 
    ${SRC_DIRS}/actor.c
    ${SRC_DIRS}/event.c
    ${SRC_DIRS}/mempool.c
    # ${SRC_DIRS}/ps.c
    ${SRC_DIRS}/state_machine.c
)

target_include_directories(LibActor} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(LibActor} PUBLIC osal_lib)
# target_link_libraries(main PUBLIC LibActor})
endif(TEST)

message(" ========== Exitting ${CMAKE_CURRENT_LIST_DIR}/CMakelists.txt ========== ")


# idf_component_register(SRCS "./src/lwmem/lwmem.c"
#                       INCLUDE_DIRS "./src/include/lwmem/")

