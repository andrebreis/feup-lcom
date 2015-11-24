

#include "sprite.h"
#include "video_gr.h"

/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[], char *base) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			base[getPixelPosition(xi + j, yi + i)] = xpm[i * width + j];
		}
	}
	return sp;
}

void destroy_sprite(Sprite *sp, char *base) {
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
	// hopeless: pointer is passed by value
}

int animate_sprite(Sprite *sp, char *base) {
	/*...*/
}

/* Some useful non-visible functions */
static int check_collision(Sprite *sp, char *base) {
	/*...*/
}

static int draw_sprite(Sprite *sp, char *base) {
	/*...*/
}
