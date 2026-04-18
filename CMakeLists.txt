cmake_minimum_required(VERSION "3.25" FATAL_ERROR)

set(CMAKE_UTIL_FILE "" CACHE STRING "...")
set(USE_CONAN "1" CACHE BOOL "...")

message(STATUS "USE_CONAN: '${USE_CONAN}'")

string(REPLACE "." ";" PRESET_NAME_ELEMENTS "${PRESET_NAME}")
if("${USE_CONAN}")
    message(STATUS "USE_CONAN: '${USE_CONAN}'")
    if("windows" IN_LIST "PRESET_NAME_ELEMENTS")
        find_program(CONAN_COMMAND NAMES "conan.exe" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
    else()
        find_program(CONAN_COMMAND NAMES "conan" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
    endif()
    message(STATUS "CONAN_COMMAND: '${CONAN_COMMAND}'")
endif()

if("${CMAKE_UTIL_FILE}" STREQUAL "")
    if("$ENV{CMAKE_UTIL_FILE}" STREQUAL "")
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake")
            if("${USE_CONAN}")
                #[[execute_process(
                    COMMAND "${CONAN_COMMAND}" "user" "$ENV{CONAN_LOGIN_USERNAME}" "-r" "artifactory" "-p"
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                    COMMAND_ECHO "STDOUT"
                    ENCODING "UTF-8"
                    COMMAND_ERROR_IS_FATAL "ANY"
                )]]
                execute_process(
                    COMMAND "${CONAN_COMMAND}" "install" "--tool-requires=exqudens-cmake/0.0.1" "--deployer" "direct_deploy" "--deployer-folder" "${CMAKE_SOURCE_DIR}/build/dependencies"
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                    COMMAND_ECHO "STDOUT"
                    ENCODING "UTF-8"
                    COMMAND_ERROR_IS_FATAL "ANY"
                )
                set(CMAKE_UTIL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake" CACHE STRING "..." FORCE)
            else()
                message(FATAL_ERROR "Unable to resolve CMAKE_UTIL_FILE: '${CMAKE_UTIL_FILE}'")
            endif()
        else()
            set(CMAKE_UTIL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake" CACHE STRING "..." FORCE)
        endif()
    else()
        set(CMAKE_UTIL_FILE "$ENV{CMAKE_UTIL_FILE}" CACHE STRING "..." FORCE)
    endif()
endif()
message(STATUS "CMAKE_UTIL_FILE: '${CMAKE_UTIL_FILE}'")

include("${CMAKE_UTIL_FILE}")

if("msvc-x64-x64" IN_LIST "PRESET_NAME_ELEMENTS")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/msvc-x64-x64-toolchain.cmake")
        set_msvc_toolchain(TOOLCHAIN_CONTENT
            PROCESSOR "x86_64"
            OS "${CMAKE_HOST_SYSTEM_NAME}"
            VERSION "17"
            HOST "x64"
            TARGET "x64"
            OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/msvc-x64-x64-toolchain.cmake"
        )
    endif()
elseif("gcc" IN_LIST PRESET_NAME_ELEMENTS)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-toolchain.cmake")
        find_file(COMPILER_FILE NAMES "gcc" PATHS ENV GCC_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
        set_gnu_toolchain(TOOLCHAIN_CONTENT
            PROCESSOR "x86_64"
            OS "${CMAKE_HOST_SYSTEM_NAME}"
            PATH "${COMPILER_FILE}"
            OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-toolchain.cmake"
        )
    endif()
elseif("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-arm-toolchain.cmake")
        find_file(COMPILER_FILE NAMES "arm-none-eabi-gcc.exe" "arm-none-eabi-gcc" PATHS ENV GCC_ARM_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
        set_gnu_toolchain(TOOLCHAIN_CONTENT
            PROCESSOR "armv7"
            OS "Generic"
            PATH "${COMPILER_FILE}"
            OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-arm-toolchain.cmake"
        )
    endif()
endif()
message(STATUS "CMAKE_TOOLCHAIN_FILE: '${CMAKE_TOOLCHAIN_FILE}'")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/name-version.txt" INFO)
string(REPLACE ":" ";" INFO_LIST "${INFO}")
list(GET INFO_LIST 0 RAW_NAME)
list(GET INFO_LIST 1 RAW_VERSION)
string(STRIP "${RAW_NAME}" NAME)
string(STRIP "${RAW_VERSION}" VERSION)
project("${NAME}" VERSION "${VERSION}" LANGUAGES "NONE")

message(STATUS "project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

if("$ENV{CLION_IDE}")
    cmake_path(CONVERT "$ENV{PATH}" TO_CMAKE_PATH_LIST TMP_CMAKE_ENV_PATH NORMALIZE)
    list(REMOVE_AT TMP_CMAKE_ENV_PATH 0)
    cmake_path(CONVERT "${TMP_CMAKE_ENV_PATH}" TO_NATIVE_PATH_LIST TMP_NATIVE_ENV_PATH NORMALIZE)
    set("ENV{PATH}" "${TMP_NATIVE_ENV_PATH}")
endif()

enable_language("C")
enable_language("CXX")
if("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    enable_language("ASM")
endif()
include("GenerateExportHeader")
include("CMakePackageConfigHelpers")
enable_testing()
include("GoogleTest")

if("${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT}")
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/cmake-install" CACHE PATH "..." FORCE)
endif()
set(CMAKE_VERBOSE_MAKEFILE "1" CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS "1" CACHE BOOL "" FORCE)

cmake_path(RELATIVE_PATH "PROJECT_BINARY_DIR" BASE_DIRECTORY "${PROJECT_SOURCE_DIR}" OUTPUT_VARIABLE "PROJECT_BINARY_DIR_REL")
cmake_path(CONVERT "$ENV{PATH}" TO_CMAKE_PATH_LIST "ENV_PATH" NORMALIZE)
set(CMAKE_OBJECT_PATH_MAX "1000")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_STAGING_PREFIX "")
set(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
set(CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH "0")
set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH "0")
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH "1")
set(CMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY "0")
set(CMAKE_FIND_USE_PACKAGE_REGISTRY "0")
set(CMAKE_FIND_USE_PACKAGE_ROOT_PATH "0")
set(CMAKE_FIND_USE_CMAKE_PATH "1")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG "0")
set(CMAKE_BUILD_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_SKIP_BUILD_RPATH "1")
set(CMAKE_BUILD_RPATH_USE_ORIGIN "1")
set(CMAKE_BUILD_WITH_INSTALL_RPATH "1")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH "0")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "0")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden") # "default" or "hidden"
set(CMAKE_VISIBILITY_INLINES_HIDDEN "1")
set(CMAKE_C_STANDARD "11")
set(CMAKE_C_STANDARD_REQUIRED "1")
set(CMAKE_C_EXTENSIONS "1")
if("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    set(CMAKE_CXX_STANDARD "17")
    set(CMAKE_CXX_STANDARD_REQUIRED "1")
    set(CMAKE_CXX_EXTENSIONS "1")
    set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
    set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
else()
    set(CMAKE_CXX_STANDARD "20")
    set(CMAKE_CXX_STANDARD_REQUIRED "1")
    set(CMAKE_CXX_EXTENSIONS "0")
endif()

if("msvc-x64-x64" IN_LIST "PRESET_NAME_ELEMENTS" OR "msvc-x64-x64" IN_LIST "PRESET_NAME_ELEMENTS")
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} Ws2_32.lib")
    separate_arguments(CMAKE_CXX_FLAGS NATIVE_COMMAND "${CMAKE_CXX_FLAGS}")
    if(NOT "/EHa" IN_LIST CMAKE_CXX_FLAGS AND "/EHsc" IN_LIST CMAKE_CXX_FLAGS)
        list(REMOVE_ITEM CMAKE_CXX_FLAGS "/EHsc")
        list(APPEND CMAKE_CXX_FLAGS "/EHa")
    endif()
    string(JOIN " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -D_WINDOWS")
    foreach(v IN LISTS "CMAKE_C_STANDARD_INCLUDE_DIRECTORIES")
        cmake_path(NATIVE_PATH "v" NORMALIZE "vn")
        set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -I \"${vn}\"")
    endforeach()
