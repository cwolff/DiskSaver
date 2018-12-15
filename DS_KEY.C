/*
 *************************************************************************
 *                                                                       *
 *      DiskSaver (C)opyright 1985 Personal Touch Software Inc.          *
 *                                                                       *
 *************************************************************************
*/

/*
 * Include files
 */

#include "disk.h"
#include "dos.h"

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              get_key()                                |
 |                                                                       |
 |                          Get the next key                             |
 |                                                                       |
 |                 Returns : scan code and ascii character               |
 |                                                                       |
 -------------------------------------------------------------------------
*/

int
get_key()

{

   union REGS inregs;      /* Used for interrupt registers           */
   union REGS outregs;

   /*
    * Get next key
    */

   inregs.x.ax = 0x0000;

   int86(0x0016, &inregs, &outregs);

   return(outregs.x.ax);

}

