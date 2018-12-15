/*
 *************************************************************************
 *                                                                       *
 *    DiskSaver (C)opyright 1985 Personal Touch Software Inc.            *
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

extern char Dta[512];
extern char Video_seg[2000];

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                               sector()                                |
 |                                                                       |
 |            select, read, write, previous, next sector driver          |
 |                                                                       |
 -------------------------------------------------------------------------
*/

sector()

{

   unsigned int key_hit;      /* Both control and ascii keyboard         */
   unsigned int key_ah;       /* Control code from keyboard              */
   unsigned int key_al;       /* Ascii code from keyboard                */
   unsigned int side;         /* Side of disk                            */
   unsigned int track;        /* Track of on side of disk                */
   unsigned int sector;       /* Sector of track on side of disk         */
   char *show_hex();          /* Convert to hex                          */
   unsigned int prev_next;    /* 0 = first page, 1 = second page         */
   unsigned int retry;        /* disk retry counter                      */

   unsigned int row_col;      /* row and col of cursor                   */
   unsigned int offset;       /* offset into Dta                         */
   unsigned int hi_lo;        /* hex view : 1 = left char 2 = right char */
   unsigned int view;         /* 1 = hex view 2 = ascii view             */

   char hex[3];               /* Hex data string                         */
   char ascii[2];             /* Ascii data                              */

   struct SREGS segregs;      /* Used for segment registers              */

   /*
    * Determine DS (Data Segment)
    */

   segread(&segregs);

   /*
    * Initialize end of hex string
    */

   hex[1] = '\0';
   hex[2] = '\0';
   ascii[1] ='\0';

   /*
    * Initialize position values
    */

   side = 0;
   track = 0;
   sector = 1;

   row_col = 0x0509;
   offset = 0;
   hi_lo = 1;
   view = 1;

   /*
    * set page value
    */

   prev_next = 0;

   /*
    * Set up menu and open disk
    */

   color(0x0007);
   clr_scrn();

   print_header(" D i s k S a v e r ");
   print_w1();

   print_trailer(" Sector View "," ESC when DONE ");
   print_menu("Select Sector","Read Sector","Write Sector","Prev Sector","Next Sector");
   print_w3();

   /*
    * Display position template and disk location
    */

   color(0x0007);

   locate(0x0000);
   print("Side     Track     Sector  ");
   print_w1();

   show_select(0,0x0004,side);
   show_select(0,0x000e,track);
   show_select(0,0x0019,sector);
   print_w1();

   print_trailer(" Reading Diskette "," One moment please ");
   print_w3();

   retry = 0;
   while(read_sector(0,side,track,sector,Dta) && retry++ < 3)
	;

   print_trailer(" Sector View "," ESC when DONE ");
   print_w3();

   show_page(prev_next);
   print_w1();

   show_sector(Dta,0 + (prev_next * 256));
   print_w2();

   locate(row_col = 0x0509);
   offset = 0;
   hi_lo = 1;
   view = 1;

   /*
    * Get function keys until ESC pressed
    */

   do
   {

      cur_on();

      key_hit = get_key();
      key_ah = key_hit >> 8;
      key_al = key_hit & 0xff;

      switch(key_ah)
      {

         /*
          * Sector View (F1)
          */

         case 59 :
         case 94 :
         case 84 :
         case 104 :
         {
			   break;
         }

         /*
          * File Allocation View (FAT) Sector View (F3)
          */

         case 61 :
         case 96 :
         case 86 :
         case 106 :
         {
			   break;
         }

         /*
          * Directory View (F5)
          */

         case 63 :
         case 98 :
         case 88 :
         case 108 :
         {
			   break;
         }

         /*
          * File View (F7)
          */

         case 65 :
         case 100 :
         case 90 :
         case 110:
         {
			   break;
         }

         /*
          * Print according to View (F9)
          */

         case 67 :
         case 102 :
         case 92 :
         case 112 :
         {
			   break;
         }

         /*
          * Select Sector (F2)
          */

         case 0x003c :
         case 0x0055 :
         case 0x005f :
         case 0x0069 :
         {

            cur_off();

            /*
             * Display trailer update
             */

            print_trailer(" Select Disk Sector "," ESC when DONE ");
            print_w3();

            select_sector(&side,&track,&sector);

            print_trailer(" Reading Diskette "," One moment please ");
            print_w3();

            retry = 0;
            while(read_sector(0,side,track,sector,Dta) && retry++ < 3)
            ;

            print_trailer(" Sector View "," ESC when DONE ");
            print_w3();

            show_sector(Dta,0 + ( prev_next * 256));
            print_w2();

            locate(row_col);

            cur_on();

            break;
         }

         /*
          * Read Sector	(F4)
          */

         case 0x003e :
         case 0x0057 :
         case 0x0061 :
         case 0x006b :
         {

            cur_off();

            print_trailer(" Reading Diskette "," One moment please ");
            print_w3();

            retry = 0;
            while(read_sector(0,side,track,sector,Dta) && retry++ < 3)
            ;

            print_trailer(" Sector View "," ESC when DONE ");
            print_w3();

            show_sector(Dta,0 + ( prev_next * 256));
            print_w2();

            locate(row_col);

            cur_on();

            break;
         }

         /*
          * Write Sector (F6)
          */

         case 0x0040 :
         case 0x0059 :
         case 0x0063 :
         case 0x006d :
			{

            cur_off();

            print_trailer(" Writting Diskette "," One moment please ");
            print_w3();

            retry = 0;
            while(write_sector(0,side,track,sector,Dta) && retry++ < 3)
            ;

            print_trailer(" Sector View "," ESC when DONE ");
            print_w3();

            locate(row_col);

            cur_on();

            break;
         }

         /*
          * Previos Sector (F8)
          */

         case 0x0042 :
         case 0x005b :
         case 0x0065 :
         case 0x006f :
			{

            cur_off();

            if (sector > 1)
            {
               sector -= 1;
            }
            else if (side == 1)
            {
               sector = 9;
               side = 0;
            }              
            else if (track > 0)
            {
               sector = 9;
               track -= 1;
               side = 1;
            }
           else
            {
               sector = 9;
               track = 39;
               side = 1;
            }

            show_select(0,0x0004,side);
            show_select(0,0x000e,track);
            show_select(0,0x0019,sector);
            print_w1();

            print_trailer(" Reading Diskette "," One moment please ");
            print_w3();

            retry = 0;
            while(read_sector(0,side,track,sector,Dta) && retry++ < 3)
            ;

            print_trailer(" Sector View "," ESC when DONE ");
            print_w3();

            show_sector(Dta,prev_next * 256);
            print_w2();

            locate(row_col);

            cur_on();
 
            break;
               
         }

         /*
          * Next Sector	(F10)
          */

         case 0x0044 :
         case 0x005d :
         case 0x0067 :
         case 0x0071 :
			{

            cur_off();

            if (sector < 9)
            {
               sector += 1;
            }
            else if (side == 0)
            {
               sector = 1;
               side = 1;
            }              
            else if (track < 39)
            {
               sector = 1;
               track += 1;
               side = 0;
            }
            else
            {
               sector = 1;
               track = 0;
               side = 0;
            }

            show_select(0,0x0004,side);
            show_select(0,0x000e,track);
            show_select(0,0x0019,sector);
            print_w1();

            print_trailer(" Reading Diskette "," One moment please ");
            print_w3();

            retry = 0;
            while(read_sector(0,side,track,sector,Dta) && retry++ < 3)
            ;

            print_trailer(" Sector View "," ESC when DONE ");
            print_w3();

            show_sector(Dta,prev_next * 256);
            print_w2();

            locate(row_col);

            cur_on();
 
            break;
         }

         /*
          * Previous Key (PgUp)
          */

         case 73 :
         case 132 :
         {

            if (prev_next != 0)
            {
               cur_off();

               prev_next = 0;
               offset -= 256;

               show_page (prev_next);
               print_w1();

               show_sector(Dta,0);
               print_w2();

               locate(row_col);

               cur_on();
            }

            break;
         }

         /*
          * Next Key	(PgDn)
          */

         case 81 :
         case 118 :
			{

            if (prev_next != 1)
            {
               cur_off();

               prev_next = 1;
               offset += 256;

               show_page(prev_next);
               print_w1();

               show_sector(Dta,256);
               print_w2();

               locate(row_col);

               cur_on();
            }

            break;
         }

         /*
          * Begin Key (Home)
          */

         case 71 :
         case 119 :
         {

            /*
             * Determine if home key or begin key
             */

            if (key_al)
            {

               /*
                * Home key hit
                */

               if (prev_next != 0)
               {
                  cur_off();

                  prev_next = 0;
                  show_page (prev_next);
                  print_w1();

                  show_sector(Dta,0);
                  print_w2();

                  cur_on();
               }

               offset = 0;

            }
            else
            {

               /*
                * Begin key hit
                */

               if (prev_next)
               {

                  /*
                   * Second page
                   */

						offset = 256;

               }
               else
               {

                  /*
                   * First page
                   */

                  offset = 0;

               }

            }

            row_col = 0x0509;
            view = 1;
            hi_lo = 1;

            locate(row_col);

            break;
         }

         /*
          * End Key (End)
          */

         case 79 :
         case 117 :
			{

            /*
             * Determine if end or end home was hit
             */

            if (key_al)
            {

               /*
                * End how was hit
                */

               if (prev_next != 1)
               {
                  cur_off();

                  prev_next = 1;
                  show_page(prev_next);
                  print_w1();

                  show_sector(Dta,256);
                  print_w2();

                  cur_on();
               }

               offset = 511;

            }
            else
            {

               /*
                * End was hit
                */

               if (prev_next)
               {

                  /*
                   * Second Page
                   */

                  offset = 511;

               }
               else
               {

                  /*
                   * First Page
                   */

                  offset = 255;

               }

             }

            view = 0;
            row_col = 0x1400 + 77;
            locate(row_col);

            break;
         }

         /*
          * Up Arrow
          */

         case 72 :
         {

            if ((row_col & 0xff00) == 0x0500)
            {
               locate(row_col = 0x1400 | (row_col & 0x00ff));
               offset += 240;
            }
            else
            {
               locate(row_col = row_col - 0x0100);
               offset -= 16;
            }

            break;
         }

         /*
          * Right Arrow
          */

         case 77 :
         {

            switch(row_col & 0x00ff)
            {

               case 9 :
               case 12 :
               case 15 :
               case 18 :
               case 21 :
               case 24 :
               case 27 :
               case 30 :
               {
                  locate(row_col = (row_col + 0x0001));
                  hi_lo = 0;
                  break;
               }

               case 10 :
               case 13 :
               case 16 :
               case 19 :
               case 22 :
               case 25 :
               case 28 :
               {
                  locate(row_col = (row_col + 0x0002));
                  offset++;
                  hi_lo = 1;
                  break;
               }

               case 31 :
               {
                  locate(row_col = (row_col & 0xff00) + 35);
                  offset++;
                  hi_lo = 1;
                  break;
               }

               case 35 :
               case 38 :
               case 41 :
               case 44 :
               case 47 :
               case 50 :
               case 53 :
               case 56 :
               {
                  locate(row_col = (row_col + 0x0001));
                  hi_lo = 0;
                  break;
               }

               case 36 :
               case 39 :
               case 42 :
               case 45 :
               case 48 :
               case 51 :
               case 54 :
               {
                  locate(row_col = (row_col + 0x0002));
                  offset++;
                  hi_lo = 1;
                  break;
               }

               case 57 :
               {
                  locate(row_col = (row_col & 0xff00) + 62);
                  offset -= 15;
                  view = 0;
                  break;
               }

               case 62 :
               case 63 :
               case 64 :
               case 65 :
               case 66 :
               case 67 :
               case 68 :
               case 69 :
               case 70 :
               case 71 :
               case 72 :
               case 73 :
               case 74 :
               case 75 :
               case 76 :
               {
                  locate(row_col = (row_col + 0x0001));
                  offset++;
                  break;
               }

               case 77 :
               {
                  locate(row_col = (row_col & 0xff00) + 9);
                  offset -= 15;
                  hi_lo = 1;
                  view = 1;
                  break;
               }

               default :
               {
                  break;
               }

				}
            break;
         }

         /*
          * Down Arrow
          */

         case 80 :
         {

            if ((row_col & 0xff00) == 0x1400)
            {
               locate(row_col = 0x0500 | (row_col & 0x00ff));
               offset -= 240;
            }
            else
            {
               locate(row_col = row_col + 0x0100);
               offset += 16;
            }

            break;
         }

         /*
          * Left Arrow
          */

         case 75 :
         {
 
            switch(row_col & 0x00ff)
            {

               case 9 :
               {
                  locate(row_col = (row_col & 0xff00) + 77);
                  offset += 15;
                  view = 0;
                  break;
               }

               case 10 :
               case 13 :
               case 16 :
               case 19 :
               case 22 :
               case 25 :
               case 28 :
               case 31 :
               {
                  locate(row_col = (row_col - 0x0001));
                  hi_lo = 1;
                  break;
               }

               case 12 :
               case 15 :
               case 18 :
               case 21 :
               case 24 :
               case 27 :
               case 30 :
               {
                  locate(row_col = (row_col - 0x0002));
                  offset--;
                  hi_lo = 0;
                  break;
               }

               case 35 :
               {
                  locate(row_col = (row_col & 0xff00) + 31);
                  offset--;
                  hi_lo = 0;
                  break;
               }

               case 36 :
               case 39 :
               case 42 :
               case 45 :
               case 48 :
               case 51 :
               case 54 :
               case 57 :
               {
                  locate(row_col = (row_col - 0x0001));
                  hi_lo = 1;
                  break;
               }

               case 38 :
               case 41 :
               case 44 :
               case 47 :
               case 50 :
               case 53 :
               case 56 :
               {
                  locate(row_col = (row_col - 0x0002));
                  offset--;
                  hi_lo = 0;
                  break;
               }

               case 62 :
               {
                  locate(row_col = (row_col & 0xff00) + 57);
                  offset += 15;
                  hi_lo = 0;
                  view = 1;
                  break;
                }

               case 63 :
               case 64 :
               case 65 :
               case 66 :
               case 67 :
               case 68 :
               case 69 :
               case 70 :
               case 71 :
               case 72 :
               case 73 :
               case 74 :
               case 75 :
               case 76 :
               case 77 :
               {
                  locate(row_col = (row_col - 0x0001));
                  offset--;
                  break;
               }

               default :
               {
                  break;
               }

             }
             break;
         }

         /*
          * Undo Key (-)
          */

         case 45 :
         {
            break;
         }

         /*
          * Help Key (+)
          */

         case 43 :
         {
            break;
         }

         /*
          * Del Key
          * Ins Key
          */

         case 83 :
         case 82 :
         {
			   break;
         }

         /*
          * All other keys
          */

         default :
			{

            /*
             * Edit keys
             */

            if (view)
            {

               /*
                * Hex field - receive and display character accordingly
                * (Take only hex characters)
                */

               switch(key_al)
               {

                  /*
                   * 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
                   * A, a, B, b, C, c, D, d, E, e, F, f
					    */

                  case 48 :
                  case 49 :
                  case 50 :
                  case 51 :
                  case 52 :
                  case 53 :
                  case 54 :
                  case 55 :
                  case 56 :
                  case 57 :
                  case 65 :
                  case 66 :
                  case 67 :
                  case 68 :
                  case 69 :
                  case 70 :
                  case 97 :
                  case 98 :
                  case 99 :
                  case 100 :
                  case 101 :
                  case 102 :
                  {

                     /*
                      * In hex field display appropriately
                      *
                      * if upper case convert to lower case
                      */

                     if (key_al > 0x39)
                     {
                        key_al &= 0xdf;
                     }

                     /*
                      * Save ascii value to print
                      */

                     hex[0] = key_al;

                     /*
                      * Convert to hex for buffer
                      */

                     if(key_al > 0x39)
                     {
                        key_al -= (char) ( (unsigned int) '7');
                     }
                     else
                     {
                        key_al -= (char) ( (unsigned int) '0');
                     }

                     /*
                      * Place in disk buffer
                      */

                     Dta[offset] =
                        (
                           hi_lo ?
                           ( (key_al << 4) | (Dta[offset] & 0x0f) ) :
                           ( (Dta[offset] & 0xf0) | key_al)
                        );

                     /*
                      * Print character
                      */

                     locate(row_col);
                     print(hex);

                     /*
                      * Locate for ascii
                      * Display ascii
                      * Reset cusror so ti can be incremented properly
                      */

                     locate((row_col & 0xff00) | ((offset % 16) + 62));
                     ascii[0] = Dta[offset];

                     /*
                      * If ascii character is the non printable
                      * character 0 or ff then show blank
                      */

                     if (ascii[0] == 0 || ascii[0] == 0xff)
                     {
                        ascii[0] = ' ';
                     }

                     print(ascii);
                     locate(row_col);

                     print_row(row_col >> 8);

                     /*
                      * Increment cursor (like left arrow)
                      */

                     switch(row_col & 0x00ff)
                     {

                        case 9 :
                        case 12 :
                        case 15 :
                        case 18 :
                        case 21 :
                        case 24 :
                        case 27 :
                        case 30 :
                        {
                           locate(row_col = (row_col + 0x0001));
                           hi_lo = 0;
                           break;
                        }

                        case 10 :
                        case 13 :
                        case 16 :
                        case 19 :
                        case 22 :
                        case 25 :
                        case 28 :
                        {
                           locate(row_col = (row_col + 0x0002));
                           offset++;
                           hi_lo = 1;
                           break;
                        }

                        case 31 :
                        {
                           locate(row_col = (row_col & 0xff00) + 35);
                           offset++;
                           hi_lo = 1;
                           break;
                        }

                        case 35 :
                        case 38 :
                        case 41 :
                        case 44 :
                        case 47 :
                        case 50 :
                        case 53 :
                        case 56 :
                        {
                           locate(row_col = (row_col + 0x0001));
                           hi_lo = 0;
                           break;
                        }

                        case 36 :
                        case 39 :
                        case 42 :
                        case 45 :
                        case 48 :
                        case 51 :
                        case 54 :
                        {
                           locate(row_col = (row_col + 0x0002));
                           offset++;
                           hi_lo = 1;
                           break;
                        }

                        case 57 :
                        {
                           locate(row_col = (row_col & 0xff00) + 62);
                           offset -= 15;
                           view = 0;
                           break;
                        }

                        case 62 :
                        case 63 :
                        case 64 :
                        case 65 :
                        case 66 :
                        case 67 :
                        case 68 :
                        case 69 :
                        case 70 :
                        case 71 :
                        case 72 :
                        case 73 :
                        case 74 :
                        case 75 :
                        case 76 :
                        {
                           locate(row_col = (row_col + 0x0001));
                           offset++;
                           break;
                        }

                        case 77 :
                        {
                           locate(row_col = (row_col & 0xff00) + 9);
                           offset -= 15;
                           hi_lo = 1;
                           view = 1;
                           break;
                        }

                        default :
                        {
                           break;
                        }

                     }

  					      break;

                  }

                  /*
                   * Any other key
                   */

                  default :
                  {
                     break;
                  }

               }
				   
            }
            else
            {

               /*
                * In ascii field - receive and display accordingly
                *	(Take any key that has not been filtered)
                * Convert to hex for buffer
                */

               show_hex(key_al, hex);

               /*
                * Place in disk buffer
                */

               Dta[offset] = key_al;

               /*
                * Set cursor and print hex characters
                */

               if ((offset % 16) < 8)
               {
                  locate((row_col & 0xff00) + 9 + ((offset % 8) * 3));
               }
               else
               {
                  locate((row_col & 0xff00) + 35 + ((offset % 8) * 3));
               }

               print(hex);

               /*
                * Reset buffer to display 1 character (used by hex)
                */

               hex[1] = 0;

               /*
                * Display ascii
                * Reset cusror so ti can be incremented properly
                */

               ascii[0] = Dta[offset];

               /*
                * If ascii character is the non printable
                * character 0 or ff then show blank
                */

               if (ascii[0] == 0 || ascii[0] == 0xff)
               {
                  ascii[0] = ' ';
               }

               locate(row_col);
               print(ascii);
               locate(row_col);

               print_row(row_col >> 8);

               /*
                * Increment cursor (like left arrow)
                */

               switch(row_col & 0x00ff)
               {

                  case 9 :
                  case 12 :
                  case 15 :
                  case 18 :
                  case 21 :
                  case 24 :
                  case 27 :
                  case 30 :
                  {
                     locate(row_col = (row_col + 0x0001));
                     hi_lo = 0;
                     break;
                  }

                  case 10 :
                  case 13 :
                  case 16 :
                  case 19 :
                  case 22 :
                  case 25 :
                  case 28 :
                  {
                     locate(row_col = (row_col + 0x0002));
                     offset++;
                     hi_lo = 1;
                     break;
                  }

                  case 31 :
                  {
                     locate(row_col = (row_col & 0xff00) + 35);
                     offset++;
                     hi_lo = 1;
                     break;
                  }

                  case 35 :
                  case 38 :
                  case 41 :
                  case 44 :
                  case 47 :
                  case 50 :
                  case 53 :
                  case 56 :
                  {
                     locate(row_col = (row_col + 0x0001));
                     hi_lo = 0;
                     break;
                  }

                  case 36 :
                  case 39 :
                  case 42 :
                  case 45 :
                  case 48 :
                  case 51 :
                  case 54 :
                  {
                     locate(row_col = (row_col + 0x0002));
                     offset++;
                     hi_lo = 1;
                     break;
                  }

                  case 57 :
                  {
                     locate(row_col = (row_col & 0xff00) + 62);
                     offset -= 15;
                     view = 0;
                     break;
                  }

                  case 62 :
                  case 63 :
                  case 64 :
                  case 65 :
                  case 66 :
                  case 67 :
                  case 68 :
                  case 69 :
                  case 70 :
                  case 71 :
                  case 72 :
                  case 73 :
                  case 74 :
                  case 75 :
                  case 76 :
                  {
                     locate(row_col = (row_col + 0x0001));
                     offset++;
                     break;
                  }

                  case 77 :
                  {
                     locate(row_col = (row_col & 0xff00) + 9);
                     offset -= 15;
                     hi_lo = 1;
                     view = 1;
                     break;
                  }

                  default :
                  {
                     break;
                  }

               }

            }

            break;
         }

      }

   }
   while(key_ah != 01);

   cur_off();

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                      select_sector(side,track,sector)                 |
 |                                                                       |
 |                    Select side track and sector on disk               |
 |                                                                       |
 -------------------------------------------------------------------------
*/

select_sector(side,track,sector)

unsigned int *side;         /* Pointer to Side of Disk                     */
unsigned int *track;        /* Pointer to Track on Side of disk            */
unsigned int *sector;       /* Pointer to Sector on Track on Side of disk  */

{

   struct position                  /* Select sector structure            */
   {
      unsigned int sts_select;      /* Changing side track or sector      */
      unsigned int sts_pos[3];      /* Row Column on screen               */
      unsigned int sts_left[3];     /* Next position if left key hit      */
      unsigned int sts_right[3];    /* Next position if right key hit     */
      unsigned int min_max[6];      /* Mimimum Maximum value for sts      */
      unsigned int sts_value[3];    /* Current sts values                 */
   };

   struct position disk;      /* Select sector structure                     */
   unsigned int key;          /* Scan code of pressed key                    */
   char *show_hex();          /* Convert to hex                              */
   unsigned int current;      /* Current selection                           */
   unsigned int next;         /* Next selection                              */

   /*
    * Initialize select disk sector structure
    */

   disk.sts_select = 0;             /* Selecting side, track or sector     */

   disk.sts_pos[0] = 0x0004;        /* Row Col of side                     */
   disk.sts_pos[1] = 0x000e;        /* Row Col of track                    */
   disk.sts_pos[2] = 0x0019;        /* Row Col of sector                   */

   disk.sts_left[0] = 2;            /* Left key hit wrap to end            */
   disk.sts_left[1] = 0;            /* Left key hit go to start            */
   disk.sts_left[2] = 1;            /* Left key hit go to middle           */

   disk.sts_right[0] = 1;           /* Right key hit go to middle          */
   disk.sts_right[1] = 2;           /* Right key hit go to end             */
   disk.sts_right[2] = 0;           /* Right key hit wrap to start         */

   disk.min_max[0] = 0;             /* Sides minimum value = 0             */
   disk.min_max[1] = 1;             /* Sides maximum value = 1             */
   disk.min_max[2] = 0;             /* Tracks minimum value = 0            */
   disk.min_max[3] = 39;            /* Tracks maximum value = 39           */
   disk.min_max[4] = 1;             /* Sectors minimum value = 1           */
   disk.min_max[5] = 9;             /* Sectors maximum value = 8           */

   disk.sts_value[0] = *side;       /* Current Side value                 */
   disk.sts_value[1] = *track;      /* Current track value                */
   disk.sts_value[2] = *sector;     /* Current sector value               */

   /*
    * Side selection picked by default
    */

   show_select(1,disk.sts_pos[0],disk.sts_value[0]);
   print_w1();

   do
   {

      key = get_key() >> 8;

      switch(key)
      {

         /*
          * Left arrow key
          */

         case 0x004b :
         case 0x0073 :
			{
            current = disk.sts_select;
            next = disk.sts_left[current];
            disk.sts_select = next;
  
            show_select(0,disk.sts_pos[current],disk.sts_value[current]);
            show_select(1,disk.sts_pos[next],disk.sts_value[next]);
            print_w1();

            break;
         }

         /*
          * Right arrow key
          */

         case 0x004d :
         case 0x0074 :
			{
            current = disk.sts_select;
            next = disk.sts_right[current];
            disk.sts_select = next;
  
            show_select(0,disk.sts_pos[current],disk.sts_value[current]);
            show_select(1,disk.sts_pos[next],disk.sts_value[next]);
				print_w1();

            break;
         }

         /*
          * Previous key
          */

         case 0x0049 :
         case 0x0084 :
			{
            current = disk.sts_select;

            if(disk.sts_value[current] == disk.min_max[2*current])
            {
               disk.sts_value[current] = disk.min_max[(2*current)+1];
            }
            else
            {
               disk.sts_value[current] -= 1;
            }

            show_select(1,disk.sts_pos[current],disk.sts_value[current]);
            print_w1();

            break;
         }

         /*
          * Next Key
          */

         case 0x0051 :
         case 0x0076 :
			{
            current = disk.sts_select;

            if(disk.sts_value[current] == disk.min_max[(2*current)+1])
            {
               disk.sts_value[current] = disk.min_max[2*current];
            }
            else
            {
               disk.sts_value[current] += 1;
            }

            show_select(1,disk.sts_pos[current],disk.sts_value[current]);
            print_w1();

            break;
         }

         /*
          * All other keys
          */

         default :
			{
            break;
         }

      }

   }
   while(key != 01);

   /*
    * Ouptut Disk Location
    */

   show_select(0,disk.sts_pos[0],disk.sts_value[0]);
   show_select(0,disk.sts_pos[1],disk.sts_value[1]);
   show_select(0,disk.sts_pos[2],disk.sts_value[2]);
   print_w1();

   /*
    * Set side track sector
    */

   *side = disk.sts_value[0];      /* Current Side value                 */
   *track = disk.sts_value[1];     /* Current track value                */
   *sector = disk.sts_value[2];    /* Current sector value               */

}
