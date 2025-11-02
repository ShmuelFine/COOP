# ===== COOP Benchmarks =====

set(PROJECT_NAME Benchmarks_${MEM_SIZE})

################################################################################
# Source groups
################################################################################
set(BenchSources
    "coop_bench_queue.c"
)

source_group("" FILES ${BenchSources})

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${BenchSources})

set(ROOT_NAMESPACE ${PROJECT_NAME})
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

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../COOP"
)

################################################################################
# Compile definitions
################################################################################

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<CONFIG:Release>:NDEBUG>"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<CONFIG:Release>:NDEBUG>"
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
            $<$<CONFIG:Release>:/Oi;/Gy>
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/Oi;/Gy>
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    else()
        
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/Oi;/Gy>
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()

    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:/INCREMENTAL>
            $<$<CONFIG:Release>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO>
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:/INCREMENTAL>
            $<$<CONFIG:Release>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO>
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    else()
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:/INCREMENTAL>
            $<$<CONFIG:Release>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO>
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()

elseif(UNIX AND NOT APPLE)
   
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-incompatible-pointer-types)
else()
   
endif()

################################################################################
# Dependencies
################################################################################

target_link_libraries(${PROJECT_NAME} PRIVATE COOP)
