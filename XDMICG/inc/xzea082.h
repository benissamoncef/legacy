/*E*/
/* Fichier : $Id: xzea082.h,v 1.1 2018/05/11 17:07:44 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date : $Date: 2018/05/11 17:07:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea082.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea082.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	23/02/18	creation CTRL_FEUX 1.1 DEM1283
*
------------------------------------------------------ */

#ifndef xzea082
#define xzea082

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

extern int	XZEA082_Abt_Etat_CFE (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des PMV d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
