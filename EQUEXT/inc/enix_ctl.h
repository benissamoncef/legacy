/*E*/
/*Fichier :  $Id: enix_ctl.h,v 1.1 2008/02/13 13:57:47 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:57:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_ctl.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure es donnees du fichier de gestion cyclique
*  de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 Sept 2007 : GTC rénovée DEM 664
*
------------------------------------------------------ */
 
#ifndef enix_ctl
#define enix_ctl

/* fichiers inclus */

#include "ex_msok.h"
#include "enix_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern  T_INT4       vg_SockST;
extern  XDY_NomSite  vg_NomSite;

/* delaration de fonctions externes */

extern  void         et_controle ( int             va_NumSock,
                                   XDY_NomMachine  pa_NomMachine );


extern  void         et_init     ( XDY_NomMachine  pa_NomMachine );

extern  void         et_sortir   ();


#endif
