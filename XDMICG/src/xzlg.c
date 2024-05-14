/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZLG * FICHIER XZLG.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG	01/11/08	creation passerelle generique 1.1
* JPL	08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.2
* JPL	08/12/10 : Migration architecture HP ia64 (DEM 961) : retour de C.R. Ok ou NOK ()  1.3
* VR	07/10/11 : Ajout vitesse et vitesse_opposee 1.4 (DEM/1015)
* JMG	14/02/12 : migration linux 1.5
* VR	25/05/12 : regulationDEM/1016 1.6
* JMG	15/12/15: ajout loc RDS dans ZSP (TDP ASF) 1.8 DEM 1151
* JMG	10/12/16 : ajout majeur lot 23 1.9
* JMG	12/12/16 : correction warnings 1.10
* JPL	05/10/17 : Lecture des zones Temps de parcours : xzao4001 remplace xzao400 (DEM 1236)  1.11
* xxx	29/01/19 : Ajout Personnel � XZLG49_GetFmcTravaux   1.12
* JPL	04/04/19 : XZLG40_GetFmc : ajout indicateur d'�coulement � XZAE40_Lire_Fiche_MC (DEM 1326)  1.13
* PNI   28/08/19 : Si evt queue bouchon alors indique fin diffusion t�te �galement      (DEM 1358)  1.14
* JPL	09/09/19 : XZLG49_GetFmcTravaux : ajout bouchon mobile � XZAE49_Lire_Fiche_Travaux (non utilis�) (DEM 1346)  1.15
* ABE	14/09/20 : XZLG49_GetFmcTravaux : Mise ne commentaire d'un log qui fait crash DEM-SAE130 V1.16
* ABE	25/01/20 : XZLG49_GetFmcTravaux suppression & pour passage pointeur vl_vitesse DEM-SAE130 V1.17
* ABE	19/05/21 : Ajout print semcall et semget process fmc_evt ID pour debug en prod DEM-SAE130 V1.21
* PNI	20/05/21 : correctif du test (vl_liste[vl_index].Diffusion == pow(2,XZLGC_NB_CLIENTS) - 1)  DEM-SAE130 V1.23
* JPL	20/02/24 : Traces; traces inutiles XZLG984_Ecrit_Memoire_Partagee_FMC
------------------------------------------------------ */


/* fichiers inclus */

#include "xzlg.h"

#include "xzae185.h"
#include "xzae155.h"
#include "xzae40.h"
#include "xzae42.h"
#include "xzae44.h"
#include "xzae136.h"
#include "xzae555.h"
#include "xzae220.h"
#include "xzae556.h"
#include "xzae641.h"
#include "xzae146.h"
#include "xzao4001.h"
#include "xzas100.h"
#include "xzas88.h"
#include "xzas54.h"
#include "xzas87.h"
#include "xzae565.h"

#define C_l01           "F32"
#define C_l02           "LOC"
#define C_l03           4
#define C_l07_P         "Aix en Provence - Frontiere Italienne"
#define C_l07_N         "Frontiere italienne - Aix en Provence"
#define C_l07_U         "Sens non defini"

static char                     *tm_l07_p[]={
        "",
        "Aix en Provence - Frontiere Italienne",
        "A8 - Monaco",
        "Marseille - Toulon",
        "Marseille - A52",
        "Aix - Les Alpes",
        "Aix - Aubagne",
        "A52 - Auriol",
        "Le Luc - Toulon"};

static char                     *tm_l07_n[]={
        "",
        "Frontiere Italienne - Aix en Provence",
        "MonacoA8 - Monaco",
        "Toulon - Marseille",
        "A52 - Marseille",
        "Les Alpes - Aix",
        "Aubagne - Aix",
        "Auriol - A52",
        "Toulon - Le Luc"};

XZLGT_Liste_Zdp *pg_zdp = NULL;
XZLGT_Liste_Zdp *pg_zdp_courant = NULL;
void *pm_mem_partagee_fmc=NULL;
void *pm_mem_partagee_zdp=NULL;
void *pm_mem_partagee_rdt=NULL;
void *pm_mem_partagee_nb=NULL;
XZLGT_Liste_RADT *pg_radt = NULL;
XZLGT_Liste_RADT *pg_radt_courant = NULL;
XZLGT_Liste_FMC pg_fmc[XZLGC_NB_MAX_FMC];
XZLGT_Liste_FMC *pg_fmc_courant=NULL;
int vm_semid_fmc;
int vm_semid_tdp;
int vm_semid_rdt;

/*int XZLG_ajouter_zdp( );
int XZLG_ajouter_radt();
int XZLG_StockeNiveau();
int XZLG_ajouter_fmc();*/

int XZLG_ajouter_zdp(   int		va_nombre_zdp, 
			int		va_indice_zdp,
			XDY_ZDP         va_zdp,
			char		*va_nom,
                        int             *pa_resultat);

int XZLG_ajouter_fmc(   XDY_Entier      va_indice_fmc,
			XDY_Entier va_nombre_fmc,
			XDY_Entier va_numero,
                        XDY_Octet va_cle,
			XDY_Mot va_type,
			XDY_Horodate va_validation,
			XDY_Octet va_VR,
			XDY_Octet va_VM2,
			XDY_Octet va_VM1,
			XDY_Octet va_VL,
			XDY_Octet va_BAU,
			XDY_Octet va_VR_I,
			XDY_Octet va_VM2_I,
			XDY_Octet va_VM1_I,
			XDY_Octet va_VL_I,
			XDY_Octet va_BAU_I,
			XDY_Octet va_confirme,
			XDY_Autoroute va_autoroute,
			XDY_PR va_pr,
			XDY_Sens va_sens,
			XDY_Octet va_type_pt_car,
			XDY_Octet va_num_pt_car,
			XDY_Entier va_longueur,
			int va_zz,
			XDY_PR va_pr_fin);

int XZLG_ajouter_radt(  XDY_Eqt         va_numero,
			/*XDY_Nom		va_nom,*/
			XDY_Sens	va_sens,
			int             va_nombre_rdt,
			int             va_indice_rdt,
                        int             *pa_resultat);

int XZLG_StockeNiveau(XDY_Mot va_numero_in, XDY_Sens va_sens_in, XDY_Octet va_niveau_in);


static int vm_xzlg204_init_done=FALSE;



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liste des  FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG01_GetFMC(  XDY_Booleen         va_datex_in,
                        XDY_Booleen  va_confirme_in,
		XDY_Booleen va_fini_in,
		XDY_Booleen va_essai_in,
		XDY_Booleen va_exercice_in,
		XDY_Booleen va_flag_web_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Entier vl_cr;

  for (vl_cr=0;vl_cr<XZLGC_NB_MAX_FMC;vl_cr++) 
      pg_fmc[vl_cr].Evt.numero=0;

  if (XZAE185_Recherche_Dernieres_FMC((XDY_FonctionInt)XZLG_ajouter_fmc,&vl_cr) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG01_GetFMC: pb appel a XZAE125_Recherche_Dernieres_FMC");
    return (XDC_NOK);
  }

  return (XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Liste des commentaires valides de la FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG02_GetComment (   XDY_Entier      va_NumEvt_in,
                                        XDY_Octet       va_CleEvt_in,
                                        XDY_FonctionInt va_FonctionUtilisateur_in)


/*
*
* PARAMETRES EN ENTREE :
*
* int           va_NumEvt_in
* tinyint       va_CleEvt_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier    va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque autoroute appel de la fonction utilisateur avec
*
* MECANISMES :
*
------------------------------------------------------*/
{
  if (XZAE155_LireCommentaires(va_NumEvt_in,va_CleEvt_in,va_FonctionUtilisateur_in)!= XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG02_GetComment: pb appel a XZAE155_LireCommentaires");
    return (XDC_NOK);
  }
  return (XDC_OK);
}

/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur 
* ajoute une fmc
*
******************************************************
* SEQUENCE D'APPEL :
* XZLG_ajouter_fmc()
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int XZLG_ajouter_fmc(   XDY_Entier      va_indice_fmc,
			XDY_Entier va_nombre_fmc,
			XDY_Entier va_numero,
                        XDY_Octet va_cle,
			XDY_Mot va_type,
			XDY_Horodate va_validation,
			XDY_Octet va_VR,
			XDY_Octet va_VM2,
			XDY_Octet va_VM1,
			XDY_Octet va_VL,
			XDY_Octet va_BAU,
			XDY_Octet va_VR_I,
			XDY_Octet va_VM2_I,
			XDY_Octet va_VM1_I,
			XDY_Octet va_VL_I,
			XDY_Octet va_BAU_I,
			XDY_Octet va_confirme,
			XDY_Autoroute va_autoroute,
			XDY_PR va_pr,
			XDY_Sens va_sens,
			XDY_Octet va_type_pt_car,
			XDY_Octet va_num_pt_car,
			XDY_Entier va_longueur,
			int va_zz,
			XDY_PR	va_pr_fin)
{
  XZLGT_Nombre_Mem *vl_nb;
  int vl_indice;

  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG_ajouter_fmc: ajout indice %d fmc numero %d/%d/%d",
			va_indice_fmc,va_numero, va_cle, va_pr_fin);

  if (va_indice_fmc==0) {
    vl_nb=XZLG993_Get_Nombre();
    vl_nb->Nombre_FMC=va_nombre_fmc;
    XZLG988_Ecrit_Nombre(*vl_nb);
  }
  if (va_indice_fmc>=XZLGC_NB_MAX_FMC) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG_ajouter_fmc:  nb max de FMC %d atteint",XZLGC_NB_MAX_FMC);
    return (XDC_OK);
  }

    pg_fmc[va_indice_fmc].Evt.numero=va_numero;
    pg_fmc[va_indice_fmc].Evt.cle=va_cle;
    pg_fmc[va_indice_fmc].Diffusion=0;
    pg_fmc[va_indice_fmc].Type=va_type;
    pg_fmc[va_indice_fmc].HValidation=va_validation;
    pg_fmc[va_indice_fmc].VR=va_VR;
    pg_fmc[va_indice_fmc].VM2=va_VM2;
    pg_fmc[va_indice_fmc].VM1=va_VM1;
    pg_fmc[va_indice_fmc].VL=va_VL;
    pg_fmc[va_indice_fmc].BAU=va_BAU;
    pg_fmc[va_indice_fmc].VR_I=va_VR_I;
    pg_fmc[va_indice_fmc].VM2_I=va_VM2_I;
    pg_fmc[va_indice_fmc].VM1_I=va_VM1_I;
    pg_fmc[va_indice_fmc].VL_I=va_VL_I;
    pg_fmc[va_indice_fmc].BAU_I=va_BAU_I;
    pg_fmc[va_indice_fmc].Autoroute=va_autoroute;
    pg_fmc[va_indice_fmc].PR_Debut=va_pr;
    pg_fmc[va_indice_fmc].Sens=va_sens;
    pg_fmc[va_indice_fmc].ZZ=va_zz;
    /*pg_fmc->Suivant=NULL;*/
    pg_fmc[va_indice_fmc].Type_Point_Car=va_type_pt_car;
    pg_fmc[va_indice_fmc].Numero_Point_Car=va_num_pt_car;
    pg_fmc[va_indice_fmc].Longueur=va_longueur;	
    pg_fmc[va_indice_fmc].PR_Fin=va_pr_fin;	

  return (XDC_OK);

}

