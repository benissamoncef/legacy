/* Fichier : $Id: acet_evt.c,v 1.9 2020/06/02 21:23:39 devgfi Exp $        $Revision: 1.9 $        $Date: 2020/06/02 21:23:39 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acet_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evenement
* pour le PC STRADA
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* JMG	13/02/16	creation
* JPL	19/03/19 : Ajout nouveaux param�tres d'alerte � XZAE08_Creer_Fiche  1.5
* JPL	19/03/19 : Appel XZAE65_Ecrire_Fiche_Manifestation avec date validation et �tat FMC (DEM 1327)  1.6
* JPL	09/04/19 : Appel XZAE11_Valider_Fiche_MC avec indicateur d'�coulement � Faux (DEM 1326)  1.7
* JPL	05/09/19 : Appel XZAE212_Ecrire_Fiche_Incendie avec code de nature incendie par defaut (DEM 1352)  1.8
* JPL	03/10/19 : Ajout arguments manquants � XZAE61_Ecrire_Fiche_Travaux (DEM 1346)  1.9
************************************************/


#include "acet_evt.h"
#include "xzae63.h"
#include "xzae66.h"
#include "xzae08.h"
#include "xzae53.h"
#include "xzae11.h"
#include "xzae568.h"
#include "xzae65.h"

#define C_DATE_NULLE	-1
#define C_PATH	"/produits/migrazur/appliSD/fichiers/stra/"

liste_att       pm_att[NBR_ELT_TABLE];
liste_att 	vm_noeud;
int vm_hash;
XDY_Octet vm_po_saga;
XDY_Octet vm_cle_site;

/* definition des variables locales au module */

static char *	version = "@(#)acet_evt.c	1.1 05/15/97 : commentaire" ;
static int	vm_frequence = 0; 

/* + les stations*/

/* declaration de fonctions internes */
int     	acet_evt_hash(char *);
void    	acet_evt_ftp();
void    	acet_evt_decode_readfile(char *);
void    	acet_evt_decode();
int	 	acet_evt_decode_readfile_date(char *,XZSMT_Horodate *,char *, int,XDY_Horodate *);
int 		acet_evt_en_base    (int, XDY_Entier *, XDY_Entier *);
int		acet_cree_fmc(XDY_Entier *, XDY_Entier *);
int		acet_valide_fmc(XDY_Entier, XDY_Entier);
int		acet_comment_fmc(int);

/* definition de fonctions externes */
XDY_Entier   vg_NbAutoroutes;
XZAOT_Liste_Auto     *vg_Autoroutes;

char		pm_localisation[TAILLE*4];


static XDY_Booleen acet_bool(char *va_att)
{
  XDY_Booleen vl_ret;
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_bool(%s): %s - %s ",
  				va_att,vm_noeud.name,vm_noeud.valeur);

  vl_ret = (XDY_Booleen)atoi(vm_noeud.valeur);
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_bool: %d",vl_ret);
  return (vl_ret);
}
static XDY_Mot	acet_mot(char *va_att)
{
  XDY_Mot vl_mot;
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];

  vl_mot = (XDY_Mot)atoi(vm_noeud.valeur);
  return (vl_mot);
}
static char* acet_string(char *va_att)
{
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];

  return (vm_noeud.valeur);
}
static XDY_Octet acet_octet(char *va_att)
{
  XDY_Octet vl_ret;
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];

  XZST_03EcritureTrace(   XZSTC_INFO,"acet_octet: %s - %s ",
  				vm_noeud.name,vm_noeud.valeur);
  vl_ret = (XDY_Octet)atoi(vm_noeud.valeur);
  return (vl_ret);
}
static XDY_Entier acet_entier(char *va_att)
{
  XDY_Entier vl_ret;
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];

  XZST_03EcritureTrace(   XZSTC_INFO,"acet_entier: %s - -%s ",
  				vm_noeud.name,vm_noeud.valeur);
  vl_ret = (XDY_Entier)atoi(vm_noeud.valeur);
  XZST_03EcritureTrace(   XZSTC_INFO,"%d",vl_ret);
  return (vl_ret);
}
static XDY_Horodate acet_horodate(char *va_att)
{
  XDY_Horodate vl_ret;
  vm_hash = acet_evt_hash(va_att);
  vm_noeud = pm_att[vm_hash];
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_horodate(%s): %s - -%s ",
  				va_att,vm_noeud.name,vm_noeud.valeur);
  if (!strcmp(vm_noeud.valeur,""))
     return ((XDY_Horodate)C_DATE_NULLE);
  XZSM_13ConversionHorodateSec(vm_noeud.valeur,&vl_ret);
  return (vl_ret);
}

