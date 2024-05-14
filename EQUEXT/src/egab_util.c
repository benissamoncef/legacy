/*E*/
/*Fichier :  $Id: egab_util.c,v 1.6 2021/07/01 10:09:01 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2021/07/01 10:09:01 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de ajuste de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG		15/05/18 creation 1.1
* JMG		26/10/18 reveil 1.2
* JPL		09/04/19 Ajout indicateur d'�coulement � XZAE11 (DEM 1326)  1.3
* LCL		29/06/21 Ajout reveil XDC_REV_FMC_MOD dans terminerFMC DEM-SAE300 1.4
* LCL		01/07/21 Fix reveil FIN FMC au CI 1.5
* ABE		25/08/21 Ajout ValiderFinFMC pour réveil DEM-SAE300 1.6
* LCL		29/09/21 Correctif perte cause lorsqu'on termine la FMC DEM-SAE300 1.7
* ABE		28/07/22 Ajout libelle GABEB dans le texte reveil pour catch dans ihm Magistra pour afficher un toast DEM-SAE402
------------------------------------------------------*/

/* fichiers inclus */
#include "egab_util.h"
#include "egab_cfg.h"
#include "egab_valide.h"
#include "egab_trace.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzit.h"
#include "xzia.h"
#include "xzdd.h"
#include "xzaac.h"
#include "xzaec.h"
#include "xzae08.h"
#include "xzae11.h"
#include "xzae60.h"
#include "xzae66.h"

#define C_DATE_NULLE	-1
#define C_MAX_REVEILS	5
#define C_MODE_SUD        XDC_SENS_SUD
#define C_MODE_NORD        XDC_SENS_NORD
#define C_LIBELLE_GABEB "GABEB : "

extern XDY_NomSite 			vg_NomSite;
extern T_Liste_CAB	*pg_CAB;
extern XDY_District			vg_NumeroSite;

typedef struct {
        int     Numero;
        XDY_Octet       Cle;
       char    Texte[250];
} T_Reveil;

static T_Reveil vm_reveils[C_MAX_REVEILS];
static int vm_last_reveil;

int validerFMC(XDY_Entier va_numero, XDY_Octet va_type,
		XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_PR va_pr,
		XDY_Octet va_ptcar_in,
		XDY_Entier va_numero_cause, XDY_Entier va_cle_cause,
		XDY_Horodate va_horodate) ;

int validerFinFMC(XDY_Entier va_numero, XDY_Octet va_type,
		XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_PR va_pr,
		XDY_Octet va_ptcar_in,
		XDY_Entier va_numero_cause, XDY_Entier va_cle_cause,
		XDY_Horodate va_horodate) ;

