/* --------------------------------------------------------------- */
/* COPYRIGHT, 1991-1994 TALARIAN CORPORATION. ALL RIGHTS RESERVED. */
/* UNPUBLISHED -- RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF      */
/* THE UNITED STATES. USE OF A COPYRIGHT NOTICE IS PRECAUTIONARY   */
/* ONLY AND DOES NOT IMPLY PUBLICATION OR DISCLOSURE.              */
/*                                                                 */
/* THIS DOCUMENTATION CONTAINS PROPRIETARY AND CONFIDENTIAL        */
/* INFORMATION OF TALARIAN CORPORATION. USE, DISCLOSURE, OR        */
/* REPRODUCTION IS PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN    */
/* PERMISSION OF TALARIAN CORPORATION.                             */
/*                                                                 */
/*                   RESTRICTED RIGHTS LEGEND                      */
/*                                                                 */
/* Use, duplication, or disclosure by the Government is subject    */
/* to restrictions set forth in subparagraph (c)(1)(ii) of the     */
/* rights in Technical Data and Computer Software clause at        */
/* DFARS 252.227-7013.                                             */
/*                     Talarian Corporation                        */
/*     444 Castro Street, Suite 140, Mountain View, CA 94041       */
/* --------------------------------------------------------------- */

/* =================================================================== */
/*                                                                     */
/*                   ie_debug.h -- debugging macros                    */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_IE_DEBUG_H_
#define _RTWORKS_IE_DEBUG_H_

/* ----------------------------- */
/* Inference Engine Compile Time */
/* ----------------------------- */
#define T_DBG_SLOT        (T_DBG_IE_START + 0) /* slot parsing */
#define T_DBG_EXPRESS     (T_DBG_IE_START + 1) /* expression parsing */
#define T_DBG_CODE        (T_DBG_IE_START + 2) /* code buffer */
#define T_DBG_PARSE       (T_DBG_IE_START + 3) /* parse trees */
#define T_DBG_BIND        (T_DBG_IE_START + 4) /* binding */

/* ------------------------- */
/* Inference Engine Run Time */
/* ------------------------- */
#define T_DBG_ARCHIVE     (T_DBG_IE_START + 5) /* archiving into ringbuffers */
#define T_DBG_STACK       (T_DBG_IE_START + 6) /* operand stack */
#define T_DBG_BLOCK       (T_DBG_IE_START + 7) /* code buffer execution */
#define T_DBG_FRAME       (T_DBG_IE_START + 8) /* data */
#define T_DBG_EFUNC       (T_DBG_IE_START + 9) /* external functions */
#define T_DBG_TEMPORAL    (T_DBG_IE_START + 10) /* temporal functions */
#define T_DBG_CHAIN       (T_DBG_IE_START + 11) /* chaining */
#define T_DBG_SAVE        (T_DBG_IE_START + 12) /* save */
#define T_DBG_LOAD        (T_DBG_IE_START + 13) /* load */
#define T_DBG_BCHAIN      (T_DBG_IE_START + 14) /* back chaining */

#endif /* _RTWORKS_IE_DEBUG_H_ */






