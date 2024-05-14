/* Fichier :  $Id: itre_ax.h,v 1.68 2021/06/03 07:16:37 pc2dpdy Exp $      Release : $Revision: 1.68 $        Date : $Date: 2021/06/03 07:16:37 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_ax.h				*	
*************************************************
* DESCRIPTION :
*
*
*************************************************
* HISTORIQUE :
*
* J.M. Guilhou	29 Sep 1994 Creation                                      V1.1
* J.M. Guilhou	11 Oct 1994 Ajout vg_nomTache                             V1.3
* J.Cornu	19 Oct 1994 Ajout de ITRE_AX_XZEV0x	V1.4
* J.Cornu	26 Oct 1994 Ajout de ITRE_AX_XZEA13	V1.5
* J.Cornu	29 Nov 1994 Ajout de ITRE_AX_XZEA18
*					 ITRE_AX_XZEA22	
*					 ITRE_AX_XZEA23	
*					 ITRE_AX_XZEA24 V1.7
* J.Cornu	09 Dec 1994 Ajout de ITRE_AX_XZEN
* J.Cornu	20 Dec 1994 Ajout de ITRE_AX_XZEP
* Guilhou	20 dec 1994 Ajout de ITRE_AX_OUVREFMC	V1.10
* Guilhou	22 dec 1994 Ajout de ITRE_AX_DEMANDESURCHARGE	
*					ITRE_AX_REPONSESURCHARGE	  V1.12
* Lecrivain	29 dec 1994 Ajout primitives pilotage RAU et telephone    V1.15
* Lecrivain	02 Jan 1995 Ajout fonction ITRE_AX_XZEA30                 V1.16
* Lecrivain	02 Jan 1995 Ajout fonction ITRE_AX_XZEL01                 V1.17
* Mismer	05 Jan 1995 Ajout fonction include xzeo et xzel           V1.18
* Lecrivain	06 Jan 1995 Ajout pseudo-fonction ITRE_AX_XZEU00          V1.19
* Guilhou	10 jan 1995 Ajout fonction ITRE_AX_FINSURCHARGE		  V1.20
* J.Cornu	13 Dec 1994 Ajout de ITRE_AX_XZEL02 & 03                  V1.21
* Lecrivain	13 Jan 1995 Ajout fonction ITRE_AX_XZCA02_03              V1.22
* Lecrivain	18 Jan 1995 Ajout fonction ITRE_AX_XZEA14                 V1.23
* Volcic	20 Jan 1995 Ajout fonction ITRE_AX_XZEA15                 V1.24
* Lecrivain	27 Jan 1995 Ajout fonctions ITRE_AX_XZEC0x                V1.26
* Lecrivain	29 Jan 1995 Ajout fonction ITRE_AX_XZAA10                 V1.28
* Lecrivain	02 Avr 1995 Ajout argument canal IHM a itre_ax_processMsg V1.33
* Lecrivain	04 Avr 1995 Ajout ITRE_AX_XZEL02_03 et ITRE_AX_MSG_ADM    V1.34
* Torregrossa	06 Jul 1995 Ajout ITRE_AX_SONALERTE                       V1.35
* Guilhou	19 Sep 1995 Ajout ITRE_AX_DONINDIV			  V1.36
* Mismer	21 Sep 1995 Ajout ITRE_AX_XZEA26			  V1.40
* Noel		07 Fev 1996 Ajout ITRE_AX_ARRET_SYNOP et
*					ITRE_AX_RELANCER_SYNOP	(DEM807)  V1.41
* Guilhou	22 aut 1996 Ajout ITRE_AX_ENVOI_FIN	(RADT)		  V1.42
* niepceron     26 Sep 1996     : Ajout de ITRE_AX_ACQT (dem/424) v1.43
* Mismer  	25 Mar 1997 	: Ajout fct pour TDP (DEM/1417) V1.44
* Guilhou	01 dec 1997	: ajout ITRE_AX_XZAA01 (strada) 1.45
* Guilhou	01 dec 1997	: suppression ITRE_AX_XZAA01 (strada) 1.46
* Guilhou	19 jan 1998	: ajout ITRE_AX_INITPRES 1.47
* niepceron     20 nov 1998	: ajout de ITRE_AX_DEMCAT et EXPCAT dem/1723 1.48
* cluchague     20 sep 2002	: ajout de XZET03 (98)  1.49 (REPERE1)
* JMG		28/09/05	: ajout ITRE_AX_XZEA080 1.50
* JMG         23/10/07        : ajout ITRE_AX_XZEU10 1.51
* JBL		Nov 2007 : DEM 665
* VR		16/12/11	: Ajout PRV (DEM/1016)
* JPL		23/12/11	: Ajout ITRE_AX_TIMER_TDP (DEM 1007)  1.54
* JPL		23/12/11	: Suppression declarations inutiles et incoherentes  1.55
* SDU       30/01/12    : Ajout ITRE_AX_XZEL10 à ITRE_AX_XZEL12 (DEM 1015)
* MG		11/06/12	: Ajout ITRE_AX_XZEZ04 (DEM/1014 PMA)
* JMG		29/09/14	: ajout ITRE_AX_FACTIS
* JMG		26/04/16 : ajout ITRE_AX_XZEA180 SAGA et ITRE_AX_XZEU11  1.60
* JMG		20/06/17 : ajout ITRE_AX_TIGMOT* tigmot 1.61
* LCL		29/04/18 : ajout ITRE_AX_XZEA082 CTRL_FEUX 1.62 DEM1284
* JMG		29/10/18: ajout ITRE_AX_RMVI DEM1315 1.63
* JMG		30/11/18 : ajout AVA 1.64
* JMG		12/06/19 : ajout INFO_MESSAGE DEM1338 ODI-C 1.65
* JMG		12/11/19 : ajout ITRE_AX_TDP_SURCHARGE DEM1365 1.66
* CGR		13/01/21 : ajout ITRE_AX_XZEA084 IMU DEM-SAE155 1.67
* JMG		04/03/21 : interface itre_ax_processMsg API 1.68
* CGR		02/04/21 : ajout ITRE_AX_XZEP09 et ITRE_AX_XZEA085 SONO PAU DEM-SAE244 1.68
* GGY		21/11/23 : ajout ITRE_AX_XZED03 pour demande d'enregistrement video DAI (DEM483)
* GGY		24/11/23 : ajout ITRE_AX_XZEA71 pour PIC (DEM483)
*************************************************/

