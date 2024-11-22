local PointLight = tdengine.entity.define('PointLight') 

PointLight.components = {
  'Collider'
}

PointLight.editor_fields = {
  'radial_falloff',
  'angular_falloff',
  'intensity',
  'color',
  'volumetric_intensity',
  'angle', 
} 

PointLight:set_field_metadatas({ 
  angle = FieldMetadata.Presets.Float_01,
  angular_falloff = FieldMetadata.Presets.Float_01,
  radial_falloff = FieldMetadata.Presets.Float_01,
  volumetric_intensity = FieldMetadata.Presets.Float_01,
  intensity = {
    slider_min = 0,
    slider_max = 100,
  },
})


function PointLight:init(params)
  self.color = tdengine.color(params.color)
  self.radial_falloff = params.radial_falloff or 0.5
  self.angular_falloff = params.angular_falloff or 0.5
  self.intensity = params.intensity or 0.5
  self.volumetric_intensity = params.volumetric_intensity or 1.0
  self.angle = params.angle or 0.0
  
  local collider = self:find_component('Collider')
  collider:set_shape(tdengine.enums.ColliderShape.Circle)
  collider.impl:set_radius(12)
end

function PointLight:to_ctype()
  local ctype = ffi.new('Light', self.color:to_ctype(), self:find_component('Collider'):get_position():to_ctype(), self.radial_falloff, self.angular_falloff, self.intensity)
  ctype.padding[0] = self.volumetric_intensity
  ctype.padding[1] = self.angle
  return ctype
end
 
function PointLight:draw()
  -- self.angle = tdengine.ffi.perlin(tdengine.elapsed_time / 2, self.id, .4, .7)
  self.angle = tdengine.math.ranged_sin(tdengine.elapsed_time * 2, .4, .7)
end