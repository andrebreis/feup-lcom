#include <minix/drivers.h>
#include <minix/syslib.h>
#include "timer.h"

int main(int argc, char **argv) {

  /* Initialize service */

  sef_startup();

  timer_test_config(0);
  timer_test_square(240);
  timer_test_config(0);
  return 0;

}
