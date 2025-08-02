import mesa3d

stage = mesa3d.gl_shader_stage.COMPUTE
info = mesa3d.shader_info()
info.stage = stage
options = mesa3d.nir_shader_compiler_options()

shader = mesa3d.nir_shader_create(None, stage, options, info)

builder = mesa3d.nir_builder_init_simple_shader(stage, options, "simple")
#builder.shader = shader

# Manually create the function and implementation
main = mesa3d.nir_function_create(shader, "main")
main_impl = mesa3d.nir_function_impl_create(main)

# Set the builder to use the new implementation
builder.impl = main_impl
#builder.cursor = mesa3d.nir_start_block(main_impl)

# Initialize GLSL types
mesa3d.glsl_type_singleton_init_or_ref()
it = mesa3d.glsl_int_type()
at = mesa3d.glsl_array_type(it, 10, 0)

# Create and configure the nir_variable for the SSBO
nir_var = mesa3d.nir_variable_create(shader,
                                     mesa3d.nir_var_mem_ssbo,
                                     at,
                                     "ssbo_var_x")
nir_var.data.binding = 66
nir_var.data.explicit_binding = True

# Now the builder and shader are in a fully consistent state
# And you can safely use nir_load_var
ssbo_def = mesa3d.nir_load_var(builder, nir_var)


mesa3d.nir_validate_shader(shader, None)
mesa3d.nir_opt_constant_folding(main_impl)
mesa3d.nir_remove_dead_variables(shader, mesa3d.nir_var_mem_all, None)
mesa3d.nir_shader_destroy(shader)

