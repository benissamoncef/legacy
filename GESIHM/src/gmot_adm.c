/*E*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm * FICHIER gmot_adm.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient les fonctions appelees a partir
* du menu "Administration" de MIGRAZUR
*
* 1) Fonction "Synoptiques des sous-systemes"
*    menu2_sss_cb puis
*    bdsit_val
*
* 2) Fonction "Synoptiques des taches"
*    menu2_sta_cb puis
*    bdsta_val
*
* 3) Fonction "Synoptiques de l'architecture generale"
*    menu2_sar_cb 
*
* 4) Fonction "Basculement serveur de donnees"
*    bdbsd_val
*
* 5) Fonction "Basculement serveur de communication"
*    menu2_bsc_cb puis
*    bdbsc_val
*
* 6) Fonction "Arret/Lancement de MIGRAZUR"
*    menu2_ars_cb puis
*    bdasu_val
*
* 7) Fonction "Arret d'un calculateur"
*    menu2_aca_cb puis
*    bdaca_val
*
* 8) Fonction "Arret/lancement d'un sous-systeme"
*    menu2_ass_cb puis
*    bdass_val_cb
*    (bdass_li_mac_cb actualise la liste des sous-systemes)
*
* 9) Fonction "Arret/lancement d'une tache"
*    menu2_ata_cb puis
*    bdata_val_cb
*    (bdata_li_mac_cb actualise la liste des taches)
*
* 12) Fonction "Impression de la Modification"
*    menu2_ico_cb 
*
* 13) Fonction "Gestion dynamique des traces"
*    menu2_gdt_cb puis
*    bdsta_val_cb puis
*    grtra_val_cb
*    (grtra_selec_cb actualise les toggle buttons)
*
* 15) Fonction "Consultation des versions logicielles"
*    menu2_cvl_cb
*
* 16) Fonction "Demande d'archivage"
*    menu2_arc_cb
*    -- pour version ulterieure 
*
* 17) Fonction "Diffusion des fichiers d'aide"
*    menu2_aid_cb
*    -- pour version ulterieure 
*
* 18) Fonction "Consultation du journal"
*    menu2_jas_cb
*
* 19) Fonction "Impression du journal d'administration
*    menu2_ijo_cb 
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	14 Nov 94 	: Creation
*
***********
* O.MERCIER	27 Janv 1995	: Ajout callbk pour prise 	V 1.10
* en compte des equipements
*
* T.Milleville	15 Mar 95 	: Modification de la fonction permettant
*		d'afficher le BL   V1.3
* T.Milleville	15 Mar 95 	: Ajout de la fonction list_bl_imp_cb V1.3
* T.Milleville	16 Mar 95 	: Ajout de la fonction livl_imp_cb V1.3
* T.Milleville	20 Jul 95 	: Appel XZSS_O4 plutot que XZSS_03 V1.12
* T.Milleville	18 Aou 95 	: Modification tri des versions logiciels 
*	suivant l'horodate V1.5
* P.Niepceron	25 Jan 96	: Modif de menu2_uni_cb pour fen. UNIX v1.6
* P.Niepceron   21 Fev 96	: Remplacement de malloc par calloc avant l'appel GMStockeTache (893) v1.7
* P.Niepceron   22 Fev 96	: Modif des tabulation pour les versions logicielles et bon de livraison et
				  Tri sur le sous systeme (963) v1.8
* P.Niepceron   22 Fev 96	: Correction du traitement d'affichage des bilans (982) v1.9
* P.Niepceron   04 Mar 96	: Ajout bouton de lancement tsconf.x v.10 (896) 
* P.Niepceron   08 Mar 96	: Ajout boutons de vidage pour les bilans (576)
				  Ajout svg manuelle (577)
				  Ajout envoi du nom machine a tcsva avant sauvegarde /restauration v1.12 v1.13
* P.Niepceron   28 Aou 96	: Ajout filtrage sur Site dans GMStockeListMachines et GMStockeListSite pour pc simpl v1.14
* P.Niepceron   19 Sep 96	: Correction de GMStockeListSites v1.15
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

#include <stdlib.h>

#include "gmot_ser.h" 
#include "gmot_var.h" 

#include "gmot_adm.h" 
#include "gmot_mot_bdl.h" 
#include "gmot_adm_dv.h"
#include "xzcg.h"
#include "xzca.h"
#include "xzit.h"

#define CL_NBLIST 	100

  /* constantes pour determiner le type de tableau a utiliser */
#define CL_SITE 	0
#define CL_MACHINE 	1
#define CL_SSYS 	2
#define CL_TACHE 	3
#define CL_VERSLOG 	4
#define CL_JOURNAL 	5
#define CL_BC 		6
#define CL_BL 		7
#define CL_ARC 		8
#define CL_BRBH 	9
#define CL_BSBH 	10
#define CL_HEBDO 	11

#define CL_BUFSIZ 	80
#define CL_LONG_CMD 	200

#define CL_SIZDEF	100

typedef char tl_ExecFormat[120];
typedef char tl_EnregJournal[255];
typedef char tl_Format[128];
typedef char tl_EnregBC[XDC_LINE_CMD_SIZE];
typedef char tl_EnregBL[XDC_LINE_CMD_SIZE];
typedef char tl_EnregARC[XDC_LINE_CMD_SIZE];
typedef char tl_EnregBRBH[XDC_LINE_CMD_SIZE];
typedef char tl_EnregBSBH[XDC_LINE_CMD_SIZE];
typedef char tl_EnregHEBDO[XDC_LINE_CMD_SIZE];

typedef struct {
	XZSCT_NomTache	vl_NomTache;
	int		vl_TypeTache;
	} tl_NomTache;


static int 			vs_ope_adm;

static int              	vs_NbTacheReel;
static int              	vs_indexServeur;
static XZSCT_NomMachine		vs_machine;
static XZSCT_NomMachine		vs_machine_tra;
static XZSCT_NomMachine		vs_filtre;

	/* Identificateur du fichier BL */
static FILE*                 pl_idJournal_BL = NULL;

static int		     vm_nb=0;



  /* tableaux contenant la liste des machines, sites, etc */

static XZSCT_NomMachine	 	*ps_TabMac = NULL;
static XZSCT_NomSite 		*ps_TabSite = NULL;
static XZSCT_NomSSysteme 	*ps_TabSSys = NULL;

/*static XZSCT_NomTache        	*ps_TabTache = NULL;*/
static tl_NomTache        	*ps_TabTache = NULL;

static tg_Executable    	*ps_TabVerLog = NULL;
static tl_EnregJournal		*ps_TabEnregJournal = NULL;
static tl_EnregBC		*ps_TabEnregBC = NULL;
static tl_EnregBL		*ps_TabEnregBL = NULL;
static tl_EnregARC		*ps_TabEnregARC = NULL;
static tl_EnregBRBH		*ps_TabEnregBRBH = NULL;
static tl_EnregBSBH		*ps_TabEnregBSBH = NULL;
static tl_EnregHEBDO		*ps_TabEnregHEBDO = NULL;

static tl_ExecFormat   		*pg_ListVerLogFormat = NULL;

static int vg_numEnreg_BL		= 0;


  /* menu a partir duquel un callback a ete appelle */
static Widget                    vs_menu_parent;

  /* tableaux des widgets GRSYN_FM, GRSYN_DA, GRTRA_FM et GRTRA_DA */
static Widget                    vs_GRSYN_FM[]={NULL, NULL, NULL,
						NULL, NULL, NULL};
static Widget                    vs_GRSYN_DA[]={NULL, NULL, NULL,
						NULL, NULL, NULL};
static Widget                    vs_GRTRA_FM[]={NULL, NULL, NULL,
						NULL, NULL, NULL};
static Widget                    vs_GRTRA_DA[]={NULL, NULL, NULL,
						NULL, NULL, NULL};
static Widget                    vs_GRMACH_SYNT_DA[]={NULL, NULL, NULL,
						NULL, NULL, NULL};
static Widget                    vs_GRMACH_SYNT_FM[]={NULL, NULL, NULL,
						NULL, NULL, NULL};

void gmot_creer_ficimp(int va_indImp);
void gmot_recup_textimp(int va_indImp, FILE *pa_idFic);



/* Fonction Internes */
void GMAffich_SSSYT(
	Widget va_MenuPere,
	char * va_NomSite,
	int	va_TypeAffich);

void gmot_synchro_disk(XZSCT_NomSite	va_NomSite);



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
******************************************************
* SEQUENCE D'APPEL :
*/

FermetureMere( Widget va_w )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   WidgetClass 	 vl_classWid; /* classe du widget */

   vl_classWid = XtClass( va_w );

   if ( strcmp( vl_classWid->core_class.class_name, "XmPushButton" ) == 0 )
   /* on veut fermer le dialogue apres clic sur un push button */ 
   {
     /* le push button est le petit fils du dialogue */
     XtDestroyWidget( XtParent( XtParent(va_w) ) );
   }
   else
   /* on veut fermer le dialogue apres double clic */
   /* de la liste sur un element  */
   {
     /* l element d'une liste est l arriere petit fils du dialogue */
     XtDestroyWidget( XtParent( XtParent( XtParent(va_w) )));
   }
}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke le nom de la machine dans un tableau
*
******************************************************
* SEQUENCE D'APPEL :
*/

int GMStockeListMachines( 
	XZSCT_NomMachine  va_nom, 
	int 		    va_nb,  
	void 		    *pa_dummy )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XZSCT_NomSite vl_Site;
  
  if (vg_TypeMachine == XZSCC_TYPEM_PCS) 
     {
     (void) XZSA_23SiteMachine( va_nom, vl_Site );
     if  ( strcmp( vl_Site , vg_NomSite) )
     	return XDC_OK;
    }
    ps_TabMac = realloc( ps_TabMac, (vm_nb+1)*sizeof(XZSCT_NomMachine) );
    if ( ps_TabMac == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, "GMStockeListMachines: pb au realloc " );
    else
        {
        strcpy( ps_TabMac[vm_nb] , va_nom ) ;
        vm_nb++;
        }
}




/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke le nom de la machine dans un tableau ssi la machine est un serveur
*
******************************************************
* SEQUENCE D'APPEL :
*/

int GMStockeListServeurs( 
	XZSCT_NomMachine  va_nom, 
	int 		    va_nb,  
	XZSCT_NomMachine *pa_TabMac )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
    int vl_type;

    if ( XZSA_17TypeMachine( va_nom, &vl_type )  != XDC_OK )
	XZST_03EcritureTrace( XZSTC_WARNING,
		 "GMStockeListServeurs: pb a l'appel de XZSA_17Type");

    if ( vl_type == XZSAC_TYPEM_SD )
    {
       strcpy( pa_TabMac[vs_indexServeur] , va_nom ) ;
       vs_indexServeur++;
    }
}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke le nom du site dans un tableau
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMStockeListSites( XZSCT_NomSite  va_nom, 
			       int 	      va_nb,  
			       void           *pa_dummy )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  
  if (vg_TypeMachine == XZSCC_TYPEM_PCS) 
     {
     if  ( strcmp( va_nom , vg_NomSite) )
     	return;
    }
    ps_TabSite = realloc( ps_TabSite, (vm_nb+1)*sizeof(XZSCT_NomSite) );
    if ( ps_TabSite == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, "GMStockeListSites: pb au realloc " );
    else
       {
        strcpy( ps_TabSite[vm_nb] , va_nom ) ;
        vm_nb++;
        }
}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke le nom du sous-systeme dans un tableau
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMStockeListSSys( XZSCT_NomSSysteme   va_nom, 
			       int 	          va_nb,  
			       void           	  *pa_dummy )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
    ps_TabSSys = realloc( ps_TabSSys, (va_nb+1)*sizeof(XZSCT_NomSSysteme) );

    if ( ps_TabSSys == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, "GMStockeListSSys: pb au realloc " );
    else
        strcpy( ps_TabSSys[va_nb] , va_nom ) ;
}



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke le nom de la tache dans un tableau
*
******************************************************
* SEQUENCE D'APPEL :
*/

void GMStockeListTache( 
	XZSCT_NomTache  va_nom, 
	int 	       va_nb,  
	int            va_TypeTache )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "GMStockeListTache : IN \
			Nom Tache %s -- Nb %d -- Type %d \n",
			va_nom,va_nb,va_TypeTache);

    if ( va_nb > (CL_SIZDEF)-1 ) 
    	ps_TabTache = realloc( ps_TabTache, (va_nb+1)*sizeof(tl_NomTache) );


    if ( ps_TabTache == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, "GMStockeListTache: pb au realloc " );
    else
    {
        strcpy( ps_TabTache[va_nb].vl_NomTache , va_nom ) ;
	ps_TabTache[va_nb].vl_TypeTache = va_TypeTache;
    }

	XZST_03EcritureTrace(XZSTC_FONCTION, "GMStockeListTache : OUT \n");

}	/* Fin GMStockeListTache */


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke la tache et sa version logicielle dans un tableau global
*  en fonction du filtre (si le filtre est vide OU s'il contient 
*  un nom de machine et que cette machine est celle sur laquelle
*  se trouve la tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMStockeListVerLog( XZSPT_ComposantVL  va_EnregVL,
				int                va_Nb )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{

	XZST_03EcritureTrace(XZSTC_FONCTION, "GMStockeListVerLog : IN \
	-- Nom Mach %s -- Nom Fichier %s -- Num Version %d -- Horodate %s \
	Nb elts %d \n",va_EnregVL.NomMachine, va_EnregVL.NomFichier,
	va_EnregVL.NumVersion, va_EnregVL.Horodate, va_Nb);

    if ( ( strcmp( vs_filtre, "" ) == 0 ) ||
         ( (strcmp(vs_filtre,"")!=0) 
	    && (strcmp(va_EnregVL.NomMachine,vs_filtre)==0)) )
    {
      	ps_TabVerLog = 
		realloc( ps_TabVerLog, (va_Nb+1) * sizeof(tg_Executable) );

    	if ( ps_TabVerLog == NULL )
		XZST_03EcritureTrace (XZSTC_WARNING, 
			"GMStockeListVerLog: pb au realloc " );
    	else
	{
      		strcpy( ps_TabVerLog[vs_NbTacheReel].typeFic, 
				va_EnregVL.TypeFic );
      		strcpy( ps_TabVerLog[vs_NbTacheReel].nomMac, 
				va_EnregVL.NomMachine );
      		strcpy( ps_TabVerLog[vs_NbTacheReel].nomFichier, 
				va_EnregVL.NomFichier );
      		strcpy( ps_TabVerLog[vs_NbTacheReel].numVersion, 
				va_EnregVL.NumVersion );
      		strcpy( ps_TabVerLog[vs_NbTacheReel].horodate, 
				va_EnregVL.Horodate );

      		vs_NbTacheReel++;
	}
    }

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMStockeListVerLog : OUT \n");

}	/* Fin GMStockeListVerLog */


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  stocke les enregistrements du journal d'administration dans
* le tableau ps_Tab
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregJournal( 	
			XZSMT_Horodate	va_horodate,
			int             va_nb,
			char 		* pa_Libelle )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   tl_Format 		vl_leformat;
   XDLT_SystemeAlarme 	vl_SysAlarme;
   va_list 		vl_ap;
   int			vl_ValRet;
   

   ps_TabEnregJournal = realloc( ps_TabEnregJournal, 
				 (va_nb+1) * sizeof(tl_EnregJournal) );

   if ( ps_TabEnregJournal == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, "GMStockeListEnregJournal: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*					dans le tableau */
   	strcpy( ps_TabEnregJournal[va_nb], va_horodate );
   	strcat( ps_TabEnregJournal[va_nb], pa_Libelle );
   }
}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  compare 2 elements de type tg_Executable (fonction de comparaison 
*  utilisee par qsort)
*
******************************************************
* SEQUENCE D'APPEL :
*/

static int GMCompareExec( tg_Executable *exec1, tg_Executable *exec2 )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int vl_retour;
  int vl_res;
  int vl_type1,vl_type2;
  double vl_HoroSev_1	= 0.0;
  double vl_HoroSev_2	= 0.0;

	XZST_03EcritureTrace(XZSTC_FONCTION, " GMCompareExec : IN \n");

	XZST_03EcritureTrace(XZSTC_DEBUG1, " Compare horodate %s -- %s \n",
		(*exec1).horodate, (*exec2).horodate );

	/*A Transformation de l'horodate en secondes */
	XZSM_13ConversionHorodateSec((*exec1).horodate,&vl_HoroSev_1);
	XZSM_13ConversionHorodateSec((*exec2).horodate,&vl_HoroSev_2);

  /*vl_retour = strcmp( (*exec1).horodate, (*exec2).horodate );*/
  /*if (vl_HoroSev_1 < vl_HoroSev_2)
  {
	vl_retour = +1;
  }
  else if (vl_HoroSev_1 > vl_HoroSev_2)
  {
	vl_retour = -1;
  }
   else if (vl_HoroSev_1 = vl_HoroSev_2)
   {
	vl_retour = -1;
    }
*/
	vl_res = strcmp( (*exec1).typeFic, (*exec2).typeFic );

	if ( vl_res == 0  )
	  {
	   vl_res = strcmp( (*exec1).nomMac, (*exec2).nomMac );
	   if ( vl_res > 0 )
	   {
		 vl_retour = +1;
	   }
	   else if (vl_res < 0)
	   {
		 vl_retour = -1;
	   }
	   else 
	   {
		vl_retour = -1;
	   }
	  }
	else
	   vl_retour = (vl_res > 0 ) ? +1 : -1;


