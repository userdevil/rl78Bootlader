#pragma section @@DATA RAM_VECT AT 0F9000H


__far void (*RAM_INTWDTI_ISR)();
__far void (*RAM_INTLVI_ISR)();
__far void (*RAM_INTP0_ISR)();
__far void (*RAM_INTP1_ISR)();
__far void (*RAM_INTP2_ISR)();
__far void (*RAM_INTP3_ISR)();
__far void (*RAM_INTP4_ISR)();
__far void (*RAM_INTP5_ISR)();
__far void (*RAM_INTST2_ISR)();
__far void (*RAM_INTSR2_ISR)();
__far void (*RAM_INTSRE2_ISR)();
 ////////////////DMA//////////////
 ////////////////DMA//////////////
__far void (*RAM_INTST0_ISR)();
__far void (*RAM_INTSR0_ISR)();
__far void (*RAM_INTSRE0_ISR)();
__far void (*RAM_INTST1_ISR)();
__far void (*RAM_INTSR1_ISR)();
__far void (*RAM_INTSRE1_ISR)();
__far void (*RAM_INTIICA0_ISR)();
__far void (*RAM_INTTM00_ISR)();
__far void (*RAM_INTTM01_ISR)();
__far void (*RAM_INTTM02_ISR)();
__far void (*RAM_INTTM03_ISR)();
__far void (*RAM_INTAD_ISR)();
__far void (*RAM_INTRTC_ISR)();
__far void (*RAM_INTIT_ISR)();
__far void (*RAM_INTKR_ISR)();
 //////////////UART3/////////////
 //////////////UART3/////////////
 //__far void (*RAM_INTTM13_ISR)();
__far void (*RAM_INTTM04_ISR)();
__far void (*RAM_INTTM05_ISR)();
__far void (*RAM_INTTM06_ISR)();
__far void (*RAM_INTTM07_ISR)();
__far void (*RAM_INTP6_ISR)();
__far void (*RAM_INTP7_ISR)();
__far void (*RAM_INTP8_ISR)();
__far void (*RAM_INTP9_ISR)();
__far void (*RAM_INTP10_ISR)();
__far void (*RAM_INTP11_ISR)();
//__far void (*RAM_INTIICA1_ISR)();
__far void (*RAM_INTFL_ISR)();
