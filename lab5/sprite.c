#include <minix/drivers.h>

#include "sprite.h"
#include "video_gr.h"

/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[], int x, int y) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	int i, j;
	sp->x = x;
	sp->y = y;
	return sp;
}

void destroy_sprite(Sprite *sp, char *base) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
	// hopeless: pointer is passed by value
}

/* Some useful non-visible functions */
int check_collision(Sprite *sp, char *base) {
	/*...*/
}

int draw_sprite(Sprite *sp, char *base) {
	if (sp->x + sp->width > getHRes() || sp->y + sp->height > getVRes())
		return -1;

	int i, j;
	for (i = 0; i < sp->height; i++) {
		for (j = 0; j < sp->width; j++) {
			base[getPixelPosition(sp->x + j, sp->y + i)] = sp->map[i * sp->width
					+ j];
		}
	}
	return 0;
}

void clear_sprite(Sprite *sp, char *base) {
	int i, j;
	for (i = 0; i < sp->height; i++) {
		for (j = 0; j < sp->width; j++) {
			base[getPixelPosition(sp->x + j, sp->y + i)] = 0;
		}
	}
}

int animate_sprite(Sprite *sp, char *base, int x, int y) {
	clear_sprite(sp, base);
	sp->x = x;
	sp->y = y;
	draw_sprite(sp, base);
}
