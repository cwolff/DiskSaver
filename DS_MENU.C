/*
 *************************************************************************
 *                                                                       *
 *     DiskSaver tm (C)opyright 1985 Personal Touch Software Inc         *
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
 |                           print_first_menu()                          |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_first_menu()

{

   char top_frame[81];   /* First line of top of screen         */
   char key_t[4];        /* Top of key prompt                   */
   char key_b[4];        /* Bottom of key prompt                */
   char key_str[4];      /* Key prompt                          */
   unsigned int loop;    /* Loop Counter                        */

   /*
    * Display Header
    */

   top_frame[0] = (char) 0x00c9;
   top_frame[79] = (char) 0x00bb;
   top_frame[80] = '\0';

   for(loop=1;loop<79;loop++)
   {
      top_frame[loop] = (char) 0x00cd;
   }

   top_frame[20] = 't';
   top_frame[21] = 'm';

   color(0x0007);

   locate(0x0000);
   print(top_frame);

   top_frame[0] = (char) 0x00ba;

   strcpy(&top_frame[1]," D i s k S a v e r                 Disk Tool");
   strcpy(&top_frame[45],"                                  ");

   top_frame[79] = (char) 0x00ba;

   locate(0x0100);
   print(top_frame);

   top_frame[0] = (char) 0x00c8;
   top_frame[79] = (char) 0x00bc;

   for(loop=1;loop<79;loop++)
   {
      top_frame[loop] = (char) 0x00cd;
   }

   locate(0x0200);
   print(top_frame);

   /*
    * Display Trailer
    */

   print_trailer(" First Choice "," ESC when DONE ");

   /*
    * Display copyright message
    */

   color(0x0007);
   locate(0x1809);
   print("(C)opyright 1985, Personal Touch Software, All Rights Reserved");

   /*
    * Display menu prompt
    */

   color(0x0007);
   locate(0x0510);
   print("Use keys in group at left to chose from the list");

   /*
    * Display function keys
    */

   key_t[0] = (char) 0x00df;
   key_t[1] = (char) 0x00df;
   key_t[2] = (char) 0x00df;
   key_t[3] = '\0';

   key_str[0] = ' ';
   key_str[1] = (char) 0x0005;
   key_str[2] = ' ';
   key_str[3] = '\0';

   key_b[0] = (char) 0x00dc;
   key_b[1] = (char) 0x00dc;
   key_b[2] = (char) 0x00dc;
   key_b[3] = '\0';

   color(0x0070);

   for(loop=0;loop<3;loop++)
   {

      locate(0x0019 | ((7 + (loop * 4)) << 8));
      print(key_t);

      locate(0x0019 | ((8 + (loop * 4)) << 8));
      key_str[1] = (char) (0x0005 - loop);
      print(key_str);

      locate(0x0019 | ((9 + (loop * 4)) << 8));
      print(key_b);

   }

   /*
    * Display choices
    */

   color(0x0007);

   locate(0x081e);
   print("Main Control Panel");

   locate(0x0c1e);
   print("How to use DiskSaver");

   locate(0x101e);
   print("Open Diskette");

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                      print_menu(f1,f2,f3,f4,f5)                       |
 |                                                                       |
 |        print menu onf 25th line with the 5 function texts             |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_menu(f1,f2,f3,f4,f5)

char *f1;          /* Soft function key 1                   */
char *f2;          /* Soft function key 2                   */
char *f3;          /* Soft function key 3                   */
char *f4;          /* Soft function key 4                   */
char *f5;          /* Soft function key 5                   */

{

   char function_key;     /* Function key symbols          */
   char key_string[2];   /* Function Key string              */

   key_string[0] = (char) 0x0005;
   key_string[1] = '\0';
   color(0x000f);
   locate(0x1800);
   print(key_string);

   color(0x0007);
   locate(0x1802);
   print(f1);

   key_string[0] = (char) 0x0004;
   key_string[1] = '\0';
   color(0x000f);
   locate(0x1810);
   print(key_string);

   color(0x0007);
   locate(0x1812);
   print(f2);

   key_string[0] = (char) 0x0003;
   key_string[1] = '\0';
   color(0x000f);
   locate(0x1820);
   print(key_string);

   color(0x0007);
   locate(0x1822);
   print(f3);

   key_string[0] = (char) 0x0006;
   key_string[1] = '\0';
   color(0x000f);
   locate(0x1830);
   print(key_string);

   color(0x0007);
   locate(0x1832);
   print(f4);

   key_string[0] = (char) 0x000f;
   key_string[1] = '\0';
   color(0x000f);
   locate(0x1840);
   print(key_string);

   color(0x0007);
   locate(0x1842);
   print(f5);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                       print_header(title)                             |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_header(title)

char *title;           /* Title string                          */

{

   char frame[81];      /* frame string                        */
   unsigned int loop;            /* loop counter                        */

   frame[0] = (char) 0x00d5;
   frame[79] = (char) 0x00b8;
   frame[80] = '\0';

   for(loop=1;loop<79;loop++)
   {
      frame[loop] = (char) 0x00cd;
   }

   frame[7] = (char) 0x00d1;
   frame[20] = (char) 0x00d1;
   frame[33] = (char) 0x00d1;
   frame[46] = (char) 0x00d1;
   frame[60] = (char) 0x00d1;

   color(0x0007);

   clr_scrn();

   color(0x000f);

   locate(0x0000 | ( 40 - ((strlen(title) & 0x00ff) / 2)));
   print(title);

   color(0x0007);

   locate(0x0100);
   print(frame);

}

/*
 -------------------------------------------------------------------------
 |                                                                       |
 |                    print_trailer(" Sector Display")                   |
 |                                                                       |
 -------------------------------------------------------------------------
*/

print_trailer(left,right)

char *left;          /* Left side prompt                       */
char *right;            /* Right side prompt                      */

{

   char frame[81];     /* Frame string                           */
   unsigned int loop;           /* loop counter                           */
   unsigned int left_size;      /* Size of left prompt                    */
   unsigned int right_size;     /* Size of right prompt                   */

   left_size = strlen(left);
   right_size = strlen(right);

   for(loop=0;loop<(80-right_size)-left_size;loop++)
   {
      frame[loop] = (char) 0x00cd;
   }

   frame[loop] = '\0';

   color(0x0070);

   locate(0x1700);
   print(left);

   color(0x0007);

   locate(0x1700 | left_size);
   print(frame);

   color(0x0070);
       
   locate(0x1700 | (80 - right_size));
   print(right);

   color(0x0007);

}

