/* Fichier : $Id: drol_caltdp.c,v 1.28 2020/12/03 09:40:51 pc2dpdy Exp $        Release : $Revision: 1.28 $        Date : $Date: 2020/12/03 09:40:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER drol_caltdp.c
******************************************************
* DESCRIPTION DU MODULE :
* module de calcul des temps de parcours
*
******************************************************
* HISTORIQUE :
*
*  Guilhou  03 fev 1997      : Creation
*  Claudel  09 Jul 2007      : Suppression DY
* JMG	18/11/08 : modif interface de XZAO400 DEM/837
* JMG 	02/10/09 : secto DEM887 
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.7
* JMG	20/06/11 : filtrer XZAD_rechercher_dest_fmc que si pas sur A8 1.8
* VR	25/11/11 : Ajout recherche_dest_pmv (DEM/1014)
* VR	08/06/12 : Ajout recherche_dest_pmva (DEM/1014 PMA)
* JMG	12/09/13 : recherche_dest_pmva sens 2 
* PNI	20/01/15 : modification dela pol d'affichage PMV PMVA 1.15 DEM1106
* JPL	02/12/16 : Traces  1.16
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.17
* JMG	23/02/17	: lot 23 traversee DEM1223 1.18
* JPL	05/10/17 : Lecture destinations PMV[A] : eqt., destination, code zdp sur 2 octets; xzao4001 remplace xzao400 (DEM 1236)                                     1.19
* JMG	02/09/18 : ajout XZAD30_Traversee_Bouchon LOT25 DEM1301 1.20
* RGR	23/10/18 : Correction Synchro revision                  1.22
* JMG   14/11/19 : TDP surcharge DEM1365 1.23
* LPE   21/08/20 : Ajout XZAO962_Calcul_TDP_FCD dans XZAD23_Calculer_TdP_PMV_Dest DEM-SAE152 1.24
* ABE	25/09/20 : Correction Calcul TdP_FCD DEM-SAE152 1.25
* LPE	09/10/20 : Récupération nom destination dans XZAD23_Calculer_TdP_PMV_Dest DEM-SAE152 1.26
* LPE	19/11/20 : XZAD23_Calculer_TdP_PMV_Dest : Correction de l'appel a XZA0962 suite a la modification de la procédure DEM-SAE152 1.27
* ABE	03/12/20 : Changement des seuils de validation TDP_FCD pour un const declarée dans xdc.h DEM-SAE152 1.28
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL 	21/04/23 : Correction traces inhibition temps de parcours SAE-431
* LCL	28/09/23 	: Adaptations CNA DEM-483	
*******************************************************/

/* fichiers inclus */
#include "drol_caltdp.h"
#include "xzsc.h"
#include "xzao412.h"
#include "xzao470.h"
#include "xzao.h"
#include	<xdc.h>				


/* declaration des donnees globales */
static	XZADT_Liste_Zdp 		*pg_zdp = NULL;
static	XZADT_Liste_Zdp 		*pg_zdp_fourche = NULL;
static	XZADT_Liste_Zdp 		*pg_zdp_courant;
static	XZADT_Liste_Zdp 		*pg_zdp_fourche_courant;
XZADT_Liste_Destinations 	*pg_dest = NULL;
static	XZADT_Liste_Destinations 	*pg_dest_courant;
static	XZAOT_Liste_PMV_Destinations 	*pg_dest_pmv = NULL;
static	XZAOT_Liste_PMV_Destinations 	*pg_dest_pmv_courant = NULL;
static	XZADT_CONFIG_PMV		*pg_ListePMV=NULL;
static	XZADT_CONFIG_PMA		*pg_ListePMA=NULL;
extern XZSCT_NomMachine	NomMachine;

int	vm_num_site;

/*fonctions utilisateurs passees dans les xzao40x*/
int 	XZAD_ajouter_zdp();		/*construction de la liste des zdp*/
int	XZAD_ajouter_dest();		/*construction de la liste des destinations*/
int	XZAD_rechercher_dest_zdp();	/*recherche de la destination associee a une zdp*/
int XZAD_ajouter_dest_pmv();
static int XZAD_creer_liste_Destination_pmv ( XDY_Eqt va_PMV, XDY_Mot va_Destination,
			XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
			int             va_NumAppel );

static int XZAD_creer_liste_Destination_pmva ( XDY_Eqt va_PMA, XDY_Mot va_Destination,
                                        XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
                                        int             va_NumAppel );


/* declaration des fonctions internes */
int XZAD_recherche_dest_zdp (XDY_ZDP, XDY_Destination_SAD *);



/* declaration des fonctions externes */

static void XZAD20_surcharge_cb(
	T_IPC_CONN	va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG	pa_Arg)
{
#ifdef _TIBCO_EMS
 XDY_Mot	vl_zone;
 XDY_Entier	vl_surcharge;
#else
 T_INT2	vl_zone;
 T_INT4	vl_surcharge;
#endif
  XZADT_Liste_Zdp       *pl_liste_zdp;

 if (!TipcMsgRead(pa_Data->msg,
 			T_IPC_FT_INT2, &vl_zone,
			T_IPC_FT_INT4, &vl_surcharge,
			NULL))
  {
  	XZST_03EcritureTrace(XZSTC_WARNING,"impossible de lire le message XDM_ETDP_SURCHARGE");
	return;
  }
  else
  {
	XZST_03EcritureTrace(XZSTC_FONCTION,"zone %d surcharge %d",
			vl_zone, vl_surcharge);
  }
  pl_liste_zdp=pg_zdp;

  /* la liste est triee par autoroute, sens, PR*/
  while (pl_liste_zdp!=NULL)  {
   if (pl_liste_zdp->zdp.Numero == vl_zone)  
   {
    pl_liste_zdp->zdp.TDP_Surcharge = vl_surcharge;
    XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge stockee");
     return;
   }
    pl_liste_zdp = pl_liste_zdp->suivant;
  }
}

/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* initialise le module de calcul des temps de parcours
* et des destinations
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD20_Init_Calcul_TdP() 
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
extern int XZAD20_Init_Calcul_TdP()
{
  XDY_Entier 	vl_cr=0;
  XDY_TDP	vl_tdp;
  XDY_Horodate	vl_horodate;
  int		vl_jour;
  XZADT_Liste_Zdp       *pl_liste_zdp;
  XZSCT_NomSite   vl_nom;
  XDY_Entier vl_NbElement;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD20_Init_Calcul_TdP : IN");

  if (XZAO4001_Liste_Zones_De_Parcours (XDC_BASE_CFG, (XDY_District) 0,
					(XDY_FonctionInt) XZAD_ajouter_zdp, &vl_cr) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_FATAL,"XZAD20_Init_Calcul_TdP: pb dans l'appel a XZAO4001_Liste_Zones_De_Parcours");
    return (XDC_NOK);
  }

  if (XZSC_04NomSite(vl_nom) != XDC_OK)
    XZST_03EcritureTrace(XZSTC_FATAL, "NomSite inconnu");

  if (!strcmp(vl_nom, XDC_NOM_SITE_CI))
    vm_num_site = XDC_CI;
  else if (!strcmp(vl_nom, XDC_NOM_SITE_VC))
    vm_num_site = XDC_VC;
  else if (!strcmp(vl_nom, XDC_NOM_SITE_DP))
    vm_num_site = XDC_DP;
  else if (!strcmp(vl_nom, XDC_NOM_SITE_CA))
    vm_num_site = XDC_CA;
  else
    XZST_03EcritureTrace(XZSTC_FATAL, "%s : NomSite non prevu", vl_nom);

  XDG_JoindreDG(XDG_ETDP_SURCHARGE);
  TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETDP_SURCHARGE),
  				XZAD20_surcharge_cb, (T_CB_ARG)NULL);
  /*appel XZAO412*/
  if (XZAO412_Lire_Liste_Destination_TDP_SAD(XDC_BASE_CFG,(XDY_FonctionInt)XZAD_ajouter_dest,&vl_cr) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_FATAL,"XZAD20_Init_Calcul_TdP: pb dans l'appel a XZAO401_Lire_Liste_Destination_TDP");
    return (XDC_NOK);
  }
  
