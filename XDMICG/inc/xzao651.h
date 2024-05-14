/*E*/
/* Fichier : $Id: xzao651.h,v 1.1 2016/12/08 16:20:55 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/12/08 16:20:55 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao651.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL       15 Nov 2016     : Creation (LOT23)
------------------------------------------------------ */

#ifndef xzao651
#define xzao651

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO651_RPC_NAME "XZAO651"

#define XZAOC_XZAO651_NB_PARM_RETURN 1

/* definitions de types exportes */

int XZAO651_Liste_Viaducs( XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Basedd        va_Basedd_in,
					 XDY_Octet        va_site_in,
					 XDY_Entier      *va_Resultat_out
					);


#endif