void egab_util_init() {
int vl_indice;
vm_last_reveil = 0;
for (vl_indice=0; vl_indice< C_MAX_REVEILS; vl_indice++) {
  vm_reveils[0].Numero = 0;
}
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'un CAB dans la liste chainee 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_cab(XDY_Eqt va_numero, T_CAB **pa_result)
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_numeroZone 	: numero de la zone
*
* ARGUMENTS EN SORTIE :
*  T_ZONE_REGUL *pa_result  	: renvoie un pointeur vers la zone. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de la ZONE qui a le numero passe en parametre
*
------------------------------------------------------*/
{
T_Liste_CAB *pl_liste;

	XZST_03EcritureTrace( XZSTC_INFO, "IN : rechercher_cab numero=%d", va_numero);

	pl_liste = pg_CAB;

	while(pl_liste != NULL){
		if (pl_liste->cab.config.NumeroCAP == va_numero)
			break;

		pl_liste = pl_liste->suivant;
	}

	if (pl_liste != NULL){
		*pa_result = &(pl_liste->cab);
		XZST_03EcritureTrace( XZSTC_INFO, "OUT : rechercher_cab");
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_cab : Attention aucun CAB correspondant au numero %d n'a ete trouve.", va_numero);
		*pa_result = NULL;
		return XDC_NOK;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "OUT : rechercher_cab");
}


int purgerReveil(int va_numero, int va_cle) {
  int vl_indice;
  XZST_03EcritureTrace( XZSTC_FONCTION, "IN : purgerReveil %d/%d",
  		va_numero, va_cle);
  for (vl_indice=0; vl_indice < C_MAX_REVEILS; vl_indice++) {
    if( (vm_reveils[vl_indice].Numero == va_numero) && (vm_reveils[vl_indice].Cle == va_cle) )
   	{ 
	strcpy(vm_reveils[vl_indice].Texte,"");
	return (XDC_OK);
  	}
  }
  return (XDC_OK);
}
int genererReveil(int va_numero, int va_cle, char *va_texte, int va_type, XDY_Eqt va_ech, XDY_Eqt va_cab) {
  XZITT_Destinataire vl_destinataire;
  XDY_Evt vl_fmc;
  int vl_indice;
  int vl_trouve;
  XDY_Texte vl_surchargeGabeb;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN : genererReveil %d/%d %d : %s",
  		va_numero, va_cle, va_type, va_texte);

  vl_trouve=0;
  /*verifie si le reveil n a pas deja ete emis*/
  for (vl_indice=0; vl_indice < C_MAX_REVEILS; vl_indice++) {
    if( (vm_reveils[vl_indice].Numero == va_numero) && (vm_reveils[vl_indice].Cle == va_cle) )
   	{ 
		if (!strcmp(vm_reveils[vl_indice].Texte, va_texte))
			/*meme texte ne rien faire */
			vl_trouve = 2;
    		else
			vl_trouve = 1;
	break;
  	}
  }

  if ((vl_trouve==0) || (vl_trouve==1)){
	trace_process(va_type,
			va_ech,
			va_numero,va_cle,
			va_cab,
			3 /*C_CONTEXTE_REVEIL*/,
			0,0,
			va_texte);

  	vl_fmc.numero = va_numero;
  	vl_fmc.cle  = va_cle;
	strcpy(vl_destinataire.site,vg_NomSite);

	/* Ajout libelle GABEB dans le texte reveil pour catch dans ihm Magistra pour afficher un toast DEM-SAE402 */
	strcpy(vl_surchargeGabeb, C_LIBELLE_GABEB);

	/* Fix reveil Fin FMC au CI */
	if (!strcmp(va_texte, XDC_REV_FMC_FIN))
	{
		strcat(vl_surchargeGabeb,XDC_REV_FMC_MOD);
		strcpy(vl_destinataire.site,XDC_NOM_SITE_CI);
	  	XZIT08_Modif_Liste_FMC(vl_fmc, vl_destinataire, vl_surchargeGabeb);
	}
	else
	{
		strcat(vl_surchargeGabeb,va_texte);
	  	XZIT08_Modif_Liste_FMC(vl_fmc, vl_destinataire, vl_surchargeGabeb);
		if (vl_trouve==1) /*deja un reveil pour cette FMC mais avec un texte different*/
		{
			strcpy(vm_reveils[vl_indice].Texte, va_texte);
		}
		else { /*on rajoute la FMC dans la liste des reveils*/
			vm_reveils[vm_last_reveil].Numero = va_numero;
			vm_reveils[vm_last_reveil].Cle = va_cle;
			strcpy(vm_reveils[vm_last_reveil].Texte, va_texte);
			vm_last_reveil++;
			if (vm_last_reveil>= C_MAX_REVEILS) vm_last_reveil = 0;
  		}
	}
  }
  else
    XZST_03EcritureTrace( XZSTC_FONCTION, "reveil deja emis, on ne renvoie pas");
  return (XDC_OK);
}


