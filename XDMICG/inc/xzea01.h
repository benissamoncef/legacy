/*E*/
/* Fichier : $Id: xzea01.h,v 1.2 1995/05/24 14:25:31 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/24 14:25:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea01.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Sep 1994	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef xzea01
#define xzea01

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

extern int XZEA01_Abt_Mesures_6gl_Voie (	XDY_Booleen	va_Abonnement,
						XDY_Datagroup	pa_DGInit,
						XDY_Fonction	pa_FonctUtilisateur,
						XDY_NomSite	pa_NomSite,
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
* XDY_Nom	pa_NomSite		: Nom du site demande
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
