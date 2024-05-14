/*E*/
/* Fichier : $Id: enic_ctl.h,v 1.2 1995/05/22 15:46:46 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 15:46:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_ctl.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure es donnees du fichier de gestion cyclique
*  de la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef enic_ctl
#define enic_ctl

/* fichiers inclus */

#include "ex_msok.h"
#include "enic_don.h"


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
