/*E*/
/* Fichier : $Id: xzea23.h,v 1.4 1995/05/24 14:26:30 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/24 14:26:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea23.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du ficheir xzea23.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	21 Nov 1994	: Creation
* Mismer.D	version 1.2	02 Dec 1994	:
* Mismer.D	version 1.3	18 Mai 1995	: Ajout numero tube en parametre d'entree 
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef xzea23
#define xzea23

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des panneaux de la GTC de Nice   >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZEA23_Abt_Etat_Sequence_Tube (	XDY_Booleen	va_Abonnement,
						XDY_Datagroup	pa_DGInit,
						XDY_Fonction	pa_FonctUtilisateur,
						XDY_Nom		pa_NomSite,
						XDY_Eqt		va_NumEqt,
						int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Eqt	va_NumEqt		: Numero du tube 
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
*   Ce service est demande pour s'abonner aux etats des sequences tube de la GTC Nice
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
