/*E*/
/*Fichier : $Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $      Release : $Revision: 1.28 $        Date : $Date: 2020/12/04 08:38:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE XZIT * FICHIER xzit.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module regroupe tous les services de rafraichissement 
* de l'IHM textuelle.
*
*
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	05 Sep 1994	: Creation			V1.1
* NOEL  22 Sep 1994     : ajout xzit06          	V1.2
* NOEL  23 Sep 1994     : chgt titre fct en majuscules	V1.3
* NOEL  06 Oct 1994     : ajout XZIT02			V1.4
* GUILHOU  13 Oct 1994  : correction malloc		V1.5
* NOEL	14 Oct 1994	: codage XZIT07 		V1.7
* NOEL	14 Oct 1994	: codage XZIT	  		V1.8
* NOEL	21 Oct 1994	: correction malloc 		V1.9
* NOEL	28 Dec 1994	: Flush dans xzit06		V1.11
* LECRIVAIN 12 Jan 1995	: ajout xzit09, xzit10
* LECRIVAIN 27 Jan 1995	: ajout xzit11			V1.14
* C.T.  14 Mar 1995	: modif interface xzit08	V1.15
* Niepceron 05 Nov 1996 : ajout niveau tfm xit02 et xzit03(dem/1248) V1.16
* Penot	 06 Nov 1997 : codage XZIT12 		V1.17
* JPL	02/11/2009	: Ajout de deux arguments a XZIT06 (DEM 917) 1.18
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : XDG_EncodeDG2 : utiliser un champ d'union  1.19
* JPL	09/01/2012	: Changement libelles "trafic FM" en "RVA" (DEM 1009)  1.20
* VR	25/05/12	: ajout xzit20 et 21 (DEM/1016)
* JMG	24/02/14	: ajout xzit22 1.22
* JMG	27/02/19 11:10:40 : envoi poke pour RVA si CI et nouvelle FMC dans XZIT08 1.23
* JMG	12/06/19 : ajout XZIT23 DEM1338 ODI-C 1.24
* ABE	24/09/20 : Ajout envoie message sur XDM_AS_TIMER_DATEX2	vers XDG_TIMER_TAGEN pour d�clencher une mise � jour d'une FMC pour le datex2 DEM-SAE130 1.26
* GGY	09/03/23 : Suppression de l'envoie message sur XDM_AS_TIMER_DATEX2 (deplace dans le trigger fmc_gen_iu_trg)	(DEM-SAE438)			
* LCL	03/10/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzit.h"
#include "xzsc.h"

/* definitions de constantes */

#define		CM_ComplementMax	100	/* taille maximale du complement d'un message RTWorks */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : xzit" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIT02_Message_TFM_Pret(XDY_Evt va_NumFMC, XDY_NomMachine va_PO,
			XDY_Booleen va_Existance_message,
			XDY_Horodate va_Horodate, XDY_Mot va_NumPA,
			XDY_Octet va_nivTFM,
			char* pa_TexteMsg)

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
* XDC_OK		Bonne execution de la primitive
* XDC_NOK		Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION 
*Construction et emission du message XDM_IMESSAGE_TFM_PRET dans le datagroup
* XDG_IMESSAGE_TFM_PRET_nompo
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT02_Message_TFM_Pret" ;

XZSCT_Datagroup		vl_DG;
char			*pl_str;

/*A Controle des variables en entree */

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IMESSAGE_TFM_PRET, va_PO);

XZST_03EcritureTrace(XZSTC_INTERFACE,
		"%s datagroup %s,texte %s, num %d,cle %d,message %d,plan %d,niv %d,date %f",
			version,
			vl_DG,
			pa_TexteMsg,va_NumFMC.numero,va_NumFMC.cle,
			va_Existance_message,
			va_NumPA,va_nivTFM,va_Horodate);

/*A Construction et envoi du message XDM_IMESSAGE_TFM_PRET*/
pl_str=(char *)malloc(strlen(pa_TexteMsg) + 1 + CM_ComplementMax);
sprintf(pl_str,"%d|%d|%d|%d|%d|%lf|%s",va_NumFMC.numero,
				va_NumFMC.cle,
				va_Existance_message,
				va_NumPA,
				va_nivTFM,
				va_Horodate,
				pa_TexteMsg);
	
