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
    { hang, 100 }
};

static const command_t fly_to_breeding_steps[] = {
    // need to tune timings
    { hang, 10 },
    { press_a, 5 },
    { hang, 10 },
    { press_x, 7},
    { hang, 25 },
    { press_a, 5 },
    { hang, 85 },
    { press_a, 5 },
    { hang, 40 },
    { press_a, 5 },
    { hang, 70 },
};

static const command_t get_egg_steps[] = {
    { hang, 10 },
    // need to tune movement
    { move_backward, 70 },
    { reset_view_angle, 5 },
    { hang, 5 },
    { move_right, 5 },

    // a spam in case no egg, take total of as and hangs as hang if egg
    { press_a, 5 },
    { hang, 30 },
    { press_a, 5 },
    { hang, 20 },
    // move down twice in case no egg
    { move_backward, 5 },
    { hang, 5 },
    { move_backward, 5 },
    { hang, 5 },

    { press_a, 5 },
    // this bottom hang a little long
    { hang, 130 },
    { press_a, 5 },
    // move left if no egg would have left dialogue if thats the case
    // take total as hang if egg
    { hang, 30 },
    { move_left, 10 },
    { hang, 40 },
    { press_a, 5 },
    { hang, 50 },
    { press_a, 5 },
    { hang, 70 },
    // { press_a, 5 },
    // { hang, 70 },
    // { press_a, 5 },
    // { hang, 70 },
};

static command_t swap_slot_2[] = {
    { move_backward, 5 }, 
    { hang, 5 }
};

static command_t swap_slot_3[] = {
    { move_backward, 5 }, 
    { hang, 5 }, 
    { move_backward, 5 },
    { hang, 5 }
};

static command_t swap_slot_4[] = {
    { move_backward, 5 }, 
    { hang, 5 }, 
    { move_backward, 5 }, 
    { hang, 5 }, 
    { move_backward, 5 }, 
    { hang, 5 }
};

static command_t swap_slot_5[] = {
    { move_forward, 5 }, 
    { hang, 5 }, 
    { move_forward, 5 }, 
    { hang, 5 }
};

static command_t swap_slot_6[] = {
    { move_forward, 5 },
    { hang, 5 }
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
    // need to tune these
    { press_a, 10 },
    { hang, 50 },
    { press_b, 10 },
    { hang, 50 },
    { press_b, 10 },
    { hang, 30 },
    { press_b, 10 },
    { hang, 30 },
    { press_b, 10 },
    { hang, 10 }
};

static const command_t breeding_prep_steps[] = {
    { move_backward, 35 },
    { hang, 5 },
    { move_left, 90 },
    { hang, 5 },
    { press_plus, 5 },
    { hang, 5 },
};

static const command_t next_round_steps[] = {
    { hang, 5 },
    { press_plus, 5 },
    { hang, 5 }
};

static const uint16_t breeding_duration = 2250; // CHANGE THIS VALUE DEPENDING ON SCRIPT RESULTS

#endif