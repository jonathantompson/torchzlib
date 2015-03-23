torchzlib
=========

A utility library for zlib compression / decompression of Torch7 tensors.

The main (and only) API entry point is a new class ```torch.CompressedTensor```.  This is a super simple class that creates a compressed ByteTensor of an input tensor (using zlib ```deflate```) and has a single ```decompress()``` method to return the original data.

As per the zlib library, all compression is lossless.

Usage:

```lua
require 'torchzlib'

data = torch.rand(5,5):double() -- Can be any other tensor
data_compressed = torch.CompressedTensor(data)  -- Compress using default compression level (1)

-- Alternatively:
data_compressed = torch.CompressedTensor(data, 0)  -- Compress using fast compression (low compression ratio)
data_compressed = torch.CompressedTensor(data, 2)  -- Compress using slow compression (high compression ratio)

-- Do whatever you want in here (including saving and loading data_compressed to file)

data_decompressed = data_compressed:decompress()
```
