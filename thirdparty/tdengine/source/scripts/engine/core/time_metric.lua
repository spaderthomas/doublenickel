local self = tdengine.time_metric

function tdengine.time_metric.init(name)
  self.metrics = tdengine.data_types.Array:new()
  self.metrics:add('frame') -- Created in C, so we don't need to check for its existence just for the first frame
  self.add('update')
  self.add('render')
  self.add('gc')
end

function tdengine.time_metric.add(name)
  self.metrics:add(name)
  tdengine.ffi.tm_add(name)
end

function tdengine.time_metric.query(name)
	local metrics = {
		average = tdengine.ffi.tm_average(name),
		last = tdengine.ffi.tm_last(name),
		largest = tdengine.ffi.tm_largest(name),
		smallest = tdengine.ffi.tm_smallest(name)
	}

  for key, value in pairs(metrics) do
    metrics[key] = truncate(value * 1000, 4)
  end

  return metrics
end

function tdengine.time_metric.query_all()
  local result = {}
  for name in self.metrics:iterate_values() do
    result[name] = self.query(name)
  end

	return result
end