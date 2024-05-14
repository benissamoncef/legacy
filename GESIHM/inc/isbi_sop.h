/*E Fichier : $Id: isbi_sop.h,v 1.6 2017/11/22 16:33:45 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/11/22 16:33:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSOP * FICHIER isyn_sop.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Prise en compte des Actions Operateur sur Dataviews
*   
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	13 Sep 1994	: Creation
* JMG		04/07/12 : ajout PRV DEM/1016
* JMG   20/03/17        saga DEM1191
* JMG	20/06/17	tigmot
* JMG	22/11/17	arret CM_TIGMOT_CMD_ARRET
------------------------------------------------------ */

#ifndef MSOP
#define MSOP

/* fichiers inclus */

#include "xzst.h"
#include "xzsc.h"
#include "xzsm.h"
#include "xzsa.h"
#include "xzic.h"
#include "xzea.h"

#include "xdc.h"

#include "isbi_sra_tri.h"


/* Definition des types de masque pour linux */
#define	CM_EQUIP_RADT				26
#define	CM_EQUIP_PMV				2
#define	CM_EQUIP_CAMERA				104
#define	CM_EQUIP_DAI				4
#define	CM_EQUIP_PAU				14
/*#define	CM_EQUIP_RESEAU		0 */
#define CM_EQUIP_PR					109
#define	CM_EQUIP_NAV				1
#define	CM_EQUIP_RESEAU_PI	112
#define	CM_EQUIP_RESEAU_ELECT_MT	110
#define	CM_EQUIP_RESEAU_ELECT_BT	111
#define	CM_EQUIP_RESEAU_ELECT_HT	113
#define	CM_EQUIP_RESEAU_ELECT		117
#define	CM_EQUIP_RESEAU_GAZ			116
#define	CM_EQUIP_RESEAU_EAU			115
#define	CM_EQUIP_RESEAU_AUTRE		118
#define	CM_EQUIP_RESEAU_PS			114
#define	CM_EQUIP_LIT				10
/*#define	CM_EQUIP_PAT			0*/
/*#define	CM_EQUIP_REF			0*/
#define	CM_EQUIP_LTV				103
#define	CM_EQUIP_TRAV_PREVUS		132
/*#define	CM_EQUIP_FERM		19*/
#define	CM_EQUIP_PAL				45
#define	CM_EQUIP_PMVA				46
#define	CM_EQUIP_BAF				47
#define	CM_EQUIP_BAD				48
#define	CM_EQUIP_PRV				62
#define	CM_EQUIP_CANTON				133

/**/
#define CM_EQUIP_ZDP		    105
#define CM_EQUIP_ECHANGEUR 		121
#define CM_EQUIP_TUBE 		    299
#define CM_EQUIP_PEAGE 			106
#define CM_EQUIP_ITPC 			125
#define CM_EQUIP_VILLE 			135
#define CM_EQUIP_AIRE 			119
#define CM_EQUIP_PORTAIL 		124
#define CM_EQUIP_PASSAGE 		8

#define CM_EQUIP_SAGA_PMV_1_18       201
#define CM_EQUIP_SAGA_PMV_2_18       202
#define CM_EQUIP_SAGA_PMV_2_15       203
#define CM_EQUIP_SAGA_PMV_3_15       204
#define CM_EQUIP_SAGA_PMV_2_18_P     205
#define CM_EQUIP_SAGA_PMV_3_15_P     206
#define CM_EQUIP_SAGA_BAF            207
#define CM_EQUIP_SAGA_BRA            208
#define CM_EQUIP_SAGA_PMVA           209
#define CM_EQUIP_SAGA_DAI            210
#define CM_EQUIP_SAGA_CAMERA         211
#define CM_EQUIP_SAGA_RADT           212
#define CM_EQUIP_SAGA_META_EQT       213
#define CM_EQUIP_SAGA_RAU            214
#define CM_EQUIP_SAGA_SAV_1          215
#define CM_EQUIP_SAGA_SAV_2          216
#define CM_EQUIP_SAGA_SAV_3          217
#define CM_EQUIP_SAGA_SAV_4          218
#define CM_EQUIP_SAGA_SAV_5          219
#define CM_EQUIP_SAGA_SAV_6          220

/*definition des types d'action BOUTON*/
#define CM_ACTION_BINTERV	1
#define CM_ACTION_BALARM	2
#define CM_ACTION_BETAT		3
#define CM_ACTION_RAPPELCOURBE 4
#define CM_TIGMOT_CMD_SOUS_SYSTEME	5
#define CM_TIGMOT_CMD_TRACE		6
#define CM_TIGMOT_CMD_TACHE		7
#define CM_TIGMOT_CMD_ARRET		8

/* definitions de constantes exportees */

/* definitions de types exportes */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
