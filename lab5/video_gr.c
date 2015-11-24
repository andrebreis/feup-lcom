#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "sprite.h"
#include "keyboard.h"
#include "timer.h"

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

static int timerHook = TIMER0_IRQ;
static int kbHook = KB_IRQ;

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

	video_mem_size = (h_res * v_res * bits_per_pixel) / 8;

	struct mem_range memRange;
	memRange.mr_base = VRAM_PHYS_ADDR;
	memRange.mr_limit = memRange.mr_base + video_mem_size;

	if (OK != (returnValue = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &memRange)))
		panic("video_grafic: sys_privctl (ADD_MEM) failed: %d\n", returnValue);

	video_mem = vm_map_phys(SELF, (void *) memRange.mr_base, video_mem_size);

	return video_mem;

}

unsigned long getPixelPosition(unsigned short x, unsigned short y) {
	return y * h_res + x;
}

unsigned getHRes() {
	return h_res;
}

unsigned getVRes() {
	return v_res;
}

int vg_draw_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	int i, j;
	if (x + size > h_res || y + size > v_res)
		return -1;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			video_mem[getPixelPosition(x + j, y + i)] = color;
		}
	}
}

int vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	if (xi < 0 || xf < 0 || yi < 0 || yf < 0)
		return -1;
	if (xi > h_res || xf > h_res || yi > v_res || yf > v_res)
		return -1;

	int steep = abs(yf - yi) > abs(xf - xi);

	if (steep) {
		// swap x1 and y1
		int tmp = xi;
		xi = yi;
		yi = tmp;
		// swap x2 and y2
		tmp = xf;
		xf = yf;
		yf = tmp;
	}
	if (xi > xf) {
		// swap x1 and x2
		int tmp = xi;
		xi = xf;
		xf = tmp;
		// swap y1 and y2
		tmp = yi;
		yi = yf;
		yf = tmp;
	}

	int dx, dy, error, ystep, y;

	dx = xf - xi;
	dy = abs(yf - yi);
	error = dx / 2;
	y = yi;

	if (yi < yf)
		ystep = 1;
	else
		ystep = -1;

	int x;
	for (x = xi; x <= xf; x++) {
		if (steep)
			video_mem[getPixelPosition(y, x)] = color;
		else
			video_mem[getPixelPosition(x, y)] = color;
		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}

	return 0;
}

int vg_draw_xpm(unsigned short xi, unsigned short yi, char* xpm[]) {
	Sprite *sp = create_sprite(xpm, xi, yi);
	draw_sprite(sp, video_mem);
	return 0;
}

int vg_move_xpm(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	int irq_kbc, irq_timer, counter = 0, returnValue, timeCounter = 0;
	float deltax, deltay;
	unsigned char firstByte = 0;
	int r, ipc_status;
	message msg;

	irq_timer = subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &timerHook);
	irq_kbc = subscribe_int(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbHook);

	if (irq_timer == -1 || irq_kbc == -1)
		return -1;

	float pixelsPerTick = (float) delta / (time*60);


	Sprite *sp = create_sprite(xpm, xi, yi);

	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_kbc) { /* subscribed interrupt */
					returnValue = kbdReadKey(&firstByte);
					if (returnValue != 0)
						return returnValue;
					if (firstByte == ESC) {
						unsubscribe_int(&kbHook);
						unsubscribe_int(&timerHook);
						return 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_timer) {
					if (timeCounter < time*60) {
						deltax = xi + pixelsPerTick * timeCounter * (hor != 0);
						deltay = yi + pixelsPerTick * timeCounter * (hor == 0);
						animate_sprite(sp, video_mem, (int)deltax, (int)deltay);
					}
					timeCounter++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}

}