if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IMESSAGE_TFM_PRET),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE, 
	T_IPC_FT_INT4, COM_CANAL_MTMC_TFM, 			/* numero du poke.*/
	T_IPC_FT_STR,  pl_str, 
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message RVA au poste exploitant");
  free(pl_str);
  return(XDC_NOK);
}
TipcSrvFlush();
free(pl_str);
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Pr�vient l'IHM textuelle que le Plan d'Actions est pr�t 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT03_PA_Pret(XDY_Evt va_NumFMC, XDY_NomMachine va_PO, XDY_Horodate va_Horodate, XDY_Mot va_NumPA, XDY_Octet va_niv)

/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC     NumEro de la fiche main courante
* va_PO         Identifiant du Poste OpErateur qui a demandE le calcul du PA
* va_Horodate   Horodate de calcul
* va_NumPA      NumEro du PA calculE et disponible en BDD
* va_niv        Niv du message NTFM
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'ex�cution de la fonction
* XDC_OK	Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert � BDD qui est appel� des qu'un PA a �t� mis en base.
*
* FONCTION 
*  Construction et �mission du message XDM_IPA_PRET dans le Datagroup
*  XDG_IPA_PRET_NomPO
*
*-----------------------------------------------------------*/

{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT03_PA_Pret" ;

XZSCT_Datagroup		vl_DG;
char			*pl_str;

/*A Controle des variables en entree */

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IPA_PRET, va_PO);

/*A Construction et envoi du message XDM_IPA_PRET */
XZST_03EcritureTrace(XZSTC_INTERFACE,
		"%s datagroup %s, num %d,cle %d,plan %d,date %f",
		version, vl_DG,va_NumFMC.numero,va_NumFMC.cle,va_NumPA,va_Horodate);
pl_str=(char *)malloc(CM_ComplementMax);
sprintf(pl_str,"%d|%d|%d|%lf|%d",va_NumFMC.numero,
				va_NumFMC.cle,
				va_NumPA,
				va_Horodate,
				(int) va_niv);


if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IPA_PRET),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTMC_PA,	/* numero du poke associe a la boite de dialogue */
	T_IPC_FT_STR,  pl_str, 
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message RVA au poste exploitant");
  free(pl_str);
  return(XDC_NOK);
}
TipcSrvFlush();
free(pl_str);
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Pr�vient l'animateur d'une modification dans la liste des messages Trafic FM 
* a diffuser
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT06_Modif_Animateur_TFM (XDY_Octet va_Commande, XDY_Act va_ActionTFM)

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
* Construction et emission du message XDM_IMODIF_LISTE_TFM dans le datagroup
* XDG_IMODIF_LISTE_TFM
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT06_Modif_Animateur_TFM" ;

char			pl_str[40];

/*A Construction et envoi du message XDM_IMODIF_LISTE_TFM */
XZST_03EcritureTrace(XZSTC_INTERFACE,
		"%s datagroup %s, commande %d, action %d, site %d",
		version, XDG_IMODIF_LISTE_TFM, (int) va_Commande, (int) va_ActionTFM.numero, (int) va_ActionTFM.cle);
sprintf(pl_str,"%d|%d|%d", (int) va_Commande, (int) va_ActionTFM.numero, (int) va_ActionTFM.cle);

