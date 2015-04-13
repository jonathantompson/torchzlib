require 'torchzlib'

test_types = {'torch.ByteTensor', 'torch.CharTensor', 'torch.ShortTensor', 'torch.IntTensor', 'torch.LongTensor', 'torch.FloatTensor', 'torch.DoubleTensor'}

for _, cur_type in pairs(test_types) do
  for apply_sub_filter = 0, 1 do
    for quality = 0, 2 do

      local data = torch.rand(125,126):mul(255):clamp(0,255):type(cur_type)
      local data_compressed = torch.CompressedTensor(data, quality, apply_sub_filter == 1)

      -- Make sure data integrety is maintained when serializing to / from disk
      torch.save('tmp.bin', data_compressed)
      data_compressed = torch.load('tmp.bin')
    
      local data_decompressed = data_compressed:decompress()
      assert(torch.type(data_decompressed) == cur_type)

      local err = data_decompressed - data
      if apply_sub_filter == 0 then
        assert(err:max() == 0 and err:min() == 0,  -- Lossless!  It should be exactly the same
          'failed on '..cur_type..', quality='..quality..', apply_sub_filter='..apply_sub_filter)
      else
        print(err:max())
        print(err:min())
        assert(err:max() < 1e-3 and err:min() > -1e-3,  -- Filter will introduce floating point error
          'failed on '..cur_type..', quality='..quality..', apply_sub_filter='..apply_sub_filter)
      end
    end
  end
end



print('All tests passed!')
