/*
 *************************************************************************
 *                                                                       *
 *     DiskSaver tm (C)opyright 1985 Personal Touch Software Inc.        *
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
 |                             reset_disk()                              |
 |                                                                       |
 -------------------------------------------------------------------------
*/

reset_disk()

{

   union REGS inregs;             /* Used for interrupt registers          */
   union REGS outregs;
   struct SREGS segregs;

   /*
    * Reset the drive
    */

   inregs.x.ax = 0x0000;

   int86x(0x13, &inregs, &outregs, &segregs);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                 read_sector(drive,side,track,sector,cdta)             |
 |                                                                       |
 |             Read disk in drive for side track sector into cdta        |
 |                                                                       |
 -------------------------------------------------------------------------
*/

read_sector(drive,side,track,sector,cdta)

unsigned int drive;           /* Drive selector (A = 0, B = 1, etc)          */
unsigned int side;            /* Disk side (0 or 1)                          */
unsigned int track;           /* Disk track (0 to 39)                        */
unsigned int sector;          /* Disk sector (1 to 8 DOS 1.X 1 to 9 DOS 2.x) */
char *cdta;                   /* Pointer to current DTA                      */

{

   struct SREGS segregs;          /* Used for segment registers            */
   union REGS inregs;             /* Used for interrupt registers          */
   union REGS outregs;

   /*
    * Reset the drive as a precaution
    */

   reset_disk();

   /*
    * Determine DS (Data Segment)
    */

   segread(&segregs);

   /*
    * Set up interrupt registers for
    * Reading * sectors
    * into the dta
    * starting at track 0 sector 1
    * from drive 0 side 0
    * ES = DS
    */

   inregs.x.ax = 0x0201;
   inregs.x.bx = (int) cdta;
   inregs.x.cx = (track << 8) | sector;
   inregs.x.dx = (side << 8) | drive;
   segregs.es = segregs.ds;

   int86x(0x13, &inregs, &outregs, &segregs);

   /*
    * Check to see if disk operation successful
    */

   if((outregs.x.ax >> 8) == 0)
   {
      return(0);
   }
   else
   {
      return(outregs.x.ax >> 8);
   }

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                          write_sector()                               |
 |                                                                       |
 |                 Write current dta to specified sector                 |
 |                                                                       |
 -------------------------------------------------------------------------
*/

write_sector(drive,side,track,sector,cdta)

unsigned int drive;           /* Drive selector (A = 0, B = 1, etc)          */
unsigned int side;            /* Disk side (0 or 1)                          */
unsigned int track;           /* Disk track (0 to 39)                        */
unsigned int sector;          /* Disk sector (1 to 8 DOS 1.X 1 to 9 DOS 2.x) */
char *cdta;                   /* Pointer to current DTA                      */

{

   struct SREGS segregs;          /* Used for segment registers            */
   union REGS inregs;             /* Used for interrupt registers          */
   union REGS outregs;

   /*
    * Reset the drive as a precaution
    */

   reset_disk();

   /*
    * Determine DS (Data Segment)
    */

   segread(&segregs);

   /*
    * Set up interrupt registers for writing sectors into the dta
    * starting at track, sector, side for drive
    * ES = DS
    */

   inregs.x.ax = 0x0301;
   inregs.x.bx = (int) cdta;
   inregs.x.cx = (track << 8) | sector;
   inregs.x.dx = (side << 8) | drive;
   segregs.es = segregs.ds;

   int86x(0x13, &inregs, &outregs, &segregs);

   /*
    * Check to see if disk operation successful
    */

   if((outregs.x.ax >> 8) == 0)
   {
      return(0);
   }
   else
   {
      return(outregs.x.ax >> 8);
   }

}