#ifndef MTRE_AX_H
#define MTRE_AX_H

/***********************************************/
/* Acces aux services                          */
/***********************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rtworks/common.h>
#include <rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include <rtworks/ipc_proto.h>
#endif
#include "xzst.h"
#include "xzss.h"
#include "xzsa.h"
#include "itre_com.h"
#include "xzdd.h"
#include "xzea.h"
#include "xzea28.h"
#include "xzec.h"
#include "xzev.h"
#include "xzex.h"
#include "xzen.h"
#include "xzel.h"
#include "xzeo.h"
#include "xzic.h"
#include "xzep.h"
#include "xzae103.h"
#include "xdl.h"

/***********************************************/
/* Constantes 				       */
/***********************************************/
/*fonctions dont la tache relais demande l'execution a Applix*/
#define ITRE_AX_SENDPOKE	(2)
#define ITRE_AX_NEWTASK	(3)
#define ITRE_AX_SETVARCTX	(4)
#define ITRE_AX_GETVARCTX	(5)
#define ITRE_AX_SONS (6)
#define ITRE_AX_INFO_MESSAGE		(13)

/*fonctions dont Applix demande l'execution*/
#define ITRE_AX_JOINDG		(1)
#define ITRE_AX_LEAVEDG		(6)
#define ITRE_AX_TRACE		(10)
#define ITRE_AX_PRINT		(11)
#define ITRE_AX_LANCEMCE	(12)
#define ITRE_AX_XZDD02		(13)
#define ITRE_AX_LOGIN		(14)
#define ITRE_AX_OUVREFMC	(15)
#define ITRE_AX_DEMANDESURCHARGE	(16)
#define ITRE_AX_REPONSESURCHARGE	(17)
#define ITRE_AX_FINSURCHARGE	(18)
#define ITRE_AX_XZEA28		(21)
#define ITRE_AX_XZEC02		(22)
#define ITRE_AX_XZEV01		(23)
#define ITRE_AX_XZEV02		(24)
#define ITRE_AX_XZEV03		(25)
#define ITRE_AX_XZEV04		(26)
#define ITRE_AX_XZEV05		(27)
#define ITRE_AX_XZEV06		(28)
#define ITRE_AX_XZEV07		(29)
#define ITRE_AX_XZEA13		(30)  
#define ITRE_AX_XZET01		(31)  
#define ITRE_AX_XZET02		(32)  
#define ITRE_AX_XZET03		(98)  
#define ITRE_AX_XZEA18		(33)
#define ITRE_AX_XZEA22		(34)
#define ITRE_AX_XZEA23		(35)
#define ITRE_AX_XZEA24		(36)
/*
#define ITRE_AX_XZEC05		(37)
*/
#define ITRE_AX_XZEC07		(38)
#define ITRE_AX_XZEA10		(39)
#define ITRE_AX_XZEN01		(40)
#define ITRE_AX_XZEN02		(41)
#define ITRE_AX_XZEN03		(42)
#define ITRE_AX_XZEN04		(43)
#define ITRE_AX_XZEA08		(44)
#define ITRE_AX_XZEP01		(45)
#define ITRE_AX_XZEP01_ARRET	(46)
#define ITRE_AX_XZEP02		(47)
#define ITRE_AX_XZEA09		(48)
#define ITRE_AX_XZEX01		(49)
#define ITRE_AX_XZEX02		(50)
#define ITRE_AX_XZEA12		(51)
#define ITRE_AX_XZEO01		(52)
#define ITRE_AX_XZEO02		(53)
#define ITRE_AX_XZEA25		(54)
#define ITRE_AX_XZEU00		(55)
#define ITRE_AX_XZEU01		(56)
#define ITRE_AX_XZEU02		(57)
#define ITRE_AX_XZEU03		(58)
#define ITRE_AX_XZEU04		(59)
#define ITRE_AX_XZEU05		(60)
#define ITRE_AX_XZEU06		(61)
#define ITRE_AX_XZEU07		(62)
#define ITRE_AX_XZEU08		(63)
#define ITRE_AX_XZEU09		(64)
#define ITRE_AX_XZEA30		(65)
#define ITRE_AX_XZEL01		(66)
#define ITRE_AX_XZEL02		(67)
#define ITRE_AX_XZEL03		(68)
#define ITRE_AX_XZCA02_03	(69)
#define ITRE_AX_XZEA14		(70)
#define ITRE_AX_XZEA15		(71)
#define ITRE_AX_XZEC00		(72)
#define ITRE_AX_XZEC01		(73)
#define ITRE_AX_XZEC05		(74)
#define ITRE_AX_XZAA10		(75)
#define ITRE_AX_XZEL02_03	(76)
#define ITRE_AX_MSG_ADM		(77)
#define ITRE_AX_Jour_Nuit	(78)
#define ITRE_AX_SONALERTE	(79)
#define ITRE_AX_DONINDIV	(80)
#define ITRE_AX_LOGINDONE	(81)
#define ITRE_AX_XZEA26		(82)
#define ITRE_AX_ARRET_SYNOP	(83)
#define ITRE_AX_RELANCER_SYNOP	(84)
#define ITRE_AX_FIN_POSTE	(85)
#define ITRE_AX_ETAT_SIMPLIFIE	(86)
#define ITRE_AX_PRESENCE_PCS    (87)
#define ITRE_AX_XZST10		(88)
#define ITRE_AX_NUM_SITE	(89)
#define ITRE_AX_ACQT		(90)
#define ITRE_AX_XZEZ01		(91)
#define ITRE_AX_INITPRES	(92)
#define ITRE_AX_XZEB01		(93)
#define ITRE_AX_XZEA38		(94)
#define ITRE_AX_DEMCAT		(95)
#define ITRE_AX_EXPCATF		(96)
#define ITRE_AX_EXPCATC		(97)
#define ITRE_AX_XZEA080		(99)
#define ITRE_AX_XZEP04		(101)
#define ITRE_AX_XZEP04_ARRET	(102)
#define ITRE_AX_XZEN10		(103)
#define ITRE_AX_XZEN11		(104)
#define ITRE_AX_XZEN13		(105)
#define ITRE_AX_XZEN15		(106)
#define ITRE_AX_XZEA40		(107)
#define ITRE_AX_XZEA41		(108)
#define ITRE_AX_XZEU10		(109)


