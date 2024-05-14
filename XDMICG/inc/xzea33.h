/*E*/
/* Fichier : $Id: xzea33.h,v 1.1 1996/10/04 14:12:41 mismer Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/04 14:12:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea33.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea33.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* D.Mismer	version 1.1	04 Oct 1996	: Creation Pesage RADT (DEM/1227)
*
------------------------------------------------------ */

#ifndef xzea33
#define xzea33

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abt aux mesures de Pesage des stations RADT
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZEA33_Abt_Mesures_Pesage (	XDY_Fonction	pa_FonctUtilSilhouette,
						XDY_Fonction	pa_FonctUtilEssieu,
						XDY_Fonction	pa_FonctUtilPoids,
						XDY_Nom		pa_NomSite,
						int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtilSilhouette	: Nom de la fonction utilisateur pour les silhouettes
* XDY_Fonction	pa_FonctUtilEssieu	: Nom de la fonction utilisateur pour les nb essieu
* XDY_Fonction	pa_FonctUtilPoids	: Nom de la fonction utilisateur pour les poids
* XDY_Nom	pa_NomSite		: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures Pesage RADT
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/

#endif
