# CMakeLists.txt for timeBase module

# Check if the timeBase module is enabled
if (CONFIG_TIMEBASE)

    # Check current directory for the source files
    zephyr_include_directories(.)

    # Add the timeBase source file to the build
    zephyr_library_sources(timeBase.c)

endif()
