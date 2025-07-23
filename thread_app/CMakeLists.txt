cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE}) 
project(thread_app)

# if (CONFIG_BUILD_MAIN_APP)
#     target_sources(app PRIVATE src/main.c)
#     message(STATUS "Building main application")
# else()
#   message(FATAL_ERROR "Main application build is disabled. Set CONFIG_BUILD_MAIN_APP=y in prj.conf.")
# endif()

zephyr_library_sources_ifdef(CONFIG_BUILD_MAIN_APP src/main.c)

zephyr_library_sources_ifdef(CONFIG_BUILD_THREAD_APP src/thread.c)
