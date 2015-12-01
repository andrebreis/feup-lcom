#include <minix/drivers.h>

#include "test4.h"


static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"packet <number of packets>\"\n"
	 "\t service run %s -args \"async <duration>\"\n"
	 "\t service run %s -args \"config\"\n"
	 "\t service run %s -args \"gesture <length> <tolerance>\"\n",
	 argv[0], argv[0], argv[0], argv[0]);
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
	  printf("test4: parse_ulong: no digits were found in %s \n", str);
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
	  printf("test4: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static int proc_args(int argc, char *argv[]) {

  unsigned short numPackets, duration, tolerance;
  short length;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
	  if( argc != 3 ) {
		  printf("test4: Wrong number of arguments for test of test_packet()\n");
		  return 1;
	  }
	  if((numPackets = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("test4: test_packet(%d)\n", numPackets); /* Actually, it was already invoked */
	  return test_packet(numPackets);
  }
  else if (strncmp(argv[1], "async", strlen("async")) == 0) {
  	  if( argc != 3 ) {
  		  printf("test4: Wrong number of arguments for test of test_async()\n");
  		  return 1;
  	  }
  	  if((duration = parse_ulong(argv[2], 10)) == ULONG_MAX)
  		  return 1;
  	  printf("test4: test_async(%d)\n", duration); /* Actually, it was already invoked */
  	  return test_async(duration);
    }
  else if (strncmp(argv[1], "config", strlen("config")) == 0) {
	  if( argc != 2 ) {
		  printf("test4: wrong number of arguments for test of test_config() \n");
		  return 1;
	  }
	  printf("test4: test_config()\n"); /* Actually, it was already invoked */
	  test_config();
	  return 0;
  }
  else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
	  if( argc != 4 ) {
		  printf("test4: Wrong number of arguments for test of test_gesture()\n");
		  return 1;
	  }
	  if((length = parse_long(argv[2], 10)) == LONG_MAX)
		  return 1;
	  if((tolerance = parse_ulong(argv[3], 10)) == ULONG_MAX)
		  return 1;
	  printf("test4: test_gesture(%d, %d)\n", length, tolerance); /* Actually, it was already invoked */
	  return test_gesture(length, tolerance);
  }
  else {
	  printf("test4: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

int main(int argc, char **argv) {

	sef_startup();
    /* Enable IO-sensitive operations for ourselves */
   // sys_enable_iop(SELF);

	if ( argc == 1 ) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}
