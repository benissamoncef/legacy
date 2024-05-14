/*E*/
/* Fichier : $Id: gmot_adm_dv.h,v 1.6 2017/01/20 09:58:21 gesconf Exp $		        Release : $Revision: 1.6 $              Date : $Date: 2017/01/20 09:58:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv * FICHIER gmot_adm_dv.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.milleville	17 Nov 94 	: Creation
* P.Niepceron	29 Aou 96 	: Ajout des fichiers specifiques PC simplifie 1.2
* P.Niepceron   29 Aou 96       : Ajout de SSS_PCS	v1.3
* P.Niepceron   18 Dec 97	: Ajout de SSS_PC2 PC2LT + STA v1.4
* P.Niepceron   01 Jul 98	: Ajout du type de machine Historique (dem/1699) v1.5
* P.Niepceron   20 Jan 17	: Modif cm_MAX_DEBUG apres redimensionnement SSS v1.6 DEM1150
******************************************************/

#ifndef gmot_adm_dv
#define gmot_adm_dv

/* fichiers inclus */

/* definitions de constantes exportees */

#define cm_MAX_SCREENS  10   /* nombre max d ecrans */
#define cm_MAX_SYN_FILE 23    /* nombre max de fichiers */
#define cm_COLORS_VIEW  "SynColors.v"   /* fichier des couleurs */
#define cm_DATA_TABLE   "Table de symboles"

#define cm_BUF_SECU_MRG 500 /* Marge de securite ajoute a la taille
			       calculee du buffer de donnees */
				
#define cm_NB_TACHES    20  /* Nbre max de var dynamiques d'une tache */
#define cm_ST_NVARS     300  /* Nbre max de var dynamiques d'une station */
#define cm_SP_NVARS     300  /* Iden pour un serveur de protocoles */
#define cm_LT_NVARS     300  /* Idem pour un Local Technique */
#define cm_SD_NVARS     200 /* idem pour un serveur de donnees */
#define cm_SC_NVARS     200  /* idem pour un serveur de communications */
#define cm_PERI_NVARS   1   /* idem pour un peripherique */
#define cm_INTE_NVARS   1   /* idem pour une interface */
#define cm_SITE_NVARS  	1  /* idem pour une interface */
				 
#define cm_MAX_DEBUG    10   /* Nbre de niveaux de trace */

#define cm_SSS_SD_MAX   82  /* Nbre max de machines affichees dans SSS */

/* fichiers contenant les vues Dataviews pour les synoptiques d administration */

#define cm_SYNT_GEN_v      "SYNT_GEN.v"
#define cm_SYNT_GEN_pcs_v      "SYNT_GEN_pcs.v"
#define cm_ARC_syn_v      "ARC_syn.v"
#define cm_ARC_syn_pcs_v      "ARC_syn_pcs.v"
#define cm_SSS_syn_v      "SSS_syn.v"
#define cm_TAC_ST_syn_v   "TAC_ST_syn.v"
#define cm_TAC_SP_syn_v   "TAC_SP_syn.v"
#define cm_TAC_LT_syn_v   "TAC_LT_syn.v"
#define cm_TAC_SD_syn_v   "TAC_SD_syn.v"
#define cm_TAC_SC_syn_v   "TAC_SC_syn.v"
#define cm_TAC_PCS_syn_v  "TAC_PCS_syn.v"
#define cm_TAC_PC2_syn_v  "TAC_PC2_syn.v"
#define cm_TAC_PC2LT_syn_v  "TAC_PC2LT_syn.v"
#define cm_TAC_STA_syn_v  "TAC_STA_syn.v"
#define cm_TAC_HIS_syn_v  "TAC_HIS_syn.v"
#define cm_TRA_ST_syn_v   "TRA_ST_syn.v"
#define cm_TRA_SP_syn_v   "TRA_SP_syn.v"
#define cm_TRA_PCS_syn_v  "TRA_PCS_syn.v"
#define cm_TRA_PC2_syn_v  "TRA_PC2_syn.v"
#define cm_TRA_PC2LT_syn_v  "TRA_PC2LT_syn.v"
#define cm_TRA_STA_syn_v   "TRA_STA_syn.v"
#define cm_TRA_HIS_syn_v   "TRA_HIS_syn.v"
#define cm_TRA_LT_syn_v   "TRA_LT_syn.v"
#define cm_TRA_SD_syn_v   "TRA_SD_syn.v"
#define cm_TRA_SC_syn_v   "TRA_SC_syn.v"

#define cm_SSS_Station_v  "SSS_Station.v"
#define cm_SSS_LT_v  	  "SSS_LT.v"
#define cm_SSS_ServerP_v  "SSS_ServerP.v"
#define cm_SSS_ServerD_v  "SSS_ServerD.v"
#define cm_SSS_ServerC_v  "SSS_ServerC.v"
#define cm_SSS_PCS_v  "SSS_PCS.v"
#define cm_SSS_PC2_v  "SSS_PC2.v"
#define cm_SSS_PC2LT_v  "SSS_PC2LT.v"
#define cm_SSS_StationA_v  "SSS_StationA.v"
#define cm_SSS_HIS_v  "SSS_HIS.v"


/* fichiers contenant les vues Dataviews pour les synoptiques d aide */

#define cm_ARC_help_v     "ARC_help_syn.v"
#define cm_SYNT_help_v    "SYNT_help_syn.v"
#define cm_SSS_help_v     "SSS_help_syn.v"
#define cm_TAC_help_v     "TAC_help_syn.v"
#define cm_TRA_help_v     "TRA_help_syn.v"

/* niveaux de trace pour les taches */

#define cm_Debug_FO       "FO"
#define cm_Debug_ME       "ME"
#define cm_Debug_IN       "IN"
#define cm_Debug_BD       "BD"
#define cm_Debug_D1       "D1"
#define cm_Debug_D2       "D2"
#define cm_Debug_D3       "D3"
#define cm_Debug_INTER    "INTER"
#define cm_Debug_REGLE    "REGLE"
#define cm_Debug_Ecr      "Ecr"

/* definitions de types exportes */

typedef int tg_TYP_SYN;


/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void MajAdmSsSys( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmTache( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmServeur( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmPeriph( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmInterf( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmTrace( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmStatiM( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void MajAdmStatiE( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void RafraichitAdm( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );

#endif



