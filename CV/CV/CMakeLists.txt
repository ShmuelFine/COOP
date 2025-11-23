set(PROJECT_NAME CV_Lib) 

################################################################################
# Source groups
################################################################################

set(CV_Lib_Sources
    "GrayImage.c"
    "GrayImage.h"
    "ImageProcessor.h"
    "ImageProcessor.c"
    "GaussianProcessor.h"
    "GaussianProcessor.c"
    "SobelProcessor.h"
    "SobelProcessor.c"
    "NMSProcessor.h"
    "NMSProcessor.c"
    "ThresholdProcessor.h"
    "ThresholdProcessor.c"
    "BorderProcessor.h"
    "BorderProcessor.c"
)
source_group("CV_Lib" FILES ${CV_Lib_Sources})

set(ALL_FILES
    ${CV_Lib_Sources}
)

################################################################################
# Library Target 
################################################################################
add_library(CV_Lib STATIC ${CV_Lib_Sources})

target_include_directories(CV_Lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../COOP")

target_link_libraries(CV_Lib PRIVATE COOP)