if (!TipcSrvMsgWrite(XDG_IMODIF_LISTE_TFM,
     	XDM_IdentMsg(XDM_IMODIF_LISTE_TFM),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,	/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTMA, 	/* numero du poke associe a la boite de dialogue receptrice*/
	T_IPC_FT_STR,  pl_str, 
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IMODIF_LISTE_TFM");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE, "%s OUT ", version);
return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Pr�vient l'IHM textuelle d'une modification dans la liste des alarmes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT07_Modif_Liste_Alarmes(
          )

/*
* ARGUMENTS EN ENTREE :
*	Aucun 
*
*
* ARGUMENTS EN SORTIE :
*	Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Utilise par la base de donnees des qu'une modification dans la liste des 
*	alarmes apparait. L'alarme est envoy�e � tout le site d'appartenance du
*	serveur de donn�es.
*
* FONCTION 
*  Construction et �mission du message XDM_IMODIF_ALARMES dans le Datagroup
*  XDG_IMODIF_ALARMES_site
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT07_Modif_Liste_Alarmes" ;

XZSCT_Datagroup		vl_DG;
XZSCT_NomSite 		vl_NomSite;


XZST_03EcritureTrace(XZSTC_INTERFACE,"%s IN", version);
/*A recuperation du nom du site */
if (XDC_OK != XZSC_04NomSite(vl_NomSite))
	XZST_03EcritureTrace(XZSTC_WARNING, "impossible de recup�rer le non du site");

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IMODIF_ALARMES, vl_NomSite);

/*A Construction et envoi du message XDM_IMODIF_ALARMES */
XZST_03EcritureTrace(XZSTC_INTERFACE, "datagroup %s", vl_DG);

if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IMODIF_ALARMES),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTMT_ALM, 		/* numero du poke associe a la boite de dialogue */
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IMODIF_ALARMES");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Pr�vient l'IHM evenement Strada 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT12_Evt_Strada()

/*
* ARGUMENTS EN ENTREE :
*	Aucun 
*
*
* ARGUMENTS EN SORTIE :
*	Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Utilise par la base de donnees des qu'un evenement STRADA est arrive
*
* FONCTION 
*  Construction et �mission du message  dans le Datagroup
*  
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT12_Evt_Strada" ;


XZST_03EcritureTrace(XZSTC_INTERFACE,"%s IN", version);


/*A Construction et envoi du message XDM_IMODIF_ALARMES */
XZST_03EcritureTrace(XZSTC_INTERFACE, "datagroup %s", XDG_IAEVT_STRADA);

if (!TipcSrvMsgWrite(XDG_IAEVT_STRADA,
     	XDM_IdentMsg(XDM_IAEVT_STRADA),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTES_MODIF, 		/* numero du poke associe a la boite de dialogue */
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IMODIF_ALARMES");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Pr�vient d'une modification dans la liste des fiches main courante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	XZIT08_Modif_Liste_FMC (XDY_Evt va_NumFMC, XZITT_Destinataire va_nomDestinataire,
				XDY_TexteReveil  va_TexteReveil)
/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC		Identifiant de la FMC concern�e par la modification. Sa
*			valeur est NULL si pas de r�veil mais autre type de modification.
* va_nomDestinataire 	Est compose du nom du district ou du poste op�rateur concerne.
* va_TexteReveil 	Texte du reveil si les champs de va_NumFMC sont diff�rents de NULL.
*
* ARGUMENTS EN SORTIE :
*
*	Aucun
*
* CODE RETOUR : 
* 
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Utilise par BDD si modification dans la liste des FMC.
*
* FONCTION 
*  Construction et �mission du message XDM_IMODIF_FMC dans le Datagroup
*  XDG_IMODIF_FMC_Po
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT08_Modif_Liste_FMC" ;

XZSCT_Datagroup		vl_DG;		/* datagroup d'emission */
char			*pl_str;	/* contenu utile du message pour MTLT */
  XDY_Act vl_action_tfm;

XZST_03EcritureTrace(XZSTC_INTERFACE,"%s IN", version);
/*A Construction du datagroup d'emission */
/* JPL 07/12/10 : utiliser l'adresse d'un champ est preferable a celle du compose de type 'union' */
XDG_EncodeDG2(vl_DG,XDG_IMODIF_FMC, va_nomDestinataire.site);

/*A Construction et envoi du message XDM_IMODIF_FMC */
XZST_03EcritureTrace(XZSTC_INTERFACE,
		"datagroup %s, num %d,cle %d,reveil %s. site=%s.",
		vl_DG,va_NumFMC.numero,va_NumFMC.cle,va_TexteReveil,va_nomDestinataire.site);
XZST_03EcritureTrace(XZSTC_WARNING,
		"datagroup %s, num %d,cle %d,reveil %s. site=%s.",
		vl_DG,va_NumFMC.numero,va_NumFMC.cle,va_TexteReveil,va_nomDestinataire.site);
pl_str=(char *)malloc(strlen(va_TexteReveil) + 1 + CM_ComplementMax);
sprintf(pl_str,"%d|%d|%s",va_NumFMC.numero,
			va_NumFMC.cle,
			va_TexteReveil);

if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IMODIF_FMC),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTLT, 		/* numero du poke associe a la boite de dialogue */
	T_IPC_FT_STR,  pl_str, 
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IMODIF_FMC");
  free(pl_str);
  return(XDC_NOK);
}
free(pl_str);

