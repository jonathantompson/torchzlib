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

To compare libpng vs torchzlib compression ratios:

```lua
require 'torchzlib'
require 'image'

im = image.lena()
torch.save('image.bin', im)  -- Baseline
image.savePNG('image.png', im)  -- PNG compression
torch.save('image.zlib', torch.CompressedTensor(im, 2))  -- zlib compression
os.execute('gzip -c image.bin > image.bin.gz')  -- zlib compression from the command line (Linux)
```

Note: png is not just zlib.  It's also uses a preprocessing step (called delta filtering) that enables extremely high compression ratios on images once the output is passed through DEFLATE.  In the above example ```image.bin.gz``` should be approximately the same size as ```image.zlib```.
