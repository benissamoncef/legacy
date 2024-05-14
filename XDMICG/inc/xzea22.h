/*E*/
/* Fichier : $Id: xzea22.h,v 1.3 1995/05/24 14:26:25 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/24 14:26:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea22.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea22.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	21 Nov 1994	: Creation
* Mismer.D	version 1.2	18 Mai 1995	: Ajout type et numero Equipement en parametre d'entree 
* Fontaine.C	version 1.3	24 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef xzea22
#define xzea22

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des panneaux de la GTC de Nice ( Panneaux et FAV )  >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZEA22_Abt_Etat_Telealarme (	XDY_Booleen	va_Abonnement,
						XDY_Datagroup	pa_DGInit,
						XDY_Fonction	pa_FonctUtilisateur,
						XDY_Nom		pa_NomSite,
						XDY_Eqt		va_TypeEqt,
						XDY_Eqt		va_NumEqt,
						int *		pa_Resultat );
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
* XDY_Eqt	va_TypeEqt		: Type equipement ( Tube ou Echangeur )
* XDY_Eqt	va_NumEqt		: Numero du tube ou de l'echangeur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des Telealarmes de la GTC Nice
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
