# ===== COOP Benchmarks =====

# 1. Define a list of all C source files in the current directory
# The GLOB command finds all files matching the pattern.
# We'll use this list for the loop below.
file(GLOB BenchSources "*.c")

# Set the base project name without the MEM_SIZE suffix for the loop
# We'll re-apply the full name inside the loop for each target.
set(PROJECT_BASE_NAME Benchmarks)

################################################################################
# Source groups
################################################################################
# This is mainly for IDE organization, we can leave it out for a minimal change
# source_group("" FILES ${BenchSources}) 
################################################################################
# Include directories
################################################################################

# Loop through each source file found
foreach(SOURCE_FILE ${BenchSources})
    # Extract the base name of the source file (e.g., 'coop_bench_queue' from 'coop_bench_queue.c')
    get_filename_component(BENCH_NAME ${SOURCE_FILE} NAME_WE)
    
    # Construct the unique project name for this executable (e.g., Benchmarks_coop_bench_queue_${MEM_SIZE})
    # Note: We append the existing MEM_SIZE variable from your original project scope
    set(PROJECT_NAME "${PROJECT_BASE_NAME}_${BENCH_NAME}_${MEM_SIZE}")

    message(STATUS "Creating executable: ${PROJECT_NAME} from source: ${SOURCE_FILE}")

    ############################################################################
    # Target (The key change: creating an executable for a single source file)
    ############################################################################
    add_executable(${PROJECT_NAME} ${SOURCE_FILE})

    # Add include directory for *this* target
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../COOP"
    )

   
    set(ROOT_NAMESPACE ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
    )

    # --- Properties and Definitions (apply to the current target) ---
    
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
        )
    endif()

    ############################################################################
    # Compile definitions
    ############################################################################

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

    ############################################################################
    # Compile and link options
    ############################################################################
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

    ############################################################################
    # Dependencies (apply to the current target)
    ############################################################################
    target_link_libraries(${PROJECT_NAME} PRIVATE COOP)
    
# End of the loop - repeat for the next source file
endforeach()