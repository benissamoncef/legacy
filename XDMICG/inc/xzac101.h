/*E*/
/*X  Fichier : @(#)xzac101.h	1.1      Release : 1.1        Date : 12/05/06
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac101.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	23/08/06	creation
* VR	04/11/11	Ajout type XDY_Mot (DEM/1014)
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

int XZAC101_Commande_PMV_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 char      	*va_Ligne1_in,
                                 char      	*va_Ligne2_in,
                                 char      	*va_Ligne3_in,
                                 char      	*va_Alternat1_in,
                                 char      	*va_Alternat2_in,
                                 char      	*va_Alternat3_in,
                                 XDY_Octet     	va_Clignotement1_in,
                                 XDY_Octet     	va_Clignotement2_in,
                                 XDY_Octet     	va_Clignotement3_in,
                                 XDY_Octet     	va_Flash_in,
                                 char      	*va_TypePicto_in,
                                 char      	*va_TextePicto_in,
                                 XDY_Octet     	va_ClignotementPicto_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      	*va_LigneDir_in,
                                 char      	*va_AlternatDir_in,
				 XDY_Mot	va_Destination_in,
				 XDY_Mot	va_DestinationAlternat_in,
				 XDY_Octet	va_DepassSeuil_in,
                                 XDY_Octet      va_site_origine_in,
								 XDY_Mot	va_DestinationNoeud_in);

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



