#ifndef __VIDEOGRAPHICS_H
#define __VIDEOGRAPHICS_H

/** @defgroup videoGraphics videoGraphics
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *videoGraphicsInit(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int videoGraphicsExit(void);

/**
 * @brief returns a pixel position on a matrix given its x and y
 */
unsigned long getPixelPosition(unsigned short x, unsigned short y);

/**
 * @brief returns the video mode horizontal resolution
 */
unsigned getHRes();

/**
 * @brief returns the video mode vertical resolution
 */
unsigned getVRes();

/**
 * @brief returns a pointer to the video memory
 */
char* getVideoMem();

/**
 * @brief returns a pointer to the mouse buffer
 */
char* getMouseBuffer();

/**
 * @brief returns a pointer to the graphics buffer
 */
char* getBuffer();

/**
 * @brief copies the content of the graphics buffer to the video memory
 */
void flipBuffer();

/**
 * @brief copies the content of the mouse buffer to the video memory
 */
void flipMouseBuffer();

/**
 * @brief copies the graphics buffer to the mouse buffer, and draws the mouse on the mouse buffer
 */
void drawMouse();

 /** @} end of videoGraphics */
 
#endif /* __VIDEOGRAPHICS_H */