/*  vl_retour = strcmp( (*exec1).horodate, (*exec2).horodate );*/
 /* if ( vl_retour == 0 )
  	vl_retour = strcmp( (*exec1).typeFic, (*exec2).typeFic );
  	if ( vl_retour == 0 )
    		vl_retour = strcmp( (*exec1).nomFichier, (*exec2).nomFichier );
  		if ( vl_retour == 0 )
      			vl_retour = strcmp( (*exec1).nomMac, (*exec2).nomMac );
      			if ( vl_retour == 0 )
        			vl_retour = strcmp( (*exec1).numVersion, (*exec2).numVersion );*/


	XZST_03EcritureTrace(XZSTC_FONCTION, " GMCompareExec : OUT -- Retour %d  \n", vl_retour);
  return(vl_retour);
}
 

/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  remplit le tableau ps_TabVerLog avec les taches et le trie
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMPutVerLogIntoList( )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int vl_NbTache;

  /* allocation memoire du tableau destine a recevoir la liste des taches */
  ps_TabVerLog = malloc( sizeof(tg_Executable) );

  if ( ps_TabVerLog != NULL )
  {
  /* recuperation de la liste des taches avec leur version logicielle */
  if ( XZSP_08ListeVersionLog(  (void (*)())GMStockeListVerLog, 
				XZSPC_ACTUEL, 
				&vl_NbTache ) != XDC_OK )
	XZST_03EcritureTrace( XZSTC_WARNING,
	      "GMPutVerLogIntoList: pb a l'appel de XZSP_08ListeVersionLog");

  /* tri du tableau ps_TabVerLog en prenant en compte les champs dans l'ordre */
  /* nomTache, nomMac, numVersion et horodate                                  */
  qsort( ps_TabVerLog, vs_NbTacheReel, sizeof(tg_Executable), 
	   (int (*)( const void *, const void *))GMCompareExec );
  }
}


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  formatage de la liste de taches
*  (appelle par menu2_cvl_cb et livl_fil_cb)
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMFormatteListVerLog( tl_ExecFormat *va_ListVerLogFormat )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int i,vl_ind;
  char vl_blanc[]="                    ";

  /* alignement des champs */
  for ( i=0; i<vs_NbTacheReel; i++ )
  {

  /*    sprintf( (va_ListVerLogFormat)[i] , "%s%s%s%s%s%s%s%s%s\r",
				         ps_TabVerLog[i].typeFic,
					 &vl_blanc[(vl_ind=strlen(ps_TabVerLog[i].typeFic)) < 20 ? vl_ind : 20],
				         ps_TabVerLog[i].nomFichier,
				         &vl_blanc[(vl_ind=strlen(ps_TabVerLog[i].nomFichier)) < 20 ? vl_ind : 20],
					 ps_TabVerLog[i].nomMac,
				         &vl_blanc[(vl_ind=strlen(ps_TabVerLog[i].nomMac)) < 20 ? vl_ind : 20],
					 ps_TabVerLog[i].numVersion,
				         &vl_blanc[(vl_ind=strlen(ps_TabVerLog[i].numVersion)) < 20 ? vl_ind : 20],
					 ps_TabVerLog[i].horodate );*/
      sprintf( (va_ListVerLogFormat)[i] , "%-18s%-15s%-10s%s\r",
					 ps_TabVerLog[i].typeFic,
					 ps_TabVerLog[i].nomMac,
					 ps_TabVerLog[i].numVersion,
					  ps_TabVerLog[i].horodate );

  }
}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* recupere les 2 serveurs de donnees
*
******************************************************
* SEQUENCE D'APPEL :
*/

int GMGetTwoServers( 	XZSCT_NomMachine va_serveurActif, 
			XZSCT_NomMachine va_serveur1, 
			XZSCT_NomMachine va_serveur2 )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   XZSCT_NomSite  	vl_NomSite;
   XZSCT_NomMachine 	vl_TabServeurs[2];
   int			vl_NbMac;

   /* on recupere le site "Berne" qui est le site du SD actif   */
   if ( XZSA_23SiteMachine( va_serveurActif, vl_NomSite ) != XDC_OK )
   {
       	XZST_03EcritureTrace( XZSTC_WARNING,
		"GMGetTwoServers: pb a l'appel de XZSA_23SiteMachine");
	return XDC_NOK ;
   }

   vs_indexServeur = 0;

   /* on recupere la liste des machines de Berne;               */
   /* de cette liste, on ne garde que celles de type "Serveur"  */
   if ( XZSA_19ListeMachineSite( vl_NomSite, 
       		    		 &GMStockeListServeurs,
			    	 &vl_NbMac, 
			    	 vl_TabServeurs ) != XDC_OK )
   {
       	XZST_03EcritureTrace( XZSTC_WARNING,
		"GMGetTwoServers: pb a l'appel de XZSA_19ListeMachineSite");
	return XDC_NOK ;
   }

   strcpy( va_serveur1, vl_TabServeurs[0] );
   strcpy( va_serveur2, vl_TabServeurs[1] );

   return XDC_OK ;
}

