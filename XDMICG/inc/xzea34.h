/*E*/
/* Fichier : $Id: xzea34.h,v 1.1 1996/11/21 15:50:15 mismer Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/11/21 15:50:15 $
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
* D.Mismer	version 1.1	21 Nov 1996	: Creation pour METEO (DEM/1232)
*
------------------------------------------------------ */

#ifndef xzea34
#define xzea34

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

extern int	XZEA34_Abt_Mesures_Meteo (	XDY_Fonction	pa_FonctUtil,
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
*   Ce service est demande pour s'abonner aux mesures Meteo
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/

#endif
