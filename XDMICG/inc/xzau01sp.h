/*E*/
/*Fichier : $Id: xzau01sp.h,v 1.1 1994/11/10 10:51:41 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:51:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau01sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef xzau01sp
#define xzau01sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomProfil_in
* 
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_NumProfil_out
* XDY_Entier		va_Resultat_out
*
*/

#define XZAU01_Creer_Profil	XZAU;01

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
