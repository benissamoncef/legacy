/*E*/
/*Fichier : $Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $      Release : $Revision: 1.47 $        Date : $Date: 2021/05/20 09:04:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE XZIS * FICHIER xzis.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Il s'agit du module d'interface avec la t�che IHM
* synoptique. Les composants de ce module permettent de 
* rafra�chir les objets des vues DataViews. Vient en compl�ment 
* des primitives offertes par le sous syst�me EQUEXT qui est 
* charg� aussi de 'rafra�chir' la base de donn�es.
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	    29 Aug 1994 : Creation			V1.1
* NOEL	    22 Sep 1994 : titre xzis -> XZIS		V1.2
* NOEL	    27 Sep 1994 : ajout static pour what	V1.3
* NIEPCERON 24 Oct 1994 : ajout des fonctions 01 -> 09  V1.3
* NIEPCERON 26 Oct 1994 : Correction type action en XDY_Act  V1.5
* NIEPCERON 16 Nov1994  : Ajout de XZIS10_  V1.6
* NIEPCERON 16 Nov 1994 : Correction de XZIS10_  V1.7
* NIEPCERON 29 Nov 1994 : Ajout XZIS11 12 13 14   V1.9
* NIEPCERON 29 Nov 1994 : Correction de XZIS11 et XZIS12   V1.10
* NIEPCERON 22 Dec 1994 : Ajout XZIS08 pour echangeur v1.10
* NIEPCERON 12 Jan 1995   Ajout XZIS15               V1.10
* NIEPCERON 18 Jan 1995   Ajout XZIS13               V1.12
* NIEPCERON 18 Jan 1995   Modif XZIS11 et 12         V1.13
* NIEPCERON 17 Fev 1995   Ajout XZIS02_Init*         V1.15
* NIEPCERON 17 Fev 1995   Ajout XZIS02_Init*         V1.15
* NIEPCERON 17 Fev 1995   Modif XZIS02_Init*         V1.16
* NIEPCERON 17 Fev 1995 : Nettoyage des lignes dupliquees par ce CHER sccs  v1.17
* NIEPCERON  9 Mar 1995 : Modif XZIS13 et 12 : plus de test sur pr_debut et fin v1.18
* NIEPCERON 28 Mar 1995 : Modif XZIS08_Raf_Objet_Mobile_Portion: ne provoque plus d'erreur pour un sens inconnu v1.19
* NIEPCERON  9 Mar 1995 : Modif XZIS13 et 12 : plus de test sur pr_debut et fin v1.18
* NIEPCERON  9 Mar 1995 : On ne jete plus le demande objet sur ech. pour localisation nulle v1.20
* NIEPCERON 23 Mar 1995 : Ajout de  XZIS16_Detruire_Mobiles v1.21
* NIEPCERON 21 Sep 1995 : XZIS13: allocation du tableau a envoyer malgr�s un nb de donnees nulle v1.22
* NIEPCERON 25 Oct 1995 : Suppr. des controles sur va_coef_trans ds XZIS09_Raf_Objet_Mobile_Port_Aff_Voies pour le cas des autoroutes inversees v1.23
* NIEPCERON 19 Dec 1995 : Ajout de XZIS17_ReinitEqt v1.24
* GUILHOU   22 Aut 1996 : Ajout de XZIS20_Envoyer_FMC_Locales (RADT) V1.25
* GUILHOU   29 Aou 1996 : Ajout su sites dans les arg des XZISxx_Raf_zzz (RADT) 1.26
* NIEPCERON 05 Sep 1996 : Ajout d'un arg dans XZIS20_Envoyer_FMC_Locales 1.27
* TORREGROSSA 04 Dec 1996 : Modif interface de XZIS20_Envoyer_FMC_Locales (680) V1.28
* NIEPCERON 09 Jan 1997 : AJout d'un num ordre pour l'affectation es voies (dem/1372) v1.29
* NIEPCERON 04 Fev 1997 : Ajout de l'xzis21  pour les donnees tdp (dem/) v 1.30
* Guilhou 05 mai 1997 : ajout XZIS22 pour donnees trafic strada 1.31
* Guilhou 03 juin 1997: suppression num ordre (dem/1372) TEMPORAIRE car pas livree ds SAE V7 1.32
* Guilhou 27 oct 1997 : ajout XZIS23 pour strada 1.33
* Guilhou 14 nov 1997 : remise numero ordre 1.34
* NIEPCERON 19 nov 1998 : Ajout de XZIS24_Raf_PMVDATEX v1.35 dem/1723
* NIEPCERON 11 mar 2005 : Ajout de XZIS04_Raf_Bouchon_Chaussee_Voie 1.37 DEM444
* JMG	22/11/10	: ajout fonctions flash 1.40
* Hilmarcher 07 sep 2005: Modif XZIS13 : ajout param numero FMC DEM509 v1.38 
* JMG	15/09/09	: FACTIS DEM914
* JMG	29/10/18	: RMVI DEM1315 1.43
* JMG	3011/18	: AVA 1.44
* JMG	10/05/20 : EMS 1.45
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/


/* fichiers inclus */
#include <stdio.h>
#include "xzis.h"

/* definitions de constantes */
#define CM_MAX_COEF 10000
#define CM_MIN_COEF 0


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/ 
/*------------------------------------------------------
* SERVICE RENDU : 
*  Positionner un indicateur dans le synoptique pour indiquer 
* que la liste des alarmes a �t� modifi�e
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS01_Positionner_Indicateur_Alarme(XDY_Booleen va_rouge
          )

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si ex�cution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
*	A utiliser d�s que la liste des alarmes est modifi�e.
*
* FONCTION 
*
*	Construction du message XDM_Positionner_Indicateur_Alarme
* et envoi dans le datagroupe XDG_Positionner_Indicateur_Alarme.
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS01_Positionner_Indicateur_Alarme" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS02_Positionner_Indicateur_Alarme: " );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( XZSC_04NomSite(vl_NomSite) ==     XZSCC_INI_INV )
   {
   XZST_03EcritureTrace(   XZSTC_WARNING, "Initialisation du nom du site non effectuee " );
   return XDC_NOK;
   }  
         
XDG_EncodeDG2( vl_Dg , XDG_IPOSIT_INDIC_ALARME , vl_NomSite );
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IPOSIT_INDIC_ALARME ) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2) (va_rouge ? 1 : 0)
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IPOSIT_INDIC_ALARME) );
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS02_Positionner_Indicateur_Alarme: %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir l'�tat de disponibilit� d'un �quipement.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS02_Raf_Etat_Eqt(XDY_TypeEqt va_TypeEqt, XDY_Eqt va_NumEqt, XDY_Etat_Eqt va_Etat_Eqt)

/*
* ARGUMENTS EN ENTREE :
* XZIS_TypeEqt	Type de l'�quipement � rafra�chir
* XZIS_NumEqt	Num�ro de l'�quipement,
* XZIS_Etat_Eqt	Etat de l'�quipement.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XZISC_ERR_EXE		Erreur dans l'ex�cution de la primitive
* XDY_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_Etat_Dispo_Eqt et envoi dans le datagroup
* XDG_Etat_Dispo_Eqt
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS02_Raf_Etat_Eqt" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS02_Raf_Etat_Eqt: va_TypeEqt:%d, va_NumEqt:%d, va_Etat_Eqt:%d", va_TypeEqt , va_NumEqt , va_Etat_Eqt );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( XZSC_04NomSite(vl_NomSite) ==     XZSCC_INI_INV )
   {
   XZST_03EcritureTrace(   XZSTC_WARNING, "Initialisation du nom du site non effectuee " );
   return XDC_NOK;
   }  
         
XDG_EncodeDG2( vl_Dg , XDG_IETAT_DISPO_EQT , vl_NomSite );
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IETAT_DISPO_EQT) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2) va_TypeEqt
			,T_IPC_FT_INT2,(T_INT2) va_NumEqt
			,T_IPC_FT_INT4,(T_INT4) va_Etat_Eqt,
			NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,%d,%d,%d,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IETAT_DISPO_EQT), va_TypeEqt , va_NumEqt , va_Etat_Eqt );
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS02_Raf_Etat_Eqt: %d " , vl_code_retour);
return vl_code_retour;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Initialiser l'�tat de disponibilit� d'un �quipement.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int     XZIS02_Init_Etat_Eqt(XDY_Datagroup va_Datagroup, XZIST_Etat_Eqt *va_Eqt , XDY_Entier va_nb_eqt )


/*
* ARGUMENTS EN ENTREE :
* XDY_Datagroup  Datagroup d'envoie du msg
* XZIS_Etat_Eqt  Etat des equipements a initialiser
* XDY_Entier va_nb_eqt : nombre d'eqt
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDY_OK                Ex�cution correcte de la primitive
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS02_Init_Raf_Etat_Eqt" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;
int i=0;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS02_Init_Raf_Etat_Eqt: va_Datagroup:%s, va_nb_eqt:%d", va_Datagroup , va_nb_eqt );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
for ( i = 0; i < va_nb_eqt; i++ )        
  if (!TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IETAT_DISPO_EQT) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2) va_Eqt[i].type
			,T_IPC_FT_INT2,(T_INT2) va_Eqt[i].numero
			,T_IPC_FT_INT4,(T_INT4) va_Eqt[i].etat,
			NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,%d,%d,%d,NULL)" 
			, va_Datagroup ,XDM_IdentMsg(XDM_IETAT_DISPO_EQT), va_Eqt[i].type , va_Eqt[i].numero , va_Eqt[i].etat );
     vl_code_retour = XDC_NOK;
     }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS02_Init_Raf_Etat_Eqt: %d " , vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'un bouchon sur une chaussee.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS03_Raf_Bouchon_Chaussee_Portion(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Sens va_Sens,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau,
                                            XDY_Nom va_NomSite )

/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_Sens : Sens du bouchon : XDC_SENS_NORD, XDC_SENS_SUD
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_CoefExtension : Coeff d'extension [0,CM_MAX_COEF]
* va_niveau : Niveau de Bouchon : XDC_... a definir ?
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IRAF_BOUCHON_CHAUSSEE_PORTION et envoi dans le datagroup
* XDG_IRAF_BOUCHON_CHAUSSEE_PORTION
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS03_Raf_Bouchon_Chaussee_Portion" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN : XZIS03_Raf_Bouchon_Chaussee_Portion: va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idportion:%d,va_Sens:%d,va_CoefTranslation:%d,va_CoefExtension:%d,va_niveau:%d"
 ,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idportion,va_Sens,va_CoefTranslation,va_CoefExtension,va_niveau);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS03_Raf_Bouchon_Chaussee_Portion: XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS03_Raf_Bouchon_Chaussee_Portion: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_CoefExtension < CM_MIN_COEF ) || ( va_CoefExtension > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefExtension %d incorrecte", va_CoefExtension );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS03_Raf_Bouchon_Chaussee_Portion: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

   
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS03_Raf_Bouchon_Chaussee_Portion: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

/*! Tester niveau */