//TODO
    /*appel XZAO463*/
  /*
  if (XZAO463_Lire_Liste_PMV(XDC_BASE_CFG,NomMachine, (XDY_FonctionInt)XZAD_ajouter_dest_pmv,&vl_cr) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_FATAL,"XZAD20_Init_Calcul_TdP: pb dans l'appel a XZAO463_Lire_Liste_PMV");
    return (XDC_NOK);
  }
  
  */
  
  /*appel XZAD05*/
  /*horodate courante*/
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jour,&vl_horodate);

  /*on se place au debut de la liste chainee*/
  pg_zdp_courant=pg_zdp;
  
  while (pg_zdp_courant!=NULL) {
    if (XZAD05_Dernier_TDP(pg_zdp_courant->zdp.Numero,
				vl_horodate,
				&vl_tdp)!= XDC_OK)
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD20_Init_Calcul_TdP: pb dans l'appel a XZAD05_Dernier_TDP(%d)",
				pg_zdp_courant->zdp.Numero);
    else {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD20_Init_Calcul_TdP: le tdp de la zdp %d vaut %d",
				pg_zdp_courant->zdp.Numero,
				vl_tdp);
      pg_zdp_courant->zdp.TDP_Courant=vl_tdp;
    }

    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  pg_zdp_fourche_courant=pg_zdp_fourche;
  while (pg_zdp_fourche_courant!=NULL) {
    /*j'associe chaque fourche a sa zdp sur section courante*/
    pl_liste_zdp=pg_zdp;

    pg_zdp_fourche_courant->zdp.ZDP_Section=NULL;
    while (pl_liste_zdp!=NULL) {
      if ( (pl_liste_zdp->zdp.Type_Zone==XDC_ZDP_TDP_SECTION)           &&
	     (pl_liste_zdp->zdp.Autoroute==pg_zdp_fourche_courant->zdp.Autoroute) &&
	     (pl_liste_zdp->zdp.Sens==pg_zdp_fourche_courant->zdp.Sens)           &&
	     (pl_liste_zdp->zdp.PR_Debut>=pg_zdp_fourche_courant->zdp.PR_Debut)   &&
	     (pl_liste_zdp->zdp.PR_Debut<=pg_zdp_fourche_courant->zdp.PR_Fin)
	   ) {
	  pg_zdp_fourche_courant->zdp.ZDP_Section=pl_liste_zdp->zdp.Numero;

	  XZST_03EcritureTrace(XZSTC_INFO,"XZAD20_Init_Calcul_TdP: zdp fourche %d associe a zdp %d",
				pg_zdp_fourche_courant->zdp.Numero,
				pl_liste_zdp->zdp.Numero);
	  break;
	}
	pl_liste_zdp=pl_liste_zdp->suivant;
      }
    pg_zdp_fourche_courant=pg_zdp_fourche_courant->suivant;
  }

   vl_cr = XZAO470_Lire_Liste_Destination_PMV (XDC_BASE_CFG,
                                                (XDY_FonctionInt) XZAD_creer_liste_Destination_pmv,
                                                &vl_NbElement );
   vl_cr = XZAO469_Lire_Liste_Destination_PMVA (XDC_BASE_CFG,
                                                (XDY_FonctionInt) XZAD_creer_liste_Destination_pmva,
                                                &vl_NbElement );

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD20_Init_Calcul_TdP : OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* met a jour le TDP 6 minutes pour une ZDP
* 
******************************************************
* SEQUENCE D'APPEL :
* XZAD21_Maj_TdP_ZdP(va_numero_zdp,
*		       va_tdp)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD21_Maj_TdP_ZdP(	XDY_Entier 	va_numero_zdp,
				XDY_TDP 	va_tdp)
{
  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD21_Maj_TdP_ZdP : IN");

  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD21_Maj_TdP_ZdP: num zdp %d, tdp %d",
					va_numero_zdp,
					va_tdp);

  /*on se place au debut de la liste chainee*/
  pg_zdp_courant=pg_zdp;

  /*parcourt cette liste*/
  while (pg_zdp_courant!=NULL) {
    /*si je suis sur la bonne zdp*/
    if (pg_zdp_courant->zdp.Numero==va_numero_zdp) {
      /*mise a jour du tdp*/
      pg_zdp_courant->zdp.TDP_Courant = va_tdp;
      return (XDC_OK);
    }
    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  /*si je suis en fin de liste*/
  if (pg_zdp_courant==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZAD21_Maj_TdP_ZdP: la Zdp %d n'existe pas dans la liste",
				va_numero_zdp);
    return (XDC_NOK);
  }
  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD21_Maj_TdP_ZdP : OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* retourne le TDP 6 minutes pour une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD22_Lire_TdP_ZdP(va_numero_zdp,
*			   va_site_zdp,
*			   *pa_tdp) 
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD22_Lire_TdP_ZdP(	XDY_Entier 	va_numero_zdp,
				XDY_Mot 	va_site_zdp,
			       	XDY_TDP 	*pa_tdp)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD22_Lire_TdP_ZdP : IN");

  /*on se place au debut de la liste chainee*/
  pg_zdp_courant=pg_zdp;

  /*parcourt cette liste*/
  while (pg_zdp_courant!=NULL) {
    /*si je suis sur la bonne zdp*/
    if ( (pg_zdp_courant->zdp.Numero==va_numero_zdp) &&
	 (pg_zdp_courant->zdp.Site==va_site_zdp)) {
      /*retour du tdp*/
      *pa_tdp=pg_zdp_courant->zdp.TDP_Courant;

      /*trace*/
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD22_Lire_TdP_ZdP: le tdp de la zdp %d %d vaut %d",
					va_numero_zdp,
					va_site_zdp,
					*pa_tdp);
    }
  }

  /*si je suis en fin de liste*/
  if (pg_zdp_courant==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZAD22_Lire_TdP_ZdP: la Zdp %d %d n'existe pas dans la liste",
				va_numero_zdp,
				va_site_zdp);
    return (XDC_NOK);
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD22_Lire_TdP_ZdP : OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* calcule le temps de parcours entre un PMV et une
* destination
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD23_Calculer_TdP_PMV_Dest(va_pr_pmv, 
*				va_autoroute_pmv,
*			        va_sens_pmv, 
*				va_destination,
				pa_tdp)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
*
******************************************************/