/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* affiche le contenu d'un tableau dans un widget liste 
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void GMAfficheListeDansWidget( Widget  va_widget,
				      void    *va_Tableau, 
				      int     va_NbTab,
				      int     va_typeTableau )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XmString  *pl_List	= NULL;
  XmString  *pl_List_Temp	= NULL;
  XmString  *pl_List_No	= NULL;
  XmString  pl_Comment_1	= NULL;
  XmString  pl_Comment_2	= NULL;
  XmString  pl_Comment_3	= NULL;
  XmString  pl_Comment_4	= NULL;
  XmString  pl_Comment_5	= NULL;
  int       i		= 0;
  int 		vl_NbTab_Pre	= 0;
  int 		vl_NbTab_NoPre	= 0;
  int 		vl_NbTab_Temp	= 0;
  Arg arg[50];
  XFontStruct	*font1	= NULL;
  XmFontList    fontlist1,fontlist2;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMAfficheListeDansWidget : IN \
		nb elts %d \n",va_NbTab);
	font1 = XLoadQueryFont(XtDisplay( va_widget),"iso1.20b");
	fontlist1 = XmFontListCreate(font1,"chset1");

	pl_Comment_1 = XmStringCreateLtoR("---------------","chset1");
	pl_Comment_2 = XmStringCreateLtoR("Taches Non Presentes","chset1");
	pl_Comment_3 = XmStringCreateLtoR("               ","chset1");
	pl_Comment_4 = XmStringCreateLtoR("Taches Temporaires","chset1");
	pl_Comment_5 = XmStringCreateLtoR("Taches Permanentes","chset1");


  pl_List = (XmString *)XtMalloc( va_NbTab * sizeof(XmString) ); 
  pl_List_No = (XmString *)XtMalloc( va_NbTab * sizeof(XmString) ); 
  pl_List_Temp = (XmString *)XtMalloc( va_NbTab * sizeof(XmString) ); 

  for ( i=0; i<va_NbTab; i++ )
  {
     switch ( va_typeTableau )
     {
       case CL_SITE:
	  pl_List[i] = XmStringCreateLtoR( ((XZSCT_NomSite *)va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
	  break;

       case CL_MACHINE:
	  pl_List[i] = XmStringCreateLtoR( ((XZSCT_NomMachine *)va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;

	  break;

       case CL_SSYS:
	  pl_List[i] = XmStringCreateLtoR( ((XZSCT_NomSSysteme *)va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
	  break;

       case CL_TACHE:
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Type Tache %d \n",((tl_NomTache *)va_Tableau)[i].vl_TypeTache);

		if( ((tl_NomTache *)va_Tableau)[i].vl_TypeTache == XZSAC_TYPE_TPER)
		{	
	  		pl_List[vl_NbTab_Pre] = XmStringCreateLtoR( 
				((tl_NomTache *)va_Tableau)[i].vl_NomTache,
			 	(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
			vl_NbTab_Pre++;
		}
		else if ( ((tl_NomTache *)va_Tableau)[i].vl_TypeTache == XZSAC_TYPE_TTMP)
		{
	  		pl_List_Temp[vl_NbTab_Temp] = XmStringCreateLtoR( 
				((tl_NomTache *)va_Tableau)[i].vl_NomTache,
			 	(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
			vl_NbTab_Temp++;
		}
		else
		{
	  		pl_List_No[vl_NbTab_NoPre] = XmStringCreateLtoR( 
				((tl_NomTache *)va_Tableau)[i].vl_NomTache,
			 	"chset1");
			vl_NbTab_NoPre++;
		}

	  break;

       case CL_VERSLOG:
	  pl_List[i] = XmStringCreateLtoR( ((tl_ExecFormat *)va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
          break;

       case CL_JOURNAL:
	  pl_List[i] = XmStringCreateLtoR( ((tl_EnregJournal *)va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;

	  break;

       case CL_BC:
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregBC *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       case CL_BL:
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregBL *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       case CL_ARC:
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"ARC libelle %s \n",((tl_EnregARC *) va_Tableau)[i]);
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregARC *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       case CL_BRBH:
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregBRBH *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       case CL_BSBH:
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"BSBH libelle %s \n",((tl_EnregBSBH *) va_Tableau)[i]);
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregBSBH *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       case CL_HEBDO:
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"HEBDO libelle %s \n",((tl_EnregHEBDO *) va_Tableau)[i]);
	  pl_List[i] = XmStringCreateLtoR(((tl_EnregHEBDO *) va_Tableau)[i],
			 (XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
	  vl_NbTab_Pre++;
		break;

       default:
   	  /* erreur */
	  XZST_03EcritureTrace( XZSTC_WARNING,
	     "gmot_adm: GMAfficheListeDansWidget: mauvais parametre d'entree ");
          return;
     }
  }

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Nb Tache Pre %d -- Non Pre %d \n",
		vl_NbTab_Pre,vl_NbTab_NoPre);

  XmListDeleteAllItems(va_widget);

	/*A Affichage des taches non presentes */
  if (vl_NbTab_NoPre != 0)
  {
  	XmListAddItems(va_widget,pl_List_No,vl_NbTab_NoPre,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_2,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_3,1);
  }
	/*A Affichage des taches Temporaires */
  if (vl_NbTab_Temp != 0)
  {
  	XmListAddItems(va_widget,pl_List_Temp,vl_NbTab_Temp,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_4,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_3,1);
  }

	/*A Affichage des taches presentes */
  if (vl_NbTab_Pre != 0)
  {
  	XmListAddItems(va_widget,pl_List,vl_NbTab_Pre,1);
	if (va_typeTableau == CL_TACHE)
	{
  		XmListAddItem(va_widget,pl_Comment_1,1);
  		XmListAddItem(va_widget,pl_Comment_5,1);
  		XmListAddItem(va_widget,pl_Comment_1,1);
	}
  }

  XmListSelectPos( va_widget, 1, False );

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Affichage de la Liste \n");

  for ( i=0; i<vl_NbTab_Pre; i++ )
  {
       XmStringFree( pl_List[i] ); 
  }
  free( pl_List );
  for ( i=0; i<vl_NbTab_NoPre; i++ )
  {
       XmStringFree( pl_List_No[i] ); 
  }
  free( pl_List_No );
  for ( i=0; i<vl_NbTab_Temp; i++ )
  {
       XmStringFree( pl_List_Temp[i] ); 
  }
  free( pl_List_Temp );

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMAfficheListeDansWidget : OUT \n");

}	/* GMAfficheListeDansWidget */




/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
******************************************************
* SEQUENCE D'APPEL :
*/

GMAfficheTachesDeMachine( Widget  	  va_widget,
			XZSCT_NomMachine  va_machine )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int 		vl_NbTache	= 0;
  int 		vl_NbTache_Pre	= 0;
  int 		vl_NbTache_NoPre	= 0;
  int 		vl_NbTache_Temp	= 0;
  XmString  	*pl_List	= NULL;
  XmString  	*pl_List_No	= NULL;
  XmString  	*pl_List_Temp	= NULL;
  int       	i		= 0;
  int       	vl_ValRet	= XDC_OK;
  XmString  pl_Comment_1	= NULL;
  XmString  pl_Comment_2	= NULL;
  XmString  pl_Comment_3	= NULL;
  XmString  pl_Comment_4	= NULL;
  XmString  pl_Comment_5	= NULL;
  XFontStruct	*font1	= NULL;
  XmFontList    fontlist1,fontlist2;
  int		vl_Ind	= XDC_VRAI;



	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"GMAfficheTachesDeMachine : IN -- Nom Machi %s \n",
		va_machine);

	font1 = XLoadQueryFont(XtDisplay( va_widget),"iso1.20b");
	fontlist1 = XmFontListCreate(font1,"chset1");

	pl_Comment_1 = XmStringCreateLtoR("---------------","chset1");
	pl_Comment_2 = XmStringCreateLtoR("Taches Non Presentes","chset1");
	pl_Comment_3 = XmStringCreateLtoR("               ","chset1");
	pl_Comment_4 = XmStringCreateLtoR("Taches Temporaires","chset1");
	pl_Comment_5 = XmStringCreateLtoR("Taches Permanentes","chset1");

  /*ps_TabTache = malloc( sizeof(tl_NomTache) );*/

  ps_TabTache = calloc( CL_SIZDEF ,sizeof(tl_NomTache) );

  if ( ps_TabTache != NULL )
  {
  	vl_ValRet = XZSA_03ListeTacheMachine( va_machine, 
				    GMStockeListTache, 
				    &vl_NbTache, 
				    &vl_Ind);

  	pl_List = (XmString *)XtMalloc( vl_NbTache * sizeof(XmString) ); 
  	pl_List_No = (XmString *)XtMalloc( vl_NbTache * sizeof(XmString) ); 
  	pl_List_Temp = (XmString *)XtMalloc( vl_NbTache * sizeof(XmString) ); 

  	for ( i=0; i<vl_NbTache; i++ )
  	{
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Type Tache %d \n",((tl_NomTache *)ps_TabTache)[i].vl_TypeTache);
		if( ((tl_NomTache *)ps_TabTache)[i].vl_TypeTache == XZSAC_TYPE_TPER)
		{	
      			pl_List[vl_NbTache_Pre] = XmStringCreateLtoR( 
				((tl_NomTache *)ps_TabTache)[i].vl_NomTache,
				(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);

			vl_NbTache_Pre++;
		}
		else if( ((tl_NomTache *)ps_TabTache)[i].vl_TypeTache == XZSAC_TYPE_TTMP)
		{	
      			pl_List_Temp[vl_NbTache_Temp] = XmStringCreateLtoR( 
				((tl_NomTache *)ps_TabTache)[i].vl_NomTache,
				(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);
			vl_NbTache_Temp++;
		}
		else
		{
      			pl_List_No[vl_NbTache_NoPre] = XmStringCreateLtoR( 
				((tl_NomTache *)ps_TabTache)[i].vl_NomTache,
				"chset1");

			vl_NbTache_NoPre++;
		}
  	}

  XmListDeleteAllItems(va_widget);

	/*A Affichage des taches non presentes */
  if (vl_NbTache_NoPre != 0)
  {
  	XmListAddItems(va_widget,pl_List_No,vl_NbTache_NoPre,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_2,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_3,1);
  }
	/*A Affichage des taches Temporaires */
  if (vl_NbTache_Temp != 0)
  {
  	XmListAddItems(va_widget,pl_List_Temp,vl_NbTache_Temp,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_4,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_3,1);
  }

	/*A Affichage des taches presentes */
  if (vl_NbTache_Pre != 0)
  {
  	XmListAddItems(va_widget,pl_List,vl_NbTache_Pre,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  	XmListAddItem(va_widget,pl_Comment_5,1);
  	XmListAddItem(va_widget,pl_Comment_1,1);
  }

  XmListSelectPos( va_widget, 1, False );

  for ( i=0; i<vl_NbTache_Pre; i++ )
  {
       XmStringFree( pl_List[i] ); 
  }
  free( pl_List );
  for ( i=0; i<vl_NbTache_NoPre; i++ )
  {
       XmStringFree( pl_List_No[i] ); 
  }
  free( pl_List_No );
  for ( i=0; i<vl_NbTache_Temp; i++ )
  {
       XmStringFree( pl_List_Temp[i] ); 
  }
  free( pl_List_Temp );


  	free( ps_TabTache );
  }
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"GMAfficheTachesDeMachine : OUT \n");

}	/* Fin GMAfficheTachesDeMachine */



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de visualisation du synoptique des 
* sous-systemes
* - stockage de l'operation (cm_SYN_SSS) dans une 
*   variable globale
* - affichage de BDSIT qui permet de preciser:
*    - le nom du site
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_sss_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int           vl_NbSite;

  /* stockage de l'operation dans une variable globale */
   vs_ope_adm = cm_SYN_SSS;

  ps_TabSite = malloc( sizeof(XZSCT_NomSite) );

  if ( ps_TabSite != NULL )
  {

  /* recuperation de la liste des sites par XZSA_18ListeSite */
  vm_nb=0;
  XZSA_18ListeSite( GMStockeListSites, &vl_NbSite, ps_TabSite );
  vl_NbSite=vm_nb;
  
  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDSIT */
  create_BDSIT ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de sites dans BDSIT */
  GMAfficheListeDansWidget( BDSIT_LI_SIT, ps_TabSite, vl_NbSite, CL_SITE );

  /* ouverture de BDSIT */
  XtManageChild( BDSIT_FM );

  free( ps_TabSite );
  }
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de visualisation du synoptique des 
* taches
* - stockage de l'operation (cm_SYN_STA) dans une 
*   variable globale
* - affichage de BDSTA qui permet de preciser:
*    - le nom de la machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_sta_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

  /* stockage de l'operation dans une variable globale */
  vs_ope_adm = cm_SYN_STA;

  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines, &vl_NbMac, ps_TabMac );
  vl_NbMac=vm_nb;
  
  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDSTA */
  create_BDSTA ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDSTA */
  GMAfficheListeDansWidget( BDSTA_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  /* affichage de BDSTA */
  XtManageChild( BDSTA_FM );

  free( ps_TabMac );
  }
}


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* creation ou management de GRSYN
* - recuperation du numero de screen du parent
* - si GRSYN_FM existe pour ce screen (vs_GRSYN_FM non NULL)
*   - creation par create_GRSYN
*   - valorisation de vs_GRSYN_FM et vs_GRSYN_DA
* - sinon
*   - management du widget
*
******************************************************
* SEQUENCE D'APPEL :
*/

int cree_ou_manage_GRSYN(Widget w, int *p_num_screen)

/*
* PARAMETRES EN ENTREE :
*
* w: parent
*
* PARAMETRES EN SORTIE :
*
* num_screen: numero de screen du parent
*
* VALEUR(S) DE RETOUR :
* 1 si creation
* 0 si management
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int vl_num_screen=(*p_num_screen)=XScreenNumberOfScreen(XtScreen(w));

  /* creation de GRSYN la premiere fois uniquement */
  if(!vs_GRSYN_FM[vl_num_screen])
  {
    /* creation de GRSYN */
    create_GRSYN ( w, vg_mrm_hierarchy, &vg_class );
    vg_GRSYN[vl_num_screen]=GRSYN;

    vg_GRSYN_FM[vl_num_screen]=GRSYN_FM;
    vs_GRSYN_FM[vl_num_screen]=GRSYN_FM;
    vs_GRSYN_DA[vl_num_screen]=GRSYN_DA;

    /* retourner 1: la fenetre a ete cree */
    return 1;
  }
  else
  {
    /* affichage du synoptique */
    XtManageChild ( vs_GRSYN_FM[vl_num_screen] );

    /* retourner 0: la fenetre a ete managee (elle etait deja cree) */
    return 0;
  }
}

/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* creation ou management de GRTRA
* - recuperation du numero de screen du parent
* - si GRTRA_FM existe pour ce screen (vs_GRTRA_FM non NULL)
*   - creation par create_GRTRA
*   - valorisation de vs_GRTRA_FM et vs_GRTRA_DA
* - sinon
*   - management du widget
*
******************************************************
* SEQUENCE D'APPEL :
*/

int cree_ou_manage_GRTRA(Widget w, int *p_num_screen)

/*
* PARAMETRES EN ENTREE :
*
* w: parent
*
* PARAMETRES EN SORTIE :
*
* num_screen: numero de screen du parent
*
* VALEUR(S) DE RETOUR :
* 1 si creation
* 0 si management
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int vl_num_screen=(*p_num_screen)=XScreenNumberOfScreen(XtScreen(w));

  /* creation de GRTRA la premiere fois uniquement */
  if(!vs_GRTRA_FM[vl_num_screen])
  {
    /* creation de GRTRA */
    create_GRTRA ( w, vg_mrm_hierarchy, &vg_class );
    vs_GRTRA_FM[vl_num_screen]=GRTRA_FM;
    vs_GRTRA_DA[vl_num_screen]=GRTRA_DA;

    /* affichage du synoptique */
    XtManageChild ( vs_GRTRA_FM[vl_num_screen] );

    /* retourner 1: la fenetre a ete cree */
    return 1;
  }
  else
  {
    XtManageChild ( vs_GRTRA_FM[vl_num_screen] );

    /* retourner 0: la fenetre a ete managee (elle etait deja cree) */
    return 0;
  }
}



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de visualisation du synoptique de 
* l'architecture generale
* Si GRSYN n'a pas encore ete cree, l'affichage est differe jusqu'au
* premier expose
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_sar_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  void GRSYN_TYP_ARC_exposeCb(Widget, XtPointer, XtPointer);
  int vl_num_screen;

  /* creation ou management de GRSYN */
  if(cree_ou_manage_GRSYN(w, &vl_num_screen))
  {

	XZST_03EcritureTrace(XZSTC_DEBUG1,"Affichge Synoptique \
		-- Widgget Affich %d \n",vs_GRSYN_DA[vl_num_screen]);

    AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], cg_TYP_ARC, NULL );
    /*XtAddCallback(vs_GRSYN_DA[vl_num_screen], XmNexposeCallback,
		GRSYN_TYP_ARC_exposeCb, NULL);*/
  }
  else
  {
    AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], cg_TYP_ARC, NULL );
  }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void GRSYN_TYP_ARC_exposeCb(Widget w,
		XtPointer client_data, XtPointer xt_call_data)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GRSYN_TYP_ARC_exposeCb " ;

  XtRemoveCallback(w, XmNexposeCallback, GRSYN_TYP_ARC_exposeCb, client_data);
  AfficheVueAdm( w, cg_TYP_ARC, client_data );
  if(client_data)
    free(client_data);

}	/* Fin GRSYN_TYP_ARC_exposeCb */



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de basculement d'un serveur de communication
*
* ouverture de BDBSC permettant de preciser le 
* basculement desire:
* - type de basculement
* - periode (si cyclique)
* - site
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_bsc_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int           vl_NbSite;

  ps_TabSite = malloc( sizeof(XZSCT_NomSite) );

  if ( ps_TabSite != NULL )
  {
  /* recuperation de la liste des sites par XZSA_18ListeSite */
  vm_nb=0;
  XZSA_18ListeSite( GMStockeListSites, &vl_NbSite, ps_TabSite );
  vl_NbSite=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDBSC */
  create_BDBSC ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de sites dans BDBSC */
  GMAfficheListeDansWidget( BDBSC_LI_SIT, ps_TabSite, vl_NbSite, CL_SITE );

  free( ps_TabSite );

  /* ouverture de BDBSC */
  XtManageChild( BDBSC_FM );
  }
}


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'arret/lancement MIGRAZUR
* - affichage de BDASU qui permet de preciser:
*    - le nom de la machine
*    - le type d'operation souhaite
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_ars_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

	XZST_03EcritureTrace(XZSTC_FONCTION,"menu2_ars_cb : IN \n");


  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines, &vl_NbMac, ps_TabMac );
  vl_NbMac=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDASU */
  create_BDASU ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDASU */
  GMAfficheListeDansWidget( BDASU_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  /* initialisation de BDASU */
  /*EcritChampsASU( cg_VIDE );*/

  free( ps_TabMac );

  /* affichage de BDASU */
  XtManageChild( BDASU_FM );
  }

	XZST_03EcritureTrace(XZSTC_FONCTION,"menu2_ars_cb : OUT \n");

}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'arret d'un calculateur
* - stockage de l'operation (cm_ARR_CAL) dans une 
*   variable globale
* - affichage de BDACA qui permet de preciser:
*    - le nom de la machine
*    - le  type d'arret (avec ou sans relance)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_aca_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

  /* stockage de l'operation dans une variable globale */
  vs_ope_adm = cm_ARR_CAL;

  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines, &vl_NbMac, ps_TabMac );
  vl_NbMac=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDACA */
  create_BDACA ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDACA */
  GMAfficheListeDansWidget( BDACA_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  /* initialisation de BDACA */
  EcritChampsACA( cg_VIDE );

  free( ps_TabMac );

  /* affichage de BDACA */
  XtManageChild( BDACA_FM );
  }
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'arret ou de lancement d'un sous-systeme
* - affichage de BDASS qui permet de preciser:
*    - le nom de la machine
*    - le nom du sous-systeme
*    - l'operation (arret ou lancement)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_ass_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines , &vl_NbMac, ps_TabMac ); 
  vl_NbMac=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDASS */
  create_BDASS ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDASS */
  GMAfficheListeDansWidget( BDASS_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  /* initialisation de BDASS */
  EcritChampsASS( cg_VIDE );

  free( ps_TabMac );

  /* affichage de BDASS */
  XtManageChild( BDASS_FM );

  /* affichage des sous-systemes de la premiere machine de la liste */
  bdass_li_mac_cb( NULL, NULL, NULL );
  }
}



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'arret ou de lancement d'une tache
* - affichage de BDATA qui permet de preciser:
*    - le nom de la machine
*    - le nom de la tache
*    - l'operation (arret ou lancement)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_ata_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines, &vl_NbMac, ps_TabMac );
  vl_NbMac=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDATA */
  create_BDATA ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDATA */
  GMAfficheListeDansWidget( BDATA_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  /* initialisation de BDATA */
  EcritChampsATA( cg_VIDE );

  free( ps_TabMac );

  /* affichage de BDATA */
  XtManageChild( BDATA_FM );

  /* affichage des taches de la premiere machine de la liste */
  bdata_li_mac_cb(NULL,NULL, NULL );
  }
}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		Release : $Revision: 1.15 $              Date :  $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'impression de la configuration:
*  - on copie les fichiers se trouvant dans l'espace 
*    operationnel XDC_NSUP_PATHOPER du serveur actif
*    vers l'espace temporaire XDC_PATHTEMP_SUP de la 
*    machine locale
*  - on imprime les fichiers presents dans XDC_PATHTEMP_SUP
*  - puis on les detruit
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_ico_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   char 		vl_cmd[CL_LONG_CMD];
   FILE			*pl_cr;
   char 		buf[CL_BUFSIZ];
   char 		vl_NomFichier[XZSSC_PATH_ABS_SIZE];
   XZSCT_NomMachine  	vl_serveurActif;
   XZSCT_NomMachine     vl_macLocale;

   /* recuperation du nom du serveur actif */
   if ( XZSE_19NomSDActif( vl_serveurActif ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, 
		"menu2_ico_cb: impossible de recuperer le serveur actif" );
      return;
   }

   /* recuperation du nom de la machine locale */
   if ( XZSC_07NomMachine( vl_macLocale ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING,
		"menu2_ico_cb: impossible de recuperer la machine locale" );
      return;
   }

   /* recuperation du nom nsup_adminis  du Fichier d'administartion de 
   /* l'espace operationnel du serveur actif en executant   */
   /* un "ls" distant                                       */
   sprintf( vl_cmd, "remsh %s \"cd %s;/bin/ls %s\" ", 
		    vl_serveurActif, 
		    XDC_NSUP_PATHOPER ,
		    XDF_NSUP_ADMINIS);

   if ( (pl_cr = popen(vl_cmd, "r")) != NULL )
   {
	/* on parcourt la liste des fichiers */
	while ( fscanf( pl_cr, "%s\n", buf ) != EOF )
	{
	   /* "buf" contient le nom du fichier a copier vers ....*/

	   /* S'il ne s'agit pas du fichier journal */
	   if (strcmp(buf, XDF_NSUP_JOURADM) != 0)
	   {
 	      /* l'espace temporaire XDC_PATHTEMP_SUP de vl_macLocale */
	      if (XZSS_07TransfertFichier( vl_serveurActif, 
					buf, 
					XDC_NSUP_PATHOPER,
				        vl_macLocale,     
					buf, 
					XDC_REP_TMP ) != XDC_OK )
	         XZST_03EcritureTrace( XZSTC_WARNING,
		   "menu2_ico_cb: pb a l'appel de XZSS_07TransfertFichier");
	      else
              {
	         /* impression du fichier */
                 if (XZSS_04ImprimerFichier( XZSSC_TYPE_IMPRIMA, 
					buf, XDC_REP_TMP,XDC_IMP_ASCII ) != XDC_OK )
	         {
                    XZST_03EcritureTrace( XZSTC_WARNING, 
			"menu2_ico_cb: pb a l'appel de XZSS_04ImprimerFichier");
                 }
	         else
	         {
		    /* creation du nom de fichier */
		    sprintf(vl_NomFichier, "%s/%s",XDC_REP_TMP,buf);

		    /* supression du fichier */
		    if (XZSS_06DetruireFichier(vl_NomFichier))
		    {
                       XZST_03EcritureTrace( XZSTC_WARNING, 
			  "menu2_ico_cb: pb a l'appel de XZSS_06DetruireFichier");
		    }
                 }

	       }
          }

	}
	pclose( pl_cr );
   }
   else
   {
	XZST_03EcritureTrace( XZSTC_WARNING,
		      "menu2_ico_cb: pb a l'appel de popen pour ls");
   }

}	/* Fin menu2_ico_cb */


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de visualisation du synoptique des traces
* - stockage de l'operation (cm_SYN_TRA) dans une 
*   variable globale
* - affichage de BDSTA qui permet de preciser:
*    - le nom de la machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_gdt_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int              vl_NbMac;

  /* stockage de l'operation dans une variable globale */
  vs_ope_adm = cm_SYN_TRA;
	
  ps_TabMac = malloc( sizeof(XZSCT_NomMachine) );

  if ( ps_TabMac != NULL )
  {
  /* recuperation de la liste des machines */
  vm_nb=0;
  XZSA_01ListeMachine( &GMStockeListMachines, &vl_NbMac, ps_TabMac );
  vl_NbMac=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDSTA */
  create_BDSTA ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de machines dans BDSTA */
  GMAfficheListeDansWidget( BDSTA_LI_MAC, ps_TabMac, vl_NbMac, CL_MACHINE );

  free( ps_TabMac );

  /* affichage de BDSTA */
  XtManageChild( BDSTA_FM );

  }

}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de consultation des versions logicielles
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_cvl_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int   	  vl_ValRet;

  /* on ne positionne pas de filtre, par defaut */
  strcpy( vs_filtre, "" );

  vs_NbTacheReel=0;

  /* recuperation de la liste des taches */
  GMPutVerLogIntoList( );

  /* allocation memoire du tableau formatte */
  pg_ListVerLogFormat = malloc( vs_NbTacheReel *sizeof(tl_ExecFormat) );

  if ( pg_ListVerLogFormat != NULL )
  {
  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de LIVL */
  create_LIVL  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* formatage de la liste */
  GMFormatteListVerLog( pg_ListVerLogFormat );

  /* affichage de cette liste dans LIVL */
  GMAfficheListeDansWidget( LIVL_LI, 
			  (void *)pg_ListVerLogFormat, 
			  vs_NbTacheReel, 
			  CL_VERSLOG );

  /* affichage de LIVL */
  XtManageChild( LIVL_FM );
  }
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de filtrage de LIVL
*
* l'operateur a saisi le nom d'une machine dans
* le champ texte LIVL_CH_MAC;
* - s'il est non vide, et coherent,
*   on n'affiche plus que les taches appartenant a
*   cette machine
* - s'il est vide, on affiche toutes les taches
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
livl_fil_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int 		   vl_ValRet;
  XZSCT_NomMachine vl_machine;

  /* recuperation du nom de la machine */
  vl_ValRet = LitChampsLIVL( vl_machine );

  if ( vl_ValRet == cg_VIDE )
    strcpy( vs_filtre, "" );
  else
  {
    /* la machine existe-t-elle ? */
    vl_ValRet = XZSA_05ExisterMachine( vl_machine );
    if ( vl_ValRet != XDC_OK )
    {
        /* erreur de saisie pour le nom de la machine */
	construit_message( vs_menu_parent, XDL_GES46_MAC_NON_EXIS );
	return;
    }
    else
    {
	/* filtre sur ce nom de machine */
        strcpy( vs_filtre, vl_machine );
    }
  }

  /* le filtre etant positionne, on passe a la suite ... */

  vs_NbTacheReel=0;

  /* recuperation de la liste des taches */
  GMPutVerLogIntoList( );

  /* allocation memoire du tableau formatte */
  pg_ListVerLogFormat = malloc( vs_NbTacheReel *sizeof(tl_ExecFormat) );

  if ( pg_ListVerLogFormat != NULL )
  {
  /* formatage de la liste */
  GMFormatteListVerLog( pg_ListVerLogFormat );

  /* affichage de cette liste dans LIVL */
  GMAfficheListeDansWidget(	LIVL_LI, 
			  	(void *)pg_ListVerLogFormat, 
			  	vs_NbTacheReel, 
			  	CL_VERSLOG );

  /* liberation memoire du tableau pg_ListVerLogFormat */
  free( pg_ListVerLogFormat );

  /* liberation memoire du tableau ps_TabVerLog */
  free( ps_TabVerLog );
  }
}



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'archivage envoyee au sous-systeme ARCHIV
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_arc_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  /* appel a XZAD_29 */

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  construit_message( vs_menu_parent, XDL_GES57_FONC_PAS_IMPL );
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande d'acces a UNIX: ouverture d'un xterm 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_uni_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XDY_DisplayUnix     pl_displayUnix;
  char               vl_commande[256];
  FILE               *pl_cr;

  /* de quel ecran emane la demande ? */
  get_display_unix( w, pl_displayUnix );

  /* preparation de la commande */
  sprintf( vl_commande, "remsh `hostname` \". .profile;/usr/bin/X11/xterm -display %s -geometry 80x24+330+270 -title UNIX -bg wheat -fg black -fn 9*15 -sb\"&", pl_displayUnix );

  if ( (pl_cr = popen( vl_commande, "w")) == NULL)
        XZST_03EcritureTrace( XZSTC_WARNING, "menu2_uni_cb: echec de popen pour lancer le xterm" );
  else
	pclose (pl_cr);
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de lancement de tsconf.x 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu_tsconf (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XDY_DisplayUnix     pl_displayUnix;
  char               vl_commande[256];
  FILE               *pl_cr;

  /* de quel ecran emane la demande ? */
  get_display_unix( w, pl_displayUnix );

  /* preparation de la commande */
  sprintf( vl_commande, "remsh PODFA \". .profile;cd %s;export DISPLAY=%s; nohup tsconf.x >>/dev/null 2>>/dev/null&\"&", XDC_PATHEXE,pl_displayUnix );

  if ( (pl_cr = popen( vl_commande, "w")) == NULL)
        XZST_03EcritureTrace( XZSTC_WARNING, "menu_tsconf: echec de popen pour lancer tsconf" );
  else
	pclose (pl_cr);
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande  de sauvegarde sur bande de la BD statique
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_sav_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XDY_DisplayUnix     pl_displayUnix;

  /* de quel ecran emane la demande ? */
  get_display_unix( w, pl_displayUnix );

  /* appel de la procedure de CONFIG qui tente la sauvegarde */
  /* en cas d'echec cette procedure utilise gpop pour avertir l'utilisateur */

  /* XZCD_08SauvegardeBD( pl_displayUnix, vg_LangueCourante );*/
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de diffusion des fichiers d'aide 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_aid_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  /* appel a XZCD_06 */
 
  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  construit_message( vs_menu_parent, XDL_GES57_FONC_PAS_IMPL );
}


