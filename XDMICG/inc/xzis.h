
/*E*/
/*Fichier : $Id: xzis.h,v 1.39 2021/05/20 09:05:07 gesconf Exp $      Release : $Revision: 1.39 $        Date : $Date: 2021/05/20 09:05:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE XZIS * FICHIER XZIS.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Il s'agit du module d'interface avec la t¿che IHM
* synoptique. Les composants de ce module permettent de 
* rafra—chir les objets des vues DataViews. Vient en complement 
* des primitives offertes par le sous syst…me EQUEXT qui est 
* charge aussi de 'rafra—chir' la base de donnees.
*
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	29 Aug 1994	: Creation
* NOEL  22 Sep 1994	: declaration des fonctions externes
* NIEPCERON 24 Oct 1994 : ajout des fonctions 01 -> 09  V1.3
* NIEPCERON 24 Oct 1994 : Corrections  V1.4
* NIEPCERON 24 Oct 1994 : Corrections   type des action en XDY_Act V1.5
* NIEPCERON 16 Nov 1994 : ajout des XZIS10 V1.5
* NIEPCERON 22 Nov 1994 : rien V1.9
* NIEPCERON 29 Nov 1994 : Ajout XZIS11 12 13 14   V1.11
* NIEPCERON 06 Dec 1994 : Modif dans  XZIST_Ligne_QVTO  
            12 Jan 1995   Ajout XZIS15               V1.12
* C.T.	    17 Fev 1995 : Ajout XZIS_Etat_Eqt V1.13
*                         et XZIS02_Init_Etat_Eqt en commentaire
* NIEPCERON 17 Fev 1995 : Modif de   XZIS02_Init_Etat_Eqt v1.16
* NIEPCERON 17 Fev 1995 : Modif de   XZIS02_Init_Etat_Eqt v1.17
* NIEPCERON 17 Fev 1995 : Nettoyage des lignes dupliquees par ce CHER sccs  v1.18
* NIEPCERON 23 Mar 1995 : Ajout de  XZIS16_Detruire_Mobiles v1.19
* Guilhou   22 aut 1996 : Ajout de XZIS20_Envoyer_FMC_Locales (RADT) v1.20
* NIEPCERON 28 Aou 1996 : Ajout d'un arg NomSite aux XZISxx_Raf_zzzz v1.21
* NIEPCERON 05 Sep 1996 : Ajout du nb de ligne en arg de XZIS20_Envoyer_FMC_Locales 1.22
* TORREGROSSA 04 Dec 1996 : Modif structure XZIST_FMC_LOC (680 (V1.23)
* NIEPCERON 09 Jan 1997 : AJout d'un num ordre pour l'affectation es voies (dem/1372) v1.24
* NIEPCERON 04 Fev 1997 : Ajout de l'xzis21  pour les donnees tdp (dem/) v 1.25
* Guilhou 05 mai 1997 : ajout de XZIS22 pour strada 1.26
* Guilhou 03 juin 1997 : suppression num ordre (DEM/1372) TEMPORAIRE car pas livree ds release juin 97 1.27
* Guilhou 27 oct 1997 : ajout XZIS23 pour strada 1.28
* Guilhou 14 nov 1997 : remise numero ordre 1.29
* NIEPCERON 19 nov 1998 : Ajout de XZIS24_Raf_PMVDATEX v1.30 dem/1723
* NIEPCERON 10 mar 1999 : Ajout de datex pour fmc locales . Modif interges 1.31 dem/1723
* NIEPCERON 11 mar 2005 : Ajout de XZIS04_Raf_Bouchon_Chaussee_Voie 1.32 DEM444
* Hilmarcher 07 sep 2005: Modif structure XZIST_Ligne_PRBouchons DEM509 v1.33
* JMG	15/09/09: FACTIS DEM914
* JMG	22/11/10 : ajout fonctions  flash
* JMG	14/10/12 : projection flash 
* JMG	29/10/18 : RMVI DEM1315 1.37
* JMG	30/11/18 : ajout AVA 1.38
* PNI	29/03/21 : ajout XZISC_NB_LIG_MESURES200 et mise ‡ 60 de XZISC_NB_LIG_MESURES 1.39
------------------------------------------------------ */

#ifndef XZIS
#define XZIS

/* fichiers inclus */

