/*E*/
/* Fichier : $Id: xzea37.h,v 1.1 1997/02/24 17:18:09 mismer Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/02/24 17:18:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea37.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures 1heu Temps de parcours >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Fev 1997	: Creation (DEM/1396)
*
------------------------------------------------------ */

#ifndef xzea37
#define xzea37

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

extern int	XZEA37_Abt_TdP_1heu (	XDY_Fonction	pa_FonctUtil,
					int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtil	: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures TdP horaire
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/

#endif