int XZLG10_Memorise_Diffusion_FMC(XDY_Entier va_numero_in,
		XDY_Octet va_cle_in,
		XDY_Octet va_valeur)
{
  XZLGT_Liste_FMC * vl_liste;
  int vl_indice;

  XZST_03EcritureTrace(XZSTC_INTERFACE,"XZLG10_Memorise_Diffusion_FMC : fmc(%d/%d) diff=%d",
			va_numero_in,va_cle_in,va_valeur);

  /*on recherche la FMC dans la liste des FMC*/
  vl_liste=(XZLGT_Liste_FMC *)pm_mem_partagee_fmc;

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++)
  {
    if (vl_liste[vl_indice].Evt.numero==0)
      break;

    if ( (vl_liste[vl_indice].Evt.numero==va_numero_in) &&
	  (vl_liste[vl_indice].Evt.cle==va_cle_in)) {
      vl_liste[vl_indice].Diffusion=vl_liste[vl_indice].Diffusion|va_valeur;
      XZST_03EcritureTrace(XZSTC_INFO,"XZLG10_Memorise_Diffusion_FMC: diff %d(%d) memorisee pour %d/%d",
				vl_liste[vl_indice].Diffusion,va_valeur,va_numero_in,va_cle_in);
    }
  }
  return (XDC_OK);
}


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques generales d.une FMC 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG40_GetFmc ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Entier   *va_Numero_out,
        XDY_Octet    *va_Cle_out,
        XDY_Octet    *va_Site_out     ,
        XDY_Octet    *va_Type_out,
        XDY_Horodate *va_DebutPrevu_out  ,
        XDY_Horodate *va_Debut_out      ,
        XDY_Horodate *va_FinPrevu_out  ,
        XDY_Horodate *va_Fin_out      ,
        XDY_Horodate *va_Validation_out,
        XDY_Entier   *va_NumCause_out   ,
        XDY_Octet    *va_CleCause_out  ,
        XDY_Mot      *va_TypeCause_out,
        XDY_Entier   *va_NumAlerte_out ,
        XDY_Horodate *va_HoroAlerte_out,
        XDY_Nom      *va_TypeAlerte_out  ,
        XDY_Nom      *va_Origine_out    ,
        XDY_Booleen  *va_Confirme_out,
        XDY_Octet    *va_Autoroute_out ,
        XDY_PR       *va_PR_out            ,
        XDY_Octet    *va_Sens_out    ,
        XDY_Octet    *va_PointCar_out,
        XDY_Nom      *va_NomPtCar_out   ,
        XDY_Octet    *va_VR_out    ,
        XDY_Octet    *va_VM2_out  ,
        XDY_Octet    *va_VM1_out  ,
        XDY_Octet    *va_VL_out  ,
        XDY_Octet    *va_BAU_out,
        XDY_Octet    *va_VR_I_out   ,
        XDY_Octet    *va_VM2_I_out ,
        XDY_Octet    *va_VM1_I_out,
        XDY_Octet    *va_VL_I_out,
        XDY_Octet    *va_BAU_I_out )

/*
* ARGUMENTS EN ENTREE :
*
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
{
        XDY_Octet    vl_AutorouteCause_out ;
        XDY_PR       vl_PRCause_out        ;
        XDY_Octet    vl_SensCause_out;
        XDY_Horodate vl_HoroCause_out ;
        XDY_Octet    vl_Position_out  ;
        XDY_Octet    vl_Degats_out   ;
        XDY_Mot      vl_TypeAnterieur_out;
        XDY_Octet    vl_FausseAlerte_out;
        XDY_Octet    vl_BAU_Etroite_out   ;
        XDY_Octet    vl_BAU_I_Etroite_out ;
        XDY_Entier   vl_Longueur_out;
        XDY_Nom      vl_AbrPtCar_out     ;
        XDY_Octet    vl_NumPtCar_out    ;
        XDY_Octet    vl_Degrade_out    ;
        XDY_Entier        vl_NumEvtInit_out;
        XDY_Octet         vl_CleEvtInit_out;
        XDY_Commentaire   vl_ComEvtInit_out;
        XDY_Octet         vl_TypeEvtInit_out;
        XDY_Octet         vl_AutorouteEvtInit_out;
        XDY_PR            vl_PREvtInit_out;
        XDY_Octet         vl_SensEvtInit_out;
        XDY_Horodate      vl_HoroEvtInit_out;
        XDY_Octet    vl_SiteAlerte_out;
        XDY_Octet    vl_Datex_out    ;
        XDY_Octet    vl_Trafic_out  ;
        XDY_Octet    vl_Gravite_out;
        XDY_Booleen  vl_Bloquant_out;
        XDY_Octet    vl_Duree_out; 
	XDY_Octet	vl_Majeur_out;
	XDY_Octet	vl_Ecoulement_out;

  if (XZAE40_Lire_Fiche_MC((XDY_Horodate)-1,va_numero_in,va_cle_in,va_Numero_out,va_Cle_out,va_Site_out,va_Type_out,
        va_DebutPrevu_out  , va_Debut_out      , va_FinPrevu_out  , va_Fin_out      , va_Validation_out, va_NumCause_out   , va_CleCause_out  ,
        va_TypeCause_out, va_NumAlerte_out , va_HoroAlerte_out, va_TypeAlerte_out  , va_Origine_out    , va_Confirme_out, va_Autoroute_out ,
        va_PR_out            , va_Sens_out    , va_PointCar_out, va_NomPtCar_out   , va_VR_out    , va_VM2_out  , va_VM1_out  , va_VL_out  ,
        va_BAU_out, va_VR_I_out   , va_VM2_I_out , va_VM1_I_out, va_VL_I_out, va_BAU_I_out , &vl_AutorouteCause_out , &vl_PRCause_out        , &vl_SensCause_out,
        &vl_HoroCause_out , &vl_Position_out  , &vl_Degats_out   , &vl_TypeAnterieur_out, &vl_FausseAlerte_out, &vl_BAU_Etroite_out   , &vl_BAU_I_Etroite_out ,
        &vl_Longueur_out, vl_AbrPtCar_out     , &vl_NumPtCar_out    , &vl_Degrade_out    ,
        &vl_NumEvtInit_out, &vl_CleEvtInit_out, vl_ComEvtInit_out, &vl_TypeEvtInit_out, &vl_AutorouteEvtInit_out,
        &vl_PREvtInit_out, &vl_SensEvtInit_out, &vl_HoroEvtInit_out,
         &vl_SiteAlerte_out, &vl_Datex_out    , &vl_Trafic_out  , &vl_Gravite_out,
        &vl_Bloquant_out, &vl_Duree_out,& vl_Majeur_out, &vl_Ecoulement_out ) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG40_GetFmc: pb appel a XZAE40_Lire_Fiche_MC(%d/%d)",
			va_numero_in,va_cle_in);
    return (XDC_NOK);
  }
  return (XDC_OK);
}
/*------------------------------------------------------
* SERVICE RENDU :

*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG41_GetFmcAccident ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Octet      *va_vl_out,
        XDY_Octet      *va_pl_out,
        XDY_Octet      *va_remorque_out,
        XDY_Octet      *va_car_out,
        XDY_Octet      *va_moto_out,
        XDY_Octet      *va_bl_out,
        XDY_Octet      *va_bg_out,
        XDY_Octet      *va_morts_out,
	XDY_Booleen	*va_matiere_out,
        XDY_Booleen     *va_vehEscota_out,
        XDY_Booleen     *va_persEscota_out,
        XDY_Booleen     *va_perso_out,
        XDY_Booleen     *va_degats_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE41_Lire_Fiche_Accident(va_numero_in,va_cle_in,
				va_vl_out,va_pl_out,va_car_out,va_remorque_out,
				va_moto_out,va_bl_out,va_bg_out,va_morts_out,va_matiere_out,
				va_vehEscota_out,va_persEscota_out,va_perso_out,va_degats_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG41_GetFmcAccident: pb appel a XZAE41_Lire_Fiche_Accident(%d/%d)",
			va_numero_in,va_cle_in);
    return (XDC_NOK);
  }
  return (XDC_OK);
}



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC delestge ou deviation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG42_GetFmcDelestageDeviation (XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        char *va_nomPlan_out,
        XDY_Entier      *va_numPlan_out,
        char *va_hierarchie_out,
	XDY_PR *va_pr,
	XDY_PR *va_pr2)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE42_Lire_Fiche_Delestage(va_numero_in,va_cle_in,va_nomPlan_out,va_numPlan_out,va_hierarchie_out,va_pr,va_pr2)!= XDC_OK) {

    return (XDC_NOK);
  }
  return (XDC_OK);
}




/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC meteo
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG44_GetFmcMeteo (XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_PR  *va_PRfin_out,
	XDY_Octet	*va_perturbation_out,
        XDY_Mot      *va_visibilite_out,
        XDY_Entier      *va_longueur_out,
        XDY_Octet       *va_nature_out,
	XDY_Nom		va_libelle_nature_out)



/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE44_Lire_Fiche_Meteo(va_numero_in,va_cle_in,va_PRfin_out,
			va_perturbation_out,va_visibilite_out,
			va_longueur_out,va_nature_out,va_libelle_nature_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG44_GetFmcMeteo : pb appel a XZAE44_Lire_Fiche_Meteo");
    return (XDC_NOK);
  }
  return (XDC_OK);
}




/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caract�ristiques d.une FMC meteo
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG565_GetFmcRegulation (XDY_Entier va_Numero_in,
        XDY_Entier  va_Cle_in,
        XDY_Entier  *va_Contexte_out,
		XDY_Entier	*va_Scenario_out,
        XDY_Entier  *va_Mesure_out,
        XDY_Entier  *va_Indicateur_Horaire_out,
        char*       va_Transitoire_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{ XDY_Entier	vl_scenario_cible;
   XDY_Entier vl_zone;
   XDY_Horodate vl_horodate;
 int vl_jourSemaine;

  XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodate );

  if (XZAE565_Lire_FMC_Regulation(va_Numero_in,va_Cle_in,vl_horodate,va_Contexte_out,
			va_Scenario_out,&vl_scenario_cible,va_Mesure_out,
			va_Indicateur_Horaire_out,&vl_zone,va_Transitoire_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG565_GetFmcRegulation : pb appel a XZAE565_Lire_FMC_Regulation");
    return (XDC_NOK);
  }
  return (XDC_OK);
}



/*------------------------------------------------------
* SERVICE RENDU :
* lecture des caract�ristiques d.une FMC V�hicule en Marche Arri�re, V�hicule � Contresens, V�hicule Arr�t
�, V�hicule en Panne, V�hicule en Feu
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG45_GetFmcVehicule ( XDY_Entier   va_numero_in,
                            XDY_Octet    va_cle_in,
                            XDY_Octet   *va_Type_out,
                            XDY_Octet   *va_passagers_out,
	                    XDY_Booleen *va_bebe_out,
	                    XDY_Booleen *va_persAgee_out,
	                    XDY_Booleen *va_animaux_out,
	                    XDY_Booleen *va_type_veh_escota_out,
	                    XDY_Octet   *va_chargement_out,
                            char        *va_codemat_out,
	                    char        *va_code_danger_out,
	                    XDY_Mot     *va_code_nature_panne_out,
	                    char        *va_nature_panne_out,
	                    char        *va_positionnement_out,
	                    XDY_Octet   *va_vehicule_roulant_out,
	                    XDY_Octet   *va_pres_mat_dangereuse_out,
	                    XDY_Mot     *va_nb_bebes_out,
	                    XDY_Mot     *va_nb_agees_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Entier   va_depannage_out;
  XDY_Octet    va_site_depannage_out;
  char         va_texte_out[20];
  XDY_Mot      va_numveh_out;
  char         va_marque_out[20];
  char         va_type_out[20];
  char         va_couleur_out[20];
  char         va_immatriculation_out[20];
  XDY_Mot      va_code_pays_out;
  char         va_pays_out[20];
  char         va_nom_out[20];
  char         va_ville_out[20];
  char         va_adresse_out[20];
  char         va_enlev_dem_par_out[20];
  XDY_Entier   va_numero_fiche_appel_out;
  XDY_Octet    va_fax_envoye_out;
  XDY_Mot      va_code_marque_out;
  XDY_Mot      va_code_couleur_out;
  XDY_Mot      va_code_enlev_dem_par_out;
  XDY_Octet    va_site_creation_out;

  if (XZAE45_Lire_Fiche_Vehicule( va_numero_in,
				 va_cle_in,
                                 va_Type_out,
                                 va_passagers_out,
	                         va_bebe_out,
	                         va_persAgee_out,
	                         va_animaux_out,
	                         va_type_veh_escota_out,
	                         va_chargement_out,
                                 va_codemat_out,
	                         &va_depannage_out,
	                         &va_site_depannage_out,
	                         va_texte_out,
	                         &va_numveh_out,
	                         va_marque_out,
	                         va_type_out,
	                         va_couleur_out,
	                         va_immatriculation_out,
	                         &va_code_pays_out,
	                         va_pays_out,
	                         va_nom_out,
	                         va_ville_out,
	                         va_adresse_out,
	                         va_code_danger_out,
	                         va_code_nature_panne_out,
	                         va_nature_panne_out,
	                         va_positionnement_out,
	                         va_vehicule_roulant_out,
	                         va_enlev_dem_par_out,
	                         &va_numero_fiche_appel_out,
	                         va_pres_mat_dangereuse_out,
	                         &va_fax_envoye_out,
	                         &va_code_marque_out,
	                         &va_code_couleur_out,
	                         &va_code_enlev_dem_par_out,
	                         &va_site_creation_out,
	                         va_nb_bebes_out,
	                         va_nb_agees_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG45_GetFmcVehicule : pb appel a XZAE45_Lire_Fiche_Vehicule");
    return (XDC_NOK);
  }
  return (XDC_OK);
}
/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC convoi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG46_GetFmcConvoi ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Mot     *va_longueur_out,
        XDY_Octet    *va_vitesse_out,
        XDY_Mot      *va_hauteur_out,
        XDY_Mot      *va_largeur_out,
        XDY_Mot      *va_poids_out,
        char *va_contenu_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE46_Lire_Fiche_VehLent(va_numero_in,va_cle_in,va_longueur_out,va_vitesse_out,va_hauteur_out,va_largeur_out,va_poids_out,va_contenu_out)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG46_GetFmcConvoi : pb appel a XZAE46_Lire_Fiche_VehLent");
    return (XDC_NOK);
  }
  return (XDC_OK);
}


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Manifestation, grève, opération escargot
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG47_GetFmcManifestation ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Mot      *va_nb_out,
        XDY_Booleen     *va_perception_out,
        XDY_Booleen    *va_violence_out)



/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE47_Lire_Fiche_Manif(va_numero_in,va_cle_in,va_nb_out,va_perception_out,va_violence_out)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG47_GetFmcManifestation: pb appel a XZAE47_Lire_Fiche_Manif");
    return (XDC_NOK);
  }
  return (XDC_OK);
}





/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Basculement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG48_GetFmcBasculement( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_PR  *va_PR_out,
        XDY_Entier      *va_longueur_out,
        XDY_Octet       *va_VR_out,
        XDY_Octet       *va_VM2_out,
        XDY_Octet       *va_VM1_out,
        XDY_Octet       *va_VL_out,
        XDY_Octet       *va_BAU_out,
        XDY_Octet       *va_VR_I_out,
        XDY_Octet       *va_VM2_I_out,
        XDY_Octet       *va_VM1_I_out,
        XDY_Octet       *va_VL_I_out,
        XDY_Octet       *va_BAU_I_out,
	XDY_Entier	*va_vitesse_out,
	XDY_Entier	*va_vitesse_opposee_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Octet vl_type_loc;
  XDY_Nom vl_nom_loc;
  XDY_Entier vl_num_loc;

  if (XZAE48_Lire_Fiche_Basculement(va_numero_in,va_cle_in,
					va_PR_out,va_longueur_out,va_VR_out,va_VM2_out,va_VM1_out,
					va_VL_out,va_BAU_out,va_VR_I_out,va_VM2_I_out,va_VM1_I_out,
					va_VL_I_out,va_BAU_I_out,
					va_vitesse_out, va_vitesse_opposee_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG48_GetFmcBasculement: pb appel a XZAE48_Lire_Fiche_Basculement");
    return (XDC_NOK);
  }
  return (XDC_OK);
}





/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC travaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG49_GetFmcTravaux ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Booleen     *va_mobile_out,
        XDY_Booleen     *va_nature_out,
        XDY_Octet       *va_type_out,
        XDY_Booleen     *va_continu_out,
        XDY_Octet       *va_VR_out,
        XDY_Octet       *va_VM2_out,
        XDY_Octet       *va_VM1_out,
        XDY_Octet       *va_VL_out,
        XDY_Octet       *va_BAU_out,
        XDY_Octet       *va_VR_I_out,
        XDY_Octet       *va_VM2_I_out,
        XDY_Octet       *va_VM1_I_out,
        XDY_Octet       *va_VL_I_out,
        XDY_Octet       *va_BAU_I_out,
        XDY_PR          *va_PRdebut_VR_out,
        XDY_PR          *va_PRfin_VR_out,
        XDY_PR          *va_PRdebut_VM2_out,
        XDY_PR          *va_PRfin_VM2_out,
        XDY_PR          *va_PRdebut_VM1_out,
        XDY_PR          *va_PRfin_VM1_out,
        XDY_PR          *va_PRdebut_VL_out,
        XDY_PR          *va_PRfin_VL_out,
        XDY_PR          *va_PRdebut_BAU_out,
        XDY_PR          *va_PRfin_BAU_out,
        XDY_PR          *va_PRdebut_VR_I_out,
        XDY_PR          *va_PRfin_VR_I_out,
        XDY_PR          *va_PRdebut_VM2_I_out,
        XDY_PR          *va_PRfin_VM2_I_out,
        XDY_PR          *va_PRdebut_VM1_I_out,
        XDY_PR          *va_PRfin_VM1_I_out,
        XDY_PR          *va_PRdebut_VL_I_out,
        XDY_PR          *va_PRfin_VL_I_out,
        XDY_PR          *va_PRdebut_BAU_I_out,
        XDY_PR          *va_PRfin_BAU_I_out,
        XDY_PR          *va_pr_tete_out,
        XDY_Entier      *va_longueur_out,
	XDY_Entier	*va_vitesse_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Horodate va_horodebc_out,va_horofinc_out;
  XDY_Autoroute va_autoroute_out;
  XDY_Sens va_sensm_out;
  XDY_PR va_prtetem_out,va_prqueuem_out;
  XDY_Horodate va_debut_vm2_out,va_fin_vm2_out,va_fin_vr_i_out,va_debut_vr_i_out;
  XDY_Horodate va_fin_bau_i_out,va_debut_bau_i_out,va_fin_vl_i_out,va_debut_vl_i_out,va_fin_vm1_i_out,va_debut_vm1_i_out;
  XDY_Horodate va_fin_vm2_i_out,va_debut_vm2_i_out,va_debut_bau_out,va_debut_vm1_out;
  XDY_Octet va_voiesret_out;
  XDY_Entier va_longueurm_out;
  XDY_Horodate va_fin_vr_out,va_fin_vm1_out,va_fin_vl_out;
  XDY_Horodate va_debut_vr_out,va_debut_vl_out,va_fin_vl_outva_debut_bau_out,va_fin_bau_out;
  XDY_Octet va_personnel;
  XDY_Octet va_bouchon;

  if (XZAE49_Lire_Fiche_Travaux (va_numero_in , va_cle_in,
                                        va_nature_out, va_type_out, va_continu_out, &va_horodebc_out, &va_horofinc_out,
                                        va_mobile_out, &va_autoroute_out, &va_prtetem_out, &va_prqueuem_out, &va_sensm_out,
                                        va_pr_tete_out, &va_voiesret_out, va_longueur_out, &va_longueurm_out, va_VR_out,
                                        va_VM2_out, va_VM1_out, va_VL_out, va_BAU_out, va_VR_I_out, va_VM2_I_out,
                                        va_VM1_I_out, va_VL_I_out, va_BAU_I_out, va_PRdebut_VR_out, va_PRfin_VR_out, &va_debut_vr_out,
                                        &va_fin_vr_out, va_PRdebut_VM2_out, va_PRfin_VM2_out, &va_debut_vm2_out, &va_fin_vm2_out,
                                        va_PRdebut_VM1_out, va_PRfin_VM1_out, &va_debut_vm1_out, &va_fin_vm1_out, va_PRdebut_VL_out,
                                        va_PRfin_VL_out, &va_debut_vl_out, &va_fin_vl_out, va_PRdebut_BAU_out, va_PRfin_BAU_out, &va_debut_bau_out,
                                        &va_fin_bau_out, va_PRdebut_VR_I_out,va_PRfin_VR_I_out, &va_debut_vr_i_out, &va_fin_vr_i_out,va_PRdebut_VM2_I_out,
                                        va_PRfin_VM2_I_out, &va_debut_vm2_i_out, &va_fin_vm2_i_out, va_PRdebut_VM1_I_out,  va_PRfin_VM1_I_out,
                                        &va_debut_vm1_i_out, &va_fin_vm1_i_out, va_PRdebut_VL_I_out, va_PRfin_VL_I_out, &va_debut_vl_i_out,
                                        &va_fin_vl_i_out, va_PRdebut_BAU_I_out, va_PRfin_BAU_I_out, &va_debut_bau_i_out, &va_fin_bau_i_out,
                                        va_vitesse_out, &va_personnel, &va_bouchon) != XDC_OK) {

   return (XDC_NOK);
  }
  //Alex : ce print fait crash si il est activ�
  //XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG49 : %d",*va_vitesse_out);
  return (XDC_OK);
}



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Incident sous concessionnaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG50_GetFmcSousCnc ( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        char *va_nom_out,
        XDY_Booleen   *  va_ferme_out,
        XDY_Booleen     *va_penurie_out,
        XDY_Octet       *va_carburant_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE50_Lire_Fiche_SousCnc(va_numero_in,va_cle_in,va_nom_out,va_ferme_out,va_penurie_out,va_carburant_out)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG50_GetFmcSousCnc: pb appel a XZAE50_Lire_Fiche_SousCnc");
    return (XDC_NOK);
  }
  return (XDC_OK);
}




/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Bouchon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG51_GetFmcBouchon( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_PR  *va_prQueue_out,
        XDY_Mot       *va_niveau_out,
        XDY_Entier      *va_longueur_out,
	XDY_Entier	*va_num_cause_out,
	XDY_Octet	*va_cle_cause_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Octet vl_type_loca;
  XDY_Nom vl_nom_loca;
  int vl_num_loca;
  XDY_Octet vl_autoroute;
  XDY_Octet vl_sens;

  /*if (XZAE51_Lire_Fiche_Bouchon(va_numero_in,va_cle_in,va_prQueue_out,va_niveau_out,va_longueur_out,va_num_cause_out,va_cle_cause_out)!=XDC_OK) {*/
  if (XZAE136_Lire_Fiche_Bouchon_Pour_Strada(va_numero_in,va_cle_in,va_niveau_out,va_longueur_out,
				&vl_autoroute,va_prQueue_out,&vl_sens,va_num_cause_out,va_cle_cause_out,
				&vl_type_loca,vl_nom_loca,&vl_num_loca)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG51_GetFmcBouchon : pb appel a XZAE136_Lire_Fiche_Bouchon_Pour_Strada");
    return (XDC_NOK);
  }
  return (XDC_OK);
}



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Fermeture Echangeur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG52_GetFmcFermEchangeur( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Booleen     *va_entree1_out,
        XDY_Booleen     *va_entree2_out,
	XDY_Booleen     *va_sortie1_out,
        XDY_Booleen     *va_sortie2_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE52_Lire_Fiche_Echangeur(        va_numero_in,va_cle_in,va_entree1_out,va_entree2_out,va_sortie1_out,va_sortie2_out) != XDC_OK ) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG52_GetFmcFermEchangeur: pb appel a XZAE51_Lire_Fiche_Echangeur");
    return (XDC_NOK);
  }
  return (XDC_OK);
}


