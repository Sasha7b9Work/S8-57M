# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (DLoader)

#set(CMAKE_UNITY_BUILD OFF)

include_directories(../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ..
                    ../common/_VS
                    ../common/_VS/std_includes
                    src
                    src/Hardware/HAL
                    src/Hardware/FatFS
                    src/Hardware/USBH)
                    
add_definitions(-DSTM32F437xx -DLOADER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4100)    # unreferenced formal parameter
add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # implicit conversion from 'xxx' to 'float', possible loss of data
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd5264)    # 'const' variable is not used
add_compile_options(/wd26812)   # The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)

file(GLOB LIBS_FATFS         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/*.*)
file(GLOB LIBS_HAL           ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)
file(GLOB LIBS_USBH          ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/*.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/*.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/*.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/*.c)
file(GLOB SRC                src/*.h src/*.cpp)
file(GLOB COMMON             ../common/Command.h
                             ../common/Decoder_d.h
                             ../common/Decoder_d.cpp)
file(GLOB DISPLAY            src/Display/*)
file(GLOB DISPLAY_FONT       ../common/Display/Font/Font_d.h
                             ../common/Display/Font/Font_d.cpp
                             ../common/Display/Font/font8.inc
                             src/Display/AdvancedFont_d.cpp)
file(GLOB FDRIVE             src/FlashDrive/*)
file(GLOB HARDWARE           src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS     src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS     src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL       src/Hardware/HAL/*)
file(GLOB HARDWARE_STM32     src/Hardware/stm32/*)
file(GLOB HARDWARE_USBH      src/Hardware/USBH/*)
file(GLOB KEYBOARD           src/Keyboard/*)
file(GLOB UTILS              src/Utils/*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\FatFS     FILES ${LIBS_FATFS})
source_group(libs\\HAL       FILES ${LIBS_HAL})
source_group(libs\\USBH      FILES ${LIBS_USBH})
source_group(src             FILES ${SRC})
source_group(commmon         FILES ${COMMON})
source_group(Display         FILES ${DISPLAY})
source_group(Display\\Font   FILES ${DISPLAY_FONT})
source_group(FDrive          FILES ${FDRIVE})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware\\CMSIS FILES ${HARDWARE_CMSIS})
source_group(Hardware\\FatFS FILES ${HARDWARE_FATFS})
source_group(Hardware\\HAL   FILES ${HARDWARE_HAL})
source_group(Hardware\\stm32 FILES ${HARDWARE_STM32})
source_group(Hardware\\USBH  FILES ${HARDWARE_USBH})
source_group(Keyboard        FILES ${KEYBOARD})
source_group(Utils           FILES ${UTILS})

add_library(DLoader ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} ${SRC} ${COMMON} ${DISPLAY} ${DISPLAY_FONT} ${FDRIVE} ${HARDWARE} ${HARDWARE_CMSIS}
                    ${HARDWARE_FATFS} ${HARDWARE_HAL} ${HARDWARE_STM32} ${HARDWARE_USBH} ${KEYBOARD} ${UTILS})
