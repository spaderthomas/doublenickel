return {
  ["68f555b4-6475-45cb-a53e-63fd9738fe0b"] = {
    bounding_volume = {
      a = {
        x = 400,
        y = 450
      },
      b = {
        x = 500,
        y = 550
      },
      radius = 50
    },
    fluid = {
      dt = 0.016666666666666666,
      gravity = 20,
      particle_mass = 1,
      pressure = 100,
      smoothing_radius = 4,
      viscosity = 50
    },
    name = "LagrangianFluidSystem",
    num_particles = 5000,
    start_disabled = true,
    uuid = "68f555b4-6475-45cb-a53e-63fd9738fe0b"
  },
  ["116f773c-21f6-42a8-a963-32c001266df4"] = {
    name = "FluidSimulation",
    uuid = "116f773c-21f6-42a8-a963-32c001266df4"
  },
  ["8544db91-175a-4b9e-bfb9-9477ec76c644"] = {
    grid_size = 50,
    name = "EulerianFluidSystem",
    render_size = 1000,
    start_disabled = true,
    uuid = "8544db91-175a-4b9e-bfb9-9477ec76c644"
  }
}