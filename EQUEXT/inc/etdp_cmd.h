/*E*/
/*  Fichier : @(#)etdp_cmd.h	1.2      Release : 1.2        Date : 04/11/97
-------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cmd.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* MG		29/05/12 : Ajout TDP PMVA (DEM/1014 PMVA)
-----------------------------------------------------------------------------------------*/


/* Declarations de donnees globales externes */
/* ----------------------------------------- */

extern INFO_EQT_FEN_LCR	*pg_DebutListeTDPFLcr;

/* Declaration de fonctions externes */
/* --------------------------------- */

extern void et_Init_IHM_Mes_TDP_cb	( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void et_CMD_LCR			( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void et_controle_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );      	
extern void et_ACQ_donnees_TDP_cb	( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_cmd_PMV_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
/**************************/
extern void et_cmd_PMVA_cb				( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_demande_TDP_PMVA_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_TDP_voisin_PMVA_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_CompteRendu_PMVA_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
/***************************/
extern void et_demande_TDP_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_TDP_voisin_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void et_CompteRendu_cb		( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
