set(CMAKE_BUILD_TYPE Release)
cmake_minimum_required(VERSION 3.20)
project(Motor)
set(CMAKE_CXX_STANDARD 23)
set(x86 OFF CACHE BOOL "Build for 32bit")

set(MAKE_GENERATOR_PLATFORM x64)
set(OUTPUT_TYPE RELEASE)
set(CMAKE_BUILD_TYPE Release)

set(VERSION "0.0.1" CACHE STRING "")
#add_compile_definitions(MOTOR_VERSION=${VERSION})

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set(W_LIB_PATH ${LIB_PATH}windows/)
set(U_LIB_PATH ${LIB_PATH}linux/)
set(M_LIB_PATH ${LIB_PATH}macos/)


if(x86)
	set(CMAKE_CXX_FLAGS "-m32")
	set(W_LIB_PATH ${LIB_PATH}windows/x86/ )
	set(U_LIB_PATH ${LIB_PATH}linux/x86/)
	set(M_LIB_PATH ${LIB_PATH}macos/x86/)
	message("Building for x86 (32-Bit)")
endif(x86)

if(UNIX)
	if(APPLE) 
		file(GLOB LIBRARIES CONFIGURE_DEPENDS "${M_LIB_PATH}*.dylib")
		message("OS: MacOS")
	endif(APPLE)
	
	if(NOT APPLE)
		message("OS: Linux")
	endif(NOT APPLE)
endif(UNIX)

file(GLOB_RECURSE LIBRARIES CONFIGURE_DEPENDS "${LIBRARY_PATH}*.lib" "${LIBRARY_PATH}*.dylib" "${LIBRARY_PATH}*.so" "${LIBRARY_PATH}*.a")

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_MODULE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread -lallegro_monolith-static -ldumb -lFLAC
	-lfreetype -ljpeg -lopusfile -lopus -lphysfs -lpng16 -ltheoradec -lvorbis -lvorbisfile -logg -lwebp -lzlib -ldsound -lopengl32
	-luser32 -lgdi32 -lcomdlg32 -lole32 -lwinmm -lkernel32 -lpsapi -lshlwapi /MT")
	file(GLOB LIBRARIES CONFIGURE_DEPENDS "${W_LIB_PATH}*.lib")
	message("OS: Windows")
endif(WIN32)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)

set(CMAKE_CXX_FLAGS "-pthread -ldl")
link_libraries(${LIBRARIES})
set(PROJECT_LINK_LIBS ${LIBRARIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/includes/ "H:/CPP_HEADERS/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
file(GLOB_RECURSE SOURCES_CPP CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
message("Sources:" ${SOURCES_CPP})
#add_executable(Motor ${SOURCES_CPP})
add_library(Motor STATIC ${SOURCES_CPP})
target_compile_features(Motor PUBLIC cxx_std_23)
set_property(TARGET Motor PROPERTY CXX_STANDARD 23)
if (MSVC)
target_compile_options(Motor PRIVATE "/MT")
endif(MSVC)
