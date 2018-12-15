/*
 *************************************************************************
 *                                                                       *
 *      DiskSaver (C)opyright 1985 Personal Touch Software Inc.          *
 *                                                                       *
 *************************************************************************
*/

/*
 * Include file
 */

#include "disk.h"

/*
** Gloabal storage
*/

char Dta[512];             /* Disk Transfer Area */

unsigned int Address_6845; /* Address of motorola 6845 */

unsigned int Attribute;    /* Screen attribute */

unsigned int Row_col;      /* Row and Column of screen map */

unsigned int Video_addr;   /* Address of video buffer */

unsigned int Video_seg;    /* Segment of video memory */

struct t_scrn_map Scrn_map[2000];   /* Video memory map */
