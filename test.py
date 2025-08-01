import ctypes
import mesa3d

mem_ctx = mesa3d.ralloc_context(None)
stage = mesa3d.gl_shader_stage.COMPUTE
options = mesa3d.nir_shader_compiler_options()
si = mesa3d.shader_info()
si.stage = stage
shader = mesa3d.nir_shader_create(mem_ctx, stage, options, si)
function = mesa3d.nir_function_create(shader, "f")

mesa3d.ralloc_free(mem_ctx)
