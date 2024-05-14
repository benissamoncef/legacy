/*E*/
/*  Fichier : $Id: etdp_cfg.h,v 1.3 2019/12/12 17:29:42 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/12/12 17:29:42 $
------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cfg.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de config de la tache TETDP
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* JMG	13/11/19	version 1.3 DEM1365 TDP surcharge
----------------------------------------------------------------------------------------*/

/* Declarations de donnees globales */
/* -------------------------------- */

extern ETDP_DONNEES_UGTP 		vg_DonneesUGTP[ETDPC_NB_MAX_UGTP];
extern ETDP_DONNEES_CONFIG_TACHE	vg_ConfigTDP;

/* Declaration de fonctions externes */
/* --------------------------------- */

void ec_controle( int, XDY_NomMachine );
int ec_init( XDY_NomMachine );
extern void ec_sortir( void);
void et_surcharge ( T_IPC_CONN  ,T_IPC_CONN_PROCESS_CB_DATA , T_PTR    );
void et_init_surcharge ( T_IPC_CONN  ,T_IPC_CONN_PROCESS_CB_DATA , T_PTR    );

