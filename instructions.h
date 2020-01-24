#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include "Joystick.h"

#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

typedef enum {
    circle_around,
    get_on_bike,
    get_off_bike,
    turn_left,
    turn_right,
    move_forward,
    move_backward,
    move_right,
    move_left,
    open_menu,
    close_menu,
    press_a,
    press_b,
    press_x,
    press_y,
    reset_view_angle,
    press_plus,
    press_home,
    hang
} action_t;

typedef struct {
    action_t action;
    uint16_t duration;
} command_t;

static const command_t wake_up_hang[] = {
    { hang,  150 }
};

static const command_t fly_to_breading_steps[] = {
    { hang, 50 },
    { press_a, 5 },
    { hang, 50 },
    { press_x, 7},
    { hang, 70 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 }
};

static const command_t get_egg_steps[] = {
    { move_backward, 70 }, // need to tune
    {reset_view_angle, 5 },
    { move_right, 10 },
    { press_a, 5 },
    { hang, 20 },
    { press_a, 5 },
    { hang, 20 },
    { move_backward, 5 },
    { hang, 20 },
    { move_backward, 5 },
    { hang, 20 },
    { press_a, 5 },
    { hang, 70 },
    { press_a, 5 },
    { hang, 5 },
    { move_right, 25 },
    { hang, 60 },
    { press_a, 5 },
    { hang, 70 },
    { press_a, 5 },
    { hang, 70 },
    { press_a, 5 },
    { hang, 70 },
};

static command_t swap_slot_2[] = {
    { move_backward, 5 }, 
    { hang, 30 }
};

static command_t swap_slot_3[] = {
    { move_backward, 5 }, 
    { hang, 30 }, 
    { move_backward, 5 },
    { hang, 30 }
};

static command_t swap_slot_4[] = {
    { move_backward, 5 }, 
    { hang, 30 }, 
    { move_backward, 5 }, 
    { hang, 30 }, 
    { move_backward, 5 }, 
    { hang, 30 }
};

static command_t swap_slot_5[] = {
    { move_forward, 5 }, 
    { hang, 30 }, 
    { move_forward, 5 }, 
    { hang, 30 }
};

static command_t swap_slot_6[] = {
    { move_forward, 5 },
    { hang, 30 }
};

static const command_t* swap_slot[] = {
    swap_slot_2,
    swap_slot_3,
    swap_slot_4,
    swap_slot_5,
    swap_slot_6
};

static const uint8_t swap_slot_size[] = {
    2, 4, 6, 4, 2
};

static const command_t swap_pokemon_steps[] = {
    { press_a, 10 },
    { hang, 100 },
    { press_b, 10 },
    { hang, 100 },
    { press_b, 10 },
    { hang, 100 },
    { press_b, 10 },
    { hang, 70 },
    { press_b, 10 },
    { hang, 70 }
};

static const command_t breeding_prep_steps[] = {
    { move_backward, 30 },
    { hang, 10 },
    { move_left, 80 },
    { hang, 5 },
    { press_plus, 5 },
    { hang, 5 },
};

static const command_t next_round_steps[] = {
    { hang, 20 },
    { press_plus, 5 },
    { hang, 20 }
};

static const uint16_t breeding_duration = 3000;

#endif