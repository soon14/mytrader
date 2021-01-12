#cmake_minimum_required(VERSION "2.8.1")
cmake_minimum_required(VERSION 3.4.1)

MESSAGE(STATUS "This is zqapp PROJECT BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is zqapp CMAKE BINARY dir " ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "This is zqapp PROJECT SOURCE dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is zqapp CMAKE SOURCE dir " ${CMAKE_SOURCE_DIR})

MESSAGE(STATUS "This is zqapp current BINARY dir " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "This is zqapp current SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is zqapp current CMAKE SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is zqapp current CMakeList.txt dir " ${CMAKE_CURRENT_LIST_DIR})
MESSAGE(STATUS "This is zqapp current CMakeList.txt line " ${CMAKE_CURRENT_LIST_LINE})

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
MESSAGE(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

IF(CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-D_DEBUG)
ENDIF()

if (MSVC)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(MSVC)

# add location of platform.hpp for Windows builds
if(WIN32)
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_definitions(-DWIN32 -D_WINDOWS)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE=1)
  add_definitions(-D_CRT_NON_CONFORMING_SWPRINTFS=1)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS=1)
  add_definitions(-D__WXMSW__)
  add_definitions(-DNOPCH)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  # Same name on 64bit systems
  link_libraries(ws2_32.lib)
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
endif()

IF(WIN32)
	#需要兼容XP时,定义_WIN32_WINNT 0x0501
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0602)
	ADD_DEFINITIONS(-DLIB_ZQDB_API)
	ADD_DEFINITIONS(-DLIB_CALC_API)
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
	#SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
ELSE()
	ADD_DEFINITIONS(-fpermissive)
	ADD_DEFINITIONS(-fPIC)
	#-w的意思是关闭编译时的警告，也就是编译后不显示任何warning
	#-Wall选项意思是编译后显示所有警告
	#-W选项类似-Wall，会显示警告，但是只显示编译器认为会出现错误的警告
	#调试信息格式主要有下面几种：stabs，COFF，PE-COFF，OMF，IEEE-695和DWARF
	#其中DWARF在Linux中被普遍使用，dwarf2对dwarf1的改变很大，dwarf3大多是对dwarf2的扩充，可以支持C、C++、JAVA、Fortran等语言
	#使用readelf –w* transfer命令，*是调试节名的第一个字母，如-wi就是查看.debug_info节的内容，-wl就是查看.debug_line节的内容
	#-g、-ggdb、-g3和-ggdb3，-g产生OS native format的debug信息，GDB可以使用之。而-ggdb产生的debug信息更倾向于给GDB使用的
	#如果你用的GDB调试器，那么使用-ggdb选项。如果是其他调试器，则使用-g。3只是debug信息级别，3这个级别可以调试宏。
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -w -gdwarf-2 -ggdb3")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -DNODEBUG -gdwarf-2 -ggdb")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -W -g3")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -W")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -W -gdwarf-3 -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -W")
  	INCLUDE_DIRECTORIES(/usr/local/include)
	LINK_DIRECTORIES(/usr/local/lib)
ENDIF()
CMAKE_POLICY(SET CMP0015 NEW)

#设置需要的Boost组件
#SET(BOOST_COMPONENTS log_setup log regex date_time chrono system filesystem thread)
#使用cmake find_package 查找boost库位置
#FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
FIND_PACKAGE(Boost REQUIRED)
IF(Boost_FOUND)
	MESSAGE(STATUS "Boost library status:")
	MESSAGE(STATUS "     version: ${Boost_VERSION}")
ELSE()
	#MESSAGE(FATAL_ERROR "BOOST library not found")
	if(BOOST_ROOT)
		set(Boost_INCLUDE_DIRS ${BOOST_ROOT})
	else()
		set(Boost_INCLUDE_DIRS ../../boost_1_66_0)
	endif()
ENDIF()
IF(WIN32)
  	SET(Boost_LIBRARY_DIRS "$ENV{BOOST_ROOT}/lib64")
