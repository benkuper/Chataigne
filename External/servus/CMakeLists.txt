cmake_minimum_required(VERSION 3.16)
project(ChataigneServus VERSION 1.6.0 LANGUAGES CXX)

# Build the pinned Servus sources without its obsolete CMake bootstrap.
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVAHI REQUIRED IMPORTED_TARGET avahi-client)
find_package(Threads REQUIRED)
set(INCLUDE_NAME servus)
set(PROJECT_INCLUDE_NAME servus)
set(Name Servus)
set(NAME SERVUS)
set(namespace servus)
set(NAMESPACE SERVUS)
set(PROJECT_VERSION_ABI 6)
set(GIT_REVISION a44cf13)
foreach(file api.h version.h version.cpp)
    configure_file("${SERVUS_SOURCE_DIR}/CMake/common/cpp/${file}"
                   "${CMAKE_CURRENT_BINARY_DIR}/servus/${file}" @ONLY)
endforeach()
# These configuration headers are already used by Chataigne on every Linux arch.
foreach(file defines.h definesLinux.h)
    configure_file("include/servus/${file}" "servus/${file}" COPYONLY)
endforeach()
add_library(Servus SHARED
    "${CMAKE_CURRENT_BINARY_DIR}/servus/version.cpp"
    "${SERVUS_SOURCE_DIR}/servus/md5/md5.cc"
    "${SERVUS_SOURCE_DIR}/servus/serializable.cpp"
    "${SERVUS_SOURCE_DIR}/servus/servus.cpp"
    "${SERVUS_SOURCE_DIR}/servus/uint128_t.cpp"
    "${SERVUS_SOURCE_DIR}/servus/uri.cpp")
target_include_directories(Servus PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${SERVUS_SOURCE_DIR}")
target_compile_features(Servus PRIVATE cxx_std_11)
target_compile_definitions(Servus PRIVATE SERVUS_SHARED)
target_link_libraries(Servus PRIVATE PkgConfig::AVAHI Threads::Threads)
set_target_properties(Servus PROPERTIES VERSION 1.6.0 SOVERSION 6)
install(TARGETS Servus LIBRARY DESTINATION lib)
