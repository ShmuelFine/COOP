set(PROJECT_NAME COOP_C_TestProxy)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "ClassTest.c"
    "ClassTest.h"
    "ProxyExportDefs.h"
)
source_group("" FILES ${no_group_source_files})

set(Class_Examples
    "BaseClassExample.c"
    "BaseClassExample.h"
    "DerivedClassExample.c"
    "DerivedClassExample.h"
    "Rectangle.c"
    "Rectangle.h"
)
source_group("Class Examples" FILES ${Class_Examples})

set(Class_Tests
    "ClassUnitTest.c"
    "ClassUnitTest.h"
    "InheritTest.c"
    "InheritTest.h"
    "rbTreeIterUnitTest.c"
    "rbTreeIterUnitTest.h"
    "rbTreeUnitTest.c"
    "rbTreeUnitTest.h"
    "vectorIteratorUnitTest.c"
    "vectorIteratorUnitTest.h"
    "vectorUnitTest.c"
    "vectorUnitTest.h"
)
source_group("Class Tests" FILES ${Class_Tests})

set(Exceptions
    "ExeptionUnitTest.c"
    "ExeptionUnitTest.h"
)
source_group("Exceptions" FILES ${Exceptions})

set(MemManagementTesting
    "MemManagementUnitTest.c"
    "MemManagementUnitTest.h"
)
source_group("MemManagementTesting" FILES ${MemManagementTesting})

set(ScopeTesting
    "CharTest.c"
    "CharTest.h"
    "ScopesUnitTest.c"
    "ScopesUnitTest.h"
    "ScopeTester.c"
    "ScopeTester.h"
    "TypeTest.c"
    "TypeTest.h"
)
source_group("ScopeTesting" FILES ${ScopeTesting})

set(ALL_FILES
    ${no_group_source_files}
    ${Class_Examples}
    ${Class_Tests}
    ${Exceptions}
    ${MemManagementTesting}
    ${ScopeTesting}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE COOPCTestProxy)

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}_d"
    )
endif()
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/bin/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/../COOP"
        ">"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_COOP_PROXY_EXPORTING_;"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_BASE_COOP_EXPORTING_;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    COOP
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/bin"
        ">"
    )
endif()

