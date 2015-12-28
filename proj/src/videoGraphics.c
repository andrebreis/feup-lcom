#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "Mouse.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */
static unsigned int videoMemSize;
static char *videoMem; /* Process address to which VRAM is mapped */
static char *mouseBuffer;
static char *buffer;

static unsigned hRes; /* Horizontal screen resolution in pixels */
static unsigned vRes; /* Vertical screen resolution in pixels */
static unsigned bitsPerPixel; /* Number of VRAM bits per pixel */

int videoGraphicsExit() {
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

void *videoGraphicsInit(unsigned short mode) {
	struct reg86u r;
	vbe_mode_info_t vmi_p;
	int returnValue;
	r.u.w.ax = SET_VBE_MODE; // VBE call, function 02 -- set VBE mode - set vi
	r.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	r.u.b.intno = INT_VIDEO;

	if (sys_int86(&r) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}

	vbe_get_mode_info(mode, &vmi_p);

	hRes = vmi_p.XResolution;
	vRes = vmi_p.YResolution;
	bitsPerPixel = vmi_p.BitsPerPixel;

	videoMemSize = (hRes * vRes * bitsPerPixel) / 8;

	struct mem_range memRange;
	memRange.mr_base = (phys_bytes)(vmi_p.PhysBasePtr);
	memRange.mr_limit = memRange.mr_base + videoMemSize;

	if (OK != (returnValue = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &memRange)))
		panic("video_grafic: sys_privctl (ADD_MEM) failed: %d\n", returnValue);

	videoMem = vm_map_phys(SELF, (void *) memRange.mr_base, videoMemSize);
	mouseBuffer = (char*) malloc(videoMemSize);
	buffer = (char*) malloc(videoMemSize);

	return videoMem;

}

unsigned long getPixelPosition(unsigned short x, unsigned short y) {
	return y * hRes + x;
}

unsigned getHRes() {
	return hRes;
}

unsigned getVRes() {
	return vRes;
}

unsigned getBytesPerPixel(){
	return (bitsPerPixel/8);
}

unsigned int getVideoMemSize(){
	return videoMemSize;
}

char* getVideoMem(){
	return videoMem;
}

char* getMouseBuffer(){
	return mouseBuffer;
}

char* getBuffer(){
	return buffer;
}

void flipBuffer(){
	memcpy(videoMem, buffer, videoMemSize);
}

void flipMouseBuffer(){
	memcpy(videoMem, mouseBuffer, videoMemSize);
}

void drawMouse(){
	memcpy(getMouseBuffer(), getBuffer(), getVideoMemSize());
	drawMouseBitmap(getMouse()->icon, getMouse()->cornerX, getMouse()->cornerY, ALIGN_LEFT);
}
