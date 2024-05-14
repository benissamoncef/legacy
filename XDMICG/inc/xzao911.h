/*E*/
/* Fichier : $Id: xzao911.h,v 1.1 2016/09/29 15:37:04 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:37:04 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao911.h
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

#ifndef xzao911
#define xzao911

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO911_RPC_NAME "XZAO911"

#define XZAOC_XZAO911_NB_PARM_RETURN 0

/* definitions de types exportes */

int XZAO911_Ecrire_Zone_RAU( char *va_zone,
				char *va_etat);


#endif
