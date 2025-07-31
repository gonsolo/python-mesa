# meson_bindings/meson.build

project('mesa-python', 'cpp', version: '0.1.0',
    default_options: ['cpp_std=c++17'])

# --- Find Python and Pybind11 ---
python = import('python').find_installation()
pybind11_dep = dependency('pybind11', required: true)

#mesa_proj = subproject('mesa', default_options: ['default_library=static'])
mesa_proj = subproject(
    'mesa',
    default_options:  [
        'build-tests=false',
        'gallium-drivers=[]',
        'dri-drivers=[]',
        'vulkan-drivers=[]',
        'platforms=[]',
        'egl=disabled',
        'gbm=disabled',
        'opengl=false',
        'valgrind=disabled'
    ]
)
mesa_dep = mesa_proj.get_variable('idep_nir')

# Use the 'idep_nir' internal dependency provided by the subproject
#py_mod.add_unstable_dependency(mesa_proj.get_variable('idep_nir'))

# --- Check for platform features and add preprocessor defines ---
cc = meson.get_compiler('cpp')
extra_args = []

# Check for pthreads and add HAVE_PTHREAD if found
pthreads_dep = dependency('threads', required: false)
if pthreads_dep.found()
    extra_args += '-DHAVE_PTHREAD'
endif

# Check for secure_getenv and add HAVE_SECURE_GETENV if found
if cc.has_function('secure_getenv')
    extra_args += '-DHAVE_SECURE_GETENV'
endif

# Check for struct timespec and add HAVE_STRUCT_TIMESPEC if found
if cc.has_type('struct timespec', prefix : '#include <time.h>')
    extra_args += '-DHAVE_STRUCT_TIMESPEC'
endif

# --- Create the Python Extension Module ---
python.extension_module(
    'mesanir',
    'bindings.cpp',
    dependencies: [
        pybind11_dep,
        mesa_dep,
        pthreads_dep,
    ],
    cpp_args: extra_args,
    install: true,
)




