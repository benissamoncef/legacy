/*E*/
/* Fichier : $Id: sapl_dir.h,v 1.1 1994/11/22 16:43:15 milleville Exp $	Release : $Revision: 1.1 $        Date : $Date: 1994/11/22 16:43:15 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DIR * FICHIER sapl_dir.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Module directeur de la tache tsaplg
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 10 Oct 1994	: Creation
* ABE   06/12/2021 : Ajout Fichier de configuration TOPS pour envoie message XDM_MGA_INIT_PO 1.2
*
******************************************************/

#ifndef SAPL_DIR
#define SAPL_DIR

/* fichiers inclus */

/* definitions de constantes exportees */
#define	VAR_MONOECRAN	"MIGRAZUR_MONO"

#define	SAPL_RELAIS_BI		"titbi.x"
#define	SAPL_RELAIS_LT		"titlt.x"
#define	SAPL_RELAIS_LT_DBG	"titltdbg.x"
#define	SAPL_RELAIS_TITRE	"titre.x"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern	void	sp_sortir();

#define CO_FIC_TOPS_DISPLAY1 "display1"
#define CO_FIC_TOPS_DISPLAY2 "display2"
#define CO_FIC_TOPS_GEN_PATH "/produits/migrazur/appliSD/fichiers/adm/TOPS_"

#endif

