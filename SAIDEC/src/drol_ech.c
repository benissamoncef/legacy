/* Fichier : $Id: drol_ech.c,v 1.7 2017/10/13 13:43:34 pc2dpdy Exp $        Release : $Revision: 1.7 $        Date : $Date: 2017/10/13 13:43:34 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_tun.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* JMG	16/02/05	ajout explication claire dans XZAP
*			le scenario propose est 1 (et non plus ce qu il y avait dans ACT_ECH!) v1.2
*	13/05/05	initialisation de l index de recherche des propositions courantes des echangeurs REPERE1 1.2
* PNI	21/04/08	Sectorisation : XDC_DN->XDC_DM 1.4 DEM743
* JMG	2/10/09		secto 1.5 DEM887
* JPL	17/01/11	Migration architecture HP ia64 (DEM 971) : adequation types parametres et profil fonctions  1.6
* LCL	04/10/23        Adaptations CNA DEM-483 : lecture site principal en base
************************************************/
#include "drol_ech.h"

#include "xzao21.h"
#include "xzat08.h"
#include "xzap05.h"


#define C_MAX_ECH	2

static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_NOEUD 	*vm_noeud;
T_ECH	tm_ech[C_MAX_ECH];

static int FctEch(int);

static int drol_ech_recherche(XDY_Autoroute ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_District,
			XDY_Horodate);

/* definition des variables locales au module */
static char *version = "$Id: drol_ech.c,v 1.7 2017/10/13 13:43:34 pc2dpdy Exp $ : drol_ech" ;

static XDY_District     DistrictDemandeur;
static XDY_Horodate     HorodateDemande;
static int		vm_last_ech;
extern XDY_Mot	        vg_SitePrincipal;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_ech_init()

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE69
*
* FONCTION
*   on stocke le resultat dans des variables globales
*
------------------------------------------------------*/
{
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_ech_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_ech_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_ech_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  int vl_indice;

  for (vl_indice=0;vl_indice<C_MAX_ECH;vl_indice++)
    tm_ech[vl_indice].Numero=0;
  vm_last_ech=0;

  vm_noeud=va_noeud;
  vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;

  /*s'il y a au moins une voie bloquee ou neutralisee, ou s'il s'agit d'une deviation, on cherche les tunnels candidats*/
  if (va_noeud->Infos_TFM.Termine) return(XDC_OK);
  if (va_noeud->Infos_TFM.Prevu) return (XDC_OK);
  if ( (va_noeud->Type==XZAEC_FMC_Delestage) ||
       (va_noeud->Type==XZAEC_FMC_Deviation) ||
       (va_noeud->Type==XZAEC_FMC_FermetureEch) 
     )
    {
      /*recherche les echangeurs candidats*/
      drol_ech_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation.Sens,
		va_site,
		va_horodate);

    for (vl_indice=0;vl_indice<C_MAX_ECH;vl_indice++)
    {
      if (tm_ech[vl_indice].Numero!=0)
        /*mise en base de la proposition echangeur*/
        if (XZAP05_Ajout_Prop_Echangeur(vg_numero_plan,
			  vg_ordre_ech++,
			  tm_ech[vl_indice].District,
			  "",
			  tm_ech[vl_indice].PrioriteEvtC,
			  tm_ech[vl_indice].Numero,
			  "", 
			  "Fermeture echangeur",
			  tm_ech[vl_indice].ScenarioC,
			  TRUE) != XDC_OK)
            XZST_03EcritureTrace(XZSTC_FONCTION,"drol_ech : pb appel a XZAP05_Ajout_Prop_Echangeur");
     } 
  }
  return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des echangeurs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_ech_recherche(XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_District va_site,
			XDY_Horodate va_horodate)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  XDY_Mot         Eqt=NULL;
  XDY_District    DistrictEqt;
  XDY_Autoroute   AutorouteEqt;
  XDY_PR          PREqt;
  XDY_Sens        SensEqt;
  XDY_Entier      PRInfluence;
  XDY_Octet       Type;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       Priorite;
  XDY_Entier      DistanceEvt;
  int		vl_indice;
  XDY_Mot	vl_ech;
  XDY_District vl_district;

  XZST_03EcritureTrace(   XZSTC_INFO,"IN : drol_ech_recherche aut=%d,PR=%d,sens=%d,site=%d",
						va_autoroute,va_PR,va_sens,va_site);

  if ((va_sens == XDC_SENS_INCONNU) || 
      (va_sens == XDC_SENS_ENTREE_NORD) || 
      (va_sens == XDC_SENS_ENTREE_SUD)  ) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_tun_recherche : OUT evt sur une entree");
    return (XDC_NOK);
  }

  DistrictDemandeur = va_site;
  HorodateDemande = va_horodate;

  /*appelle XZAO21*/
  XZAO21_Rech_Echangeur(va_autoroute, va_PR, va_sens, &vl_ech, &vl_district,va_horodate) ;

  XZST_03EcritureTrace(   XZSTC_INFO,"drol_ech_recherche %d/%d",vl_ech,vl_district);
  if (vl_ech==0) return (XDC_OK);

  FctEch(vl_ech);

  /*suppression des utilisations identiques a l'utilisation courante*/
  /* REPERE1 JMG 1.2*/
  vl_indice=0;
  /*JMG 1.2 FIN*/

  if (tm_ech[vl_indice].ScenarioC==tm_ech[vl_indice].Scenario) {
   	/*mise en base de la proposition avec priorite=255 si prop identique a l'utilisation courante*/
    if (XZAP05_Ajout_Prop_Echangeur(vg_numero_plan,
			  vg_ordre_ech++,
			  tm_ech[vl_indice].District,
			  "",
			  255,
			  tm_ech[vl_indice].Numero,
			  "", 
			  "Fermeture echangeur",
			  tm_ech[vl_indice].ScenarioC,
			  TRUE) != XDC_OK)
            XZST_03EcritureTrace(XZSTC_FONCTION,"drol_ech : pb appel a XZAP05_Ajout_Prop_Echangeur");
    XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
    tm_ech[vl_indice].Numero=0;
  }

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  creation d'un tunnel dans le RTie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int FctEch(Ech)
XDY_Mot         Ech;

