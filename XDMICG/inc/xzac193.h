/*E*/
/*X  Fichier : $Id: xzac193.h,v 1.1 2006/12/05 16:17:26 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:17:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac193.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*
* JMG	23/08/06	creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC193SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC193_Commande_BAD_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      va_Scenario_in[20],
                                 XDY_Octet      va_Site_in,
                                 XDY_Octet      va_site_origine_in);





