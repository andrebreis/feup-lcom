#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	  0xE0000000
#define H_RES             1024
#define V_RES		  	  768
#define BITS_PER_PIXEL	  8

/* Private global variables */
static unsigned int video_mem_size;
static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {
	struct reg86u r;
	int returnValue;
	r.u.w.ax = SET_VBE_MODE; // VBE call, function 02 -- set VBE mode - set vi
	r.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	r.u.b.intno = INT_VIDEO;

	if (sys_int86(&r) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}

	// vbe_get_mode_info(); to get resolutions and so

	h_res = H_RES;
	v_res = V_RES;
	bits_per_pixel = BITS_PER_PIXEL;

	video_mem_size = (h_res * v_res * bits_per_pixel)/8;

	struct mem_range memRange;
	memRange.mr_base = VRAM_PHYS_ADDR;
	memRange.mr_limit = memRange.mr_base + video_mem_size;

	if (OK != (returnValue = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &memRange)))
		panic("video_grafic: sys_privctl (ADD_MEM) failed: %d\n", returnValue);

	video_mem = vm_map_phys(SELF, (void *) memRange.mr_base, video_mem_size);

	return video_mem;

}

int vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color){
	int i, j;
	if(x + size > h_res || y + size > v_res)
		return -1;
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			video_mem[(y+i)*h_res+x+j] = color;
		}
	}


}
