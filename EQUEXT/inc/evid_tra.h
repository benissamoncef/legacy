/*E Fichier : $Id: evid_tra.h,v 1.5 2017/02/28 14:50:12 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2017/02/28 14:50:12 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la 
*  tache VIDEO.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E 	version 1.1	04 Sep 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* JPL		04/11/16 : Ajout parametres type commande et retour status lecture au traitement reponse (DEM 1189)  1.5
*
------------------------------------------------------ */

#ifndef evid_tra
#define evid_tra

/* fichiers inclus */

#include "evid_don.h"
#include "xzaac.h"
#include "xzamc.h"
/*#include "ex_malt.h"*/


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame (int va_socket, int va_type_cmde, int *va_cr_reception);

#endif
