#include <minix/drivers.h>
#include <minix/syslib.h>
#include "timer.h"

int main(int argc, char **argv) {

  /* Initialize service */

  sef_startup();

  timer_test_int(15);
  return 0;

}
