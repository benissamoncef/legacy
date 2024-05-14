/*E Fichier : $Id: xzit.h,v 1.17 2019/06/28 15:26:02 pc2dpdy Exp $      Release : $Revision: 1.17 $        Date : $Date: 2019/06/28 15:26:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE XZIT * FICHIER XZIT.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Il s'agit des primitives d'interface permettant de 
* rafraÑchir l'IHM textuelle.
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	05 Sep 1994	: Creation			V1.1
* NOEL  22 Sep 1994	: ajout xzit06			V1.2
* NOEL  23 Sep 1994	: Chgt titre fct en majuscules 	V1.3
* NOEL  12 Oct 1994	: ajout include xzic et d'un 
*			  parametre dans xzit02		V1.4
* NOEL  14 Oct 1994	: codage XZIT07			V1.6 
* LECRIVAIN 12 Jan 1995	: codage XZIT09, XZIT10		V1.7
* GESCONF-MERCIER 	27 Jan 1995 : ajout de 		V1.9
* XZIT11_Dialogue_Reconfig_machine
* C.T.  14 Mar 1995	: modif interface XZIT08 V1.10
* Niepceron 10 Nov 1996	: ajout du niveau  TFM ds interface XZIT02 et XZIT03 v1.11
* Penot	07 Nov	1997 	: ajout de XZIT12 1.12
* JPL	02/11/2009	: Ajout de deux arguments a XZIT06 1.13
* VR 	14/02/2012 	: Ajout XZIT20 XZIT21(DEM/1016)
* JMG	24/02/14	: ajout XZIT22 1.15
* JMg	12/06/19 	: ajout XZIT23 ODI-C DEM1338 1.16
------------------------------------------------------ */

#ifndef XZIT
#define XZIT

/* fichiers inclus */

#include	"xdc.h"
#include	"xdy.h"
#include	"xdg.h"
#include	"xdm.h"
#include	"xzic.h"
#include	"xzst.h"
#include	<rtworks/common.h>
#ifdef _HPUX_SOURCE
#include        <rtworks/ipc_proto.h>
#endif

#include	<rtworks/ipc.h>

/* definitions de constantes exportees */

#define		XZITC_AVEC_MESSAGE_TFM		TRUE
#define		XZITC_SANS_MESSAGE_TFM		FALSE


/* definitions de types exportes */
typedef union  {
		XDY_NomMachine machine;
		XDY_NomSite site;
} XZITT_Destinataire;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*------------------------------------------------------
* SERVICE RENDU :
* PrÅvient l'IHM textuelle que le Plan d'Actions est prÁt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIT03_PA_Pret(XDY_Evt va_NumFMC, XDY_NomMachine va_PO, XDY_Horodate va_Horodate, XDY_Mot va_NumPA, XDY_Octet va_niv);

/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC     NumÅro de la fiche main courante
* va_PO         Identifiant du Poste OpÅrateur qui a demandÅ le calcul du PA
* va_Horodate   Horodate de calcul
* va_NumPA      NumÅro du PA calculÅ et disponible en BDD
* va_niv	Niv du message NTFM
*
*
* ARGUMENTS EN SORTIE :
* Aucuna
*
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
------------------------------------------------------*/

/*------------------------------------------------------
* SERVICE RENDU :
* Previent l'animateur d'une modification dans la liste des messages Trafic FM
* a diffuser
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIT06_Modif_Animateur_TFM (XDY_Octet va_Commande, XDY_Act va_ActionTFM);

/*
* ARGUMENTS EN ENTREE :
*   va_Commande       : commande eventuelle (ajout, modification, suppression...)
*   va_ActionTFM      : Identifiant de l'action Trafic FM concernee.
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*
*
* FONCTION
*
*
------------------------------------------------------*/

/*------------------------------------------------------
* SERVICE RENDU :
*  PrÅvient l'IHM textuelle d'une modification dans la liste des alarmes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern	int     XZIT07_Modif_Liste_Alarmes();
/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*	aucun
* ARGUMENTS EN SORTIE :
*       Aucun
*
*	aucun
*
* CODE RETOUR :
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*       Utilise par la base de donnees des qu'une modification dans la liste des
*       alarmes apparait. L'alarme est envoyÅe È tout le site d'appartenance du
*       serveur de donnÅes.
*
------------------------------------------------------*/
/*------------------------------------------------------
* SERVICE RENDU :
* PrÅvient d'une modification dans la liste des fiches main courante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern	int     XZIT08_Modif_Liste_FMC(XDY_Evt va_NumFMC, XZITT_Destinataire va_nomDestinataire,
				XDY_TexteReveil  va_TexteReveil);
/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC             Identifiant de la FMC concernÅe par la modification. Sa
*                       valeur est NULL si pas de rÅveil mais autre type de modification.
* va_nomDestinataire    Est compose du nom du district ou du poste opÅrateur concerne.
* va_TexteReveil        Texte du reveil si les champs de va_NumFMC sont diffÅrents de NULL.
*
* ARGUMENTS EN SORTIE :
*
*       Aucun
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*
*       Utilise par BDD si modification dans la liste des FMC.
*
------------------------------------------------------*/
						
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIT02_Message_TFM_Pret(XDY_Evt va_NumFMC, XDY_NomMachine va_PO,
			XDY_Booleen va_Existance_message,
			XDY_Horodate va_Horodate, XDY_Mot va_NumPA,
			XDY_Octet va_nivTFM,
			char* pa_TexteMsg);

