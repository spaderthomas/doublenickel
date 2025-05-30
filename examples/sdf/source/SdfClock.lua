SdfClockBubble = dn.class.define('SdfClockBubble')
function SdfClockBubble:init(params)
  self.sdf = SdfCircle:new({
    color = dn.colors.indian_red,
    position = Vector2:new(
      dn.math.random_float(-params.face_size, params.face_size),
      -params.face_size
    ),
    rotation = 0,
    radius = 3
  })

  self.speed = dn.math.random_float(0.04, 0.1)
  self.interpolation = dn.interpolation.SmoothDamp:new({
    start = self.sdf.radius * dn.math.random_float(0.5, 1.0),
    target = self.sdf.radius * dn.math.random_float(1.0, 1.5),
    velocity = 0.05
  })
end


SdfClock = dn.entity.define('SdfClock')
function SdfClock:init()
  self.speed = 1

  self.time = {
    second = 0,
    minute = 0,
    hour = 0
  }
  self.accumulated = 0

  self.sizes = {}
  self.sizes.face = 80
  self.sizes.border_scale = self.sizes.face * 1.1
  self.sizes.hand = {
    second = Vector2:new(2, self.sizes.face * .9),
    minute = Vector2:new(3, self.sizes.face * .8),
    hour = Vector2:new(6, self.sizes.face * .66),
  }

  self.colors = {
    face = dn.colors.white,
    border = dn.colors.white,
  }

  self.hand_interpolation = dn.interpolation.EaseInOut:new({
    start = 0,
    target = 1,
    exponent = 2,
    time = 0.5
  })

  self.sdf = app.sdf_renderer

  self:init_bubbles()
end

function SdfClock:init_bubbles()
  self.bubbles = Array:new()
  self.num_bubbles = 100
  for i = 1, self.num_bubbles do
    self.bubbles:add(SdfClockBubble:new({
      face_size = self.sizes.face
    }))
  end
end

function SdfClock:draw()
  self.colors = {
    face = dn.colors.white,
    border = dn.colors.white,
  }

  self.accumulated = self.accumulated + dn.dt * self.speed
  while self.accumulated >= 1.0 do
    self.accumulated = self.accumulated - 1.0
    self.time.second = self.time.second + 1

    if self.time.second >= 60.0  then
      self.time.second = 0
      self.time.minute = self.time.minute + 1
      if self.time.minute >= 60.0 then
        self.time.minute = self.time.minute % 60.0

        self.time.hour = (self.time.hour + 1) % 12

      end
    end

    self.hand_interpolation:reset()

  end

  self.sdf:circle_ex(
    0, 0, 
    self.colors.face.r, self.colors.face.g, self.colors.face.b,
    0.0,
    1.5,
    self.sizes.face
  )



  self.header = self.sdf:combination_begin()

  self.sdf:combination_append(
    self.header,
    ffi.C.DN_SDF_SHAPE_RING, 
    ffi.C.DN_SDF_COMBINE_OP_UNION,
    ffi.C.DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC
  )

  self.sdf:ring_ex(
    0, 0, 
    self.colors.border.r, self.colors.border.g, self.colors.border.b,
    0.0,
    1.5,
    self.sizes.face * .95, self.sizes.face * 1.1
  )

  self:update_bubbles()
  self:draw_bubbles()

  self.hand_interpolation:update()
  self:draw_hand(((self.time.second - 1) + self.hand_interpolation:get_value()) / 60.0, self.sizes.hand.second, self.colors.border)
  self:draw_hand(self.time.minute / 60.0, self.sizes.hand.minute, self.colors.border)
  self:draw_hand(self.time.hour   / 12.0, self.sizes.hand.hour,   self.colors.border)
  self.sdf:combination_commit()

  self.sdf:circle_ex(
    0, 0, 
    self.colors.border.r, self.colors.border.g, self.colors.border.b,
    0.0,
    1.5,
    4.0
  )

end

function SdfClock:draw_hand(turns, hand_size, color)
  local angle = -dn.math.turns_to_rads(turns)
  local hand_angle = angle + dn.math.pi / 2
  local hand_direction = Vector2:new(math.cos(hand_angle), math.sin(hand_angle))
  local hand_center = hand_direction:scale(hand_size.y / 2)
  
  self.sdf:combination_append(
    self.header,
    ffi.C.DN_SDF_SHAPE_ORIENTED_BOX, 
    ffi.C.DN_SDF_COMBINE_OP_UNION,
    ffi.C.DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC
  )

  self.sdf:oriented_box_ex(
    hand_center.x, hand_center.y,
    color.r, color.g, color.b,
    angle,
    1.5,
    hand_size.x, hand_size.y
  )
end

function SdfClock:update_bubbles()
  for bubble in self.bubbles:iterate_values() do
    bubble.sdf.header.position.y = bubble.sdf.header.position.y + bubble.speed
    if (bubble.sdf.header.position.y - bubble.sdf.radius) > app.native_resolution.y / 2 then
      bubble.sdf.header.position.y = -app.native_resolution.y / 2 - bubble.sdf.radius
    end

    if bubble.interpolation:update() then
      bubble.interpolation:reverse()
      bubble.interpolation:reset()
    end
    bubble.sdf.radius = bubble.interpolation:get_value()
  end
end

function SdfClock:draw_bubbles()
  for bubble in self.bubbles:iterate_values() do
    self.sdf:combination_append(
      self.header,
      ffi.C.DN_SDF_SHAPE_CIRCLE, 
      ffi.C.DN_SDF_COMBINE_OP_UNION,
      ffi.C.DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC
    )

    local sdf = bubble.sdf
    self.sdf:circle_ex(
      sdf.header.position.x, sdf.header.position.y,
      sdf.header.color.x, sdf.header.color.y, sdf.header.color.z,
      sdf.header.rotation,
      sdf.header.edge_thickness,
      sdf.radius
    )
  end
end