/*------------------------------------------------------
* SERVICE RENDU :
* bouchon consequence ou pas
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG55_IsBouchonConsequence(XDY_Entier va_numero_in,
			XDY_Octet va_cle_in,
			XDY_Octet *va_nb_out,
			XDY_Entier	*va_num_out,
			XDY_Octet	*va_cle_out)


/*
* ARGUMENTS EN ENTREE :
*
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
{
  *va_nb_out=0;
  if  (XZAE555_Bouchon_Conseq(va_numero_in,va_cle_in,va_nb_out,va_num_out,va_cle_out)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG55_IsBouchonConsequence: pb appel a XZAE555_Bouchon_Conseq");
    return (XDC_NOK);
  }
  return (XDC_OK);
}



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG60_Lire_Config_Voies( XDY_Entier va_numero_in,
        XDY_Octet va_cle_in,
        XDY_Octet       *va_VR_out,
        XDY_Octet       *va_VM2_out,
        XDY_Octet       *va_VM1_out,
        XDY_Octet       *va_VL_out,
        XDY_Octet       *va_BAU_out,
        XDY_Octet       *va_VR_I_out,
        XDY_Octet       *va_VM2_I_out,
        XDY_Octet       *va_VM1_I_out,
        XDY_Octet       *va_VL_I_out,
        XDY_Octet       *va_BAU_I_out,
	XDY_Octet	*va_point_car_out,
	XDY_Entier	*va_num_point_car_out,
	XDY_Octet	*va_nb_entrees_out,
	XDY_Octet	*va_nb_sorties_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAE220_Lire_Config_Voies(va_numero_in,va_cle_in,
					va_VR_out,va_VM2_out,va_VM1_out,
					va_VL_out,va_BAU_out,va_VR_I_out,va_VM2_I_out,va_VM1_I_out,
					va_VL_I_out,va_BAU_I_out,
					va_point_car_out, va_num_point_car_out,
					va_nb_entrees_out,va_nb_sorties_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG60_Lire_Config_Voies: pb appel a XZAE220_Lire_Config_voies");
    return (XDC_NOK);
  }
  return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  config des voies dans un tube
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG85_Lire_Config_Voies_Tube(XDY_Eqt         va_tube_in,
XDY_Octet       *va_VR_out,
XDY_Octet       *va_VM2_out,
XDY_Octet       *va_VM1_out,
XDY_Octet       *va_VL_out,
XDY_Octet       *va_BAU_out,
XDY_Octet       *va_VR_I_out,
XDY_Octet       *va_VM2_I_out,
XDY_Octet       *va_VM1_I_out,
XDY_Octet       *va_VL_I_out,
XDY_Octet       *va_BAU_I_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  /*recherche la somme des neutralisations de voies*/
  /*des evenements en cours dans la zone d influence*/
  /*du tunnel en entree*/

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  config des voies sur le terrain
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG86_Lire_Config_Terrain(  XDY_Autoroute         va_autoroute_in,
			XDY_PR	va_PR_in,
			XDY_Sens	va_sens_in,
        XDY_Octet       *va_VR_out,
        XDY_Octet       *va_VM2_out,
        XDY_Octet       *va_VM1_out,
        XDY_Octet       *va_VL_out,
        XDY_Octet       *va_BAU_out,
        XDY_Octet       *va_VR_I_out,
        XDY_Octet       *va_VM2_I_out,
        XDY_Octet       *va_VM1_I_out,
        XDY_Octet       *va_VL_I_out,
        XDY_Octet       *va_BAU_I_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
        CS_CHAR         *pl_rpc_name = "XZAE86";

        CS_CONNECTION   *pl_connection=XZAGV_Connexion;
        CS_COMMAND      *pl_cmd;

        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
        int             vl_num_Tab_donnees2=11;
        tg_Colonne_donnees2 vl_Tab_donnees2[vl_num_Tab_donnees2];

        XDY_Entier      vl_SPstatus;
        CS_DATETIME     vl_Horodate;
        XDY_TDP         vl_Temps_out;
	CS_DATETIME 	vl_horodate_out;
	CS_TINYINT	vl_Validite_out;

        XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZLG86_Lire_Config_Terrain : Debut d execution" );

        /*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        /*A
        ** Initialisation de la commande
        */

        if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande RMET pour notre stored procedure.
        */

        if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_command() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Initialisation de la structure pour chacun des parametres d I/O
        ** A faire passer a la rpc.
        */

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_autoroute_in,"@va_NumAutoroute_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: asql_ctparam_smallint_input(va_Numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: asql_ctparam_smallint_input(va_Numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_sens_in,"@va_Sens_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: asql_ctparam_smallint_input(va_Numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
        /*
        ** 1 parametre en output
        */
        vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_VR_out;
        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_out,"@va_VR_out"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86 : \nasql_ctparam_smallint_output(va_VR_out) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
                asql_clean_cmd (pl_connection,pl_cmd);
                return(XDC_NOK) ;
        }

	vl_Tab_donnees2[1].pt_value = (CS_VOID *)va_VM2_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_out,"@va_VM2_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[2].pt_value = (CS_VOID *)va_VM1_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_out,"@va_VM1_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[4].pt_value = (CS_VOID *)va_VL_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_out,"@va_VL_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[5].pt_value = (CS_VOID *)va_BAU_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_out,"@va_BAU_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[6].pt_value = (CS_VOID *)va_VR_I_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_I_out,"@va_VR_I_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[7].pt_value = (CS_VOID *)va_VM2_I_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_I_out,"@va_VM2_I_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[8].pt_value = (CS_VOID *)va_VM1_I_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_I_out,"@va_VM1_I_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[9].pt_value = (CS_VOID *)va_VL_I_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_I_out,"@va_VL_I_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[10].pt_value = (CS_VOID *)va_BAU_I_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_out,"@va_BAU_I_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86_Lire_Config_Terrain: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(pl_cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_send() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Traitement du resultat
        */

        while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
        {
                switch ((int)vl_res_type)
                {
                        case CS_ROW_RESULT:
                        case CS_PARAM_RESULT:
                        case CS_STATUS_RESULT:
                                /*B
                                ** Impression de l entete en fonction du type de resultat.
                                */
                                switch ((int)vl_res_type)
                                {
                                        case  CS_ROW_RESULT:
                                                vl_retcode = asql_fetch_data (pl_cmd);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG86: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
                                                vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG86 : PARAMETER RESULTS\n");
                                        case  CS_STATUS_RESULT:
                                                vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG86 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG86 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne 1");
                                                        XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZLG86_Lire_Config_Terrain retourne 1 : %d", (int) vl_SPstatus);
                                                        asql_clean_cmd (pl_connection,pl_cmd);
                                                        return (XDC_NOK);
                                                }
                                                break;
                                }

                                break;
                                case CS_MSG_RESULT:
                                        /*
                                        **
                                        */
                                        vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                                        if (vl_retcode != CS_SUCCEED)
                                        {
                                                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                                                asql_clean_cmd (pl_connection,pl_cmd);
                                                return (XDC_NOK);
                                        }
                                        break;

                                case CS_CMD_SUCCEED:
                                        /*
                                        ** Pas de colonnes retournees.
                                        */
                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une ereeur en executant notre commande.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain Fin d execution", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG86: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );
                                }
        }

        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */
        asql_clean_cmd (pl_connection,pl_cmd);
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG86_Lire_Config_Terrain retourne %d", vl_retcode);
        return(XDC_OK);

}


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG90_Recherche_FMC_Aval(XDY_Octet va_type_in,
        XDY_Autoroute va_autoroute_in,
        XDY_PR  va_PR_in,
        XDY_Octet va_sens_in,
	int	va_distance_in,
        XDY_Booleen       *va_presence_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Entier vl_nb=0;

  if (XZAE556_Recherche_FMC_Aval(va_type_in,va_autoroute_in,va_PR_in,va_sens_in,va_distance_in,&vl_nb)) {
    *va_presence_out=FALSE;
    return (XDC_NOK);
  }
  *va_presence_out = vl_nb>0 ;
  return (XDC_OK);
}

