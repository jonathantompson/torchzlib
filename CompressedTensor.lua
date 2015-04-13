-- This class is just a wrapper around the compressed ByteTensor which makes 
-- the API a little less painful.

local CompressedTensor = torch.class("torch.CompressedTensor")

function CompressedTensor:__init(tensor, quality, apply_sub_filter)
  assert(torch.typename(tensor):find('torch%..+Tensor'), 
    'tensor input must be a torch tensor')
  self.apply_sub_filter = apply_sub_filter or false
  self.src_type = torch.typename(tensor)
  self.data = torch.ByteTensor()
  self.size = torch.LongStorage(tensor:size():size())  -- Size of storage
  self.size:copy(tensor:size())
  self.quality = quality or 1  -- 0, 1, 2 (for low to high compression)
  -- TODO: the src_type string above is at least a few 10s of bytes overhead
  --       --> change to an enum value

  local filt_tensor = tensor
  if self.apply_sub_filter then
    -- PNG sub filter, assume last dimension is the X dim (ie width):
    --   clone so we don't destroy the original tensor and so we can calculate 
    --   the difference image in parallel
    filt_tensor = tensor:clone()  
    local dim = filt_tensor:dim()
    local sz = filt_tensor:size(dim)
    filt_tensor:narrow(dim, 2, sz-1):add(-1, tensor:narrow(dim, 1, sz-1))
  end

  filt_tensor = filt_tensor:contiguous()
  
  -- Compress the tensor
  tensor.torchzlib.compress(filt_tensor, self.data, self.quality)
end

function CompressedTensor:decompress()
  local return_tensor = torch.Tensor():type(self.src_type):resize(self.size)
  return_tensor.torchzlib.decompress(self.data, return_tensor)
  
  if self.apply_sub_filter then
    local dim = return_tensor:dim()
    local sz = return_tensor:size(dim)
    for i = 2, sz do
      -- Here, order matters so we need to loop...
      return_tensor:narrow(dim, i, 1):add(return_tensor:narrow(dim, i-1, 1))
    end
  end
  
  return return_tensor
end 
