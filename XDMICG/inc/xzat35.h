/*E*/
/*  Fichier : $Id: xzat35.h,v 1.2 2011/01/12 11:13:38 gesconf Exp $     Release : $Revision: 1.2 $        Date : $Date: 2011/01/12 11:13:38 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat35.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JPL	12/01/11 : Migration architecture HP ia64 (DEM 971) : declaration correcte XZAT35 (nom)  1.2
------------------------------------------------------ */

#ifndef xzat35
#define xzat35

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT35_RPC_NAME "XZAT35"

#define XZATC_XZAT35_NB_PARM_RETURN 14

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT35_Utilisation_BAF ( 	XDY_Horodate,
					XDY_Eqt,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier * ,
					XDY_Octet *);



#endif