int terminerFMC(T_Bouchon va_bouchon, T_Liste_ECH *pa_ech)
{
  	XDY_Evt vl_evt;
	XDY_Horodate    vl_horodate;
	int vl_jourSemaine;

	/*Recherche de l'horodate courant*/
	if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine,
			(double *) &vl_horodate) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
		return XDC_NOK;
	}

	if (XZAE571_Terminer_FMC(va_bouchon.numeroT, va_bouchon.cleT) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING, "pb appel XZAE571_Terminer_FMC tete %d/%d");
		return (XDC_NOK);
	}
	if (XZAE571_Terminer_FMC(va_bouchon.numeroQ, va_bouchon.cleQ)) {
		XZST_03EcritureTrace(XZSTC_WARNING, "pb appel XZAE571_Terminer_FMC queue %d/%d",
		va_bouchon.numeroQ, va_bouchon.cleQ);
		return (XDC_NOK);
	}

	if (validerFinFMC(va_bouchon.numeroT, XZAEC_FMC_TeteBouchon,
		va_bouchon.autorouteT,va_bouchon.sensT,va_bouchon.prT,
		(va_bouchon.sensT==XDC_SENS_SORTIE_NORD || va_bouchon.sensT == XDC_SENS_SORTIE_SUD) ? va_bouchon.ptCarT : 0,
		va_bouchon.numero_cause, va_bouchon.cle_cause,
		vl_horodate)) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel validerFinFMC tete");
	return (XDC_NOK);
	} 

	if (validerFinFMC(va_bouchon.numeroQ, XZAEC_FMC_QueueBouchon,
		va_bouchon.autorouteQ,va_bouchon.sensQ,va_bouchon.prQ,
		(va_bouchon.sensQ==XDC_SENS_SORTIE_NORD || va_bouchon.sensQ == XDC_SENS_SORTIE_SUD) ? va_bouchon.ptCarQ : 0,
		va_bouchon.numeroT, vg_NumeroSite,
		vl_horodate)) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel validerFinFMC tete");
	return (XDC_NOK);
	} 

  /*generation reveil*/
  genererReveil(va_bouchon.numeroQ, va_bouchon.cleQ, "Mise en fin FMC",EGAB_TRACE_TYPE_APPLICATION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			pa_ech->ech.cab->config.NumeroCAP);
  /* Ajout reveil pour envoyer fin de FMC a tada2 et aux autres POs */
  genererReveil(va_bouchon.numeroQ, va_bouchon.cleQ, XDC_REV_FMC_FIN,EGAB_TRACE_TYPE_APPLICATION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			pa_ech->ech.cab->config.NumeroCAP);
   vl_evt.numero = va_bouchon.numeroQ;
   vl_evt.cle = va_bouchon.cleQ;
   appli_PA(vl_evt,pa_ech);
  return (XDC_OK);
}

int genererAlerte(T_Liste_ECH *pa_ech, T_Bouchon va_bouchon, int va_mode,
                        XDY_Octet va_sous_type_alerte, char *va_texte)
{
        XZST_03EcritureTrace( XZSTC_FONCTION, "IN : genererAlerte ");

        XDY_TexteAlerte vl_TexteAlerte;
        XDY_TexteAlerte vl_TexteAlerteComplet;
        XDY_Horodate    vl_horodate;
        XDY_Entier              vl_jourSemaine;
        T_CAB *pl_cab1;
	T_Liste_Generique *pl_listeGenerique=NULL;
	T_Alerte vl_alerte;
        /*Recherche de l'horodate courant*/
       if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine,
                (double *) &vl_horodate) != XDC_OK) {
         XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
         return XDC_NOK;
       }

	  /* NE PAS GENERER SIL EXISTE DEJA LA MEME ALERTE*/
	  pl_listeGenerique = pa_ech->ech.ListeAlertes;
	  while (pl_listeGenerique != NULL){
	  	if ( !strcmp(((T_Alerte *)pl_listeGenerique->element)->alerte_envoyee,va_texte) ){
			XZST_03EcritureTrace(XZSTC_INFO,"genererAlerte : La meme alerte a deja ete envoyee et non acquitee");
			return XDC_OK;
		}
		Liste_Generique_Suivant(pl_listeGenerique);
	  }

          pl_cab1 = pa_ech->ech.cab;

        if (xzaa01_Creer_Alerte(vl_horodate,
                                          XDC_EQT_CAP,
                                          pl_cab1->config.NumeroCAP,
                                         XZAAC_ALERTE_GABEB,
                                             va_texte,
                      vg_NumeroSite) != XDC_OK){
          XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible de Creer l'Alerte");
          return (XDC_NOK);
        }

	/*trace*/
	trace_process(EGAB_TRACE_TYPE_AJUSTEMENT,
			pa_ech->ech.echangeur.NumeroEchangeur,
			0,0,
			pl_cab1->config.NumeroCAP,
			pa_ech->ech.contexte,
			0,0,
			"Generation alerte %s",va_texte);

	/* Memorisation de l'alerte */
	if ((pl_listeGenerique = (T_Liste_Generique *)malloc(sizeof(T_Liste_Generique))) == NULL){
		XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible d'allouer de la memoire");
		return (XDC_NOK);
	}
	if ((pl_listeGenerique->element = malloc(sizeof(T_Alerte))) == NULL) {
		XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible d'allouer de la memoire");
		return (XDC_NOK);
	}
	vl_alerte.bouchon= va_bouchon;
	strcpy(vl_alerte.alerte_envoyee,va_texte);
	vl_alerte.sous_type_alerte_envoyee = va_sous_type_alerte;
	*((T_Alerte *)pl_listeGenerique->element)= vl_alerte;
	 Liste_Generique_Inserer_En_Tete(pl_listeGenerique, pa_ech->ech.ListeAlertes);

  return (XDC_OK);
}

