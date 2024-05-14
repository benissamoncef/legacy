/*E Fichier : $Id: drol_arbre.h,v 1.3 2020/11/03 17:31:55 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:31:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DPAC_ARBRE * FICHIER drol_arbre.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fonctions de creation et de parcours de l'arbre des 
* evenements lies.
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation
* JMG 18/06/15 : ajout des 4 blocs DEM1130 1.2
------------------------------------------------------ */


/* fichiers inclus */

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzems.h"
#include "xzae68.h"
#include "xzae69.h"
#include "xzae97.h"

#include "drol_fax.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
/* definitions de types locaux */
extern T_NOEUD *vg_B1[50];
extern T_NOEUD	 *vg_B2[50];
extern T_NOEUD	 *vg_B3[50];
extern T_NOEUD	 *vg_B4[50];
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int arbre_creer(XDY_Entier,XDY_Octet,XDY_Octet,XDY_Horodate);

