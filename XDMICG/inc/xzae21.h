/*E*/
/*  Fichier : 	$Id: xzae21.h,v 1.11 2017/10/10 08:27:54 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2017/10/10 08:27:54 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae21.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Sep 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
* volcic	14 Nov 1994	: Ajout 9 Parametres de sorties (v 1.4)
* volcic	22 Nov 1994	: Modif 9 Parametres de sorties (v 1.5)
* torregrossa	22 Nov 1995	: Ajout Parametre de sortie (v 1.6)
* guilhou	12 dec 1997	: ajout pr_bifurc_amont et pr_bifurc_aval en sortie (dem/1534) 1.7
* PNI		25 Nov 2010	: Suppression du ; DEM963 1.8
* JPL		26/06/2012	: Ajout parametres sortie usager et indicateur bifurcation en amont et aval (DEM 1033)  1.9
* LCL		15/11/2016	: Ajout parametre Viaduc, passage a 34 paramatres (LOT23) 1.10
* JMG		26/06/2017	: ajout zone urbaine convergence 1.11
------------------------------------------------------*/

#ifndef xzae21_
#define xzae21_

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAEC_XZAE21_RPC_NAME "XZAE21"

#define XZAEC_XZAE21_NB_PARM_RETURN 35

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE21_Localiser_Evt_PA (	XDY_Autoroute,
						XDY_PR,
						XDY_Sens,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Entier *,
						XDY_Autoroute *,
						XDY_Entier *,
						XDY_Autoroute *,
						XDY_Entier *,
						XDY_Nom ,
						XDY_Nom ,

						XDY_Nom ,
						XDY_Abrev ,
						XDY_Sortie ,
						XDY_Nom ,
						XDY_Abrev ,
						XDY_Sortie ,
						XDY_Nom ,
						XDY_Abrev ,
						XDY_Nom ,
						XDY_Abrev ,
						XDY_Mot *,
						XDY_Octet *,
						XDY_PR	*,
						XDY_PR  *,
						XDY_PR	*,
						XDY_PR	*,
						XDY_Nom,
						XDY_Mot *);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
