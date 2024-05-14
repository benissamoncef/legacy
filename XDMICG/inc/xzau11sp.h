/*E*/
/*Fichier : $Id: xzau11sp.h,v 1.1 1994/11/10 10:51:59 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:51:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau11sp.h
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

#ifndef xzau11sp
#define xzau11sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Login	va_LoginUtilisateur_in
* XDY_Login	va_MotDePasse_in
* XDY_Mot	va_Machine_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot	va_NumUtilisateur_out
* XDY_Nom	va_Utilisateur_out
* XDY_Mot	va_NumProfil_out
* XDY_Nom	va_NomProfil_out
* XDY_Liste	ListeFonctions 
*
*/

#define XZAU11_Verifier_Identite	XZAU;11

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
