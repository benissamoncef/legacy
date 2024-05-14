/*E Fichier : $Id: epic_tra.h,v 1.1 2023/08/16 14:28:24 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2023/08/16 14:28:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epic_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPICTO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK	16/08/23		: Creation (DEM/483)
------------------------------------------------------ */

#ifndef epic_tra
#define epic_tra

/* fichiers inclus */

#include "xzex.h"
#include "xzst.h"
#include "xdc.h"
#include "xdg.h"
#include "epic_don.h"
#include "epic_cmd.h"

/* definitions de constantes exportees a deplace ds xdc.h */

extern XDY_NomSite        	vg_NomSite;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int);
void ep_envoi_affichage_PIC ( 	XDY_Etat_PIC, XDY_Octet , XDY_District );

#endif
