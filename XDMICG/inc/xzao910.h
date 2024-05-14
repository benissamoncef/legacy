/*E*/
/* Fichier : $Id: xzao910.h,v 1.1 2016/09/29 15:36:59 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:36:59 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao910.h
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

#ifndef xzao910
#define xzao910

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO910_RPC_NAME "XZAO910"

#define XZAOC_XZAO910_NB_PARM_RETURN 1

/* definitions de types exportes */

int XZAO910_Liste_Zones_RAU( XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Basedd        va_Basedd_in,
					 XDY_Octet        va_site_in,
					 XDY_Entier      *va_Resultat_out
					);


#endif
