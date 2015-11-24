#include <minix/drivers.h>

#include "timer.h"
#include "keyboard.h"
#include "utilities.h"
#include "read_xpm.h"

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

	int width, height;
	char *map;
	// get the pix map from the XPM
	map = read_xpm(xpm, &width, &height);

	vg_init(0x105);

	vg_draw_xpm(xi, yi, width, height, map);

	waitFor(ESC);

	vg_exit();

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],

		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

int test_controller() {

	/* To be completed */

}

