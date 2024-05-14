/*E*/
/* Fichier : $Id: xzea02.h,v 1.3 1998/01/29 09:24:21 mismer Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/01/29 09:24:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea02.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea02.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Sep 1994	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	22 Jan 1998	: Modif raf syno (DEM/1543)
*
------------------------------------------------------ */

#ifndef xzea02
#define xzea02

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

extern int XZEA02_Abt_Mesures_6gl_Chaussee (	XDY_Booleen	va_Abonnement,
						XDY_Datagroup	pa_DGInit,
						XDY_Fonction	pa_FonctUtilisateur,
						XDY_Nom		pa_NomSite,
						int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
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
*   Ce service est demande pour s'abonner aux mesures 6mn glissantes 
*   par chaussee d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