#include "xdy.h"		/* fichier general de declaration des structures	*/
#include "xdm.h"		/* fichier general de declaration des messages		*/
#include "xdg.h"		/* fichier general de declaration des datagroups	*/
#include "xdc.h"		/* fichier general de declaration des datagroups	*/
#include "xzst.h"               /* fichier general de declaration des outils de trace */

/* definitions de constantes exportees */

#define XZISC_NB_LIG_MESURES 60
#define XZISC_NB_LIG_MESURES200 200

#define XZISC_GRAPHE_TDP_MZ   1
#define XZISC_GRAPHE_F2T      2
#define XZISC_GRAPHE_F2T_REF  3
#define XZISC_GRAPHE_INRETS   4

	/* les param…tres de retour des primitives */

#define	XZISC_ERR_EXE		10	/* Erreur d'execution de la primitive 			*/
#define	XZISC_ERR_PARAMETRES	11	/* Erreur dans un ou plusieurs des param…tres passes 	*/

/* definitions de types exportes */

typedef struct {
	XDY_Mot Valeur;
	XDY_Octet Validite;
} XZIST_LigneDonnees;

typedef struct {
        XDY_Entier 	numero;
        XDY_PR		PRDebut;
        XDY_PR		PRFin;
        } XZIST_Ligne_INRETS_PR;
        
typedef struct {
        XDY_Entier 	numero;
        XDY_Horodate	Horodate;
        XDY_Octet	Niveau;
        } XZIST_Ligne_INRETS_Trafic;
        
typedef struct {
        XDY_Entier	vl_Num_fmc;
	XDY_Horodate 	HorodateDebut;
        XDY_Horodate 	HorodateFin;
        XDY_Entier	PRDebut;
        XDY_Entier	PRFin;
        XDY_Octet	Niveau;
        } XZIST_Ligne_PRBouchons;
        
typedef struct {
        XDY_Horodate	Horodate;
        XDY_Mot		Q;
        XDY_Octet	validite_Q;
        XDY_Octet	V;
        XDY_Octet	validite_V;
        XDY_Octet	TO;
        XDY_Octet	validite_TO;
        } XZIST_Ligne_QVTO;
        
typedef struct {
        XDY_Nom         Nom_Ech;
        XDY_PR		PR;
        XDY_Mot		Retard_H;
        XDY_Mot		Retard_H_M1;
        XDY_Mot		Retard_H_P1;
        XDY_Entier 	Cumul_H;
        XDY_Entier	Cumul_H_M1;
        XDY_Entier	Cumul_H_P1;
        } XZIST_Ligne_Retard;
        
typedef struct {
        XDY_Mot 	numero;
        XDY_Octet	type;
        XDY_Entier	etat;
        } XZIST_Etat_Eqt;

typedef struct {
	XDY_Entier	numero;
	XDY_Octet	cle;
	XDY_Octet	type;
	XDY_Horodate	horodate;
	XDY_Nom		localisation;
	XDY_Horodate	debut;
	XDY_Horodate	fin;
	XDY_Octet	operation;
	XDY_Octet       datex;
	} XZIST_FMC_LOC;


typedef XZIST_LigneDonnees 		XZIST_Tab_LignesDonnees[XZISC_NB_LIG_MESURES];
typedef XZIST_Ligne_INRETS_PR 		XZIST_Tab_Lignes_INRETS_PR[XZISC_NB_LIG_MESURES];
typedef XZIST_Ligne_INRETS_Trafic 	XZIST_Tab_Lignes_INRETS_Trafic[XZISC_NB_LIG_MESURES];

typedef XZIST_Ligne_PRBouchons 		XZIST_Tab_Lignes_PRBouchons[XZISC_NB_LIG_MESURES];
typedef XZIST_Ligne_QVTO 		XZIST_Tab_Lignes_QVTO[XZISC_NB_LIG_MESURES];


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*---------------------------------------------------------
* SERVICE RENDU :
*  Positionner un indicateur dans le synoptique pour indiquer
* que la liste des alarmes a ete modifiee
-----------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIS01_Positionner_Indicateur_Alarme(XDY_Booleen va_rouge
	  );

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR :
*
*       XDY_OK          si execution correcte de la primitive.
*       XZISC_ERR_EXE   si erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
*       A utiliser d…s que la liste des alarmes est modifiee.
*
-----------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Rafra—chir l'etat de disponibilite d'un equipement.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIS02_Raf_Etat_Eqt(XDY_TypeEqt va_TypeEqt, XDY_Eqt va_NumEqt, XDY_Etat_Eqt va_Etat_Eqt);

