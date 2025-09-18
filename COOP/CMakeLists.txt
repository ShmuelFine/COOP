set(PROJECT_NAME COOP)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "SharedPodPtr.c"
    "SharedPodPtr.h"
	"SharedObjPtr.c"
    "SharedObjPtr.h"
    "Tensor.c"
    "Tensor.h"
    "Vector.c"
    "Vector.h"
)
source_group("" FILES ${no_group_source_files})

set(Infra
    "ClassDefMacros.h"
    "COOP.h"
    "ExportDefs.h"
    "InheritenceDefMacros.h"
    "MathUtils.h"
    "Object.h"
    "ObjectBaseStructs.h"
    "ObjectLifecycleManagement.c"
    "ObjectLifecycleManagement.h"
    "UnitTestsInfra.h"
    "VirtualFuncCalling.c"
    "VirtualFuncCalling.h"
)
source_group("Infra" FILES ${Infra})

set(MemoryManagement
    "DynamicMemoryManagement.c"
    "DynamicMemoryManagement.h"
    "HeapCache.c"
    "HeapCache.h"
    "ICache.c"
    "ICache.h"
    "InMemoryCache.c"
    "InMemoryCache.h"
)
source_group("MemoryManagement" FILES ${MemoryManagement})

set(ALL_FILES
    ${no_group_source_files}
    ${Infra}
    ${MemoryManagement}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES} "Queue.h" "Queue.c")

set(ROOT_NAMESPACE COOP)


################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}"
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
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    endif()
else(IF UNIX AND NOT APPLE)
    # For non-osx platforms
    # Specify `-Wno-incompatible-pointer-types` for GCC to avoid it emitting
    # a lot of warnings due to the function parameter types
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-incompatible-pointer-types)
endif()

