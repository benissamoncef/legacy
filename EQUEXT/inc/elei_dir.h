/*E Fichier : $Id: elei_dir.h,v 1.4 1995/05/23 09:45:42 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/23 09:45:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELEi * FICHIER elei_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TELEi. 
* 
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	04 Nov 1994	: Creation
* Flaidar.A	version 1.2	02 Dec 1994	:
* Mismer.D	version 1.3 	19 Mai 1995 	: Modif gestion deconnexion ST
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef elei_dir
#define elei_dir

/* fichiers inclus */

#include "ex_mcom.h"
#include "ex_msok.h"
#include "elei_cfg.h"
#include "elei_pro.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZEXT_MASK	*vg_SockList 	= (XZEXT_MASK*)NULL;	/* liste globale des sockets */
XZEXT_ST_Config		vg_Config;	/* struct config tache */	
XZEXT_ST_Sockets	vg_DesSocks;	/* struct. descs. sockets typiques tache TELxi */
int		vg_RepTEzzz = XDC_OK;	/* flag reception reponse tache TEzzz */
char		vg_AdrSAE[4];		/* adresse SAE */						
int		vg_EtatST = XDC_OK;

/* declaration de fonctions externes */

#endif
