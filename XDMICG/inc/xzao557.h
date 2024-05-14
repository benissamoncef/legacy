/*E*/
/* Fichier : $Id: xzao557.h,v 1.1 2012/07/04 10:59:59 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:59:59 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao557.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao557
#define xzao557

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO557_RPC_NAME "XZAO557"

#define XZAOC_XZAO557_NB_PARM_RETURN 1

/* definitions de types exportes */

int XZAO557_Liste_Zones(XDY_Entier 	va_NumSecteur_in, 
					 XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Basedd        va_Basedd_in,
					 XDY_Entier      *va_Resultat_out
					);


#endif