/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION );
   }  
else         
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION , va_NomSite );
   }  
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_BOUCHON_CHAUSSEE_PORTION) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT4,(T_INT4)va_CoefExtension
			,T_IPC_FT_INT2,(T_INT2)va_niveau
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS03_Raf_Bouchon_Chaussee_Portion:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IETAT_DISPO_EQT));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS03_Raf_Bouchon_Chaussee_Portion: %d " , vl_code_retour);
return vl_code_retour;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'un bouchon sur une voie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS04_Raf_Bouchon_Chaussee_Voie(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau,
                                            XDY_Nom va_NomSite )

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
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IRAF_BOUCHON_CHAUSSEE_VOIE et envoi dans le datagroup
* XDG_IRAF_BOUCHON_CHAUSSEE_PORTION
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS04_Raf_Bouchon_Chaussee_Voie" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN : XZIS04_Raf_Bouchon_Chaussee_Voie: va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idportion:%d,va_Sens:%d,va_CoefTranslation:%d,va_CoefExtension:%d,va_niveau:%d"
 ,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idportion,va_Sens,va_CoefTranslation,va_CoefExtension,va_niveau);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS04_Raf_Bouchon_Chaussee_Voie: XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS04_Raf_Bouchon_Chaussee_Voie: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_CoefExtension < CM_MIN_COEF ) || ( va_CoefExtension > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefExtension %d incorrecte", va_CoefExtension );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS04_Raf_Bouchon_Chaussee_Voie: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

   
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS04_Raf_Bouchon_Chaussee_Voie: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

/*! Tester niveau */

/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION );
   }  
else         
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION , va_NomSite );
   }  
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_BOUCHON_CHAUSSEE_VOIE) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT4,(T_INT4)va_pr
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT4,(T_INT4)va_CoefExtension
			,T_IPC_FT_INT2,(T_INT2)va_niveau
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS04_Raf_Bouchon_Chaussee_Voie:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IETAT_DISPO_EQT));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS04_Raf_Bouchon_Chaussee_Voie: %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'une voie neutralisee sur un segment.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS05_Raf_Voie_Neutralisee_Segment(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr, 
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Nom va_NomSite)

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
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IRAF_VOIE_NEUTRALISEE et envoi dans le datagroup
* XDG_IRAF_VOIE_NEUTRALISEE
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : " ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS05_Raf_Voie_Neutralisee_Segment : va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idportion:%d,va_pr:%d,va_Sens:%d,va_voie:%d,va_CoefTranslation:%d,va_CoefExtension:%d"
 ,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idportion,va_pr,va_Sens,va_Voie,va_CoefTranslation,va_CoefExtension);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS05_Raf_Voie_Neutralisee_Segment: XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS05_Raf_Voie_Neutralisee_Segment: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_CoefExtension < CM_MIN_COEF ) || ( va_CoefExtension > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefExtension %d incorrecte", va_CoefExtension );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS05_Raf_Voie_Neutralisee_Segment: XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

   
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS05_Raf_Voie_Neutralisee_Segment : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

/*if ( ( va_Voie !=  )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Voie %d incorrecte", va_Voie );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS05_Raf_Voie_Neutralisee_Segment: XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
*/
/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_VOIE_NEUTRALISEE );
   }  
else
   {         
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_VOIE_NEUTRALISEE , va_NomSite );
   }
   
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_VOIE_NEUTRALISEE) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT4,(T_INT4)va_pr
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT4,(T_INT4)va_CoefExtension
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS05_Raf_Voie_Neutralisee_Segment:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IETAT_DISPO_EQT));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS05_Raf_Voie_Neutralisee_Segment  %d " , vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'une voie radt inverse.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS06_Raf_Voie_RADT_Inverse       (XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Station va_Idstation,
                                            XDY_Sens    va_Sensradt,
                                            XDY_Nom va_NomSite)

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
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IRAF_VOIE_RADT_INVERSE et envoi dans le datagroup
* XDG_IRAF_VOIE_RADT_INVERSE
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ :XZIS06_Raf_Voie_RADT_Inverse " ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS06_Raf_Voie_RADT_Inverse : va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idportion:%d,va_pr:%d,va_Sens:%d,va_voie:%d,va_CoefTranslation:%d,va_CoefExtension:%d,va_Idstation:%d,va_Sensradt:%d"
 ,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idportion,va_pr,va_Sens,va_Voie,va_CoefTranslation,va_CoefExtension,va_Idstation,va_Sensradt);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS06_Raf_Voie_RADT_Inverse : XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_Sensradt != XDC_SENS_NORD ) && ( va_Sensradt != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sensradt %d incorrecte", va_Sensradt );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS06_Raf_Voie_RADT_Inverse : XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZIS06_Raf_Voie_RADT_Inverse XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_CoefExtension < CM_MIN_COEF ) || ( va_CoefExtension > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefExtension %d incorrecte", va_CoefExtension );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS06_Raf_Voie_RADT_Inverse  XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

   
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS06_Raf_Voie_RADT_Inverse : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }


/*A Recuperation du nom du site */
if ( va_NomSite  ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_VOIE_RADT_INVERSE );
   }  
else         
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_VOIE_RADT_INVERSE , va_NomSite );
   }  
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_VOIE_RADT_INVERSE) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT4,(T_INT4)va_pr
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT4,(T_INT4)va_CoefExtension
			,T_IPC_FT_INT2,(T_INT2)va_Idstation
			,T_IPC_FT_INT2,(T_INT2)va_Sensradt
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS06_Raf_Voie_RADT_Inverse:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_VOIE_RADT_INVERSE));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS06_Raf_Voie_RADT_Inverse : %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'un Basculement de Bouchon vur voie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS07_Raf_Basculement_Bouchon_Voie(XDY_Octet va_TypeRaf, 
                                            XDY_Evt va_Idfmc_basc, 
                                            XDY_Evt va_Idfmc_bou, 
                                            XDY_Mot va_Idportion,
                                            XDY_Entier va_pr,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Entier va_CoefExtension,
                                            XDY_Octet  va_niveau ,
                                            XDY_Nom va_NomSite)

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
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IRAF_BASCULEMENT_BOUCHON_VOIE et envoi dans le datagroup
* 
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS07_Raf_Basculement_Bouchon_Voie" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS07_Raf_Basculement_Bouchon_Voie :  va_TypeRaf:%d,va_Idfmc_basc:%d:%d,va_Idfmc_bou:%d:%d,va_Idportion:%d,va_pr:%d,va_Sens:%d,va_voie:%d,va_CoefTranslation:%d,va_CoefExtension:%d,va_niveau:%d"
 ,va_TypeRaf,va_Idfmc_basc.numero,va_Idfmc_basc.cle,va_Idfmc_bou.numero,va_Idfmc_bou.cle,va_Idportion,va_pr,va_Sens,va_Voie,va_CoefTranslation,va_CoefExtension,va_niveau);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS07_Raf_Basculement_Bouchon_Voie : XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS07_Raf_Basculement_Bouchon_Voie : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_CoefExtension < CM_MIN_COEF ) || ( va_CoefExtension > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefExtension %d incorrecte", va_CoefExtension );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS07_Raf_Basculement_Bouchon_Voie : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

   
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS07_Raf_Basculement_Bouchon_Voie : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

/*! Tester niveau */

/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE  );
   }  
else
   {         
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE , va_NomSite );
   }
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_BASCULEMENT_BOUCHON_VOIE) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc_basc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc_basc.cle
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc_bou.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc_bou.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT4,(T_INT4)va_pr
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT4,(T_INT4)va_CoefExtension
			,T_IPC_FT_INT2,(T_INT2)va_niveau
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS07_Raf_Basculement_Bouchon_Voie:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_BASCULEMENT_BOUCHON_VOIE));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS07_Raf_Basculement_Bouchon_Voie : %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'un bouchon sur une chaussee.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS08_Raf_Objet_Mobile_Portion(    XDY_Octet va_TypeObj, 
                                            XDY_Octet va_TypeRaf, 
                                            XDY_Mot va_Idportion,
                                            XDY_Sens va_Sens,
                                            XDY_Voie va_Voie,
                                            XDY_Entier va_CoefTranslation,
                                            XDY_Evt va_Idfmc, 
                                            XDY_Act va_Idact,
                                            XDY_Nom va_NomSite )

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
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IRAF_OBJET_MOBILE_PORTION et envoi dans le datagroup
* XDM_IRAF_OBJET_MOBILE_PORTION
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ :XZIS08_Raf_Objet_Mobile_Portion " ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS08_Raf_Objet_Mobile_Portion : va_TypeObj:%d,va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idact:%d:%d,va_Idportion:%d,va_Sens:%d,va_Voie:%d,va_CoefTranslation:%d,"
 ,va_TypeObj,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idact.numero,va_Idact.cle,va_Idportion,va_Sens,va_Voie,va_CoefTranslation);
