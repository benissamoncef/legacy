/*E*/
/* Fichier : $Id: xzao790.h,v 1.1 2016/12/11 16:48:04 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/12/11 16:48:04 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao790.h
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

#ifndef xzao790
#define xzao790

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO790_RPC_NAME "XZAO790"

#define XZAOC_XZAO790_NB_PARM_RETURN 1

/* definitions de types exportes */

int XZAO790_Liste_Eqt_SAGA( XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Basedd        va_Basedd_in,
					 XDY_Entier      *va_Resultat_out
					);


#endif
