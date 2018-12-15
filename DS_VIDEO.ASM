;
; ************************************************************************
; *                                                                      *
; *  (C)opyright 1985, Personal Touch Software Inc. All Rights Reserved  *
; *                                                                      *
; ************************************************************************
;

title    ds_video

;
; *** Include
;

include sm8086.mac

;
; *** Publics
;

public SHOW_VID

Page

;
; ########################################################################
; #                                                                      #
; #                              EQUATES                                 #
; #                                                                      #
; ########################################################################
;

BLOCK_SIZE                   equ       0f0h

Page

PSEG

;
; ########################################################################
; #                                                                      #
; #                           DEFINED STORAGE                            #
; #                                                                      #
; ########################################################################
;

bmove                        db        BLOCK_SIZE

Page

;
; ========================================================================
; |                                                                      |
; | show_video(source_seg,source_off,dest_seg,dest_off,no_of_characters) |
; |                                                                      |
; ========================================================================
;

find_me  dw        3412h

SHOW_VID proc      near

         push      bp
         mov       bp,sp
         push      ax
         push      bx
         push      cx
         push      dx
         push      si
         push      di
         push      ds
         push      es
         pushf

;
; *** Get source segment
;

         mov       ds,4[bp]            ; get source segment

;
; *** Get source offset
;

         mov       si,6[bp]            ; get source offset

;
; *** Get destination segment
;

         mov       es,8[bp]            ; get destination segment

;
; *** Get destination offset
;

         mov       di,10[bp]            ; get destination offset

;
; *** Get the number of characters (words ) to move
;

         mov       cx,12[bp]

;
; *** Calculate move to video buffer
; *** CX = number of characters to move / three rows of characters (240)
; *** AX = number of characters to move % three rows of characters (240)
;

        mov        ax,cx
        div        bmove

        xor        ch,ch
        mov        cl,al

        mov        al,ah
        xor        ah,ah

        or         cx,cx
        jz         less_than_block

;
; *** Loop to display to screen in 3 row increments
;

page_loop:

         push      cx

         mov       cx,BLOCK_SIZE

         call      color_off

         cld

         rep       movsw

         call      color_on

         pop       cx

         loop      page_loop

;
; *** Display remainder of bytes to video
;

less_than_block:

         mov       cx,ax

         call      color_off

         cld

         rep       movsw

         call      color_on

;
; *** Clean up and return
;

         popf
         pop       es
         pop       ds
         pop       di
         pop       si
         pop       dx
         pop       cx
         pop       bx
         pop       ax
         pop       bp

         ret

SHOW_VID endp

;
; ------------------------------------------------------------------------
; |                                                                      |
; |                                COLOR OFF                             |
; |                                                                      |
; ------------------------------------------------------------------------
;

color_off proc     near

         push      ax
         push      dx

         mov       dx,3dah

color_off_loop:

         in        al,dx

         test      al,8
         jz        color_off_loop

         mov       al,00001001b
         and       al,0f7h

         mov       dx,03d8h
         out       dx,al

         pop       dx
         pop       ax

         ret

color_off endp

Page

; ------------------------------------------------------------------------
; |                                                                      |
; |                                COLOR ON                              |
; |                                                                      |
; ------------------------------------------------------------------------

color_on proc      near

         push      ax
         push      dx

         mov       al,000001001b
         mov       dx,03d8h
         out       dx,al

         pop       dx
         pop       ax

         ret

color_on endp

         ENDPS

         end