int creerFMC (XDY_Autoroute va_autorouteQ, XDY_Sens va_sensQ, XDY_PR va_prQ,
		XDY_Autoroute va_autorouteT, XDY_Sens va_sensT, XDY_PR va_prT,
		XDY_Octet va_ptcarQ_in,XDY_Octet va_ptcarT_in,
		XDY_Entier *pa_numero_fmc, XDY_Entier *pa_numero_fmc_tete,
		T_Liste_ECH *pa_ech, int va_num_alerte){
XDY_Octet vl_cle_out;
XDY_Octet vl_po=15;	/*POVC1*/
XDY_Horodate vl_horodate;
XDY_Octet vl_pat = XDC_OUI;
XDY_Entier              vl_jourSemaine;
XDY_Octet vl_autoroute,vl_sens,vl_ptcar,vl_typeptcar,vl_position;
XDY_PR vl_pr;
XDY_Octet vl_vr,vl_vm2,vl_vm1,vl_vl, vl_bau, vl_vri,vl_vm2i,vl_vm1i,vl_vli, vl_baui;
XDY_Octet vl_bauet, vl_baueti,vl_dis, vl_disr;
XDY_Entier vl_num_cause;
int vl_cle_cause;
XDY_Entier vl_fiche_appel_out;
int vl_res;
XDY_Evt vl_evt;
T_CAB *pl_cab1;

	XZST_03EcritureTrace( XZSTC_FONCTION, "creerFMC IN queue %d/%d/%d; tete %d/%d/%d alerte=%d",
			va_autorouteQ, va_sensQ, va_prQ,
			va_autorouteT, va_sensT, va_prT, va_num_alerte);

        /*Recherche de l'horodate courant*/
       if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine,
                (double *) &vl_horodate) != XDC_OK) {
         XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
         return XDC_NOK;
       }

  /*creation queue bouchon*/
  if (XZAE08_Creer_Fiche(XZAEC_FMC_QueueBouchon,
  	vl_po,
	vl_pat,
	vl_horodate,
	(XDY_Horodate)C_DATE_NULLE,
	vg_NumeroSite,
	(XDY_Octet)XDC_FAUX, 
	pa_numero_fmc, &vl_cle_out,
	&vl_fiche_appel_out, va_num_alerte, vg_NumeroSite)!= XDC_OK) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_cree_fmc: pb appel a XZAE08_Creer_Fiche queue");
	*pa_numero_fmc=0;
	*pa_numero_fmc_tete=0;
	return (XDC_NOK);
  }

  /*creation tete  bouchon*/
  if (XZAE08_Creer_Fiche(XZAEC_FMC_TeteBouchon,
  	vl_po,
	vl_pat,
	vl_horodate,
	(XDY_Horodate)C_DATE_NULLE,
	vg_NumeroSite,
	(XDY_Octet)XDC_FAUX,
	pa_numero_fmc_tete, &vl_cle_out,
	&vl_fiche_appel_out, va_num_alerte, vg_NumeroSite)!= XDC_OK) {
	  XZST_03EcritureTrace(   XZSTC_WARNING, "acet_cree_fmc: pb appel a XZAE08_Creer_Fiche tete");
	  *pa_numero_fmc=0;
	  *pa_numero_fmc_tete=0;
	  return (XDC_NOK);
  }

  /*valide la partie bouchon*/
  if (XZAE60_Ecrire_Fiche_Bouchon(*pa_numero_fmc, vl_cle_out,
  		(XDY_Octet)1,vl_horodate, vl_po, XDC_FMC_ETAT_TRAI)  != XDC_OK)    {
	XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel XZAE60_Ecrire_Fiche_Bouchon");
	return (XDC_NOK)  ;
  }
  if (validerFMC(*pa_numero_fmc, XZAEC_FMC_QueueBouchon,
		va_autorouteQ , va_sensQ,va_prQ,
		(va_sensQ==XDC_SENS_SORTIE_NORD || va_sensQ == XDC_SENS_SORTIE_SUD) ? va_ptcarQ_in : 0,
		*pa_numero_fmc_tete, vg_NumeroSite, vl_horodate)) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel validerFMC queue");
	*pa_numero_fmc=0;
       	*pa_numero_fmc_tete=0;
	return (XDC_NOK);
  } 
  if (validerFMC(*pa_numero_fmc_tete, XZAEC_FMC_TeteBouchon,
		va_autorouteT , va_sensT,va_prT,
		(va_sensT==XDC_SENS_SORTIE_NORD || va_sensT == XDC_SENS_SORTIE_SUD) ? va_ptcarT_in : 0,
		0, 0,
		vl_horodate)) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel validerFMC tete");
	*pa_numero_fmc=0;
       	*pa_numero_fmc_tete=0;
	return (XDC_NOK);
  } 

	   genererReveil(*pa_numero_fmc,vl_cle_out,XDC_REV_FMC_CRE,EGAB_TRACE_TYPE_APPLICATION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			pa_ech->ech.cab->config.NumeroCAP);

	pl_cab1 = pa_ech->ech.cab;

	trace_process(EGAB_TRACE_TYPE_AJUSTEMENT,
			va_ptcarT_in,
			*pa_numero_fmc,vg_NumeroSite,
			pl_cab1->config.NumeroCAP,
			pa_ech->ech.contexte,
			0,0,
			"Creatio FMC Bouchon");
   /*calcul PA*/
   vl_evt.numero = *pa_numero_fmc;
   vl_evt.cle = vg_NumeroSite;
   appli_PA(vl_evt,pa_ech);
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT creerFMC queue = %d, tete = %d",
		*pa_numero_fmc, *pa_numero_fmc_tete);   	
  return (XDC_OK);
}


