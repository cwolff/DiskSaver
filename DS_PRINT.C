/*
 *************************************************************************
 *                                                                       *
 *       DiskSaver tm (C)opyright WareWolf Program Publishers 1984       *
 *                                                                       *
 *************************************************************************
*/

/*
 * Include files
 */

#include "disk.h"
#include "dos.h"

/*
 * Externals
 */

extern char Attribute;
extern unsigned int Row_col;
extern unsigned int Video_addr;
extern unsigned int Video_seg;
extern unsigned int Address_6845;
extern struct t_scrn_map Scrn_map[2000];

extern int SHOW_VID();

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print(string)                            |
 |                                                                       |
 |   Display a string to the screen at the current cursor position       |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print(string)

char *string;     /* Pointer to string to display            */

{

   unsigned int offset;


   /*
    * Set attribute and number of character to output to one
    */

   offset = ((Row_col >> 8) * 80) + (Row_col & 0xff);

   while (*string != '\0')
   {
      Scrn_map[offset].character = *string;
      Scrn_map[offset].attribute = Attribute;
      string++;
      offset++;
   }

   Row_col = ((offset / 80) << 8) | (offset % 80);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                            locate(position)                           |
 |                                                                       |
 |              Locate the cursor to the position of row,col             |
 |                                                                       |
 -------------------------------------------------------------------------
*/

locate(position)

unsigned int position;        /* Row and Column to locate cursor        */

{

   union REGS inregs;        /* Used for interrupt registers    */
   union REGS outregs;

   Row_col = position;

   inregs.x.ax = 0x0200;
   inregs.x.bx = 0x0000;
   inregs.x.cx = 0x0000;
   inregs.x.dx = position;
		
   int86(0x0010, &inregs, &outregs);

   Row_col = position;

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              color(fore_back)                         |
 |                                                                       |
 |     Set the screens foreground and background display sttributes      |
 |                                                                       |
 -------------------------------------------------------------------------
*/

color(back_fore)

unsigned int back_fore;   /* Background Foreground attribute for screen */

{

   Attribute = back_fore;

}
  
/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                               clr_scrn()                              |
 |                                                                       |
 |                             clear the screen                          |
 |                                                                       |
 -------------------------------------------------------------------------
*/

clr_scrn()

{

   union REGS inregs;        /* Used for interrupt registers    */
   union REGS outregs;
   unsigned int offset;

   inregs.x.ax = 0x0600;
   inregs.x.bx = 0xff00 & (Attribute << 8);
   inregs.x.cx = 0x0000;
   inregs.x.dx = 0x184f;
		
   int86(0x0010, &inregs, &outregs);

   for (offset = 0; offset < 2000; offset++)
   {
      Scrn_map[offset].character = ' ';
      Scrn_map[offset].attribute = Attribute;
   }

   print_w1();
   print_w2();
   print_w3();

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print_w1()                               |
 |                                                                       |
 |        Move rows 0, 1, 2 from video memory map to screen memory       |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_row(row)

unsigned int row;

{

   struct SREGS segregs;          /* Used for segment registers */

   /*
    * Determine Data Segment
    */

   segread(&segregs);

   if (Video_seg == COLOR_ADDR)
   {
      SHOW_VID(segregs.ds,&Scrn_map[row * 80].character,Video_seg,row * 80 * 2,80);
   }
   else
   {
      movedata(segregs.ds,&Scrn_map[row * 80].character,Video_seg,row * 80 * 2,80 * 2);
 	}

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print_w1()                               |
 |                                                                       |
 |        Move rows 0, 1, 2 from video memory map to screen memory       |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_w1()

{

   struct SREGS segregs;          /* Used for segment registers */

   /*
    * Determine Data Segment
    */

   segread(&segregs);

   if (Video_seg == COLOR_ADDR)
   {
      SHOW_VID(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 3);
   }
   else
   {
      movedata(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 3 * 2);
 	}

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print_w2()                               |
 |                                                                       |
 |         Move rows 3 - 21 from video memory map to screen memory       |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_w2()

{

   struct SREGS segregs;          /* Used for segment registers */

   /*
    * Determine DS (Data Segment)
    */

   segread(&segregs);

   if (Video_seg == COLOR_ADDR)
   {
      SHOW_VID(segregs.ds,&Scrn_map[80 * 3].character,Video_seg,80 * 3 * 2,80 * 21);
   }
   else
   {
      movedata(segregs.ds,&Scrn_map[80 * 3].character,Video_seg,80 * 3 * 2,80 * 21 * 2);
 	}

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print_w3()                               |
 |                                                                       |
 |     Move rows 22, 23, 24 from video memory map to screen memory       |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_w3()

{

   struct SREGS segregs;          /* Used for segment registers */

   /*
    * Determine DS (Data Segment)
    */

   segread(&segregs);

   if (Video_seg == COLOR_ADDR)
   {
      SHOW_VID(segregs.ds,&Scrn_map[80 * 22].character,Video_seg,80 * 22 * 2,80 * 3);
   }
   else
   {
      movedata(segregs.ds,&Scrn_map[80 * 22].character,Video_seg,80 * 22 * 2,80 * 3 * 2);
 	}

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                              print_all()                              |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_all()

{

   print_w1();
   print_w2();
   print_w3();

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                                init()                                 |
 |                                                                       |
 |                    Set up video for use by this program               |
 |                                                                       |
 -------------------------------------------------------------------------
*/

init()

{

   union REGS inregs;        /* Used for interrupt registers    */
   union REGS outregs;

   unsigned int equip_flag;
   char crt_mode;

   /*
    * Get equipment flag
    */

   int86(0x0011, &inregs, &outregs);

   equip_flag = outregs.x.ax;

   if ( (VIDEO_STATE & equip_flag) == VIDEO_STATE)
   {
      Video_seg = COLOR_ADDR;
   }
   else
   {
      Video_seg = BW_ADDR;
   }

   /*
    * Get the address of the 6845
    * Get the crt mode setting
    * Turn off the background blink
    * Save new crt mode setting
    */

   peek(DATA_SEG, ADDR_6845, &Address_6845, 2);

   peek(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

   crt_mode &= 0x1f;

   /*
    * Address of Mode Control Register (diable background blink)
    */

   outp(Address_6845 + 4, crt_mode);

   poke(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

   cur_off();

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                               cur_off()                               |
 |                                                                       |
 -------------------------------------------------------------------------
*/

cur_off()

{

   /*
    * Disable cursor (port address of 6845 index and data register)
    */

   outp(Address_6845, 10);
   outp(Address_6845 + 1, 0x20);   
   outp(Address_6845, 11);
   outp(Address_6845 + 1, 0x20);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                             reset()                                   |
 |                                                                       |
 |                  Restore video to pre set up state                    |
 |                                                                       |
 -------------------------------------------------------------------------
*/

reset()

{

   char crt_mode;

   /*
    * Get address of 6845
    * Get the crt mode setting
    * Enable background blink
    * Save old crt mode setting
    */

   peek(DATA_SEG, ADDR_6845, &Address_6845, 2);

   peek(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

   crt_mode |= 0x20;

   outp(Address_6845 + 4, crt_mode);

   cur_on();

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                               cur_on()                                |
 |                                                                       |
 -------------------------------------------------------------------------
*/

cur_on()

{

   /*
    * Enable cursor (port address of 6845 index and data register)
    */

   outp(Address_6845, 10);
   outp(Address_6845 + 1, 0x06);
   outp(Address_6845, 11);
   outp(Address_6845 + 1, 0x07);

}







		  

