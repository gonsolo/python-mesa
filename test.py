import mesa3d

stage = mesa3d.gl_shader_stage.COMPUTE
options = mesa3d.nir_shader_compiler_options()
builder = mesa3d.nir_builder_init_simple_shader(stage, options, "simple")
main = mesa3d.nir_shader_get_function_for_name(builder.shader, "main");

mesa3d.glsl_type_singleton_init_or_ref()
i = mesa3d.glsl_int_type()
f = mesa3d.glsl_float_type()
it = mesa3d.glsl_int_type()
at = mesa3d.glsl_array_type(it, 10, 0)

nir_var = mesa3d.nir_variable_create(builder.shader,
                                     mesa3d.nir_var_mem_ssbo,
                                     at,
                                     f"ssbo_var_x")
nir_var.data.binding = 66
nir_var.data.explicit_binding = True
mesa3d.nir_validate_shader(builder.shader, None);