/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC		Numero de la fiche main courante concernee
* va_PO			Identifiant du poste opirateur concerne
* va_Existance_message	Defini si le message TFM a ete calcule ou pas.			
*			XZITC_AVEC_MESSAGE_TFM ou XZITC_SANS_MESSAGE_TFM
* va_Horodate		
* va_NumPA		Numero du plan d'actions
* va_NivTFM		Niveau TFM
* pa_TexteMsg		Texte du message trafic FM
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR :
* XDC_OK                Bonne exicution de la primitive
* XDC_NOK               Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION
*Construction et emission du message XDM_IMESSAGE_TFM_PRET dans le datagroup
* XDG_IMESSAGE_TFM_PRET_numpo
*
------------------------------------------------------*/



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU : 
* Demande l'IHM l'ouverture de la boite de dialogue de sauvegarde historique
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIT09_Dialogue_Sauvegarde_Histo (XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*
* va_PO		Identifiant du Poste OpÅrateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'exÅcution de la fonction
* XDC_OK	Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert È la tache GMOT.
*
* FONCTION 
*  Appel d'une fonction commune sauvegarde / restauration de l'historique
*
*-----------------------------------------------------------*/




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU : 
* Demande l'IHM l'ouverture de la boite de dialogue de restauration historique
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIT10_Dialogue_Restauration_Historique (XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*
* va_PO		Identifiant du Poste OpÅrateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'exÅcution de la fonction
* XDC_OK	Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert È la tache GMOT.
*
* FONCTION 
*  Appel d'une fonction commune sauvegarde / restauration de l'historique
*
*-----------------------------------------------------------*/

extern int	XZIT09_10_Sauvegarde_Restauration (XDY_NomMachine va_PO, XDY_Booleen va_sauvegarde);

/*-----------------------------------------------------------------------------
* SERVICE RENDU :
* Demande l'IHM l'ouverture de la boite de dialogue de reconfiguration machine
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int     XZIT11_Dialogue_Reconfig_machine (XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*
* va_PO         Identifiant du Poste OpÅrateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*       Service offert È la tache GMOT.
*
* FONCTION
*  Construction et Åmission du message XDM_IMODIF_FMC dans le Datagroup
*  XDG_IMODIF_FMC_Po
*
*-----------------------------------------------------------*/

extern int     XZIT12_Evt_Strada();

/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*
* ARGUMENTS EN SORTIE :
*       Aucun
*
*
* CODE RETOUR :
* 
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*
*       Utilise par la base de donnees des qu'un evenement STRADA est arrive
*
* FONCTION
*  Construction et �mission du message  dans le Datagroup
* 
*
------------------------------------------------------*/

extern int XZIT20_Fin_Simulation(XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*
* ARGUMENTS EN SORTIE :
*       Aucun
*
*
* CODE RETOUR :
* 
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*       Utilise par la base de donnees des qu'un evenement STRADA est arrive
*
* FONCTION
*  Construction et �mission du message  dans le Datagroup
* 
*
------------------------------------------------------*/
extern int XZIT21_Simulation_En_Cours(XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*
* ARGUMENTS EN SORTIE :
*       Aucun
*
*
* CODE RETOUR :
* 
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*       Utilise par la base de donnees des qu'un evenement STRADA est arrive
*
* FONCTION
*  Construction et �mission du message  dans le Datagroup
*
*
------------------------------------------------------*/
extern int XZIT22_Fax_Auto(XDY_Entier NumPlan, XDY_Entier NumOrdre, XDY_Entier NumFmc, XDY_Octet CleFmc, char* LibelleFmc, XDY_Entier Auto, XDY_NomMachine va_PO);

/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*
* ARGUMENTS EN SORTIE :
*       Aucun
*
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*       Utilise par la base de donnees des qu'un evenement STRADA est arrive
*
* FONCTION
*  Construction et �mission du message  dans le Datagroup
* 
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  PrÅvient l'IHM textuelle d'une modification dans la liste des alarmes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern	int     XZIT22_Modif_Liste_Alarmes_Crit();
/*
* ARGUMENTS EN ENTREE :
*       Aucun
*
*	aucun
* ARGUMENTS EN SORTIE :
*       Aucun
*
*	aucun
*
* CODE RETOUR :
* XDC_NOK       Erreur d'exÅcution de la fonction
* XDC_OK        Bonne exÅcution de la fonction
*
* CONDITION D'UTILISATION
*       Utilise par la base de donnees des qu'une modification dans la liste des
*       alarmes apparait. L'alarme est envoyÅe È tout le site d'appartenance du
*       serveur de donnÅes.
*
------------------------------------------------------*/

int XZIT23_Info_Message(char *, XDY_NomMachine);

#endif
