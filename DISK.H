/*
 *************************************************************************
 *                                                                       *
 *           DiskSaver tm (C)opyright WareWolf Publishing 1984           *
 *                                                                       *
 *************************************************************************
*/

/*
** Templates for special library calls
*/

struct seg_regs            /* Template for Segment Registers             */
{
   unsigned int s_cs;      /* Segment register CS (Code Segment)         */
   unsigned int s_ss;      /* Segment register SS (Stack Segment)        */
   unsigned int s_ds;      /* Segment register DS (Data Segment)         */
   unsigned int s_es;      /* Segment register ES (Extra Segment)        */
};

struct intr_regs           /* Template for Interrupt Registers           */
{
   unsigned int i_ax;      /* Interrupt registers AX (Accumalator)       */
   unsigned int i_bx;      /* Interrupt registers BX (Accumalator)       */
   unsigned int i_cx;      /* Interrupt registers CX (Accumalator)       */
   unsigned int i_dx;      /* Interrupt registers DX (Accumalator)       */
   unsigned int i_si;      /* Interrupt registers SI (Source Index)      */
   unsigned int i_di;      /* Interrupt registers DI (Destination Index) */
   unsigned int i_ds;
   unsigned int i_es;
 };

struct t_scrn_map                 /* Template for video memory map      */
{
   char character;
   char attribute;
};

#define   DATA_SEG            0x40
#define   ADDR_6845           0x63
#define   CRT_MODE_SET        0x65
#define   VIDEO_STATE         0x20
#define   COLOR_ADDR          0xb800
#define   BW_ADDR             0xb000

