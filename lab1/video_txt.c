#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

/* Custom Variables */

int elementSize, lineSize;

void vt_fill(char ch, char attr) {
	int i;
	for(i = 0; i < scr_lines*scr_width*elementSize ; i+=elementSize){
		video_mem[i] = ch;
		video_mem[i+1] = attr;
	}

}

void vt_blank() {
	vt_fill(00, 00);
}

int vt_print_char(char ch, char attr, int r, int c) {

	if(r > scr_lines-1 || c > scr_width-1 || r < 0 || c < 0)
		return -1;
	else{
		video_mem[r*lineSize+c*elementSize]=ch;
		video_mem[r*lineSize+c*elementSize+1]=attr;
		return 0;
	}

}

int vt_print_string(char *str, char attr, int r, int c) {
	if(r*lineSize+c*elementSize+(strlen(str)-1)*elementSize < scr_lines*scr_width*elementSize){
		int i;
		for(i = 0; i<strlen(str)*elementSize; i+=elementSize){
			video_mem[r*lineSize+c*elementSize+i] = str[i/2];
			video_mem[r*lineSize+c*elementSize+i+1]=attr;
		}
		return 0;
	}
	else
		return -1;

}

int vt_print_int(int num, char attr, int r, int c) {

	/* To complete ... */

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

	/* To complete ... */

}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes)(vi_p->vram_base);
	mr.mr_limit = mr.mr_base + vi_p->vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	/* Save text mode resolution */

	scr_lines = vi_p->scr_lines;
	scr_width = vi_p->scr_width;

	/* Custom Variables */

	elementSize = 2;
	lineSize = scr_width*elementSize;

	return video_mem;
}
