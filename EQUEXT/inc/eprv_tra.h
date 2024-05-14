/*E Fichier : $Id: eprv_tra.h,v 1.1 2012/07/04 14:28:24 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:28:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef eprv_tra
#define eprv_tra

/* fichiers inclus */

#include "xzex.h"
#include "xzst.h"
#include "xdc.h"
#include "xdg.h"
#include "eprv_don.h"
#include "eprv_cmd.h"

/* definitions de constantes exportees a deplace ds xdc.h */

extern XDY_NomSite        	vg_NomSite;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int);
void ep_envoi_affichage_PRV ( 	XDY_Etat_PRV, XDY_Octet , XDY_District );

#endif
