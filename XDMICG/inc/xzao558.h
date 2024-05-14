/*E*/
/*Fichier :  @(#)xzao558.h	1.1      Release : 1.1        Date : 22/03/12
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao558.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    22/03/12   : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao558
#define xzao558

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO558_RPC_NAME "XZAO558"

#define XZAOC_XZAO558_NB_PARM_RETURN 1

/* definitions de types exportes */

int XZAO558_Scenarios_Zone(XDY_Entier 	va_NumZone_in,
						 XDY_FonctionInt pa_FonctionUtilisateur_in,
						 XDY_Basedd        va_Basedd_in,
						 XDY_Entier      *va_Resultat_out);


#endif
