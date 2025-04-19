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
  dn.ffi.time_metrics_add(name)
end

function doublenickel.time_metric.begin(name)
  dn.ffi.time_metric_begin(String:new(name))
end

function doublenickel.time_metric.stop(name)
  dn.ffi.time_metric_end(String:new(name))
end

function doublenickel.time_metric.query(name)
	local metrics = {
		average = dn.ffi.time_metric_average(name),
		last = dn.ffi.time_metric_last(name),
		largest = dn.ffi.time_metric_largest(name),
		smallest = dn.ffi.time_metric_smallest(name)
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