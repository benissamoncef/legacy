/* Fichier : $Id: xzea71.h
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea71.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea06.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	23/11/23	: Creation (DEM483)
*
------------------------------------------------------ */

#ifndef xzea71
#define xzea71

/* fichiers inclus */
/* definitions de constantes exportees */
/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZEA71_Abt_Etat_PIC (	XDY_Booleen     va_Abonnement,
							XDY_Datagroup   pa_DGInit,
							XDY_Fonction    pa_FonctUtilisateur,
							XDY_Nom         pa_NomSite,
							int *           pa_Resultat );
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen   va_Abonnement           : Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup pa_DGInit               : Nom du DataGroup d'Init
* XDY_Fonction  pa_FonctUtilisateur     : Nom de la fonction utilisateur
* XDY_Nom       pa_NomSite              : Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int           pa_resultat             : Code retour resultat
*
* CODE RETOUR :
*   XDC_OK              Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des PRV d'un site
*
* FONCTION
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif