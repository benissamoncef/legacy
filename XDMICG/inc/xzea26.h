/*E*/
/* Fichier : $Id: xzea26.h,v 1.3 1995/09/22 11:02:44 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/09/22 11:02:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea26.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea26.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Fev 1995	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	21 Sep 1995	: Modif argument d'entree
*
------------------------------------------------------ */

#ifndef xzea26
#define xzea26

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

extern int XZEA26_Abt_Etat_Mat_NAV (	XDY_Booleen,
					XDY_Datagroup,
					XDY_Fonction,
					XDY_Eqt,
					int * );

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
*   Ce service est demande pour s'abonner aux etats Matrice des NAV d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/


#endif
