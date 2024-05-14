/*E Fichier : $Id: elei_mdon.h,v 1.4 1995/05/23 09:45:43 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/23 09:45:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELEi * FICHIER elei_mdon.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires par inclusion de elexi_common.h
* Declaration en Extern des varaibles globales modules elmi_xxx.c.
* Fichier inclus ds. tous les mods. elmi_xxx.c sauf elmi_dir.c
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	27 Oct 1994	: Creation
* Flaidar.A	version 1.2	02 Dec 1994	:
* Mismer.D	version 1.3	19 Mai 1995	: Modif gestion deconnexion ST
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef elei_mdon
#define elei_mdon

/* fichiers inclus */

#include "ex_mcom.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK		*vg_SockList ;	/* liste globale des sockets */
extern XZEXT_ST_Config		vg_Config;	/* struct config tache */
extern XZEXT_ST_Sockets		vg_DesSocks;	/* struct. descs. sockets typiques tache TELxi */
extern int			vg_RepTEzzz;	/* flag rep. tache traitmnt */
extern char			vg_AdrSAE[4];	/* adresse SAE */
extern int			vg_EtatST;

/* declaration de fonctions externes */

#endif
