/*E*/
/*X  Fichier : $Id: xzac182.h,v 1.3 2012/11/07 15:37:49 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2012/11/07 15:37:49 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac182.c
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
* VR	08/06/12 : Ajout TDP PMVA (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC182SP via tacli
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC182_Commande_PMVA_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 char      	va_Ligne1_in[20],
                                 char      	va_Ligne2_in[20],
                                 char      	va_Ligne3_in[20],
                                 char      	va_Ligne4_in[20],
                                 char      	va_Ligne5_in[20],
                                 char      	va_Alternat1_in[20],
                                 char      	va_Alternat2_in[20],
                                 char      	va_Alternat3_in[20],
                                 char      	va_Alternat4_in[20],
                                 char      	va_Alternat5_in[20],
                                 XDY_Octet     	va_Clignotement1_in,
                                 XDY_Octet     	va_Clignotement2_in,
                                 XDY_Octet     	va_Clignotement3_in,
                                 XDY_Octet     	va_Clignotement4_in,
                                 XDY_Octet     	va_Clignotement5_in,
                                 XDY_Octet     	va_Flash_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 XDY_Octet      va_site_origine_in,
                				 XDY_Mot		va_Dest_un_proche_in,
                				 XDY_Mot		va_Dest_un_loin_in,
                				 XDY_Mot		va_Dest_deux_proche_in,
                				 XDY_Mot		va_Dest_deux_loin_in);




