/*E*/
/*  Fichier : 	$Id: xzac103.h,v 1.1 2006/12/05 16:17:25 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2006/12/05 16:17:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac103.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/11/04	creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  envoie XDM_ENAV_AUTO 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC103_Commande_NAV_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      *va_Scenario_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Octet      va_site_origine_in);