int deplacerFMC(T_Liste_ECH *pa_ech,int va_mode, T_Bouchon va_bouchon,
		XDY_Autoroute va_autorouteT, XDY_Sens va_sensT, XDY_PR va_prT,
		XDY_Autoroute va_autorouteQ, XDY_Sens va_sensQ, XDY_PR va_prQ) {
XZAOT301_Echangeur vl_ech;
XDY_Octet vl_ptcar;
XDY_Horodate vl_horodate;
int vl_jourSemaine;
T_CAB *pl_cab1;
XDY_Evt vl_evt;
char vl_texte[100];


if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine,
                (double *) &vl_horodate) != XDC_OK) {
         XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
         return XDC_NOK;
       }

vl_ech = pa_ech->ech.echangeur;

  vl_ptcar = (va_sensQ==XDC_SENS_SORTIE_NORD || va_sensQ == XDC_SENS_SORTIE_SUD) ? vl_ech.NumeroEchangeur : 0;
  if (validerFMC(va_bouchon.numeroQ, XZAEC_FMC_QueueBouchon,
		va_autorouteQ, va_sensQ ,va_prQ, 
		vl_ptcar,  va_bouchon.numeroT, vg_NumeroSite,
		vl_horodate) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "pb appeldeplacerFMC queue");
    return (XDC_NOK);
  }
  XZST_03EcritureTrace(   XZSTC_FONCTION,"avant validerFMC tete %d",va_bouchon.numeroT);
  vl_ptcar = (va_sensT==XDC_SENS_SORTIE_NORD || va_sensT == XDC_SENS_SORTIE_SUD) ? vl_ech.NumeroEchangeur : 0;
  if (validerFMC(va_bouchon.numeroT,  XZAEC_FMC_TeteBouchon,
		va_autorouteT, va_sensT, va_prT,
		vl_ptcar, va_bouchon.numero_cause, va_bouchon.cle_cause,
		vl_horodate) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "pb appeldeplacerFMC tete");
    return (XDC_NOK);
  }
	pl_cab1 = pa_ech->ech.cab;
	trace_process(EGAB_TRACE_TYPE_AJUSTEMENT,
			pa_ech->ech.echangeur.NumeroEchangeur,
			va_bouchon.numeroQ,va_bouchon.cleQ,
			pl_cab1->config.NumeroCAP,
			pa_ech->ech.contexte,
			0,0,
			"changement localisation FMC");
   sprintf(vl_texte, "Modification FMC auto"); 
   genererReveil(va_bouchon.numeroQ,va_bouchon.cleQ,vl_texte,EGAB_TRACE_TYPE_APPLICATION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			pl_cab1->config.NumeroCAP);
   vl_evt.numero = va_bouchon.numeroQ;
   vl_evt.cle = va_bouchon.cleQ;
   appli_PA(vl_evt,pa_ech);
  return (XDC_OK);
}


