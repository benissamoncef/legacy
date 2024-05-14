/*E*/
/*Fichier : $Id: xzev.h,v 1.5 2017/02/28 14:46:53 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2017/02/28 14:46:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEV  * FICHIER xzev.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzev.c, interface video >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	27 Oct 1994	: Creation
* Fontaine.C	version 1.2	24 mai 1995	: Reprise entete, historique
* Mismer.D	version 1.3	24 Avr 1997	: Commande magneto EQT2 (DEM/1427)
* PNI		version 1.4	19 mai 2011	: ajout XZEVC_JOYSTICK_P02 
* JPL		27/10/16 : Modif. ctes. positions, en fichier separe; suppression ctes. joysticks  (DEM 1189)  1.5
------------------------------------------------------ */

#ifndef XZEV
#define XZEV

/* fichiers inclus  */

#include "xdm.h"
#include "xdy.h"
#include "xdg.h"

#include "xzec.h"
#include "xzevc.h"

#include "xzst.h"

/* definitions de constantes exportees */

#define XZEVC_ERR_POSITION		2


/* definitions de types exportes */
                                
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affectation canal moniteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV01_Affecte_Canal_Moniteur(XDY_NomSite 	va_nomSite,
                                  int 		va_NumCanal,
                                  int 		va_NumMoniteur,
                                  int 		va_NumLT);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumCanal : no du canal a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_NumLT : no du local technique
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affectation camera a un moniteur et a un joystick
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV02_Affecte_Image_Monit_Joy(XDY_NomSite 	va_nomSite,
                                  int 		va_NumImage,
                                  int 		va_NumMoniteur,
                                  int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_PO : Numero poste operateur = no joystick
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de positionnement camera 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV03_Positionnement(XDY_NomSite 	        va_nomSite,
                                  int 		va_NumImage,
                                  int 		va_NumMoniteur,
                                  int 		va_IdPosition);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_IdPosition : Numero de position de camera
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEVC_ERR_POSITION  position camera invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de memorisation positionnement camera 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV04_Memorise_Position(XDY_NomSite 	va_nomSite,
                             int 		va_NumImage,
                             int 		va_IdPosition);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_IdPosition : Numero de position de camera
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEVC_ERR_POSITION  position camera invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'instinction de moniteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV05_Eteint_Moniteur(XDY_NomSite 	va_nomSite,
                           int 		va_NumMoniteur);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumMoniteur :no du moniteur a eteindre
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande magnetoscope
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEV06_Commande_Magneto(XDY_NomSite 	va_nomSite,
                             int 		va_TypeCmd,
                             int 		va_NumImage,
                             int 		va_NumMagneto,
                             int 		va_NumMoniteur);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_TypeCmd : Type de commande (Enregistre,arret,retour,etc..)
* va_NumImage : no image camera a affecter
* va_NumMagneto : Numero de magneto sur lequel enregistrer l'image
* va_NumMoniteur : no moniteur pour sortie magneto
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/

#endif
