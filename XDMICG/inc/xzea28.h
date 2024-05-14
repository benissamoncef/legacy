/*E*/
/* Fichier : $Id: xzea28.h,v 1.4 1995/05/24 14:26:44 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/24 14:26:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea28.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea28.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Oct 1994	: Creation
* Guilhou.JM	version 1.2	13 Oct 1994	: Constrate-->Contraste
* Mismer.D	version 1.3	09 Nov 1994	:
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef xzea28
#define xzea28

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats de contraste d'affichage des PMV et NAV ... (Surbrillance) >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA28_Abt_Etat_Contraste (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats contraste des PMV et des NAV d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/


#endif