int acet_cree_fmc (XDY_Entier *pa_numero_fmc, XDY_Entier *pa_numero_fmc_tete){
XDY_Octet vl_cle_out;
XDY_Entier vl_fiche_appel_out;
XDY_Octet vl_po_saga;

if (acet_bool("PAT")==XDC_NON) vl_po_saga = 0 ;
else vl_po_saga = vm_po_saga;


  *pa_numero_fmc_tete=0;
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_cree_fmc: appel a XZAE08_Creer_Fiche type %d\npo saga %d\na traiter %d\n",acet_octet("TEV"),vl_po_saga,acet_bool("PAT"));
  XZST_03EcritureTrace(   XZSTC_INFO,"debut %lf\n debut prevue %lf\nsite %d\n",
  			acet_horodate("DDE"),
			acet_horodate("DPR"),
			vm_cle_site);

  if (XZAE08_Creer_Fiche(acet_octet("TEV"),
			vl_po_saga,
			acet_bool("PAT"),
			acet_horodate("DDE"),
			acet_horodate("DPR"),
			vm_cle_site,
			(XDY_Octet)XDC_FAUX,
			pa_numero_fmc,
			&vl_cle_out,
			&vl_fiche_appel_out,
			0, 0)!= XDC_OK) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_cree_fmc : pb appel a XZAE08_Creer_Fiche %d",acet_octet("TEV"));
	*pa_numero_fmc=0;
	return (XDC_NOK);
  }
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_cree_fmc: FMC %d creee",*pa_numero_fmc);

  if (acet_octet("TEV")==XZAEC_FMC_QueueBouchon) {
    /*creation de la tete*/
        XZST_03EcritureTrace(   XZSTC_INFO,"acet_cree_fmc: appel a XZAE08_Creer_Fiche XZAEC_FMC_TeteBouchon");
  	if (XZAE08_Creer_Fiche(XZAEC_FMC_TeteBouchon,
			vm_po_saga,
			acet_bool("PAT"),
			acet_horodate("DDE"),
			acet_horodate("DPR"),
			vm_cle_site,
			(XDY_Octet)XDC_FAUX,
			pa_numero_fmc_tete,
			&vl_cle_out,
			&vl_fiche_appel_out,
			0, 0)!= XDC_OK) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_cree_fmc : pb appel a XZAE08_Creer_Fiche Tete Bouchon");
	*pa_numero_fmc_tete=0;
	return (XDC_NOK);
    }
    XZST_03EcritureTrace(   XZSTC_INFO,"acet_cree_fmc: FMC TETE BOUCHON %d creee",*pa_numero_fmc_tete);
  }

  return (XDC_OK);
}

int acet_valide_accident(XDY_Entier va_numero_fmc) {
  if (XZAE53_Ecrire_Fiche_Accident(va_numero_fmc, vm_cle_site,
			acet_octet("NVL"),
			acet_octet("NPL"),
			acet_octet("NCA"),
			acet_octet("NBU"),
			acet_octet("NMO"),
			acet_octet("NBL"),
			acet_octet("NBG"),
			acet_octet("NMT"),
			XDC_FAUX,	/*va_VehEscota_in*/
			acet_bool("MDA"),
			acet_bool("NES"),
			acet_bool("NPE"),
			acet_bool("NDE"),
			vm_po_saga,
			acet_horodate("VAL"),
			(XDY_Octet)0,	(XDY_Octet)0,(XDY_Octet)0,(XDY_Octet)0) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_accident: pb appel a XZAE53_Ecrire_Fiche_Accident");
    return (XDC_NOK);
  }
  return (XDC_OK);
}

int acet_valide_travaux(XDY_Entier va_numero_fmc) {
  if (XZAE61_Ecrire_Fiche_Travaux(va_numero_fmc, vm_cle_site,
  			acet_bool("NAT"),
			acet_octet("TYP"),
			XDC_FAUX,
			(XDY_Horodate)C_DATE_NULLE,
			(XDY_Horodate)C_DATE_NULLE,
			XDC_FAUX,
			acet_octet("NAU"),
			-1,
			-1,
			0,
			acet_entier("PRF"),
			XDC_FAUX,
			acet_horodate("VAL"),
			vm_po_saga,
			acet_octet("PRE"), /*PAT ? */
			acet_octet("VRN"),acet_octet("VMD"),acet_octet("VMU"),acet_octet("VLN"),acet_octet("BAU"),
			acet_octet("VRI"),acet_octet("VDI"),acet_octet("VUI"),acet_octet("VLI"),acet_octet("BAI"),
			acet_octet("VRN") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRI") : -1, 
			acet_octet("VRN") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRF") : -1,
			acet_octet("VRN") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DDE") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VRN") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DFI") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VMD") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRI") : -1,
			acet_octet("VMD") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRF") : -1,
			acet_octet("VMD") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DDE") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VMD") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DFI") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VMU") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRI") : -1,
			acet_octet("VMU") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRF") : -1,
			acet_octet("VMU") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DDE") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VMU") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DFI") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VLN") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRI") : -1,
			acet_octet("VLN") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRF") : -1,
			acet_octet("VLN") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DDE") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("VLN") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DFI") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("BAU") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRI") : -1,
			acet_octet("BAU") == XDC_VOIE_NEUTRALISEE ? acet_entier("PRF") : -1,
			acet_octet("BAU") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DDE") : (XDY_Horodate)C_DATE_NULLE,
			acet_octet("BAU") == XDC_VOIE_NEUTRALISEE ? acet_horodate("DFI") : (XDY_Horodate)C_DATE_NULLE,
			-1, -1,(XDY_Horodate)C_DATE_NULLE,(XDY_Horodate)C_DATE_NULLE,
			-1, -1,(XDY_Horodate)C_DATE_NULLE,(XDY_Horodate)C_DATE_NULLE,
			-1, -1,(XDY_Horodate)C_DATE_NULLE,(XDY_Horodate)C_DATE_NULLE,
			-1, -1,(XDY_Horodate)C_DATE_NULLE,(XDY_Horodate)C_DATE_NULLE,
			-1, -1,(XDY_Horodate)C_DATE_NULLE,(XDY_Horodate)C_DATE_NULLE,
			0, XDC_FAUX, XDC_FAUX)  != XDC_OK) {
  XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_travaux : pb appel a XZAE61_Ecrire_Fiche_Travaux");
  return (XDC_NOK);
  }
  return (XDC_OK);
}

int acet_valide_meteo(XDY_Entier va_numero_fmc) {
  
  if (XZAE56_Ecrire_Fiche_Meteo(va_numero_fmc, vm_cle_site,
		acet_horodate("VAL"),
		acet_entier("PRF"),
		acet_octet("PEB"),
		(XDY_Mot)0,
		vm_po_saga,
		acet_octet("PAT")) != XDC_OK) {
	XZST_03EcritureTrace(   XZSTC_WARNING, " acet_valide_meteo: pb appel a XZAE56_Ecrire_Fiche_Meteo");
	return (XDC_NOK);
  }
  return (XDC_OK);
}

int acet_valide_delestage(XDY_Entier va_numero_fmc) {
  if (XZAE63_Ecrire_Fiche_Delestage (      va_numero_fmc ,
                                        vm_cle_site,
                                        0,
                                        acet_string("HIE"),
                                        vm_po_saga) != XDC_OK) {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_delestage: pb appel a XZAE63_Ecrire_Fiche_Delestage");
	return (XDC_NOK);
  }
  return (XDC_OK);
}
int acet_valide_fermetureech(XDY_Entier va_numero_fmc) {
  if (XZAE57_Ecrire_Fiche_Fermeture_Echangeur(va_numero_fmc, vm_cle_site,
			acet_bool("ENT"),
			acet_bool("ENR"),
			acet_bool("SOR"),
			acet_bool("SOT"),
  			vm_po_saga)!=XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_fermetureech : pb appel a XZAE57_Ecrire_Fiche_Fermeture_Echangeur %d",va_numero_fmc);
    return (XDC_NOK);
  }
}
int acet_valide_vehlent(XDY_Entier va_numero_fmc) {
  char vl_str[50];
  strcpy(vl_str,acet_string("CMA"));
  if (XZAE55_Ecrire_Fiche_Vehicule_Lent(va_numero_fmc, vm_cle_site,
			acet_horodate("VAL"),
			(XDY_Mot)0,
			(XDY_Octet)0,
			(XDY_Mot)0,
			(XDY_Mot)0,
			(XDY_Mot)0,
			vl_str,
			vm_po_saga,
			acet_octet("PAT"))!= XDC_OK)	{
    XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_vehlent : pb appel a XZAE55_Ecrire_Fiche_Vehicule_Lent %d",va_numero_fmc);
    return (XDC_NOK);
  }
  return (XDC_OK);

}
int acet_valide_vehicule(XDY_Entier va_numero_fmc) {
  char vl_str[5];
  XDY_Octet vl_animaux;

  strcpy(vl_str,acet_string("CMA"));
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_vehicule (%d/%d) : TYP %d, VES %d, CMA %s",
  				va_numero_fmc,vm_cle_site,
				acet_octet("TYP"),
				acet_bool("VES"),
				vl_str);

  if (!strcmp(acet_string("COU"),"Animaux vivants"))
    vl_animaux=1;
  else
    vl_animaux = 0;
  if (XZAE54_Ecrire_Fiche_Vehicule (       va_numero_fmc ,
                                        vm_cle_site,
                                        acet_octet("TYP"),
                                        0,
                                        vl_animaux     ,
                                        acet_bool("VES")     ,
                                        0       ,
                                        vl_str,
                                        ""    ,
                                        1,
                                        XDC_DP,
                                        0         ,
                                        0         ,
                                        0       ,
                                        0       ,
                                        "") != XDC_OK) {
   XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_vehicule : pb appel a XZAE54_Ecrire_Fiche_Vehicule");
   return (XDC_NOK);
  }
 return (XDC_OK);
}
int acet_valide_manifestation(XDY_Entier va_numero_fmc) {
  if (XZAE65_Ecrire_Fiche_Manifestation(va_numero_fmc, vm_cle_site,
		acet_horodate("VAL"),
  		0,
		XDC_FAUX,
		XDC_FAUX,
		(XDY_Mot) vm_po_saga,
		acet_octet("PAT")) != XDC_OK)    {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_manifestation pb appel a XZAE65_Ecrire_Fiche_Manifestation");
	return (XDC_NOK);
  }
  return (XDC_OK);
}
int acet_valide_nature(XDY_Entier va_numero_fmc) {
 char vl_nature[100]="";
 if (XZAE208_Ecrire_Fiche_Nature(va_numero_fmc, vm_cle_site,
 		acet_horodate("VAL"),
		vl_nature,
		acet_octet("NAT"),
		vm_po_saga,
		acet_octet("PAT"))!= XDC_OK)    {
   XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_nature : pb appel a XZAE208_Ecrire_Fiche_Nature");
   return (XDC_NOK);
  }
 return (XDC_OK);
}
int acet_valide_incendievegetation(XDY_Entier va_numero_fmc) {
 if (XZAE212_Ecrire_Fiche_Incendie(va_numero_fmc, vm_cle_site,
 		acet_horodate("VAL"),
		(XDY_Octet) XDC_VRAI,
		(XDY_Octet) 0,
		vm_po_saga,
		acet_octet("PAT"))!= XDC_OK)    {
   XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_nature : pb appel a XZAE208_Ecrire_Fiche_Nature");
   return (XDC_NOK);
  }
 return (XDC_OK);
}
int acet_valide_bouchon(XDY_Entier va_numero_fmc) {
 if (XZAE60_Ecrire_Fiche_Bouchon(va_numero_fmc, vm_cle_site,
 			acet_octet("TYP"),
			acet_horodate("VAL"),
			vm_po_saga,
			acet_octet("PRE")) != XDC_OK)    {
	XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_bouchon pb appel a XZAE60_Ecrire_Fiche_Bouchon");
	return (XDC_NOK);
 }
  return (XDC_OK);
}
int acet_valide_fmc(XDY_Entier va_numero_fmc, XDY_Entier va_numero_fmc_tete) {
XDY_Octet vl_type;

  vl_type = acet_octet("TEV");

  XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_fmc: numero %d, type %d",va_numero_fmc,vl_type);

  switch(vl_type) {
    case (XZAEC_FMC_Accident): acet_valide_accident(va_numero_fmc);
				break;
    case (XZAEC_FMC_Travaux) : acet_valide_travaux(va_numero_fmc);
				break;
    case (XZAEC_FMC_Meteo) : acet_valide_meteo(va_numero_fmc);
				break;
    case (XZAEC_FMC_Deviation) : acet_valide_delestage(va_numero_fmc);
				break;
    case (XZAEC_FMC_Delestage) : acet_valide_delestage(va_numero_fmc);
				break;
    case (XZAEC_FMC_TraitChaussees) : acet_valide_meteo(va_numero_fmc); 
				break;
    case (XZAEC_FMC_FermetureEch) : acet_valide_fermetureech(va_numero_fmc);
				break;
    case (XZAEC_FMC_ConvoiExc) : acet_valide_vehlent(va_numero_fmc);
				break;
    case (XZAEC_FMC_AnimalErrant): break;
    case (XZAEC_FMC_PietonChaussee) : acet_valide_nature(va_numero_fmc);
				break;
    case (XZAEC_FMC_VehLent) : acet_valide_vehlent(va_numero_fmc);
				break;
    case (XZAEC_FMC_Contresens) : acet_valide_vehicule(va_numero_fmc);
				break;
    case (XZAEC_FMC_VehPanne) : acet_valide_vehicule(va_numero_fmc);
				break;
    case (XZAEC_FMC_VehFeu) : acet_valide_vehicule(va_numero_fmc);
				break;
    case (XZAEC_FMC_Manifestation) : acet_valide_manifestation(va_numero_fmc);
				break;
   case (XZAEC_FMC_Escargot) : acet_valide_manifestation(va_numero_fmc);
				break;
   case (XZAEC_FMC_Obstacle) : acet_valide_nature (va_numero_fmc);
			break;
    case (XZAEC_FMC_Inondation) : break;
    case (XZAEC_FMC_ChausseeGliss) : acet_valide_meteo(va_numero_fmc); 
				break;
   case (XZAEC_FMC_Autre) : acet_valide_nature(va_numero_fmc);
				break;
      case (XZAEC_FMC_Incendie):acet_valide_incendievegetation(va_numero_fmc);
			break;
   case (XZAEC_FMC_QueueBouchon): acet_valide_bouchon(va_numero_fmc);
			break;
	default : XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_fmc : type inconnu %d",vl_type);
			return (XDC_NOK);
  }

  XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_fmc: numero %d",va_numero_fmc);
  acet_valide_gen_fmc(va_numero_fmc, va_numero_fmc_tete);

  XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_fmc: numero %d",va_numero_fmc);
/*  if (va_numero_fmc_tete!=0)
    acet_valide_gen_fmc(va_numero_fmc_tete,va_numero_fmc_tete);*/

  return (XDC_OK);
}

