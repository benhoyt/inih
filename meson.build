project('inih',
    ['c','cpp'],
    meson_version : '>= 0.46.0',
    default_options : ['default_library=both'],
    license : 'BSD-3-Clause',
    version : '47'
)

pkg = import('pkgconfig')

#### inih ####
install_headers('ini.h')

lib_inih = library('inih',
    ['ini.c', 'ini.h'],
    install : true,
    version : meson.project_version(),
    soversion : '1'
)

pkg.generate(lib_inih,
    name : 'inih',
    description : 'simple .INI file parser',
    version : meson.project_version()
)

inih_dep = declare_dependency(link_with : lib_inih)

#### INIReader ####
install_headers('cpp/INIReader.h')

lib_INIReader = library('INIReader',
    ['cpp/INIReader.cpp', 'cpp/INIReader.h'],
    dependencies : inih_dep,
    install : true,
    version : meson.project_version(),
    soversion : '1'
)

pkg.generate(lib_INIReader,
    name : 'INIReader',
    description : 'simple .INI file parser for C++',
    version : meson.project_version()
)

INIReader_dep = declare_dependency(link_with : lib_inih)
