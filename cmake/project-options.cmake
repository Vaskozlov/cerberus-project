option(CCL_LIBCPP "Use libc++ when compiling with clang" OFF)
option(CCL_STRICT_COMPILATION "Enable -Werror strict compilation" ON)
option(CCL_PRECOMPILED_HEADERS "Use precompiled headers" OFF)
option(CCL_ADDRESS_SANITIZER "Use sanitizers" OFF)
option(CCL_THREAD_SANITIZER "Use thread sanitizers" OFF)
option(CCL_UNDEFINED_SANITIZER "Use undefined behavior sanitizers" OFF)
option(CCL_STATIC_LIBRARY "Create static version of library " OFF)

message(STATUS "CC " ${CMAKE_C_COMPILER})
message(STATUS "CXX " ${CMAKE_CXX_COMPILER})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Use libcpp? " ${CCL_LIBCPP})
message(STATUS "Strict compilation? " ${CCL_STRICT_COMPILATION})
message(STATUS "Precompiled headers? " ${CCL_PRECOMPILED_HEADERS})
message(STATUS "Static version? " ${CCL_STATIC_LIBRARY})
message(STATUS "Address sanitizer? " ${CCL_ADDRESS_SANITIZER})
message(STATUS "Thread sanitizer? " ${CCL_THREAD_SANITIZER})
message(STATUS "Undefined behavior sanitizers? " ${CCL_UNDEFINED_SANITIZER})
