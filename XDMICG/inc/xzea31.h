/*E*/
/* Fichier : $Id: xzea31.h,v 1.2 1995/05/24 14:26:49 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/24 14:26:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea31.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea31.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	12 Fev 1995	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef xzea31
#define xzea31

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abonnement a l'etat des liaisons
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEA31_Abt_Etat_Liaison (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_NomMachine	pa_NomMachine,
					int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen		va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup		pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction		pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_NomMachine	pa_NomMachine		: Nom de la Machine demandee
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des liaisons d'une Machine
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/


#endif