/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
int XZLG979_Ecrit_Memoire_Partagee_RDT() {
int vl_indice;
XZLGT_Liste_RADT *vl_liste;
XZLGT_Nombre_Mem *vl_nb;
 
  vl_liste=(XZLGT_Liste_RADT *)pm_mem_partagee_rdt;
  vl_nb=XZLG993_Get_Nombre();
  
  for (vl_indice=0;vl_indice<vl_nb->Nombre_RDT;vl_indice++) {
    XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG979_Ecrit_Memoire_Partagee_RDT : %d sens %d niveau %d",
		vl_liste[vl_indice].Numero,
		vl_liste[vl_indice].Sens,
		vl_liste[vl_indice].Niveau);
    if (XZLG206_Get_Niveau(vl_liste[vl_indice].Numero,
				vl_liste[vl_indice].Sens,
				&(vl_liste[vl_indice].Niveau)) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZLG979_Ecrit_Memoire_Partagee_RDT: pb appel a XZLG206_Get_Niveau");
      return (XDC_NOK);
    }
  }
 
  return (XDC_OK);
}
XZLGT_Liste_RADT *XZLG980_Get_RADT()
{
  return ((XZLGT_Liste_RADT *)pm_mem_partagee_rdt);
}

int XZLG981_Accede_Memoire_Partagee_RDT()
{
  int vl_memID;
  XZLGT_Nombre_Mem *vl_nombre_mem;
  XZLGT_Liste_RADT *vl_liste;
  int vl_indice;

  vl_nombre_mem = XZLG993_Get_Nombre();

  /*acces a la memoire partagee*/
  if ((vl_memID = shmget(XZLGC_CLE_RADT, sizeof(XZLGT_Liste_RADT)*vl_nombre_mem->Nombre_RDT, 0444)) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG981_Accede_Memoire_Partagee_RDT: pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  if  ((pm_mem_partagee_rdt=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG981_Accede_Memoire_Partagee_RDT: pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  vl_liste=(XZLGT_Liste_RADT *)pm_mem_partagee_rdt;

  for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_RDT;vl_indice++) {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "XZLG981_Accede_Memoire_Partagee_RDT : station %d %d",
			vl_liste[vl_indice].Numero,
			vl_liste[vl_indice].Sens);
  } 
  return (XDC_OK);
}

int XZLG982_Cree_Memoire_Partagee_RDT()
{
  int vl_memID;
  int vl_indice;
  XZLGT_Liste_RADT *vl_liste;
  XZLGT_Nombre_Mem *vl_nb;
  int vl_nb2;

  vl_nb=XZLG993_Get_Nombre();
  XZST_03EcritureTrace(XZSTC_INFO," XZLG982_Cree_Memoire_Partagee_RDT : vl_nb->Nombre_RDT = %d",vl_nb->Nombre_RDT);
  vl_nb2=vl_nb->Nombre_RDT;
  /*cree segment de memoire de la bonne taille*/
  /*if ((vl_memID=shmget(XZLGC_CLE_RADT,sizeof(XZLGT_Liste_RADT)*vl_nb->Nombre_RDT, 0666 |IPC_CREAT) ) < 0) {*/
  if ((vl_memID=shmget(XZLGC_CLE_RADT,sizeof(XZLGT_Liste_RADT)*vl_nb2, 0666 |IPC_CREAT) ) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG982_Cree_Memoire_Partagee_RDT : pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  if  ((pm_mem_partagee_rdt=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG982_Cree_Memoire_Partagee_RDT : pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  memcpy(pm_mem_partagee_rdt,pg_radt,sizeof(XZLGT_Liste_RADT)*vl_nb->Nombre_RDT);
  return (XDC_OK);
}

int XZLG983_Update_Checksum()
{
  XZLGT_Nombre_Mem *vl_nb;

  vl_nb=XZLG993_Get_Nombre();
  vl_nb->Checksum=(vl_nb->Checksum+1)%1000;
  XZST_03EcritureTrace(XZSTC_INFO,"XZLG983_Update_Checksum: %d",vl_nb->Checksum);
  XZLG988_Ecrit_Nombre(*vl_nb);
  return (XDC_OK);
}

int XZLG984_Ecrit_Memoire_Partagee_FMC()
{
  XZLGT_Liste_FMC *vl_liste;
  int vl_indice;
  int vl_index_vide=0;
  int vl_index;
  int vl_trouve=0;
  XZLGT_Liste_FMC vl_liste_a_garder[XZLGC_NB_MAX_FMC];

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    vl_liste_a_garder[vl_indice].Evt.numero=0;
    vl_liste_a_garder[vl_indice].Evt.cle=0;
    vl_liste_a_garder[vl_indice].Diffusion=0;
  }

  /*ce qui est en memoire partagee*/
  vl_liste=(XZLGT_Liste_FMC *)pm_mem_partagee_fmc;

  /*parcourt les nouvelles FMC*/
  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    /*si fin de liste*/
    if (pg_fmc[vl_indice].Evt.numero==0)
      break;

    /*recherche si la FMC existait en memoire partagee*/
    vl_trouve=0;
    for (vl_index=0;vl_index<XZLGC_NB_MAX_FMC;vl_index++) {
      if (vl_liste[vl_index].Evt.numero==0) {
	/*si FMC pas trouvee*/
	if (vl_trouve==0) {
	  XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : ajout FMC %d/%d",
					pg_fmc[vl_indice].Evt.numero,
					pg_fmc[vl_indice].Evt.cle);
	  vl_liste_a_garder[vl_index_vide]=pg_fmc[vl_indice];
	  vl_index_vide++;
	}
	break;
      }
      else {
      if (vl_trouve==1)
	break;
      if ((pg_fmc[vl_indice].Evt.numero==vl_liste[vl_index].Evt.numero) &&
	  (pg_fmc[vl_indice].Evt.cle==vl_liste[vl_index].Evt.cle) 
         ) {
	vl_trouve=1;
        /*suivant les zz*/
	if (pg_fmc[vl_indice].ZZ > vl_liste[vl_index].ZZ) {
	  XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : new ZZ(%d) > old ZZ(%d) => on garde la fmc %d/%d",
				pg_fmc[vl_indice].ZZ,
				vl_liste[vl_index].ZZ,
				pg_fmc[vl_indice].Evt.numero,
				pg_fmc[vl_indice].Evt.cle);
	  vl_liste_a_garder[vl_index_vide]=pg_fmc[vl_indice];
	    vl_liste_a_garder[vl_index_vide].Diffusion = 0;
	  vl_index_vide++;
	}
	else if (pg_fmc[vl_indice].ZZ ==  vl_liste[vl_index].ZZ) {
          /*si complement diffusee et FINIE*/
	  /*if ( (vl_liste[vl_index].Diffusion == pow(2,XZLGC_NB_CLIENTS) - 1) &&*/
	  if ( (vl_liste[vl_index].Diffusion >= XZLGC_NB_CLIENTS) &&
	       (pg_fmc[vl_indice].Bloc_Optio.Fin=='Y')){
	    /*FMC completement diffusee*/
	    /*on la vire et on memorise cette diffusion en base*/
	    XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : FMC %d/%d completement diffusee %d => on la vire + SGBD (fin = %c)",
							pg_fmc[vl_indice].Evt.numero,
							pg_fmc[vl_indice].Evt.cle,
							vl_liste[vl_index].Diffusion,
							pg_fmc[vl_index].Bloc_Optio.Fin);
	    if (XZAE641_Memorise_Diffusion(vl_liste[vl_index].Evt.numero,
						vl_liste[vl_index].Evt.cle,
						2)==XDC_NOK) {
				pg_fmc[vl_indice].Bloc_Optio.Fin=='N';
	    			XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : apres verif on ne la vire pas");
				vl_liste_a_garder[vl_index_vide]=pg_fmc[vl_indice];
				vl_liste_a_garder[vl_index_vide].Diffusion = vl_liste[vl_index].Diffusion;
				vl_index_vide++;
				}
	  else {
	/* si Evt Queue de bouchon alors indique la fin de diffusion pour la tete */
	   if (vl_liste[vl_index].Type==XZAEC_FMC_QueueBouchon) {
	   	if (XZAE641_Memorise_Diffusion(vl_liste[vl_index].Evt_Cause.numero,
						vl_liste[vl_index].Evt_Cause.cle,
						2)==XDC_NOK) {
				pg_fmc[vl_indice].Bloc_Optio.Fin=='N';
				}
		}
		}
	  }
	  else {
	    /*on la garde avec son nombre de diffusion*/
	    XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : FMC %d/%d  gardee avec diffusions = %d/%c",
					pg_fmc[vl_indice].Evt.numero,
					pg_fmc[vl_indice].Evt.cle,
					vl_liste[vl_index].Diffusion,
					pg_fmc[vl_index].Bloc_Optio.Fin);
	    vl_liste_a_garder[vl_index_vide]=pg_fmc[vl_indice];
	    vl_liste_a_garder[vl_index_vide].Diffusion = vl_liste[vl_index].Diffusion;
	    vl_index_vide++;
	  }
	}
      }
      /*else {
	XZST_03EcritureTrace(XZSTC_INFO,"XZLG984_Ecrit_Memoire_Partagee_FMC : nouvelle FMC %d/%d",
					pg_fmc[vl_indice].Evt.numero,
					pg_fmc[vl_indice].Evt.cle);
      }*/
      }
    }
  }

  memcpy(pm_mem_partagee_fmc,vl_liste_a_garder,sizeof(XZLGT_Liste_FMC)*XZLGC_NB_MAX_FMC);

  vl_liste=(XZLGT_Liste_FMC *)pm_mem_partagee_fmc;

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "XZLG984_Ecrit_Memoire_Partagee_FMC : fmc %d %d %d",
			vl_liste[vl_indice].Evt.numero,
			vl_liste[vl_indice].Evt.cle,
			vl_liste[vl_indice].Diffusion);
    if (vl_liste[vl_indice].Evt.numero == 0) {
      break;
    }
  } 
  return (XDC_OK);
}