extern int XZAD23_Calculer_TdP_PMV_Dest( 
                          XDY_PR 		va_pr_pmv,
                          XDY_Autoroute 	va_autoroute_pmv,
			  XDY_Sens 		va_sens_pmv,
			  int		va_destination,
                          XZEZT_Liste_Zdp	*pa_zdp,
			  XDY_TDP		*pa_tdp,
			  char			*va_nom,
        int       numPMV,
        int       typeEqt)
{
  XDY_TDP			vl_tdp_zdp=0;
  XDY_PR        		vl_PR_d=-1;
  XDY_Autoroute 		vl_autoroute_d=0;
  XDY_Sens     			vl_sens_d=0;		
  XDY_TDP       		vl_tdp_fcd=0;
  XDY_Octet    			vl_validite_fcd=0;
  XDY_Booleen			vl_validite_faible=1;
  XZADT_Liste_Destinations	*pl_dest;
  XDY_Octet						vl_inhb;
		
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : IN");

  XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZAD23_Calculer_TdP_PMV_Dest: pr_pmv %d, autoroute %d, sens %d",
			va_pr_pmv,
			va_autoroute_pmv,
			va_sens_pmv
			);

  /* Test pour savoir si les tdp sur cette eqt sont inhibe */
  xzae59_Rechercher_EQT_TDP_INHIB(numPMV,typeEqt,&vl_inhb);
  if (vl_inhb)
  {
    if (typeEqt==XDC_EQT_PMV)
      XZST_03EcritureTrace(XZSTC_INFO,"XZAD23_Calculer_TdP_PMV_Dest TDP PMV %d inhibe, on retourne -1",numPMV);
    if (typeEqt==XDC_EQT_PMVA)
      XZST_03EcritureTrace(XZSTC_INFO,"XZAD23_Calculer_TdP_PMV_Dest TDP PMVA %d inhibe, on retourne -1",numPMV);
    *pa_tdp=-1;
    return (XDC_OK);
  }

  /*On recupere le PR, l'autoroute et le sens de la destination*/
  pl_dest=pg_dest;

  while (pl_dest!=NULL) {
    if (pl_dest->destination.Numero==va_destination)  {
      vl_PR_d=pl_dest->destination.PR;
      vl_autoroute_d=pl_dest->destination.Autoroute;
      vl_sens_d=pl_dest->destination.Sens;
      strncpy(va_nom,pl_dest->destination.Libelle_PMV,14);
      va_nom[14]='\0';
      break;
    }
    pl_dest=pl_dest->suivant;
  }

  /*Recuperation du TDP grace a l'appel a la procedure XZAO962*/

  if (XZAO962_Calcul_TDP_FCD(  	va_autoroute_pmv,
                            	va_pr_pmv,
                            	va_sens_pmv,
                            	vl_autoroute_d,
                            	vl_PR_d,
                            	vl_sens_d,
                            	0,
                            	0,
                            	&vl_tdp_fcd,
                            	&vl_validite_fcd,
                            	&vl_validite_faible)!= XDC_OK)
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"XZAD23_Calculer_TdP_PMV_Dest: pb appel a XZAO962_Calcul_TDP_FCD");
        return (XDC_NOK);
        }
    
  /*Si vl_validite_fcd >= 60% et que toutes les données ont une validite > 20% (vl_validite_faible = 0) alors on utilise le temps de parcous de la procedure XZAO962*/
  if((vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0))
    {		
        *pa_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi à la minute superieure*/
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD23_Calculer_TdP_PMV_Dest: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d validite tdp_fcd>%d. Tdp=%d",vl_autoroute_d, va_pr_pmv,vl_validite_fcd,vl_validite_faible,XDC_SEUIL_AFFICHAGE_TDP_FCD, vl_tdp_fcd);
	XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : OUT");
        return (XDC_OK);
    }
    /*Si vl_validite_fcd < 60%*/
  else
  {
  	if (XZEZ02_Calcul_TDP(	va_autoroute_pmv
				,va_pr_pmv
				,va_sens_pmv
				,va_destination
				,(XZEZT_Liste_Zdp *)pa_zdp
				,(XZEZT_Liste_Destinations *)pg_dest
				,(XZEZT_Liste_Zdp *)pg_zdp
				,&vl_tdp_zdp , va_nom
				) != XDC_OK) {
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD23_Calculer_TdP_PMV_Dest: pb appel a XZEZ02_Calcul_TDP");				
    	return (XDC_NOK);
  	}

  	*pa_tdp=vl_tdp_zdp;

  	/*Tdp en fonction du tdp issu de XZEZ02_Calcul_TDP*/
	XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : XZEZ02 TDP = %d",vl_tdp_zdp);
  	if (*pa_tdp<=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA) // Si le tdp vaut -1 et si la validite issue de XZAO962 >=40% alors on recupere le tdp issu de XZAO962*/
  	{
		*pa_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi à la minute superieure*/
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAD23_Calculer_TdP_PMV_Dest: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d, validite tdp_fcd>%d & ESCOTA tdp<=0. Tdp=%d",vl_autoroute_d, va_pr_pmv,vl_validite_fcd,vl_validite_faible,XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA, pa_tdp);
		XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : OUT");				
        	return (XDC_OK);
  	}
  	else // Sinon on utilise le tdp issu de XZEZ02_Calcul_TDP
  	{	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD23_Calculer_TdP_PMV_Dest: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d. Tdp ESCCOTA=%d",vl_autoroute_d, va_pr_pmv,vl_validite_fcd,vl_validite_faible, pa_tdp);
	  	XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : OUT");
	  	return (XDC_OK);
  	}
  }
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* met a jour l'etat d'un franchissement de seuil
* pour une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD24_Maj_Franchissement(	va_numero,
				va_type_franchissement)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD24_Maj_Franchissement(	XDY_Entier	va_numero,
					XDY_Octet	va_type_franchissement)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD24_Maj_Franchissement : IN");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAD24_Maj_Franchissement: num zdp %d, franchissement %d",
					va_numero,
					va_type_franchissement);

  /*on se place au debut de la liste chainee*/
  pg_zdp_courant=pg_zdp;

  /*parcourt cette liste*/
  while (pg_zdp_courant!=NULL) {
    /*si je suis sur la bonne zdp*/
    if (pg_zdp_courant->zdp.Numero==va_numero) {
      /*mise a jour du type de franchissement*/
      pg_zdp_courant->zdp.Franchissement_Seuil = va_type_franchissement;

      /*si ce n'est pas une zdp sur fourche*/
      if (pg_zdp_courant->zdp.Type_Zone==XDC_ZDP_TDP_SECTION)
        return (XDC_OK);
      else
        goto pos_fourche;
    }
    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  /*si je suis en fin de liste*/
  if (pg_zdp_courant==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZAD24_Maj_Franchissement: la Zdp %d n'existe pas dans la liste",
				va_numero);
    return (XDC_NOK);
  }

  /*met egalement a jour la liste des zdp fourche*/
pos_fourche:
  pg_zdp_courant=pg_zdp_fourche;
  while (pg_zdp_courant!=NULL) {
    /*si je suis sur la bonne zdp*/
    if (pg_zdp_courant->zdp.Numero==va_numero) {
      /*mise a jour du type de franchissement*/
      pg_zdp_courant->zdp.Franchissement_Seuil = va_type_franchissement;

      /*trace*/
      XZST_03EcritureTrace(XZSTC_WARNING,
                                "XZAD24_Maj_Franchissement: mise a jour du franchissement de seuil de la zdp fourche %d",
                                va_numero);
      return (XDC_OK);
    }
    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD24_Maj_Franchissement : OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche les zones perturbees et revenues a la 
* normale et appelle la fonction utilisateur
* pour les PMV
******************************************************
* SEQUENCE D'APPEL :
* XZAD26_Calculer_Zones_PMV(	va_numero_fmc,
				va_cle_fmc,
				pa_fct_util,
				va_pr,
				va_prtete,
				va_sens,
				va_type_fmc)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD26_Calculer_Zones_PMV(	XDY_Entier 	va_numero_fmc,
					XDY_Mot		va_cle_fmc,
					XDY_Fonction	pa_fct_util,
					XDY_PR		va_pr,
					XDY_PR		va_prtete,
					XDY_Sens	va_sens,
					XDY_Octet	va_type_fmc)
{
  XZADT_Liste_Zdp		*pl_liste_zdp;
  XZADT_Liste_Zdp		*pl_liste_zdp_fourche;
  XZADT_Liste_Destinations	*pl_liste_dest;
  XDY_Destination_SAD		vl_dest;
  XDY_Destination_SAD		vl_dest_fourche;
  XDY_PR			vl_pr=NULL;
  XDY_PR			vl_pr_alternat=NULL;
  XDY_ZDP			vl_zdp;
  XDY_ZDP			vl_zdp_fourche;
  XDY_ZDP			vl_zdp_amont;
  XDY_PR			vl_pr_debut=NULL;
  int				vl_first=TRUE;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD26_Calculer_Zones_PMV: IN");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAD26_Calculer_Zones_PMV :num fmc %d, cle fmc %d, pr %d, prtete %d",
					va_numero_fmc,
					va_cle_fmc,
					va_pr,
					va_prtete);

  pl_liste_zdp=pg_zdp;

  /* la liste est triee par autoroute, sens, PR*/
  while (pl_liste_zdp!=NULL)  {
    /*on commence par les zdp sur section courante*/
    if ( (pl_liste_zdp->zdp.Type_Zone==XDC_ZDP_TDP_SECTION)           &&
	 (pl_liste_zdp->zdp.Franchissement_Seuil==XDC_SEUIL_TDP_HAUT) &&
	 ( (va_type_fmc==XZAEC_FMC_PosteOperateur) ||
	   ( (pl_liste_zdp->zdp.Sens == va_sens)   &&
	     (pl_liste_zdp->zdp.PR_Debut <= va_prtete) &&
	     (pl_liste_zdp->zdp.PR_Fin >= va_prtete)
	   )
	 )
       ) {

      XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZAD26_Calculer_Zones_PMV: la zdp %d concerne l'evenement",
			pl_liste_zdp->zdp.Numero);

      vl_zdp=pl_liste_zdp->zdp;

      if (vl_zdp.Sens==XDC_SENS_SUD)
	vl_pr_debut=vl_zdp.PR_Debut;
      else
	vl_pr_debut=vl_zdp.PR_Fin;

      /*sur fiche operateur*/
      if (va_type_fmc==XZAEC_FMC_PosteOperateur) {
	/*creer une zdp_pmv pour chaque zdp perturbe sur section courante*/
	vl_zdp_amont=vl_zdp;
	goto pos_zdp_operateur;
      }
      else { /*ne traiter que la zdp ou se situe la fmc*/
        if (vl_first) {
	  vl_zdp_amont=vl_zdp;
	  vl_first=FALSE;
        }
        else { /*ne conserver que la zdp amont*/
          if (vl_zdp.Sens==XDC_SENS_SUD)
	    if (vl_zdp.PR_Debut < vl_zdp_amont.PR_Debut)
	      vl_zdp_amont=vl_zdp;
	  else if (vl_zdp.Sens==XDC_SENS_NORD)
	    if (vl_zdp.PR_Fin > vl_zdp_amont.PR_Fin)
	      vl_zdp_amont=vl_zdp;
        }
      }
    }
pos_boucle:
    pl_liste_zdp = pl_liste_zdp->suivant ;
  }

