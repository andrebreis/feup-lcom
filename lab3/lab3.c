#include <minix/drivers.h>
#include <minix/syslib.h>
#include "test3.h"

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"scan <0 to C and 1 to Assembly>\"\n"
	 "\t service run %s -args \"leds <array size> <array with elements from 0 to 2>\"\n"
	 "\t service run %s -args \"timed_scan <duration>\"\n",
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

  unsigned short assembly, arraySize, *toggle, duration, arrayIndex, argvIndex, arrayElement;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
	  if( argc != 3 ) {
		  printf("test3: Wrong number of arguments for test of kbd_test_scan()\n");
		  return 1;
	  }
	  if((assembly = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("test3: kbd_test_scan(%d)\n", assembly); /* Actually, it was already invoked */
	  return kbd_test_scan(assembly);
  }
  else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
	  if( argc < 4 ) {
		  printf("test3: Wrong number of arguments for test of kbd_test_leds()\n");
		  return 1;
	  }
	  if((arraySize = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  if(argc != (3 + arraySize))
	  {
		  printf("The array size you inserted doesn't match the number of elements of the array\n");
		  return 1;
	  }
	  unsigned short leds[arraySize];
	  argvIndex = 3;
	  for(arrayIndex = 0; i < arraySize; arrayIndex++){
		  if((arrayElement = parse_ulong(argv[argvIndex], 10)) == ULONG_MAX)
			  return 1;
		  leds[arrayIndex] = arrayElement;
		  argvIndex++;
	  }
	  printf("test3: kbd_test_leds(%d, ["); /* Actually, it was already invoked */
	  for(arrayIndex = 0; arrayIndex < arraySize; arrayIndex++){
		  if(arrayIndex == arraySize - 1)
			  printf("%d])\n", leds[arrayIndex]);
		  else
			  printf("%d, ", leds[arrayIndex]);
	  }
	  return kbd_test_leds(arraySize, leds);
  }
  else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
	  if( argc != 3 ) {
		  printf("test3: Wrong number of arguments for test of kbd_test_timed_scan()\n");
		  return 1;
	  }
	  if((duration = parse_ulong(argv[2], 10)) == ULONG_MAX)
		  return 1;
	  printf("test3: kbd_test_timed_scan(%d)\n", duration); /* Actually, it was already invoked */
	  return kbd_test_timed_scan(duration);
  }
  else {
	  printf("test3: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

int main(int argc, char **argv) {

	sef_startup();
    /* Enable IO-sensitive operations for ourselves */
    sys_enable_iop(SELF);

	if ( argc == 1 ) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}