XZLGT_Liste_FMC *XZLG985_Get_FMC()
{
  return ((XZLGT_Liste_FMC *)pm_mem_partagee_fmc);
}

int XZLG986_Accede_Memoire_Partagee_FMC()
{
  int vl_memID;
  XZLGT_Nombre_Mem *vl_nombre_mem;

  vl_nombre_mem = XZLG993_Get_Nombre();

  /*acces a la memoire partagee*/
  if ((vl_memID = shmget(XZLGC_CLE_FMC,  sizeof(XZLGT_Liste_FMC)*XZLGC_NB_MAX_FMC, 0444)) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG986_Accede_Memoire_Partagee_FMC: pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  if  ((pm_mem_partagee_fmc=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG986_Accede_Memoire_Partagee_FMC: pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  return (XDC_OK);
}



int XZLG987_Libere_Memoire(int va_type) {
  switch (va_type) {
    case (0) : shmdt(pm_mem_partagee_zdp);
		break;
    case (1) : shmdt(pm_mem_partagee_fmc);
		break;
    case (2) : shmdt(pm_mem_partagee_rdt);
		break;
  }
  return (XDC_OK);
}

int XZLG988_Ecrit_Nombre(XZLGT_Nombre_Mem va_nombre)
{
  *((XZLGT_Nombre_Mem *)pm_mem_partagee_nb)=va_nombre;
  return (XDC_OK);
}

int XZLG989_Libere_Memoire() {
  XZLGT_Nombre_Mem vl_nb;
  vl_nb.Nombre_ZDP=0;
  vl_nb.Nombre_FMC=0;
  vl_nb.Nombre_RDT=0;
  vl_nb.Checksum=0;

  *((XZLGT_Nombre_Mem *)pm_mem_partagee_nb)=vl_nb;

  shmdt(pm_mem_partagee_nb);
  shmdt(pm_mem_partagee_zdp);
  shmdt(pm_mem_partagee_fmc);
  return (XDC_OK);
}


int XZLG990_Ecrit_Memoire_Partagee_ZDP() {
int vl_indice;
XZLGT_Liste_Zdp *vl_liste;
XZLGT_Nombre_Mem *vl_nb;
 
  vl_liste=(XZLGT_Liste_Zdp *)pm_mem_partagee_zdp;
  vl_nb=XZLG993_Get_Nombre();
  
  for (vl_indice=0;vl_indice<vl_nb->Nombre_ZDP;vl_indice++) {
    XZST_03EcritureTrace(XZSTC_INFO,"XZLG990_Ecrit_Memoire_Partagee_ZDP : %d(%s) filtre %d",
		vl_liste[vl_indice].Zdp.Numero,
		vl_liste[vl_indice].Zdp.Libelle,
		vl_liste[vl_indice].Valide);
    if (XZLG101_GetZone_TDP(vl_liste[vl_indice].Zdp.Numero,
				&(vl_liste[vl_indice].Tdp.Tdp),
				&(vl_liste[vl_indice].Tdp.Horodate),
				&(vl_liste[vl_indice].Tdp.Validite)) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZLG990_Ecrit_Memoire_Partagee_ZDP: pb appel a XZLG101_GetZone_TDP");
      return (XDC_NOK);
    }
  }
 
  return (XDC_OK);
}



XZLGT_Liste_Zdp *XZLG992_Get_ZDP()
{
  return ((XZLGT_Liste_Zdp *)pm_mem_partagee_zdp);
}

XZLGT_Nombre_Mem *XZLG993_Get_Nombre()
{
  return ((XZLGT_Nombre_Mem *)pm_mem_partagee_nb);
}

int XZLG994_Accede_Memoire_Partagee_Nombre()
{
  int vl_memID;

  if ((vl_memID = shmget(XZLGC_CLE_NOMBRE,sizeof(XZLGT_Nombre_Mem),0444)) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG994_Accede_Memoire_Partagee_Nombre : pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  if ((pm_mem_partagee_nb=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG994_Accede_Memoire_Partagee_Nombre: pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  return (XDC_OK);
}


int XZLG995_Accede_Memoire_Partagee_TDP()
{
  int vl_memID;
  XZLGT_Nombre_Mem *vl_nombre_mem;

  vl_nombre_mem = XZLG993_Get_Nombre();

  /*acces a la memoire partagee*/
  if ((vl_memID = shmget(XZLGC_CLE_TDP, sizeof(XZLGT_Liste_Zdp)*vl_nombre_mem->Nombre_ZDP, 0444)) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG995_Accede_Memoire_Partagee_TDP: pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  if  ((pm_mem_partagee_zdp=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG995_Accede_Memoire_Partagee_TDP: pb appel a shmati. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  return (XDC_OK);
}

int XZLG996_Cree_Memoire_Partagee_TDP()
{
  int vl_memID;
  int vl_indice;
  XZLGT_Liste_Zdp *vl_liste;
  XZLGT_Nombre_Mem *vl_nb;

  vl_nb=XZLG993_Get_Nombre();
  XZST_03EcritureTrace(XZSTC_WARNING," XZLG996_Cree_Memoire_Partagee_TDP : vl_nb->Nombre_ZDP = %d",vl_nb->Nombre_ZDP);
  /*cree segment de memoire de la bonne taille*/
  if ((vl_memID=shmget(XZLGC_CLE_TDP,sizeof(XZLGT_Liste_Zdp)*vl_nb->Nombre_ZDP, 0666 |IPC_CREAT) ) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG996_Cree_Memoire_Partagee_TDP : pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  if  ((pm_mem_partagee_zdp=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG996_Cree_Memoire_Partagee_TDP : pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  memcpy(pm_mem_partagee_zdp,pg_zdp,sizeof(XZLGT_Liste_Zdp)*vl_nb->Nombre_ZDP);
  vl_liste=(XZLGT_Liste_Zdp *)pm_mem_partagee_zdp;

  for (vl_indice=0;vl_indice<vl_nb->Nombre_ZDP;vl_indice++) {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "XZLG996_Cree_Memoire_Partagee_TDP : zdp %d libelle %s",
			vl_liste[vl_indice].Zdp.Numero,
			vl_liste[vl_indice].Zdp.Libelle);
  } 
  return (XDC_OK);
}

int XZLG997_Cree_Memoire_Partagee_FMC()
{
  int vl_memID;
  int vl_indice;

  /*cree segment de memoire de la bonne taille*/
  XZST_03EcritureTrace( XZSTC_WARNING,"XZLG997_Cree_Memoire_Partagee_FMC");

  if ((vl_memID=shmget(XZLGC_CLE_FMC,sizeof(XZLGT_Liste_FMC)*XZLGC_NB_MAX_FMC, 0666 |IPC_CREAT) ) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG997_Cree_Memoire_Partagee : pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  if  ((pm_mem_partagee_fmc=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG997_Cree_Memoire_Partagee : pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++)
    pg_fmc[vl_indice].Evt.numero=0;
  memcpy(pm_mem_partagee_fmc,pg_fmc,sizeof(XZLGT_Liste_FMC)*XZLGC_NB_MAX_FMC);

  return (XDC_OK);
}

int XZLG999_Cree_Memoire_Partagee_Nombre()
{
  int vl_memID;
  XZLGT_Nombre_Mem vl_nb;

  /*cree segment de memoire de la bonne taille*/
  if ((vl_memID=shmget(XZLGC_CLE_NOMBRE,sizeof(XZLGT_Nombre_Mem), 0666 |IPC_CREAT) ) < 0) {
    XZST_03EcritureTrace(XZSTC_WARNING," XZLG999_Cree_Memoire_Partagee_Nombre : pb appel a shmget. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }

  if ((pm_mem_partagee_nb=shmat(vl_memID, NULL, 0)) == (void*) -1) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG999_Cree_Memoire_Partagee_Nombre : pb appel a shmat. Error=%d;%s",errno,strerror(errno));
    return (XDC_NOK);
  }
  vl_nb.Nombre_FMC=0;
  vl_nb.Nombre_ZDP=0;
  vl_nb.Checksum=0;
  *((XZLGT_Nombre_Mem *)pm_mem_partagee_nb)=vl_nb;
  return (XDC_OK);
}




/*------------------------------------------------------
* SERVICE RENDU :
*  acces a la liste memoire des FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XZLGT_Liste_FMC *XZLG99_GetListe(XDY_Entier *va_nb)

/*
* ARGUMENTS EN ENTREE :
*
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
{
   *va_nb=(XZLG993_Get_Nombre())->Nombre_FMC;
   return (pg_fmc);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liberation des ressources memoire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG98_FreeFMC()

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XZLGT_Liste_FMC *vl_liste;
/*
  vl_liste=pg_fmc;

  while (pg_fmc!=NULL) {
    vl_liste= pg_fmc;
    free((void *)vl_liste);
  }
  pg_fmc=NULL;*/
  return (XDC_OK);
}






/*------------------------------------------------------
* SERVICE RENDU :
*  liste des zones de parcours definies dans le SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG100_GetZones(  XDY_District         va_district_in,
                        XZLGT_Liste_Zdp  *va_liste_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Entier vl_cr ;

  XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : XZLG100_GetZones");

  if (XZAO4001_Liste_Zones_De_Parcours (XDC_BASE_CFG,(XDY_District)0,
					(XDY_FonctionInt)XZLG_ajouter_zdp,
					&vl_cr) != XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING,"XZLG100_GetZones : pb appel a XZAO4001_Liste_Zones_De_Parcours");
	return (XDC_NOK);
  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  informations courantes de TDP d'une ZDP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG101_GetZone_TDP(  XDY_Eqt         va_Numero_in,
			XDY_TDP	*va_tdp_out,
			XDY_Horodate *va_horodate_out,
			XDY_Octet *va_validite_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
        CS_CHAR         *pl_rpc_name = "XZAD11";

        CS_CONNECTION   *pl_connection=XZAGV_Connexion;
        CS_COMMAND      *pl_cmd;

        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
        int             vl_num_Tab_donnees2=3;
        tg_Colonne_donnees2 vl_Tab_donnees2[vl_num_Tab_donnees2];

        XDY_Entier      vl_SPstatus;
        CS_DATETIME     vl_Horodate;
        XDY_TDP         vl_Temps_out;
	CS_DATETIME 	vl_horodate_out;
	CS_TINYINT	vl_Validite_out;

        XZST_03EcritureTrace( XZSTC_DEBUG1,"IN : XZLG101_GetZone_TDP : Debut d execution (%d)" ,va_Numero_in);

        /*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        /*A
        ** Initialisation de la commande
        */

        if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande RMET pour notre stored procedure.
        */

        if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_command() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Initialisation de la structure pour chacun des parametres d I/O
        ** A faire passer a la rpc.
        */

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Numero_in,"@va_Numero_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: asql_ctparam_smallint_input(va_Numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }


        /*
        ** 1 parametre en output
        */
        vl_Tab_donnees2[0].pt_value = (CS_VOID *)&vl_Temps_out;
        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)&vl_Temps_out,"@va_Temps_out"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: asql_ctparam_smallint_output(va_Temps_out) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
                asql_clean_cmd (pl_connection,pl_cmd);
                return(XDC_NOK) ;
        }

	vl_Tab_donnees2[1].pt_value = (CS_VOID *)&vl_horodate_out;
	if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_horodate_out,"@va_Horodate_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101_GetZone_TDP: asql_ctparam_date_output(vl_horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	vl_Tab_donnees2[2].pt_value = (CS_VOID *)&vl_Validite_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)&vl_Validite_out,"@va_Validite_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: asql_ctparam_tinyint_output(va_Validite_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP : sortie");
		asql_clean_cmd (pl_connection,pl_cmd);
		return(XDC_NOK) ;
	}

        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(pl_cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_send() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Traitement du resultat
        */

        while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
        {
                switch ((int)vl_res_type)
                {
                        case CS_ROW_RESULT:
                        case CS_PARAM_RESULT:
                        case CS_STATUS_RESULT:
                                /*B
                                ** Impression de l entete en fonction du type de resultat.
                                */
                                switch ((int)vl_res_type)
                                {
                                        case  CS_ROW_RESULT:
                                                vl_retcode = asql_fetch_data (pl_cmd);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG101: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
                                                vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG101 : PARAMETER RESULTS\n");
                                                break;
                                        case  CS_STATUS_RESULT:
                                                vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG101 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG101 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne 1");
                                                        XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZLG101_GetZone_TDP retourne 1 : %d", (int) vl_SPstatus);
                                                        asql_clean_cmd (pl_connection,pl_cmd);
                                                        return (XDC_NOK);
                                                }
                                                break;
                                }

                                break;
                                case CS_MSG_RESULT:
                                        /*
                                        **
                                        */
                                        vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                                        if (vl_retcode != CS_SUCCEED)
                                        {
                                                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                                                asql_clean_cmd (pl_connection,pl_cmd);
                                                return (XDC_NOK);
                                        }
                                        break;

                                case CS_CMD_SUCCEED:
                                        /*
                                        ** Pas de colonnes retournees.
                                        */
                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une ereeur en executant notre commande.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP Fin d execution", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );
                                }
        }
        *va_tdp_out = vl_Temps_out;
	*va_validite_out = vl_Validite_out;

 	if ((vl_retcode = asql_date_Sybase2Ux(vl_horodate_out,va_horodate_out))!= XDC_OK)
	{

		 XZST_03EcritureTrace(XZSTC_WARNING,"XZLG101: asql_date_Sybase2Ux(va_DebutPrevu_out,&va_DebutPrevu) a echoue");
		 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
	 	return (XDC_NOK);
	}

        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */
        asql_clean_cmd (pl_connection,pl_cmd);
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne %d", vl_retcode);
        return(XDC_OK);

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  elaboration TDP en fonction données RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG102_GetRADT_TDP(  XDY_Autoroute         va_autoroute_in,
                        XDY_PR  va_debut_in,
			XDY_PR	va_fin_in,
			XDY_Sens	va_sens_in,
			XDY_Entier	*va_tdp_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAS100_TdP_RADT(va_autoroute_in,
			va_debut_in,
			va_fin_in,
			va_sens_in,
			va_tdp_out) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG102_GetRADT_TDP : pb appel a XZAS100_TdP_RADT");
    return (XDC_NOK);
  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* filtrage d une zone
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG103_FiltreZone(  XDY_Eqt         va_numero_in,
                        XDY_Booleen  va_filtre_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XZLGT_Liste_Zdp *vl_liste;
  int vl_indice;
  XZLGT_Nombre_Mem *vl_nb;

  /*vl_liste=pg_zdp;*/
  vl_liste=(XZLGT_Liste_Zdp *)pm_mem_partagee_zdp;
  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG103_FiltreZone: %d IN %d", 
				va_numero_in);

  vl_nb=XZLG993_Get_Nombre();
  for (vl_indice=0;vl_indice<vl_nb->Nombre_ZDP;vl_indice++) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG103_FiltreZone : zone %d ",
				vl_liste[vl_indice].Zdp.Numero);
    if (vl_liste[vl_indice].Zdp.Numero==va_numero_in) {
      vl_liste[vl_indice].Valide = va_filtre_in;
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG103_FiltreZone : zone %d filtre %d",
					va_numero_in,
					va_filtre_in);
      return (XDC_OK);
    }
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZLG103_FiltreZone: zone %d non trouvee !!",va_numero_in);
  XZST_03EcritureTrace(XZSTC_INTERFACE,"XZLG103_FiltreZone: OUT");
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* acces a pg_zdp
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XZLGT_Liste_Zdp * XZLG104_GetListe()

/*
* ARGUMENTS EN ENTREE :
*
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
{
  return (pg_zdp);
}





/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO4001
* ajoute une zdp a la liste des zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZLG_ajouter_zdp(va_zdp,pa_resultat)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int XZLG_ajouter_zdp(   int		va_nombre_zdp, 
			int		va_indice_zdp,
			XDY_ZDP         va_zdp,
			char		*va_nom,
                        int             *pa_resultat)
{
  XZLGT_Nombre_Mem *vl_nb;
XDY_Entier            vl_entier1,vl_entier2,vl_entier3,vl_entier4,vl_entier5, vl_entier6;
char                  vl_nom_autoroute[10];
  char                  vl_str1[51],vl_str2[51],vl_str3[51];
loc_rds vl_loc;
  XDY_Entier            vl_pr,vl_pr_inf,vl_pr_sup;
  XDY_Octet             vl_sens,vl_sens_sc;
  XDY_Octet             vl_autoroute;
  XDY_PR vl_pr_min,vl_pr_max;
int vl_i;
int vl_pr_entier,vl_pr_m;


  XZST_03EcritureTrace(XZSTC_INTERFACE,"XZLG_ajouter_zdp: %s/%d/%d/%d ajout zdp numero %d",va_nom, va_zdp.Numero,
					va_nombre_zdp,va_zdp.Sens,va_indice_zdp);

  /*premiere insertion*/
  if (pg_zdp==NULL) {
    pg_zdp=calloc(va_nombre_zdp,sizeof(XZLGT_Liste_Zdp));
    vl_nb=XZLG993_Get_Nombre();
    vl_nb->Nombre_ZDP=va_nombre_zdp;
    XZLG988_Ecrit_Nombre(*vl_nb);
  }
    /*valorise la zdp*/
    pg_zdp[va_indice_zdp].Zdp=va_zdp;
    /*sprintf(pg_zdp[va_indice_zdp].Zdp.Libelle,"%d a %d",pg_zdp[va_indice_zdp].Zdp.PR_Debut,pg_zdp[va_indice_zdp].Zdp.PR_Fin);*/
    strcpy(pg_zdp[va_indice_zdp].Zdp.Libelle,va_zdp.Libelle);
    strcpy(pg_zdp[va_indice_zdp].Nom,va_nom);
    pg_zdp[va_indice_zdp].Tdp.Validite=0;
    pg_zdp[va_indice_zdp].Tdp.Tdp=0;
    pg_zdp[va_indice_zdp].Valide=FALSE;
    pg_zdp[va_indice_zdp].Zdp.Franchissement_Seuil=XDC_SEUIL_TDP_ABSENT;


  vl_sens = pg_zdp[va_indice_zdp].Zdp.Sens;
  switch (pg_zdp[va_indice_zdp].Zdp.Sens)
  {
  case XDC_SENS_ENTREE_SUD :
  case XDC_SENS_SORTIE_SUD :
  case XDC_SENS_SUD :   vl_sens_sc=XDC_SENS_SUD;
                        break;
  case XDC_SENS_ENTREE_NORD :
  case XDC_SENS_SORTIE_NORD :
  case XDC_SENS_NORD :  vl_sens_sc=XDC_SENS_NORD;
                        break;
  default :     vl_sens_sc=vl_sens;
                break;
  }

  /*localisation RDS*/
 vl_pr = pg_zdp[va_indice_zdp].Zdp.PR_Debut;
 vl_pr_min = pg_zdp[va_indice_zdp].Zdp.PR_Debut < pg_zdp[va_indice_zdp].Zdp.PR_Fin ? pg_zdp[va_indice_zdp].Zdp.PR_Debut : pg_zdp[va_indice_zdp].Zdp.PR_Fin;
 vl_pr_max =  pg_zdp[va_indice_zdp].Zdp.PR_Debut  >=  pg_zdp[va_indice_zdp].Zdp.PR_Fin ?  pg_zdp[va_indice_zdp].Zdp.PR_Debut : pg_zdp[va_indice_zdp].Zdp.PR_Fin;

 if (XZAE146_Recherche_Loc_Rds(
                                vl_pr_max /*pg_zdp[va_indice_zdp].Zdp.PR_Debut*/,
                                pg_zdp[va_indice_zdp].Zdp.Autoroute,
                                vl_loc.l01,
                                &vl_entier1 ,
                                &vl_entier2 ,
                                vl_str1,
                                vl_str2,
                                &vl_entier3,
                                &vl_entier4,
                                vl_nom_autoroute) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG_ajouter_zdp: pb appel a XZAE146_Recherche_Loc_Rds %d ",
                                va_zdp.Numero);
    return (XDC_NOK);
  }
  vl_i= strlen(vl_nom_autoroute)-1;
  while (vl_i > 0 && vl_nom_autoroute[vl_i]==' ' ) {
                vl_i--;
  }
  vl_nom_autoroute[vl_i+1]='\0';
  strcpy(vl_loc.rnp,vl_nom_autoroute);
  if ((vl_sens == XDC_SENS_SUD) ||
        (vl_sens == XDC_SENS_ENTREE_SUD) ||
        (vl_sens == XDC_SENS_SORTIE_SUD))
        {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str1); /*secundary name*/
        vl_loc.l10[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str2); /*primary name*/
        vl_loc.l06[vl_i+1]='\0';
 
 /*155 162 sens 1 : mougins (pr sup), les adrets (pr inf)*/

        vl_loc.l09=vl_entier3; /*secondary PR*/
        vl_loc.l04=vl_entier4; /* primary PR*/
        vl_loc.l03 = C_l03;
 
        strcpy(vl_loc.l05,"positive");
        strcpy(vl_loc.drp,"positive");
        strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);

        vl_loc.l12 = vl_pr_max - vl_entier1; /*secondary distance*/
        vl_loc.l11 = vl_entier2 - vl_pr_max; /*primary distance*/
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
        if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
    sprintf(pg_zdp[va_indice_zdp].RDS.l04,"%d",vl_loc.l04); /*l09*/
    strcpy(pg_zdp[va_indice_zdp].RDS.l06,vl_loc.l06); /*l10*/
    sprintf(pg_zdp[va_indice_zdp].RDS.l11,"%d",vl_loc.l11); /*l12*/
  }
 /*agen_evt.c 759*/
  else if ((vl_sens == XDC_SENS_NORD) ||
        (vl_sens == XDC_SENS_ENTREE_NORD) ||
        (vl_sens == XDC_SENS_SORTIE_NORD))
   {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
       vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str1);
        vl_loc.l06[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str2);
        vl_loc.l10[vl_i+1]='\0';
 

        vl_loc.l04=vl_entier3;
        vl_loc.l09=vl_entier4;
        vl_loc.l03 = C_l03;
 
        strcpy(vl_loc.l05,"negative");
        strcpy(vl_loc.drp,"negative");
        strcpy(vl_loc.l07,tm_l07_n[vl_autoroute]);

        vl_loc.l11 = vl_pr_max - vl_entier1;
        vl_loc.l12 = vl_entier2 - vl_pr_max;
        if (vl_loc.l12<0) vl_loc.l12 = -vl_loc.l12;
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
    sprintf(pg_zdp[va_indice_zdp].RDS.l09,"%d",vl_loc.l09); /*l09*/
    strcpy(pg_zdp[va_indice_zdp].RDS.l10,vl_loc.l10); /*l10*/
    sprintf(pg_zdp[va_indice_zdp].RDS.l12,"%d",vl_loc.l12); /*l12*/
  }
  else {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str1);
        vl_loc.l10[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str2);
        vl_loc.l06[vl_i+1]='\0';
 

        vl_loc.l09=vl_entier3;
        vl_loc.l04=vl_entier4;
        vl_loc.l03 = C_l03;
        strcpy(vl_loc.l05,"positive");
        strcpy(vl_loc.drp,"unknown");
        strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
       vl_loc.l12 = vl_pr_min - vl_entier1;
        vl_loc.l11 = vl_entier2 - vl_pr_min;
        if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;

        strcpy(vl_loc.l05,"unknown");
        strcpy(vl_loc.l07,C_l07_U);
        }
  vl_pr_entier=floor(vl_pr_max/1000)*1000;
  sprintf(vl_loc.rpip,"%d",vl_pr_entier);
  vl_pr_m= vl_pr_max - vl_pr_entier;
  sprintf(vl_loc.dfpp,"%d",vl_pr_m);
  strcpy(vl_loc.dfps,vl_loc.dfpp);
  strcpy(vl_loc.rpis,vl_loc.rpip);
  strcpy(vl_loc.drs,vl_loc.drp);
  strcpy(vl_loc.rns,vl_loc.rnp);

  vl_loc.l08=0;
  sprintf(pg_zdp[va_indice_zdp].RDS.l01,"%s",vl_loc.l01);
  sprintf(pg_zdp[va_indice_zdp].RDS.l02,"%s",vl_loc.l02);
  sprintf(pg_zdp[va_indice_zdp].RDS.l03,"%d",vl_loc.l03);
  sprintf(pg_zdp[va_indice_zdp].RDS.l05,"%s",vl_loc.l05);
  sprintf(pg_zdp[va_indice_zdp].RDS.l07,"%s",vl_loc.l07);
  sprintf(pg_zdp[va_indice_zdp].RDS.l08,"%d",vl_loc.l08);
  strcpy(pg_zdp[va_indice_zdp].RDS.rnp,vl_loc.rnp);
  strcpy(pg_zdp[va_indice_zdp].RDS.rpip,vl_loc.rpip);
  strcpy(pg_zdp[va_indice_zdp].RDS.dfpp,vl_loc.dfpp);
  strcpy(pg_zdp[va_indice_zdp].RDS.drp,vl_loc.drp);
  strcpy(pg_zdp[va_indice_zdp].RDS.rns,vl_loc.rns);
  strcpy(pg_zdp[va_indice_zdp].RDS.rpis,vl_loc.rpis);
  strcpy(pg_zdp[va_indice_zdp].RDS.dfps,vl_loc.dfps);
  strcpy(pg_zdp[va_indice_zdp].RDS.drs,vl_loc.drs);

   /*pour la fin de la zdp*/
 vl_pr = vl_pr_min /*pg_zdp[va_indice_zdp].Zdp.PR_Fin*/;
 if (XZAE146_Recherche_Loc_Rds(
                                vl_pr_min /*pg_zdp[va_indice_zdp].Zdp.PR_Fin*/,
                                pg_zdp[va_indice_zdp].Zdp.Autoroute,
                                vl_loc.l01,
                                &vl_entier1 ,
                                &vl_entier2 ,
                                vl_str1,
                                vl_str2,
                                &vl_entier3,
                                &vl_entier4,
                                vl_nom_autoroute) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZLG_ajouter_zdp: pb appel a XZAE146_Recherche_Loc_Rds %d ",
                                va_zdp.Numero);
    return (XDC_NOK);
  }
  vl_i= strlen(vl_nom_autoroute)-1;


  if ((vl_sens == XDC_SENS_SUD) ||
        (vl_sens == XDC_SENS_ENTREE_SUD) ||
        (vl_sens == XDC_SENS_SORTIE_SUD))
        {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str1);
        vl_loc.l10[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str2);
        vl_loc.l06[vl_i+1]='\0';
 

        vl_loc.l09=vl_entier3;
        vl_loc.l04=vl_entier4;
        vl_loc.l03 = C_l03;
 
        strcpy(vl_loc.l05,"positive");
        strcpy(vl_loc.drp,"positive");
        strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);

        vl_loc.l12 = vl_pr_min - vl_entier1;
        vl_loc.l11 = vl_entier2 - vl_pr_min;
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
        if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
    sprintf(pg_zdp[va_indice_zdp].RDS.l09,"%d",vl_loc.l09); /*l09*/
    strcpy(pg_zdp[va_indice_zdp].RDS.l10,vl_loc.l10); /*l10*/
    sprintf(pg_zdp[va_indice_zdp].RDS.l12,"%d",vl_loc.l12); /*l12*/
  }
 /*agen_evt.c 759*/
  else if ((vl_sens == XDC_SENS_NORD) ||
        (vl_sens == XDC_SENS_ENTREE_NORD) ||
        (vl_sens == XDC_SENS_SORTIE_NORD))
   {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
       vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str1);
        vl_loc.l06[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str2);
        vl_loc.l10[vl_i+1]='\0';
 

        vl_loc.l04=vl_entier3;
        vl_loc.l09=vl_entier4;
        vl_loc.l03 = C_l03;
 
        strcpy(vl_loc.l05,"negative");
        strcpy(vl_loc.drp,"negative");
        strcpy(vl_loc.l07,tm_l07_n[vl_autoroute]);

        vl_loc.l11 = vl_pr_min - vl_entier1;
        vl_loc.l12 = vl_entier2 - vl_pr_min;
        if (vl_loc.l12<0) vl_loc.l12 = -vl_loc.l12;
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
    sprintf(pg_zdp[va_indice_zdp].RDS.l04,"%d",vl_loc.l04); /*l09*/
    strcpy(pg_zdp[va_indice_zdp].RDS.l06,vl_loc.l06); /*l10*/
    sprintf(pg_zdp[va_indice_zdp].RDS.l11,"%d",vl_loc.l11); /*l12*/
  }
  else {
        strcpy(vl_loc.l02,C_l02);
        vl_i= strlen(vl_str1)-1;
         while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str1[vl_i+1]='\0';
        strcpy(vl_loc.l10,vl_str1);
        vl_loc.l10[vl_i+1]='\0';
 
        vl_i= strlen(vl_str2)-1;
        while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
                vl_i--;
        }
        vl_str2[vl_i+1]='\0';
        strcpy(vl_loc.l06,vl_str2);
        vl_loc.l06[vl_i+1]='\0';
 

        vl_loc.l09=vl_entier3;
        vl_loc.l04=vl_entier4;
        vl_loc.l03 = C_l03;
        strcpy(vl_loc.l05,"positive");
        strcpy(vl_loc.drp,"unknown");
        strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
       vl_loc.l12 = vl_pr_max - vl_entier1;
        vl_loc.l11 = vl_entier2 - vl_pr_max;
        if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
        if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;

        strcpy(vl_loc.l05,"unknown");
        strcpy(vl_loc.l07,C_l07_U);
        }
  vl_pr_entier=floor(vl_pr_max/1000)*1000;
  sprintf(vl_loc.rpip,"%d",vl_pr_entier);

  return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* liste des stations de comptage du SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG200_GetStations( XZLGT_Liste_RADT        *va_liste_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XDY_Entier vl_cr;

  /*if (XZAS55_Lect_RADT_A_Exporter(XZLG_ajouter_radt,&vl_cr) != XDC_OK) {*/
  if (XZAS88_Lect_RADT((XDY_FonctionInt)XZLG_ajouter_radt,&vl_cr) != XDC_OK) {

    return (XDC_NOK);
  }
  va_liste_out=pg_radt;
  return (XDC_OK);
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* valeurs des derniers niveaux de trafic enregistres dans le SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG202_GetNiveaux()

/*
* ARGUMENTS EN ENTREE :
*
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
{
        CS_CHAR         *pl_rpc_name = "XZAS86";

        CS_CONNECTION   *pl_connection=XZAGV_Connexion;
        CS_COMMAND      *pl_cmd;

        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
        int             i;
        XDY_Entier      vl_SPstatus;


        XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZLG202_GetNiveaux : Debut d execution" );

        if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202: ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande RMET pour notre stored procedure.
        */

        if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202: ct_command() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(pl_cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202: ct_send() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Traitement du resultat
        */

        while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
        {
                switch ((int)vl_res_type)
                {
                        case CS_ROW_RESULT:
                        case CS_PARAM_RESULT:
                        case CS_STATUS_RESULT:
                                /*B
                                ** Impression de l entete en fonction du type de resultat.
                                */
                                switch ((int)vl_res_type)
                                {
                                        case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,
									&vl_Nb_Col,&vl_Tab_Ligne);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG202_GetNiveaux: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG202_GetNiveaux : PARAMETER RESULTS\n");
                                                break;
                                        case  CS_STATUS_RESULT:
                                                vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG202_GetNiveaux : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG202_GetNiveaux : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG101_GetZone_TDP retourne 1");
                                                        XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZLG101_GetZone_TDP retourne 1 : %d", (int) vl_SPstatus);
                                                        asql_clean_cmd (pl_connection,pl_cmd);
                                                        return (XDC_NOK);
                                                }
                                                break;
                                }

                                break;
                                case CS_MSG_RESULT:
                                        /*
                                        **
                                        */
                                        vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                                        if (vl_retcode != CS_SUCCEED)
                                        {
                                                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202_GetNiveaux: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
                                                asql_clean_cmd (pl_connection,pl_cmd);
                                                return (XDC_NOK);
                                        }
                                        break;

                                case CS_CMD_SUCCEED:
                                        /*
                                        ** Pas de colonnes retournees.
                                        */
                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une ereeur en executant notre commande.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202_GetNiveaux: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux Fin d execution", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG202_GetNiveaux: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );
                                }
        }

        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {

	  XZLG_StockeNiveau(* (XDY_Mot *) vl_Tab_Ligne[i][0].pt_value,
			  * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value,
			  * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value);
	}

        /*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */

        vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

        if (vl_retcode != XDC_OK)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae68 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */
        asql_clean_cmd (pl_connection,pl_cmd);
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG202_GetNiveaux retourne %d", vl_retcode);
        return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* infos courantes de trafic d'une station de comptage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG201_GetTrafic(XDY_Eqt    va_numero_in,
                        XDY_Sens        va_sens_in,
                        XDY_Entier      *va_Q_out,
                        XDY_Entier      *va_V_out,
                        XDY_Entier      *va_TO_out,
                        XDY_Horodate    *va_horodate_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAS54_Donnees_RADT(va_numero_in,
				va_sens_in,
				va_Q_out,
				va_V_out,
				va_TO_out) != XDC_OK) {
    return (XDC_NOK);
  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* parcours liste des stations
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG203_ParcoursRADT(XDY_FonctionInt    pa_fct_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XZLGT_Liste_RADT *vl_liste;
  XZLGT_Nombre_Mem *vl_nb;
  int vl_indice;

  vl_nb=XZLG993_Get_Nombre();
  vl_liste=(XZLGT_Liste_RADT *)pm_mem_partagee_rdt;
  for (vl_indice=0;vl_indice<vl_nb->Nombre_RDT;vl_indice++) {
    (pa_fct_in)(vl_liste[vl_indice].Numero,
		vl_liste[vl_indice].Sens,
		vl_liste[vl_indice].Niveau);
  }
  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  dernieres infos de comptage pour un ensemble de stations
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG204_GetRADT_Data(int ta_stations_numeros[50], int ta_stations_sens[50], XDY_FonctionInt pa_FonctionUtilisateur_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  int vl_indice;
    for (vl_indice=0;vl_indice<50;vl_indice++) {
      if (XZLG205_GetRADT_Data_Station(ta_stations_numeros[vl_indice],
				ta_stations_sens[vl_indice],
				pa_FonctionUtilisateur_in)!=XDC_OK) {
        return (XDC_NOK);
      }
    }
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  dernieres infos de comptage pour une station
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG205_GetRADT_Data_Station(int va_station_in, XDY_Octet va_sens_in, XDY_FonctionInt pa_FonctionUtilisateur_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
	CS_CHAR         *pl_rpc_name = "XZAS85";

        CS_CONNECTION   *pl_connection=XZAGV_Connexion;
        CS_COMMAND      *pl_cmd;

        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
        int             vl_num_Tab_donnees2 = 8;
        tg_Colonne_donnees2 Tab_donnees2[8];
        tg_row_result   vl_Tab_Ligne;
        int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        int             i;

        XDY_Entier      vl_SPstatus;
        CS_DATETIME     vl_Horodate;
        XDY_Horodate    vl_HorodateFin;

        int             vl_indice;
        XDY_Booleen     vl_Bool;
        int             retcode;

        char vl_identifiant[20];
        char    vl_horodate[50];
        int     vl_validite;
        int     vl_debit;
        int     vl_sens;
        int     vl_debit_PL;
        int     vl_vitesse;
        int     vl_taux;

        XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZLG205_GetRADT_Data_Station : Debut d execution %d/%d",
			va_station_in,va_sens_in			);


        /*A
        ** Initialisation de la commande
        */

        if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande RMET pour notre stored procedure.
        */

        if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_command() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }

        /*A
        ** Initialisation de la structure pour chacun des parametres d I/O
        ** A faire passer a la rpc.
        */

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_station_in,"@va_numero_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }


        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_sens_in,"@va_sens_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205: asql_ctparam_date_input(va_cle_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }




        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(pl_cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_send() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
        /*A
        ** Traitement du resultat
        */

        while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
        {
                switch ((int)vl_res_type)
                {
                        case CS_ROW_RESULT:
                        case CS_PARAM_RESULT:
                        case CS_STATUS_RESULT:
                                /*B
                                ** Impression de l entete en fonction du type de resultat.
                                */
                                switch ((int)vl_res_type)
                                {
                                        case  CS_ROW_RESULT:
                                                vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzas82: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
                                                break;

                                        case  CS_STATUS_RESULT:
                                                vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG205_GetRADT_Data_Station : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZLG205_GetRADT_Data_Station : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne 1");
                                                        XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZLG205_GetRADT_Data_Station retourne 1 : %d", (int) vl_SPstatus);
                                                        asql_clean_cmd (pl_connection,pl_cmd);
                                                        return (XDC_NOK);
                                                }
                                                break;

                                        case  CS_PARAM_RESULT:
                                                vl_retcode = asql_fetch_param_result (pl_cmd ,
                                                        vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,
                                                                "XZLG205_GetRADT_Data_Station : PARAMETER RESULTS\n");
                                                break;
                                }

                                break;


                                case CS_MSG_RESULT:
                                        /*
                                        **
                                        */
                                        vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                                        if (vl_retcode != CS_SUCCEED)
                                        {
                                                XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", vl_retcode);
                                                asql_clean_cmd (pl_connection,pl_cmd);
                                                return (XDC_NOK);
                                        }
                                        break;
                                case CS_CMD_SUCCEED:
                                        /*
                                        ** Pas de colonnes retournees.
                                        */
                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une ereeur en executant notre commande.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station Fin d execution", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", vl_retcode);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        return ( XDC_NOK );
                                }
        }

        /*A
        ** Appel de la fonction utilisateur pour chaque FMC
        */

        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {
           /*B
           * recup des parametres de retour
           */
           strcpy(vl_identifiant, (char *)vl_Tab_Ligne[i][0].pt_value);
           strcpy (vl_horodate, (char *) vl_Tab_Ligne[i][1].pt_value);
           /*vl_sens              = * (XDY_Entier *) vl_Tab_Ligne[i][2].pt_value;*/
           vl_validite          = * (XDY_Entier *) vl_Tab_Ligne[i][3].pt_value;
           vl_debit             = * (XDY_Entier *) vl_Tab_Ligne[i][4].pt_value;
           vl_debit_PL          = * (XDY_Entier *) vl_Tab_Ligne[i][5].pt_value;
           vl_vitesse           = * (XDY_Entier *) vl_Tab_Ligne[i][6].pt_value;
           vl_taux              = * (XDY_Entier *) vl_Tab_Ligne[i][7].pt_value;

           XZST_03EcritureTrace(XZSTC_INFO,"XZLG205_GetRADT_Data_Station: id %s, debit %d, vitesse %d",
                                        vl_identifiant,
                                        vl_debit,
                                        vl_vitesse);
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */
           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"XZLG205_GetRADT_Data_Station : pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }
           else
           {
              if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_horodate,
                                                                vl_identifiant,
                                                                va_sens_in,
                                                                vl_validite,
                                                                vl_debit,
                                                                vl_debit_PL,
                                                                vl_vitesse,
                                                                vl_taux,
								va_station_in) ) != XDC_OK )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
                 asql_clean_cmd(pl_connection, pl_cmd);
                 return (XDC_NOK);
              }
           }

        }

        /*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */
        retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

        if (retcode != XDC_OK)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZLG205_GetRADT_Data_Station retourne %d", retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* infos courantes de niveau de trafic d'une station de comptage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG206_Get_Niveau(XDY_Eqt    va_numero_in,
                        XDY_Sens        va_sens_in,
                        XDY_Octet    *va_niveau_out)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  if (XZAS87_Niveau_RADT(va_numero_in,
				va_sens_in,
				va_niveau_out) != XDC_OK) {
    return (XDC_NOK);
  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liberation des ressources memoire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG299_FreeRADT()

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XZLGT_Liste_RADT *vl_liste;

  vl_liste=pg_radt;
/*
  while (pg_radt!=NULL) {
    vl_liste= pg_radt;
    pg_radt=pg_radt->Suivant;
    free((void *)vl_liste);
  }
  */
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* mise a jour du niveau de trafic dans pg_radt
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG_StockeNiveau(XDY_Mot va_numero_in, XDY_Sens va_sens_in, XDY_Octet va_niveau_in)

/*
* ARGUMENTS EN ENTREE :
*
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
{
  XZLGT_Liste_RADT	*pl_liste;
  XZLGT_Nombre_Mem *vl_nb;
  int vl_indice;

  pl_liste=(XZLGT_Liste_RADT *)pm_mem_partagee_rdt;
  vl_nb=XZLG993_Get_Nombre();

      XZST_03EcritureTrace(XZSTC_INTERFACE,"XZLG_StockeNiveau: station %d, sens %d, niveau %d",
				va_numero_in,va_sens_in,va_niveau_in);

  for (vl_indice=0;vl_indice<vl_nb->Nombre_RDT;vl_indice++) {
    if ( (pl_liste[vl_indice].Numero==va_numero_in) &&
         (pl_liste[vl_indice].Sens==va_sens_in)
       ) {
      XZST_03EcritureTrace(XZSTC_INFO," station %d, sens %d, niveau %d",
				va_numero_in,va_sens_in,va_niveau_in);
      pl_liste[vl_indice].Niveau = va_niveau_in;
      return (XDC_OK);
    }
  }
  return (XDC_NOK);
}

/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE 
******************************************************
* DESCRIPTION DE LA FONCTION :
* ajoute une station a la liste des radt
*
******************************************************
* SEQUENCE D'APPEL :
* XZLG_ajouter_radt()
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int XZLG_ajouter_radt(  XDY_Eqt         va_numero,
			/*XDY_Nom		va_nom,*/
			XDY_Sens	va_sens,
			int             va_nombre_rdt,
			int             va_indice_rdt,
                        int             *pa_resultat)
{
  XZLGT_Nombre_Mem *vl_nb;
  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZLG_ajouter_radt: %d/%d ajout radt %d/%d",
			va_nombre_rdt,va_indice_rdt,va_numero,va_sens);

  /*premiere insertion*/
  if (pg_radt==NULL) {
    pg_radt=calloc(va_nombre_rdt,sizeof(XZLGT_Liste_RADT));
vl_nb=XZLG993_Get_Nombre();
    vl_nb->Nombre_RDT=va_nombre_rdt;
	XZLG988_Ecrit_Nombre(*vl_nb);

  }
    /*valorise la zdp*/
    pg_radt[va_indice_rdt].Numero=va_numero;
    pg_radt[va_indice_rdt].Sens=va_sens;
    /*strcpy(pg_radt[va_indice_rdt].Nom,va_nom);*/
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*    trim
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void XZLG300_trim(char *Ligne)

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/
{
        int     i=0, j=0;

        XZST_03EcritureTrace(XZSTC_DEBUG1, "IN : XZLG300, Ligne = %s.",Ligne);

        /*A on supprime les espaces de debut de ligne */
        while ( Ligne[j] == ' ' )
                j++;
        for ( i=j; i<strlen(Ligne); i++)
                Ligne[i-j] = Ligne[i];
        Ligne[i-j] = '\0';

        /*A on supprime les espaces de fin de ligne */
        i = strlen(Ligne);
        for (j=i-1;j>=0;j--) {
                if ( Ligne[j] == ' ' )
                        Ligne[j] = '\0';
                else break;
        }
        XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : XZLG300, Ligne = %s.", Ligne);
}


