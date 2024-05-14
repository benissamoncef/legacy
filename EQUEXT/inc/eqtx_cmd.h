/*E*/
/*  Fichier : $Id: eqtx_cmd.h,v 1.1 1996/12/05 18:59:47 orengo Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/12/05 18:59:47 $
-------------------------------------------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TERAU * FICHIER eqtx_cmd.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TEQTX
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	30 Oct 1996	: Creation
* Orengo.A	version 1.2	30 Oct 1996	: Chagement du nom de l'equipement
*
-----------------------------------------------------------------------------------------*/


/* Declarations de donnees globales externes */
/* ----------------------------------------- */

extern INFO_EQT_FEN_LCR	*pg_DebutListeQTXFLcr;

/* Declaration de fonctions externes */
/* --------------------------------- */

extern void er_CMD_LCR( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern void er_controle_cb( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );      	
extern void er_ACQ_donnees_meteo_QTX_cb( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
