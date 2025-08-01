import ctypes
import mesa3d

all_attributes = dir(mesa3d)
functions = [attr for attr in all_attributes if callable(getattr(mesa3d, attr))]
print(functions)

mem_ctx = mesa3d.ralloc_context(None)
print(mem_ctx)
mesa3d.ralloc_free(mem_ctx)
#shader = mesa3d.nir_shader_create(mem_ctx)
