/*E*/
/* Fichier : $Id: xzao840.h,v 1.1 2017/03/31 17:15:34 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:15:34 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao840.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   30/0317 creation 1.1 DEM1191
------------------------------------------------------ */

#ifndef xzao840
#define xzao840

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO840_RPC_NAME "XZAO840"

#define XZAOC_XZAO840_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO840_Rechercher_tous_les_PMVA_SAGA_amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						XDY_Octet	,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
