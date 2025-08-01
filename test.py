import mesa3d

stage = mesa3d.gl_shader_stage.COMPUTE
options = mesa3d.nir_shader_compiler_options()
builder = mesa3d.nir_builder_init_simple_shader(stage, options, "simple")
main = mesa3d.nir_shader_get_function_for_name(builder.shader, "main");
mesa3d.nir_validate_shader(builder.shader, None);


i = mesa3d.glsl_int_type()
f = mesa3d.glsl_float_type()