pos_zdp_operateur:
   vl_zdp=vl_zdp_amont;
   if (vl_pr_debut!=NULL) { /*si j'ai trouve une zdp sur section courante*/
      /*recherche de la destination associee a la zdp*/
      if (XZAD_recherche_dest_zdp(vl_zdp,&vl_dest)==XDC_OK)
 	vl_pr=vl_dest.PR;
      else /*pas de destination*/
        vl_pr=NULL;

      /*si j'ai une destination*/
      if (vl_pr!=NULL) {
        /*recherche de l'eventuelle zone fourche associe a la zdp sur section courante*/
        if (vl_zdp.Type_Zone==XDC_ZDP_TDP_SECTION) {
	  pl_liste_zdp_fourche=pg_zdp_fourche;

	  vl_zdp_fourche.PR_Debut=NULL;
	  vl_zdp_fourche.Numero=NULL;

          while (pl_liste_zdp_fourche!=NULL) {
            if (   (pl_liste_zdp_fourche->zdp.ZDP_Section==vl_zdp.Numero)
		&& (pl_liste_zdp_fourche->zdp.Franchissement_Seuil==XDC_SEUIL_TDP_HAUT) ) {
  	      vl_zdp_fourche=pl_liste_zdp_fourche->zdp;
	      pl_liste_zdp_fourche->zdp.Traitee=TRUE;

	      XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZAD26_Calculer_Zones_PMV: fourche %d associee a zdp %d",
			vl_zdp_fourche.Numero,
			vl_zdp.Numero);
              break;
            }
            pl_liste_zdp_fourche=pl_liste_zdp_fourche->suivant;
          }

	  /*si j'ai trouve une zdp fourche perturbee associee a la zdp sur section courante*/
	  if (pl_liste_zdp_fourche!=NULL) { 
	    /*recherche de la destination associee a la zone fourche*/
	    if (XZAD_recherche_dest_zdp(vl_zdp_fourche,&vl_dest_fourche)==XDC_OK) {
  	      if (vl_zdp.Sens==XDC_SENS_SUD)
	        vl_pr_alternat=vl_zdp_fourche.PR_Debut;
              else
	        vl_pr_alternat=vl_zdp_fourche.PR_Fin;
	    }
	    else { /*pas de destination pour la zdp fourche associee*/
	      vl_pr_alternat=NULL;
	      vl_dest_fourche.Numero=NULL;
	    }
   	  }
	  else { /*pas de zdp fourche associee*/
            vl_pr_alternat=NULL;
            vl_dest_fourche.Numero=NULL;
	    XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZAD26_Calculer_Zones_PMV: pas de fourche associee a zdp %d",
		vl_zdp.Numero);
          }
	
          /*appel de la fonction utilisateur de creation de l'objet zdp_pmv dans le RTie*/
          (*pa_fct_util)(vl_zdp.Autoroute
		       ,vl_dest.Sens
		       ,vl_zdp.Numero
		       ,vl_dest.Numero
		       ,vl_dest.Type_Localisation
		       ,vl_pr_debut
		       ,vl_pr_alternat
		       ,vl_dest_fourche.Numero);
        }
      }
      else
        XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD26_Calculer_Zones_PMV: pas de destinations pour la zdp %d",
				vl_zdp.Numero);
  }

  /*sur fiche operateur*/
  if ( (va_type_fmc==XZAEC_FMC_PosteOperateur) && (pl_liste_zdp!=NULL) ) {
    /*il faut traiter toutes les zdp perturbees*/
    vl_pr_debut=NULL;

    XZST_03EcritureTrace(XZSTC_FONCTION,
	"XZAD26_Calculer_Zones_PMV: on traite la zdp suivante");
    goto pos_boucle;
  }

  /*je traite maintenant les zdp sur fourche qui n'ont pas de zdp sur section courante associee*/
  pl_liste_zdp=pg_zdp_fourche;

  vl_pr_debut=NULL;
  while (pl_liste_zdp!=NULL) {
    if ( (pl_liste_zdp->zdp.Traitee==FALSE) &&
         (pl_liste_zdp->zdp.Franchissement_Seuil==XDC_SEUIL_TDP_HAUT) &&
         ( (va_type_fmc==XZAEC_FMC_PosteOperateur) ||
           ( (pl_liste_zdp->zdp.Sens == va_sens)   &&
             (pl_liste_zdp->zdp.PR_Debut <= va_prtete) &&
             (pl_liste_zdp->zdp.PR_Fin >= va_prtete)
           )
         )
       ) {
      vl_zdp=pl_liste_zdp->zdp;

      XZST_03EcritureTrace(XZSTC_FONCTION,
	"XZAD26_Calculer_Zones_PMV: traite fourche pas associee %d",vl_zdp.Numero);

      if (vl_zdp.Sens==XDC_SENS_SUD)
        vl_pr_debut=vl_zdp.PR_Debut;
      else
        vl_pr_debut=vl_zdp.PR_Fin;

      /*recherche de la destination associee a la zdp*/
      if (XZAD_recherche_dest_zdp(vl_zdp,&vl_dest)==XDC_OK) {
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZAD26_Calculer_Zones_PMV: dest de la fourche = %d",vl_dest.Numero);

 	/*cree l'objet zdp_pmv si j'ai une destination*/
        (*pa_fct_util)(vl_zdp.Autoroute
                     ,vl_dest.Sens
                     ,vl_zdp.Numero
                     ,vl_dest.Numero
                     ,vl_dest.Type_Localisation
                     ,vl_pr_debut
                     ,NULL
                     ,NULL);
      }
      else
        XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD26_Calculer_Zones_PMV: pas de destinations pour la zdp %d",
	            vl_zdp.Numero);
    }
    else 
      pl_liste_zdp->zdp.Traitee=FALSE;
    pl_liste_zdp=pl_liste_zdp->suivant;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD26_Calculer_Zones_PMV: OUT");

  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD28_Libelle_Destination(va_dest,pa_libelle);
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int XZAD27_Libelle_Destination(XDY_Entier	va_dest,
					char		*pa_libelle)
{
  XZADT_Liste_Destinations	*pl_dest;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD28_Libelle_Destination: IN");

  pl_dest=pg_dest;

  while (pl_dest!=NULL) {
    if (pl_dest->destination.Numero==va_dest)  {
      strcpy(pa_libelle,pl_dest->destination.Libelle_PMV);
      return (XDC_OK);
    }
    pl_dest=pl_dest->suivant;
  }
  /*si on est ici, on n'a pas trouve la destination*/
  XZST_03EcritureTrace(XZSTC_WARNING,"XZAD28_Libelle_Destination: pas de destination %d",va_dest);
  return (XDC_NOK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD30_Libere_Ressources_Calcul_TdP();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD30_Libere_Ressources_Calcul_TdP()
{
  XZADT_Liste_Zdp 		*pl_ex_zdp;
  XZADT_Liste_Destinations 	*pl_ex_dest;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Libere_Ressources_Calcul_TdP: IN");

  if (pg_zdp!=NULL) {
    pg_zdp_courant = pg_zdp;

    while (pg_zdp_courant!=NULL) {
      pl_ex_zdp=pg_zdp_courant;

      pg_zdp_courant = pg_zdp_courant->suivant;

      free(pl_ex_zdp);
    }
    pg_zdp=NULL;
  }

  if (pg_dest!=NULL) {
    pg_dest_courant=pg_dest;

    while (pg_dest_courant!=NULL) {
      pl_ex_dest=pg_dest_courant;

      pg_dest_courant=pg_dest_courant->suivant;

      free(pl_ex_dest);
    }
    pg_dest=NULL;
  }

  if (pg_zdp_fourche!=NULL) {
    pg_zdp_courant = pg_zdp_fourche;

    while (pg_zdp_courant!=NULL) {
      pl_ex_zdp=pg_zdp_courant;

      pg_zdp_courant = pg_zdp_courant->suivant;

      free(pl_ex_zdp);
    }
    pg_zdp_fourche=NULL;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Libere_Ressources_Calcul_TdP: OUT");
  return (XDC_OK);
}



/*X*/
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
* XZAD_ajouter_zdp(va_zdp,pa_resultat)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

int XZAD_ajouter_zdp(	int 		va_nb_zdp,
			int		va_indice,
			XDY_ZDP 	va_zdp,
			int 		*pa_resultat)
{
  XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAD_ajouter_zdp: ajout zdp numero %d",va_zdp.Numero);

  /*premiere insertion*/
  if (pg_zdp==NULL) {
    if ((pg_zdp=(XZADT_Liste_Zdp *)malloc(sizeof(XZADT_Liste_Zdp)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_zdp: pb de memoire pour le malloc debut de liste");
      return (XDC_NOK);
    }

    /*valorise la zdp*/
    pg_zdp->zdp=va_zdp;
    pg_zdp->zdp.Franchissement_Seuil=XDC_SEUIL_TDP_ABSENT;
    pg_zdp->suivant=NULL;
    pg_zdp_courant=pg_zdp;
  }
  else { /*inserer en fin de chaine*/
    /*on est en fin de chaine*/
    if ((pg_zdp_courant->suivant=(XZADT_Liste_Zdp *)malloc(sizeof(XZADT_Liste_Zdp)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_zdp: pb de memoire pour le malloc fin de liste");
      return (XDC_NOK);
    }
    
    /*valorise la zdp*/
    pg_zdp_courant=pg_zdp_courant->suivant;
    pg_zdp_courant->zdp=va_zdp;
    pg_zdp_courant->zdp.Franchissement_Seuil=XDC_SEUIL_TDP_ABSENT;
    pg_zdp_courant->suivant=NULL;
  }

  if (va_zdp.Type_Zone==XDC_ZDP_TDP_SECTION)
    return (XDC_OK);

  if (pg_zdp_fourche==NULL) {
    if ((pg_zdp_fourche=(XZADT_Liste_Zdp *)malloc(sizeof(XZADT_Liste_Zdp)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_zdp: pb de memoire pour le malloc debut de liste");
      return (XDC_NOK);
    }

    /*valorise la zdp*/
    pg_zdp_fourche->zdp=va_zdp;
    pg_zdp_fourche->zdp.Traitee=FALSE;
    pg_zdp_fourche->zdp.Franchissement_Seuil=XDC_SEUIL_TDP_ABSENT;
    pg_zdp_fourche->suivant=NULL;
    pg_zdp_fourche_courant=pg_zdp_fourche;
  }
  else { /*inserer en fin de chaine*/
    /*on est en fin de chaine*/
    if ((pg_zdp_fourche_courant->suivant=(XZADT_Liste_Zdp *)malloc(sizeof(XZADT_Liste_Zdp)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_zdp: pb de memoire pour le malloc fin de liste");
      return (XDC_NOK);
    }

    /*valorise la zdp*/
    pg_zdp_fourche_courant=pg_zdp_fourche_courant->suivant;
    pg_zdp_fourche->zdp=va_zdp;
    pg_zdp_fourche->zdp.Franchissement_Seuil=XDC_SEUIL_TDP_ABSENT;
    pg_zdp_fourche->zdp.Traitee=FALSE;
    pg_zdp_fourche->suivant=NULL;
  }

  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO401
* ajoute une dest a la liste des dest
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_ajouter_dest(va_dest,pa_resultat)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

int XZAD_ajouter_dest(	XDY_Destination_SAD 	va_dest,
			int 			*pa_resultat)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_ajouter_dest: ajout dest numero %d",va_dest.Numero);

  /*premiere insertion*/
  if (pg_dest==NULL) {
    if ((pg_dest=(XZADT_Liste_Destinations *)malloc(sizeof(XZADT_Liste_Destinations)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_dest: pb de memoire pour le malloc debut de liste");
      return (XDC_NOK);
    }

    /*valorise la dest*/
    pg_dest->destination=va_dest;
    pg_dest->suivant=NULL;
    pg_dest_courant=pg_dest;
  }
  else { /*inserer en fin de chaine*/
    if ((pg_dest_courant->suivant=(XZADT_Liste_Destinations *)malloc(sizeof(XZADT_Liste_Destinations)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_dest: pb de memoire pour le malloc fin de liste");
      return (XDC_NOK);
    }
    
    /*valorise la dest*/
    pg_dest_courant=pg_dest_courant->suivant;
    pg_dest_courant->destination=va_dest;
    pg_dest_courant->suivant=NULL;
  }

  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO463
* ajoute un pmv et ses dest a la liste des pmv
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_ajouter_dest_pmv(va_dest,pa_resultat)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
//TODO
int XZAD_ajouter_dest_pmv(	XZAOT_PMV_DEST_TDP 	va_dest_pmv,
			int 			*pa_resultat)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_ajouter_dest_pmv: ajout dest numero %d",va_dest_pmv.NumeroPMV);

  /*premier maillon*/
  /* premiere insertion de la structure */
  if (pg_dest_pmv==NULL) {
    if ((pg_dest_pmv=(XZAOT_Liste_PMV_Destinations *)malloc(sizeof(XZAOT_Liste_PMV_Destinations)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_dest_pmv: pb de memoire pour le malloc debut de liste");
      return (XDC_NOK);
    }

    /*valorise la dest*/
    pg_dest_pmv->destination_pmv=va_dest_pmv;
    pg_dest_pmv->suivant=NULL;
    pg_dest_pmv_courant=pg_dest_pmv;
  }
  else { /*inserer en fin de chaine*/
    if ((pg_dest_pmv_courant->suivant=(XZAOT_Liste_PMV_Destinations *)malloc(sizeof(XZAOT_Liste_PMV_Destinations)))==NULL) {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_ajouter_dest_pmv: pb de memoire pour le malloc fin de liste");
      return (XDC_NOK);
    }
    
    /*valorise la dest*/
    pg_dest_pmv_courant=pg_dest_pmv_courant->suivant;
    pg_dest_pmv_courant->destination_pmv=va_dest_pmv;
    pg_dest_pmv_courant->suivant=NULL;
  }

  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* abonnement au TDP pour les ZDP d'un site
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD27_Abonnement_TDP(	va_site,
				pa_fct_util,
				va_datagroup)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD27_Abonnement_TDP(	XDY_Mot		va_site,
					XDY_Fonction	pa_fct_util,
					XDY_Datagroup  	va_datagroup)
{
  int			vl_cr;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD27_Abonnement_TDP : IN");

  pg_zdp_courant=pg_zdp;;

  /*appel XZEA35*/
  while (pg_zdp_courant!=NULL) {
    if (XZEA35_Abt_TdP_6mn_Zone(  XZEAC_DEBUT_ABONNEMENT,
					va_datagroup,
					pa_fct_util,
					pg_zdp_courant->zdp.Numero,
					&vl_cr) != XDC_OK)
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAD27_Abonnement_TDP: pb dans l'appel a XZEA35_Abt_TdP_6mn_Zone");
    pg_zdp_courant=pg_zdp_courant->suivant;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD27_Abonnement_TDP :  OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* verifie qu'un PMV est sur une ZDP
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD28_PMV_Sur_ZDP(        va_autoroute_pmv,
                                va_sens_pmv,
                                va_pr_pmv)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int XZAD28_PMV_Sur_ZDP(  XDY_Autoroute va_autoroute_pmv,
                                XDY_Sens va_sens_pmv,
                                XDY_PR va_pr_pmv)
{
  XZADT_Liste_Zdp		*pl_liste_zdp;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD28_PMV_Sur_ZDP : IN aut %d pr %d sens %d",
			va_autoroute_pmv,
			va_pr_pmv,
			va_sens_pmv);

  pl_liste_zdp=pg_zdp;

  while (pl_liste_zdp!=NULL)  {
    if ( (pl_liste_zdp->zdp.Autoroute == va_autoroute_pmv )  &&
	 (pl_liste_zdp->zdp.Sens == va_sens_pmv )	     &&
	 (pl_liste_zdp->zdp.PR_Debut <= va_pr_pmv )	     &&
	 (pl_liste_zdp->zdp.PR_Fin >= va_pr_pmv )            
       ) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD28_PMV_Sur_ZDP: le PMV est sur la zdp %d",
				pl_liste_zdp->zdp.Numero);
      return (XDC_OK);
    }
    pl_liste_zdp = pl_liste_zdp->suivant;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD28_PMV_Sur_ZDP: le PMV n'est sur aucune zdp");
  return (XDC_NOK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche la destination associee a une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_recherche_dest_zdp(va_zdp,pa_dest)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

int XZAD_recherche_dest_zdp(XDY_ZDP         va_zdp, XDY_Destination_SAD       *pa_dest)
{
  XZADT_Liste_Destinations *pl_liste_dest;
  XDY_PR		vl_pr=0;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_zdp: zdp %d",va_zdp.Numero);

      /*recherche de la destination associee a la zdp*/
      pl_liste_dest=pg_dest;

      while (pl_liste_dest!=NULL) {
        /*sens croissant*/
        if (va_zdp.Sens==XDC_SENS_SUD) {
          if ( (pl_liste_dest->destination.Autoroute==va_zdp.Autoroute) &&
               (pl_liste_dest->destination.Sens==va_zdp.Sens) &&
               (pl_liste_dest->destination.PR>=va_zdp.PR_Fin) &&
               ((pl_liste_dest->destination.PR<vl_pr) || (vl_pr==0)) ) {
            *pa_dest=pl_liste_dest->destination;
            vl_pr=pl_liste_dest->destination.PR;
          }
        }
        else { /*sens decroissant*/
          if ( (pl_liste_dest->destination.Autoroute==va_zdp.Autoroute) &&
               (pl_liste_dest->destination.Sens==va_zdp.Sens) &&
               (pl_liste_dest->destination.PR<=va_zdp.PR_Debut) &&
               ((pl_liste_dest->destination.PR>vl_pr) || (vl_pr==0)) )       {
            *pa_dest=pl_liste_dest->destination;
            vl_pr=pl_liste_dest->destination.PR;
          }
        }
        pl_liste_dest=pl_liste_dest->suivant;
      }

  if (vl_pr!=0) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_zdp: dest trouve %d",pa_dest->Numero);
    return (XDC_OK);
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_zdp: pas de destination");
    return (XDC_NOK);
  }
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reinitialise l'etat d'un franchissement de seuil
*  des zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD29_Reinit_Franchissement()
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD29_Reinit_Franchissement()
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD29_Reinit_Franchissement : IN");

  /*on se place au debut de la liste chainee*/
  pg_zdp_courant=pg_zdp;

  /*parcourt cette liste*/
  while (pg_zdp_courant!=NULL) {
    pg_zdp_courant->zdp.Franchissement_Seuil = XDC_SEUIL_TDP_ABSENT;
    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  pg_zdp_courant=pg_zdp_fourche;
  while (pg_zdp_courant!=NULL) {
    /*mise a jour du type de franchissement*/
    pg_zdp_courant->zdp.Franchissement_Seuil = XDC_SEUIL_TDP_ABSENT;
    pg_zdp_courant=pg_zdp_courant->suivant;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD29_Reinit_Franchissement : OUT");
  return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche la destination associee a une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_recherche_dest_fmc(va_zdp,pa_dest)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD_recherche_dest_fmc(XDY_Autoroute va_autoroute,XDY_PR va_pr_debut, 
		XDY_PR va_pr_fin, XDY_Sens va_sens, XDY_Destination_SAD       *pa_dest)
{
  XZADT_Liste_Destinations *pl_liste_dest;
  XDY_PR		vl_pr=0;

      /*recherche de la destination associee a la fmc*/
      pl_liste_dest=pg_dest;

  if (va_autoroute!=1) {
     XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_fmc: pas de destination pour PMV as sur A8");
     return (XDC_NOK);
  }
  /*
  if ((va_sens==XDC_SENS_SUD) && (va_pr_debut < 150000)) {
     XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_fmc: pas de destination pour PMV en amont de 150.000");
     return (XDC_NOK);
  }

  if ((va_sens==XDC_SENS_NORD) && (va_pr_debut > 210000)) {
     XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_fmc: pas de destination pour PMV en amont de 210.000");
     return (XDC_NOK);
  }*/
		
      while (pl_liste_dest!=NULL) {
        /*sens croissant*/
        if (va_sens==XDC_SENS_SUD) {
          if ( (pl_liste_dest->destination.Autoroute==va_autoroute) &&
               (pl_liste_dest->destination.Sens==va_sens) &&
               (pl_liste_dest->destination.PR>=va_pr_fin+XDC_DISTANCE_TDP) &&
               ((pl_liste_dest->destination.PR<vl_pr) /*|| (vl_pr==0)*/) ) {
            *pa_dest=pl_liste_dest->destination;
            vl_pr=pl_liste_dest->destination.PR;
          }
        }
        else { /*sens decroissant*/
          if ( (pl_liste_dest->destination.Autoroute==va_autoroute) &&
               (pl_liste_dest->destination.Sens==va_sens) &&
               (pl_liste_dest->destination.PR<=va_pr_debut-XDC_DISTANCE_TDP) &&
               ((pl_liste_dest->destination.PR>vl_pr) || (vl_pr==0)) )       {
            *pa_dest=pl_liste_dest->destination;
            vl_pr=pl_liste_dest->destination.PR;
          }
        }
        pl_liste_dest=pl_liste_dest->suivant;
      }

  if (vl_pr!=0) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_fmc: dest trouve %d",pa_dest->Numero);
    return (XDC_OK);
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD_recherche_dest_fmc: pas de destination");
    return (XDC_NOK);
  }
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche la destination associee au PMV
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_recherche_dest_pmv(va_numero)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD_recherche_dest_pmv(XDY_Entier va_numero, XZADT_CONFIG_PMV *pa_PMV
			)
{

  int vl_trouve;
  XZADT_CONFIG_PMV	*pl_PMV;
  
  XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmv : pmv %d",va_numero);
  /*recherche le PMV*/
  vl_trouve=0;
  for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  )
  {
	  if ( va_numero == pl_PMV->Numero )
	  {
		  vl_trouve=1;
		  XZST_03EcritureTrace ( XZSTC_FONCTION,"PMV num %d, dest %d %d %d",
				pl_PMV->Numero,
				pl_PMV->Destination,
				pl_PMV->DestinationLointaine,
				pl_PMV->DestinationNoeud);
		  break;
	  }
  }
  if (vl_trouve==0) {
    XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmv : pas de PMV");
    pa_PMV=NULL;
    return (XDC_NOK);
  }
                memcpy ( pa_PMV, pl_PMV, sizeof (XZADT_CONFIG_PMV) );
  return (XDC_OK);
}


static int XZAD_creer_liste_Destination_pmv ( XDY_Eqt va_PMV, XDY_Mot va_Destination,
                                        XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
                                        int             va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*       Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
 XZADT_CONFIG_PMV *    pl_PMV = NULL;
 XZADT_CONFIG_PMV *    pl_NewPMV = NULL;
 XZEZT_Liste_Zdp	*pl_NewZDP = NULL;
 XZADT_Liste_Zdp *pl_zdp;
 int vl_trouve;

  XZST_03EcritureTrace ( XZSTC_DEBUG3,"XZAD_creer_liste_Destination_pmv : pmv=%d, dest=%d, type=%d, zdp=%d",
                                        va_PMV,va_Destination,va_TypeDestination,va_zdp);

  /*recherche la zdp*/
  vl_trouve = 0;
  for (pl_zdp = pg_zdp; pl_zdp!=NULL; pl_zdp = pl_zdp->suivant) {
    if (pl_zdp->zdp.Code_Zone == va_zdp) {
        vl_trouve=1;
        break;
    }
  }
  if (vl_trouve==0) {
        XZST_03EcritureTrace ( XZSTC_WARNING,"ZDP non trouve %d",va_zdp);
        return (XDC_OK);
  }

  /*recherche le PMV*/
  vl_trouve=0;
   for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  )
   {
      if ( va_PMV == pl_PMV->Numero )
        {
		vl_trouve=1;
		break;
	}
  }

  if (vl_trouve==0) {
     /*A Allocation memoire pour la nouvelle structure */
    if ( ( pl_NewPMV = ( XZADT_CONFIG_PMV *) malloc
				(sizeof(XZADT_CONFIG_PMV))) == NULL) {
	XZST_03EcritureTrace ( XZSTC_WARNING,
		" OUT: ec_creer_liste_Destination_pmv Allocation memoire impossible");
	return XDC_NOK;	
	}
    pl_NewPMV->Numero = va_PMV;
    pl_NewPMV->suivant = pg_ListePMV;
    pl_NewPMV->Graphe_Destination=NULL;
    pl_NewPMV->Graphe_DestinationLointaine=NULL;
    pl_NewPMV->Graphe_DestinationNoeud=NULL;
    pl_NewPMV->Destination=0;
    pl_NewPMV->DestinationLointaine=0;
    pl_NewPMV->DestinationNoeud=0;
    pg_ListePMV = pl_NewPMV;
    pl_PMV  = pl_NewPMV;
  }
 
                /*XZST_03EcritureTrace ( XZSTC_DEBUG2,"ajout zdp %d pour pmv %d",
                                        va_zdp,va_PMV);*/
                /*A Allocation memoire pour la nouvelle structure */
                if ( ( pl_NewZDP = (XZEZT_Liste_Zdp *) malloc
                                ( sizeof(XZEZT_Liste_Zdp) ) ) == NULL)
                {
                  /* Ecriture trace */
                        XZST_03EcritureTrace ( XZSTC_WARNING,
                                " OUT: ec_creer_liste_Destination_pmv Allocation memoire impossible");
                  return XDC_NOK;
                }
                memcpy ( &(pl_NewZDP->zdp), pl_zdp, sizeof (XDY_ZDP) );
                pl_NewZDP->suivant= NULL;

		XZST_03EcritureTrace ( XZSTC_DEBUG2,"type dest %d, dest %d",
					va_TypeDestination,va_Destination);
                switch (va_TypeDestination) {
                        case XDC_DESTINATION_PROCHE :
                                        pl_PMV->Destination=va_Destination;
                                        pl_NewZDP->suivant = pl_PMV->Graphe_Destination;
                                        pl_PMV->Graphe_Destination=pl_NewZDP;
					break;

			case XDC_DESTINATION_LOINTAINE :
                                        pl_PMV->DestinationLointaine=va_Destination;
                                        pl_NewZDP->suivant = pl_PMV->Graphe_DestinationLointaine;
                                        pl_PMV->Graphe_DestinationLointaine=pl_NewZDP;
                                        break;

                        case XDC_DESTINATION_NOEUD :
                                        pl_PMV->DestinationNoeud=va_Destination;
                                        pl_NewZDP->suivant = pl_PMV->Graphe_DestinationNoeud;
                                        pl_PMV->Graphe_DestinationNoeud=pl_NewZDP;
                                        break;
                }


   /*
   for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  )
	{
		XZST_03EcritureTrace ( XZSTC_INFO,"dest %d %d %d",
			pl_PMV->Destination,
			pl_PMV->DestinationLointaine,
			pl_PMV->DestinationNoeud);
	}	
	  */
          return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche la destination associee au PMV
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD_recherche_dest_pmv(va_numero)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD_recherche_dest_pmva(XDY_Entier va_numero, XZADT_CONFIG_PMA *pa_PMA, XDY_Sens va_sens,
			XDY_Autoroute va_autoroute, XDY_PR va_pr
			)
{
   XZADT_CONFIG_PMA *pl_PMA;
   XZADT_Liste_Destinations      *pl_dest;
   XZADT_Liste_Destinations      *pl_dest_un_proche=NULL;
   XZADT_Liste_Destinations      *pl_dest_deux_proche=NULL;
   XZADT_Liste_Destinations      *pl_dest_un_lointain=NULL;
   XZADT_Liste_Destinations      *pl_dest_deux_lointain=NULL;
  int vl_trouve;

  XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmva pma %d autoroute %d sens %d PR %d",
				(int) va_numero, (int) va_autoroute, (int) va_sens, (int) va_pr);

  /*recherche le PMV*/
  vl_trouve=0;
  for ( pl_PMA = pg_ListePMA; pl_PMA != NULL; pl_PMA = pl_PMA->suivant  )
  {
/*	  XZST_03EcritureTrace ( XZSTC_FONCTION,"pma %d liste %d destination %d",
				va_numero, pl_PMA->Numero, pl_PMA->Dest_un_proche);*/
	  if ( va_numero == pl_PMA->Numero )
	  {
		  vl_trouve=1;
		  break;
	  }
  }
  if (vl_trouve==0) {
    XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmva : pas de PMVA");
    pa_PMA=NULL;
    return (XDC_NOK);
  }
  pl_dest=pg_dest;
  vl_trouve=0;

  while (pl_dest!=NULL) {XZST_03EcritureTrace ( XZSTC_DEBUG1,"pl_des tpmv: %d %d %d %d %d %",pl_PMA->Numero,pl_PMA->Dest_un_proche,pl_PMA->Dest_un_loin,pl_PMA->Dest_deux_proche,pl_PMA->Dest_deux_loin,pl_dest->destination.Numero);
    if (pl_dest->destination.Numero==pl_PMA->Dest_un_proche)  {
      pl_dest_un_proche = pl_dest;
      vl_trouve=1; }
    else if (pl_dest->destination.Numero==pl_PMA->Dest_deux_proche)  {
      pl_dest_deux_proche = pl_dest;
      vl_trouve=1; }
    else if (pl_dest->destination.Numero==pl_PMA->Dest_un_loin)  {
      pl_dest_un_lointain = pl_dest;
      vl_trouve=1; }
    else if (pl_dest->destination.Numero==pl_PMA->Dest_deux_loin)  {
      pl_dest_deux_lointain = pl_dest;
      vl_trouve=1; }
    pl_dest=pl_dest->suivant;
  }

  /*on traite une perturbation en sens 1*/
  /*XZST_03EcritureTrace ( XZSTC_FONCTION,"SENS %d %d %d %d %d",va_sens,pl_dest_un_proche->destination.Numero,pl_dest_un_lointain->destination.Numero,pl_dest_deux_proche->destination.Numero,pl_dest_deux_lointain->destination.Numero);*/
  if ((va_sens == XDC_SENS_SUD) && (va_autoroute != 8 ) && (vl_trouve==1) ) {
     /*si autoroute differente, on prend la destination un proche*/
     if (pl_dest_un_proche!=NULL) {
       XZST_03EcritureTrace ( XZSTC_FONCTION,"LCL %d",pl_dest_un_proche->destination.Numero);
       XZST_03EcritureTrace ( XZSTC_FONCTION,"%d %d",pl_dest_un_proche->destination.Autoroute,va_autoroute);
       if (pl_dest_un_proche->destination.Autoroute != va_autoroute) {
  	  pa_PMA->Dest_un_proche = pl_PMA->Dest_un_proche;
  	  pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_proche;
	  return (XDC_OK);
       }
     
       /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
       XZST_03EcritureTrace ( XZSTC_FONCTION,"%d %d",pl_dest_un_proche->destination.PR, va_pr);
       if (pl_dest_un_proche->destination.PR >= va_pr) {
  	  pa_PMA->Dest_un_proche = pl_PMA->Dest_un_proche;
  	  pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_proche;
	  return (XDC_OK);
       }
      
       /*si la dest un proche est trop pres, regarder pour la dest un lointaine*/
       if (pl_dest_un_lointain != NULL) {
         if (pl_dest_un_lointain->destination.Autoroute != va_autoroute) {
  	    pa_PMA->Dest_un_proche = pl_PMA->Dest_un_loin;
  	    pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_Loin;
	    return (XDC_OK);
         }
     
         /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
         if (pl_dest_un_lointain->destination.PR >= va_pr) {
  	    pa_PMA->Dest_un_proche = pl_PMA->Dest_un_loin;
  	    pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_Loin;
	    return (XDC_OK);
         }
       }
  	XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmva out : NOK3");
       return (XDC_NOK);
     }
  	XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmva out : NOK2");
     return (XDC_NOK);
  }
  else if (vl_trouve==1) {
    /*si autoroute differente, on prend la destination un proche*/
     if (pl_dest_deux_proche!=NULL) {
       XZST_03EcritureTrace ( XZSTC_FONCTION,"LCL %d",pl_dest_deux_proche->destination.Numero);
       XZST_03EcritureTrace ( XZSTC_FONCTION,"%d %d",pl_dest_deux_proche->destination.Autoroute,va_autoroute);
       if ((pl_dest_deux_proche->destination.Autoroute != va_autoroute) && (va_autoroute!=8)) {
          pa_PMA->Dest_un_proche = pl_PMA->Dest_deux_proche;
          pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_deux_proche;
          return (XDC_OK);
       }

       /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
     	XZST_03EcritureTrace ( XZSTC_FONCTION,"%d %d",pl_dest_deux_proche->destination.PR, va_pr);
       if (pl_dest_deux_proche->destination.PR <=  va_pr) {
         pa_PMA->Dest_un_proche = pl_PMA->Dest_deux_proche;
          pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_deux_proche;
          return (XDC_OK);
       }

       /*si la dest un proche est trop pres, regarder pour la dest un lointaine*/
       if (pl_dest_deux_lointain != NULL) {
	 if ((pl_dest_deux_lointain->destination.Autoroute != va_autoroute) && (va_autoroute!=8)){
            pa_PMA->Dest_un_proche = pl_PMA->Dest_deux_loin;
            pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_deux_loin;
            return (XDC_OK);
         }

       /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
       if (pl_dest_deux_lointain->destination.PR  <=  va_pr) {
            pa_PMA->Dest_un_proche = pl_PMA->Dest_deux_loin;
            pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_deux_loin;
            return (XDC_OK);
         }


       } 
     }     if (pl_dest_un_proche!=NULL) {
       XZST_03EcritureTrace ( XZSTC_FONCTION,"LCL sens 2 %d",pl_dest_un_proche->destination.Numero);
       XZST_03EcritureTrace ( XZSTC_FONCTION,"sens 2 %d %d",pl_dest_un_proche->destination.Autoroute,va_autoroute);
       if ((pl_dest_un_proche->destination.Autoroute != va_autoroute) && (va_autoroute!=8)) {
  	  pa_PMA->Dest_un_proche = pl_PMA->Dest_un_proche;
  	  pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_proche;
	  return (XDC_OK);
       }
     
       /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
       XZST_03EcritureTrace ( XZSTC_FONCTION,"sens 2 %d %d",pl_dest_un_proche->destination.PR, va_pr);
       if (pl_dest_un_proche->destination.PR <= va_pr) {
  	  pa_PMA->Dest_un_proche = pl_PMA->Dest_un_proche;
  	  pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_proche;
	  return (XDC_OK);
       }
      
       /*si la dest un proche est trop pres, regarder pour la dest un lointaine*/
       if (pl_dest_un_lointain != NULL) {
         if ((pl_dest_un_lointain->destination.Autoroute != va_autoroute) && (va_autoroute!=8)){
  	    pa_PMA->Dest_un_proche = pl_PMA->Dest_un_loin;
  	    pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_Loin;
	    return (XDC_OK);
         }
     
         /*mm autoroute, il faut prendre la destination en aval du PR en entree*/
         if (pl_dest_un_lointain->destination.PR <= va_pr) {
  	    pa_PMA->Dest_un_proche = pl_PMA->Dest_un_loin;
  	    pa_PMA->Graphe_Dest_un_proche = pl_PMA->Graphe_Dest_un_Loin;
	    return (XDC_OK);
         }
       }
	}
 
  }

  XZST_03EcritureTrace ( XZSTC_FONCTION,"XZAD_recherche_dest_pmva out : NOK1");
  return (XDC_NOK);
}

static int XZAD_creer_liste_Destination_pmva ( XDY_Eqt va_PMA, XDY_Mot va_Destination,
                                        XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
                                        int             va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*       Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
 XZADT_CONFIG_PMA *    pl_PMA = NULL;
 XZADT_CONFIG_PMA *    pl_NewPMA = NULL;
 XZEZT_Liste_Zdp	*pl_NewZDP = NULL;
 XZADT_Liste_Zdp *pl_zdp;
 int vl_trouve;

  XZST_03EcritureTrace ( XZSTC_DEBUG3,"XZAD_creer_liste_Destination_pmva : pmva=%d, dest=%d, type=%d, zdp=%d",
                                        va_PMA,va_Destination,va_TypeDestination,va_zdp);

  /*recherche la zdp*/
  vl_trouve = 0;
  for (pl_zdp = pg_zdp; pl_zdp!=NULL; pl_zdp = pl_zdp->suivant) {
    if (pl_zdp->zdp.Code_Zone == va_zdp) {
        vl_trouve=1;
        break;
    }
  }
  if (vl_trouve==0) {
        XZST_03EcritureTrace ( XZSTC_WARNING,"ZDP non trouve %d",va_zdp);
        return (XDC_OK);
  }

  /*recherche le PMV*/
  vl_trouve=0;
   for ( pl_PMA = pg_ListePMA; pl_PMA != NULL; pl_PMA = pl_PMA->suivant  )
   {
      if ( va_PMA == pl_PMA->Numero )
        {
		vl_trouve=1;
		break;
	}
  }

  if (vl_trouve==0) {
     /*A Allocation memoire pour la nouvelle structure */
    if ( ( pl_NewPMA = ( XZADT_CONFIG_PMA *) malloc
				(sizeof(XZADT_CONFIG_PMA))) == NULL) {
	XZST_03EcritureTrace ( XZSTC_WARNING,
		" OUT: ec_creer_liste_Destination_pma Allocation memoire impossible");
	return XDC_NOK;
	}
    pl_NewPMA->Numero = va_PMA;
    pl_NewPMA->suivant = pg_ListePMA;
    pl_NewPMA->Graphe_Dest_un_proche=NULL;
    pl_NewPMA->Graphe_Dest_un_Loin=NULL;
    pl_NewPMA->Graphe_Dest_deux_proche=NULL;
    pl_NewPMA->Graphe_Dest_deux_loin=NULL;
    pg_ListePMA = pl_NewPMA;
    pl_PMA  = pl_NewPMA;
  }

                /*XZST_03EcritureTrace ( XZSTC_DEBUG3,"ajout zdp %d pour pma %d",
                                        va_zdp,va_PMA);*/
                /*A Allocation memoire pour la nouvelle structure */
                if ( ( pl_NewZDP = (XZEZT_Liste_Zdp *) malloc
                                ( sizeof(XZEZT_Liste_Zdp) ) ) == NULL)
                {
                  /* Ecriture trace */
                        XZST_03EcritureTrace ( XZSTC_WARNING,
                                " OUT: ec_creer_liste_Destination_pma Allocation memoire impossible");
                  return XDC_NOK;
                }
                memcpy ( &(pl_NewZDP->zdp), pl_zdp, sizeof (XDY_ZDP) );
                pl_NewZDP->suivant= NULL;

                switch (va_TypeDestination) {
                        case XDC_DEST_UN_PROCHE :
                                        pl_PMA->Dest_un_proche=va_Destination;
                                        pl_NewZDP->suivant = pl_PMA->Graphe_Dest_un_proche;
                                        pl_PMA->Graphe_Dest_un_proche=pl_NewZDP;
                                        break;

                        case XDC_DEST_UN_LOIN :
                                        pl_PMA->Dest_un_loin=va_Destination;
                                        pl_NewZDP->suivant = pl_PMA->Graphe_Dest_un_Loin;
                                        pl_PMA->Graphe_Dest_un_Loin=pl_NewZDP;
                                        break;

                        case XDC_DEST_DEUX_PROCHE :
                                        pl_PMA->Dest_deux_proche=va_Destination;
                                        pl_NewZDP->suivant = pl_PMA->Graphe_Dest_deux_proche;
                                        pl_PMA->Graphe_Dest_deux_proche=pl_NewZDP;
                                        break;

                        case XDC_DEST_DEUX_LOIN :
										pl_PMA->Dest_deux_loin=va_Destination;
										pl_NewZDP->suivant = pl_PMA->Graphe_Dest_deux_loin;
										pl_PMA->Graphe_Dest_deux_loin=pl_NewZDP;
										break;
                }
          return (XDC_OK);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* calcule le temps de parcours entre un PMV et une
* destination
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD30_Calculer_Traversee(va_pr_pmv, 
*				va_autoroute_pmv,
*			        va_sens_pmv, 
*				va_destination,
				pa_tdp)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
*
******************************************************/

extern int XZAD30_Calculer_Traversee( 
                          XDY_Autoroute 	va_autoroute_pmv,
			  XDY_PR                va_pr_pmv,
			  XDY_Sens 		va_sens_pmv,
			  XDY_PR		va_pr_min,
			  XDY_PR		va_pr_max,
                          XZEZT_Liste_Zdp	*pa_zdp,
			  XDY_TDP		*pa_tdp)
			  
{
  XDY_TDP		vl_tdp_zdp=0;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Calculer_Traversee : IN");

  XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZAD30_Calculer_Traversee: pr_pmv %d, autoroute %d, sens %d, PR min %d, PR max %d",
			va_pr_pmv,
			va_autoroute_pmv,
			va_sens_pmv,
			va_pr_min, va_pr_max
			);

 if (XZEZ10_Calcul_Traversee(va_autoroute_pmv, va_pr_pmv, va_sens_pmv,
                                va_pr_min,
                                va_pr_max,
                                pa_zdp,
                                pg_zdp,
                                &vl_tdp_zdp) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZAD30_Calculer_Traversee : pb appel a XZEZ10_Calcul_Traversee");
    return (XDC_NOK);
  }
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Calculer_Traversee : OUT");
  *pa_tdp=vl_tdp_zdp;
  return (XDC_OK);
}



extern int XZAD30_Calculer_Traversee_Bouchon( 
                          XDY_Autoroute 	va_autoroute,
			  XDY_Sens 		va_sens,
			  XDY_PR		va_pr_min,
			  XDY_PR		va_pr_max,
                          XZEZT_Liste_Zdp	*pa_zdp,
			  XDY_TDP		*pa_tdp)
			  
{
  XDY_TDP		vl_tdp_zdp=0;
  XZADT_Liste_Zdp       *pl_liste_zdp;
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Calculer_Traversee_Bouchon : IN");

  XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZAD30_Calculer_Traversee_Bouchon: autoroute %d, sens %d, PR min %d, PR max %d",
			va_autoroute,
			va_sens,
			va_pr_min, va_pr_max
			);

 pl_liste_zdp = pg_zdp;
   while (pl_liste_zdp != NULL) {
           XZST_03EcritureTrace(   XZSTC_FONCTION, "zdp %d",pl_liste_zdp->zdp.Numero);
	           pl_liste_zdp = pl_liste_zdp->suivant;
		     }

 if (XZEZ11_Calcul_Traversee_Bouchon(va_autoroute, va_sens,
                                va_pr_min,
                                va_pr_max,
                                (XZEZT_Liste_Zdp *)pg_zdp,
                                (XZEZT_Liste_Zdp *)pg_zdp,
                                &vl_tdp_zdp) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"XZAD30_Calculer_Traversee_Bouchon : pb appel a XZEZ10_Calcul_Traversee");
    return (XDC_NOK);
  }
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD30_Calculer_Traversee_Bouchon : OUT");
  *pa_tdp=vl_tdp_zdp;
  return (XDC_OK);
}
