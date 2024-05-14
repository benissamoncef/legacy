/*E*/
/*  Fichier : $Id: xzeu.h,v 1.14 2021/05/03 14:05:18 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2021/05/03 14:05:18 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEU  * FICHIER xzeu.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier xzeu.c, interface RAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	29 Nov 1994 	: Creation
* Nagiel.E	version 1.2	30 Nov 1994	:
* Nagiel.E	version 1.3	05 Dec 1994	:
* Nagiel.E	version 1.4	05 Dec 1994	:
* Nagiel.E	version 1.5	05 Dec 1994	:
* Nagiel.E	version 1.6	05 Dec 1994	:
* Nagiel.E	version 1.7	05 Dec 1994	:
* Mismer.D	version 1.8	16 Jan 1995	:
* Lecrivain.JP	version 1.9	18 Jan 1995 	: Ctes. d'etats RAU remontees dans xdc.h 
* Fontaine.C	version 1.10 	24 Mai 1995	: Reprise entete, historique
* JMG		version 1.11	22 oct 2007 : ajout XZEU10_Demande_Etat_Bascul_RAU et XZEUC_ETAT_BASCUL
* JMG		version 1.13	18 aoout 2016 : ajout XZEU11_Bascul_RAU
* ABE		version 1.14	08 Avr 2021 	: Ajout XZEUC_SONO DEM-SAE244
------------------------------------------------------ */

#ifndef XZEU
#define XZEU

/* fichiers inclus  */

#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
/*#include "xzec.h"*/

#include "xzst.h"

/* definitions de constantes exportees */

#define XZEUC_PLATINE_PO1	1
#define XZEUC_PLATINE_PO2	2
#define XZEUC_PLATINE_GN	3

#define XZEUC_ERR_PO		-1

#define XZEUC_DEM_COMM		1
#define XZEUC_APPEL		2
#define XZEUC_LIB_COMM		3
#define XZEUC_GARDE		4
#define XZEUC_REPRISE		5
#define XZEUC_TRANS_GN		6
#define XZEUC_ECOUTE_GN		7
#define XZEUC_TST_UNITAIRE	8
#define XZEUC_TST_SEMI_AUTO	9
#define XZEUC_ETAT_BASCUL	10

#define XZEUC_SONO_ATTENTE	"ATTENTE"
#define XZEUC_SONO_EVACUATION	"EVACUATION"
#define XZEUC_SONO_REPOS	"REPOS"

#define	XZEUC_ETAT_REPOS	XDC_ETAT_RAU_REPOS
#define	XZEUC_ETAT_APPEL	XDC_ETAT_RAU_APPEL
#define	XZEUC_ETAT_COMM		XDC_ETAT_RAU_COMM
#define	XZEUC_ETAT_GARDE	XDC_ETAT_RAU_GARDE
#define	XZEUC_ETAT_DEFAUT	XDC_ETAT_RAU_DEFAUT
#define	XZEUC_ETAT_EN_TEST	XDC_ETAT_RAU_EN_TEST

#define XZEUC_PIRAU_DP	1
#define XZEUC_PIRAU_DA	2
#define XZEUC_PIRAU_DC	3
#define XZEUC_PIRAU_DS	4

/* definitions de types exportes */
                                
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Demande mise en communication pau-platine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU01_Demande_Mise_En_Comm(XDY_NomSite 	va_nomSite,
                                  int 		va_NumPAU,
                                  int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'appel borne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU02_Appel_Borne(XDY_NomSite 	va_nomSite,
                       int 		va_NumPAU,
                       int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande liberation d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU03_Liberation_Ligne(XDY_NomSite 	va_nomSite,
                            int 		va_NumPAU,
                            int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande mise en garde d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU04_Mise_En_Garde(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande reprise garde d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU05_Reprise_Garde(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU,
                         int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande transfert d'une ligne vers la gendarmerie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU06_Transfert_GN(XDY_NomSite 	va_nomSite,
                        int 		va_NumPAU);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande prise en main d'une ligne vers la gendarmerie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU07_Demande_Prise_En_Main_GN(XDY_NomSite 	va_nomSite,
                         	    int 		va_NumPAU);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de test manuel
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU08_Test_Unitaire(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU,
                         int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande test semi-automatique
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU09_Test_Semi_Auto(XDY_NomSite 	va_nomSite,
			  int		va_antenne,
			  int 		va_PO);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande etat basculement RAU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU10_Demande_Etat_Bascul_RAU(XDY_NomSite  va_nomSite);

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   Envoie le message XDM_ECMD_RAU sur le
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande mise en communication pau-platine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZEU11_Bascul_RAU(XDY_Octet         va_site,
							  char *va_zone,
							   char *va_etat);


/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/

#endif
