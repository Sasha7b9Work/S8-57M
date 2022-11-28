project (Panel)

include_directories(../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ..
                    ../common/_VS
                    ../common/_VS/std_includes
                    ../common/Display/Font
                    src
                    src/Hardware/HAL)
                    
add_definitions(-DSTM32F479xx -DPANEL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4710)    # 'xxx': function not inlined
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # implicit conversion from 'xxx' to 'float', possible loss of data
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd5264)    # `xxx': 'const' variable is not used
add_compile_options(/wd26812)   #The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)

file(GLOB LIBS_HAL        ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)
file(GLOB SRC             src/*.h src/*.cpp)
file(GLOB COMMUNICATOR    ../common/Communicator/Message_.h
                          ../common/Communicator/Communicator_.h
                          ../common/Communicator/Communicator_p.cpp)
file(GLOB DISPLAY         src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT    src/Display/Font/*.*)
file(GLOB HARDWARE        src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS  src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL    src/Hardware/HAL/*.*)
file(GLOB SETTINGS        src/Settings/*.*)
file(GLOB UTILS           src/Utils/*.*)
file(GLOB VS              ../common/_VS/*.h)
file(GLOB VS_STD_INCLUDES ../common/_VS/std_includes/*)
file(GLOB VS_CMSIS        ../common/_VS/CMSIS/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\HAL         FILES ${LIBS_HAL})
source_group(src               FILES ${SRC})
source_group(Communicator      FILES ${COMMUNICATOR})
source_group(Display           FILES ${DISPLAY})
source_group(Display\\Font     FILES ${DISPLAY_FONT})
source_group(Hardware          FILES ${HARDWARE})
source_group(Hardware\\CMSIS   FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL     FILES ${HARDWARE_HAL})
source_group(Settings          FILES ${SETTINGS})
source_group(Utils             FILES ${UTILS})
source_group(_VS               FILES ${VS})
source_group(_VS\\std_includes FILES ${VS_STD_INCLUDES})
source_group(_VS\\CMSIS        FILES ${VS_CMSIS})

add_library(Panel ${LIBS_HAL} ${SRC} ${COMMUNICATOR} ${DISPLAY} ${DISPLAY_FONT} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL}
                  ${SETTINGS} ${UTILS} ${VS} ${VS_STD_INCLUDES} ${VS_CMSIS}
)
