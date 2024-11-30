local self = doublenickel.time_metric

function doublenickel.time_metric.init(name)
  self.metrics = doublenickel.data_types.Array:new()
  self.metrics:add('frame') -- Created in C, so we don't need to check for its existence just for the first frame
  self.add('update')
  self.add('render')
  self.add('gc')
end

function doublenickel.time_metric.add(name)
  self.metrics:add(name)
  doublenickel.ffi.dn_time_metric_add(name)
end

function doublenickel.time_metric.query(name)
	local metrics = {
		average = doublenickel.ffi.dn_time_metric_average(name),
		last = doublenickel.ffi.dn_time_metric_last(name),
		largest = doublenickel.ffi.dn_time_metric_largest(name),
		smallest = doublenickel.ffi.dn_time_metric_smallest(name)
	}

  for key, value in pairs(metrics) do
    metrics[key] = truncate(value * 1000, 4)
  end

  return metrics
end

function doublenickel.time_metric.query_all()
  local result = {}
  for name in self.metrics:iterate_values() do
    result[name] = self.query(name)
  end

	return result
end