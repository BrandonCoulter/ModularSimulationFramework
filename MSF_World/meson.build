# MSF_World library

# World sources
world_sources = [
    'models/src/Entity.cpp',
    'models/src/PhysicsEntity.cpp',
]

# Include directories for headers
world_inc = include_directories(
    'models/include',
    'models/missile/include',
    'models/waypoint/include',
    'environment/include',
)

# Create the world library
msfworld_lib = library(
    'msfworld',
    world_sources,
    dependencies: [msfutil_dep],
    include_directories: world_inc,
    install: true,
)

# Export for other projects to use
msfworld_dep = declare_dependency(
    link_with: msfworld_lib,
    dependencies: [msfutil_dep],
    include_directories: world_inc,
)