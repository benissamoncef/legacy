/*E*/
/*X  Fichier : $Id: xzac106.h,v 1.2 2011/01/14 10:57:08 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/01/14 10:57:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac106.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*
* JMG	23/08/106	creation
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : suppression declaration variable 'version'  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC106SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC106_Commande_Tunnel_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 XDY_Octet	va_Sequence_in,
				 XDY_Octet	va_site_in);





