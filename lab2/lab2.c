#include <minix/drivers.h>
#include <minix/syslib.h>
#include "timer.h"

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"timer <timer from 0 to 2>\"\n"
	 "\t service run %s -args \"frequency <frequency>\"\n"
	 "\t service run %s -args \"duration <duration>\"\n",
	 argv[0], argv[0], argv[0]);
}



static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
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
	  printf("timer_txt: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static int proc_args(int argc, char *argv[]) {

  unsigned long timer, frequency, duration;
  char *str;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "timer", strlen("timer")) == 0) {
	  if( argc != 3 ) {
		  printf("timer_txt: wrong no of arguments for test of timer_test_config() \n");
		  return 1;
	  }
	  if((timer = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("timer_txt:: timer_test_config(%lu)\n", timer); /* Actually, it was already invoked */
	  return timer_test_config(timer);
  }
  else if (strncmp(argv[1], "frequency", strlen("frequency")) == 0) {
	  if( argc != 3 ) {
		  printf("timer_txt: wrong no of arguments for test of timer_test_square() \n");
		  return 1;
	  }
	  if((frequency = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("timer_txt:: timer_test_square(%lu)\n", frequency); /* Actually, it was already invoked */
	  return timer_test_square(frequency);
  }
  else if (strncmp(argv[1], "duration", strlen("duration")) == 0) {
	  if( argc != 3 ) {
		  printf("timer_txt: wrong no of arguments for test of timer_test_int() \n");
		  return 1;
	  }
	  if((duration = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("timer_txt:: timer_test_int(%lu)\n", duration); /* Actually, it was already invoked */
	  return timer_test_int(duration);
  }
  else {
	  printf("timer_txt: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

int main(int argc, char **argv) {

	sef_startup();

	if ( argc == 1 ) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}
