cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(starter_project)

# If Conan exists, try to use it
find_program(CONAN_BIN NAMES conan)
if(CONAN_BIN)
  message(STATUS "Conan found at ${CONAN_BIN}. Using.")

  # Download the integration from github
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(
      STATUS
        "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    # Notice the 0.15 tag. Update as needed.
    file(DOWNLOAD
         "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
         "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP BUILD missing)
else()
  message(WARNING "Conan not found, trying to proceed without it!")
endif()

# conanfile.txt is set to use Find<package_name> targets. If conan indeed ran,
# let's add the targets generated.
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

# Find dependencies (hopefully installed with conan)
find_package(SQLite3 REQUIRED)

add_executable(sqlite_example src/sqlite_example/main.cpp)
# Target oriented features without explicit compiler flags!
target_compile_features(sqlite_example PRIVATE cxx_std_14)
# Sometimes the name of the targets aren't that trivial...
target_link_libraries(sqlite_example PRIVATE SQLite::SQLite3)

# For Qt we should turn on the meta-object compilers and resource compilers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)

add_executable(qt5_example src/qt5_example/main.cpp)
target_link_libraries(qt5_example Qt5::Widgets)

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(qt5_example PROPERTIES WIN32_EXECUTABLE YES)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(
      SEND_ERROR
        "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif(MSVC)
elseif(APPLE)
  set_target_properties(qt5_example PROPERTIES MACOSX_BUNDLE YES)
elseif(UNIX)
  # Nothing special required
else()
  message(
    WARNING
      "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)

# On Windows, run windeployqt after building
if(WIN32)
  # Run winddeployqt if it can be found
  find_program(
    WINDEPLOYQT_EXECUTABLE
    NAMES windeployqt
    HINTS ${QTDIR} ENV QTDIR
    PATH_SUFFIXES bin)
  add_custom_command(
    TARGET qt5_example
    POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:qt5_example>)
endif()
