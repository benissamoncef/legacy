/*E*/
/*X  Fichier : $Id: xzac192.h,v 1.2 2011/01/14 10:56:59 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/01/14 10:56:59 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac192.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	23/08/06	creation
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : suppression declaration variable 'version'  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC192SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC192_Commande_BAF_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
				 XDY_Octet	va_Module11_in,
				 XDY_Octet	va_Module12_in,
				 XDY_Octet	va_Module15_in,
				 XDY_Octet	va_Module21_in,
				 XDY_Octet	va_Module22_in,
				 XDY_Octet	va_Module25_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Octet      va_site_origine_in);

