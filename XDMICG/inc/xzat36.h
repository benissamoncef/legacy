/*E*/
/*  Fichier : $Id: xzat36.h,v 1.1 2007/03/26 10:34:05 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 10:34:05 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat36.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation

------------------------------------------------------ */

#ifndef xzat36
#define xzat36

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT36_RPC_NAME "XZAT36"

#define XZATC_XZAT36_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT36_Utilisation_BAD ( 	XDY_Horodate,
					XDY_Eqt,
					XDY_Octet *,
					char *,
					XDY_Entier *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier * );



#endif