/* si reveil nouvelle FMC recu au CI alors on rafraichie la liste msg RVA pour y ajouter le 1 msg RVA automatqiue */
if (((!strcmp(va_nomDestinataire.site,"POCI1"))||
     (!strcmp(va_nomDestinataire.site,XDC_NOM_SITE_CI))) &&
    (strstr(va_TexteReveil,XDC_REV_FMC_CRE)!=NULL)) 
{
   	/*!strncmp(va_TexteReveil,XDC_REV_FMC_CRE,strlen(XDC_REV_FMC_CRE))) {*/
	vl_action_tfm.numero=0;
	vl_action_tfm.cle = XDC_CI;
		
  	XZST_03EcritureTrace(XZSTC_WARNING,"Appel XZIT06 CI");
	XZIT06_Modif_Animateur_TFM(XDC_AJOUT, vl_action_tfm);
} else
	if (((!strcmp(va_nomDestinataire.site,"POCA1"))||
		(!strcmp(va_nomDestinataire.site,XDC_NOM_SITE_CA))) &&
		(strstr(va_TexteReveil,XDC_REV_FMC_CRE)!=NULL)) 
	{
		vl_action_tfm.numero=0;
		vl_action_tfm.cle = XDC_CA;
			
		XZST_03EcritureTrace(XZSTC_WARNING,"Appel XZIT06 CA");
		XZIT06_Modif_Animateur_TFM(XDC_AJOUT, vl_action_tfm);
	}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU : 
