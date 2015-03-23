
#include <TH.h>
#include <luaT.h>

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_Tensor TH_CONCAT_STRING_3(torch., Real, Tensor)
#define torchzlib_(NAME) TH_CONCAT_3(torchzlib_, Real, NAME)

#ifdef max
#undef max
#endif
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )

#ifdef min
#undef min
#endif
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )

#include "generic/torchzlib.c"
#include "THGenerateAllTypes.h"

DLL_EXPORT int luaopen_libtorchzlib(lua_State *L)
{
  torchzlib_ByteMain_init(L);
  torchzlib_CharMain_init(L);
  torchzlib_ShortMain_init(L);
  torchzlib_IntMain_init(L);
  torchzlib_LongMain_init(L);
  torchzlib_FloatMain_init(L);
  torchzlib_DoubleMain_init(L);

  luaL_register(L, "torchzlib.byte", torchzlib_ByteMain__);
  luaL_register(L, "torchzlib.char", torchzlib_CharMain__);
  luaL_register(L, "torchzlib.short", torchzlib_ShortMain__);
  luaL_register(L, "torchzlib.int", torchzlib_IntMain__);
  luaL_register(L, "torchzlib.long", torchzlib_LongMain__);
  luaL_register(L, "torchzlib.float", torchzlib_FloatMain__);
  luaL_register(L, "torchzlib.double", torchzlib_DoubleMain__); 
  
  return 1;
}
