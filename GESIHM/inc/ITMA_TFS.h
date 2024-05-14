/*E*/
/* Fichier : $Id: ITMA_TFS.h,v 1.3 1996/12/05 13:16:42 verdier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/12/05 13:16:42 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TFS * FICHIER ITMA_TFS.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche astreinte
*
*   Cf. DCG 2.4.32
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	19 Oct 1994	: Creation
* Guilhou	14 nov 1996 	: refonte complete de la gestion des astreintes (DEM/1306) 1.2
---------------------------------------------------------------------------- */


/*A
 * Types des identifiants de type et categorie d'astreintes,
 * des informations de duree et d'heure debut d'astreinte
 * ---------------------------------------------------------
 */

DEFINE	TFS_Numero_Type_Astreinte	SYB#INT1_

DEFINE	TFS_Numero_Categorie_Astreinte	SYB#INT1_

DEFINE	TFS_Duree_Astreinte		SYB#INT2_

DEFINE	TFS_Heure_Debut_Astreinte	SYB#DATETIME_

DEFINE	TFS_Date_Astreinte		SYB#DATETIME_

DEFINE	TFS_Sous_Type_Astreinte		SYB#CHAR_

DEFINE	TFS_District_Astreinte		SYB#INT1_

DEFINE	TFS_Inhibition_Astreinte	SYB#INT1_