/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de consultation du journal d'administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_jas_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;



  ps_TabEnregJournal = malloc( sizeof(tl_EnregJournal) );

  if ( ps_TabEnregJournal != NULL )
  {
  	/* recuperation des enregistrements du journal */
  	vl_ValRet = XZSP_07ConsultJournalAdmin(
		(void (*)())GMStockeListEnregJournal, &vl_numEnreg );

  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIJA */
  	create_LIJA  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIJA */
  	GMAfficheListeDansWidget(	LIJA_LI, 
			  	(void *)ps_TabEnregJournal, 
			  	vl_numEnreg, 
			  	CL_JOURNAL );

  	/* liberation memoire du tableau ps_TabEnregJournal */
  	free( ps_TabEnregJournal );

  	/* affichage de LIJA */
  	XtManageChild( LIJA_FM );
  }
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele par "Quitter" du menu d'administration
* 
* destruction des menus d administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int i;
  XZSCT_NomMachine	vl_NomMachine;


  /* destruction des menus d'administration */
   for ( i=0; i< vg_NumScreens; i++ )
  {
     XtDestroyWidget( vg_MENU[i].w_menu_fen);
     vs_GRSYN_FM[i]=vs_GRSYN_DA[i]=vs_GRTRA_FM[i]=vs_GRTRA_DA[i]=NULL;
  }

    /* appel de la fonction qui fait le menage */
    fonc_sortie_gmot();


}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDBSD (touche "Valider")
*
*  - recuperation du basculement desire
*    (type de basculement, periode)
*  - selon le type de basculement, appel de :
*    XZSA_07 ou XZSA_08
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdbsd_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int                  vl_ValRet;

	XZST_03EcritureTrace(XZSTC_FONCTION,"bdbsd_val_cb : IN \n");

       	/* basculement NON cyclique */
       	vl_ValRet = XZSE_07BasculementSD();
  	if ( vl_ValRet == XDC_NOK )
    	XZST_03EcritureTrace( XZSTC_WARNING,
	  	"bdbsd_val_cb: pb a l'execution de XZSE_07BasculementSD ");

  	if ( vl_ValRet == XDC_NOK )
    		construit_message( vs_menu_parent, XDL_GES48_BASC_SD_IMPOSS );

	XZST_03EcritureTrace(XZSTC_FONCTION,"bdbsd_val_cb : OUT \n");

}	/* Fin bdbsd_val_cb */


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele sur selection d'un toggle button
* grisant ou activant les widgets permettant de saisir 
* la periode du basculement (inutile si le basculement
* est non cyclique)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdbsd_to_chg_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  if ( (XmToggleButtonGetState( BDBSD_TO_CYC ) ) == True )
  {
    /* si le toggle "Cyclique" est selectionne, le champ "Periode" est actif */
    XtSetSensitive( BDBSD_CH_PER, True );
    XtSetSensitive( BDBSD_LA_1, True );
    XtSetSensitive( BDBSD_LA_2, True );
  }
  else
  {
    /* sinon il est grise */
    XtSetSensitive( BDBSD_CH_PER, False );
    XtSetSensitive( BDBSD_LA_1, False );
    XtSetSensitive( BDBSD_LA_2, False );
  }
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDBSC (double clic sur
* un element de la liste des sites)
*
*  - recuperation du basculement desire
*    (type de basculement, periode, site)
*  - selon le type de basculement, appel de :
*    XZSA_05 ou XZSA_06
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdbsc_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XZSCT_NomSite        	vl_site;
  tg_Basculement       	vl_basc;
  int                  	vl_ValRet;
  char                 	*pl_text;
  XmString		*pl_select_list;

  /* recuperation du site selectionne dans la liste */
  XtVaGetValues( BDBSC_LI_SIT, XmNselectedItems, &pl_select_list, NULL );
  XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
  strcpy( vl_site, pl_text );
  XtFree( pl_text );

       	/* basculement NON cyclique */
       	vl_ValRet = XZSE_05BasculementSC( vl_site );
  	if ( vl_ValRet == XDC_NOK )
    	XZST_03EcritureTrace( XZSTC_WARNING,
	  	"bdbsc_val_cb: pb a l'execution de XZSE_05BasculementSC ");

  if ( vl_ValRet == XDC_NOK )
      construit_message( vs_menu_parent, XDL_GES49_BASC_SC_IMPOSS );

  /* destruction de BDBSC */
  FermetureMere( w );
}