int acet_valide_gen_fmc(XDY_Entier va_numero_fmc, XDY_Entier va_numero_fmc_tete) {
  XDY_Entier vl_num_cause;
  XDY_Entier vl_num_tete;
  int vl_cle_cause;
  int vl_evt;
  int vl_evt2;
  int vl_com_evt_init;
  char vl_str[10];
 XDY_Octet vl_majeur;
 char vl_ori[25];

    	 XDY_Octet vl_autoroute,vl_sens,vl_ptcar,vl_typeptcar,vl_position;
	 XDY_PR vl_pr;
	XDY_Octet vl_vr,vl_vm2,vl_vm1,vl_vl, vl_bau, vl_vri,vl_vm2i,vl_vm1i,vl_vli, vl_baui;
	XDY_Octet vl_bauet, vl_baueti,vl_dis, vl_disr;
	int vl_res;

  vl_num_cause=0;
  vl_cle_cause=0;
  vl_com_evt_init=0;

     XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_gen_fmc: FMC %d, FMC tete %d",va_numero_fmc,va_numero_fmc_tete);

  /*chercher la cause eventuelle dans le SAE*/
  /* chercher si on a une FMC qui a ComEvtInit valoris� avec le num�ro de l'�vt SAGA*/
  vl_evt2 = acet_entier("NCE");
  vl_evt = acet_entier("CCE");
  XZST_03EcritureTrace(   XZSTC_INFO,"JMG %d %d",vl_evt, vl_evt2);
  if (vl_evt==XDC_DP)
    if (acet_evt_en_base(vl_evt2,&vl_num_cause,&vl_num_tete)!= XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"acet_valide_gen_fmc: pb appel a acet_evt_en_base");
    }

  if (vl_num_cause==0) {
     XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_gen_fmc: pas de cause dans le SAE");
    /*pas de  FMC existante dans le SAE*/ 
    /*on ne garde pas le lien cause*/
    vl_num_cause=0;
    vl_cle_cause=0;
  }
  else {
   /*j'ai une FMC cause dans le SAE*/
   XZST_03EcritureTrace(XZSTC_FONCTION,"cause trouve : %d",vl_num_cause);
   vl_cle_cause=XDC_DP;
  }

  if (va_numero_fmc_tete!=0) {
     vl_num_cause = va_numero_fmc_tete;
     vl_cle_cause = XDC_DP;
  }

  vl_com_evt_init=acet_entier("COV");
  sprintf(vl_str,"%d",vl_com_evt_init);

  /*on traite un bouchon*/
  if (va_numero_fmc_tete!=0) {
    /*recupere la config des voies car saga nous envoie que dalle */
    XZAE66_Lire_Config_Voies(acet_octet("AUT"), acet_entier("PRI"),  acet_octet("SEN"), 0,0,0,0,
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
  }
  else {
   vl_vr = acet_octet("VRN");
   vl_vm2 = acet_octet("VMD");
   vl_vm1 =  acet_octet("VMU");
   vl_vl = acet_octet("VLN");
   vl_bau = acet_octet("BAU");
   vl_vri = acet_octet("VRI");
   vl_vm2i = acet_octet("VDI");
   vl_vm1i = acet_octet("VUI");
   vl_vli = acet_octet("VLI");
   vl_baui =  acet_octet("BAI");
  }
  strncpy(vl_ori,acet_string("ORI"),25);

XZST_03EcritureTrace(   XZSTC_INFO,"acet_valide_gen_fmc:ORI=%s",vl_ori);

  if (acet_mot("TEV")==XZAEC_FMC_Contresens)
    vl_majeur = XDC_VRAI;
  else
    vl_majeur = XDC_FAUX;
  if (XZAE11_Valider_Fiche_MC(va_numero_fmc ,
                                        XDC_DP,
                                        vm_po_saga,
                                        acet_octet("PRE"),
                                        acet_mot("TEV"),
                                        acet_horodate("DPR"),
                                        acet_horodate("DDE"),
                                        (XDY_Horodate)C_DATE_NULLE,
                                        acet_horodate("DFI"),
                                        acet_horodate("VAL"),
					vl_num_cause,
                                        vl_cle_cause /*vl_cle_cause*/,
                                        0,
                                        vl_ori /*acet_string("ORI")*/ ,
                                        acet_octet("PRE") == XDC_FMC_ETAT_CLOS ? 0 : vm_po_saga,
                                        acet_bool("CSI"),
                                        acet_octet("AUT"),
                                        acet_entier("PRI"),
                                        acet_octet("SEN"),
                                        acet_octet("ECH")/*va_PtCaracteristique_in*/,
                                        acet_octet("ECH") > 0 ? XDC_POINT_CARACT_ECHANGEUR : XDC_POINT_CARACT_AUCUN /*va_TypePtCaracteristique_in*/,
                                        0,
                                        vl_vr /*acet_octet("VRN")*/,
					vl_vm2 /*acet_octet("VMD")*/,
                                        vl_vm1 /*acet_octet("VMU")*/,
                                        vl_vl /*acet_octet("VLN")*/,
                                        vl_bau /*acet_octet("BAU")*/,
                                        vl_vri /*acet_octet("VRI")*/,
                                        vl_vm2i /*acet_octet("VDI")*/,
                                        vl_vm1i /*acet_octet("VUI")*/,
                                        vl_vli /*acet_octet("VLI")*/,
                                        vl_baui /*acet_octet("BAI")*/,
					XDC_FAUX,
                                        XDC_FAUX,
                                        acet_octet("NDE"),
                                        acet_mot("TEV"),
                                        acet_octet("FAL"),
                                        acet_entier("LON"),
                                        vl_str,
                                        0,
					vl_majeur,
					XDC_FAUX /*ecoulement*/) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_gen_fmc: pb appel a XZAE11_Valider_Fiche_MC");
    return (XDC_NOK);
  }


  /*et si tete de bouchon ???? */
  if (va_numero_fmc_tete != 0) {
   /*je valide la tete*/
 /*  vl_com_evt_init = 0;
   vl_num_cause= va_numero_fmc;
   vl_cle_cause = XDC_DP;*/
  vl_com_evt_init=acet_entier("COV");
  sprintf(vl_str,"%d",vl_com_evt_init);

   if (vl_num_cause == va_numero_fmc_tete)
     vl_num_cause = 0;
   if (vl_num_cause==0)
   	vl_cle_cause=0;
   /*validation de la FMc tete de bouchon*/
    if (XZAE11_Valider_Fiche_MC(va_numero_fmc_tete ,
                                        XDC_DP,
                                        vm_po_saga,
                                        acet_octet("PRE"),
                                        XZAEC_FMC_TeteBouchon,
                                        acet_horodate("DPR"),
                                        acet_horodate("DDE"),
                                        (XDY_Horodate)C_DATE_NULLE,
                                        acet_horodate("DFI"),
                                        acet_horodate("VAL"),
					vl_num_cause,
                                        vl_cle_cause,
                                        0,
                                        acet_string("ORI"),
                                        vm_po_saga,
                                        acet_bool("CSI"),
                                        acet_octet("AUT"),
                                        acet_entier("PRF"),
                                        acet_octet("SEN"),
                                         0/*va_PtCaracteristique_in*/,
                                        0/*va_TypePtCaracteristique_in*/,
                                        0,
                                        vl_vr /*acet_octet("VRN")*/,
					vl_vm2 /*acet_octet("VMD")*/,
                                        vl_vm1 /*acet_octet("VMU")*/,
                                        vl_vl /*acet_octet("VLN")*/,
                                        vl_bau /*acet_octet("BAU")*/,
                                        acet_octet("VRI"),
                                        acet_octet("VDI"),
                                        acet_octet("VUI"),
                                        acet_octet("VLI"),
                                        acet_octet("BAI"),
					XDC_FAUX,
                                        XDC_FAUX,
                                        acet_octet("NDE"),
                                        XZAEC_FMC_TeteBouchon,
                                        acet_octet("FAL"),
                                        acet_entier("LON"),
                                        vl_str,
                                        0,
					vl_majeur,
					XDC_FAUX /*ecoulement*/) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "acet_valide_gen_fmc: pb appel a XZAE11_Valider_Fiche_MC");
    return (XDC_NOK);
  }
  }
  return (XDC_OK);
}

int acet_comment_fmc(int va_numero_fmc) {
  char vl_str[400];

  XZST_03EcritureTrace(   XZSTC_WARNING,"acet_comment_fmc : %d",va_numero_fmc);

  strcpy(vl_str,acet_string("COM"));
  XZST_03EcritureTrace(   XZSTC_WARNING,"acet_comment_fmc : %d %d // %lf // %s",
  			va_numero_fmc, vm_cle_site,acet_horodate("VAL"),vl_str);
  if (strlen(vl_str)==0)
    return (XDC_OK);
  if (XZAE67_Ecrire_Commentaires(va_numero_fmc, vm_cle_site,
  				0,
				acet_horodate("VAL"),
				(XDY_Horodate)C_DATE_NULLE,
				vl_str,
				vm_po_saga) != XDC_OK) {
  XZST_03EcritureTrace(   XZSTC_WARNING, "acet_comment_fmc : pb appel a XZAE67_Ecrire_Commentaires");
  return (XDC_NOK);
  }
  return (XDC_OK);
}

/*A
*******************************************
* Fonction qui permet d'ellaborer le
* hashage d'une string
*******************************************
* Arguments : pointeur sur chaine de caracteres
*******************************************/
int acet_evt_hash(char * pa_str)
{
int vl_i,vl_r;

	vl_r = 0;
	
	for(vl_i=0;vl_i<3;vl_i++)
	{
		vl_r = (vl_r*NBR_PREMIER+((int)(pa_str[vl_i]))*(vl_i+1)) % NBR_ELT_TABLE;
	}
	/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : hash : (%s) %d",pa_str,vl_r); */
	return vl_r;
}

