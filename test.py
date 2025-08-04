import mesa3d

stage = mesa3d.gl_shader_stage.COMPUTE
info = mesa3d.shader_info()
info.stage = stage
options = mesa3d.nir_shader_compiler_options()
shader = mesa3d.nir_shader_create(None, stage, options, info)
builder = mesa3d.nir_builder_init_simple_shader(stage, options, "simple")
const_index = mesa3d.nir_imm_int(builder, 3)
bit_size = mesa3d.get_nir_def_bit_size(const_index)
print(bit_size)
#print(const_index.bit_size)

#main = mesa3d.nir_function_create(shader, "main")
#main_impl = mesa3d.nir_function_impl_create(main)

#builder.impl = main_impl
#builder.cursor = mesa3d.nir_start_block(main_impl)

#mesa3d.glsl_type_singleton_init_or_ref()
#it = mesa3d.glsl_int_type()
#at = mesa3d.glsl_array_type(it, 10, 0)

#nir_var = mesa3d.nir_variable_create(shader,
#                                     mesa3d.nir_var_mem_ssbo,
#                                     at,
#                                     "ssbo_var_x")
#nir_var.data.binding = 66
#nir_var.data.explicit_binding = True

# 1. Create a variable dereference using the builder API
#var_deref = mesa3d.nir_build_deref_var(builder, nir_var)

# 3. Use the variable dereference and the index to create an array dereference
#array_deref = mesa3d.nir_build_deref_array(builder, var_deref, const_index)

# 4. Load the value from the dereferenced array element
#ssbo_def = mesa3d.nir_load_deref(builder, array_deref)

#mesa3d.nir_validate_shader(shader, None)
#mesa3d.nir_opt_constant_folding(main_impl)
#mesa3d.nir_remove_dead_variables(shader, mesa3d.nir_var_mem_all, None)
#mesa3d.nir_shader_destroy(shader)