/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdbsc_to_chg_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  if ( (XmToggleButtonGetState( BDBSC_TO_CYC ) ) == True )
  {
    /* si le toggle "Cyclique" est selectionne, le champ "Periode" est actif */
    XtSetSensitive( BDBSC_CH_PER, True );
    XtSetSensitive( BDBSC_LA_1, True );
    XtSetSensitive( BDBSC_LA_2, True );
  }
  else
  {
    /* sinon il est grise */
    XtSetSensitive( BDBSC_CH_PER, False );
    XtSetSensitive( BDBSC_LA_1, False );
    XtSetSensitive( BDBSC_LA_2, False );
  }
}
 

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de selection de la liste des calculateurs 
* (double clic sur un element de la liste BDASS_LI_MAC)
*
* Rafraichit la liste des sous-systemes avec les sous-
* systemes du calculateur selectionne:
*  - recuperation du calculateur selectionne (stockage 
*    dans une variable statique vs_machine)
*  - recuperation des sous-systemes de ce calculateur 
*  - mise de ces sous-systemes dans la liste BDASS_LI_SSS
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdass_li_mac_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  char                 *pl_text;
  int              	vl_NbSSys;
  int              	vl_ValRet;
  XmString		*pl_select_list;

  /* recuperation du calculateur selectionne dans la liste */
  XtVaGetValues( BDASS_LI_MAC, XmNselectedItems, &pl_select_list, NULL );
  XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
  strcpy( vs_machine, pl_text );
  XtFree( pl_text );

  ps_TabSSys = malloc( sizeof(XZSCT_NomSSysteme) );

  if ( ps_TabSSys != NULL )
  {
  /* recuperation des sous-systemes de ce calculateur */
  vl_ValRet = XZSA_02ListeSSysteme( vs_machine, GMStockeListSSys, &vl_NbSSys, ps_TabSSys);

  /* mise de ces sous-systemes dans la liste BDASS_LI_SSS */
  GMAfficheListeDansWidget( BDASS_LI_SSS, ps_TabSSys, vl_NbSSys, CL_SSYS );

  free( ps_TabSSys );
  }

}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDASS
* (double clic sur un element de la liste BDASS_LI_SSS)
*
*  - recuperation du sous-systeme selectionne 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdass_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  char                 	*pl_text;
  XZSCT_NomSSysteme     vl_ssys;
  int                   vl_ValRet = XDC_OK;
  XmString		*pl_select_list;

  /* recuperation du sous-systeme selectionne dans la liste */
  XtVaGetValues( BDASS_LI_SSS, XmNselectedItems, &pl_select_list, NULL );
  if ( pl_select_list == NULL )
  {
      XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDATA_VAL : tache non identifiee");
      vl_ValRet = XDC_NOK;
  }
  else
  {
    XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
    strcpy( vl_ssys, pl_text );
    XtFree( pl_text );

    if ( ( vl_ssys , "" ) != 0 )
    {
    if ( (XmToggleButtonGetState( BDASS_TO_AR ) ) == True )
    {
       /* arret d'un sous-systeme */
       vl_ValRet = XZSP_02CmdSSyst( vs_machine, vl_ssys, XZSPC_ARRET );
       if ( vl_ValRet == XDC_NOK )
      		construit_message( vs_menu_parent, XDL_GES50_ARR_SSYS_IMPOSS );
    }
    else
       if ( (XmToggleButtonGetState( BDASS_TO_LA ) ) == True )
       {
          /* lancement d'un sous-systeme */
          vl_ValRet = XZSP_02CmdSSyst( vs_machine, vl_ssys, XZSPC_MARCHE );
  	  if ( vl_ValRet == XDC_NOK )
      		construit_message( vs_menu_parent, XDL_GES51_LAN_SSYS_IMPOSS );
       }
       else
       {
 	  XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDASS_VAL : aucun toggle selectionne" );		
	  return;
       }
    }
    else
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDASS_VAL : aucun sous-systeme selectionne" ); 
    }
  } 

  /* fermeture de BDASS */
  XtUnmanageChild(BDASS_FM );
}



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de GRTRA ( bouton Valider)
*
* - recupere le nom de la tache selectionnee dans
* la liste et les toggle buttons selectionnes
* - construit le niveau de trace (OU logique des
* niveaux selectionnes)
* - appelle XZST_06CmdNivVisuTrace
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
grtra_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   XZSCT_NomTache      	 vl_tache	= "";
   char                 *pl_text	= NULL;
   int			 vl_nivTrace	=0;
   int			 vl_visu	= 0;
   int			 vl_ValRet	= XDC_OK;
   XmString		*pl_select_list	= NULL;

   /* recuperation de la tache selectionnee dans la liste */
   XtVaGetValues( GRTRA_LI, XmNselectedItems, &pl_select_list, NULL );
   XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
   strcpy( vl_tache, pl_text );
   XtFree( pl_text );

	/* A Teste si tache non null */
	if (strcmp(vl_tache,"") != 0)
	{
  /* en fonction des toggle buttons selectionnes */
  /* calcul du niveau de trace (OU logique)      */
  if ( (XmToggleButtonGetState( GRTRA_TO_FO) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_FONCTION;
  if ( (XmToggleButtonGetState( GRTRA_TO_ME) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_MESSAGE;
  if ( (XmToggleButtonGetState( GRTRA_TO_IN) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_INFO;
  if ( (XmToggleButtonGetState( GRTRA_TO_BD) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_SGBD;
  if ( (XmToggleButtonGetState( GRTRA_TO_D1) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_DEBUG1;
  if ( (XmToggleButtonGetState( GRTRA_TO_D2) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_DEBUG2;
  if ( (XmToggleButtonGetState( GRTRA_TO_D3) ) == True )
     vl_nivTrace = vl_nivTrace | XZSTC_DEBUG3;
  if ( (XmToggleButtonGetState( GRTRA_TO_INTER) ) == True )
	  vl_nivTrace = vl_nivTrace | XZSTC_INTERFACE;
  if ( (XmToggleButtonGetState( GRTRA_TO_REGLE) ) == True )
	  vl_nivTrace = vl_nivTrace | XZSTC_REGLE;


  /* calcul du niveau de visualisation  */
  if ( (XmToggleButtonGetState( GRTRA_TO_EC) ) == True )
     vl_visu = XZSTC_ECRAN;
  else
     vl_visu = XZSTC_NECRAN;

  /* appel de XZST_06CmdNivVisuTrace avec le nouveau niveau */
  /* de trace et de visualisation                           */
  vl_ValRet = XZST_06CmdNivVisuTrace( vs_machine_tra, 
				  vl_tache, 
				  vl_nivTrace, 
				  vl_visu );

	}
	else
	{
		/* Deselectionne */
      		XmListDeselectAllItems(w);
		return;
	}

  XZST_03EcritureTrace( XZSTC_FONCTION,
		"gmot_adm, grtra_val_cb : appel a XZST_06CmdNivVisuTrace avec mac=%s, tache=%s, niveau=%d et visu=%d", vs_machine_tra, vl_tache, vl_nivTrace, vl_visu );

  if ( vl_ValRet == XDC_NOK )
      		construit_message( vs_menu_parent, XDL_GES58_COMM_TRAC_IMPOSS );

}	/* Fin grtra_val_cb */





/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Met a jour l etat des toggles en fonction du niveau 
* de trace de la tache passee en parametre
*
* - appelle GetStateTraceTache avec cette tache
*   et la machine pour recuperer le niveau de
*   trace de cette tache
* - positionne les toggle buttons a ON ou OFF
*   selon le retour de GetStateTraceTache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
SelectTogglesDeLaTache( XZSCT_NomTache   va_tache )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  tg_EtatTache		 vl_EtatTache;

  /* recuperation du niveau de trace de cette tache */
  GetStateTraceTache( vs_machine_tra, va_tache, &vl_EtatTache );

  /* positionnement des toggle buttons en fonction de vl_EtatTache */
  if ( vl_EtatTache.FO == 1 )
	XmToggleButtonSetState( GRTRA_TO_FO, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_FO, False, False );
  if ( vl_EtatTache.ME == 1 )
	XmToggleButtonSetState( GRTRA_TO_ME, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_ME, False, False );
  if ( vl_EtatTache.IN == 1 )
	XmToggleButtonSetState( GRTRA_TO_IN, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_IN, False, False );
  if ( vl_EtatTache.BD == 1 )
	XmToggleButtonSetState( GRTRA_TO_BD, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_BD, False, False );
  if ( vl_EtatTache.D1 == 1 )
	XmToggleButtonSetState( GRTRA_TO_D1, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_D1, False, False );
  if ( vl_EtatTache.D2 == 1 )
	XmToggleButtonSetState( GRTRA_TO_D2, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_D2, False, False );
  if ( vl_EtatTache.D3 == 1 )
	XmToggleButtonSetState( GRTRA_TO_D3, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_D3, False, False );
  if ( vl_EtatTache.Ecr == 1 )
	XmToggleButtonSetState( GRTRA_TO_EC, True, False );
  else
	XmToggleButtonSetState( GRTRA_TO_EC, False, False );
  if ( vl_EtatTache.INTER == 1 )
	XmToggleButtonSetState( GRTRA_TO_INTER, True, False );
  else
  	XmToggleButtonSetState( GRTRA_TO_INTER, False, False );
  if ( vl_EtatTache.REGLE == 1 )
	XmToggleButtonSetState( GRTRA_TO_REGLE, True, False );
  else
  	XmToggleButtonSetState( GRTRA_TO_REGLE, False, False );

}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de selection d'une tache de la liste 
* de GRTRA ( clic sur element )
* 
* - recupere le nom de la tache selectionnee dans
*   la liste
* - appelle SelectTogglesDeLaTache avec cette
*   tache pour selectionner les bons toggles
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
grtra_selec_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   XZSCT_NomTache      	 vl_tache;
   char                 *pl_text;
   XmString		*pl_select_list;


	XZST_03EcritureTrace(XZSTC_FONCTION, " grtra_selec_cb : IN \n");


   /* recuperation de la tache selectionnee dans la liste */
   XtVaGetValues( GRTRA_LI, XmNselectedItems, &pl_select_list, NULL );
   XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
   strcpy( vl_tache, pl_text );
   XtFree( pl_text );

	/*A Teste si une selection correcte */
	if (strcmp(vl_tache,"") != 0)
	{
   		/* positionnement des toggles */
   		SelectTogglesDeLaTache( vl_tache );
	}
	else
	{
		/* Deselectionne */
		XmListDeselectAllItems(w);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, " grtra_selec_cb : OUT \
		tache %s \n",vl_tache);
}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDSTA
*
* recupere le nom du calculateur saisi puis lance
* l'operation dont le type est passe en parametre,
* parmi:
* - affichage du synoptique des traces
* - affichage du synoptique des taches
* Si GRSYN ou GRTRA n'a pas encore ete cree, l'affichage est differe
* jusqu'au premier expose
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdsta_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   void GRTRA_TYP_TRA_exposeCb(Widget, XtPointer, XtPointer);
   void GRSYN_TYP_TAC_exposeCb(Widget, XtPointer, XtPointer);
   int	                vl_ValRet;
   XZSCT_NomTache      	vl_tache;
   XZSCT_NomMachine     vl_machine;
   XmString		*pl_select_list;
   char                 *pl_text;
   int   		vl_dummy;
   void  		*pl_dummy;
   int			vl_num_screen;


   /* recuperation de la machine selectionnee dans la liste */
   XtVaGetValues( BDSTA_LI_MAC, XmNselectedItems, &pl_select_list, NULL );
   XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
   strcpy( vl_machine, pl_text );
   XtFree( pl_text );

   /* suite des operations ... */
   switch (vs_ope_adm)
   {
     case cm_SYN_TRA :
	/* creation ou management de GRTRA */
	if(cree_ou_manage_GRTRA(vs_menu_parent, &vl_num_screen))
	{
	  /* affichage du synoptique des taches */
	  pl_text=malloc(strlen(vl_machine)+1);
	  strcpy(pl_text, vl_machine);

	  AfficheVueAdm( vs_GRTRA_DA[vl_num_screen], cg_TYP_TRA, vl_machine );
	  /*XtAddCallback(vs_GRTRA_DA[vl_num_screen], XmNexposeCallback,
			GRTRA_TYP_TRA_exposeCb, pl_text);*/
	}
	else
	{
	  AfficheVueAdm( vs_GRTRA_DA[vl_num_screen], cg_TYP_TRA, vl_machine );
	}

	/* memorisation du nom de la machine */
	strcpy( vs_machine_tra, vl_machine );

	/* affichage des taches de cette machine dans la liste */
	GMAfficheTachesDeMachine( GRTRA_LI, vs_machine_tra );

        /* la premiere tache etant selectionnee par defaut, on affiche */
	/* ses niveaux de trace grace aux toggles */
	grtra_selec_cb( NULL, NULL, NULL );

        break;

     case cm_SYN_STA :
	/* creation ou management de GRSYN */
	if(cree_ou_manage_GRSYN(vs_menu_parent, &vl_num_screen))
	{
	  /* affichage du synoptique des taches */
	  pl_text=malloc(strlen(vl_machine)+1);
	  strcpy(pl_text, vl_machine);

	  AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], cg_TYP_TAC, vl_machine );
	  /*XtAddCallback(vs_GRSYN_DA[vl_num_screen], XmNexposeCallback,
			GRSYN_TYP_TAC_exposeCb, pl_text);*/
	}
	else
	{
	  AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], cg_TYP_TAC, vl_machine );
	}

        break;

     default:
        /* Erreur */
	XZST_03EcritureTrace( XZSTC_WARNING,
		"gmot_adm, bdsta_val_cb : mauvais parametre d'entree " ); 
   }

  FermetureMere( w );
}



void GRTRA_TYP_TRA_exposeCb(Widget w,
		XtPointer client_data, XtPointer xt_call_data)
{
  XtRemoveCallback(w, XmNexposeCallback, GRTRA_TYP_TRA_exposeCb, client_data);
  AfficheVueAdm( w, cg_TYP_TRA, client_data );
  if(client_data)
    free(client_data);
}

void GRSYN_TYP_TAC_exposeCb(Widget w,
		XtPointer client_data, XtPointer xt_call_data)
{
  XtRemoveCallback(w, XmNexposeCallback, GRSYN_TYP_TAC_exposeCb, client_data);
  AfficheVueAdm( w, cg_TYP_TAC, client_data );
  if(client_data)
    free(client_data);
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDSTA
*
* recupere le nom du calculateur saisi puis lance
* l'operation dont le type est passe en parametre,
* parmi:
* - affichage du synoptique des traces
* - affichage du synoptique des taches
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdasu_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   int	                vl_ValRet = XDC_OK;
   XZSCT_NomMachine     vl_machine;
   char                 *pl_text;
   XmString		*pl_select_list;

   /* recuperation de la machine selectionnee dans la liste */
   XtVaGetValues( BDASU_LI_MAC, XmNselectedItems, &pl_select_list, NULL );
   XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
   strcpy( vl_machine, pl_text );
   XtFree( pl_text );

  if ( ( vl_machine , "" ) != 0 )
  {
   if ( (XmToggleButtonGetState( BDASU_TO_AR ) ) == True )
   {
        /* arret de MIGRAZUR */
        vl_ValRet = XZSP_01CmdApplication( vl_machine , XZSPC_ARRET );
	if ( vl_ValRet == XDC_NOK )
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, 
	    "gmot_adm, bdasu_val_cb probleme a XZSP_01CmdApplication pour %s avec XZSP_ARRET", vl_machine );
      	  construit_message( vs_menu_parent, XDL_GES52_ARR_MIGRAZUR_IMPOSS );
	}
   }
   else
     if ( (XmToggleButtonGetState( BDASU_TO_LA ) ) == True )
     {
        /* redemarrage de MIGRAZUR */
        vl_ValRet = XZSP_01CmdApplication( vl_machine, XZSPC_MARCHE );
	if ( vl_ValRet == XDC_NOK )
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, 
	    "gmot_adm, bdasu_val_cb probleme a XZSP_01CmdApplication pour %s avec XZSP_MARCHE", vl_machine );
      	  construit_message( vs_menu_parent, XDL_GES53_LAN_MIGRAZUR_IMPOSS );
	}
     }
     else
     {
        XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDASU_VAL : aucun toggle selectionne");
	return;
     }
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDASU_VAL : aucune machine selectionnee");
  }


  /* fermeture de BDASU */
  FermetureMere( w );

}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDACA
*
* recuperation du calculateur saisi et du type 
* d'arret desire, puis execution de l'ordre d'arret
* avec ou sans relance
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdaca_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
   int	                vl_ValRet = XDC_OK;
   XZSCT_NomMachine 	vl_machine;
   char                 *pl_text;
   XmString		*pl_select_list;

   /* recuperation de la machine selectionnee dans la liste */
   XtVaGetValues( BDACA_LI_MAC, XmNselectedItems, &pl_select_list, NULL );
   XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
   strcpy( vl_machine, pl_text );
   XtFree( pl_text );

  if ( ( vl_machine , "" ) != 0 )
  {
   /* suite des operations ... */
   switch (vs_ope_adm)
   {
     case cm_ARR_CAL :
   	if ( (XmToggleButtonGetState( BDACA_TO_AVE ) ) == True )
	{
	   /* arret d'un calculateur avec relance */
           vl_ValRet = XZSP_04ArretMachine( vl_machine , XZSPC_MARCHE );
	   if ( vl_ValRet != XDC_OK )
	   {
	      XZST_03EcritureTrace( XZSTC_WARNING,
	      	"gmot_adm, bdaca_val_cb: appel a XZSP_04ArretMachine pour %s avec XZSPC_MARCHE", vl_machine );
      	      construit_message( vs_menu_parent, XDL_GES54_ARRMAC1_IMPOSS );
	   }

	}
	else
	{
	   /* arret d'un calculateur sans relance */
           vl_ValRet = XZSP_04ArretMachine( vl_machine , XZSPC_ARRET );
	   if ( vl_ValRet != XDC_OK )
	   {
	      XZST_03EcritureTrace( XZSTC_FONCTION,
	         "gmot_adm, bdaca_val_cb: appel a XZSP_04ArretMachine pour %s avec XZSPC_ARRET", vl_machine );
      	      construit_message( vs_menu_parent, XDL_GES55_ARRMAC2_IMPOSS );
	   }

	}
        break;

     default:
        /* Erreur */
	XZST_03EcritureTrace( XZSTC_WARNING,
		"gmot_adm, bdaca_val_cb : mauvais parametre d'entree " ); 
   }
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDACA_VAL : machine non selectionnee");
  }

  /* fermeture de BDACA */
  FermetureMere( w );
}

/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDSIT
*
* - recuperation du site selectionne, puis affichage du
*   synoptique des sous-systemes 
* Si GRSYN n'a pas encore ete cree, l'affichage est differe jusqu'au
* premier expose
*   
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdsit_val_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  void GRSYN_TYP_SSS_exposeCb(Widget, XtPointer, XtPointer);
  XZSCT_NomSite        	vl_site;
  char                 	*pl_text;
  XmString		*pl_select_list;
  int			vl_num_screen;

  /* recuperation du site selectionne dans la liste */
  XtVaGetValues( BDSIT_LI_SIT, XmNselectedItems, &pl_select_list, NULL );
  XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
  strcpy( vl_site, pl_text );
  XtFree( pl_text );

  /* suite des operations ... */
  switch (vs_ope_adm)
  {
     case cm_SYN_SSS :

	/*A Affichage Synoptique Sous-Systeme */
	pl_text=malloc(strlen(vl_site)+1);
	strcpy(pl_text, vl_site);

	GMAffich_SSSYT(vs_menu_parent,pl_text,XDC_FAUX);

       	break;

     case cm_SYNC_DISK :

	/*A Demande de synchronisation des disques miroirs */
	pl_text=malloc(strlen(vl_site)+1);
	strcpy(pl_text, vl_site);

	gmot_synchro_disk(pl_text);

	break;

     default:
       	/* Erreur */
	XZST_03EcritureTrace( XZSTC_WARNING,
		"gmot_adm, bdsit_val_cb : mauvais parametre d'entree " ); 
  }

  /* fermeture de BDSIT */
  FermetureMere( w );
}



void GRSYN_TYP_SSS_exposeCb(Widget w,
		XtPointer client_data, XtPointer xt_call_data)
{
  XtRemoveCallback(w, XmNexposeCallback, GRSYN_TYP_SSS_exposeCb, client_data);
  AfficheVueAdm( w, cg_TYP_SSS, client_data );
  if(client_data)
    free(client_data);
}


/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de selection de la liste des calculateurs 
* (double clic sur un element de la liste BDATA_LI_MAC)
*
* Rafraichit la liste des taches avec les taches
* du calculateur selectionne:
*  - recuperation du calculateur selectionne (stockage 
*    dans une variable statique vs_machine)
*  - recuperation des taches de ce calculateur 
*  - mise de ces taches dans la liste BDATA_LI_TAC
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdata_li_mac_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  char                 *pl_text		= NULL;
  int              	vl_NbTache	= 0;
  int              	vl_ValRet	= XDC_OK;
  XmString		*pl_select_list	= NULL;
  int                   vl_Ind          = XDC_VRAI;


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"bdata_li_mac_cb : IN \n");


  /* recuperation du calculateur selectionne dans la liste */
  XtVaGetValues( BDATA_LI_MAC, XmNselectedItems, &pl_select_list, NULL );
  XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
  strcpy( vs_machine, pl_text );
  XtFree( pl_text );

/*  ps_TabTache = malloc( sizeof(tl_NomTache) );*/
  /*ps_TabTache = calloc(1,sizeof(tl_NomTache) );*/
  ps_TabTache = calloc( CL_SIZDEF ,sizeof(tl_NomTache) );
  
  if ( ps_TabTache != NULL )
  {
  	/* recuperation des taches de ce calculateur */
  	vl_ValRet = XZSA_03ListeTacheMachine( vs_machine, 
				    GMStockeListTache, 
				    &vl_NbTache, 
				    &vl_Ind);

  	/* mise de ces taches dans la liste BDATA_LI_TAC */
  	GMAfficheListeDansWidget( BDATA_LI_TAC, ps_TabTache, 
				vl_NbTache, CL_TACHE );

  	XZST_03EcritureTrace(XZSTC_DEBUG3,"bdata_li_mac_cb:ps_TabTache %ld %s",ps_TabTache,ps_TabTache[0].vl_NomTache );
	free( ps_TabTache );
	ps_TabTache=NULL;
  }

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"bdata_li_mac_cb : OUT  \n");

}	/* Fin bdata_li_mac_cb */



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDATA
* (double clic sur un element de la liste BDATA_LI_TAC)
*
*  - recuperation de la tache selectionne 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
bdata_val_cb (Widget w, int *client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  char                 *pl_text		= NULL;
  XZSCT_NomTache        vl_tache	= "";
  int                   vl_ValRet 	= XDC_OK;
  XmString		*pl_select_list	= NULL;


	XZST_03EcritureTrace(XZSTC_FONCTION,"bdata_val_cb  : IN \
		-- Valeur client %d \n",*client_data);

  /* recuperation de la tache selectionnee dans la liste */
  XtVaGetValues( BDATA_LI_TAC, XmNselectedItems, &pl_select_list, NULL );

  if ( pl_select_list == NULL ) 
  {
      XZST_03EcritureTrace( XZSTC_WARNING, "gmot_adm: BDATA_VAL : tache non identifiee");
      vl_ValRet = XDC_NOK;
  }
  else
  {
    XmStringGetLtoR( pl_select_list[0], XmSTRING_DEFAULT_CHARSET, &pl_text );
    strcpy( vl_tache, pl_text );
    XtFree( pl_text );
      
      XZST_03EcritureTrace( XZSTC_DEBUG1, 
		"Recuperation Tache select %s \n",vl_tache);

    if ( ( strcmp( vl_tache , "" ) != 0 ) && (*client_data == 0) ) 
    {
    	if ( (XmToggleButtonGetState( BDATA_TO_AR ) ) == True )
	{
       		/* arret d'une tache */
       		vl_ValRet = 
			XZSP_03CmdTache( vs_machine, vl_tache, XZSPC_ARRET );
	}
    	else if ( (XmToggleButtonGetState( BDATA_TO_LA ) ) == True )
	{
          	/* lancement d'une tache */
          	vl_ValRet = 
		  XZSP_03CmdTache( vs_machine, vl_tache, XZSPC_MARCHE );
	}
       	else
       	{
        	XZST_03EcritureTrace( XZSTC_WARNING, 
		  "gmot_adm: BDATA_VAL : pas de toggle selectionne ");
	  	return;
       	}
    }
    else if ( ( strcmp( vl_tache , "" ) != 0 ) && (*client_data == 1) ) 
    {
	vl_ValRet = XDC_NOK;
    }
    else if (strcmp( vl_tache , "" ) == 0)
    {
      /*B Deselectionne la Selection */
      XmListDeselectAllItems(w);
      return;
    }
  }
  
  if ( vl_ValRet == XDC_OK )
  { 
  	/* fermeture de BDATA */
  	FermetureMere( w );
     /*construit_message( vs_menu_parent, XDL_GES56_FONC_NON_EXEC );*/
  }


	XZST_03EcritureTrace(XZSTC_FONCTION,"bdata_val_cb  : OUT \n");

}	/* Fin bdata_val_cb */ 



/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Sont regroupees ici toutes les callbacks
* d'annulation de boites de dialogue
* (Push Button "Annuler" ou "Quitter")
* qui ne font que fermer la boite de 
* dialogue 
* ( celui du menu MENU est ailleurs )
* ( celui de GRTRA est pariculier: il unmanage GRTRA_FM)
*
*
******************************************************
* 
*/
void bdbsc_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{ 
  FermetureMere( w ); 
}

void bdbsd_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w );
}

void bdata_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void bdasu_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void bderr_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
 FermetureMere( w ); 
}

