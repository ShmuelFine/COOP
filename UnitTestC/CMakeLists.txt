set(PROJECT_NAME UnitTestC)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "_RUNNER_.c"
)
source_group("" FILES ${no_group_source_files})

set(AuxClasses
    "BaseClassExample.c"
    "BaseClassExample.h"
    "DerivedClassExample.c"
    "DerivedClassExample.h"
    "Rectangle.c"
    "Rectangle.h"
    "ScopeTester.c"
    "ScopeTester.h"
)
source_group("AuxClasses" FILES ${AuxClasses})

set(DataStructuresTests
    "SharedPodPtrUnitTest.c"
    "SharedPodPtrUnitTest.h"
    "TensorUnitTest.c"
    "TensorUnitTest.h"
    "VectorUnitTest.c"
    "VectorUnitTest.h"
    "BinaryTreeUnitTest.h"
    "BinaryTreeUnitTest.c"
)
source_group("DataStructuresTests" FILES ${DataStructuresTests})

set(InfraTests
    "CodeBlocksUnitTest.c"
    "CodeBlocksUnitTest.h"
    "ExeptionUnitTest.c"
    "ExeptionUnitTest.h"
    "InheritanceTest.c"
    "InheritanceTest.h"
    "MemManagementUnitTest.c"
    "MemManagementUnitTest.h"
    "ScopesUnitTest.c"
    "ScopesUnitTest.h"
)
source_group("InfraTests" FILES ${InfraTests})

set(Sample
    "DemoClassTest.c"
    "DemoClassTest.h"
)
source_group("Sample" FILES ${Sample})

set(ALL_FILES
    ${no_group_source_files}
    ${AuxClasses}
    ${DataStructuresTests}
    ${InfraTests}
    ${Sample}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

set(ROOT_NAMESPACE UnitTestC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../COOP"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../COOP"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
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
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
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
else(IF UNIX AND NOT APPLE)
    # For non-osx platforms
    # Specify `-Wno-incompatible-pointer-types` for GCC to avoid it emitting
    # a lot of warnings due to the function parameter types
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-incompatible-pointer-types)
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    COOP
)

