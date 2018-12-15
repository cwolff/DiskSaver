/*
 *************************************************************************
 *                                                                       *
 *    DiskSaver tm (C)opyright 1985 Personal Touch Software Inc.         *
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
 |                              main()                                   |
 |                                                                       |
 -------------------------------------------------------------------------
*/

main()

{

   unsigned int key;        /* Scan code of pressed key                */
   int sector();            /* Show sector of disk                     */
   int clr_scrn();          /* Clear the screen                        */

   init();

   color(0x0007);
   clr_scrn();

   print_first_menu();
   print_all();

   do
   {

      key = get_key() >> 8;

      switch(key)
      {

         /*
          * Main Control Panel
          */

         case 0x003c :
         case 0x0055 :
         case 0x005f :
         case 0x0069 :
         {
            break;
         }

         /*
          * How to use DiskSaver
          */

         case 0x003e :
         case 0x0057 :
         case 0x0061 :
         case 0x006b :
         {
            break;
         }

         /*
          * Open Disk
          */

         case 0x0040 :
         case 0x0059 :
         case 0x0063 :
         case 0x006d :
         {

            /*
             * open disk
             */

            sector();

            /*
             * Show menu again
             */

            color(0x0007);
            clr_scrn();

            print_first_menu();
            print_all();

            break;
         }

         /*
          * All other Keys
          */

         default :
			{
            break;
         }

      }

   }
   while(key != 01);

   /*
    * Clear screen and exit
    */

   color(0x0007);
   clr_scrn();

   reset();

}

