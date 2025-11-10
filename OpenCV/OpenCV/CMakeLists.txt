set(PROJECT_NAME OpenCV_Lib) 

################################################################################
# Source groups
################################################################################

set(OpenCV_Lib_Sources
    "GrayImage.c"
    "GrayImage.h"
    "Functions.c"
)
source_group("OpenCV_Lib" FILES ${OpenCV_Lib_Sources})

set(ALL_FILES
    ${OpenCV_Lib_Sources}
)

################################################################################
# Library Target 
################################################################################
add_library(OpenCV_Lib STATIC ${OpenCV_Lib_Sources})

target_include_directories(OpenCV_Lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../COOP")

target_link_libraries(OpenCV_Lib PRIVATE COOP)
