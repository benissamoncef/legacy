/*E*/
/* Fichier : $Id: seta_bas.h,v 1.5 2011/01/11 21:01:23 gesconf Exp $        Release : $Revision: 1.5 $        Date : $Date: 2011/01/11 21:01:23 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_BAS * FICHIER seta_arc.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
 T.MIlleville  14 Oct 1994     : Creation
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.5
******************************************************/

#ifndef SETA_BAS
#define SETA_BAS


#include <rtworks/ipc.h>

extern int seta_definir_secours(
	XZSCT_NomMachine pa_NomMachine);





extern void seta_synchroactif_cb(
	T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG Arg);
extern int seta_envoie_ract(
	XZSCT_NomMachine        va_NomMachine,
	XZSCT_NomTache          va_NomTache);


#endif
