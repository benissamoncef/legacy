/*E*/
/* Fichier : $Id: xzac803.h,v 1.1 2012/07/04 17:54:35 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:54:35 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac803.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	02/05/12		Creation (DEM 1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle TACLI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC803_Commande_PRV_TACLI (XDY_Entier    	va_NumEvt_in,
								XDY_Octet     	va_CleEvt_in,
								XDY_Operateur 	va_Operateur_in,
								XDY_Eqt       	va_NumEqt_in,
								XDY_Horodate  	va_HeureLancement_in,
								XDY_Octet      va_Site_in,
								XDY_Bandeau	va_Bandeau_in,
								char      		va_TextePicto_in[20],
								XDY_Octet     	va_Priorite_in,
								XDY_Entier    	va_DistanceEvt_in,
								XDY_Octet      va_site_origine_in);

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de BAF
*
* Sequence d'appel
*
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
*
------------------------------------------------*/



