# MSF_Utilities library

# Math submodule
math_sources = [
]

# Include directory for math headers
math_inc = include_directories('math')

# Create the utilities library (header-only for now)
msfutil_lib = library(
    'msfutil',
    math_sources,
    include_directories: math_inc,
    install: true,
)

# Export for other projects to use
msfutil_dep = declare_dependency(
    link_with: msfutil_lib,
    include_directories: math_inc,
)
