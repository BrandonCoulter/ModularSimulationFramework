# MSF_Core executable

core_sources = [
    'main.cpp',
    'controller/src/Controller.cpp',
]

inc_dir = include_directories(
    'controller/include',
)

executable(
    'msf_simulation',
    core_sources,
    dependencies: [msfutil_dep, msfworld_dep],
    include_directories: inc_dir,
    install: true,
)