void bdass_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void grtra_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  XtUnmanageChild( XtParent( XtParent (w) ) ); 
  /*XtDestroyWidget( XtParent( XtParent (w) ) ); */
}

void bdsta_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
   FermetureMere( w ); 
}

void bdaca_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void bdsit_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void lija_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void livl_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
   	char 			vl_NomFichier[XZSSC_PATH_ABS_SIZE];


	/*A Destruction du Fichier de VL temporaire */
	sprintf(vl_NomFichier,"%s/%s",XDC_REP_TMP, XDF_NSUP_VERSLOG);
	if (XZSS_06DetruireFichier(vl_NomFichier))
    	{
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			  " pb a l'appel de XZSS_06DetruireFichier");
	}


	/*A Destruction du Fichier d'impression Temporaire */
	sprintf(vl_NomFichier,"%s/%s",XDC_REP_TMP, XDF_TMP_IMPRESSION);
	if (XZSS_06DetruireFichier(vl_NomFichier))
    	{
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			  " pb a l'appel de XZSS_06DetruireFichier");
	}

  	FermetureMere( w ); 

  	/*A liberation memoire du tableau ps_TabVerLog */
  	free( ps_TabVerLog );

  	/*A liberation memoire du tableau pg_ListVerLogFormat */
  	free( pg_ListVerLogFormat );


}	/* Fin livl_qui_cb */


void list_bl_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
   	char 			vl_NomFichier[XZSSC_PATH_ABS_SIZE];


	/*A Destruction du Fichier */
	sprintf(vl_NomFichier,"%s/%s",XDC_REP_TMP,XDF_BON_LIVRAISON_TMP);
	if (XZSS_06DetruireFichier(vl_NomFichier))
    	{
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			  " pb a l'appel de XZSS_06DetruireFichier");
	}

	/*A Destruction du Fichier d'impression Temporaire */
	sprintf(vl_NomFichier,"%s/%s",XDC_REP_TMP, XDF_TMP_IMPRESSION);
	if (XZSS_06DetruireFichier(vl_NomFichier))
    	{
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			  " pb a l'appel de XZSS_06DetruireFichier");
	}


	/*A Fermeture de la fenetre */
  	FermetureMere( w ); 

  	/*A liberation memoire du tableau ps_TabEnregJournal */
  	free( ps_TabEnregBL );
	
	/*A Initialisation du Nombre d'enregistrements */
	vg_numEnreg_BL = 0;


}	/*Fin list_bl_qui_cb */




void list_bc_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void list_arc_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void list_brbh_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void list_bsbh_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}

void list_hebdo_qui_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
  FermetureMere( w ); 
}



/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* demande de consultation du journal d'administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
menu2_ijo_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  int vl_ValRet;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"menu2_ijo_cb :  IN \n");


	/*A Impression du Journal */
	if (XZSP_10ImprimeJournalAdmin() != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, 
			"pb a l'a ppel de XZSS_10ImprimeJournalAdmin \n");
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"menu2_ijo_cb :  OUT \n");

}	/* Fin menu2_ijo_cb */

/*X*/
/* Fichier : $Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $		        Release : $Revision: 1.15 $              Date : $Date: 1996/09/19 17:28:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* creation ou management de GRMACH_SYNT
* - recuperation du numero de screen du parent
* - si GRSYN_FM existe pour ce screen (vs_GRSYN_FM non NULL)
*   - creation par create_GRSYN
*   - valorisation de vs_GRSYN_FM et vs_GRSYN_DA
* - sinon
*   - management du widget
*
******************************************************
* SEQUENCE D'APPEL :
*/

int cree_ou_manage_GRMACH_SYNT(Widget w,int p_num_screen)

/*
* PARAMETRES EN ENTREE :
*
* w: parent
*
* PARAMETRES EN SORTIE :
*
* num_screen: numero de screen du parent
*
* VALEUR(S) DE RETOUR :
* 1 si creation
* 0 si management
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{

  void GRSYN_TYP_SYNT_exposeCb(Widget, XtPointer, XtPointer);

	XZST_03EcritureTrace(XZSTC_FONCTION,
	"cree_ou_manage_GRMACH_SYNT : IN -- Num Screen %d \n",
		p_num_screen);

  /* creation de GRSYN la premiere fois uniquement */
  if(!vs_GRMACH_SYNT_FM[p_num_screen])
  {
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Creation GRSYN pour la 1er fois \n");

	create_GRMACH_SYNT(w, vg_mrm_hierarchy,&vg_class);

	vg_GRMACH_SYNT_FM[p_num_screen] = GRMACH_SYNT_FM;
	vs_GRMACH_SYNT_FM[p_num_screen] = GRMACH_SYNT_FM;
	vs_GRMACH_SYNT_DA[p_num_screen] = GRMACH_SYNT_DA;

    	/* affichage du synoptique */

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Avant declenchement Callback Widegt %d \n",
		GRMACH_SYNT_DA);

    	AfficheVueAdm( vs_GRMACH_SYNT_DA[p_num_screen], cg_TYP_SYNT, NULL );
    	/*XtAddCallback(vs_GRMACH_SYNT_DA[p_num_screen], 
		XmNexposeCallback, GRSYN_TYP_SYNT_exposeCb, NULL);*/
  }
  else
  {
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Creation GRSYN apres la 1er fois \n");

    /* affichage du synoptique */
    XtManageChild ( vs_GRMACH_SYNT_FM[p_num_screen] );
    AfficheVueAdm( vs_GRMACH_SYNT_DA[p_num_screen], cg_TYP_SYNT, NULL );
  }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"cree_ou_manage_GRMACH_SYNT : OUT \n");

}	/* Fin cree_ou_manage_GRMACH_SYNT */




void GRSYN_TYP_SYNT_exposeCb(Widget w,
		XtPointer client_data, XtPointer xt_call_data)
{
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GRSYN_TYP_SYNT_exposeCb : IN \n");


  XtRemoveCallback(w, XmNexposeCallback, 	
		GRSYN_TYP_SYNT_exposeCb, client_data);
  AfficheVueAdm( w, cg_TYP_SYNT, client_data );
  if(client_data)
    free(client_data);

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GRSYN_TYP_SYNT_exposeCb : OUT \n");

}	/* GRSYN_TYP_SYNT_exposeCb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'aaficher le synoptique Sous-System
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void GMAffich_SSSYT(
	Widget	va_MenuPere,
	char *	va_NomSite,
	int	va_ModeAffich)


/*
* ARGUMENTS EN ENTREE :
*   va_MenuPere : Widget Pere 
*   va_NomSite : Nom du Site 
*   va_ModeAffich : Mode Affichage 
*	(XDC_VRAI si Affichage a partir de la fenetre de Synthese
*	 XDC_FAUX sinon )
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucu,e
*
* FONCTION 
*   Recupere le nimm du site et affiche le synoptique 
*	associe
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMAffich_SSSYT " ;
	int vl_num_screen	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,"GMAffich_SSSYT : IN \
		-- Site %s \n",va_NomSite);


	/* creation ou management de GRSYN */
	if(cree_ou_manage_GRSYN(va_MenuPere, &vl_num_screen))
	{
	  	AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], 
				cg_TYP_SSS, va_NomSite );
	  	/* affichage du synoptique des taches */
	  /*	XtAddCallback(vs_GRSYN_DA[vl_num_screen], 
			XmNexposeCallback,GRSYN_TYP_SSS_exposeCb, 
			va_NomSite);*/

		/*A Teste le mode d'affichage */
		if ( va_ModeAffich == XDC_VRAI)
		{
			/*A Affichage de la Vue */
			AfficheVueAdm( vs_GRSYN_DA[vl_num_screen],
				cg_TYP_SSS, va_NomSite );
		}
	}
	else
	{
	  AfficheVueAdm( vs_GRSYN_DA[vl_num_screen], 
				cg_TYP_SSS, va_NomSite );
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
				"GMAffich_SSSYT : OUT \n");

}	/* Fin GMAffich_SSSYT */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de sauvegarder avant verification
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sauve_a_verif_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Agumentd de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucune
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : sauve_a_verif_cb " ;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	/* Sauvegarde avant Verification */
	if (xzcg06_DuplicationBD(XZCGC_SAUVEGARDE_CFG) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB Sauvegarde avnt Verif \n");
	}
	
}	/* Fin sauve_a_verif_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de sauvegarder avant verification
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void restore_a_verif_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucune
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : restore_a_verif_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	/* Sauvegarde avant Verification */
	if (xzcg06_DuplicationBD(XZCGC_RESTAURATION_DANS_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB Restauration avant Verif \n");
	}
	

}	/* Fin restore_a_verif_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de verifier le Reseau autoroutier
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_reseau_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_reseau_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
	}

	/* Verification du Reseau */
	if( xzcg04_Verification_Coherence(XZCGC_VERIF_RESE) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB verifiaction Reseau \n");
	}
	
}	/* Fin verif_reseau_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de verifier les equipements
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_equip_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_equip_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
	}

	/* Verification du Reseau */
	if( xzcg04_Verification_Coherence(XZCGC_VERIF_EQUI) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB verifiaction Equipement \n");
	}
	

}	/* Fin verif_equip_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Peremet de verifier SAIDEC Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_SAIDEC_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )


/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_SAIDEC_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	

	/* Verification du Reseau */
	if (xzcg04_Verification_Coherence(XZCGC_VERIF_RSAD) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB verifiaction SAIDEC \n");
	}
	

}	/* Fin verif_SAIDEC_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_globale_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )


/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_globale_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "gmot.x : xzcg_ChgmtBaseCour () a echoue");
	}

	/* Verification du Reseau */
	if (xzcg04_Verification_Coherence(XZCGC_VERIF_GLOBAL) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB verifiaction globale \n");
	}
	

}	/* Fin verif_globale_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void modif_reseau_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )


/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : modif_reseau_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFG) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "gmot.x : xzcg_ChgmtBaseCour () a echoue");
	}

	/* Modification du Reseau */
	if (xzcg04_Verification_Coherence(XZCGC_VERIF_RESE) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB Modification Reseau \n");
	}
	

}	/* Fin modif_reseau_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void modif_equip_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )


/*
* ARGUMENTS EN ENTREE :
*   Aguments de Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Appel fonction d'interface  xzcg04.... 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : modif_equip_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFG) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "gmot.x : xzcg_ChgmtBaseCour () a echoue");
	}

	/* Modification Equipement */
	if (xzcg04_Verification_Coherence(XZCGC_VERIF_EQUI) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB modification Equipement \n");
	}
	

}	/* Fin modif_equip_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du BC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregBC( 	
			int             va_nb,
			char 		* pa_Libelle )

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   pa_Libelle : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregBC " ;

   	ps_TabEnregBC = realloc( ps_TabEnregBC, 
				 (va_nb+1) * sizeof(tl_EnregBC) );

   if ( ps_TabEnregBC == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregBC: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*					dans le tableau */
	if (strlen(pa_Libelle) != 0)
	{
   		strcpy( ps_TabEnregBC[va_nb], pa_Libelle ); 
	}
	else
	{
   		strcpy( ps_TabEnregBC[va_nb], "");
	}
   }
	
}	/* Fin GMStockeListEnregBC */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de consulter le Bilan de Configuration
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_bc_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_bc_cb " ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregBC = malloc( sizeof(tl_EnregBC) );

  if ( ps_TabEnregBC != NULL )
  {
  	vl_ValRet = consult_BC(
		(void (*)())GMStockeListEnregBC, &vl_numEnreg );

  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_BC */
  	create_LIST_BC  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIBC_LI */
  	GMAfficheListeDansWidget(LIST_BC_LI, 
			  	(void *)ps_TabEnregBC, 
			  	vl_numEnreg, 
			  	CL_BC );

  	/* liberation memoire du tableau ps_TabEnregJournal */
  	free( ps_TabEnregBC );

  	/* affichage de LIST_BC */
  	XtManageChild( LIST_BC_FM );
   }
}	/* Fin lecture_bc_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Vide le Bilan de Svg Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void list_BC_vider_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ :  list_BC_vider_cb" ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;
  	char vl_Cmd[250]="";
	XZSCT_NomMachine      	vl_machineLocale = "";
  	
	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BRBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}
	sprintf (vl_Cmd, "rm %s/%s", XDC_REP_TMP, XDF_BILAN_CONFIG );
	system(vl_Cmd);
	sprintf (vl_Cmd, "touch %s/%s", XDC_REP_TMP, XDF_BILAN_CONFIG );
	system(vl_Cmd);
	if (XZSS_07TransfertFichier (   vl_machineLocale,
					XDF_BILAN_CONFIG,
					XDC_REP_TMP,
					XDC_MACHINE_GECONF,
					XDF_BILAN_CONFIG,
					XDC_PATHCNFFICDYN) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "list_BC_vider_cb: Transfert du fichier %s incorrect de %s a %s",
		    XDF_BILAN_CONFIG, vl_machineLocale, XDC_MACHINE_GECONF);
		 vl_ValRet = XZSPC_FIC_INV;
	}
	FermetureMere( w );

}	/* Fin list_BC_vider_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier Bilan Config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_BC(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_BC " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	FILE*                 pl_idJournal = NULL;
	char                  vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
	int			cr	= XDC_NOK;
	char                  vl_args [XDC_LINE_CMD_SIZE] = "";


	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BC: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_BILAN_CONFIG,
					XDC_PATHCNFFICDYN,
					vl_machineLocale,
					XDF_BILAN_CONFIG_TMP,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_BC: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BILAN_CONFIG, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_BILAN_CONFIG_TMP);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_BC: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_BILAN_CONFIG_TMP);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_BC: Positionnement impossible \
		 au debut de %s", XDF_BILAN_CONFIG_TMP);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
		while ( (fgets(vl_args,sizeof(tl_EnregBC),pl_idJournal) != NULL))
		{
			XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Enr BC %s \n",vl_args);

			vl_args[strlen(vl_args)-1] = (char)0;
			pa_FonctionUtil(*pa_NumEnr, vl_args);
			(*pa_NumEnr) ++;
		}			
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
			"\n\n Fin Lecture BC \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_BC: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
	unlink (vl_pathJournal);

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_BC OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_BC */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier BON LIVRAISON Config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_BL(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   (*pa_FonctionUtil) : Fonction utilisateur
*   pa_NumEnr		: Nombre d'enregistrement
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom de la machine locale, transfere le fichier de la
*	machine de configuration dans un fichier temporaire sur
*	la machine locale , puis lecture des enregistrements et memorisation
*	dans un tableau.
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_BL " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	char                  vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
	int			cr	= 0;
	XZCGT_ComposantBL     vl_Tab_ComposantBL;




	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BL: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_BON_LIVRAISON,
					XDC_PATHCNFFICDYN,
					vl_machineLocale,
					XDF_BON_LIVRAISON_TMP,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_BL: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BON_LIVRAISON, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_BON_LIVRAISON_TMP);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal_BL ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_BC: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_BON_LIVRAISON_TMP);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal_BL, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_BL: Positionnement impossible \
		 au debut de %s", XDF_BON_LIVRAISON_TMP);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
	   while ( fscanf(pl_idJournal_BL,"%s %s %s %s %lf",
			vl_Tab_ComposantBL.TypeFic,
			vl_Tab_ComposantBL.NomMachine,
			vl_Tab_ComposantBL.NomFichier,
			vl_Tab_ComposantBL.NumVersion,
			&vl_Tab_ComposantBL.Horodate) != EOF)
		{
			pa_FonctionUtil(*pa_NumEnr, vl_Tab_ComposantBL);
			(*pa_NumEnr) ++;
		}
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
				"\n\n Fin Lecture BL \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal_BL)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_BL: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_BL OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_BL */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du LB
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregBL( 	
	int             va_nb,
	XZCGT_ComposantBL  va_Tab_ComposantBL)

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   pa_Libelle : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregBL " ;
	XZSMT_Horodate vl_Horodate           = "";
	int		vl_ValRet	= XDC_OK;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMStockeListEnregBL : IN  \n");

   	ps_TabEnregBL = realloc( ps_TabEnregBL, 
		 (va_nb+1) * sizeof(tl_EnregBL) );

   if ( ps_TabEnregBL == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregBL: pb au realloc " );
   else
   {

	/*B Recuperation de l'horodate sous forme de texte */
	if ( vl_ValRet = (XZSM_11ConversionHorodate(va_Tab_ComposantBL.Horodate,
					&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Pb de conversion de l'horodate \n");
	}

					
   	/* ecriture avec le format precedent des arguments 
	*		dans le tableau */
	sprintf (ps_TabEnregBL[va_nb],"%-20s%-10s%-10s%s",
		va_Tab_ComposantBL.TypeFic ,
		va_Tab_ComposantBL.NomMachine,
		va_Tab_ComposantBL.NumVersion,
		vl_Horodate );
   }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMStockeListEnregBL : OUT \n");

}	/* Fin GMStockeListEnregBL */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le Bon de Livraison
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_bl_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_bl_cb " ;
  	int vl_ValRet 		= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";



	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregBL = malloc( sizeof(tl_EnregBL) );

  if ( ps_TabEnregBL != NULL )
  {
	/*A Consultation du Blan Configuration */	
  	vl_ValRet = consult_BL(
		(void (*)())GMStockeListEnregBL, &vg_numEnreg_BL );


  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_BL */
  	create_LIST_BL  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIST_BL */
  	GMAfficheListeDansWidget(LIST_BL_LI, 
			  	(void *)ps_TabEnregBL, 
			  	vg_numEnreg_BL, 
			  	CL_BL );

  	/* affichage de LIST_BL */
  	XtManageChild( LIST_BL_FM );
   }


}	/* Fin lecture_bl_cb */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de livrer
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void livrer_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*    Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Appel de la fonction d'interface xzcg01...
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : livrer_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"PB pour recevoir les messages de TCGCD. \n");
	}
	

	/*A Livraison */
	if (xzcg01_Livraison() != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Pb Livarison \n");
	}
	
}	/* Fin livrer_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet la Diffusion
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void diffuser_bl_cb (	
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*    Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Appel de la fonction d'interface xzcg03...
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : diffuser_bl_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	

	/*A Diffusion */
	if (xzcg03_Diffusion_Fichiers() != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Pb Diffusion \n");
	}
	

}	/* Fin diffuser_bl_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'ajouter des tests lors de l'intergartion
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tests_1_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*    Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Appel de la fonction UTILISATEUR
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : tests_1_cb " ;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	

	/*** Active Checklist */
	if (xzcg_ActivChecklist(XDC_OK) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB activation Checklist \n");
	}
	

}	/* Fin tests_1_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'ajouter des tests lors de l'intergartion
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tests_2_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*    Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Appel de la fonction UTILISATEUR
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : tests_2_cb " ;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	

	/*** Active Checklist */
	if (xzcg_ActivChecklist(XDC_NOK) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB desactivation Checklist \n");
	}
	
}	/* Fin tests_2_cb */



