/*
 *************************************************************************
 *                                                                       *
 *       DiskSaver (C)opyright 1985 Personal Touch Software Inc.         *
 *                                                                       *
 *************************************************************************
*/

/*
 * Include files
 */

#include "disk.h"

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                       show_sector(cdta,offset)                        |
 |                                                                       |
 |                 Show sector in current dta at given offet             |
 |                                                                       |
 -------------------------------------------------------------------------
*/

show_sector(cdta,offset)

char *cdta;             /* Current Disk Transfer Address         */
unsigned int offset;             /* Offset into cdta to show              */

{

   unsigned int loop1;             /* Loop counter first layer              */
   unsigned int loop2;             /* Loop counter second layer             */
   char hex[3];           /* Hex data string                       */
   char ascii[2];         /* Ascii data                            */

   /*
    * Initialize end of hex string
    */

   hex[2] = '\0';
   ascii[1] ='\0';

   /*
    * Output disk data header
    */

   color(0x0007);

   locate(0x0300);

   print("   OFF                       Hex View");
   print("                            Ascii View");

   /*
    * Calculate offset into cdta
    */

   cdta += offset;

   locate(0x0500);

   /*
    * Display disk data in hex by 16 rows of 16
    * along with the ascii data
    */

   for(loop1=0;loop1<16;loop1++)
   {

      /*
       * Display Side, track, sector and disk offset from start of sector
       */

      if(offset == 0)
      {
         if(loop1 == 0)
         {
            print("   000   ");
         }
         else
         {
            print("   0");
            show_hex(loop1 * 0x0010,hex);
            print(hex);
            print("   ");
         }
      }
      else
      {
         if(loop1 == 0)
         {
            print("   100   ");
         }
         else
         {
            print("   1");
            show_hex(loop1*0x0010,hex);
            print(hex);
            print("   ");
         }
      }

      /*
       * Display the first group of 8 values
       */

      for(loop2=0;loop2<8;loop2++)
      {
         show_hex(*(cdta + (loop1 * 16) + loop2),hex);
         print(hex);
         print(" ");
      }

      /*
       * Display the group seperator
       */

      print("- ");

      /*
       * Display the second group of 8 values
       */

      for(loop2=8;loop2<16;loop2++)
      {
         show_hex(*(cdta + (loop1 * 16) + loop2),hex);
         print(hex);
         print(" ");
      }

      /*
       * Display space seperation between hex and ascii view of data
       */

      print("   ");

      /*
       * Display the same 16 bytes of data in ascii
       */

      for(loop2=0;loop2<16;loop2++)
      {
         ascii[0] = *(cdta + (loop1*16)+loop2);

         /*
          * If non printable ascii character 0 or ff then show blank
          */

         if (ascii[0] == 0 || ascii[0] == 0xff)
         {
            ascii[0] = ' ';
         }

         print(ascii);

      }

      /*
       * Data line completed display a new line
       */

      locate(0x0600 + (loop1 << 8));

   }

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                    show_select(switch,position,value)                 |
 |                                                                       |
 |                       show selection as on or off                     |
 |                                                                       |
 -------------------------------------------------------------------------
*/

show_select(on_off,position,value)

unsigned int on_off;                /* Choice is on or off               */
unsigned int position;              /* Value position on screen          */
unsigned int value;                 /* Current value to display          */

{

   char strng[5];                   /* Value string                      */

   /*
    * Initialize value string
    */

   strng[4] = '\0';

   if(on_off == 0)
   {

      color(0x0007);

      strng[0] = ' ';
      strng[3] = ' ';

   }
   else
   {

      color(0x0070);

      strng[0] = (char) 0x00dd;
      strng[3] = (char) 0x00de;

   }

   locate(position);

   show_hex(value,&strng[1]);

   print(strng);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                          show_page(page_no)                           |
 |                                                                       |
 |                          show page 0 or page 1                        |
 |                                                                       |
 -------------------------------------------------------------------------
*/

show_page(page_no)

unsigned int page_no;               /* prev or next page                 */

{

   /*
    * Initialize value string
    */

   color(0x0007);

   locate(0x0040);

   if(page_no == 0)
   {
      print("Page 1 of 2");
   }
   else
   {
      print("Page 2 of 2");
   }

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                unsigned char show_hex(character,hex_ptr)              |
 |                                                                       |
 |           Convert one character recieved in character to hex          |
 |           and place into a hex string pointed to by hex_ptr           |
 |                                                                       |
 -------------------------------------------------------------------------
*/

show_hex(character,hex_ptr)

char character;     /* Character to convert                      */
char *hex_ptr;      /* Pointer to the converted hex string       */

{

   char right_digit;
   char left_digit;

   /*
    * Each Character will have two hex digits.
    * First get the high byte and place in the hex string
    * Then get the low byte and place in the hex string
    */

   if((left_digit = ((unsigned int) (character >> 4))) >= 0x0a)
   {
      *hex_ptr++ = (char) ( (unsigned int) '7' + left_digit);
   }
   else
   {
      *hex_ptr++ = (char) ( (unsigned int) '0' + left_digit);
   }

   if((right_digit = ((unsigned int) (character & 0x0f))) >= 0x0a)
   {
      *hex_ptr = (char) ( (unsigned int) '7' + right_digit);
   }
   else
   {
      *hex_ptr = (char) ( (unsigned int) '0' + right_digit);
   }

}

