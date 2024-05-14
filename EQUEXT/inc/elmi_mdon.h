/*E Fichier : $Id: elmi_mdon.h,v 1.5 1997/04/07 20:10:43 mismer Exp $      Release : $Revision: 1.5 $        Date : $Date: 1997/04/07 20:10:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elmi_mdon.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires par inclusion de elexi_common.h
* Declaration en Extern des variables globales modules elmi_xxx.c.
* Fichier inclus ds. tous les mods. elmi_xxx.c sauf elmi_dir.c
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	27 Oct 1994	: Creation
* Mismer.D	version 1.2	27 Fev 1995	: Ajout variable simulation
* Mismer.D	version 1.3	19 Mai 1995	: Modif gestion deconnexion ST
* Fontaine.C	version 1.4	23 Mai 1995	: RAS
* Mismer.D      version 1.5     25 Mar 1997     : Ajout date pour TDP (DEM/1396)
*
------------------------------------------------------ */

#ifndef elmi_mdon
#define elmi_mdon

/* fichiers inclus */

#include "ex_mcom.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK	*vg_SockList ;			/* liste globale des sockets */
extern XZEXT_ST_Config	vg_Config;			/* config tache TELxi */
extern XZEXT_ST_Sockets	vg_DesSocks;			/* struct. descs. sockets typiques tache TELxi */
extern XZEXT_Liste_Cmdes_LCR	*pg_ListeCmdesLCR;	/* liste cmdes. LCR empilees */
extern char		vg_idpassw[18];
extern int		vg_EtatST;

/* declaration de fonctions externes */

#endif
