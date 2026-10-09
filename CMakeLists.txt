cmake_minimum_required(VERSION 4.3.1)
project(vit LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable( ${PROJECT_NAME} 
    src/main.c 
    src/vit_cli_parser.c 
    src/commands/init.c 
    src/subsystems/cli_parser.c 
    src/subsystems/utils.c
    src/subsystems/linked_list.c
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

## === Replicate the Visual Studio Environment ===

target_include_directories(vit PRIVATE
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/include"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/atlmfc/include"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/UnitTest/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/winrt"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/cppwinrt"
)

set(ENV{PATH}
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/bin/HostX64/x64;C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64;C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x86;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/tools;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/ide;C:/Program Files (x86)/HTML Help Workshop;C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/amd64;C:/WINDOWS/Microsoft.NET/Framework64/v4.0.30319;C:/WINDOWS/Microsoft.NET/Framework/v4.0.30319;C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin;C:/WINDOWS/system32;C:/WINDOWS;C:/WINDOWS/System32/Wbem;C:/WINDOWS/System32/WindowsPowerShell/v1.0;C:/WINDOWS/System32/OpenSSH;C:/Program Files/NVIDIA Corporation/NVIDIA app/NvDLISR;C:/Program Files (x86)/NVIDIA Corporation/PhysX/Common;C:/Users/arish/.local/bin;C:/Program Files/nodejs;C:/Program Files/Git/cmd;C:/Program Files/NVIDIA Corporation/Nsight Compute 2025.3.1;C:/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit;C:/Program Files/Neovim/bin;C:/Program Files/Neovide;C:/Users/arish/AppData/Local/Microsoft/WindowsApps;C:/Users/arish/AppData/Local/bin/NASM;C:/Users/arish/custom_cli;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/PrivateAssemblies/runtimes/win-x64/native;$ENV{PATH}"
)

target_link_directories(vit PRIVATE
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/lib/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/atlmfc/lib/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/lib/x64"
    "C:/Program Files (x86)/Windows Kits/10/lib/10.0.26100.0/ucrt/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/UnitTest/lib"
    "C:/Program Files (x86)/Windows Kits/10/lib/10.0.26100.0/um/x64"
)

target_compile_options(vit PRIVATE
    "/AI\"C:/Program Files (x86)/Windows Kits/10/References\""
)

## === Done Replicating ===

# Add tests subdirectory which builds the GTest suite. The tests use a mock
# implementation when the real liist/list headers are not available.
# add_subdirectory(tests)
