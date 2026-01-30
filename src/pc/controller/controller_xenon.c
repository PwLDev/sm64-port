#ifdef TARGET_XENON
#include <input/input.h>
#include <usb/usbmain.h>

#include <ultra64.h>
#include "controller_api.h"

// macro taken from pcsxr-xenon
#define	STICK_DEAD_ZONE (32768 * 0.3)
#define HANDLE_STICK_DEAD_ZONE(x) ((((x)>-STICK_DEAD_ZONE) && (x)<STICK_DEAD_ZONE)?0:(x-x/abs(x)*STICK_DEAD_ZONE))

static void controller_xenon_init(void) {
    usb_init();
    usb_do_poll();
}

static void controller_xenon_read(OSContPad *pad) {
    struct controller_data_s controller;

    // poll the latest info from the controller
    usb_do_poll();
    get_controller_data(&controller, 0);

    if (controller.a)
        pad->button |= A_BUTTON;
    if (controller.b)
        pad->button |= B_BUTTON;
    if (controller.x)
        pad->button |= B_BUTTON;
    if (controller.start)
        pad->button |= START_BUTTON;
    if (controller.up)
        pad->button |= U_JPAD;
    if (controller.down)
        pad->button |= D_JPAD;
    if (controller.left)
        pad->button |= L_JPAD;
    if (controller.right)
        pad->button |= R_JPAD;
    if (controller.lb)
        pad->button |= Z_TRIG;
    if (controller.rb)
        pad->button |= R_TRIG;

    pad->stick_x = (s8)HANDLE_STICK_DEAD_ZONE(controller.s1_x) / 409;
    pad->stick_y = (s8)HANDLE_STICK_DEAD_ZONE(controller.s1_y) / 409;

    uint32_t magnitude_sq = (uint32_t)(pad->stick_x * pad->stick_x) + (uint32_t)(pad->stick_y * pad->stick_y);
    if (magnitude_sq < (uint32_t)(STICK_DEAD_ZONE * STICK_DEAD_ZONE)) {
        pad->stick_x = 0;
        pad->stick_y = 0;
    }

    if (controller.s2_x > STICK_DEAD_ZONE)
        pad->button |= L_CBUTTONS;
    if (controller.s2_x < -STICK_DEAD_ZONE)
        pad->button |= R_CBUTTONS;
    if (controller.s2_y > STICK_DEAD_ZONE)
        pad->button |= U_CBUTTONS;
    if (controller.s2_y < -STICK_DEAD_ZONE)
        pad->button |= D_CBUTTONS;
}

struct ControllerAPI controller_xenon = {
    controller_xenon_init,
    controller_xenon_read
};

#endif