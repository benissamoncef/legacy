/*E Fichier : $Id: epma_tra.h,v 1.3 2010/12/07 12:37:34 pc2dpdy Exp $  Release : $Revision: 1.3 $      Date : $Date: 2010/12/07 12:37:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	26/06/10	: linux DEM/934
------------------------------------------------------ */

#ifndef epma_tra
#define epma_tra

/* fichiers inclus */

#include "xzex.h"
#include "xzst.h"
#include "xdc.h"
#include "xdg.h"
#include "epma_don.h"
#include "epma_cmd.h"

/* definitions de constantes exportees a deplace ds xdc.h */

extern XDY_NomSite        	vg_NomSite;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int);
void ep_envoi_affichage_PMVA ( 	XDY_Etat_PMVA, int , XDY_District );

#endif