#define ITRE_AX_SOC_RESULT	(100)  
// GTC Renovee
#define ITRE_AX_XZEA112		(112)
#define ITRE_AX_XZEA113		(113)
#define ITRE_AX_XZEA114		(114)
#define ITRE_AX_XZEA115		(115)

#define ITRE_AX_FCT_MSQ         (116)
#define ITRE_AX_FCT_VUE         (117)
#define ITRE_AX_XZEL04          (118)
#define ITRE_AX_XZEL05          (119)

#define ITRE_AX_FCT_TDP_MZ      (120)
#define ITRE_AX_FCT_F2T         (121)
#define ITRE_AX_FCT_F2T_REF     (122)
#define ITRE_AX_FCT_INRETS      (123)

#define ITRE_AX_XZEL06          (124)
#define ITRE_AX_PLANSECOURS		(125)

#define ITRE_AX_TIMER_TDP				(126)

#define ITRE_AX_XZEL10          		(127)
#define ITRE_AX_XZEL11          		(128)
#define ITRE_AX_XZEL12          		(129)

#define ITRE_AX_XZEG01					(130)
#define ITRE_AX_XZEL13					(131)
#define ITRE_AX_XZAV03					(132)

#define ITRE_AX_XZEP06					(133)
#define ITRE_AX_XZEP06_ARRET			(134)
#define ITRE_AX_XZEA70					(135)	// PRV
#define ITRE_AX_XZEZ04					(136)	// PMVA
#define ITRE_AX_XZEA71					(137)	// PIC

