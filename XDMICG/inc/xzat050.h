/*E*/
/*  Fichier : 	$Id: xzat050.h,v 1.2 2007/03/26 10:33:42 gesconf Exp $     Release : $Revision: 1.2 $       Date : $Date: 2007/03/26 10:33:42 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat05.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
------------------------------------------------------ */

#ifndef xzat050
#define xzat050

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT050_RPC_NAME "XZAT050"

#define XZATC_XZAT050_NB_PARM_RETURN 25

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT050_Utilisation_PMVA ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		,
				XDY_Octet 	*,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA 	,
				XDY_Ligne_PMVA 	,
				XDY_Booleen 	*,
				XDY_Booleen 	*,
				XDY_Booleen 	*,
				XDY_Booleen 	*,
				XDY_Booleen 	*,
				XDY_Booleen 	*,
				XDY_Entier 	*,
				XDY_Octet 	*,
				XDY_Octet 	*,
				XDY_Entier 	* );



#endif