/*
* ARGUMENTS EN ENTREE :
*   XDY_Eqt             Tunnel;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAO20
*
* FONCTION
*   creation d'une instance de la classe tunnel dans le RTie
*   appel de XZAT07 et mise a jour des attributs du tunnel
*
------------------------------------------------------*/
{
        XDY_District    District=XDC_VC;
        T_STRING        NomTunnel;
        T_STRING        NumTunnel;
        XDY_Octet       Dispo;
        XDY_Octet       Sequence;
        XDY_Entier      NumEvt;
        XDY_Octet       CleEvt;
        XDY_Octet       Priorite;
	XDY_Entier	DistanceEvt;
	int		vl_indice;

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : Echangeur = %d",Ech);

        /*A
        **  on verifie que l'objet appartient bien au district
        */
        if ((District != DistrictDemandeur) && (DistrictDemandeur != vg_SitePrincipal))
        {
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT");
                return(XDC_OK);
        }

	/*recherche si le tunnel a deja ete traite*/
	for (vl_indice=0;vl_indice<C_MAX_ECH;vl_indice++)
	{
	  if (tm_ech[vl_indice].Numero==Ech) {
	    XZST_03EcritureTrace(XZSTC_FONCTION, " L'echangeur %d existe deja",Ech);
	    return (XDC_OK);
          }
	}

        /*A
        **  on appelle XZAT07
        */
        if (XZAT08_Utilisation_Echangeur(  HorodateDemande, Ech,
                                        &Dispo, &Sequence, &NumEvt, &CleEvt, &Priorite, &DistanceEvt) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,
                        "%s etat de %s inconnu a la date %f. On le considere indispo.",version, NomTunnel, HorodateDemande);
                Dispo = XDC_EQT_MAJEUR;
        }

        /*A
        **  on cree l'objet et on met a jour les slots
        **  identifiant, dispo, sequence, numevt_u, priorite_u, distance_u
        */
        XZST_03EcritureTrace(XZSTC_INFO, "creation de l echangeur %d",Ech);
	tm_ech[vm_last_ech].Numero=Ech;
	tm_ech[vm_last_ech].District=District;
	tm_ech[vm_last_ech].Scenario=Sequence;
	tm_ech[vm_last_ech].ScenarioC=1;
	tm_ech[vm_last_ech].DistanceEvtC=0;
	tm_ech[vm_last_ech].PrioriteEvtC=0;
        if (Dispo <= XDC_EQT_MINEUR)
		tm_ech[vm_last_ech].Dispo=TRUE;
        else
		tm_ech[vm_last_ech].Dispo=FALSE;
        if (NumEvt != 0)
        {
		tm_ech[vm_last_ech].NumEvtU=NumEvt;
		tm_ech[vm_last_ech].CleEvtU=CleEvt;
		tm_ech[vm_last_ech].PrioriteU=Priorite;
		tm_ech[vm_last_ech].DistanceEvtU=DistanceEvt;
        }
	vm_last_ech++;
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT");
        return(XDC_OK);
}