* Demande l'IHM l'ouverture de la boite de dialogue de sauvegarde historique
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT09_Dialogue_Sauvegarde_Histo (XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO		Identifiant du Poste Op�rateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'ex�cution de la fonction
* XDC_OK	Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert � la tache GMOT.
*
* FONCTION 
*  Appel d'une fonction commune sauvegarde / restauration de l'historique
*
*-----------------------------------------------------------*/

{
  return (XZIT09_10_Sauvegarde_Restauration (va_PO, (XDY_Booleen) XDC_VRAI));
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU : 
* Demande l'IHM l'ouverture de la boite de dialogue de restauration historique
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT10_Dialogue_Restauration_Historique (XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO		Identifiant du Poste Op�rateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'ex�cution de la fonction
* XDC_OK	Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert � la tache GMOT.
*
* FONCTION 
*  Appel d'une fonction commune sauvegarde / restauration de l'historique
*
*-----------------------------------------------------------*/

{
  return (XZIT09_10_Sauvegarde_Restauration (va_PO, (XDY_Booleen) XDC_FAUX));
}



int	XZIT09_10_Sauvegarde_Restauration (XDY_NomMachine va_PO, XDY_Booleen va_sauvegarde)

{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT09_10_Sauvegarde_Restauration" ;

XZSCT_Datagroup		vl_DG;
char			pl_str[20];

/*A Controle des variables en entree */

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IARCHIVAGE, va_PO);

/*A Construction et envoi du message XDM_IARCHIVAGE */
XZST_03EcritureTrace(XZSTC_INTERFACE, "%s datagroup %s", version, vl_DG);
sprintf(pl_str,"%s|%s", "GMOT", va_sauvegarde ? "S" : "R");


if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IARCHIVAGE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_NEWTASK,	/* commande d'execution de fonction */
	T_IPC_FT_STR, "ITMA_TSR_Sauv_Restaur_Histo",
	T_IPC_FT_STR,  pl_str, 
	T_IPC_FT_INT4, 0,
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message d'archivage");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU : 
* Demande l'IHM l'ouverture de la boite de dialogue de reconfiguration machine
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT11_Dialogue_Reconfig_machine (XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO		Identifiant du Poste Op�rateur.
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'ex�cution de la fonction
* XDC_OK	Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Service offert � la tache GMOT.
*
* FONCTION 
*  Construction et �mission du message XDM_IMODIF_FMC dans le Datagroup
*  XDG_IMODIF_FMC_Po
*
*-----------------------------------------------------------*/

{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT11_Dialogue_Reconfig_machine" ;

XZSCT_Datagroup		vl_DG;

/*A Controle des variables en entree */

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IARCHIVAGE, va_PO);

/*A Construction et envoi du message XDM_IARCHIVAGE */
XZST_03EcritureTrace(XZSTC_INTERFACE, "%s datagroup %s", version, vl_DG);


if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IARCHIVAGE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_NEWTASK,	/* commande d'execution de fonction */
	T_IPC_FT_STR, "ITMA_TRM_Reconfig_Machine",
	T_IPC_FT_STR,  "GMOT", 
	T_IPC_FT_INT4, 0,
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message de reconfiguration machine");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIT20_Fin_Simulation(XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_NumFMC		Numero de la fiche main courante concernee
* va_PO			Identifiant du poste opirateur concerne
* pa_TexteMsg		Texte du message trafic FM
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR : 
* XDC_OK		Bonne execution de la primitive
* XDC_NOK		Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION 
*Construction et emission du message XDM_IAX_SENDPOKE dans le datagroup
* XDG_DEMANDE_SIMU
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "@(#)xzit.c	1.19 12/07/10 : XZIT20_Fin_Simulation" ;


XZSCT_Datagroup		vl_DG;
char			*pl_str;

/*A Controle des variables en entree */


/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IPA_PRET, va_PO);

XZST_03EcritureTrace(XZSTC_INTERFACE, "datagroup %s", vl_DG);	


/*A Construction et envoi du message XDM_IAX_SENDPOKE*/

				
if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IAX_SENDPOKE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_FIN_SIMU, 		/* numero du poke associe a la boite de dialogue */
       	NULL))				
				

{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IAX_SENDPOKE");
  return(XDC_NOK);
}
TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIT21_Simulation_En_Cours(XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO			Identifiant du poste opirateur concerne
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR : 
* XDC_OK		Bonne execution de la primitive
* XDC_NOK		Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION 
*Construction et emission du message XDM_IAX_SENDPOKE dans le datagroup
* XDG_DEMANDE_SIMU
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "@(#)xzit.c	1.19 12/07/10 : XZIT21_Simulation_En_Cours" ;


XZSCT_Datagroup		vl_DG;
char			*pl_str;

/*A Controle des variables en entree */


/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IPA_PRET, va_PO);

XZST_03EcritureTrace(XZSTC_INTERFACE, "datagroup %s", vl_DG);	


/*A Construction et envoi du message XDM_IAX_SENDPOKE*/

				
if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IAX_SENDPOKE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_DEJA_SIMU, 		/* numero du poke associe a la boite de dialogue */
       	NULL))				
				

{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IAX_SENDPOKE");
  return(XDC_NOK);
}
TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return(XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIT22_Fax_Auto(XDY_Entier NumPlan, XDY_Entier NumOrdre, XDY_Entier NumFmc, XDY_Octet CleFmc, char* LibelleFmc, XDY_Entier Auto, XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO			Identifiant du poste opirateur concerne
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR :
* XDC_OK		Bonne execution de la primitive
* XDC_NOK		Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION
*Construction et emission du message XDM_IAX_SENDPOKE dans le datagroup
* XDG_DEMANDE_SIMU
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "@(#)xzit.c	1.19 12/07/10 : XZIT22_Fax_Auto" ;

XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIT22_Fax_Auto IN");

XZSCT_Datagroup		vl_DG;
char			*pl_str;
char			vl_str[50];



/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IPA_PRET, va_PO);

sprintf(vl_str,"ITMA_TPA|%d|%d|%d|%d|%s|%d", NumPlan, NumOrdre, NumFmc, CleFmc, LibelleFmc, Auto);

XZST_03EcritureTrace(XZSTC_WARNING, "datagroup %s // arguments %s", vl_DG,vl_str);


/*A Construction et envoi du message XDM_IARCHIVAGE*/


if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IARCHIVAGE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_NEWTASK,
	T_IPC_FT_STR,  "ITMA_TFX_FAX_Auto",
	T_IPC_FT_STR,  vl_str,
	T_IPC_FT_INT4, 0,
       	NULL))


