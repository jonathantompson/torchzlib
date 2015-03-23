CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
CMAKE_POLICY(VERSION 2.6)

FIND_PACKAGE(Torch REQUIRED)

ADD_SUBDIRECTORY(zlib)

SET(src torchzlib.c)

FILE(GLOB luasrc *.lua)
SET(luasrc init.lua ${luasrc})

ADD_TORCH_PACKAGE(torchzlib "${src}" "${luasrc}" "Torch zlib utility library")
TARGET_LINK_LIBRARIES(torchzlib luaT TH zlib)
INSTALL(FILES "README.md" DESTINATION "${Torch_INSTALL_LUA_PATH_SUBDIR}/torchzlib")