/*A Controle des Parametres*/
if ( ( va_Sens != XDC_SENS_NORD ) && ( va_Sens != XDC_SENS_SUD ) && ( va_Sens != XDC_SENS_INCONNU ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Portion : XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
     
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Portion : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
      
if ( ( va_CoefTranslation < CM_MIN_COEF ) || ( va_CoefTranslation > CM_MAX_COEF ))
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_CoefTranslation %d incorrecte", va_CoefTranslation );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Portion : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }

/*! Tester niveau */

/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_OBJET_MOBILE_PORTION );
   }  
else         
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_OBJET_MOBILE_PORTION , va_NomSite );
   }  

   XZST_03EcritureTrace(XZSTC_WARNING,"DG %s",vl_Dg);
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_OBJET_MOBILE_PORTION) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT4,(T_INT4)va_Idact.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idact.cle
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT2,(T_INT2)va_TypeObj
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS08_Raf_Objet_Mobile_Portion:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_OBJET_MOBILE_PORTION));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Portion : %d " , vl_code_retour);
return vl_code_retour;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'un mobile sur un echangeur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS08_Raf_Objet_Mobile_Echangeur(    XDY_Octet va_TypeObj, 
                                            XDY_Octet va_TypeRaf, 
                                            char *pa_nom_echang,
                                            XDY_Sens va_Sens,
                                            XDY_Octet va_Localisation,
                                            XDY_Evt va_Idfmc, 
                                            XDY_Act va_Idact,
                                            XDY_Nom va_NomSite )

/*
* ARGUMENTS EN ENTREE :
* va_TypeObj: Type de l'objet : XDC_SYN_...
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idact : Identifiant Fiche action
* pa_nom_echang : nom de l'echangeur
* va_Sens : Sens du mobile : XDC_SENS_....
* va_Localisation : Localisation sur echangeur
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IRAF_OBJET_MOBILE_PORTION et envoi dans le datagroup
* XDM_IRAF_OBJET_MOBILE_PORTION
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ :XZIS08_Raf_Objet_Mobile_Echangeur " ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS08_Raf_Objet_Mobile_Echangeur : va_TypeObj:%d,va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idact:%d:%d,pa_nom_echang:%d,va_Sens:%d,"
 ,va_TypeObj,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idact.numero,va_Idact.cle,pa_nom_echang,va_Sens);
/*A Controle des Parametres*/
if ( ( va_Sens == XDC_SENS_NORD ) || ( va_Sens == XDC_SENS_SUD ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_Sens %d incorrecte", va_Sens );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Echangeur : XZISC_ERR_PARAMETRES");
   return XZISC_ERR_PARAMETRES;
   }
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Echangeur : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
      
/*A Recuperation du nom du site */
if ( va_NomSite ==  NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_OBJET_MOBILE_ECHANG );
   }  
else         
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_OBJET_MOBILE_ECHANG , va_NomSite );
   }  
/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_OBJET_MOBILE_ECHANG) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT4,(T_INT4)va_Idact.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idact.cle
			,T_IPC_FT_STR,(T_STR) pa_nom_echang
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT2,(T_INT2)va_TypeObj
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Localisation
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS08_Raf_Objet_Mobile_Echangeur:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_OBJET_MOBILE_ECHANG));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS08_Raf_Objet_Mobile_Echangeur : %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafra�chir la visualisation d'une affectation de voies.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS09_Raf_Objet_Mobile_Port_Aff_Voies(XDY_Octet va_TypeRaf, 
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
                                            XDY_Nom va_NomSite)
/*
* ARGUMENTS EN ENTREE :
* va_TypeRaf: Type de raf : XDC_AJOUT,XDC_MODIF,XDC_SUPPR
* va_Idfmc : Identifiant FMC
* va_Idportion : Numero de portion
* va_DebFin : Booleen d'indication de debut ou de fin du basculement
* va_CoefTranslation : Coeff de translation [0,CM_MAX_COEF]
* va_Avoiex : etat de l'ancienne voie x
* va_Nvoiex : etat de la nouvelle voie x
* va_numordre : numero d'ordre
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* si va_CoefTranslation >= -(CM_MAX_COEF+1) et < CM_MIN_COEF alors on est sur une autoroute inversee (ex A57)
* Construction du message XDG_IRAF_PORT_AFF_VOIES et envoi dans le datagroup
* XDM_IRAF_PORT_AFF_VOIES
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS09_Raf_Objet_Mobile_Port_Aff_Voies " ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
T_INT2 vl_voie[20];

XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS09_Raf_Objet_Mobile_Port_Aff_Voies : va_TypeRaf:%d,va_Idfmc:%d:%d,va_Idportion:%d,va_debfin:%d,va_CoefTranslation:%d"
 ,va_TypeRaf,va_Idfmc.numero,va_Idfmc.cle,va_Idportion,va_DebFin,va_CoefTranslation);
/*A Controle des Parametres*/
   
if ( ( va_TypeRaf != XDC_AJOUT) && ( va_TypeRaf != XDC_MODIF) &&( va_TypeRaf != XDC_SUPPR ) )
   {
   XZST_03EcritureTrace(XZSTC_WARNING,"Parametre va_TypeRaf %d incorrecte", va_TypeRaf );
   XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS09_Raf_Objet_Mobile_Port_Aff_Voies : XZISC_ERR_PARAMETRES ");
   return XZISC_ERR_PARAMETRES;
   }
   
   
/*! Tester niveau */

/*A Recuperation du nom du site */
if ( va_NomSite ==     NULL )
   {
   strcpy( vl_Dg , XDG_IRAF_PORT_AFF_VOIES );
   }  
else        
   {
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_PORT_AFF_VOIES , va_NomSite );
   }  

/*A Construction et envoi du Message */
vl_voie[0]=(T_INT2)  va_Avoie1; vl_voie[1]=(T_INT2)  va_Nvoie1;
vl_voie[2]=(T_INT2)  va_Avoie2; vl_voie[3]=(T_INT2)  va_Nvoie2;
vl_voie[4]=(T_INT2)  va_Avoie3; vl_voie[5]=(T_INT2)  va_Nvoie3;
vl_voie[6]=(T_INT2)  va_Avoie4; vl_voie[7]=(T_INT2)  va_Nvoie4;
vl_voie[8]=(T_INT2)  va_Avoie5; vl_voie[9]=(T_INT2)  va_Nvoie5;
vl_voie[10]=(T_INT2)  va_Avoie6; vl_voie[11]=(T_INT2)  va_Nvoie6;
vl_voie[12]=(T_INT2)  va_Avoie7; vl_voie[13]=(T_INT2)  va_Nvoie7;
vl_voie[14]=(T_INT2)  va_Avoie8; vl_voie[15]=(T_INT2)  va_Nvoie8;
vl_voie[16]=(T_INT2)  va_Avoie9; vl_voie[17]=(T_INT2)  va_Nvoie9;
vl_voie[18]=(T_INT2)  va_Avoie10; vl_voie[19]=(T_INT2)  va_Nvoie10;


if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAF_PORT_AFF_VOIES) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4)va_Idfmc.numero
			,T_IPC_FT_INT2,(T_INT2)va_Idfmc.cle
			,T_IPC_FT_INT2,(T_INT2)va_DebFin
			,T_IPC_FT_INT2,(T_INT2)va_Idportion
			,T_IPC_FT_INT2,(T_INT2)va_TypeRaf
			,T_IPC_FT_INT4,(T_INT4)va_CoefTranslation
			,T_IPC_FT_INT2_ARRAY,vl_voie,20
			,T_IPC_FT_INT2,(T_INT2)va_numordre
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS09_Raf_Objet_Mobile_Port_Aff_Voies:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_PORT_AFF_VOIES));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS09_Raf_Objet_Mobile_Port_Aff_Voies : %d " , vl_code_retour);
return vl_code_retour;
}






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
                                      XZIST_LigneDonnees *va_Donnees)
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
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IDONNEES_FT et envoi dans le datagroup
* XDM_IDONNEES_FT
*
------------------------------------------------------*/

{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS10_Donnees_FdeT " ;
int vl_code_retour=XDC_OK;
T_INT4 vl_valeur[XZISC_NB_LIG_MESURES];
T_INT2 vl_validite[XZISC_NB_LIG_MESURES];
int i=0;


XZST_03EcritureTrace(XZSTC_INTERFACE ,
"IN :XZIS10_Donnees_FdeT : va_existe:%d; va_Datagroup:%s; va_NumStation:%d; va_sens:%d; va_Voie:%d; va_HorodateDebut:%f; va_TypeDonnees:%d; va_Donnees:%d",
va_Existe, va_Datagroup, va_NumStation, va_Sens, va_Voie, va_HorodateDebut, va_TypeDonnees, va_Donnees);

/*A Controle des Parametres*/
if ( (va_Existe != XDC_OUI) && (va_Existe != XDC_NON))
  {
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS10_Donnees_FdeT:Parametre va_Existe : %d invalide", va_Existe);
  return XZISC_ERR_PARAMETRES;
  }
if ( (va_TypeDonnees != XDC_RADT_V ) && (va_TypeDonnees != XDC_RADT_Q ) && (va_TypeDonnees != XDC_RADT_TO ) && (va_TypeDonnees != XDC_RADT_PL ) )
  {
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS10_Donnees_FdeT:Parametre va_TypeDonnees : %d invalide", va_TypeDonnees);
  return XZISC_ERR_PARAMETRES;
  }


/*A decomposition du tableau de donnees */
for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
   {
   vl_valeur[i]=va_Donnees[i].Valeur;
   vl_validite[i]=va_Donnees[i].Validite;
   XZST_03EcritureTrace(XZSTC_DEBUG1 , "ligne %d; valeur %d; Valid %d;",i,va_Donnees[i].Valeur,va_Donnees[i].Validite);
   }
   
/*A Construction et envoi du Message */

if (!TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_FT) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2)va_Existe
			,T_IPC_FT_INT4,(T_INT4)va_NumStation
			,T_IPC_FT_INT2,(T_INT2)va_Sens
			,T_IPC_FT_INT2,(T_INT2)va_Voie
			,T_IPC_FT_REAL8,(T_REAL8)va_HorodateDebut
			,T_IPC_FT_INT2,(T_INT2)va_TypeDonnees
			,T_IPC_FT_INT4_ARRAY,vl_valeur,(T_INT4)XZISC_NB_LIG_MESURES
			,T_IPC_FT_INT2_ARRAY,vl_validite,(T_INT4)XZISC_NB_LIG_MESURES
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS10_Donnees_FdeT:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_FT));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS10_Donnees_FdeT : %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS11_Donnees_INRETS_PR ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
                                      XZIST_Ligne_INRETS_PR *pa_INRETS_PR)
        
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
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IDONNEES_INRETS et envoi dans le datagroup
* XDM_IDONNEES_INRETS
*
------------------------------------------------------*/