{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IARCHIVAGE");
  return(XDC_NOK);
}
TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIT22_Fax_Auto %s OUT", version);
return(XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Pr�vient l'IHM textuelle d'une modification dans la liste des alarmes critiques
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIT22_Modif_Liste_Alarmes_Crit(
          )

/*
* ARGUMENTS EN ENTREE :
*	Aucun 
*
*
* ARGUMENTS EN SORTIE :
*	Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK       Erreur d'ex�cution de la fonction
* XDC_OK        Bonne ex�cution de la fonction
*
* CONDITION D'UTILISATION
*
*	Utilise par la base de donnees des qu'une modification dans la liste des 
*	alarmes apparait. L'alarme est envoy�e � tout le site d'appartenance du
*	serveur de donn�es.
*
* FONCTION 
*  Construction et �mission du message XDM_IMODIF_ALARMES dans le Datagroup
*  XDG_IMODIF_ALARMES_site
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzit.c,v 1.28 2020/12/04 08:38:29 pc2dpdy Exp $ : XZIT22_Modif_Liste_Alarmesi_Crit" ;

XZSCT_Datagroup		vl_DG;
XZSCT_NomSite 		vl_NomSite;


XZST_03EcritureTrace(XZSTC_INTERFACE,"%s IN", version);
/*A recuperation du nom du site */
if (XDC_OK != XZSC_04NomSite(vl_NomSite))
	XZST_03EcritureTrace(XZSTC_WARNING, "impossible de recup�rer le non du site");

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG,XDG_IMODIF_ALARMES_CRIT, vl_NomSite);

/*A Construction et envoi du message XDM_IMODIF_ALARMES */
XZST_03EcritureTrace(XZSTC_INTERFACE, "datagroup %s", vl_DG);

if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IAX_SENDPOKE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,		/* commande d'envoi d'un message a une boite de dialogue */
	T_IPC_FT_INT4, COM_CANAL_MTMT_ALM, 		/* numero du poke associe a la boite de dialogue */
       	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IMODIF_ALARMES");
  return(XDC_NOK);
}

TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"%s OUT", version);
return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Previent GESIHM que le message trafic FM a ete elabore
* et le communique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIT23_Info_Message(char *va_message,XDY_NomMachine va_PO)

/*
* ARGUMENTS EN ENTREE :
*
* va_PO			Identifiant du poste opirateur concerne
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR :
* XDC_OK		Bonne execution de la primitive
* XDC_NOK		Erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
* Cette primitive est appelee par le SAD
*
* FONCTION
*Construction et emission du message XDM_IAX_SENDPOKE dans le datagroup
* XDG_DEMANDE_SIMU
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "@(#)xzit.c	1.19 12/07/10 : XZIT22_Fax_Auto" ;

XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIT23_Info_Message IN %s / %s",va_PO,va_message);

XZSCT_Datagroup		vl_DG;
XDG_EncodeDG2(vl_DG,XDG_IPA_PRET, va_PO);

if (!TipcSrvMsgWrite(vl_DG,
     	XDM_IdentMsg(XDM_IAX_INFOMESSAGE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_INFOMESSAGE,
	T_IPC_FT_STR,  va_message,
       	NULL))


{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IARCHIVAGE");
  return(XDC_NOK);
}
TipcSrvFlush();
XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIT23_Info_Message %s OUT", version);
return(XDC_OK);
}

