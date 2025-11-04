set(PROJECT_NAME GrayImage) 

################################################################################
# Source groups
################################################################################

set(OpenCV_Lib_Sources
    "GrayImage.c"
    "GrayImage.h"
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