endif()

separate_arguments(CMAKE_CXX_FLAGS NATIVE_COMMAND "${CMAKE_CXX_FLAGS}")
if(MSVC)
    if(NOT "/EHa" IN_LIST CMAKE_CXX_FLAGS AND "/EHsc" IN_LIST CMAKE_CXX_FLAGS)
        list(REMOVE_ITEM CMAKE_CXX_FLAGS "/EHsc")
        list(APPEND CMAKE_CXX_FLAGS "/EHa")
    endif()
endif()
string(JOIN " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if("${BUILD_SHARED_LIBS}")
    if("${MINGW}")
        set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
    endif()
endif()

set(SKIP_TEST "0" CACHE BOOL "...")
message(STATUS "SKIP_TEST: '${SKIP_TEST}'")

set(IDE_TARGET_DEPENDS_ON "${PROJECT_NAME}")
message(STATUS "IDE_TARGET_DEPENDS_ON: '${IDE_TARGET_DEPENDS_ON}'")

if("${USE_CONAN}")
    set(CONAN_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/conan-install")

    file(MAKE_DIRECTORY "${CONAN_INSTALL_PREFIX}/bin")
    file(MAKE_DIRECTORY "${CONAN_INSTALL_PREFIX}/lib")

    file(MAKE_DIRECTORY "${CONAN_INSTALL_PREFIX}/test/bin")
    file(MAKE_DIRECTORY "${CONAN_INSTALL_PREFIX}/test/lib")

    set_conan_architecture(CONAN_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    set_conan_os(CONAN_OS "${CMAKE_SYSTEM_NAME}")
    set_conan_compiler(CONAN_COMPILER "${CMAKE_CXX_COMPILER_ID}")
    set_conan_compiler_version(CONAN_COMPILER_VERSION "${CMAKE_CXX_COMPILER_ID}" "${CMAKE_CXX_COMPILER_VERSION}" MAX_ELEMENTS "1")
    set_conan_compiler_runtime(CONAN_COMPILER_RUNTIME "${CMAKE_MSVC_RUNTIME_LIBRARY}")
    set_conan_compiler_runtime_type(CONAN_COMPILER_RUNTIME_TYPE "${CMAKE_MSVC_RUNTIME_LIBRARY}")

    set_python_boolean(CONAN_INTERFACE_ONLY "${INTERFACE_ONLY}")
    set_python_boolean(CONAN_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")

    set_conan_settings(CONAN_SETTINGS
        "arch=${CONAN_ARCH}"
        "os=${CONAN_OS}"
        "compiler=${CONAN_COMPILER}"
        "compiler.version=${CONAN_COMPILER_VERSION}"
        "compiler.cstd=${CMAKE_C_STANDARD}"
        "compiler.cppstd=${CMAKE_CXX_STANDARD}"
        "compiler.runtime=${CONAN_COMPILER_RUNTIME}"
        "compiler.runtime_type=${CONAN_COMPILER_RUNTIME_TYPE}"
        "compiler.libcxx=${CONAN_COMPILER_LIBCXX}"
        "build_type=${CMAKE_BUILD_TYPE}"
    )
    set_conan_options(CONAN_OPTIONS
        "&:shared=${CONAN_BUILD_SHARED_LIBS}"
        "*:shared=${CONAN_BUILD_SHARED_LIBS}"
    )
    if(NOT "${SKIP_TEST}" AND NOT "gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
        if(NOT EXISTS "${CONAN_INSTALL_PREFIX}/test/conan-packages.cmake")
            #[[execute_process(
                COMMAND "${CONAN_COMMAND}" "user" "$ENV{CONAN_LOGIN_USERNAME}" "-r" "artifactory" "-p"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                COMMAND_ECHO "STDOUT"
                ENCODING "UTF-8"
                COMMAND_ERROR_IS_FATAL "ANY"
            )]]
            execute_process(
                COMMAND "${CONAN_COMMAND}" "install"
                        "--output-folder" "${CONAN_INSTALL_PREFIX}/test"
                        ${CONAN_SETTINGS}
                        ${CONAN_OPTIONS}
                        "${PROJECT_SOURCE_DIR}/test-conanfile.py"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                COMMAND_ECHO "STDOUT"
                ENCODING "UTF-8"
                COMMAND_ERROR_IS_FATAL "ANY"
            )
        endif()
        if(NOT EXISTS "${CONAN_INSTALL_PREFIX}/test/conan-packages.cmake")
            message(FATAL_ERROR "Not exists: '${CONAN_INSTALL_PREFIX}/test/conan-packages.cmake'")
        endif()
        include("${CONAN_INSTALL_PREFIX}/test/conan-packages.cmake")
        list(APPEND CMAKE_MODULE_PATH "${CONAN_INSTALL_PREFIX}/test")
        foreach(
            conanPackageName
            cmakePackageName
            cmakePackageVersion
            cmakePackagePath
            IN ZIP_LISTS
            "${PROJECT_NAME}_CONAN_PACKAGE_NAMES"
            "${PROJECT_NAME}_CMAKE_PACKAGE_NAMES"
            "${PROJECT_NAME}_CMAKE_PACKAGE_VERSIONS"
            "${PROJECT_NAME}_CMAKE_PACKAGE_PATHS"
        )
            if(
                "" STREQUAL "${conanPackageName}"
                OR "" STREQUAL "${cmakePackageName}"
                OR "" STREQUAL "${cmakePackageVersion}"
                OR "" STREQUAL "${cmakePackagePath}"
            )
                string(JOIN " " errorMessage
                    "Empty value 'conanPackageName': '${conanPackageName}'"
                    "or 'cmakePackageName': '${cmakePackageName}'"
                    "or 'cmakePackageVersion': '${cmakePackageVersion}'"
                    "or 'cmakePackagePath': '${cmakePackagePath}'!"
                )
                message(FATAL_ERROR "${errorMessage}")
            elseif(NOT IS_DIRECTORY "${cmakePackagePath}")
                message(FATAL_ERROR "Not is directory 'cmakePackagePath': '${cmakePackagePath}'!")
            endif()

            if("${${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_VERSION}" STREQUAL "")
                set("${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_VERSION" "${cmakePackageVersion}")
            endif()

            if("${${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_PATH}" STREQUAL "")
                set("${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_PATH" "${cmakePackagePath}")
            endif()

            list(APPEND "${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES" "${cmakePackageName}")
            list(APPEND CMAKE_PREFIX_PATH "${cmakePackagePath}")
        endforeach()
    endif()
endif()

if("${${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES}" STREQUAL "")
    if(NOT "${SKIP_TEST}" AND NOT "gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
        list(APPEND "${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES"
            "GTest"
            "exqudens-cpp-log"
        )
    endif()
endif()
message(STATUS "${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES:")
foreach(cmakePackageName IN LISTS "${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES")
    message(STATUS "  ${cmakePackageName}")
endforeach()
unset(NOT_FOUND_PACKAGE_NAMES)
foreach(cmakePackageName IN LISTS "${PROJECT_NAME}_CMAKE_FIND_PACKAGE_NAMES")
    if("GTest" STREQUAL "${cmakePackageName}")
        file(REMOVE "${CONAN_INSTALL_PREFIX}/test/${cmakePackageName}Config.cmake")
        file(REMOVE "${CONAN_INSTALL_PREFIX}/test/${cmakePackageName}-config.cmake")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_VERSION}" EXACT QUIET)
    elseif("exqudens-cpp-log" STREQUAL "${cmakePackageName}")
        file(REMOVE "${CONAN_INSTALL_PREFIX}/test/${cmakePackageName}Config.cmake")
        file(REMOVE "${CONAN_INSTALL_PREFIX}/test/${cmakePackageName}-config.cmake")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${${PROJECT_NAME}_CMAKE_PACKAGE_${cmakePackageName}_VERSION}" EXACT QUIET)
    else()
        message(STATUS "Ignore cmakePackageName: '${cmakePackageName}'")
    endif()
endforeach()
set_not_found_package_names(NOT_FOUND_PACKAGE_NAMES ${NOT_FOUND_PACKAGE_NAMES})
if(NOT "" STREQUAL "${NOT_FOUND_PACKAGE_NAMES}")
    foreach(package IN LISTS NOT_FOUND_PACKAGE_NAMES)
        message(SEND_ERROR "${package}_FOUND: '${${package}_FOUND}' ${package}_NOT_FOUND_MESSAGE: '${${package}_NOT_FOUND_MESSAGE}'")
    endforeach()
    message(FATAL_ERROR "Some package not found!")
endif()

string(REPLACE "_" "-" BASE_DIR "${PROJECT_NAME}")
string(REPLACE "-cpp-" "-" BASE_DIR "${BASE_DIR}")
string(REPLACE "-" "/" BASE_DIR "${BASE_DIR}")

string(REPLACE "/" "_" BASE_MACRO "${BASE_DIR}")
string(TOUPPER "${BASE_MACRO}" BASE_MACRO)

set(CUBE_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/src/main/c")
set(IOC_FILE "${CUBE_OUTPUT_DIR}/st/st.ioc")

find_program(STM_CUBE_FILE NAMES "STM32CubeMX.exe" "STM32CubeMX" PATHS ENV STM_CUBE_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
find_program(STM_CUBE_JAVA NAMES "java.exe" "java" PATHS "${STM_CUBE_FILE}/../jre/bin" REQUIRED NO_CACHE NO_DEFAULT_PATH)
find_program(OPENOCD_COMMAND NAMES "openocd.exe" "openocd" PATHS ENV OPEN_OCD_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)

message(STATUS "BASE_DIR: '${BASE_DIR}'")
message(STATUS "BASE_MACRO: '${BASE_MACRO}'")
message(STATUS "IOC_FILE: '${IOC_FILE}'")
message(STATUS "CUBE_OUTPUT_DIR: '${CUBE_OUTPUT_DIR}'")
message(STATUS "STM_CUBE_FILE: '${STM_CUBE_FILE}'")
message(STATUS "STM_CUBE_JAVA: '${STM_CUBE_JAVA}'")
message(STATUS "OPENOCD_COMMAND: '${OPENOCD_COMMAND}'")

configure_file("${PROJECT_SOURCE_DIR}/stm-cube-script.txt.in" "${PROJECT_BINARY_DIR}/stm-cube-script.txt" @ONLY NEWLINE_STYLE UNIX)

add_custom_target("st-cube-update"
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CUBE_OUTPUT_DIR}/st"
    COMMAND "${CMAKE_COMMAND}" "-E" "copy" "${PROJECT_SOURCE_DIR}/st.ioc" "${CUBE_OUTPUT_DIR}/st"
    COMMAND "${STM_CUBE_JAVA}" "-Dfile.encoding=UTF-8" "-jar" "${STM_CUBE_FILE}" "-q" "${PROJECT_BINARY_DIR}/stm-cube-script.txt"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)
add_custom_target("st-cube-clean"
    COMMAND "${CMAKE_COMMAND}" "-P" "cmake/util.cmake" "--" "file_copy_string_replace"
            SRC_FILE "${PROJECT_SOURCE_DIR}/src/main/c/st/Core/Src/main.c"
            DST_FILE "${PROJECT_SOURCE_DIR}/src/main/c/${BASE_DIR}/hal/Core/Src/main.c"
            SRC_STRING "int main(void)"
            DST_STRING "int32_t main_init(void)"

    COMMAND "${CMAKE_COMMAND}" "-P" "cmake/util.cmake" "--" "file_copy_string_replace"
            SRC_FILE "${CUBE_OUTPUT_DIR}/st/STM32H7A3XX_FLASH.ld"
            DST_FILE "${PROJECT_SOURCE_DIR}/STM32H7A3XX_FLASH.ld"
            SRC_STRING " (READONLY)"
            DST_STRING ""
    COMMAND "${CMAKE_COMMAND}" "-P" "cmake/util.cmake" "--" "file_copy_string_from"
            SRC_FILE "${CUBE_OUTPUT_DIR}/st/cmake/gcc-arm-none-eabi.cmake"
            DST_FILE "${PROJECT_SOURCE_DIR}/flags.cmake"
            SRC_STRING "# MCU specific flags"

    COMMAND "${CMAKE_COMMAND}" "-E" "rm" "-rf" "${CUBE_OUTPUT_DIR}/st/cmake"
                                               "${CUBE_OUTPUT_DIR}/st/Drivers/CMSIS/Device/ST/STM32H7xx/Source"
    COMMAND "${CMAKE_COMMAND}" "-E" "rm" "-f" "${CUBE_OUTPUT_DIR}/st/Drivers/CMSIS/Device/ST/STM32H7xx/LICENSE.txt"
                                              "${CUBE_OUTPUT_DIR}/st/Drivers/CMSIS/LICENSE.txt"
                                              "${CUBE_OUTPUT_DIR}/st/Drivers/STM32H7xx_HAL_Driver/LICENSE.txt"
                                              "${CUBE_OUTPUT_DIR}/st/Middlewares/ST/STM32_USB_Device_Library/LICENSE.txt"
                                              "${CUBE_OUTPUT_DIR}/st/.mxproject"
                                              "${CUBE_OUTPUT_DIR}/st/CMakeLists.txt"
                                              "${CUBE_OUTPUT_DIR}/st/CMakePresets.json"
                                              "${CUBE_OUTPUT_DIR}/st/st.ioc"
                                              "${CUBE_OUTPUT_DIR}/st/STM32H7A3XX_FLASH.ld"

    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)
add_custom_target("st-cube"
    COMMAND "${CMAKE_COMMAND}" "--build" "${PROJECT_BINARY_DIR}" "--target" "st-cube-update"
    COMMAND "${CMAKE_COMMAND}" "--build" "${PROJECT_BINARY_DIR}" "--target" "st-cube-clean"
    COMMAND "${CMAKE_COMMAND}" "-E" "echo" "BUILD_SUCCESS"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)

configure_file("${PROJECT_SOURCE_DIR}/versions.hpp.in" "${PROJECT_BINARY_DIR}/generated/src/main/cpp/${BASE_DIR}/versions.hpp" @ONLY NEWLINE_STYLE UNIX)
add_library("${PROJECT_NAME}-hal"
    "src/main/cpp/exqudens/embedded/usb/hal/IHardware.hpp"
    "src/main/cpp/exqudens/embedded/usb/hal/HardwareUtils.hpp"
    "src/main/cpp/exqudens/embedded/usb/hal/HardwareUtils.cpp"
)
generate_export_header("${PROJECT_NAME}-hal" BASE_NAME "${BASE_MACRO}_HAL" EXPORT_FILE_NAME "generated/src/main/cpp/${BASE_DIR}/hal/export.hpp")
if(NOT "${BUILD_SHARED_LIBS}")
    target_compile_definitions("${PROJECT_NAME}-hal" PUBLIC "${BASE_MACRO}_HAL_STATIC_DEFINE")
endif()
if("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    target_sources("${PROJECT_NAME}-hal" PUBLIC
        "src/main/cpp/exqudens/embedded/usb/hal/Hardware.hpp"
        "src/main/cpp/exqudens/embedded/usb/hal/Hardware.cpp"

        "src/main/c/st/Core/Inc/stm32h7xx_hal_conf.h"
        "src/main/c/st/Core/Inc/gpio.h"
        "src/main/c/st/Core/Src/gpio.c"
        "src/main/c/st/Core/Inc/main.h"
        #"src/main/c/st/Core/Src/main.c"
        "src/main/c/exqudens/embedded/usb/hal/Core/Src/main.c"
        "src/main/c/st/Core/Inc/memorymap.h"
        "src/main/c/st/Core/Src/memorymap.c"
        "src/main/c/st/Core/Inc/stm32h7xx_it.h"
        #"src/main/c/st/Core/Src/stm32h7xx_it.c"
        "src/main/c/st/Core/Src/stm32h7xx_hal_msp.c"
        "src/main/c/st/Core/Src/syscalls.c"
        "src/main/c/st/Core/Src/sysmem.c"
        "src/main/c/st/Core/Src/system_stm32h7xx.c"

        "src/main/c/st/Drivers/CMSIS/Device/ST/STM32H7xx/Include/stm32h7a3xxq.h"
        "src/main/c/st/Drivers/CMSIS/Device/ST/STM32H7xx/Include/stm32h7xx.h"
        "src/main/c/st/Drivers/CMSIS/Device/ST/STM32H7xx/Include/system_stm32h7xx.h"

        "src/main/c/st/Drivers/CMSIS/Include/cachel1_armv7.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_armcc.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_armclang_ltm.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_armclang.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_compiler.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_gcc.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_iccarm.h"
        "src/main/c/st/Drivers/CMSIS/Include/cmsis_version.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_armv8mbl.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_armv8mml.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_armv81mml.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm0.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm0plus.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm1.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm3.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm4.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm7.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm23.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm33.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm35p.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm55.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_cm85.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_sc000.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_sc300.h"
        "src/main/c/st/Drivers/CMSIS/Include/core_starmc1.h"
        "src/main/c/st/Drivers/CMSIS/Include/mpu_armv7.h"
        "src/main/c/st/Drivers/CMSIS/Include/mpu_armv8.h"
        "src/main/c/st/Drivers/CMSIS/Include/pac_armv81.h"
        "src/main/c/st/Drivers/CMSIS/Include/pmu_armv8.h"
        "src/main/c/st/Drivers/CMSIS/Include/tz_context.h"

        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy/stm32_hal_legacy.h"

        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_cortex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_def.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_dma_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_dma.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_exti.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_flash_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_flash.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_gpio_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_gpio.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_hsem.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_i2c_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_i2c.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_mdma.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_pcd_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_pcd.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_pwr_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_pwr.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_rcc_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_rcc.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_tim_ex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_tim.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_bus.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_cortex.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_crs.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_dma.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_dmamux.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_exti.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_gpio.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_hsem.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_pwr.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_rcc.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_system.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_usb.h"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/stm32h7xx_ll_utils.h"

        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c"
        "src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c"

        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.h"
        #"src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c"
        "src/main/c/exqudens/embedded/usb/hal/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c"

        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_core.h"
        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ctlreq.h"
        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_def.h"
        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ioreq.h"

        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c"
        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c"
        "src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c"

        "src/main/c/st/USB_DEVICE/App/usb_device.h"
        "src/main/c/st/USB_DEVICE/App/usb_device.c"
        "src/main/c/st/USB_DEVICE/App/usbd_cdc_if.h"
        #"src/main/c/st/USB_DEVICE/App/usbd_cdc_if.c"
        "src/main/c/exqudens/embedded/usb/hal/USB_DEVICE/App/usbd_cdc_if.c"
        "src/main/c/st/USB_DEVICE/App/usbd_desc.h"
        "src/main/c/st/USB_DEVICE/App/usbd_desc.c"

        "src/main/c/st/USB_DEVICE/Target/usbd_conf.h"
        "src/main/c/st/USB_DEVICE/Target/usbd_conf.c"
    )
    target_include_directories("${PROJECT_NAME}-hal" PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Core/Inc>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Drivers/CMSIS/Device/ST/STM32H7xx/Include>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Drivers/CMSIS/Include>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Drivers/STM32H7xx_HAL_Driver/Inc>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/Middlewares/ST/STM32_USB_Device_Library/Core/Inc>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/USB_DEVICE/App>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/c/st/USB_DEVICE/Target>"
    )
    target_compile_definitions("${PROJECT_NAME}-hal" PUBLIC
        "USE_PWR_DIRECT_SMPS_SUPPLY"
        "USE_HAL_DRIVER"
        "STM32H7A3xxQ"
    )
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        target_compile_definitions("${PROJECT_NAME}-hal" PUBLIC "DEBUG")
    endif()

    if("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS" AND EXISTS "${CMAKE_SOURCE_DIR}/flags.cmake")
        include("${CMAKE_SOURCE_DIR}/flags.cmake")
    endif()
endif()
target_include_directories("${PROJECT_NAME}-hal" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated/src/main/cpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
set_target_properties("${PROJECT_NAME}-hal" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"
)

add_library("${PROJECT_NAME}"
    "src/main/cpp/exqudens/embedded/usb/Application.hpp"
    "src/main/cpp/exqudens/embedded/usb/Application.cpp"
)
generate_export_header("${PROJECT_NAME}" BASE_NAME "${BASE_MACRO}" EXPORT_FILE_NAME "generated/src/main/cpp/${BASE_DIR}/export.hpp")
if(NOT "${BUILD_SHARED_LIBS}")
    target_compile_definitions("${PROJECT_NAME}" PUBLIC "${BASE_MACRO}_STATIC_DEFINE")
endif()
target_include_directories("${PROJECT_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}" PUBLIC
    "${PROJECT_NAME}-hal"
)
set_target_properties("${PROJECT_NAME}" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"
)

if("gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    add_executable("${PROJECT_NAME}-app"
        "src/main/cpp/main.cpp"
        "src/main/c/st/Core/Src/stm32h7xx_it.c"
        "src/main/c/st/startup_stm32h7a3xxq.s"
    )
    target_link_libraries("${PROJECT_NAME}-app"
        "${PROJECT_NAME}"
    )
    set_target_properties("${PROJECT_NAME}-app" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/bin"

        ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"

        LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"
    )

    add_custom_target("deploy"
        COMMAND "${OPENOCD_COMMAND}" "-f" "interface/stlink.cfg" "-f" "target/stm32h7x.cfg" "-c" "program $<TARGET_FILE:${PROJECT_NAME}-app> verify reset exit"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        USES_TERMINAL
        VERBATIM
    )

    set(IDE_TARGET_DEPENDS_ON "${PROJECT_NAME}-app")
    message(STATUS "IDE_TARGET_DEPENDS_ON: '${IDE_TARGET_DEPENDS_ON}'")
endif()

if(NOT "${SKIP_TEST}" AND NOT "gcc-arm" IN_LIST "PRESET_NAME_ELEMENTS")
    add_library("test-lib"
        "src/test/cpp/TestUtils.hpp"
        "src/test/cpp/TestUtils.cpp"
        "src/test/cpp/TestApplication.hpp"
        "src/test/cpp/TestApplication.cpp"
        "src/test/cpp/unit/FunctionalUnitTests.hpp"
    )
    generate_export_header("test-lib"
        BASE_NAME "TEST_LIB"
        EXPORT_FILE_NAME "generated/src/test/cpp/test_lib_export.hpp"
    )
    target_include_directories("test-lib" PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated/src/test/cpp>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/test/cpp>"
        "$<INSTALL_INTERFACE:include>"
    )
    target_link_libraries("test-lib" PUBLIC
        "${PROJECT_NAME}"
        "GTest::gmock"
        "GTest::gtest"
        "GTest::gmock_main"
        "GTest::gtest_main"
        "exqudens-cpp-log::exqudens-cpp-log"
    )
    set_target_properties("test-lib" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/bin"

        ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"

        LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"
    )

    add_executable("test-app" "src/test/cpp/main.cpp")
    target_link_libraries("test-app"
        "test-lib"
    )
    set_target_properties("test-app" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/bin"

        ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"

        LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"
    )
    if("${BUILD_SHARED_LIBS}")
        if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
            add_custom_command(TARGET "test-app"
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},RUNTIME_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/bin" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/test/bin" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                USES_TERMINAL
                VERBATIM
            )
        else()
            add_custom_command(TARGET "test-app"
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/test/lib" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:test-lib,LIBRARY_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/lib" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},LIBRARY_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:test-app,RUNTIME_OUTPUT_DIRECTORY>"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                USES_TERMINAL
                VERBATIM
            )
        endif()
    endif()
    add_custom_command(TARGET "test-app"
        POST_BUILD
        COMMAND "$<TARGET_FILE:test-app>" "--gtest_list_tests" "--gtest_output=json:tests.json" "--project-binary-dir=${PROJECT_BINARY_DIR}"
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:test-app>"
        USES_TERMINAL
        VERBATIM
    )

    gtest_discover_tests("test-app"
        DISCOVERY_EXTRA_ARGS "--project-binary-dir=${PROJECT_BINARY_DIR}"
        EXTRA_ARGS "--project-binary-dir=${PROJECT_BINARY_DIR}"
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:test-app>"
        PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:test-app>;${ENV_PATH}"
    )

    set(TEST_GROUP "all" CACHE STRING "...")
    message(STATUS "TEST_GROUP: '${TEST_GROUP}'")
    set(TEST_CASE "all" CACHE STRING "...")
    message(STATUS "TEST_CASE: '${TEST_CASE}'")

    if("${TEST_GROUP}" STREQUAL "all" AND "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^.+\\..+$")
    elseif(NOT "${TEST_GROUP}" STREQUAL "all" AND "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^${TEST_GROUP}\\..+$")
    elseif("${TEST_GROUP}" STREQUAL "all" AND NOT "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^.+\\.${TEST_CASE}$")
    elseif(NOT "${TEST_GROUP}" STREQUAL "all" AND NOT "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^${TEST_GROUP}\\.${TEST_CASE}$")
    endif()
    message(STATUS "TEST_REGEXP: '${TEST_REGEXP}'")

    add_custom_command(
        OUTPUT "${PROJECT_BINARY_DIR}/junit.xml"
        COMMAND "${CMAKE_CTEST_COMMAND}"
                "--build-nocmake"
                "--output-junit" "${PROJECT_BINARY_DIR}/junit.xml"
                "--test-dir" "${PROJECT_BINARY_DIR}"
                #"-R" "^.+\\..+$"
                #"-R" "^FunctionalSystemTests\\..+$"
                #"-R" "^FunctionalUnitTests\\..+$"
                "-R" "${TEST_REGEXP}"
        DEPENDS "test-app"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target("cmake-test"
        DEPENDS "${PROJECT_BINARY_DIR}/junit.xml"
    )

    set(IDE_TARGET_DEPENDS_ON "test-app")
    message(STATUS "IDE_TARGET_DEPENDS_ON: '${IDE_TARGET_DEPENDS_ON}'")
endif()

add_custom_target("vscode"
    COMMAND "${CMAKE_COMMAND}" "-P" "cmake/util.cmake" "--" "vscode"
            SOURCE_DIR "${PROJECT_SOURCE_DIR}"
            BINARY_DIR "${PROJECT_BINARY_DIR}"

            LAUNCH_GENERATE "1"
            LAUNCH_FILE_OVERWRITE "1"
            LAUNCH_TEMPLATE_FILE "${PROJECT_SOURCE_DIR}/src/test/resources/vscode/command-variable-launch.json"
            LAUNCH_FILE "${PROJECT_SOURCE_DIR}/.vscode/launch.json"
            LAUNCH_TESTS_FILE "${PROJECT_BINARY_DIR}/test/bin/tests.json"
            LAUNCH_TESTS "*.*" "*UnitTests.*"
            LAUNCH_TESTS_INDENT "20"
    DEPENDS "${IDE_TARGET_DEPENDS_ON}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)

set_target_names(ALL_TARGETS "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "all-targets:")
foreach(t IN LISTS ALL_TARGETS)
    message(STATUS "${t}")
endforeach()

set("${PROJECT_NAME}_ADDITIONAL_CLEAN_FILES"
    "${CMAKE_INSTALL_PREFIX}"
)
set_directory_properties(PROPERTIES
    ADDITIONAL_CLEAN_FILES "${${PROJECT_NAME}_ADDITIONAL_CLEAN_FILES}"
)
