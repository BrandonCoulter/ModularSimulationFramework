# MSF_Core executable

core_sources = [
    'main.cpp',
    'controller/src/Controller.cpp',
    'controller/src/IO/XMLParser.cpp',
    'controller/src/IO/SCF.cpp',
]

inc_dir = include_directories(
    'controller/include',
    'controller/include/IO',
)

executable(
    'msf_simulation',
    core_sources,
    dependencies: [msfutil_dep, msfworld_dep],
    include_directories: inc_dir,
    install: true,
)