void XZLG500_SemCall(int va_op,char va_sem)
{
	int vl_cr;
	struct sembuf sb;

	sb.sem_num=0;
	sb.sem_op=va_op;
	sb.sem_flg=0;
	XZST_03EcritureTrace(XZSTC_WARNING,"XZLG500_SemCall : Evt process ID sempid avant requete (processus utilisateur): %d",semctl(vm_semid_fmc,0,GETPID));
	switch (va_sem) {
		case (XZLGC_SEM_FMC) : vl_cr = semop(vm_semid_fmc,&sb,1);
						break;
		case (XZLGC_SEM_TDP) : vl_cr = semop(vm_semid_tdp,&sb,1);
						break;
		case (XZLGC_SEM_RDT) : vl_cr = semop(vm_semid_rdt,&sb,1);
						break;
	}
	if (vl_cr==-1) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG500_SemCall : pb semop - errno=%d",errno);
	}
	
	XZST_03EcritureTrace(XZSTC_WARNING,"XZLG500_SemCall : Evt process ID sempid apr�s requete (mon ID): %d",semctl(vm_semid_fmc,0,GETPID));
}							

int XZLG501_SemCree(char *va_nom,char va_sem)
{
	key_t vl_cle;
	struct sembuf op;
	int vl_semid;

	vl_cle = ftok(va_nom,va_sem);
	/*vm_semid = semget (vl_cle, 1, IPC_CREAT | IPC_EXCL | 0666);*/
	vl_semid = semget (vl_cle, 1, IPC_CREAT | 0666);
	if (vl_semid < 0) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG501_SemCree: pb appel semget");
		return (XDC_NOK);
	}
	if (semctl(vl_semid,0,SETVAL,1) <0) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZLG501_SemCree: pb appel semctl");
		return (XDC_NOK);
	}
	switch(va_sem) {
		case (XZLGC_SEM_TDP) : vm_semid_tdp=vl_semid;
				break;
		case (XZLGC_SEM_FMC)  : vm_semid_fmc=vl_semid;
				break;
		case (XZLGC_SEM_RDT) : vm_semid_rdt=vl_semid;
				break;
	}
	return (XDC_OK);
}

int XZLG502_SemFree() {
	semctl(vm_semid_tdp,0,IPC_RMID,0);
	semctl(vm_semid_fmc,0,IPC_RMID,0);
	semctl(vm_semid_rdt,0,IPC_RMID,0);
	return (XDC_OK);
}

int XZLG503_SemGet(char *va_nom,char va_proj)
{
	key_t vl_cle;
	vl_cle = ftok(va_nom,va_proj);
	switch (va_proj) {
	case (XZLGC_SEM_FMC) : vm_semid_fmc = semget (vl_cle, 1, IPC_CREAT | 0666);
					break;
	case (XZLGC_SEM_TDP) : vm_semid_tdp = semget (vl_cle, 1, IPC_CREAT | 0666);
					break;
	case (XZLGC_SEM_RDT) : vm_semid_rdt  = semget (vl_cle, 1, IPC_CREAT | 0666);
					break;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"XZLG503_SemGet : Evt process ID : %d",vm_semid_fmc);
	return (XDC_OK);
}
