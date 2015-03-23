#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/torchzlib.c"
#else

#undef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a) : (b) )

#undef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a) : (b) )

#undef TAPI
#define TAPI __declspec(dllimport)

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#include <stdint.h>
#include "zlib/zlib.h"

static int torchzlib_(Main_compress)(lua_State *L) {
  THTensor *Tsrc = luaT_checkudata(L, 1, torch_Tensor);
  real *src_data = THTensor_(data)(Tsrc);
  int64_t src_data_size = (int64_t)THTensor_(nElement)(Tsrc) * (int64_t)sizeof(src_data[0]);
  
  THByteTensor* tensor_dest = luaT_checkudata(L, 2, "torch.ByteTensor");
  int quality = luaL_checkint(L, 3);
  
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.next_in = (uint8_t *)(src_data);
  strm.avail_in = src_data_size;
  
  int compression;
  switch (quality) {
  case 0:
    compression = Z_BEST_SPEED;
    break;
  case 1:
    compression = Z_DEFAULT_COMPRESSION;
    break;
  case 2:
    compression = Z_BEST_COMPRESSION;
    break;
  default:
    luaL_error(L, "quality must be 0, 1, or 2");
  }
  
  if (deflateInit(&strm, compression) != Z_OK) {
    luaL_error(L, "cannot open zlib deflate stream");
  }
  
  /* Allocate the maximum possible compressed array (actually larger than the input).  We COULD compress and decompress in chunks, but then we need an STL like Vector implementation in C and the worst case is that we would be allocating this much anyway.  So just compress into a large temp buffer (that is likely too big) than store only the compressed size. */
  
  int temp_buffer_size = deflateBound(&strm, src_data_size);
  uint8_t* temp_buffer = (uint8_t*)malloc(temp_buffer_size);
  
  strm.next_out = temp_buffer;
  strm.avail_out = temp_buffer_size;
  
  int res = deflate(&strm, Z_FINISH);
  
  if (res != Z_STREAM_END) {
    luaL_error(L, "zlib deflate failed! Error code: %d", res);
  }
  
  if (strm.avail_in != 0) {
    luaL_error(L, "zlib deflate failed! Bytes left in the input stream: %d", strm.avail_in);
  }

  int64_t nret_size = strm.total_out;
  deflateEnd(&strm);
  
  THByteTensor_resize1d(tensor_dest, nret_size);
  unsigned char* tensor_dest_data = THByteTensor_data(tensor_dest); 
  memcpy(tensor_dest_data, temp_buffer, nret_size);
  
  free(temp_buffer);
  
  return 0;
}

static int torchzlib_(Main_decompress)(lua_State *L) {
  THTensor *Tdst = luaT_checkudata(L, 2, torch_Tensor);
  real *dst_data = THTensor_(data)(Tdst);
  /* Here I assume that the correct output size has already been allocated.  We will handle this on the lua end */ 
  int64_t dst_data_size = (int64_t)THTensor_(nElement)(Tdst) * (int64_t)sizeof(dst_data[0]);
  
  THByteTensor* tensor_src = luaT_checkudata(L, 1, "torch.ByteTensor");
  int64_t src_data_size = (int64_t)THByteTensor_nElement(tensor_src);
  unsigned char* src_data = THByteTensor_data(tensor_src);
  
  z_stream strm ={0};
  strm.total_in = strm.avail_in = src_data_size;
  strm.total_out = strm.avail_out = dst_data_size;
  strm.next_in = src_data;
  strm.next_out= (uint8_t*)dst_data;
  
  if (inflateInit(&strm) != Z_OK) {
    luaL_error(L, "cannot open zlib inflate stream");
  }
  
  int res = inflate(&strm, Z_FINISH);
  
  if (res != Z_STREAM_END) {
    luaL_error(L, "zlib deflate failed! Error code: %d", res);
  }
  
  if (strm.avail_in != 0) {
    luaL_error(L, "zlib inflate failed! Bytes left in the input stream: %d", strm.avail_in);
  }
  
  if (strm.total_out != dst_data_size) {
    luaL_error(L, "zlib inflate failed! Incorrect return size: %d (expecting %d)", strm.total_out, dst_data_size);
  }

  inflateEnd(&strm);
  
  return 0;
}

static const struct luaL_Reg torchzlib_(Main__) [] = {
  {"compress", torchzlib_(Main_compress)},
  {"decompress", torchzlib_(Main_decompress)},
  {NULL, NULL}
};

void torchzlib_(Main_init)(lua_State *L)
{
  luaT_pushmetatable(L, torch_Tensor);
  luaT_registeratname(L, torchzlib_(Main__), "torchzlib");
}

#endif