int validerFMC(XDY_Entier va_numero, XDY_Octet va_type,
		XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_PR va_pr,
		XDY_Octet va_ptcar_in,
		XDY_Entier va_numero_cause, XDY_Entier va_cle_cause,
		XDY_Horodate va_horodate) {
XDY_Octet vl_autoroute,vl_sens,vl_ptcar,vl_typeptcar,vl_position;
XDY_PR vl_pr;
XDY_Octet vl_vr,vl_vm2,vl_vm1,vl_vl, vl_bau, vl_vri,vl_vm2i,vl_vm1i,vl_vli, vl_baui;
XDY_Octet vl_bauet, vl_baueti,vl_dis, vl_disr;
XDY_Entier vl_num_cause;
int vl_cle_cause;
XDY_Entier vl_fiche_appel_out;
int vl_res;
XDY_Octet vl_po=15;
XDY_Booleen vl_bool;

  /*recuperation de la config des voies queue*/
  XZAE66_Lire_Config_Voies(va_autoroute, va_pr, va_sens , 
	(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0,
        (va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? XDC_POINT_CARACT_ECHANGEUR : 0, 
	0,va_type,
        &vl_autoroute,&vl_pr,&vl_sens,&vl_ptcar,&vl_typeptcar,&vl_position,
        &vl_vr,&vl_vm2,&vl_vm1,&vl_vl, &vl_bau,
        &vl_vri,&vl_vm2i,&vl_vm1i,&vl_vli, &vl_baui,
        &vl_bauet, &vl_baueti,&vl_dis, &vl_disr,&vl_res);

    if (vl_bau != XDC_VOIE_INEXISTANTE)
        vl_bau = XDC_VOIE_BAU_NORMALE;
    if (vl_vl != XDC_VOIE_INEXISTANTE)
        vl_vl = XDC_VOIE_BLOQUEE;
    if (vl_vm1 != XDC_VOIE_INEXISTANTE)
        vl_vm1 = XDC_VOIE_BLOQUEE;
    if (vl_vm2 != XDC_VOIE_INEXISTANTE)
        vl_vm2 = XDC_VOIE_BLOQUEE;
    if (vl_vr != XDC_VOIE_INEXISTANTE)
        vl_vr = XDC_VOIE_BLOQUEE;
    if (vl_baui != XDC_VOIE_INEXISTANTE)
        vl_baui = XDC_VOIE_BAU_NORMALE;
    if (vl_vli != XDC_VOIE_INEXISTANTE)
      vl_vli = XDC_VOIE_SENS_NORMAL;
    if (vl_vm1i != XDC_VOIE_INEXISTANTE)
      vl_vm1i = XDC_VOIE_SENS_NORMAL;
    if (vl_vm2i != XDC_VOIE_INEXISTANTE)
      vl_vm2i = XDC_VOIE_SENS_NORMAL;
    if (vl_vri != XDC_VOIE_INEXISTANTE)
      vl_vri = XDC_VOIE_SENS_NORMAL;
  XZST_03EcritureTrace(   XZSTC_FONCTION,"aut/pr/sens/ptcar %d/%d/%d/%d",
			va_autoroute, va_pr, va_sens, (va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0);
  if (XZAE11_Valider_Fiche_MC(va_numero,
				vg_NumeroSite,
				vl_po,
				XDC_FMC_ETAT_TRAI,
				va_type,
				(XDY_Horodate)C_DATE_NULLE,
				va_horodate,
				(XDY_Horodate)C_DATE_NULLE,
				(XDY_Horodate)C_DATE_NULLE,
				va_horodate,
				va_numero_cause, /* num cause*/
				va_cle_cause, /*cle cause*/
				-1, /*num alerte*/
				"DETECTION BOUCHON", 
				vl_po,
				XZAEC_FMC_CONFIRMEE,
				va_autoroute,
				va_pr,
				va_sens,
				(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0,
				(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? XDC_POINT_CARACT_ECHANGEUR : 0,
				0,
				vl_vr ,
                                        vl_vm2 ,
                                        vl_vm1 ,
                                        vl_vl ,
                                        vl_bau ,
                                        vl_vri ,
                                        vl_vm2i,
                                        vl_vm1i,
                                        vl_vli ,
					vl_baui,
                                        XDC_FAUX,
                                        XDC_FAUX,
				0, /*degats domaine*/
					va_type, /*type anterieur*/
				0, /*fausse alerte*/
				0, /*longueur*/
				"", /*com evt init*/
				0, /*site alerte*/
				0, /*majeur*/
				0 /*ecoulement*/ ) != XDC_OK) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel XZAE11_Valider_Fiche_MC queue");
	return (XDC_NOK);
  }

  XZAE210_Reserver_FMC(va_numero, vg_NumeroSite, vl_po, XDC_FAUX, &vl_bool) ;
  return (XDC_OK);
}

int validerFinFMC(XDY_Entier va_numero, XDY_Octet va_type,
		XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_PR va_pr,
		XDY_Octet va_ptcar_in,
		XDY_Entier va_numero_cause, XDY_Entier va_cle_cause,
		XDY_Horodate va_horodate) {
XDY_Octet vl_autoroute,vl_sens,vl_ptcar,vl_typeptcar,vl_position;
XDY_PR vl_pr;
XDY_Octet vl_vr,vl_vm2,vl_vm1,vl_vl, vl_bau, vl_vri,vl_vm2i,vl_vm1i,vl_vli, vl_baui;
XDY_Octet vl_bauet, vl_baueti,vl_dis, vl_disr;
XDY_Entier vl_num_cause;
int vl_cle_cause;
XDY_Entier vl_fiche_appel_out;
int vl_res;
XDY_Octet vl_po=15;
XDY_Booleen vl_bool;

  /*recuperation de la config des voies queue*/
  XZAE66_Lire_Config_Voies(va_autoroute, va_pr, va_sens , 
	(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0,
        (va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? XDC_POINT_CARACT_ECHANGEUR : 0, 
	0,va_type,
        &vl_autoroute,&vl_pr,&vl_sens,&vl_ptcar,&vl_typeptcar,&vl_position,
        &vl_vr,&vl_vm2,&vl_vm1,&vl_vl, &vl_bau,
        &vl_vri,&vl_vm2i,&vl_vm1i,&vl_vli, &vl_baui,
        &vl_bauet, &vl_baueti,&vl_dis, &vl_disr,&vl_res);

    if (vl_bau != XDC_VOIE_INEXISTANTE)
        vl_bau = XDC_VOIE_BAU_NORMALE;
    if (vl_vl != XDC_VOIE_INEXISTANTE)
        vl_vl = XDC_VOIE_BLOQUEE;
    if (vl_vm1 != XDC_VOIE_INEXISTANTE)
        vl_vm1 = XDC_VOIE_BLOQUEE;
    if (vl_vm2 != XDC_VOIE_INEXISTANTE)
        vl_vm2 = XDC_VOIE_BLOQUEE;
    if (vl_vr != XDC_VOIE_INEXISTANTE)
        vl_vr = XDC_VOIE_BLOQUEE;
    if (vl_baui != XDC_VOIE_INEXISTANTE)
        vl_baui = XDC_VOIE_BAU_NORMALE;
    if (vl_vli != XDC_VOIE_INEXISTANTE)
      vl_vli = XDC_VOIE_SENS_NORMAL;
    if (vl_vm1i != XDC_VOIE_INEXISTANTE)
      vl_vm1i = XDC_VOIE_SENS_NORMAL;
    if (vl_vm2i != XDC_VOIE_INEXISTANTE)
      vl_vm2i = XDC_VOIE_SENS_NORMAL;
    if (vl_vri != XDC_VOIE_INEXISTANTE)
      vl_vri = XDC_VOIE_SENS_NORMAL;
  XZST_03EcritureTrace(   XZSTC_FONCTION,"aut/pr/sens/ptcar %d/%d/%d/%d",
			va_autoroute, va_pr, va_sens, (va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0);
  if (XZAE11_Valider_Fiche_MC(va_numero,
				vg_NumeroSite,
				vl_po,
				XDC_FMC_ETAT_TRAI,
				va_type,
				(XDY_Horodate)C_DATE_NULLE,
				va_horodate,
				(XDY_Horodate)C_DATE_NULLE,
				va_horodate,
				va_horodate,
				va_numero_cause, /* num cause*/
				va_cle_cause, /*cle cause*/
				-1, /*num alerte*/
				"DETECTION BOUCHON", 
				vl_po,
				XZAEC_FMC_CONFIRMEE,
				va_autoroute,
				va_pr,
				va_sens,
				(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? va_ptcar_in : 0,
				(va_sens==XDC_SENS_SORTIE_NORD || va_sens == XDC_SENS_SORTIE_SUD) ? XDC_POINT_CARACT_ECHANGEUR : 0,
				0,
				vl_vr ,
                                        vl_vm2 ,
                                        vl_vm1 ,
                                        vl_vl ,
                                        vl_bau ,
                                        vl_vri ,
                                        vl_vm2i,
                                        vl_vm1i,
                                        vl_vli ,
					vl_baui,
                                        XDC_FAUX,
                                        XDC_FAUX,
				0, /*degats domaine*/
					va_type, /*type anterieur*/
				0, /*fausse alerte*/
				0, /*longueur*/
				"", /*com evt init*/
				0, /*site alerte*/
				0, /*majeur*/
				0 /*ecoulement*/ ) != XDC_OK) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel XZAE11_Valider_Fiche_MC queue");
	return (XDC_NOK);
  }

  XZAE210_Reserver_FMC(va_numero, vg_NumeroSite, vl_po, XDC_FAUX, &vl_bool) ;
  return (XDC_OK);
}


char *egab_NomAuto (int numero) {
	switch (numero) {
	case(1) : return ("A8");break;
	case(2) : return ("A500");break;
	case(3) : return ("A50");break;
	case(4) : return ("A501");break;
	case(5) : return ("A51");break;
	case(6) : return ("A52");break;
	case(7) : return ("A520");break;
	case(8) : return ("A57");break;
	case(11) : return ("A570");break;
	}

	return ("");
}


int egab_NumAuto (char *nom)  {

if (!strcmp(nom, "A8")) return (1);
else if (!strcmp(nom, "A500")) return (2);
else if (!strcmp(nom, "A50")) return (3);
else if (!strcmp(nom, "A501")) return (4);
else if (!strcmp(nom, "A51")) return (5);
else if (!strcmp(nom, "A52")) return (6);
else if (!strcmp(nom, "A520")) return (7);
else if (!strcmp(nom, "A57")) return (8);
else if (!strcmp(nom, "A570")) return (11);
return (1);
}

int egab_echangeur_ferme(int numero, int mode) {
  XDY_Booleen vl_entree1, vl_entree2, vl_sortie1, vl_sortie2;
  XZAE213_Fermeture_Echangeur(numero,  &vl_entree1, &vl_entree2, &vl_sortie1, &vl_sortie2);

  if ((mode==C_MODE_SUD) && (vl_sortie1==XDC_OUI)) {
	XZST_03EcritureTrace( XZSTC_FONCTION,"fermeture echangeur sortie 1 present=> pas de detection forcee");
	return (XDC_OUI);
  }
  if ((mode==C_MODE_NORD) && (vl_sortie2==XDC_OUI)) {
	XZST_03EcritureTrace( XZSTC_FONCTION,"fermeture echangeur sortie 2 present=> pas de detection forcee");
	return (XDC_OUI);
  }
  return (XDC_NON);
}
