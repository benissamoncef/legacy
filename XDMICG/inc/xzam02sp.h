/*E*/
/*Fichier : $Id: xzam02sp.h,v 1.1 1994/11/10 10:45:04 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam02sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef xzam02sp
#define xzam02sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_TypeAlrm_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_HorodatePremApparition_out
* XDY_Horodate		va_HorodatePreDisparition_out
* XDY_Horodate		va_HorodateDernierChgt_out
* XDY_Mot		va_NombreDApparitions_out
* XDY_Booleen		va_Etat_out
* XDY_Phrase		va_Commentaitre_out
* XDY_DT		va_DemandeTravaux_out
* XDY_Entier		va_NombreTotalApparitions_out
*
*/

#define XZAM02_Lire_Fiche_Alarme	XZAM;02

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
