# rest of your project
add_executable(${PROJECT_NAME}
    main.cpp
    ili9341.cpp
    buffer.cpp
    map.cpp
    levelarch.cpp
    actor.cpp
    game.cpp
    tilesdata.cpp
    engine.cpp
    data.s
)
set_property(SOURCE data.s APPEND PROPERTY COMPILE_OPTIONS "-I" "${CMAKE_SOURCE_DIR}/data")

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    hardware_spi
    hardware_dma
    hardware_adc
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})