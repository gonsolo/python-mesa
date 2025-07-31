import mesanir

all_attributes = dir(mesanir)
functions = [attr for attr in all_attributes if callable(getattr(mesanir, attr))]
print(functions)

# Expected error, TODO: shader = mesanir.nir_shader_create(4)
