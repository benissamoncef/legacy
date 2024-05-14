/*E*/
/* Fichier : $Id: xzea25.h,v 1.3 1995/05/24 14:26:34 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/24 14:26:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea25.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea25.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Jan 1995	: Creation
* Mismer.D	version 1.2	02 Jan 1995	: 
* Fontaine.C	version 1.3	24 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef xzea25
#define xzea25

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

extern int	XZEA25_Abt_Etat_PAU (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					XDY_Eqt		va_NumPAU,
					int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
* XDY_Eqt	va_NumPAU		: Numero du PAU demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats d'un PAU d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/


#endif
