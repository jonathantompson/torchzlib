# -*- cmake -*-

set(VERSION "1.2.8")

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

enable_testing()

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)


set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein
		${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR})



SET(src 
     adler32.c
     crc32.c
     infback.c
     inflate.c
     uncompr.c
     compress.c
     deflate.c
     gzclose.c
     gzlib.c
     gzread.c
     gzwrite.c
     inffast.c
     inftrees.c
     trees.c
     zutil.c)

ADD_LIBRARY(zlib SHARED ${src})

IF(WIN32)
  ADD_DEFINITIONS(-DZLIB_DLL)
ENDIF(WIN32)

# To compile examples
# ADD_EXECUTABLE(example example.c)
# TARGET_LINK_LIBRARIES(example zlib)
#
# ADDZZ_EXECUTABLE(minigzip minigzip.c)
# TARGET_LINK_LIBRARIES(minigzip zlib)

INSTALL(TARGETS zlib 
  LIBRARY DESTINATION "${Torch_INSTALL_LIB_SUBDIR}"
  ARCHIVE DESTINATION "${Torch_INSTALL_LIB_SUBDIR}"
  RUNTIME DESTINATION "${Torch_INSTALL_BIN_SUBDIR}")

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/zconf.h zlib.h 
  DESTINATION "${Torch_INSTALL_INCLUDE_SUBDIR}")