ELSE()
	set(Boost_LIBRARY_DIRS $ENV{BOOST_ROOT}/output/stage/linux/lib)
	#使用log动态库需要定义BOOST_LOG_DYN_LINK
  	#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
	FIND_LIBRARY(BOOST_LOG_SETUP boost_log_setup ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_LOG boost_log ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_REGEX boost_regex ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_DATE_TIME boost_date_time ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_CHRONO boost_chrono ${Boost_LIBRARY_DIRS})
	SET (EXTRA_LIBS ${EXTRA_LIBS} ${BOOST_LOG_SETUP} ${BOOST_LOG} ${BOOST_REGEX} ${BOOST_DATE_TIME} ${BOOST_CHRONO}
		boost_locale boost_system boost_filesystem boost_serialization boost_thread -pthread dl)
ENDIF()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
MESSAGE(STATUS "boost include path: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "boost library path: ${Boost_LIBRARY_DIRS}")

find_package(Protobuf REQUIRED)
IF(PROTOBUF_FOUND)
	MESSAGE(STATUS "protobuf library status:")
	MESSAGE(STATUS "     version: ${PROTOBUF_VERSION}")
	MESSAGE(STATUS "     include path: ${PROTOBUF_INCLUDE_DIR}")
	MESSAGE(STATUS "     library path: ${PROTOBUF_LIBRARIES}")
	INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
  	LINK_DIRECTORIES(${PROTOBUF_INCLUDE_DIR}/../${CMAKE_BUILD_TYPE}/lib)
	SET(EXTRA_LIBS ${EXTRA_LIBS} ${PROTOBUF_LIBRARIES})
ELSE()
	MESSAGE(FATAL_ERROR "protobuf library not found")
ENDIF()
#target_link_libraries(mdb PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

IF(WIN32)
	INCLUDE_DIRECTORIES(../../../local ../../../local/x64-windows-static/include ../../mdb/include ../include ../src
	D:/wxWidgets/wxWidgets-3.1.3/lib/vc_lib/mswud D:/wxWidgets/wxWidgets-3.1.3/include 
	)
	LINK_DIRECTORIES(../../../local/x64-windows-static/lib ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${PLATFORM} D:/wxWidgets/wxWidgets-3.1.3/lib/vc_x64_lib)
	
	#IF(CMAKE_BUILD_TYPE STREQUAL Debug)
	SET (EXTRA_LIBS ${EXTRA_LIBS} wxmsw31ud_html.lib wxmsw31ud_aui.lib wxmsw31ud_ribbon.lib wxmsw31ud_core.lib wxbase31ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib)
	#ELSE()
	#SET (EXTRA_LIBS ${EXTRA_LIBS} wxmsw31u_html.lib wxmsw31u_aui.lib wxmsw31u_ribbon.lib wxmsw31u_core.lib wxbase31u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib)
	#ENDIF()

	SET (EXTRA_LIBS ${EXTRA_LIBS} mdb zq calc view
	#mysqlcppconn 
	#jwtd libeay32 ssleay32
	kernel32.lib
	user32.lib
	gdi32.lib
	comdlg32.lib
	winspool.lib
	winmm.lib
	shell32.lib
	shlwapi.lib
	comctl32.lib
	ole32.lib
	oleaut32.lib
	uuid.lib
	rpcrt4.lib
	advapi32.lib
	version.lib
	wsock32.lib
	wininet.lib
	)
ELSE()
	INCLUDE_DIRECTORIES(../../local ../../local/include ../../mdb/include ../include ../src)
	LINK_DIRECTORIES(../../local/lib64 ../../local/lib ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${PLATFORM})
	SET (EXTRA_LIBS ${EXTRA_LIBS} mdb zq calc view
	#mysqlcppconn jwt 
	#crypto ssl 
	pthread dl
	)
ENDIF()

# 添加可执行文件
# ADD_EXECUTABLE(zqapp
# base.cpp
# utility.cpp
# settings.cpp
# object.cpp
# app.cpp
# frame.cpp
# child.cpp
# splitter.cpp
# view.cpp
# smartkb.cpp
# datamodel.cpp
# dataview.cpp
# SmartKBDlg.cpp
# taskbar.cpp
# sample.rc
# )
# TARGET_LINK_LIBRARIES(zqapp ${EXTRA_LIBS})

ADD_EXECUTABLE(mytrader
base.cpp
utility.cpp
settings.cpp
object.cpp
smartkb.cpp
myapp.cpp
myframe.cpp
mytaskbar.cpp
mydatamodel.cpp
mydataview.cpp
mysmartkbdlg.cpp
sample.rc
)
TARGET_LINK_LIBRARIES(mytrader PRIVATE ${EXTRA_LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${PLATFORM})