/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de programmation qui envoie 
* un datagroup
* - le timer de purge
* - charger le tableau de listes chainees
* contenant l'intitule relatif au code 
* evenement Strada
*******************************************
* Arguments : Aucun
*******************************************/

int acet_evt_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE*5];
char 			vl_str1[TAILLE] ,vl_str2[TAILLE*5],vl_str3[TAILLE] ,vl_str4[TAILLE]   ,vl_str5[TAILLE] ,vl_str6[TAILLE]   ,vl_str7[TAILLE] ,vl_str8[TAILLE];
char 			vl_str9[TAILLE] ,vl_str10[TAILLE] ,vl_str11[TAILLE],vl_str12[TAILLE*5],vl_str13[TAILLE],vl_str14[TAILLE]  ,vl_str15[TAILLE],vl_str16[TAILLE];
char 			vl_str17[TAILLE],vl_str18[TAILLE] ,vl_str19[TAILLE],vl_str20[TAILLE]  ,vl_str21[TAILLE],vl_str22[TAILLE*5],vl_str23[TAILLE];
int			vl_val;
int			vl_frequence = 0; 
int			vl_validate;
int			vl_type;
int 			vl_hash;
int			vl_i;
int			vl_b;
liste_att		pl_att;

	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : acet_evt_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
  	
	vm_po_saga = 106;

  	/* Lecture de la frequence d'exportation des donnees vers Strada */
  	sprintf(vl_str1,"%s/%s",C_PATH,CO_FIC_FREQUENCE);
	if ( (pl_fp = fopen(vl_str1, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acet_evt_init : erreur de lecture du fichier %s !", CO_FIC_FREQUENCE);
		return;
	}	

	/* On lit les lignes du fichier une par une */
	while ( fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # ou ne contenant rien */
		if ( pl_ligne[0] == '#' || pl_ligne[0] == '\n' ) 
			continue;

		/* Pour les autres lignes on recherche la frequence de notre tache */
		if(sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val) == 2) {
			if ( !strcmp( pl_param, "tacet.x") ) {
				vl_frequence = vl_val;
				continue;			
			}
		}
		else {
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acet_evt_init : Le fichier %s n'a pas le bon format !", CO_FIC_FREQUENCE);
		}
	}	
	
	if (! vl_frequence)	/* dans le cas ou le fichier frequence.txt n'a pas de valeur on l'initialise avec 10 secondes */ 
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acet_evt_init : La frequence du client Strada donnees n'a pu etre lue dans le fichier %s !", CO_FIC_FREQUENCE);
		vl_frequence = CO_DELAI_FREQUENCE;
	}

  	fclose(pl_fp);
  	
	XZST_03EcritureTrace(   XZSTC_WARNING, "IN :  acet_evt_init : La frequence du timer evenements Strada est %d", vl_frequence);

	for(vl_i=0 ; vl_i < NBR_ELT_TABLE ; vl_i++){
		strcpy(pm_att[vl_i].name,"");
		strcpy(pm_att[vl_i].valeur,"");
	}

 	vm_cle_site = XDC_DP;

  	vm_frequence=vl_frequence;
  	
	/*A initialisation des timers */
	 XZSM_00InitServices(); 
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(XDG_TIMER_SAGA) ) {	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"IN : acet_evt_init : Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return;
	}
	
        vl_HorodateSec+=vm_frequence;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vm_frequence,
						XDG_TIMER_SAGA,
						XDM_AS_TIMER_CLT_EVT,
						"cle_timer_saga",
						XZSMC_SEC_VRAI) !=  XDC_OK ) {
		/* Ecriture Trace */
        	XZST_03EcritureTrace( XZSTC_WARNING, "IN : acet_evt_init : Programmation du timer de donnees evenement non declenchee. \n " );
        	return ;
	} 

       /*A
	* Initialisation de la callback du timer de programmation 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_CLT_EVT),acet_evt_decode,(T_CB_ARG)NULL) == NULL)  {	
                XZST_03EcritureTrace( XZSTC_FATAL,"IN : acet_evt_init : %s Callback sur XDM_AS_TIMER_CLT_EVT non cree.",version );
		return ;
	}

	return XDC_OK ;

}


/*A
********************************************
* fonction  de acet_evt_decode permettant de 
* lire un fichier de donees evenement Strada
*******************************************
* Arguments : le nom du fichier
*******************************************/
void acet_evt_decode_readfile (char *pa_file)
{
char			* pl_buffer;	
char 			  pl_message[TAILLE*10]="";
char 			* pl_message_buffer;
FILE 			* pl_fD;
char 			* pl_string;
char			* pl_free;

char 			tl_buffer[TAILLE];
char 			tl_buftok[TAILLE];
char			pl_donnee[TAILLE];
char			tl_message_bd[TAILLE_BD];

int			vl_att_int;			/* attribut entier de la localisation RDS Strada 	*/
double			vl_att_float;			/* attribut flottant de la localisation RDS Strada 	*/

liste_att		pl_att;
liste_att		pl_att_pha;

XZSMT_Horodate		pl_strdate;
XDY_Horodate		vl_horodate;

int			vl_i;
int			vl_loop;
int			vl_counter;
int			vl_hash;
int 			vl_memory;
int			vl_boolean;
int			vl_eof;

char			tl_emetteur[TAILLE];
char			vl_code_dob[4];
char			vl_heures[3];
char			vl_minutes[3];
char			tl_att_char[100];
char			vl_nom_att[4];
int			tl_att_int;
double			tl_att_float;
XDY_Mot         	vl_typeFMC;
XDY_Octet		vl_autoroute,vl_autoroutetete;
XDY_PR			vl_PR,vl_PRtete;
XDY_Sens		vl_sens;
int			vl_long=0,vl_long_cumul=0;
char			vl_ligne1[256],vl_ligne2[256],vl_ligne3[256],vl_ligne4[256],vl_nom[256];


char vl_att[10];
char vl_valeur[256];
XDY_Entier vl_numero_fmc=0;
XDY_Entier vl_numero_fmc_tete=0;
int vl_mode_creation = 0;

	XZST_03EcritureTrace(   XZSTC_INFO," traitement fichier %s",pa_file);
	sprintf(vl_valeur,"cp %s /produits/migrazur/appliSD/fichiers/stra/saga_backup/",pa_file);
	system(vl_valeur);

  	if ((pl_fD = fopen (pa_file,"r")) != NULL) {
    		while (fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL)	{
			XZST_03EcritureTrace(   XZSTC_DEBUG1, "buffer : %s",tl_buffer);
			sscanf(tl_buffer,"%[^#]#",vl_att);
    			
			vl_hash = acet_evt_hash(vl_att);
			strcpy(pm_att[vl_hash].name,vl_att);
			XZST_03EcritureTrace(   XZSTC_DEBUG1, "%s",tl_buffer+4);
			if ((tl_buffer+4)[0]=='#')
			  strcpy(pm_att[vl_hash].valeur,"");
 			else {
			  sscanf(tl_buffer+4,"%[^#]#",vl_valeur);
			  XZST_03EcritureTrace(   XZSTC_DEBUG1, "JMG %s",vl_valeur);
			  strcpy(pm_att[vl_hash].valeur,vl_valeur);
			}
	   		XZST_03EcritureTrace(   XZSTC_INFO, "IN :  %d--%s-%s",vl_hash,pm_att[vl_hash].name,pm_att[vl_hash].valeur) ;
		  }
      	fclose (pl_fD);  

	/*PATCH BOUCHON TROP NOMBREUX*/
	/*if (acet_octet("TEV")==XZAEC_FMC_QueueBouchon) {
	  sprintf(tl_att_char,"rm %s",pa_file);
	  system(tl_att_char);
	  return;
        }*/
      	
	/*translation des voies*/
	if (acet_octet("VRN")==XDC_VOIE_INEXISTANTE) { /* pasde VR sur SAGA*/
	  if (acet_octet("VMD") != XDC_VOIE_INEXISTANTE) { /*une VM2 a transformer en VR*/
	    vl_hash = acet_evt_hash("VRN");
	    sprintf(pm_att[vl_hash].valeur,"%d",acet_octet("VMD"));
	    vl_hash = acet_evt_hash("VMD");
	    sprintf(pm_att[vl_hash].valeur,"%d",XDC_VOIE_INEXISTANTE);
	  }
	  else if (acet_octet("VMU") != XDC_VOIE_INEXISTANTE) { /* une VM1 a transformer en VR*/
	    vl_hash = acet_evt_hash("VRN");
	    sprintf(pm_att[vl_hash].valeur,"%d",acet_octet("VMU"));
	    vl_hash = acet_evt_hash("VMU");
	    sprintf(pm_att[vl_hash].valeur,"%d",XDC_VOIE_INEXISTANTE);
	  }
	}

	if (acet_octet("PRE")==1) {
	vl_hash = acet_evt_hash("DDE");
	  strcpy(pm_att[vl_hash].valeur,"");
	}

	vl_numero_fmc=0;
	vl_numero_fmc_tete=0;
	XZST_03EcritureTrace(   XZSTC_INFO, "avant acet_evt_en_base");
      	acet_evt_en_base(acet_entier("COV"),&vl_numero_fmc,&vl_numero_fmc_tete);

        /*pas de fmc dans le SAE*/
	if (vl_numero_fmc==0) {
		vl_mode_creation=1;
		acet_cree_fmc(&vl_numero_fmc,&vl_numero_fmc_tete);

		sprintf(vl_nom,"Numero SAGA : %d",acet_entier("COV"));
  		if (XZAE67_Ecrire_Commentaires(vl_numero_fmc, vm_cle_site,
  				0,
				acet_horodate("VAL"),
				(XDY_Horodate)C_DATE_NULLE,
				vl_nom,
				vm_po_saga) != XDC_OK) {
  			XZST_03EcritureTrace(   XZSTC_WARNING, 
				"acet_comment_fmc : pb appel a XZAE67_Ecrire_Commentaires");
  		}
	}

	/*mise � jour de la FMC*/
	acet_valide_fmc(vl_numero_fmc,vl_numero_fmc_tete);

	/*mise a jour des commentaires*/
	acet_comment_fmc(vl_numero_fmc);

	/*reveil*/
	if (vl_mode_creation==0)
	  sprintf (tl_att_char,"MODIFICATION FMC");
        else
	  sprintf(tl_att_char,"NOUVELLE FMC");
        XZAR03_Reveil(vl_numero_fmc,vm_cle_site,0,tl_att_char,vm_cle_site);

	sprintf(tl_att_char,"rm %s",pa_file);
	system(tl_att_char);

	return;
    }	

    	/* Le fichier n'a pu etre ouvert */
    	XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acet_evt_decode_readfile : le fichier %s n'a pu etre lu",pa_file );
  	return ;

}

