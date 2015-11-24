#include <minix/drivers.h>

#include "timer.h"
#include "keyboard.h"
#include "utilities.h"
#include "read_xpm.h"

#define GET_VBE_CONTROLLER_INFO 0x4F00

void *test_init(unsigned short mode, unsigned short delay) {

	vg_init(mode);

	timer_test_int(delay);

	if (vg_exit() == 1)

		return 1;

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	vg_init(0x105);

	vg_draw_square(x, y, size, color);

	waitFor(ESC);

	vg_exit();

}

int test_line(unsigned short xi, unsigned short yi,

unsigned short xf, unsigned short yf, unsigned long color) {

	vg_init(0x105);

	vg_draw_line(xi, yi, xf, yf, color);

	waitFor(ESC);

	vg_exit();

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	vg_init(0x105);

	vg_draw_xpm(xi, yi, xpm);

	waitFor(ESC);

	vg_exit();

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],

unsigned short hor, short delta, unsigned short time) {

	vg_init(0x105);

	vg_move_xpm(xi, yi, xpm, hor, delta, time);

	vg_exit();


}

int test_controller() {

	/* To be completed */

}