/*
* ARGUMENTS EN ENTREE :
* XZIS_TypeEqt  Type de l'equipement » rafra—chir
* XZIS_NumEqt   Numero de l'equipement,
* XZIS_Etat_Eqt Etat de l'equipement.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XZISC_ERR_EXE         Erreur dans l'execution de la primitive
* XDY_OK                Execution correcte de la primitive
* XZISC_ERR_PARAMETRES  Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION
* Contr¬le des param…tres en entree.
* Construction du message XDM_Etat_Dispo_Eqt et envoi dans le datagroup
* XDG_Etat_Dispo_Eqt
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Initialiser l'etat de disponibilite d'un equipement.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIS02_Init_Etat_Eqt(XDY_Datagroup va_Datagroup, XZIST_Etat_Eqt *va_Eqt , XDY_Entier va_nb_eqt );


/*
* ARGUMENTS EN ENTREE :
* XDY_Datagroup  Datagroup d'envoie du msg
* XDY_Datagroup  Datagroup d'envoie du msg
* XZIS_Etat_Eqt  Etat des equipements a initialiser
* XDY_Entier va_nb_eqt : nombre d'eqt
* XDY_Entier va_nb_eqt : nombre d'eqt
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDY_OK                Execution correcte de la primitive
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'un bouchon sur une chaussee.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIS03_Raf_Bouchon_Chaussee_Portion(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Sens va_Sens,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau,
                                            XDY_NomSite va_NomSite );

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_Sens : Sens du bouchon : XDC_SENS_NORD, XDC_SENS_SUD
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_niveau : Niveau de Bouchon : XDC_... a definir ?
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IRAF_BOUCHON_CHAUSSEE_PORTION et envoi dans le datagroup
* XDG_IRAF_BOUCHON_CHAUSSEE_PORTION
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'un bouchon sur une voie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIS04_Raf_Bouchon_Chaussee_Voie(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau,
                                            XDY_NomSite va_NomSite );

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_pr : pr du basculement
* va_Sens : Sens du bouchon : XDC_SENS_NORD, XDC_SENS_SUD
* va_voie : Voie du bouchon 
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_niveau : Niveau de Bouchon : XDC_... a definir ?
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IRAF_BOUCHON_CHAUSSEE_VOIE et envoi dans le datagroup
* XDG_IRAF_BOUCHON_CHAUSSEE_PORTION
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'une voie neutralisee sur un segment.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS05_Raf_Voie_Neutralisee_Segment(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr, 
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_NomSite va_NomSite);

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_pr : pr de la voie 
* va_Sens : Sens de la voie neut. : XDC_SENS_NORD, XDC_SENS_SUD
* va_Voie : Sens de la voie neut. 
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IRAF_VOIE_NEUTRALISEE et envoi dans le datagroup
* XDG_IRAF_VOIE_NEUTRALISEE
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'une voie radt inverse.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS06_Raf_Voie_RADT_Inverse       (XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Station va_Idstation,
                                            XDY_Sens    va_Sensradt,
                                            XDY_NomSite va_NomSite);

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_pr : pr de la voie 
* va_Sens : Sens de la voie neut. : XDC_SENS_NORD, XDC_SENS_SUD
* va_Voie : Sens de la voie neut. 
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_Idstation : Numero de station a rattacher a la voie inverse
* va_Sensradt : Sens de la radt a rattacher a la voie inverse
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IRAF_VOIE_RADT_INVERSE et envoi dans le datagroup
* XDG_IRAF_VOIE_RADT_INVERSE
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'un Basculement de Bouchon vur voie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS07_Raf_Basculement_Bouchon_Voie(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc_basc, 
                                            XDY_Evt va_Idfmc_bou, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau,
                                            XDY_NomSite va_NomSite );

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc_basc : Identifiant FMC du basculement
* va_Idfmc_bou : Identifiant FMC du bouchon 
* va_Idportion : Numero de portion
* va_pr : pr du basculement
* va_Sens : Sens du bouchon : XDC_SENS_NORD, XDC_SENS_SUD
* va_voie : Voie du basculement
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_niveau : Niveau de Bouchon : XDC_... a definir ?
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IRAF_BASCULEMENT_BOUCHON_VOIE et envoi dans le datagroup
* 
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'un bouchon sur une chaussee.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS08_Raf_Objet_Mobile_Portion(    XDY_Octet va_TypeObj, 
                                            XDY_Octet va_TypeRaf, 
                                            XDY_Mot va_Idportion,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Evt va_Idfmc, 
                                            XDY_Act va_Idact,
                                            XDY_NomSite va_NomSite );

/*
* ARGUMENTS EN ENTREE :
* va_TypeObj: Type de l'objet : XDC_SYN_...
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idact : Identifiant Fiche action
* va_Idportion : Numero de portion
* va_Sens : Sens du mobile : XDC_SENS_NORD, XDC_SENS_SUD
* va_Voie : Voie du mobile ( si inconnue = XDC_VOIE_INCONNUE )
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IRAF_OBJET_MOBILE_PORTION et envoi dans le datagroup
* XDM_IRAF_OBJET_MOBILE_PORTION
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'un mobile sur un echangeur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIS08_Raf_Objet_Mobile_Echangeur(    XDY_Octet va_TypeObj, 
                                            XDY_Octet va_TypeRaf, 
                                            char *pa_nom_echang,
                                            XDY_Sens va_Sens,
                                            XDY_Octet va_Localisation,
                                            XDY_Evt va_Idfmc, 
                                            XDY_Act va_Idact,
                                            XDY_NomSite va_NomSite );

/*
* ARGUMENTS EN ENTREE :
* va_TypeObj: Type de l'objet : XDC_SYN_...
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idact : Identifiant Fiche action
* pa_nom_echang : nom de l'echangeur
* va_Sens : Sens du mobile : XDC_SENS_....
* va_Localisation : Localisation sur echangeur
* va_NomSite : Nom du Site » rafraichir
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IRAF_OBJET_MOBILE_ECHANG et envoi dans le datagroup
* XDM_IRAF_OBJET_MOBILE_ECHANG
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra—chir la visualisation d'une affectation de voies.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS09_Raf_Objet_Mobile_Port_Aff_Voies(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Booleen va_DebFin,
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Octet  va_Avoie1, XDY_Octet  va_Nvoie1,
                                            XDY_Octet  va_Avoie2, XDY_Octet  va_Nvoie2,
                                            XDY_Octet  va_Avoie3, XDY_Octet  va_Nvoie3,
                                            XDY_Octet  va_Avoie4, XDY_Octet  va_Nvoie4,
                                            XDY_Octet  va_Avoie5, XDY_Octet  va_Nvoie5,
                                            XDY_Octet  va_Avoie6, XDY_Octet  va_Nvoie6,
                                            XDY_Octet  va_Avoie7, XDY_Octet  va_Nvoie7,
                                            XDY_Octet  va_Avoie8, XDY_Octet  va_Nvoie8,
                                            XDY_Octet  va_Avoie9, XDY_Octet  va_Nvoie9,
                                            XDY_Octet  va_Avoie10,XDY_Octet  va_Nvoie10,
					    XDY_Octet  va_numordre,
                                            XDY_NomSite va_NomSite);
/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_DebFin : Booleen d'indication de debut ou de fin du basculement
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_Avoiex : etat de l'ancienne voie x
* va_Nvoiex : etat de la nouvelle voie x
* va_NomSite : Nom du Site » rafraichir
* va_numordre : numero d'ordre
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IRAF_PORT_AFF_VOIES et envoi dans le datagroup
* XDM_IRAF_PORT_AFF_VOIES
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe F de T.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS10_Donnees_FdeT ( XDY_Booleen	va_Existe, 
                                      XDY_Datagroup 	va_Datagroup, 
                                      XDY_Eqt 		va_NumStation,
                                      XDY_Sens		va_Sens,
                                      XDY_Voie		va_Voie,
                                      XDY_Horodate      va_HorodateDebut,
                                      XDY_Octet         va_TypeDonnees, 
                                      XZIST_LigneDonnees *va_Donnees);
/*
* ARGUMENTS EN ENTREE :
* va_existe : Booleen indiquant l'existance ou pas des donnees 
* va_Datagroup : Datagroup d'envoie des donnees
* va_NumStation : Numero de la station de comptage
* va_sens : Sens des donnees de comptage
* va_Voie : Voie des donnees de comptage
* va_HorodateDebut : Horodate du debut des donnees
* va_TypeDonnees : Type de donnees ( Q,TO,V,PL)
* va_Donnees : Tableau de donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IDONNEES_FT et envoi dans le datagroup
* XDM_IDONNEES_FT
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS11_Donnees_INRETS_PR ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
                                      XZIST_Ligne_INRETS_PR *pa_INRETS_PR);
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : numero d'autoroute
* va_PRDebut	
* va_PRFin		
* va_HorodateDebut	: date de debut des mesures multiple de 6mn
* pa_INRETS_PR     : Donnees PR Inrets
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IDONNEES_INRETS et envoi dans le datagroup
* XDM_IDONNEES_INRETS
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des niveaux de trafic.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS12_Donnees_INRETS_TRA ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
                                      XZIST_Ligne_INRETS_Trafic *pa_INRETS_Trafic);
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : numero d'autoroute
* va_PRDebut	
* va_PRFin		
* va_HorodateDebut	: date de debut des mesures multiple de 6mn
* va_DerniereDonnees    : Indique la derniere donnee (XDC_OUI) ou pas (XDC_NON)
* pa_INRETS_Trafic : Donnees niveau de trafic Inrets
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDG_IDONNEES_INRETS et envoi dans le datagroup
* XDM_IDONNEES_INRETS
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des bouchons pour les PR donnes.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS13_Donnees_Bouchons
                                    ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
				      XZIST_Ligne_PRBouchons	*pa_donnees);
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : numero d'autoroute
* va_PRDebut	
* va_PRFin		
* va_HorodateDebut	: date de debut des mesures multiple de 6mn
* va_DerniereDonnees    : Indique la derniere donnee (XDC_OUI) ou pas (XDC_NON)
* va_PRDebutBouchon	: Pr de debut du bouchon
* va_PRFinBouchon	: Pr de fin du bouchon

*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message va_Datagroupe et envoi dans le datagroup
* XDM_IDONNEES_BOUCHON
*
------------------------------------------------------*/





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe Retard sur Itineraire.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIS15_Donnees_Retard
                                    ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
				      XZIST_Ligne_Retard	*pa_donnees);
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_HorodateDebut	: date de debut 
* va_nb_Donnees         : Indique le nb de ligne du tableau de donnees pa_donnees
* pa_donnees    	: Tableau de Donnees