{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS11_Donnees_INRETS_PR " ;
int vl_code_retour=XDC_OK;
T_INT4 *pl_numero=NULL;
T_INT4 *pl_prdeb=NULL;
T_INT4 *pl_prfin=NULL;
int i=0;



XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS11_Donnees_INRETS_PR va_Datagroup:%s; va_Autoroute:%d; va_PRDebut:%d; va_PRFin:%d; va_HorodateDebut:%f; va_nb_Donnees:%d",
va_Datagroup,va_Autoroute,va_PRDebut,va_PRFin, va_HorodateDebut, va_nb_Donnees);

/*A Controle des Parametres*/
if ( va_Datagroup[0] == '\000' )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Le datagroup est vide !");
   return XZISC_ERR_PARAMETRES;
   }

/*A Si il y a des donnees on alloue la place memoire */
if ( va_nb_Donnees > 0 )
   {
   if ( ( pl_prdeb = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
      
   if ( ( pl_prfin = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_numero = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   }
   
/*A decomposition du tableau de donnees */
for (i=0; i < va_nb_Donnees ; i++ )
   {
   pl_numero[i] = (T_INT4) pa_INRETS_PR[i].numero;
   pl_prdeb[i] = (T_INT4) pa_INRETS_PR[i].PRDebut;
   pl_prfin[i] = (T_INT4) pa_INRETS_PR[i].PRFin;
   XZST_03EcritureTrace(XZSTC_DEBUG1 , "ligne %d; numero %d; prdeb %d; prfin %d;",i,pl_numero[i],pl_prdeb[i],pl_prfin[i]);
   }
   
/*A Construction et envoi du Message */

if (!TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_INRETS_PR) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2)va_Autoroute
			,T_IPC_FT_INT4,(T_INT4)va_PRDebut
			,T_IPC_FT_INT4,(T_INT4)va_PRFin
			,T_IPC_FT_REAL8,(T_REAL8)va_HorodateDebut
			,T_IPC_FT_INT4_ARRAY,pl_numero,(T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY,pl_prdeb,(T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY,pl_prfin,(T_INT4)va_nb_Donnees
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS11_Donnees_INRETS_PR:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_INRETS_PR));
   vl_code_retour = XDC_NOK;
   }

if ( va_nb_Donnees > 0 )
   {
   free( pl_prdeb );
   free( pl_prfin );
   free( pl_numero );
   }
   
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS11_Donnees_INRETS_PR : %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des niveaux de trafic.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS12_Donnees_INRETS_TRA ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
                                      XZIST_Ligne_INRETS_Trafic *pa_INRETS_Trafic)
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : numero d'autoroute
* va_PRDebut	
* va_PRFin		
* va_HorodateDebut	: date de debut des mesures multiple de 6mn
* va_DerniereDonnees    : Indique la derniere donnee (XDC_OUI) ou pas (XDC_NON)
* va_INRETS_Trafic : Donnees niveau de trafic Inrets
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IDONNEES_INRETS et envoi dans le datagroup
* XDM_IDONNEES_INRETS
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS12_Donnees_INRETS_TRA " ;
T_INT4 *pl_numero=NULL;
T_REAL8 *pl_horodate=NULL;
T_INT2 *pl_niveau=NULL;
int i=0;
int vl_code_retour=XDC_OK;



XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS12_Donnees_INRETS_TRA va_Datagroup:%s; va_Autoroute:%d; va_PRDebut:%d; va_PRFin:%d; va_HorodateDebut:%f;  ",
va_Datagroup,va_Autoroute,va_PRDebut,va_PRFin,va_HorodateDebut);

/*A Controle des Parametres*/

if ( va_Datagroup[0] == '\000' )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Le datagroup est vide !");
   return XZISC_ERR_PARAMETRES;
   }
   
   
/*A Si il y a des donnees on alloue la place memoire */
if ( va_nb_Donnees > 0 )
   {
   if ( ( pl_numero = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }

   if ( ( pl_horodate = (T_REAL8 *) malloc( sizeof(T_REAL8) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }

   if ( ( pl_niveau = (T_INT2 *) malloc( sizeof(T_INT2) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   }
   
/*A decomposition du tableau de donnees */
for (i=0; i < va_nb_Donnees ; i++ )
   {
   pl_numero[i] = (T_INT4) pa_INRETS_Trafic[i].numero;
   pl_horodate[i] = (T_INT4) pa_INRETS_Trafic[i].Horodate;
   pl_niveau[i] = (T_INT4) pa_INRETS_Trafic[i].Niveau;
   XZST_03EcritureTrace(XZSTC_DEBUG1 , "ligne %d; numero %d; horo %f; niveau %d;",i,pl_numero[i],pl_horodate[i],pl_niveau[i]);
   }

/*A Construction et envoi du Message */

if (!TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_INRETS_TRA) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2)va_Autoroute
			,T_IPC_FT_INT4,(T_INT4)va_PRDebut
			,T_IPC_FT_INT4,(T_INT4)va_PRFin
			,T_IPC_FT_REAL8,(T_REAL8)va_HorodateDebut
			,T_IPC_FT_INT4_ARRAY,pl_numero,(T_INT4)va_nb_Donnees
			,T_IPC_FT_REAL8_ARRAY,pl_horodate,(T_INT4)va_nb_Donnees
			,T_IPC_FT_INT2_ARRAY,pl_niveau,(T_INT4)va_nb_Donnees
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS12_Donnees_INRETS_TRA :TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_INRETS_TRA));
   vl_code_retour = XDC_NOK;
   }

if ( va_nb_Donnees > 0 )
   {
   free( pl_horodate );
   free( pl_niveau );
   free( pl_numero );
   }
   
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS12_Donnees_INRETS_TRA : %d " , vl_code_retour);
return vl_code_retour;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe trafic INRETS, Liste des bouchons pour les PR donnes.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS13_Donnees_Bouchons
                                    ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Autoroute	va_Autoroute,
				      XDY_PR		va_PRDebut,
				      XDY_PR		va_PRFin,
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
				      XZIST_Ligne_PRBouchons	*pa_donnees)
        
/*
* ARGUMENTS EN ENTREE :
* va_Datagroup : Datagroup d'envoie des donnees
* va_Autoroute : numero d'autoroute
* va_PRDebut	
* va_PRFin		
* va_HorodateDebut	: date de debut des mesures multiple de 6mn
* va_nb_Donnees         : Indique le nb de ligne du tableau de donnees pa_donnees
* pa_donnees    	: Tableau de Donnees

*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDG_IDONNEES_BOUCHON et envoi dans le datagroup
* XDM_IDONNEES_BOUCHON
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS13_Donnees_Bouchons " ;
int vl_code_retour=XDC_OK;
T_INT4 *pl_numfmc=NULL;
T_REAL8 *pl_horodeb=NULL;
T_REAL8 *pl_horofin=NULL;
T_INT4 *pl_prdeb=NULL;
T_INT4 *pl_prfin=NULL;
T_INT2 *pl_niv=NULL;
int i=0;



XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS13_Donnees_Bouchons va_Datagroup:%s; va_Autoroute:%d; va_PRDebut:%d; va_PRFin:%d; va_HorodateDebut:%f; va_nb_Donnees:%d;",
va_Datagroup,va_Autoroute,va_PRDebut,va_PRFin,va_HorodateDebut,va_nb_Donnees);

/*A Controle des Parametres*/

if ( va_Datagroup[0] == '\000' )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Le datagroup est vide !");
   return XZISC_ERR_PARAMETRES;
   }


