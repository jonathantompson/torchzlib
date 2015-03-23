torchzlib
=========

A utility library for zlib compression / decompression of Torch7 tensors.

The main (and only) API entry point is a new torch class CompressedTensor.  This is a super simple class that creates a compressed representation of an input tensor (using zlib ```deflate```) and has a single ```decompress()``` method to return the original tensor.

As per the zlib library, all compression is lossless.

Usage:

```lua
require 'torchzlib'

data = torch.rand(5,5):double() -- Can be any other tensor
data_compressed = torch.CompressedTensor(data)

-- Do whatever you want in here (including saving and loading data_compressed to file)

data_decompressed = data_compressed:decompress()
```
