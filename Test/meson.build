test_inc = include_directories(
    '../MSF_Core/controller/include',
    '../MSF_Core/controller/include/IO',
    '../MSF_World/models/include',
    '../MSF_World/models/missile/include',
    '../MSF_World/models/waypoint/include',
    '../MSF_World/environment/include',
    '../MSF_Utilities/math',
)

argparse_unit_test = executable(
    'test_argparse',
    [
        'unit/test_argparse.cpp',
        '../MSF_Core/controller/src/IO/ArgParse.cpp',
    ],
    dependencies: [msfutil_dep, msfworld_dep],
    include_directories: test_inc,
)

test(
    'argparse_unit',
    argparse_unit_test,
)

scf_integration_test = executable(
    'test_scf_integration',
    [
        'unit/test_scf_integration.cpp',
        '../MSF_Core/controller/src/IO/XMLParser.cpp',
        '../MSF_Core/controller/src/IO/SCF.cpp',
    ],
    dependencies: [msfutil_dep, msfworld_dep],
    include_directories: test_inc,
)

test(
    'scf_basic_to_50s_integration',
    scf_integration_test,
    env: ['MSF_SOURCE_ROOT=' + meson.project_source_root()],
)
