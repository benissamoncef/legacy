/*E*/
/*Fichier :  $Id: eniw_ctl.h,v 1.2 2002/04/24 08:48:46 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:48:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_ctl.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure es donnees du fichier de gestion cyclique
*  de la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */
 
#ifndef eniw_ctl
#define eniw_ctl

/* fichiers inclus */

#include "ex_msok.h"
#include "eniw_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4			vg_SockST;
extern 	XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */

extern 	void	et_controle ( 	int 		va_NumSock,
				XDY_NomMachine	pa_NomMachine );


extern 	void	et_init	    ( 	XDY_NomMachine  pa_NomMachine );

extern 	void	et_sortir   ();


#endif
