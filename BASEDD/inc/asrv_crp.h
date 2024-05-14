/*E Fichier : $Id: asrv_crp.h,v 1.20 2021/05/04 13:25:26 pc2dpdy Exp $      Release : $Revision: 1.20 $        Date : $Date: 2021/05/04 13:25:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE CRP * FICHIER ASRV_CRP.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	25 Aug 1994	: Creation
* Niepceron	07 Mar 1997 : changement de ca_acrp01_nb_param (dem/1398) v1.2
* JMG	27/04/04		: ajout ACRP99 1.3
* CHI   09/05/05        : modif param ACRP19 DEM459 v1.8
* JMG	11/10/05		: ajout PMVA BAF BAD 1.10
* JBL	Mai 2008 		:  DEM666
* JPL	03/11/09 		: Ajout de trois parametres a ACRP19 (DEM 917) 1.13
* VR    07/11/11 		: Incrementation ca_acrp01_nb_param 1.14 (DEM/1014)
* VR	05/01/12 		: Ajout PRV (DEM/1016)
* VR	08/06/12		: Ajout 2 parametres ca_acrp11_nb_param (DEM/1014 PMA)
* JMG	26/04/16 		: ajout acrp111 SAGA 1.17
* JMG	04/03/18 		: ajout controleurs de feux CTRL_FEUX 1.18 DEM1284
* ABE	28/01/21 		: ajout ca_acrp70 DEM-SAE155 1.19
* ABE	07/04/21 		: ajout acrp80 DEM-SAE244 1.20
* GGY	13/11/23		: Ajout acrp16 PIC (DEM483)
------------------------------------------------------ */
#ifndef CRP
#define CRP

/* fichiers inclus */
#include <stdio.h>
#include "ospublic.h"
#include "oserror.h"
#include "xdc.h"
#include "xdy.h"
#include "xzen.h"
#include "xzeb.h"
#include "xzep.h"
#include "xzet.h"
#include "asrv_rpcom.h"

/* definitions de constantes exportees */
#define ca_acrp01_nb_param	26
#define ca_acrp03_nb_param	4
#define ca_acrp06_nb_param	3
#define ca_acrp08_nb_param	3
#define ca_acrp19_nb_param	3
#define ca_acrp98_nb_param	4
#define ca_acrp99_nb_param	4

#define ca_acrp11_nb_param	29
#define ca_acrp12_nb_param	9
#define ca_acrp13_nb_param	4
#define ca_acrp14_nb_param	9
#define ca_acrp15_nb_param	5
#define ca_acrp16_nb_param	6

/* AAZ */
#define ca_acrp40_nb_param	4
#define ca_acrp41_nb_param	4
#define ca_acrp42_nb_param	4
#define ca_acrp43_nb_param	4
#define ca_acrp44_nb_param	4
#define ca_acrp45_nb_param	4
#define ca_acrp47_nb_param	4
#define ca_acrp111_nb_param	24
#define ca_acrp60_nb_param	4
#define ca_acrp70_nb_param	7
#define ca_acrp80_nb_param	7
/* AAZ */

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */
extern CS_RETCODE acrp01(SRV_PROC        *);
extern CS_RETCODE acrp03(SRV_PROC        *);
extern CS_RETCODE acrp06(SRV_PROC        *);
extern CS_RETCODE acrp08(SRV_PROC        *);
extern CS_RETCODE acrp19(SRV_PROC        *);
extern CS_RETCODE acrp98(SRV_PROC        *);
extern CS_RETCODE acrp99(SRV_PROC        *);
extern CS_RETCODE acrp11(SRV_PROC        *);
extern CS_RETCODE acrp12(SRV_PROC        *);
extern CS_RETCODE acrp13(SRV_PROC        *);
extern CS_RETCODE acrp14(SRV_PROC        *);
extern CS_RETCODE acrp15(SRV_PROC        *);
extern CS_RETCODE acrp16(SRV_PROC        *);
/* AAZ */
extern CS_RETCODE acrp40(SRV_PROC        *);
extern CS_RETCODE acrp41(SRV_PROC        *);
extern CS_RETCODE acrp42(SRV_PROC        *);
extern CS_RETCODE acrp43(SRV_PROC        *);
extern CS_RETCODE acrp44(SRV_PROC        *);
extern CS_RETCODE acrp45(SRV_PROC        *);
extern CS_RETCODE acrp111(SRV_PROC        *);
extern CS_RETCODE acrp60(SRV_PROC        *);
extern CS_RETCODE acrp70(SRV_PROC        *);
extern CS_RETCODE acrp80(SRV_PROC        *);
/* AAZ */
extern CS_RETCODE acrpEXEMPLE(SRV_PROC        *);
#endif
