import mesa3d

stage = mesa3d.gl_shader_stage.COMPUTE
options = mesa3d.nir_shader_compiler_options()
builder = mesa3d.nir_builder_init_simple_shader(stage, options, "simple")
main = mesa3d.nir_shader_get_function_for_name(builder.shader, "main");

mesa3d.glsl_type_singleton_init_or_ref()
ft = mesa3d.glsl_float_type()
it = mesa3d.glsl_int_type()
at = mesa3d.glsl_array_type(it, 10, 0)

nir_var = mesa3d.nir_variable_create(builder.shader,
                                     mesa3d.nir_var_mem_ssbo,
                                     at,
                                     f"ssbo_var_x")
nir_var.data.binding = 66
nir_var.data.explicit_binding = True
mesa3d.nir_validate_shader(builder.shader, None);

it32 = mesa3d.nir_type_int32
ft32 = mesa3d.nir_type_float32

immi = mesa3d.nir_imm_int(builder, 33)
immf = mesa3d.nir_imm_float(builder, 33.3)

var_type = nir_var.type
var_type_name = var_type.name
if var_type.is_array():
    fields_array = var_type.array
elif var_type.is_structure():
    fields_structure = var_type.structure

s = mesa3d.glsl_get_explicit_size(var_type, True)
