set(PROJECT_NAME UnitTest)

################################################################################
# Source files
################################################################################
set(TEST_SOURCES
    "_RUNNER_.c"         
    "GrayImageUnitTest.c" 
    "GrayImageUnitTest.h" 
)

################################################################################
# Target: Executable
################################################################################
add_executable(${PROJECT_NAME} ${TEST_SOURCES})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${CMAKE_CURRENT_SOURCE_DIR}/../../COOP"
    "${CMAKE_CURRENT_SOURCE_DIR}/../OpenCV"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../UnitTestC"
)

################################################################################
# Dependencies (Link Libraries)
################################################################################
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenCV_Lib
    COOP
)

enable_testing()
add_test(NAME ${PROJECT_NAME}_Run COMMAND ${PROJECT_NAME})