/*E*/
/*Fichier :  $Id: xzea55.h,v 1.1 2008/02/13 13:24:42 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea55.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzea55.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    version 1.1    21 Septembre 2007    : Creation DEM 662
*
------------------------------------------------------ */

#ifndef xzea55
#define xzea55

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des domaines de la GTC de Nice ( GTC Renovee )  >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int    XZEA55_Abt_Etat_Domaine ( XDY_Booleen    va_Abonnement,
                                        XDY_Datagroup  pa_DGInit,
                                        XDY_Fonction   pa_FonctUtilisateur,
                                        XDY_Nom        pa_NomSite,
                                        XDY_Entier     va_Domaine,
                                        XDY_Eqt        va_TypeEqt,
                                        XDY_Eqt        va_NumEqt,
                                        int *          pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen     va_Abonnement       : Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup   pa_DGInit           : Nom du DataGroup d'Init
* XDY_Fonction    pa_FonctUtilisateur : Nom de la fonction utilisateur
* XDY_Nom         pa_NomSite          : Nom du site demande
* XDY_Eqt         va_TypeEqt          : Type equipement
* XDY_Eqt         va_NumEqt           : Numeroequipement
* XDY_Eqt         va_Domaine          : Numero domaine
*
* ARGUMENTS EN SORTIE :
*
* int             pa_resultat         : Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK        Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des capteurs de la GTC Nice
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

#endif
