project (Device)

#set(CMAKE_UNITY_BUILD OFF)

include_directories(..
                    ../common/_VS
                    ../common/_VS/std_includes
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    src
                    src/Hardware/HAL
                    src/Hardware/FatFS
                    src/Hardware/USBH
                    src/Hardware/USBD)

add_definitions(-DSTM32F437xx -DDEVICE -DVS_KEIL -DUSE_USB_FS)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4100)    # 'xxx': unreferenced formal parameter
add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a
                                # base class default constructor is inaccessible or deleted
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # '<anonymous-tag>': 'N' bytes padding added after data member
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                # function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # implicit conversion from 'xxx' to 'float', possible loss of data
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd5264)    # 'xxx': 'const' variable is not used

file(GLOB LIBS_FATFS    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/*.*)

file(GLOB LIBS_HAL      ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                        ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)

file(GLOB LIBS_USBD     ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/*.*)

file(GLOB LIBS_USBH     ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/*.*
                        ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/*.*)

file(GLOB VS                               ../common/_VS/*.*)
file(GLOB VS_STD_INCLUDES                  ../common/_VS/std_includes/*)

file(GLOB HARDWARE                         src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS                   src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL                     src/Hardware/HAL/*.*)
file(GLOB HARDWARE_MEMORY                  src/Hardware/Memory/*.*)
file(GLOB HARDWARE_USBD                    src/Hardware/USBD/*.*)
file(GLOB HARDWARE_USBH                    src/Hardware/USBH/*.*)
file(GLOB DOC                              ../doc/*.*)
file(GLOB COMMON                           ../common/Command.h
                                           ../common/common_defines.h
                                           ../common/Decoder_d.h
                                           ../common/Decoder_d.cpp
                                           ../common/Message.h
                                           ../common/Message.cpp)
file(GLOB SRC                              src/*.*)
file(GLOB DISPLAY                          src/Display/*.*)
file(GLOB DISPLAY_FONT                     ../common/Display/Font/*.*)
file(GLOB FLASHDRIVE                       src/FlashDrive/*.*)
file(GLOB FPGA                             src/FPGA/*.*)
file(GLOB FREQMETER                        src/FreqMeter/*.*)
file(GLOB KEYBOARD                         src/Keyboard/*.*)
file(GLOB MENU                             src/Menu/*.*)
file(GLOB MENU_PAGES                       src/Menu/Pages/*.*)
file(GLOB MENU_PAGES_INCLUDE               src/Menu/Pages/Include/*.*)
file(GLOB MENU_PAGES_PAGEDISPLAY           src/Menu/Pages/PageDisplay/*.*)
file(GLOB MENU_PAGES_PAGEFUNCTION          src/Menu/Pages/PageFunction/*.*)
file(GLOB MENU_PAGES_PAGERECORDER          src/Menu/Pages/PageRecorder/*.*)
file(GLOB MENU_PAGES_PAGEMEASURES          src/Menu/Pages/PageMeasures/*.*)
file(GLOB MENU_PAGES_PAGEMEMORY            src/Menu/Pages/PageMemory/*.*)
file(GLOB MENU_PAGES_PAGESERVICE           src/Menu/Pages/PageService/*.*)
file(GLOB MENU_PAGES_PAGESERVICE_PAGEDEBUG src/Menu/Pages/PageService/PageDebug/*.*)
file(GLOB MENU_PAGES_PAGETRIG              src/Menu/Pages/PageTrig/*.*)
file(GLOB OSCI                             src/Osci/*.*)
file(GLOB OSCI_DISPLAY                     src/Osci/Display/*.*)
file(GLOB OSCI_MATH                        src/Osci/Math/*.*)
file(GLOB OSCI_MEASUREMENTS                src/Osci/Measurements/*.*)
file(GLOB RECORDER                         src/Recorder/*.*)
file(GLOB SCPI                             src/SCPI/*.*)
file(GLOB SETTINGS                         src/Settings/*.*)
file(GLOB TEST                             src/Test/*.*)
file(GLOB UTILS                            src/Utils/*.*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBD} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(common                              FILES ${COMMON})
source_group(doc                                 FILES ${DOC})
source_group(_VS                                 FILES ${VS})
source_group(_VS/std_includes                    FILES ${VS_STD_INCLUDES})
source_group(src                                 FILES ${SRC})
source_group(Display                             FILES ${DISPLAY})
source_group(Display/Font                        FILES ${DISPLAY_FONT})
source_group(FDrive                              FILES ${FLASHDRIVE})
source_group(FPGA                                FILES ${FPGA})
source_group(FreqMeter                           FILES ${FREQMETER})
source_group(Hardware                            FILES ${HARDWARE})
source_group(Hardware/CMSIS                      FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL                        FILES ${HARDWARE_HAL})
source_group(Hardware/Memory                     FILES ${HARDWARE_MEMORY})
source_group(Hardware/USBD                       FILES ${HARDWARE_USBD})
source_group(Hardware/USBH                       FILES ${HARDWARE_USBH})
source_group(libs/FatFS                          FILES ${LIBS_FATFS})
source_group(libs/HAL                            FILES ${LIBS_HAL})
source_group(libs/USBD                           FILES ${LIBS_USBD})
source_group(libs/USBH                           FILES ${LIBS_USBH})
source_group(Keyboard                            FILES ${KEYBOARD})
source_group(Menu                                FILES ${MENU})
source_group(Menu/Pages                          FILES ${MENU_PAGES})
source_group(Menu/Pages/Include                  FILES ${MENU_PAGES_INCLUDE})
source_group(Menu/Pages/PageDisplay              FILES ${MENU_PAGES_PAGEDISPLAY})
source_group(Menu/Pages/PageFunction             FILES ${MENU_PAGES_PAGEFUNCTION})
source_group(Menu/Pages/PageRecorder             FILES ${MENU_PAGES_PAGERECORDER})
source_group(Menu/Pages/PageMeasures             FILES ${MENU_PAGES_PAGEMEASURES})
source_group(Menu/Pages/PageMemory               FILES ${MENU_PAGES_PAGEMEMORY})
source_group(Menu/Pages/PageService              FILES ${MENU_PAGES_PAGESERVICE})
source_group(Menu/Pages/PageService/PageDebug    FILES ${MENU_PAGES_PAGESERVICE_PAGEDEBUG})
source_group(Menu/Pages/PageTrig                 FILES ${MENU_PAGES_PAGETRIG})
source_group(Osci                                FILES ${OSCI})
source_group(Osci/Display                        FILES ${OSCI_DISPLAY})
source_group(Osci/Math                           FILES ${OSCI_MATH})
source_group(Osci/Measurements                   FILES ${OSCI_MEASUREMENTS})
source_group(Recorder                            FILES ${RECORDER})
source_group(SCPI                                FILES ${SCPI})
source_group(Settings                            FILES ${SETTINGS})
source_group(Test                                FILES ${TEST})
source_group(Utils                               FILES ${UTILS})

add_library(Device ${COMMON} ${DOC} ${VS} ${VS_STD_INCLUDES} ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FLASHDRIVE} ${FPGA}
                   ${FREQMETER} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL}
                   ${HARDWARE_MEMORY} ${HARDWARE_USBD} ${HARDWARE_USBH} ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBD}
                   ${LIBS_USBH} ${KEYBOARD} ${MENU} ${MENU_PAGES} ${MENU_PAGES_INCLUDE} ${MENU_PAGES_PAGEDISPLAY}
                   ${MENU_PAGES_PAGEFUNCTION} ${MENU_PAGES_PAGERECORDER}
                   ${MENU_PAGES_PAGEMEASURES} ${MENU_PAGES_PAGEMEMORY}
                   ${MENU_PAGES_PAGESERVICE} ${MENU_PAGES_PAGESERVICE_PAGEDEBUG} ${MENU_PAGES_PAGETRIG}
                   ${OSCI} ${OSCI_DISPLAY} ${OSCI_MATH}
                   ${OSCI_MEASUREMENTS} ${RECORDER} ${SCPI} ${SETTINGS} ${TEST} ${UTILS})