void tests_3_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
	int	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine	vl_NomMachine = "";

	/*A Recuperation de la machine */
	XZSC_07NomMachine(vl_NomMachine);

	/*A Envoi de la demande de MAJ du fichier d'administration 
	*	a la machine locale */
 	if ( (vl_ValRet = XZSP_05MajFichierAdm(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"PB dans MAJ fichier Administartion \n");
	}
}



void tests_4_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )
{
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sauve_archiv_cb (Widget w, XtPointer client_data, XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : sauve_archiv_cb " ;
	XDY_NomMachine  vl_NomMachine   = "";
	int             vl_ValRet       = XDC_OK;
		 
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD.\n");
	}
				  
	if ( (vl_ValRet = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " xzca_EnvoieNomMachineLocal : Recuperation du Nom de la machine impossible .");
	}
												  
	if (xzca_EnvoieNomMachineLocal () != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}

	if((vl_ValRet = XZIT09_Dialogue_Sauvegarde_Histo (vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZIT09_Dialogue_Sauvegarde_Histo est sortie en erreur");
	}

	

}	/* Fin sauve_archiv_cb */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de restaurer le mois archive de la base HISTO dans la base de Donnees SVG
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void restaure_archiv_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
taure_archiv_cb CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : restaure_archiv_cb " ;
	XDY_NomMachine  vl_NomMachine   = "";
	int             vl_ValRet       = XDC_OK;


	if (xzca_EnvoieNomMachineLocal () != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCSVA. \n");
	}

	if ( (vl_ValRet = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " xzca_EnvoieNomMachineLocal : Recuperation du Nom de la machine impossible .");
	}

	if((vl_ValRet = XZIT10_Dialogue_Restauration_Historique (vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZIT09_Dialogue_Sauvegarde_Histo est sortie en erreur");
	}


}	/* Fin restaure_archiv_cb */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_defaut_cb( 
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_defaut_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
	}

	if (xzcg04_Verification_Coherence (XZCGC_VERIF_DEFT)!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer la demande de verification des fichiers Defauts. \n");
	}

}	/* Fin verif_defaut_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_seuil_tf_cb( 
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_seuil_tf_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
	}
	if ( xzcg04_Verification_Coherence(XZCGC_VERIF_SCNT)!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB rencontrer losde la demande de verification des Seuils de niveaux de traffic");
	}

}	/* Fin verif_seuil_tf_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void verif_equ_al_cb( 
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : verif_equ_al_cb " ;
	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
	if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
	}
	if ( xzcg04_Verification_Coherence(XZCGC_VERIF_EQAL)!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB rencontrer lors de la demande de verification des equations d alertes");
	}


}	/* Fin verif_equ_al_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void configurer_equt_cb(
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : configurer_equt_cb " ;
	XZSCT_NomMachine        vl_machineLocale = "";

	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
	      "configurer_equt_cb: nom de machine invalide");
      	}


	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"configurer_equt_cb:PB pour recevoir les messages de TCGCD. \n");
	}
	
	if (XZIT11_Dialogue_Reconfig_machine (vl_machineLocale)!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"configurer_equt_cb:PB pour declencher la reconfiguration d une machine ayant des equipements. \n");
	}


}	/* Fin configurer_equt_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier CATALOGUE ARCHIVE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_ARC(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   *pa_FonctionUtil : Fonction Utilisateur
*
*
* ARGUMENTS EN SORTIE :
*   pa_NumEnr  : Nombre d'enregistrements
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom de la machine locale, copie dans le 
*	repertoire temporaire XDC_REP_TMP le fichier catalogue
*	puis parcourt celui-ci et traite chaque enrgeistrement 
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_BL " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	FILE*                 pl_idJournal = NULL;
	char                  vl_pathFichier[XDC_PATH_ABS_SIZE] = "";
	int			cr	= 0;
	XZCAT_ComposantCAT     vl_Tab_ComposantARC;




	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BL: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_CATALOGUE_ARCHIVE,
					XDC_PATHFICDYN,
					vl_machineLocale,
					XDF_CATALOGUE_ARCHIVE,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_ARC: Transfert du fichier %s incorrect de %s a %s",
		   XDF_CATALOGUE_ARCHIVE, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathFichier, "%s/%s", XDC_REP_TMP, XDF_CATALOGUE_ARCHIVE);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathFichier,&pl_idJournal ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_ARC: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_CATALOGUE_ARCHIVE);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_ARC: Positionnement impossible \
		 au debut de %s", XDF_CATALOGUE_ARCHIVE);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
	   while (fread(&vl_Tab_ComposantARC,sizeof(XZCAT_ComposantCAT),1,pl_idJournal) > 0)
	   {	
			pa_FonctionUtil(*pa_NumEnr, vl_Tab_ComposantARC);
			(*pa_NumEnr) ++;
		}
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
				"\n\n Fin Lecture Catalogue Archive \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_ARC: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
	unlink (vl_pathFichier);

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_BL OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_ARC */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du CATALOGUE ARCHIVE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregARC( 	
	int             va_nb,
	XZCAT_ComposantCAT  va_Tab_ComposantARC)

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   va_Tab_ComposantARC : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregARC " ;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMStockeListEnregARC : IN  \n");

   	ps_TabEnregARC = realloc( ps_TabEnregARC, 
			 (va_nb+1) * sizeof(tl_EnregARC) );

   if ( ps_TabEnregARC == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregARC: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*			dans le tableau */
	sprintf (ps_TabEnregARC[va_nb],"%ld           %ld",
		va_Tab_ComposantARC.BandeID ,
		va_Tab_ComposantARC.NumCopie);
   }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMStockeListEnregARC : OUT \n");

}	/* Fin GMStockeListEnregARC */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le Bon de Livraison
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_catalogue_cb( 
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_catalogue_cb " ;

	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";



	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregARC = malloc( sizeof(tl_EnregARC) );

  if ( ps_TabEnregARC != NULL )
  {
	/*A Consultation du Catalogue d'Archives*/
  	vl_ValRet = consult_ARC(
		(void (*)())GMStockeListEnregARC, &vl_numEnreg );


  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_BL */
  	create_LIST_ARC  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIST_BL */
  	GMAfficheListeDansWidget(LIST_ARC_LI, 
			  	(void *)ps_TabEnregARC, 
			  	vl_numEnreg, 
			  	CL_ARC );

  	/* liberation memoire du tableau ps_TabEnregJournal */
  	free( ps_TabEnregARC );


  	/* affichage de LIST_BL */
  	XtManageChild( LIST_ARC_FM );
   }

}	/* Fin lecture_catalogue_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du BRBH
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregBRBH( 	
	int             va_nb,
	char 		* pa_Libelle )

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   pa_Libelle : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregBRBH " ;

   	ps_TabEnregBRBH = realloc( ps_TabEnregBRBH, 
				 (va_nb+1) * sizeof(tl_EnregBRBH) );

   if ( ps_TabEnregBRBH == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregBRBH: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*					dans le tableau */
	if (strlen(pa_Libelle) != 0)
	{
   		strcpy( ps_TabEnregBRBH[va_nb], pa_Libelle  );
	}
	else
	{
   		strcpy( ps_TabEnregBRBH[va_nb], "" );
	}
   }
	
}	/* Fin GMStockeListEnregBRBH */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de consulter le Bilan de Rest Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_brbh_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_brbh_cb " ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregBRBH = malloc( sizeof(tl_EnregBRBH) );

  if ( ps_TabEnregBRBH != NULL )
  {
  	vl_ValRet = consult_BRBH(
		(void (*)())GMStockeListEnregBRBH, &vl_numEnreg );

  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_BRBH */
  	create_LIST_BRBH  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIBC_LI */
  	GMAfficheListeDansWidget(LIST_BRBH_LI, 
			  	(void *)ps_TabEnregBRBH, 
			  	vl_numEnreg, 
			  	CL_BRBH );

  	/* liberation memoire du tableau  */
  	free( ps_TabEnregBRBH );

  	/* affichage de LIST_BRBH */
  	XtManageChild( LIST_BRBH_FM );
   }
}	/* Fin lecture_brbh_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Vide le Bilan de Rest Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void list_brbh_vider_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ :  list_brbh_vider_cb" ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;
  	char vl_Cmd[250]="";
	XZSCT_NomMachine      	vl_machineLocale = "";
  	
	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BRBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}
	sprintf (vl_Cmd, "rm %s/%s", XDC_REP_TMP, XDF_BILAN_REST_BASE_HIST);
	system(vl_Cmd);
	sprintf (vl_Cmd, "touch %s/%s", XDC_REP_TMP, XDF_BILAN_REST_BASE_HIST);
	system(vl_Cmd);
	if (XZSS_07TransfertFichier (   vl_machineLocale,
					XDF_BILAN_REST_BASE_HIST,
					XDC_REP_TMP,
					XDC_MACHINE_GECONF,
					XDF_BILAN_REST_BASE_HIST,
					XDC_PATHCNFFICDYN) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "list_brbh_vider_cb: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BILAN_REST_BASE_HIST, vl_machineLocale, XDC_MACHINE_GECONF);
		 vl_ValRet = XZSPC_FIC_INV;
	}
	FermetureMere( w );

}	/* Fin list_brbh_vider_cb */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier Bilan Rest Base Hist
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_BRBH(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_BRBH " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	FILE*                 pl_idJournal = NULL;
	char                  vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
	int			cr	= XDC_NOK;
	char                  vl_args [XDC_LINE_CMD_SIZE] = "";


	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BRBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_BILAN_REST_BASE_HIST,
					XDC_PATHCNFFICDYN,
					vl_machineLocale,
					XDF_BILAN_REST_BASE_HIST,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_BC: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BILAN_REST_BASE_HIST, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_BILAN_REST_BASE_HIST);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_BRBH: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_BILAN_REST_BASE_HIST);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_BRBH: Positionnement impossible \
		 au debut de %s", XDF_BILAN_REST_BASE_HIST);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
		while ( (fgets(vl_args,sizeof(tl_EnregBRBH),pl_idJournal) != NULL))
		{
			vl_args[strlen(vl_args)-1] = (char)0;
			pa_FonctionUtil(*pa_NumEnr, vl_args);
			(*pa_NumEnr) ++;
		}			
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
				"\n\n Fin Lecture BRBH \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_BRBH: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
	unlink (vl_pathJournal);

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_BRBH OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_BRBH */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du BSBH
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregBSBH( 	
	int             va_nb,
	char 		* pa_Libelle )

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   pa_Libelle : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
 x*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregBSBH " ;

   	ps_TabEnregBSBH = realloc( ps_TabEnregBSBH, 
				 (va_nb+1) * sizeof(tl_EnregBSBH) );

   if ( ps_TabEnregBSBH == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregBSBH: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*					dans le tableau */
	if (strlen(pa_Libelle) != 0)
	{	
   		strcpy( ps_TabEnregBSBH[va_nb], pa_Libelle  );
	}
	else
	{
		strcpy( ps_TabEnregBSBH[va_nb], "");
	}
   }
	
}	/* Fin GMStockeListEnregBSBH */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de consulter le Bilan de Svg Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_bsbh_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_bsbh_cb " ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregBSBH = malloc( sizeof(tl_EnregBSBH) );

  if ( ps_TabEnregBSBH != NULL )
  {
  	vl_ValRet = consult_BSBH(
		(void (*)())GMStockeListEnregBSBH, &vl_numEnreg );

  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_BSBH */
  	create_LIST_BSBH  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIBSBRH_LI */
  	GMAfficheListeDansWidget(LIST_BSBH_LI, 
			  	(void *)ps_TabEnregBSBH, 
			  	vl_numEnreg, 
			  	CL_BSBH );

  	/* liberation memoire du tableau  */
  	free( ps_TabEnregBSBH );

  	/* affichage de LIST_BSBH */
  	XtManageChild( LIST_BSBH_FM );
   }
}	/* Fin lecture_bsbh_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Vide le Bilan de Svg Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void list_bsbh_vider_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ :  list_bsbh_vider_cb" ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;
  	char vl_Cmd[250]="";
	XZSCT_NomMachine      	vl_machineLocale = "";
  	
	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BRBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}
	sprintf (vl_Cmd, "rm %s/%s", XDC_REP_TMP, XDF_BILAN_SVG_BASE_HIST);
	system(vl_Cmd);
	sprintf (vl_Cmd, "touch %s/%s", XDC_REP_TMP, XDF_BILAN_SVG_BASE_HIST);
	system(vl_Cmd);
	if (XZSS_07TransfertFichier (   vl_machineLocale,
					XDF_BILAN_SVG_BASE_HIST,
					XDC_REP_TMP,
					XDC_MACHINE_GECONF,
					XDF_BILAN_SVG_BASE_HIST,
					XDC_PATHCNFFICDYN) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "list_bsbh_vider_cb: Transfert du fichier %s incorrect de %s a %s",
		    XDF_BILAN_SVG_BASE_HIST,vl_machineLocale, XDC_MACHINE_GECONF);
		 vl_ValRet = XZSPC_FIC_INV;
	}
	FermetureMere( w );

}	/* Fin list_bsbh_vider_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier Bilan Svg Base Hist
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_BSBH(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_BSBH " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	FILE*                 pl_idJournal = NULL;
	char                  vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
	int			cr	= XDC_NOK;
	char                  vl_args [XDC_LINE_CMD_SIZE] = "";


	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BSBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_BILAN_SVG_BASE_HIST,
					XDC_PATHCNFFICDYN,
					vl_machineLocale,
					XDF_BILAN_SVG_BASE_HIST,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_BSBH: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BILAN_SVG_BASE_HIST, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_BILAN_SVG_BASE_HIST);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_BSBH: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_BILAN_SVG_BASE_HIST);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_BSBH: Positionnement impossible \
		 au debut de %s", XDF_BILAN_SVG_BASE_HIST);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
		while ( (fgets(vl_args,sizeof(tl_EnregBSBH),pl_idJournal) != NULL))
		{
			vl_args[strlen(vl_args)-1] = (char)0;
			pa_FonctionUtil(*pa_NumEnr, vl_args);
			(*pa_NumEnr) ++;
		}			
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
				"\n\n Fin Lecture BSBH \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_BSBH: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
	unlink (vl_pathJournal);

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_BSBH OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_BSBH */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de stocker les enregistrements du HEBDO
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void GMStockeListEnregHEBDO( 	
	int             va_nb,
	char 		* pa_Libelle )

