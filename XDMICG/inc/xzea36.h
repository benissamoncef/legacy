/*E*/
/* Fichier : $Id: xzea36.h,v 1.1 1997/02/24 17:17:48 mismer Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/02/24 17:17:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea36.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures 6mn Temps de parcours >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Fev 1997	: Creation (DEM/1396)
*
------------------------------------------------------ */

#ifndef xzea36
#define xzea36

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abt aux mesures des station M≈t≈o
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZEA36_Abt_TdP_6mn (	XDY_Fonction	pa_FonctUtil,
					XDY_Nom		pa_NomSite,
					int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtil	: Nom de la fonction utilisateur
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
*   Ce service est demande pour s'abonner aux mesures TdP 6 minutes
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/

#endif
