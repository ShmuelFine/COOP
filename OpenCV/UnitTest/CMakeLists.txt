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
target_link_libraries(${PROJECT_NAME} PRIVATE COOP OpenCV_Lib)

# ======================================================================
# Debug build configuration for Visual Studio (MSVC)
# Ensures proper symbol (.pdb) generation and disables optimizations
# ======================================================================
if (MSVC)
  # Compiler flags: generate debug symbols (/Zi), disable optimization (/Od),
  # and enable runtime checks (/RTC1) for better debugging experience.
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:/Zi>
    $<$<CONFIG:Debug>:/Od>
    $<$<CONFIG:Debug>:/RTC1>
  )

  # Linker flags: create .pdb file (/DEBUG) and allow incremental linking.
  target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:/DEBUG>
    $<$<CONFIG:Debug>:/INCREMENTAL>
    $<$<CONFIG:Debug>:/DEBUG:FULL>
  )

  # Define the debug information format explicitly for modern MSVC toolchains.
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<CONFIG:Debug>,ProgramDatabase,>"
      CACHE STRING "" FORCE)
endif()


enable_testing()
add_test(NAME ${PROJECT_NAME}_Run COMMAND ${PROJECT_NAME})