#define ITRE_AX_FACTIS					(140)
#define ITRE_AX_XZEA180					(141)
#define ITRE_AX_XZEP104					(142)
#define ITRE_AX_XZEU11					(143)
#define ITRE_AX_XZEL14					(144)

#define ITRE_AX_TIGMOT_VERIF_COHERENCE	(145)
#define ITRE_AX_TIGMOT_SAUVE_CFG		(146)
#define ITRE_AX_TIGMOT_DUPLIQUE_CFG		(147)
#define ITRE_AX_TIGMOT_MODIF_BASE		(148)

#define  ITRE_AX_XZEA082				(149)
#define  ITRE_AX_XZEP07					(150)
#define ITRE_AX_GABEB 					(151)
#define ITRE_AX_XZIS50					(152)
#define ITRE_AX_XZIS53					(153)
#define ITRE_AX_XZEL16					(155)
#define ITRE_AX_TDP_SURCHARGE			(156)
#define ITRE_AX_TDP_INIT_SURCHARGE		(157)
#define ITRE_AX_XZEA084					(158)
#define ITRE_AX_XZEP08					(159)
#define ITRE_AX_XZEP09					(160)
#define ITRE_AX_XZEA085					(161)
#define ITRE_AX_AVA_RETOUR_ALERTE		(162)
#define ITRE_AX_XZED03					(163)

// GTC Renovee
// Definition des types de commandes unitaire par domaine
// la valeur 1 est reservee pour la signalisation non renovee
#define ITRE_AX_DOMAINE_SIG (11)
#define ITRE_AX_DOMAINE_VEN (2)
#define ITRE_AX_DOMAINE_ECL (3)
#define ITRE_AX_DOMAINE_ENR (4)
#define ITRE_AX_DOMAINE_EXP (5)
#define ITRE_AX_DOMAINE_EXP_LIT (6)
#define ITRE_AX_DOMAINE_LTN (7)
#define ITRE_AX_DOMAINE_SYS (8)
#define ITRE_AX_DOMAINE_INC (9)
// id commande domaine
#define ITRE_AX_DOMAINE_TUNNEL  (110)
#define ITRE_AX_DOMAINE_ECHANGEUR (111)
// fin GTC Renovee
/***********************************************/
/* Type 				       */
/***********************************************/

/***********************************************/
/* Macros 				       */
/***********************************************/

/***********************************************/
/* acces au variables globales 		       */
/***********************************************/
extern int vg_socRecFromAx,vg_socEnvToAx;
extern int vg_socRecFromAxMce, vg_socEnvToAxMce;
extern int vg_socRecFromAxDial, vg_socEnvToAxDial;
extern T_INT4 vg_socRT;
extern XDY_Datagroup vg_dg_mce,vg_dg_dial;
extern XDY_Datagroup vg_dg_admin_poste, vg_dg_admin_site;
extern XDY_NomTache  vg_nomTache;
extern XDY_NomMachine vg_nomMachine;
extern int vg_loginDone;
extern int vg_type_machine;
extern XDY_NomSite	vg_nomSite;
extern int 	vg_numero_site;
extern int	vg_operateurPresent;

/***********************************************/
/* acces aux fonctions                         */
/***********************************************/


/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les messages en provenance du RT server
* de types 4301 a 4304
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_ax_processMsg(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les demandes d'execution de fonctions
* qu'applix envoie a la tache relais 
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_ax_processExec(int va_numCmd,int va_numFct,char *pa_arg, char *va_nomMachine
);

/*
* PARAMETRE EN ENTREE :
*
* arg1		Description
* arg2		..........
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/




/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les demandes d'envoi de messages que fait
* Applix a la tache relais
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_ax_processSend(int va_msgType,char *va_dg,char *pa_arg
);

/*
* PARAMETRE EN ENTREE :
*
* arg1		Description
* arg2		..........
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



#endif
