-- This class is just a wrapper around the compressed ByteTensor which makes 
-- the API a little less painful.

local CompressedTensor = torch.class("torch.CompressedTensor")

function CompressedTensor:__init(tensor, quality)
  assert(torch.typename(tensor):find('torch%..+Tensor'), 
    'tensor input must be a torch tensor')
  self.src_type = torch.typename(tensor)
  self.data = torch.ByteTensor()
  self.size = torch.LongStorage(tensor:size():size())  -- Size of storage
  self.size:copy(tensor:size())
  self.quality = quality or 1  -- 0, 1, 2 (for low to high compression)
  -- TODO: the src_type string above is at least a few 10s of bytes overhead
  --       --> change to an enum value

  -- Compress the tensor
  tensor.torchzlib.compress(tensor, self.data, self.quality)
end

function CompressedTensor:decompress()
  local return_tensor = torch.Tensor():type(self.src_type):resize(self.size)
  return_tensor.torchzlib.decompress(self.data, return_tensor)
  return return_tensor
end 