/*
* ARGUMENTS EN ENTREE :
*   va_nb : Nb d'enregistrements
*   pa_Libelle : Valeur de l'enregistrement
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   recupere la valeur de l'enregistrement et ensuite
*	demande son affichage
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : GMStockeListEnregHEBDO " ;

   	ps_TabEnregHEBDO = realloc( ps_TabEnregHEBDO, 
				 (va_nb+1) * sizeof(tl_EnregHEBDO) );

   if ( ps_TabEnregHEBDO == NULL )
	XZST_03EcritureTrace (XZSTC_WARNING, 
		"GMStockeListEnregHEBDO: pb au realloc " );
   else
   {
   	/* ecriture avec le format precedent des arguments 
	*					dans le tableau */
	if (strlen(pa_Libelle) != 0)
	{
   		/*strncpy( ps_TabEnregHEBDO[va_nb], pa_Libelle, 
						strlen(pa_Libelle)-1 );*/
		strcpy(ps_TabEnregHEBDO[va_nb], pa_Libelle);
	}
	else
	{
   		strcpy( ps_TabEnregHEBDO[va_nb], "" );
	}
   }
	
}	/* Fin GMStockeListEnregHEBDO */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de faire la Svg Manuelle
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sauvegarde_manuel_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : sauvegarde_manuel_cb " ;
  	int vl_ValRet 	= XDC_OK;
  	char vl_Cmd[250]="";
	XZSCT_NomMachine      	vl_machineLocale = "";
        FILE               *pl_cr;

	XZST_03EcritureTrace (XZSTC_WARNING ,"sauvegarde_manuel_cb: IN");
	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "sauvegarde_manuel_cb: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}
	sprintf( vl_Cmd , "remsh PODFA \". .profile;cd /produits/migrazur/appliSD/exec;/usr/bin/X11/xterm -display %s:0.0 -sb -sl 500 -bg wheat -fg black -fn 9*15 -title Sauvegarde_Manuelle -e /produits/migrazur/appliSD/exec/tcsvm.ksh\"&", vl_machineLocale);
  	if ( (pl_cr = popen( vl_Cmd, "w")) == NULL)
        	XZST_03EcritureTrace( XZSTC_WARNING, "sauvegarde_manuel_cb:: echec de popen pour lancer le xterm" );
  	else
		pclose (pl_cr);
	
	XZST_03EcritureTrace (XZSTC_WARNING ,"sauvegarde_manuel_cb: OUT %s",vl_Cmd);

}	/* Fin sauvegarde_manuel_cb */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Lancement Svg Hebdo
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sauvegarde_hebdo_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : sauvegarde_hebdo_cb " ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;

	if ( xzca_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCSVA. \n");
	}
	if ( xzca_Sauvegarder_Hebdo ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB lors de la demande de sauvegarde hebdo TCSVA. \n");
	}
	
}	/* Fin sauvegarde_hebdo_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de consulter le Bilan de Svg Hebdo
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void lecture_hebdo_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : lecture_hebdo_cb " ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;

	if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
	}
	
  ps_TabEnregHEBDO = malloc( sizeof(tl_EnregHEBDO) );

  if ( ps_TabEnregHEBDO != NULL )
  {
  	vl_ValRet = consult_HEBDO(
		(void (*)())GMStockeListEnregHEBDO, &vl_numEnreg );

  	/* recherche du menu pere */
  	vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  	/* creation de LIST_HEBDO */
  	create_LIST_HEBDO  ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  	/* affichage de cette liste dans LIHEBDO */
  	GMAfficheListeDansWidget(LIST_HEBDO_LI, 
			  	(void *)ps_TabEnregHEBDO, 
			  	vl_numEnreg, 
			  	CL_HEBDO );

  	/* liberation memoire du tableau  */
  	free( ps_TabEnregHEBDO );

  	/* affichage de LIST_HEBDO */
  	XtManageChild( LIST_HEBDO_FM );
   }
}	/* Fin lecture_hebdo_cb */






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Vide le Bilan de Svg Base Hist 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void list_hebdo_vider_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Callback Windows
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Aucun
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ :  list_hebdo_vider_cb" ;
	int vl_numEnreg	= 0;
  	int vl_ValRet 	= XDC_OK;
  	char vl_Cmd[250]="";
	XZSCT_NomMachine      	vl_machineLocale = "";
  	
	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_BRBH: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}
	sprintf (vl_Cmd, "rm %s/%s", XDC_REP_TMP, XDF_BILAN_SAUVEG_HEBDO);
	system(vl_Cmd);
	sprintf (vl_Cmd, "touch %s/%s", XDC_REP_TMP, XDF_BILAN_SAUVEG_HEBDO);
	system(vl_Cmd);
	if (XZSS_07TransfertFichier (   vl_machineLocale,
					XDF_BILAN_SAUVEG_HEBDO,
					XDC_REP_TMP,
					XDC_MACHINE_GECONF,
					XDF_BILAN_SAUVEG_HEBDO,
					XDC_PATHCNFFICDYN) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "list_hebdo_vider_cb: Transfert du fichier %s incorrect de %s a %s",
		    XDF_BILAN_SAUVEG_HEBDO,vl_machineLocale, XDC_MACHINE_GECONF);
		 vl_ValRet = XZSPC_FIC_INV;
	}
	FermetureMere( w );

}	/* Fin list_bsbh_hebdo_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de lire le fichier Bilan Svg Hebdo
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int consult_HEBDO(
	void    (*pa_FonctionUtil)(),
	int*    pa_NumEnr)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : consult_HEBDO " ;
	int 	vl_ValRet 	= XDC_OK;
	XZSCT_NomMachine      	vl_machineLocale = "";
	FILE*                 pl_idJournal = NULL;
	char                  vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
	int			cr	= XDC_NOK;
	char                  vl_args [XDC_LINE_CMD_SIZE] = "";


	if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		      "consult_HEBDO: nom de machine invalide");
		vl_ValRet = XDC_NOK;
	}

	if (XZSS_07TransfertFichier (    XDC_MACHINE_GECONF,
					XDF_BILAN_SAUVEG_HEBDO,
					XDC_PATHCNFFICDYN,
					vl_machineLocale,
					XDF_BILAN_SAUVEG_HEBDO,
					XDC_REP_TMP) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
		  "consult_BSBH: Transfert du fichier %s incorrect de %s a %s",
		   XDF_BILAN_SAUVEG_HEBDO, XDC_MACHINE_GECONF, vl_machineLocale);
		 vl_ValRet = XZSPC_FIC_INV;
	}


	/* Ouverture en lecture de ce fichier */
	sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_BILAN_SAUVEG_HEBDO);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "consult_BSBH: nom du fichier %s \
		       invalide ou fichier deja ouvert", XDF_BILAN_SVG_BASE_HIST);

	    	vl_ValRet = XZSPC_FIC_INV;
	}
	/* Positionnement au premier enregistrement */
	else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	{
	 	XZST_03EcritureTrace (XZSTC_WARNING,
		 "consult_BSBH: Positionnement impossible \
		 au debut de %s", XDF_BILAN_SAUVEG_HEBDO);

	      	vl_ValRet = XDC_NOK;
	}

	/* lecture de la ligne courante jusqu'a la fin du fichier */
	else
	{
		while ( (fgets(vl_args,sizeof(tl_EnregHEBDO),pl_idJournal) != NULL))
		{
			/*vl_args[strlen(vl_args)-1] = (char)"\0";*/
			XZST_03EcritureTrace (XZSTC_DEBUG3,"LIGNE BRUTE:%s|",vl_args);
			vl_args[strlen(vl_args)-1] = (char)0;
			XZST_03EcritureTrace (XZSTC_DEBUG3,"LIGNE :%s|",vl_args);
			pa_FonctionUtil(*pa_NumEnr, vl_args);
			(*pa_NumEnr) ++;
		}			
	}

	XZST_03EcritureTrace(XZSTC_DEBUG3,
				"\n\n Fin Lecture HEBDO \n");

	/*A Fermeture du Fichier */
    	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
    	{
    		XZST_03EcritureTrace (XZSTC_WARNING,
		    "consult_HEBDO: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
	unlink (vl_pathJournal);

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"consult_HEBDO OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);

}	/* Fin consult_HEBDO */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de declencher le menu de demande de
*	synchronisation des disques miroirs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void synchro_disques_cb(
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Affiche le widget permettant de recuperer le Site
*	sur lequel on veut synchroniser les disques
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : synchro_disques_cb " ;
  	int           vl_NbSite;

	XZST_03EcritureTrace(XZSTC_FONCTION,"synchro_disques_cb  : IN \n");

  /* stockage de l'operation dans une variable globale */
   vs_ope_adm = cm_SYNC_DISK;

  ps_TabSite = malloc( sizeof(XZSCT_NomSite) );

  if ( ps_TabSite != NULL )
  {

  /* recuperation de la liste des sites par XZSA_18ListeSite */
  vm_nb=0;
  XZSA_18ListeSite( GMStockeListSites, &vl_NbSite, ps_TabSite );
  vl_NbSite=vm_nb;

  /* recherche du menu pere */
  vs_menu_parent = XtParent( XtParent( XtParent( XtParent( w ))));

  /* creation de BDSIT */
  create_BDSIT ( vs_menu_parent, vg_mrm_hierarchy, &vg_class );

  /* affichage de cette liste de sites dans BDSIT */
  GMAfficheListeDansWidget( BDSIT_LI_SIT, ps_TabSite, vl_NbSite, CL_SITE );

  /* ouverture de BDSIT */
  XtManageChild( BDSIT_FM );

  free( ps_TabSite );
  }

	XZST_03EcritureTrace(XZSTC_FONCTION,"synchro_disques_cb  : OUT \n");


}	/* Fin synchro_disques_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de demander la synchronisation des disques miroirs 
*	du site 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void gmot_synchro_disk(XZSCT_NomSite 	va_NomSite)

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite : Noml du site
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoie un message de synchronisation "XDM_TSE_SYNCDISK" 
*	sur le datagroup NSUPDISK_NomSite pour prendre en compte
*	la synchronisation
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : gmot_synchro_disk " ;
	XZSCT_Datagroup vl_NomDatagroup = "";

	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_synchro_disk  : IN \
		-- Site  %s \n",va_NomSite);

	/*A Construction du Datagroup XDG_NSUPDISK_NomSite */
	XDG_EncodeDG2(vl_NomDatagroup,XDG_NSUPDISK,va_NomSite);


	/*A Envoie du Msg XDM_TSE_SYNCDISK */
	if (!TipcSrvMsgWrite(vl_NomDatagroup,
			XDM_IdentMsg(XDM_TSE_SYNCDISK),
			XDM_FLG_SRVMSGWRITE,
			NULL) )
	{
		/* erreur creation message de demande */
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Pb envoie msg XDM_TSE_SYNCDISK \n");
	}


	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_synchro_disk  : OUT \n");


}	/* Fin gmot_synchro_disk */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de demander une impression du fichier de BL
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void list_bl_imp_cb(
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom du ficheir et appel de la fonction
*	d'impression
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : list_bl_imp_cb ";
	XZSCT_NomMachine      	vl_machineLocale = "";

	XZST_03EcritureTrace(XZSTC_FONCTION,"list_bl_imp_cb  : IN \n");

	/*A Creation du Fichier d'impression*/
	gmot_creer_ficimp(CL_BL);

	/*A impression du fichier */
        if (XZSS_04ImprimerFichier( XZSSC_TYPE_IMPRIMA, 
				XDF_TMP_IMPRESSION, XDC_REP_TMP,XDC_IMP_ASCII ) != XDC_OK )
	{     
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			"pb a l'appel de XZSS_04ImprimerFichier");
        }

	XZST_03EcritureTrace(XZSTC_FONCTION,"list_bl_imp_cb   : OUT \n");


}	/* Fin list_bl_imp_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de demander une impression du fichier de VL
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void livl_imp_cb(
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom du fichier et appel de la fonction
*	d'impression
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : livl_imp_cb ";
	XZSCT_NomMachine      	vl_machineLocale = "";

	XZST_03EcritureTrace(XZSTC_FONCTION,"livl_imp_cb  : IN \n");

	/*A Creation du Fichier d'impression*/
	gmot_creer_ficimp(CL_VERSLOG);


	/*A impression du fichier */
        if (XZSS_04ImprimerFichier( XZSSC_TYPE_IMPRIMA, 
			XDF_TMP_IMPRESSION, XDC_REP_TMP,XDC_IMP_ASCII ) != XDC_OK )
	{     
         	XZST_03EcritureTrace( XZSTC_WARNING, 
			"pb a l'appel de XZSS_04ImprimerFichier");
        }

	XZST_03EcritureTrace(XZSTC_FONCTION,"livl_imp_cb   : OUT \n");

}	/* Fin livl_imp_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Creation d'un fichier ASCII pour l'impression
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void gmot_creer_ficimp(int va_IndFic)

/*
* ARGUMENTS EN ENTREE :
*   va_IndFic	: Indicateur du type de fichier 
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Ouvre un fichier temporaire, recupere les chaines de caracteres
*	a enregistrer et enfin ferme le fichier.
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ : gmot_creer_ficimp " ;
	int	vl_ValRet	= XDC_OK;
	FILE	* pl_idFic	= NULL;
   	char 	vl_NomFichier[XZSSC_PATH_ABS_SIZE];



	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_creer_ficimp   : IN \
		Ind Fic %d \n", va_IndFic);

	/*A Creation du Fichier */
	sprintf(vl_NomFichier,"%s/%s",XDC_REP_TMP,XDF_TMP_IMPRESSION);
	if ( (vl_ValRet = XZSS_16OuvrirFichierStream (
	  XZSSC_FIC_WR_STREAM,vl_NomFichier,&pl_idFic ) ) != XDC_OK )
	{

	       XZST_03EcritureTrace (XZSTC_WARNING,
		       "nom du fichier %s \
		       invalide ou fichier deja ouvert", vl_NomFichier);

	    	vl_ValRet = XZSPC_FIC_INV;
	}

	/*A Recuperation et enregiostreme ts des textes */
	gmot_recup_textimp(va_IndFic,pl_idFic);

	/*A Fermeture du Fichier */
	XZSS_17FermerFichierStream(pl_idFic);


	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_creer_ficimp   : OUT \n");


}	/* Fin gmot_creer_ficimp */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Construit et enregistre les chianes de caracteres 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void gmot_recup_textimp(int va_IndFic, FILE *pa_idFic)

/*
* ARGUMENTS EN ENTREE :
*  Aucun
*
*
* ARGUMENTS EN SORTIE :
*  pa_Texte : Chaine de caracteres 
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Suivant  le type de fichier a imprimer, recupere les 
*	textes et les enregistre dans le fichier
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm.c,v 1.15 1996/09/19 17:28:43 milleville Exp $ $Revision: 1.15 $ $Date: 1996/09/19 17:28:43 $ : gmot_recup_textimp " ;
	tl_Format	*vl_Texte	= NULL;
	int		i	= 0;
	char 		vl_TextSepar[128]	= "";
	char 		vl_TextInitVL[128]	= "";
	char 		vl_TextInitBL[128]	= "";


	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_recup_textimp   : IN \
		IndFic %d \n",va_IndFic);

	/*A Enregistremnt d'une Entete */
	memset(vl_TextSepar,(char) 0, 79);
	memset(vl_TextSepar,'#',78);
	sprintf(vl_TextInitBL,"%-20s%-10s%-10s%s",
		"Sous-systeme","Machine","Version"," Date");
	sprintf(vl_TextInitVL,"%-18s%-15s%-10s%s",
		"Sous-systeme","Machine","Version","Date");
	

	/*A Enregistre les textes suivant le type de fichier */
     	switch ( va_IndFic )
     	{
       		case CL_VERSLOG:
	
			fprintf(pa_idFic,"%s\n",vl_TextSepar);
			fprintf(pa_idFic,"%60s\n",
					"Fichier des Versions Logicielles");
			fprintf(pa_idFic,"%s\n\n",vl_TextSepar);
			fprintf(pa_idFic,"%s\n\n",vl_TextInitVL);

  			/*A Recuperation des textes et enregistrement 
			*	dans le fichier */
  			for ( i=0; i<vs_NbTacheReel; i++ )
  			{
				/*B Enregistrement du Texte */ 
				fprintf (pa_idFic,"%s\n",
						pg_ListVerLogFormat[i]);
  			}
			break;

       		case CL_BL:

			fprintf(pa_idFic,"%s\n",vl_TextSepar);
			fprintf(pa_idFic,"%60s\n",
				"Fichier des Bons de Livraison ");
			fprintf(pa_idFic,"%s\n\n",vl_TextSepar);
			fprintf(pa_idFic,"%s\n\n",vl_TextInitBL);

  			/*A Recuperation des textes et enregistrement 
			*	dans le fichier */
  			for ( i=0; i<vg_numEnreg_BL; i++ )
  			{
				/*B Enregistrement du Texte */ 
				fprintf (pa_idFic,"%s\n",ps_TabEnregBL[i]);
  			}

		default :
			break;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"gmot_recup_textimp   : OUT \n");

}	/* Fin gmot_recup_textimp */



/*
******************************************************
*                    FIN DE MODULE
******************************************************
*/