*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message va_Datagroupe et envoi dans le datagroup
* XDM_IDONNEES_QVTO
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande la destruction de tous les mobiles des synoptiques
* du site
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZIS16_Detruire_Mobiles();

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si execution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
*	Construction du message XDM_IRAZ_MOB
* et envoi dans le datagroupe XDG_IRAZ_MOB.
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Envoie la liste des FMC locales au site concerne 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int      XZIS20_Envoyer_FMC_Locales(	XZSCT_NomSite va_site,
						XZIST_FMC_LOC *pa_liste,
						int	      va_taille);

/*
* ARGUMENTS EN ENTREE : 
* va_site : nom du site demande
* va_liste : liste des FMC a envoyer
* va_taille: nb de ligne du tableau
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR :
*
*       XDY_OK          si execution correcte de la primitive.
*       XZISC_ERR_EXE   si erreur d'execution de la primitive
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*       Construction du message XDM_IRAF_FMCLOC
* et envoi dans le datagroupe XDG_IRAF_FMC_LOC.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour un graphe TDP
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS21_Donnees_TDP ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Nom		va_Autoroute,
				      XDY_PR		va_PR_debut,
				      XDY_PR		va_PR_fin,
				      XDY_Sens		va_Sens,
				      XDY_Mot		va_seuil_max,
				      XDY_Mot		va_seuil_bas,
				      XDY_Mot		va_seuil_haut,
                                      XDY_Octet         va_Taille, 
                                      XDY_Mot 		*va_Donnees);

/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : Nom de l'autoroute
* va_PR_debut : PR de debut de la Zone
* va_PR_fin : PR de fin de la Zone
* va_seuil_max : seuil max de la Zone
* va_seuil_bas : seuil bas de la Zone
* va_seuil_haut : seuil haut de la Zone
* va_Taille : Taille du tableau qui suit
* va_Donnees : Tableau de donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IDONNEES_TDP et envoi dans le datagroup
* XDG_IDONNEES_TDP
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees trafic pour une station strada
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int	XZIS22_Donnees_Trafic_Strada ( XDY_Eqt 		va_NumStation,
                                      XDY_Sens		va_Sens,
                                      XDY_Voie		va_Voie,
                                      XDY_Horodate      va_HorodateDebut,
                                      XDY_Mot 		va_Q,
				      XDY_Octet		va_Q_correct,
				      XDY_Mot		va_V,
				      XDY_Octet		va_V_correct,
				      XDY_Mot		va_TO,
				      XDY_Octet		va_TO_correct);
/*
* ARGUMENTS EN ENTREE :
* va_NumStation : Numero de la station de comptage
* va_sens : Sens des donnees de comptage
* va_Voie : Voie des donnees de comptage
* va_HorodateDebut : Horodate du debut des donnees
* va_TypeDonnees : Type de donnees ( Q,TO,V,PL)
* va_Q, va_V, va_TO
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* previent tases qu'une fmc a ete modifiee
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS23_Donnees_Evt_Strada ();
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exEcution de la primitive
* XDC_OK                ExEcution correcte de la primitive
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Rafraichissement de l'affichage PMV sur synoptique intergestionnaire
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS24_Raf_PMVDATEX(char 		*pa_Nom,
                                      char		*pa_texte1,
                                      char		*pa_texte2,
                                      char		*pa_texte3,
                                      char		*pa_texte4);

/*
* ARGUMENTS EN ENTREE :
* pa_Nom : Nom du PMV
* pa_texte1 : texte de la ligne 1
* pa_texte2 : texte de la ligne 2
* pa_texte3 : texte de la ligne 3
* pa_texte4 : texte de la ligne 4
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'execution de la primitive
* XDC_OK		Execution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param…tres en entree.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entree.
* Construction du message XDM_IPMV_DATEX et envoi dans le datagroup
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des equipements sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS25_MASQUE (XDY_Eqt pa_Equip);

/*
* ARGUMENTS EN ENTREE :
* pa_Equip : Equipement
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exÔøΩcution de la primitive
* XDC_OK                ExÔøΩcution correcte de la primitive
* XZISC_ERR_PARAMETRESS Erreur dans les paramÔøΩtres en entrÔøΩe.
*
* CONDITION D'UTILISATION
*
* FONCTION
* ContrÔøΩle des paramÔøΩtres en entrÔøΩe.
* Construction du message XDM_IMSQ et envoi dans le datagroup
*
------------------------------------------------------*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des vues sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS26_VUE (char *pa_Vue);

/*
* ARGUMENTS EN ENTREE :
* pa_Vue : nom de la vue
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exÔøΩcution de la primitive
* XDC_OK                ExÔøΩcution correcte de la primitive
* XZISC_ERR_PARAMETRESS Erreur dans les paramÔøΩtres en entrÔøΩe.
*
* CONDITION D'UTILISATION
*
* FONCTION
* ContrÔøΩle des paramÔøΩtres en entrÔøΩe.
* Construction du message XDM_IVUE et envoi dans le datagroup
*
------------------------------------------------------*/


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des vues sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS27_Graphe (T_INT2 va_TypeGraphe, T_CHAR *va_Donnees);

