import mesa3d

all_attributes = dir(mesa3d)
functions = [attr for attr in all_attributes if callable(getattr(mesa3d, attr))]
print(functions)

# Expected error, TODO: shader = mesa3d.nir_shader_create(4)
