require 'torchzlib'

test_types = {'torch.ByteTensor', 'torch.CharTensor', 'torch.ShortTensor', 'torch.IntTensor', 'torch.LongTensor', 'torch.FloatTensor', 'torch.DoubleTensor'}

for _, cur_type in pairs(test_types) do
  for quality = 0, 2 do

    local data = torch.rand(125,126):mul(255):type(cur_type)
    local data_compressed = torch.CompressedTensor(data, quality)

    -- Make sure data integrety is maintained when serializing to / from disk
    torch.save('tmp.bin', data_compressed)
    data_compressed = torch.load('tmp.bin')
  
    local data_decompressed = data_compressed:decompress()
    assert(torch.type(data_decompressed) == cur_type)

    local err = data_decompressed - data
    if err.abs ~= nil then
      err = err:abs()
    end
    assert(err:max() == 0)  -- Lossless!  It should be exactly the same
  end
end

print('All tests passed!')