/*
* ARGUMENTS EN ENTREE :
* va_Donnees : Donnees en entree: 9x Entier puis chaine et en dernier un entier
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exEcution de la primitive
* XDC_OK
* XZISC_ERR_PARAMETRESS Erreur dans les paramItres en entrEe.
*
* CONDITION D'UTILISATION
*
* FONCTION
* ContrBle des paramItres en entrEe.
* Construction du message XDM_ITDP_MZ et envoi dans le datagroup
*
------------------------------------------------------*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des vues sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS30_VUE_PROJETER (char *va_machine,char *pa_Vue);


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des equipements sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS29_MASQUE_PROJETER (char *va_machine, XDY_Eqt pa_Equip, XDY_Eqt pa_etat);



/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* demande liste vehicules a RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern  int     XZIS50_Demande_RMVI (char *va_site, char *va_requete, char *va_autoroute,
                                int va_pr, XDY_Octet va_sens, char *va_type);


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* retour status RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIS51_Status_RMVI(char *va_site, int va_status, char *va_texte);


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* liste vehicules RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIS52_Vehicules_RMVI(char* va_site, T_STR va_requete, T_INT4 va_nb,
                XDY_Phrase ta_id[], XDY_Phrase ta_type[], T_INT4 ta_tdp[],
		                T_INT4 ta_taux[], XDY_Phrase ta_position[], T_INT4 ta_vitesse[],
				                T_INT4 ta_dlp[], T_INT4 ta_note[]);




extern int XZIS53_Connexion_AVA(char *va_site, char *va_operateur, char *va_machine,
			int va_poste,
			int va_mode);

#endif
