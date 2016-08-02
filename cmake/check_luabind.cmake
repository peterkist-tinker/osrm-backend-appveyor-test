INCLUDE (CheckCXXSourceCompiles)
unset(LUABIND_WORKS CACHE)
unset(LUABIND51_WORKS CACHE)
set (LUABIND_CHECK_SRC "extern \"C\" {\n#include \"lua.h\"\n#include \"lauxlib.h\"\n}\n#include <luabind/open.hpp>\nint main() { lua_State *x = luaL_newstate(); luabind::open(x); }")
set (CMAKE_TRY_COMPILE_CONFIGURATION ${CMAKE_BUILD_TYPE})
set (CMAKE_REQUIRED_INCLUDES "${Boost_INCLUDE_DIR};${LUABIND_INCLUDE_DIR};${LUA_INCLUDE_DIR}")
set (CMAKE_REQUIRED_LIBRARIES "${LUABIND_LIBRARY};${LUA_LIBRARY}")

find_package(Lua52)
if(NOT APPLE)
  find_package(LuaJIT 5.2)
endif()
if(LUA52_FOUND)
  set (CMAKE_REQUIRED_INCLUDES "${Boost_INCLUDE_DIR};${LUABIND_INCLUDE_DIR};${LUA_INCLUDE_DIR}")
  set (CMAKE_REQUIRED_LIBRARIES "${LUABIND_LIBRARY};${LUA_LIBRARY}")
  CHECK_CXX_SOURCE_COMPILES("${LUABIND_CHECK_SRC}" LUABIND_WORKS)
endif()

if(LUABIND_WORKS)
  message(STATUS "Luabind/Lua5.2 combination working with ${LUA_LIBRARY}")
else()
  message(STATUS "Luabind/Lua5.2 not feasible, falling back to Lua 5.1.")
  unset(LUA_FOUND CACHE)
  unset(LUA_INCLUDE_DIR CACHE)
  unset(LUA_LIBRARY CACHE)
  find_package(Lua51 REQUIRED)
  if(NOT APPLE)
    find_package(LuaJIT 5.1)
  endif()
  set (CMAKE_REQUIRED_INCLUDES "${Boost_INCLUDE_DIR};${LUABIND_INCLUDE_DIR};${LUA_INCLUDE_DIR}")
  set (CMAKE_REQUIRED_LIBRARIES "${LUABIND_LIBRARY};${LUA_LIBRARY}")

  CHECK_CXX_SOURCE_COMPILES("${LUABIND_CHECK_SRC}" LUABIND51_WORKS)

  if(LUABIND51_WORKS)
    message(STATUS "Luabind works with Lua 5.1 at ${LUA_LIBRARY}")
  else()
    message(FATAL_ERROR	"Luabind does not work with Lua 5.1 at ${LUA_LIBRARY}, no working Luabind found")
  endif()
endif()