/*A Si il y a des donnees on alloue la place memoire */
if ( va_nb_Donnees > 0 )
   {
   if ( ( pl_numfmc = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_horodeb = (T_REAL8 *) malloc( sizeof(T_REAL8) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_horofin = (T_REAL8 *) malloc( sizeof(T_REAL8) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_prdeb = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_prfin = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_niv = (T_INT2 *) malloc( sizeof(T_INT2) *  va_nb_Donnees ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   }
else
   {
   if ( ( pl_numfmc = (T_INT4 *) malloc( sizeof(T_INT4)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_horodeb = (T_REAL8 *) malloc( sizeof(T_REAL8)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_horofin = (T_REAL8 *) malloc( sizeof(T_REAL8)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_prdeb = (T_INT4 *) malloc( sizeof(T_INT4)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_prfin = (T_INT4 *) malloc( sizeof(T_INT4)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   
   if ( ( pl_niv = (T_INT2 *) malloc( sizeof(T_INT2)  ) ) == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
      return XDC_NOK;
      }
   }
   
   
/*A decomposition du tableau de donnees */
for (i=0; i < va_nb_Donnees ; i++ )
   {
   pl_numfmc[i] = (T_INT4) (pa_donnees[i]).vl_Num_fmc;
   pl_horodeb[i] = (T_REAL8) (pa_donnees[i]).HorodateDebut;
   pl_horofin[i] = (T_REAL8) (pa_donnees[i]).HorodateFin;
   pl_prdeb[i] = (T_INT4) (pa_donnees[i]).PRDebut;
   pl_prfin[i] = (T_INT4) (pa_donnees[i]).PRFin;
   pl_niv[i] = (T_INT2) (pa_donnees[i]).Niveau;
   XZST_03EcritureTrace(XZSTC_WARNING , "ligne %d; numfmc:%d; horodeb %f; horofin : %f; prdeb %d; prfin %d; niv : %d",i,pl_numfmc[i],pl_horodeb[i],pl_horofin[i],pl_prdeb[i],pl_prfin[i],pl_niv[i]);
   }
   
/*A Construction et envoi du Message */

if (!( vl_code_retour = (int) TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_BOUCHONS) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2)va_Autoroute
			,T_IPC_FT_INT4,(T_INT4)va_PRDebut
			,T_IPC_FT_INT4,(T_INT4)va_PRFin
			,T_IPC_FT_REAL8,(T_REAL8)va_HorodateDebut
			,T_IPC_FT_INT4_ARRAY, pl_numfmc , (T_INT4)va_nb_Donnees
			,T_IPC_FT_REAL8_ARRAY, pl_horodeb , (T_INT4)va_nb_Donnees
			,T_IPC_FT_REAL8_ARRAY, pl_horofin , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY, pl_prdeb , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY, pl_prfin , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT2_ARRAY, pl_niv , (T_INT4)va_nb_Donnees
			,NULL) ) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS13_Donnees_Bouchons:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...) -> %i" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_BOUCHONS),vl_code_retour);
   vl_code_retour = XDC_NOK;
   }
   
   free (pl_numfmc );
   free( pl_horodeb );
   free( pl_horofin );
   free( pl_prdeb );
   free( pl_prfin );
   free( pl_niv );
   
   
vl_code_retour = XDC_OK;
   
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS13_Donnees_Bouchons : %d " , vl_code_retour);
return vl_code_retour;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe Retard sur Itineraire.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int		XZIS15_Donnees_Retard
                                    ( XDY_Datagroup 	va_Datagroup, 
				      XDY_Horodate	va_HorodateDebut,
                                      XDY_Entier        va_nb_Donnees,
				      XZIST_Ligne_Retard	*pa_donnees)
        
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
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message va_Datagroupe et envoi dans le datagroup
* XDM_IDONNEES_QVTO
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS15_Donnees_Retard " ;
int vl_code_retour=XDC_OK;

T_REAL8 vl_horo;
#ifndef _TIBCO_EMS
char   **pl_nom;
#else
XDY_Phrase   *pl_nom;
#endif
T_INT4 *pl_pr;
T_INT2 *pl_retH;
T_INT2 *pl_retHM1;
T_INT2 *pl_retHP1;
T_INT4 *pl_cumH;
T_INT4 *pl_cumHM1;
T_INT4 *pl_cumHP1;
int i=0;



XZST_03EcritureTrace(XZSTC_INTERFACE , 
"IN :XZIS15_Donnees_Retard va_Datagroup:%s; va_HorodateDebut:%f; va_nb_Donnees:%d;",
va_Datagroup,va_HorodateDebut,va_nb_Donnees);

/*A Controle des Parametres*/

if ( va_Datagroup[0] == '\000' )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Le datagroup est vide !");
   return XZISC_ERR_PARAMETRES;
   }


#ifndef _TIBCO_EMS
if ( ( pl_nom = (char **) malloc( sizeof(char *) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
#else
if ( ( pl_nom = (XDY_Phrase *) malloc( sizeof(XDY_Phrase) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
#endif
   
if ( ( pl_pr= (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
if ( ( pl_retH = (T_INT2 *) malloc( sizeof(T_INT2) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
if ( ( pl_retHM1 = (T_INT2 *) malloc( sizeof(T_INT2) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
if ( ( pl_retHP1 = (T_INT2 *) malloc( sizeof(T_INT2) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
      
if ( ( pl_cumH = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
if ( ( pl_cumHM1 = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
if ( ( pl_cumHP1 = (T_INT4 *) malloc( sizeof(T_INT4) *  va_nb_Donnees ) ) == NULL )
   {
   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees" );
   return XDC_NOK;
   }
   
   
   
/*A decomposition du tableau de donnees */
for (i=0; i < va_nb_Donnees ; i++ )
   {
#ifndef _TIBCO_EMS
   pl_nom[i] = pa_donnees[i].Nom_Ech;
#else
   strcpy(pl_nom[i] , pa_donnees[i].Nom_Ech);
#endif
   pl_pr[i] = (T_INT4) pa_donnees[i].PR;
   pl_retH[i] = (T_INT2) pa_donnees[i].Retard_H;
   pl_retHM1[i] = (T_INT2) pa_donnees[i].Retard_H_M1;
   pl_retHP1[i] = (T_INT2) pa_donnees[i].Retard_H_P1;
   pl_cumH[i] = (T_INT4) pa_donnees[i].Cumul_H;
   pl_cumHM1[i] = (T_INT4) pa_donnees[i].Cumul_H_M1;
   pl_cumHP1[i] = (T_INT4) pa_donnees[i].Cumul_H_P1;
   
   XZST_03EcritureTrace(XZSTC_DEBUG1 , "ligne %d; nom:%s; pr:%d; retH:%d; retHM1:%d; retHP1:%d; cumH:%d; cumHM1:%d; cumHP1:%d;"
   ,i,pl_nom[i],pl_pr[i],pl_retH[i], pl_retHM1[i],  pl_retHP1[i],  pl_cumH[i],  pl_cumHM1[i],  pl_cumHP1[i] );
   }
   
/*A Construction et envoi du Message */

if (!( vl_code_retour = (int) TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_RETARD) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_REAL8, (T_REAL8) va_HorodateDebut
			,T_IPC_FT_STR_ARRAY, pl_nom , (T_INT4)va_nb_Donnees
#ifndef _TIBCO_EMS
			,XDC_Lon_Phrase
#endif
			,T_IPC_FT_INT4_ARRAY, pl_pr , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT2_ARRAY, pl_retH , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT2_ARRAY, pl_retHM1 , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT2_ARRAY, pl_retHP1 , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY, pl_cumH , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY, pl_cumHM1 , (T_INT4)va_nb_Donnees
			,T_IPC_FT_INT4_ARRAY, pl_cumHP1 , (T_INT4)va_nb_Donnees
			,NULL) ))
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS15_Donnees_Retard:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...) -> %i" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_RETARD), vl_code_retour);
   vl_code_retour = XDC_NOK;
   }
   
   free(pl_nom);
   free(pl_pr);
   free(pl_retH);
   free(pl_retHM1);
   free(pl_retHP1);
   free(pl_cumH);
   free(pl_cumHM1);
   free(pl_cumHP1);
   
   vl_code_retour = XDC_OK;
   
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS15_Donnees_RETARD : %d " , vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande la destruction de tous les mobiles des synoptiques
* du site
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS16_Detruire_Mobiles(
          )

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si ex�cution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'ex�cution de la primitive
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
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS16_Detruire_Mobiles" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS16_Detruire_Mobiles" );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( XZSC_04NomSite(vl_NomSite) ==     XZSCC_INI_INV )
   {
   XZST_03EcritureTrace(   XZSTC_WARNING, "Initialisation du nom du site non effectuee " );
   return XDC_NOK;
   }  
         
XDG_EncodeDG2( vl_Dg , XDG_IRAZ_MOB , vl_NomSite );
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IRAZ_MOB ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IRAZ_MOB) );
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS16_Detruire_Mobiles : %d " , vl_code_retour);
return vl_code_retour;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reinit des equipement sur synoptique: etat+fonctionnement
* du site
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS17_ReinitEqt(
          XZSCT_NomSite va_NomSite)

/*
* ARGUMENTS EN ENTREE : 
*	va_NomSite : Nom du site � reinitialiser ou NULL si tous les sites � reinitialiser
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si ex�cution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
*	Construction du message XDM_IREI_EQT
* et envoi dans le datagroupe XDG_IREI_EQT.
*
------------------------------------------------------*/
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS17_ReinitEqt" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS17_ReinitEqt" );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( va_NomSite != NULL )
   {
   XZST_03EcritureTrace(XZSTC_INFO, "Demande de reinit du site %s",va_NomSite);
   XDG_EncodeDG2( vl_Dg , XDG_IREI_EQT , va_NomSite );
   if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IREI_EQT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IREI_EQT) );
     vl_code_retour = XDC_NOK;
     }
   }
else
   {
   XZST_03EcritureTrace(XZSTC_INFO, "Demande de reinit pour tous les sites");
   XDG_EncodeDG2( vl_Dg , XDG_IREI_EQT , XDC_NOM_SITE_CI );
   if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IREI_EQT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IREI_EQT) );
     vl_code_retour = XDC_NOK;
     }
   XDG_EncodeDG2( vl_Dg , XDG_IREI_EQT , XDC_NOM_SITE_VC );
   if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IREI_EQT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IREI_EQT) );
     vl_code_retour = XDC_NOK;
     }
   XDG_EncodeDG2( vl_Dg , XDG_IREI_EQT , XDC_NOM_SITE_DP );
   if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IREI_EQT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IREI_EQT) );
     vl_code_retour = XDC_NOK;
     }
   XDG_EncodeDG2( vl_Dg , XDG_IREI_EQT , XDC_NOM_SITE_CA );
   if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_IREI_EQT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_IREI_EQT) );
     vl_code_retour = XDC_NOK;
     }
   }
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS17_ReinitEqt : %d " , vl_code_retour);
return vl_code_retour;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* envoi des FMC locales a un site 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS20_Envoyer_FMC_Locales(
          XZSCT_NomSite va_NomSite,
	  XZIST_FMC_LOC *pa_liste,
	  int va_taille)

/*
* ARGUMENTS EN ENTREE : 
*	va_NomSite : Nom du site � reinitialiser ou NULL si tous les sites � reinitialiser
*	va_liste : liste des FMC locales
*	va_taille: nb de lignes
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si ex�cution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
*	Construction du message XDM_IRAF_FMCLOC
* et envoi dans le datagroupe XDG_IRAF_FMCLOC_NomSite.
*
------------------------------------------------------*/
{
static char 		*version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS20_Envoyer_FMC_Locales" ;
XDY_Datagroup	 	vl_Dg;
int 			vl_code_retour=XDC_OK;
int			vl_nb_fmc=0;
T_REAL8			*pl_horodate;
T_REAL8			*pl_debut;
T_REAL8			*pl_fin;
T_INT4			*pl_numero;
T_INT2			*pl_cle;
T_INT2			*pl_type;
T_INT2			*pl_operation;
T_INT2			*pl_datex;
char			**pl_localisation;
int			vl_indice;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS20_Envoyer_FMC_Locales" );


#if 0
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( va_NomSite == NULL ) {
	XZST_03EcritureTrace(XZSTC_WARNING
			,"XZIS20_Envoyer_FMC_Locales: il manque le nom du site");
	XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS20_Envoyer_FMC_Locales : %d " , vl_code_retour);
	return (XDC_NOK);
}

   XZST_03EcritureTrace(XZSTC_INFO, "Demande d'envoi des FMC locales au site %s",va_NomSite);
   XDG_EncodeDG2( vl_Dg , XDG_IRAF_FMCLOC , va_NomSite );

  /*recuperation nombre de donnees*/
  vl_nb_fmc=va_taille;

  /*allocation des structures memoire*/
  if ( vl_nb_fmc > 0 ) {
	if ( ( pl_debut = (T_REAL8 *) malloc( sizeof(T_REAL8) *  vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees debut" );
	    return (XDC_NOK);
	}
	if ( ( pl_fin = (T_REAL8 *) malloc( sizeof(T_REAL8) *  vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees fin" );
	    return (XDC_NOK);
	}
	if ( ( pl_horodate = (T_REAL8 *) malloc( sizeof(T_REAL8) *  vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees horodate" );
	    return (XDC_NOK);
	}
        if ( ( pl_numero = (T_INT4 *) malloc( sizeof(T_INT4) * vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees numero" );
	    return (XDC_NOK);
        }
        if ( ( pl_cle = (T_INT2 *) malloc( sizeof(T_INT2) * vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees cle" );
	    return (XDC_NOK);
        }
        if ( ( pl_type = (T_INT2 *) malloc( sizeof(T_INT2) * vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	    return (XDC_NOK);
        }
	if ( ( pl_localisation = (char **) malloc (sizeof(char *) * vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees localisarion" );
	    return (XDC_NOK);
        }
	if ( ( pl_operation = (T_INT2 *) malloc( sizeof(T_INT2) * vl_nb_fmc ) ) == NULL ) {
	   XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	   return (XDC_NOK);
	}
	if ( ( pl_datex = (T_INT2 *) malloc( sizeof(T_INT2) * vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	    return (XDC_NOK);
        }
  }
  else { /*on alloue quand meme si le nb de fmc est nul*/
	if ( ( pl_debut = (T_REAL8 *) malloc( sizeof(T_REAL8) *  vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees debut" );
	    return (XDC_NOK);
	}
	if ( ( pl_fin = (T_REAL8 *) malloc( sizeof(T_REAL8) *  vl_nb_fmc ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees fin" );
	    return (XDC_NOK);
	}
	if ( ( pl_horodate = (T_REAL8 *) malloc( sizeof(T_REAL8) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees horodate" );
	    return (XDC_NOK);
	}
        if ( ( pl_numero = (T_INT4 *) malloc( sizeof(T_INT4) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees numero" );
	    return (XDC_NOK);
        }
        if ( ( pl_cle = (T_INT2 *) malloc( sizeof(T_INT2) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees cle" );
	    return (XDC_NOK);
        }
        if ( ( pl_type = (T_INT2 *) malloc( sizeof(T_INT2) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	    return (XDC_NOK);
        }
	if ( ( pl_localisation = (char **) malloc (sizeof(char *) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees localisarion" );
	    return (XDC_NOK);
        }
        if ( ( pl_operation = (T_INT2 *) malloc( sizeof(T_INT2) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	    return (XDC_NOK);
        }
        if ( ( pl_datex = (T_INT2 *) malloc( sizeof(T_INT2) ) ) == NULL ) {
	    XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	    return (XDC_NOK);
        }
  }
  /*remplissage du message*/
  for (vl_indice=0; vl_indice < vl_nb_fmc; vl_indice++) {
    pl_debut[vl_indice]=(T_REAL8) (pa_liste[vl_indice]).debut;
    pl_fin[vl_indice]=(T_REAL8) (pa_liste[vl_indice]).fin;
    pl_horodate[vl_indice]=(T_REAL8) (pa_liste[vl_indice]).horodate;
    pl_numero[vl_indice]=(T_INT4) (pa_liste[vl_indice]).numero;
    pl_cle[vl_indice]=(T_INT2) (pa_liste[vl_indice]).cle;
    pl_type[vl_indice]=(T_INT2) (pa_liste[vl_indice]).type;
    pl_localisation[vl_indice]=pa_liste[vl_indice].localisation;
    pl_operation[vl_indice]=(T_INT2)(pa_liste[vl_indice].operation);
    pl_datex[vl_indice]=(T_INT2)(pa_liste[vl_indice].datex);
  }

  /*construction et envoi du message*/
  if (!TipcSrvMsgWrite(	vl_Dg
			,XDM_IdentMsg(XDM_IRAF_FMCLOC )
			,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4_ARRAY, pl_numero, (T_INT4)vl_nb_fmc
			,T_IPC_FT_INT2_ARRAY, pl_cle, (T_INT4)vl_nb_fmc
			,T_IPC_FT_INT2_ARRAY, pl_type, (T_INT4)vl_nb_fmc
			,T_IPC_FT_REAL8_ARRAY, pl_horodate, (T_INT4)vl_nb_fmc
			,T_IPC_FT_STR_ARRAY, pl_localisation, (T_INT4)vl_nb_fmc
			,T_IPC_FT_REAL8_ARRAY, pl_debut, (T_INT4)vl_nb_fmc
			,T_IPC_FT_REAL8_ARRAY, pl_fin, (T_INT4)vl_nb_fmc
			,T_IPC_FT_INT2_ARRAY, pl_operation, (T_INT4)vl_nb_fmc
			,T_IPC_FT_INT2_ARRAY, pl_datex, (T_INT4)vl_nb_fmc
			,NULL)) {
     XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS20_Envoyer_FMC_Locales:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...) "
			,vl_Dg,XDM_IdentMsg(XDM_IRAF_FMCLOC));
     vl_code_retour = XDC_NOK;
  }

  /*liberation structures memoire*/
  free(pl_horodate);
  free(pl_numero);
  free(pl_cle);
  free(pl_type);
  free(pl_localisation);
  free(pl_debut);
  free(pl_fin);
  free(pl_operation);
  free(pl_datex);
XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS20_Envoyer_FMC_Locales : %d " , vl_code_retour);
#endif
  return (vl_code_retour);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de Donnees BD pour une courbe F de T.
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
                                      XDY_Mot *va_Donnees)
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
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_IDONNEES_TDP et envoi dans le datagroup
* XDG_IDONNEES_TDP
*
------------------------------------------------------*/

{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS21_Donnees_TDP " ;
int vl_code_retour=XDC_OK;
T_INT2 *pl_valeur=NULL;
int i=0;


XZST_03EcritureTrace(XZSTC_INTERFACE ,
"IN :XZIS21_Donnees_TDP :  va_Datagroup:%s; ", va_Datagroup);


/*A decomposition du tableau de donnees */
if ( ( pl_valeur = (T_INT2 *) malloc( sizeof(T_INT2) *  va_Taille ) ) == NULL ) {
	XZST_03EcritureTrace(XZSTC_WARNING , "Impossible d'allouer de la memoire pour les donnees type" );
	return (XDC_NOK);
	}
	
for (i=0; i < va_Taille ; i++ )
   {
   pl_valeur[i]=(T_INT2)va_Donnees[i];
   }
   
/*A Construction et envoi du Message */

if (!TipcSrvMsgWrite(   va_Datagroup
                        ,XDM_IdentMsg(XDM_IDONNEES_TDP) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_STR,(T_STR) va_Autoroute
			,T_IPC_FT_INT2,(T_INT2) va_Sens
			,T_IPC_FT_INT4,(T_INT4)va_PR_debut
			,T_IPC_FT_INT4,(T_INT4)va_PR_fin
			,T_IPC_FT_INT2,(T_INT2)va_seuil_max
			,T_IPC_FT_INT2,(T_INT2)va_seuil_bas
			,T_IPC_FT_INT2,(T_INT2)va_seuil_haut
			,T_IPC_FT_INT2_ARRAY,pl_valeur,(T_INT4)va_Taille
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS21_Donnees_TDP:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,va_Datagroup,XDM_IdentMsg(XDM_IDONNEES_TDP));
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS21_Donnees_TDP : %d " , vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de Donnees trafic pour une station strada
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS22_Donnees_Trafic_Strada ( XDY_Eqt          va_NumStation,
                                      XDY_Sens          va_Sens,
                                      XDY_Voie          va_Voie,
                                      XDY_Horodate      va_HorodateDebut,
                                      XDY_Mot 		va_Q,
				      XDY_Octet		va_Q_correct,
				      XDY_Mot		va_V,
				      XDY_Octet		va_V_correct,
				      XDY_Mot		va_TO,
				      XDY_Octet		va_TO_correct)

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
* XDC_NOK               Erreur dans l'ex�cution de la primitive
* XDC_OK                Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/
{
  XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS22_Donnees_Trafic_Strada");

  if (!TipcSrvMsgWrite( XDG_IDONNEES_TRAFIC_STRADA
                        ,XDM_IdentMsg(XDM_IDONNEES_TRAFIC_STRADA) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,(T_INT4) va_NumStation
			,T_IPC_FT_INT2,(T_INT2) va_Sens
			,T_IPC_FT_INT2,(T_INT4)va_Voie
			,T_IPC_FT_REAL8,(T_REAL8)va_HorodateDebut
			,T_IPC_FT_INT4,(T_INT4)va_Q
			,T_IPC_FT_INT2,(T_INT2)va_Q_correct
			,T_IPC_FT_INT4,(T_INT4)va_V
			,T_IPC_FT_INT2,(T_INT2)va_V_correct
			,T_IPC_FT_INT4,(T_INT4)va_TO
			,T_IPC_FT_INT2,(T_INT2)va_TO_correct
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS22_Donnees_Trafic_Strada:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,XDG_IDONNEES_TRAFIC_STRADA,XDM_IdentMsg(XDM_IDONNEES_TRAFIC_STRADA));
   return(XDC_NOK);
  }
  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS22_Donnees_Trafic_Strada");
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* previent la tache tases qu'une fmc a ete modifiee 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIS23_Strada(
          )

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* ARGUMENTS EN SORTIE : Aucun
*
*
* CODE RETOUR : 
*
* 	XDY_OK 		si ex�cution correcte de la primitive.
*	XZISC_ERR_EXE 	si erreur d'ex�cution de la primitive
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
{
static char *version = "$Id: xzis.c,v 1.47 2021/05/20 09:04:53 gesconf Exp $ : XZIS23_Strada" ;
int vl_code_retour=XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS23_Strada" );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
if (!TipcSrvMsgWrite(   XDG_AS_SRVEVT_STRADA
                        ,XDM_IdentMsg(XDM_AS_SRVEVT_STRADA ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS23_Strada:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)" 
			,XDG_AS_SRVEVT_STRADA,XDM_IdentMsg(XDM_AS_SRVEVT_STRADA) );
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT :XZIS23_Strada");
return vl_code_retour;
}

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
                                      char		*pa_texte4)

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
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDC_OK		Ex�cution correcte de la primitive
* XZISC_ERR_PARAMETRESS	Erreur dans les param�tres en entr�e.
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Construction du message XDM_IPMV_DATEX et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZIS24_Raf_PMVDATEX");

  if (!TipcSrvMsgWrite( XDG_IPMV_DATEX
                        ,XDM_IdentMsg(XDM_IPMV_DATEX) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_STR,(T_STR) pa_Nom
			,T_IPC_FT_STR,(T_STR) pa_texte1
			,T_IPC_FT_STR,(T_STR) pa_texte2
			,T_IPC_FT_STR,(T_STR) pa_texte3
			,T_IPC_FT_STR,(T_STR) pa_texte4
			,NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ," XZIS24_Raf_PMVDATEX:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
			,XDG_IPMV_DATEX,XDM_IdentMsg(XDM_IPMV_DATEX));
   return(XDC_NOK);
  }
  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS24_Raf_PMVDATEX");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des equipements sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS25_MASQUE (XDY_Eqt pa_Equip)

/*
* ARGUMENTS EN ENTREE :
* pa_Equip : Equipement 
*
* ARGUMENTS EN SORTIE :
*
* 
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exEcution de la primitive
* XDC_OK                ExEcution correcte de la primitive
* XZISC_ERR_PARAMETRESS Erreur dans les paramItres en entrEe.
* 
* CONDITION D'UTILISATION
* 
* FONCTION
* ContrBle des paramItres en entrEe.
* Construction du message XDM_IMSQ et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;
  XZSCT_NomMachine vl_machine; 
   
 XZST_03EcritureTrace(XZSTC_WARNING , "IN : XZIS25_MASQUE");
  XZSC_07NomMachine(vl_machine);
  XDG_EncodeDG2(vl_Dg, XDG_IMASQUE, vl_machine);
  if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IMASQUE)
                       ,XDM_FLG_SRVMSGWRITE
                       ,T_IPC_FT_INT2,(T_INT2) pa_Equip
                       ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
                          ," XZIS25_MASQUE:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                          ,vl_Dg,XDM_IdentMsg(XDM_IMASQUE));
    return(XDC_NOK);
  }
    XZST_03EcritureTrace(XZSTC_WARNING
                          ," XZIS25_MASQUE:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                          ,vl_Dg,XDM_IdentMsg(XDM_IMASQUE));
 XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS25_MASQUE");
 return (XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des vues sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS26_VUE (char *pa_Vue)

/*
* ARGUMENTS EN ENTREE :
* pa_Vue : nom de la vue
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
* Construction du message XDM_IVUE et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;
  XZSCT_NomMachine vl_machine;

  XZSC_07NomMachine(vl_machine);
/*patch temporaire pour envoyer sur POLI*/
//strcpy(vl_machine,"POLI");
  XDG_EncodeDG2(vl_Dg, XDG_IVUE, vl_machine);

  if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IVUE)
                       ,XDM_FLG_SRVMSGWRITE
                       ,T_IPC_FT_STR,(T_STR) pa_Vue
                       ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
                  ," XZIS26_VUE:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                  ,vl_Dg,XDM_IdentMsg(XDM_IVUE));
    return(XDC_NOK);
  }
    XZST_03EcritureTrace(XZSTC_WARNING
                  ," XZIS26_VUE:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                  ,vl_Dg,XDM_IdentMsg(XDM_IVUE));
  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS26_VUE");
  return (XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage des graphes sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS27_Graphe (T_INT2 va_TypeGraphe,
                                                                T_CHAR *va_Donnees)

/*
* ARGUMENTS EN ENTREE :
* va_TypeGraphe : Donnees en entree: Type de Graphe
* va_Donnees    : Donnees en entree: donnees relatives au graphe
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
* Construction du message XDM_IGRAPHE et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;
  XZSCT_NomMachine vl_machine;

  XZST_03EcritureTrace(XZSTC_WARNING,"IN : XZIS27_Graphes - chaine = %s", va_Donnees);

  XZSC_07NomMachine(vl_machine);
  XDG_EncodeDG2(vl_Dg, XDG_IGRAPHE, vl_machine);

  if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IGRAPHE)
                       ,XDM_FLG_SRVMSGWRITE
               , T_IPC_FT_INT2, (T_INT2)va_TypeGraphe
                       , T_IPC_FT_STR, (T_STR)va_Donnees
                       ,NULL) )
  {
    return(XDC_NOK);
  }

  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS27_Graphes");
  return (XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage des graphes sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS28_PlanSecours (T_INT2 va_NumPlan)

/*
* ARGUMENTS EN ENTREE :
* va_TypeGraphe : Donnees en entree: Type de Graphe
* va_Donnees    : Donnees en entree: donnees relatives au graphe
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
* Construction du message XDM_IGRAPHE et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;
  XZSCT_NomMachine vl_machine;

  XZST_03EcritureTrace(XZSTC_WARNING,"IN : XZIS28_PlanSecours - numPlan = %d", va_NumPlan);

  XZSC_07NomMachine(vl_machine);
  XDG_EncodeDG2(vl_Dg, XDG_IPLANSECOURS, vl_machine);

  if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IPLANSECOURS)
                       ,XDM_FLG_SRVMSGWRITE
               , T_IPC_FT_INT4, (T_INT4)va_NumPlan
                       ,NULL) )
  {
    return(XDC_NOK);
  }

  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS28_PlanSecours");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des equipements sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern  int     XZIS29_MASQUE_PROJETER (char *va_machine, XDY_Eqt pa_Equip, XDY_Eqt pa_etat)

/*
* ARGUMENTS EN ENTREE :
* pa_Equip : Equipement
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_NOK               Erreur dans l'exEcution de la primitive
* XDC_OK                ExEcution correcte de la primitive
* XZISC_ERR_PARAMETRESS Erreur dans les paramItres en entrEe.
*
* CONDITION D'UTILISATION
*
* FONCTION
* ContrBle des paramItres en entrEe.
* Construction du message XDM_IMSQ et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;
  XZSCT_NomMachine vl_machine;

 XZST_03EcritureTrace(XZSTC_WARNING , "IN : XZIS29_MASQUE_PROJETER");
  XDG_EncodeDG2(vl_Dg, XDG_IMASQUE, va_machine);
 if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IMASQUE_PROJETER)
                       ,XDM_FLG_SRVMSGWRITE
                       ,T_IPC_FT_INT2,(T_INT2) pa_Equip
                       ,T_IPC_FT_INT2,(T_INT2) pa_etat
                       ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
                          ," XZIS29_MASQUE_PROJETER:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                          ,vl_Dg,XDM_IdentMsg(XDM_IMASQUE_PROJETER));
    return(XDC_NOK);
  }
   XZST_03EcritureTrace(XZSTC_WARNING
                          ," XZIS29_MASQUE_PROJETER:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                          ,vl_Dg,XDM_IdentMsg(XDM_IMASQUE_PROJETER));
 XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS29_MASQUE_PROJETER");
 return (XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* Affichage/Masquage des vues sur synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern  int     XZIS30_VUE_PROJETER (char *va_machine,char *pa_Vue)

/*
* ARGUMENTS EN ENTREE :
* pa_Vue : nom de la vue
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
* Construction du message XDM_IVUE et envoi dans le datagroup
*
------------------------------------------------------*/
{
  XZSCT_Datagroup vl_Dg;

  XDG_EncodeDG2(vl_Dg, XDG_IVUE, va_machine);

  if (!TipcSrvMsgWrite(vl_Dg
                       ,XDM_IdentMsg(XDM_IVUE)
                       ,XDM_FLG_SRVMSGWRITE
                       ,T_IPC_FT_STR,(T_STR) pa_Vue
                       ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
                  ," XZIS30_VUE_PROJETER:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                  ,vl_Dg,XDM_IdentMsg(XDM_IVUE));
    return(XDC_NOK);
  }
    XZST_03EcritureTrace(XZSTC_WARNING
                  ," XZIS30_VUE_PROJETER:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
                  ,vl_Dg,XDM_IdentMsg(XDM_IVUE));
  XZST_03EcritureTrace(XZSTC_WARNING , "OUT : XZIS30_VUE_PROJETER");
  return (XDC_OK);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* demande liste vehicules a RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern  int     XZIS50_Demande_RMVI (char *va_site, char *va_requete, char *va_autoroute,
				int va_pr, XDY_Octet va_sens, char *va_type) 
{

  XZSCT_Datagroup vl_Dg;

  XDG_EncodeDG2(vl_Dg, XDG_RMVI, va_site);

  if (!TipcSrvMsgWrite(XDG_RMVI
                       ,XDM_IdentMsg(XDM_RMVI_DEMANDE)
                       ,XDM_FLG_SRVMSGWRITE
                       ,T_IPC_FT_STR,(T_STR) va_requete
                       ,T_IPC_FT_STR,(T_STR) va_autoroute
                       ,T_IPC_FT_INT4,(T_INT4) va_pr
                       ,T_IPC_FT_INT2,(T_INT2) va_sens
                       ,T_IPC_FT_STR,(T_STR) va_type
                       ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
                  ," XZIS50_Demande_RMVI:pb emission message sur %s"
                  ,vl_Dg);
    return(XDC_NOK);
  }
    TipcSrvFlush();

    XZST_03EcritureTrace(XZSTC_FONCTION
                  ," XZIS50_Demande_RMVI:emission message sur %s",
                  vl_Dg);

  return (XDC_OK);

}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* retour status RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIS51_Status_RMVI(char* va_site,int va_status, char *va_texte)
{

XZSCT_Datagroup vl_Dg;

  XDG_EncodeDG2 (vl_Dg, XDG_IAX_TEXTE, va_site);
  if (!TipcSrvMsgWrite(vl_Dg
  			 ,XDM_IdentMsg(XDM_RMVI_STATUS)
			 ,XDM_FLG_SRVMSGWRITE
			 ,T_IPC_FT_INT4,(T_INT4) va_status
			 ,T_IPC_FT_STR,(T_STR) va_texte
			 ,NULL) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING
			,"XZIS51_Status_RMVI :pb emission message sur %s"
			,vl_Dg);
		return(XDC_NOK);
	}
	TipcSrvFlush();

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZIS51_Status_RMVI : emission message sur %s (%d/%s)"
			,vl_Dg, va_status, va_texte);
	return (XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
* liste vehicules RMVI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIS52_Vehicules_RMVI(char* va_site, T_STR va_requete, T_INT4 va_nb,
		XDY_Phrase ta_id[],XDY_Phrase ta_type[], T_INT4 ta_tdp[],
		T_INT4 ta_taux[], XDY_Phrase ta_position[], T_INT4 ta_vitesse[],
		T_INT4 ta_dlp[], T_INT4 ta_note[])

{

XZSCT_Datagroup vl_Dg;
int vl_ind;
#ifndef _TIBCO_EMS
char * tl_id[XZISC_NB_LIG_MESURES200];
char * tl_type[XZISC_NB_LIG_MESURES200];
char * tl_position[XZISC_NB_LIG_MESURES200];
#else
XDY_Phrase tl_id[XZISC_NB_LIG_MESURES200];
XDY_Phrase  tl_type[XZISC_NB_LIG_MESURES200];
XDY_Phrase  tl_position[XZISC_NB_LIG_MESURES200];
#endif
T_INT4 tl_tdp[XZISC_NB_LIG_MESURES200];
T_INT4 tl_taux[XZISC_NB_LIG_MESURES200];
T_INT4 tl_vitesse[XZISC_NB_LIG_MESURES200];
T_INT4 tl_dlp[XZISC_NB_LIG_MESURES200];
T_INT4 tl_note[XZISC_NB_LIG_MESURES200];
char vl_str_id[1024];
char vl_str_type[1024];
char vl_str_position[10000];


		XZST_03EcritureTrace(XZSTC_WARNING,"Entree XZIS52_Vehicules_RMVI %d lignes a traiter", va_nb);

		strcpy(vl_str_id,"");
		strcpy(vl_str_type,"");
		strcpy(vl_str_position,"");
for (vl_ind=0;vl_ind<va_nb;vl_ind++) {
		XZST_03EcritureTrace(XZSTC_WARNING,"%s\t%s\t%d\t%d\t%s\t%d\t%d\t%d",
					ta_id[vl_ind],
					ta_type[vl_ind],
					ta_tdp[vl_ind],
					ta_taux[vl_ind],
					ta_position[vl_ind],
					ta_vitesse[vl_ind],
					ta_dlp[vl_ind],
					ta_note[vl_ind]);
#ifndef _TIBCO_EMS
	tl_id[vl_ind] = ta_id[vl_ind];
	tl_type[vl_ind] = ta_type[vl_ind];
	tl_position[vl_ind] = ta_position[vl_ind];
#else
	strcpy (tl_id[vl_ind] , ta_id[vl_ind]);
	strcpy(tl_type[vl_ind] , ta_type[vl_ind]);
	strcpy(tl_position[vl_ind] , ta_position[vl_ind]);
#endif
	if (vl_ind>0)
	{
		sprintf(vl_str_id,"%s|%s",vl_str_id,tl_id[vl_ind]);
		sprintf(vl_str_type,"%s|%s",vl_str_type,tl_type[vl_ind]);
		sprintf(vl_str_position,"%s|%s",vl_str_position,tl_position[vl_ind]);
	}
	else
	{
		sprintf(vl_str_id,"%s",tl_id[vl_ind]);
		sprintf(vl_str_type,"%s",tl_type[vl_ind]);
		sprintf(vl_str_position,"%s",tl_position[vl_ind]);
	}
	tl_tdp[vl_ind] = (T_INT4)ta_tdp[vl_ind];
	tl_vitesse[vl_ind] = (T_INT4)ta_vitesse[vl_ind];
	tl_dlp[vl_ind] = (T_INT4)ta_dlp[vl_ind];
	tl_note[vl_ind] = (T_INT4)ta_note[vl_ind];
  }
  XDG_EncodeDG2 (vl_Dg, XDG_IAX_TEXTE, va_site);
  XZST_03EcritureTrace(XZSTC_WARNING,"Site encode %s",vl_Dg);

  if (!TipcSrvMsgWrite(vl_Dg,
  			 XDM_IdentMsg(XDM_RMVI_RETOUR),
			 XDM_FLG_SRVMSGWRITE,
			 T_IPC_FT_STR,(T_STR) va_requete,
			 T_IPC_FT_STR_ARRAY, tl_id, (T_INT4)va_nb,
#ifdef _TIBCO_EMS
			 XDC_Lon_Phrase,
#endif
			 T_IPC_FT_STR_ARRAY, tl_type, (T_INT4)va_nb,
#ifdef _TIBCO_EMS
			 XDC_Lon_Phrase,
#endif
			 T_IPC_FT_INT4_ARRAY, ta_tdp,(T_INT4) va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_taux, (T_INT4)va_nb,
			 T_IPC_FT_STR_ARRAY, tl_position, (T_INT4)va_nb,
#ifdef _TIBCO_EMS
			 XDC_Lon_Phrase,
#endif
			 T_IPC_FT_INT4_ARRAY, tl_vitesse, (T_INT4)va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_dlp, (T_INT4)va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_note, (T_INT4)va_nb,
			 NULL) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING
			,"XZIS52_Vehicules_RMVI :pb emission message sur %s"
			,vl_Dg);
		return(XDC_NOK);
	}
	TipcSrvFlush();

  if (!TipcSrvMsgWrite(vl_Dg,
  			 XDM_IdentMsg(XDM_RMVI_RETOUR_EMS),
			 XDM_FLG_SRVMSGWRITE,
			 T_IPC_FT_STR,(T_STR) va_requete,
			 T_IPC_FT_STR, vl_str_id,
			 T_IPC_FT_STR, vl_str_type,
			 T_IPC_FT_INT4_ARRAY, ta_tdp,(T_INT4) va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_taux, (T_INT4)va_nb,
			 T_IPC_FT_STR, vl_str_position,
			 T_IPC_FT_INT4_ARRAY, tl_vitesse, (T_INT4)va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_dlp, (T_INT4)va_nb,
			 T_IPC_FT_INT4_ARRAY, tl_note, (T_INT4)va_nb,
			 NULL) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING
			,"XZIS52_Vehicules_RMVI :pb emission message EMS sur %s"
			,vl_Dg);
		return(XDC_NOK);
	}
	TipcSrvFlush();

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZIS52_Vehicules_RMVI : emission message sur %s"
			,vl_Dg);
	return (XDC_OK);
}

extern int XZIS53_Connexion_AVA(char *va_site, char *va_operateur, char *va_machine,
                        int va_poste,int va_mode)
{

XZSCT_Datagroup vl_Dg;
	
	if (!TipcSrvMsgWrite(XDG_AVA,
				XDM_IdentMsg(XDM_AVA_CONNEXION),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,(T_STR)va_site,
				T_IPC_FT_STR,(T_STR)va_operateur,
				T_IPC_FT_STR,(T_STR)va_machine,
				T_IPC_FT_INT4,(T_INT4)va_poste,
				T_IPC_FT_INT4,(T_INT4)va_mode,
				NULL) )
        {
	                XZST_03EcritureTrace(XZSTC_WARNING
			                       ,"XZIS53_Connexion_AVA :pb emission message sur %s"
			                        ,XDG_AVA);
			return(XDC_NOK);
        }
        TipcSrvFlush();

        XZST_03EcritureTrace(XZSTC_FONCTION,
                        "XZIS53_Connexion_AVA : emission message sur %s"
			                        ,XDG_AVA);
       return (XDC_OK);
}

