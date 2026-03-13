#pragma interrupt INTWDTI watchdog_intrpt
#pragma interrupt INTLVI lvi_intrpt
#pragma interrupt INTP0 intp0_intrpt
#pragma interrupt INTP1 intp1_intrpt
#pragma interrupt INTP2 intp2_intrpt
#pragma interrupt INTP3 intp3_intrpt
#pragma interrupt INTP4 intp4_intrpt
#pragma interrupt INTP5 intp5_intrpt
#pragma interrupt INTST2 intst2_interrupt 
#pragma interrupt INTSR2 intsr2_interrupt 
#pragma interrupt INTSRE2 intsre2_interrupt
////////////////DMA//////////////
////////////////DMA//////////////
#pragma interrupt INTST0 intst0_interrupt
#pragma interrupt INTSR0 intsr0_interrupt
#pragma interrupt INTSRE0 intsre0_interrupt
#pragma interrupt INTST1  intst1_interrupt
#pragma interrupt INTSR1 intsr1_interrupt
#pragma interrupt INTSRE1 intsre1_interrupt
#pragma interrupt INTIICA0 intsic_interrupt
#pragma interrupt INTTM00 inttm00_interrupt
#pragma interrupt INTTM01 inttm01_interrupt
#pragma interrupt INTTM02 inttm02_interrupt
#pragma interrupt INTTM03 inttm03_interrupt
#pragma interrupt INTAD   intad_interrupt
#pragma interrupt INTRTC  intrtc_interrrupt
#pragma interrupt INTIT   intit_interrrupt
#pragma interrupt INTKR   intkr_interrupt
//////////////UART3/////////////
//////////////UART3/////////////
//#pragma interrupt INTTM13 inttm13_interrupt
#pragma interrupt INTTM04 inttm04_interrupt
#pragma interrupt INTTM05 inttm05_interrupt
#pragma interrupt INTTM06 inttm06_interrupt
#pragma interrupt INTTM07 inttm07_interrupt
#pragma interrupt INTP6   intp6_interrupt
#pragma interrupt INTP7   intp7_interrupt
#pragma interrupt INTP8   intp8_interrupt
#pragma interrupt INTP9   intp9_interrupt
#pragma interrupt INTP10  intp10_interrupt
#pragma interrupt INTP11  intp11_interrupt
//#pragma interrupt INTIICA1	inticca_interrupt
#pragma interrupt INTFL		intfl_interrupt
//------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "bl_ram_vector.h"
#include "r_cg_userdefine.h"
#include "r_cg_timer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


__interrupt static void watchdog_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTWDTI_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void lvi_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTLVI_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp0_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP0_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp1_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP1_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp2_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP2_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp3_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP3_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp4_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP4_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp5_intrpt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP5_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intst2_interrupt (void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTST2_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsr2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSR2_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsre2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSRE2_ISR)();
    /* End user code. Do not edit comment generated here */
}

////////////////DMA//////////////
////////////////DMA//////////////

__interrupt static void intst0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTST0_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsr0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSR0_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsre0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSRE0_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intst1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTST1_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsr1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSR1_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsre1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTSRE1_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intsic_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTIICA0_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm00_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM00_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm01_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM01_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm02_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM02_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm03_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM03_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intad_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTAD_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intrtc_interrrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTRTC_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intit_interrrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTIT_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intkr_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTKR_ISR)();
    /* End user code. Do not edit comment generated here */
}


//__interrupt static void inttm13_interrupt(void)
//{
//    /* Start user code. Do not edit comment generated here */
//    (*RAM_INTTM13_ISR)();
//    /* End user code. Do not edit comment generated here */
//}


__interrupt static void inttm04_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM04_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm05_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM05_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm06_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM06_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void inttm07_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTTM07_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp6_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP6_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp7_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP7_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp8_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP8_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp9_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP9_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp10_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP10_ISR)();
    /* End user code. Do not edit comment generated here */
}


__interrupt static void intp11_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTP11_ISR)();
    /* End user code. Do not edit comment generated here */
}


//__interrupt static void inticca_interrupt(void)
//{
//    /* Start user code. Do not edit comment generated here */
//    (*RAM_INTIICA1_ISR)();
//    /* End user code. Do not edit comment generated here */
//}


__interrupt static void intfl_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    (*RAM_INTFL_ISR)();
    /* End user code. Do not edit comment generated here */
}


