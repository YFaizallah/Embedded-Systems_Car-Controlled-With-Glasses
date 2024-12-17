# # CMakeLists.txt
# cmake_minimum_required(VERSION 3.13)

# set(PROGRAM_NAME pico-w-server)
# set(PICO_BOARD arduino_nano_rp2040_connect)

# include(pico_sdk_import.cmake)
# project(${PROGRAM_NAME})

# pico_sdk_init()

# add_executable(${PROGRAM_NAME}
#     main.c
#     sys_arch.c
#     wifi_module.c
#     wifi_module.h
#     ir_sensor.c
#     ir_sensor.h
#     eye.c
#     eye.h
#     gyroscope.c
#     gyroscope.h
# )

# target_include_directories(${PROGRAM_NAME} PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}
#     ${PICO_SDK_PATH}/lib/lwip/src/include
#     ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
#     ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include
#     ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
#     ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include
#     ${PICO_SDK_PATH}/src/common/pico_sync/include
#     ${PICO_SDK_PATH}/src/common/pico_time/include
#     ${PICO_SDK_PATH}/src/common/pico_base/include
#     ${PICO_SDK_PATH}/src/rp2_common/pico_platform/include
# )

# target_compile_definitions(${PROGRAM_NAME} PRIVATE
#     PICO_MAX_SHARED_IRQ_HANDLERS=16
#     NO_SYS=1
#     LWIP_RAW=1
#     LWIP_NETCONN=0
#     LWIP_SOCKET=0
#     LWIP_DHCP=0  # We're using static IP
#     LWIP_ICMP=1
#     LWIP_TCP=1
#     ETH_PAD_SIZE=2
#     LWIP_NETIF_LINK_CALLBACK=1
#     LWIP_NETIF_STATUS_CALLBACK=1
# )

# target_link_libraries(${PROGRAM_NAME} 
#     pico_stdlib
#     pico_platform
#     pico_rand
#     pico_time
#     pico_lwip
#     hardware_adc
#     hardware_irq
#     hardware_gpio
#     hardware_timer
#     hardware_spi
#     hardware_i2c
# )

# pico_enable_stdio_usb(${PROGRAM_NAME} 1)
# pico_enable_stdio_uart(${PROGRAM_NAME} 0)

# pico_add_extra_outputs(${PROGRAM_NAME})

cmake_minimum_required(VERSION 3.13)

set(PROGRAM_NAME pico_w_webserver)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(pico_w_webserver)

pico_sdk_init()

message("Running makefsdata python script")
execute_process(COMMAND
    py makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)



add_executable(${PROGRAM_NAME}
    main.c
    led.c
    led.h
    motor_driver.c
    motor_driver.h
    ir_sensor.c
    ir_sensor.h
    eye.c
    eye.h
    gyroscope.c
    gyroscope.h
    buzzer.c
    buzzer.h
    ultrasonic.c
    ultrasonic.h
)

target_include_directories(${PROGRAM_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROGRAM_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_spi
    hardware_i2c
    hardware_irq
    hardware_gpio
    hardware_timer
    pico_lwip_http
    pico_time
)

pico_enable_stdio_usb(${PROGRAM_NAME} 1)
pico_enable_stdio_uart(${PROGRAM_NAME} 0)


pico_add_extra_outputs(${PROGRAM_NAME})