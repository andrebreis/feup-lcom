#include <minix/drivers.h>

#include "timer.h"

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

	timer_test_int(5);

	vg_exit();

}

int test_line(unsigned short xi, unsigned short yi,

unsigned short xf, unsigned short yf, unsigned long color) {

	/* To be completed */

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	/* To be completed */

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],

unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

int test_controller() {

	/* To be completed */

}

