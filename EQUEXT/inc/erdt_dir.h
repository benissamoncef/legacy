/*E*/
/*Fichier : $Id: erdt_dir.h,v 1.21 2018/02/15 15:50:42 devgfi Exp $      Release : $Revision: 1.21 $        Date : $Date: 2018/02/15 15:50:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TERDT
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	07 Dec 1994	: Creation
* Volcic.F	version 1.2	28 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	:
* Volcic.F	version 1.4	17 Mar 1995	:
* Volcic.F	version 1.5	20 Mar 1995	:
* Fontaine.C	version 1.6	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.7	07 Jun 1995	: Ajout vg_Mode_Lent
* Mismer.D      version 1.13    15 Jul 1996     : Correction pour pb trop de station (DEM/1150)
* Volcic.F	version 1.14	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Volcic.F	version 1.15	27 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D      version 1.16    01 Oct 1996     : Modif Extension Ouest niveau de service (RADT_W)
* Mismer.D	version 1.17	03 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.18	21 Oct 1996	: Modif station virtuelle EQT12 (DEM/1252)
* JPL	19/12/11	: declaration structure constante globale de mesures inconnues (DEM 997)  1.19
* VR		version	1.20	22 Fev 2012	: Ajout ERDT_CONF_REG_PARAMS (DEM/1016)
* JPL	15/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.21
------------------------------------------------------ */

#ifndef erdt_dir
#define erdt_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"erdt_don.h"
#include 	"erdt_cmd.h"
#include 	"erdt_cfg.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ERDT_DONNEES_RADT		*pg_debutRADT = NULL;
ERDT_DONNEES_PtMes		*pg_debutPtMes = NULL;
ERDT_DONNEES_Chaussee		*pg_debutChaussee = NULL;

int		 		pg_Tab_RADT[ERDTC_NUM_MAX_STATION];
int		 		pg_Nb_Voie[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
int		 		pg_Nb_Donnees_Jour[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
int		 		pg_Nb_Q_Nuls[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
int		 		pg_Nb_PtMes[ERDTC_NUM_MAX_STATION];
int		 		pg_Nb_Chaussees[ERDTC_NUM_MAX_STATION];
int		 		pg_Seuils[ERDTC_NB_STATION][ERDTC_NB_CHAUS_SEUIL][ERDTC_NB_CONTEXTE][ERDTC_NB_SEUIL];
int		 		pg_Niveau[ERDTC_NB_STATION][ERDTC_NB_CHAUS_SEUIL][ERDTC_NB_TYPE_NIVEAU];
int		 		pg_DIV[ERDTC_NB_CLAS_DIV];
int		 		pg_REG_PARAMS[ERDTC_NB_REG_PARAMS];
int		 		pg_DIV_Moy[ERDTC_NB_DIV_MOY];

ERDT_EMes_RADT			pg_ERDT_Mes_1mn[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];
ERDT_EMes_RADT			pg_ERDT_Mes_6mn[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_6MN];
ERDT_EMes_RADT			pg_ERDT_Mes_6mn_gl[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];
ERDT_EMes_Poids         	pg_ERDT_Mes_6mn_PL[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE];
XDY_Mot                         vg_PL_Prec[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];

INFO_EQT_FEN_LCR		*pg_DebutListeRADTFLcr  = NULL;

XDY_Mes				vg_Mesures_Aucune = {
					{ XDC_RADT_Q_INCONNU,	XDC_RADT_INCONNU },
					{ XDC_RADT_V_INCONNU,	XDC_RADT_INCONNU },
					{ XDC_RADT_TO_INCONNU,	XDC_RADT_INCONNU },
					{ XDC_RADT_PL_INCONNU,	XDC_RADT_INCONNU }
				};

XDY_NomMachine			vg_NomMachine = "";
XDY_NomMachine			vg_NomSuppleant = "";
XDY_NomSite        		vg_NomSite = "";
int                     	vg_Type_Machine = 0;
XDY_Booleen			vg_Mac_Ext_Ouest = 0;
int    				vg_SockTIM = 0;
int		 		vg_SockTDC = 0;
char				vg_idpassw[18] = "";
int				vg_Contexte = 0;
int				vg_ContexteSeuils = 0;
int                		vg_Mode_Fonct = 0;
XDY_Booleen                	vg_Mode_Lent = XDC_FAUX;
XDY_Horodate			vg_Date_Actuelle = 0.0;
XDY_Horodate			vg_Date_Changement = 0.0;
char               		pg_Trame[ERDTC_NB_LS][ERDTC_DIM_TRAME_HIS];
char               		pg_Trame_PL[ERDTC_NB_LS][ERDTC_DIM_TRAME_HIS];
XDY_Booleen          		vg_Rep_His_PL[ERDTC_NB_LS];
XDY_Horodate          		pg_Heure_QVTO = 0.0;   		
XDY_Booleen          		vg_TimersMin = XDC_FAUX;   		
XDY_Booleen          		vg_Timers6Min = XDC_FAUX;   		
XDY_Booleen          		vg_Timers30Min = XDC_FAUX;   		
XDY_Booleen          		vg_Timers_Enclenches = XDC_FAUX;
XDY_Booleen          		vg_Demande_Individuelle[ERDTC_NB_DEM_INDIV];
XDY_Horodate          		vg_Horo_Dem_Ind[ERDTC_NB_DEM_INDIV];   		
FILE         	         	*vg_Fd_Dem_Ind[ERDTC_NB_DEM_INDIV];	  		
ERDT_CONF_RADT_VIRTUELLE	pg_Tab_RADT_Virtuelle[ERDTC_NUM_MAX_STATION];
ERDT_CONF_REG_PARAMS		pg_Tab_REG_PARAMS[ERDTC_NUM_REG_PARAMS];
XDY_Mot				vg_Etat_Liaison_CI = ERDTC_LIAISON_CI_OK;

XZEXT_MASK *			vg_SockMask = NULL;
T_INT4				vg_SockRT = (T_INT4) 0;

/* delaration de fonctions externes */

extern void er_Lancement_Timer ( int );

#endif