/*A
*******************************************
* Fonction call back qui reagit au datagroup
* deginit dans le timer de purge
* Elle decode les fichiers donnees trafic
* et les transmet au synoptique
*******************************************
* Arguments : Aucun
*******************************************/

void acet_evt_decode()
{
FILE 			* pl_fD=NULL;
char 			tl_buffer[TAILLE+1];
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
char 			tl_str[TAILLE];

    	XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN : acet_evt_decode" );

	/* lecture des fichiers donnees trafic Strada */
	sprintf(tl_str, "ls %s/saga/saga-*.txt 2>&-",C_PATH);

  XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN : acet_evt_decode : %s",tl_str); 

	if ( (pl_fD = popen(tl_str,"r")) != NULL) {
		while(fgets(tl_buffer,TAILLE,pl_fD) != NULL) {
			/* lecture du fichier Strada */
			if(sscanf(tl_buffer,"%s\n",tl_str)==1)
				acet_evt_decode_readfile(tl_str);
			sprintf(tl_str, "rm %s/saga/tl_str",C_PATH);
			system(tl_str);
		}
	}
	
	pclose(pl_fD);
	
  return ;
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	aacet_evt_en_base : recoit le message RTWks XDM_AA_alerte, recupere les informations
*	les met en forme Sybase et appelle la SP XZAE141.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int acet_evt_en_base 	(int		va_numero_saga, 
			XDY_Entier 	*pa_numero_fmc,
			XDY_Entier      *pa_numero_fmc_tete)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Creation de l'evt en base 
* 	
------------------------------------------------------*/
{
  char vl_num[10];
  XDY_Octet vl_cle;
  /*appelle la proc stockee XZAEXX pour verifier si la fmc SAGA existe deja dans le SAE*/
  /* chercher si on a une FMC qui a ComEvtInit valoris� avec le num�ro de l'�vt SAGA*/
  /*<=> pa_numero_fmc != 0*/
  sprintf(vl_num,"%d",va_numero_saga);
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_evt_en_base: saga %s",vl_num);
  if (XZAE568_Get_FMC_Saga ( vl_num,
                             pa_numero_fmc,
                             &vl_cle,
			     pa_numero_fmc_tete)!= XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING,"acet_evt_en_base: pb appel a XZAE568_Get_FMC_Saga");
    *pa_numero_fmc=0;
    return (XDC_NOK);
  }
  XZST_03EcritureTrace(   XZSTC_INFO,"acet_evt_en_base: SAE %d // %d",*pa_numero_fmc, *pa_numero_fmc_tete);
return (XDC_OK);
}

