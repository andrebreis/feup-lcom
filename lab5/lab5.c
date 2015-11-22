#include <minix/drivers.h>

#include "test5.h"
#include "pixmap.h"

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					" service run %s -args \"init <mode> <delay>\" \n"
					" service run %s -args \"square <xi> <yi> <size> <color>\" \n"
					" service run %s -args \"line <xi> <yi> <xf> <yf> <color>\" \n"
					" service run %s -args \"xpm <xi> <yi> <xpm>\" \n"
					" service run %s -args \"move <xi> <yi> <xpm> <hor> <delta> <time>\" \n"
					" service run %s -args \"controller\" \n", argv[0], argv[0],
			argv[0], argv[0], argv[0], argv[0]);
}

static unsigned long parse_ulong(char *str, int base) {

	char *endptr;

	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX)

	|| (errno != 0 && val == 0)) {

		perror("strtol");

		return ULONG_MAX;

	}

	if (endptr == str) {

		printf("test5: parse_ulong: no digits were found in %s \n", str);

		return ULONG_MAX;

	}

	/* Successful conversion */

	return val;

}

static long parse_long(char *str, int base) {

	char *endptr;

	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))

	|| (errno != 0 && val == 0)) {

		perror("strtol");

		return LONG_MAX;

	}

	if (endptr == str) {

		printf("test5: parse_long: no digits were found in %s \n", str);

		return LONG_MAX;

	}

	/* Successful conversion */

	return val;

}

static int proc_args(int argc, char *argv[]) {

	unsigned short hor, time, size, color, xf, yf, xi, yi, mode, delay;
	short delta;
	long num;
	char *str;
	int i;

	/* check the function to test: if the first characters match, accept it */

	if (strncmp(argv[1], "init", strlen("init")) == 0) {

		if (argc != 4) {

			printf(
					"test5: Wrong number of arguments for test of test_init()\n");

			return 1;

		}

		if ((mode = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		if ((delay = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;

		printf("test5: test_init(%lu, %lu)\n", mode, delay); /* Actually, it was already invoked */

		test_init(mode, delay);

		return 0;

	}

	else if (strncmp(argv[1], "square", strlen("square")) == 0) {

		if (argc != 6) {

			printf(
					"test5: Wrong number of arguments for test of test_square()\n");

			return 1;

		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((size = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;

		printf("test5: test_square(%lu, %lu, %lu, %lu)\n", xi, yi, size, color); /* Actually, it was already invoked */

		return test_square(xi, yi, size, color);

	}

	else if (strncmp(argv[1], "line", strlen("line")) == 0) {

		if (argc != 7) {

			printf(
					"test5: wrong number of arguments for test of test_line() \n");

			return 1;

		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((xf = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((yf = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;

		printf("test5: test_line(%lu, %lu, %lu, %lu, %lu)\n", xi, yi, xf, yf,
				color); /* Actually, it was already invoked */

		return test_line(xi, yi, xf, yf, color);

	}

	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {

			printf("test5: Wrong number of arguments for test of test_xpm()\n");

			return 1;

		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) {
			printf("test5: test_xpm(%lu, %lu, pic1)\n", xi, yi);
			return test_xpm(xi, yi, pic1);
		}
		if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) {
			printf("test5: test_xpm(%lu, %lu, pic2)\n", xi, yi);
			return test_xpm(xi, yi, pic2);
		}
		if (strncmp(argv[4], "cross", strlen("cross")) == 0) {
			printf("test5: test_xpm(%lu, %lu, cross)\n", xi, yi);
			return test_xpm(xi, yi, cross);
		}
		if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) {
			printf("test5: test_xpm(%lu, %lu, pic3)\n", xi, yi);
			return test_xpm(xi, yi, pic3);
		}
		if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) {
			printf("test5: test_xpm(%lu, %lu, penguin)\n", xi, yi);
			return test_xpm(xi, yi, penguin);
		} else {
			printf("test5: test_xpm :: unrecognized xpm\n");
			return 1;
		}
	}

	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {

			printf(
					"test5: Wrong number of arguments for test of test_move()\n");

			return 1;

		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((hor = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((delta = parse_long(argv[6], 10)) == LONG_MAX)
			return 1;
		if ((time = parse_ulong(argv[7], 10)) == ULONG_MAX)
			return 1;

		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) {
			printf("test5: test_move(%lu, %lu, pic1, %lu, %d, %lu)\n", xi, yi,
					hor, delta, time);
			return test_move(xi, yi, pic1, hor, delta, time);
		}
		if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) {
			printf("test5: test_move(%lu, %lu, pic2, %lu, %d, %lu)\n", xi, yi,
					hor, delta, time);
			return test_move(xi, yi, pic2, hor, delta, time);
		}
		if (strncmp(argv[4], "cross", strlen("cross")) == 0) {
			printf("test5: test_move(%lu, %lu, cross, %lu, %d, %lu)\n", xi, yi,
					hor, delta, time);
			return test_move(xi, yi, cross, hor, delta, time);
		}
		if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) {
			printf("test5: test_move(%lu, %lu, pic3, %lu, %d, %lu)\n", xi, yi,
					hor, delta, time);
			return test_move(xi, yi, pic3, hor, delta, time);
		}
		if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) {
			printf("test5: test_move(%lu, %lu, penguin, %lu, %d, %lu)\n", xi,
					yi, hor, delta, time);
			return test_move(xi, yi, penguin, hor, delta, time);
		} else {
			printf("test5: test_move :: unrecognized xpm\n");
			return 1;
		}

	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {

			printf(
					"test5: Wrong number of arguments for test of test_controller()\n");

			return 1;

		}
		printf("test5: test_controller()\n");

		return test_controller();
	} else {

		printf("test5: non valid function \"%s\" to test\n", argv[1]);

		return 1;

	}
}

int main(int argc, char **argv) {
	sef_startup();
	/* Enable IO-sensitive operations for ourselves */
	// sys_enable_iop(SELF);
	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {

		proc_args(argc, argv);

	}

	return 0;

}
