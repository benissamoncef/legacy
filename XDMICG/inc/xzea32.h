/*E*/
/* Fichier : $Id: xzea32.h,v 1.2 1996/09/17 10:02:05 mismer Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/17 10:02:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea32.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea32.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* F.VOLCIC	version 1.1	19 Sep 1996	: Creation
* D.Mismer	version 1.2	17 Sep 1996	: Suppression du site en entree (RADT_W)
*
------------------------------------------------------ */

#ifndef xzea32
#define xzea32

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEA32_Abt_Mesures_6mn_parVoie (	XDY_Booleen	va_Abonnement,
						XDY_Datagroup	pa_DGInit,
						XDY_Fonction	pa_FonctUtilisateur,
						XDY_Mot		va_NoEqt,
						XDY_Sens	va_Sens,
						XDY_Voie	va_Voie,
						int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Mot	va_NoEqt		: Numero d'equipement
* XDY_Sens	va_Sens			: Sens
* XDY_Voie	va_Voie			: Voie
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures 6mn glissantes 
*   par voie d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
