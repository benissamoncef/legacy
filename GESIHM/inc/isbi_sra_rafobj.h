/*E Fichier : $Id: isbi_sra_rafobj.h,v 1.3 2013/08/28 09:33:27 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/08/28 09:33:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isyn_sra_rafobj.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques: Objets graphiques non mobiles
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
* JMG				: ajout projeter
* LCL       24/10/2023 : DEM-473 Evolution PMV
------------------------------------------------------ */

#ifndef MSRA
#define MSRA


#include "xzsm.h"
#include "xzst.h"
#include "xzea.h"
#include "xzsc.h"
#include "xzst.h"
#include "xdy.h"
#include "xzen.h"


/* definitions de constantes exportees */

#define CM_NB_PICTO 129
#define CM_NB_PANNEAU 5

/*SDU*/
/* deficnition des constantes COMMANDES envoyees a Flash*/
#define CMD_FLASH_ETAT_EQUIP		1
#define CMD_FLASH_BOUCHON_CHAUSSEE	2
#define CMD_FLASH_BOUCHON_VOIE		3
#define CMD_FLASH_VOIE_NEUTRALISEE	4
#define CMD_FLASH_RADT_INVERSEE		5
#define CMD_FLASH_BASC_BOUCHON		6
#define CMD_FLASH_MOBILE_PORTION	7
#define CMD_FLASH_MOBILE_ECHANGEUR	8
#define CMD_FLASH_AFFECT_VOIE		9
#define CMD_FLASH_ANIM_EQUIP		10
#define CMD_FLASH_ANIM_RADT  		11
#define CMD_FLASH_MASQUE			12
#define CMD_FLASH_VUE				13
#define CMD_FLASH_INIT_COURBE		14
#define CMD_FLASH_FIN_INIT_COURBE	15
#define CMD_FLASH_DONNEES_COURBE	16
#define CMD_FLASH_ARRET_COURBE		17
#define CMD_FLASH_FIN_DONNEES_COURBE	18

#define CMD_FLASH_PLAN_SECOURS		20
#define CMD_FLASH_COMMUTER		21
#define CMD_FLASH_TRONCON		22
#define CMD_FLASH_PROJETER		25

/*SDU*/

/* definitions de types exportes */

/* definition de macro exportees */
#define rtrim(c) \
	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0'

/* declarations de donnees globales */
extern char *vm_table_picto[CM_NB_PICTO+1];
extern char *vm_table_panneau[CM_NB_PANNEAU+1];

extern XDY_Entier vm_long_boucl;
extern XDY_Entier vm_long_veh;

/* declarations de donnees externes */

/* delaration de fonctions externes */

#endif
