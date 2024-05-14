/*E*/
/* Fichier : $Id: gmot_mai.h,v 1.2 1996/08/29 10:27:38 milleville Exp $		        Release : $Revision: 1.2 $              Date : $Date: 1996/08/29 10:27:38 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mai * FICHIER gmot_mai.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	15 Nov 94	: Creation
*
* T.Milleville	5 Dec 94	: Ajout de la fnction
*					InitAdm_cb
*
* Niepceron    29 Aou 96	: Ajout de gmot_var.h 1.2
******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

#ifndef gmot_mai
#define gmot_mai

/* fichiers inclus */
#include "gmot_var.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void InitAdm_cb(
	T_IPC_CONN                 va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG                   pa_Arg );

#endif
