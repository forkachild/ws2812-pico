// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ----------- //
// inmp441_pio //
// ----------- //

#define inmp441_pio_wrap_target 0
#define inmp441_pio_wrap 9

#define inmp441_pio_CLOCK_DIV_INT 40
#define inmp441_pio_CLOCK_DIV_FRAC 160
#define inmp441_pio_BITS_PER_WORD 26

static const uint16_t inmp441_pio_program_instructions[] = {
            //     .wrap_target
    0x5861, //  0: in     null, 1         side 3     
    0xe039, //  1: set    x, 25           side 0     
    0x4801, //  2: in     pins, 1         side 1     
    0x0042, //  3: jmp    x--, 2          side 0     
    0xe826, //  4: set    x, 6            side 1     
    0xa042, //  5: nop                    side 0     
    0x0845, //  6: jmp    x--, 5          side 1     
    0xf03f, //  7: set    x, 31           side 2     
    0xb842, //  8: nop                    side 3     
    0x1048, //  9: jmp    x--, 8          side 2     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program inmp441_pio_program = {
    .instructions = inmp441_pio_program_instructions,
    .length = 10,
    .origin = -1,
};

static inline pio_sm_config inmp441_pio_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + inmp441_pio_wrap_target, offset + inmp441_pio_wrap);
    sm_config_set_sideset(&c, 2, false, false);
    return c;
}

#include "hardware/clocks.h"
#include "hardware/gpio.h"
static inline void inmp441_pio_program_init(PIO pio, uint sm, uint offset,
                                            uint sck_pin, uint ws_pin,
                                            uint data_pin) {
    pio_sm_set_pindirs_with_mask(
        pio, sm, (1u << sck_pin) | (1u << ws_pin) | (0u << data_pin),
        (1u << sck_pin) | (1u << ws_pin) | (1u << data_pin));
    pio_gpio_init(pio, sck_pin);
    pio_gpio_init(pio, ws_pin);
    pio_gpio_init(pio, data_pin);
    gpio_pull_down(data_pin);
    pio_sm_config c = inmp441_pio_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, sck_pin);
    sm_config_set_in_pins(&c, data_pin);
    sm_config_set_in_shift(&c, false, true, inmp441_pio_BITS_PER_WORD);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    sm_config_set_clkdiv_int_frac(&c, inmp441_pio_CLOCK_DIV_INT, inmp441_pio_CLOCK_DIV_FRAC);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void inmp441_pio_program_deinit(PIO pio, uint sm) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
}

#endif
