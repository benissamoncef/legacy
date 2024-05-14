/*E*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv * FICHIER gmot_adm_dv.c
******************************************************
* DESCRIPTION DU MODULE :
*
* 1ere partie:
* ----------
* fonctions d'initialisation, construction, affichage
* et gestion des synoptiques d'administration
*
* 2e partie:
* ---------
* fonctions de traitement executees sur
* reception de messages RT-Works destines a rafraichir
* les synoptiques d'administration systeme
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	14 Nov 94	: Creation
* T.Milleville	08 Mar 95	: Modification des constantes XZGAC_xx 
*	en XZSIC_xx    V1.2
* T.Milleville	15 Mar 95	: Correction des erreurs Warning
*	de compilation V 1.3
* T.Milleville	11 Sep 95	: Ajout test de verification si 
*	les vues existent avant d'eesayer de les detruire dans 
*	FreeVuesAdm V1.4
* T.Milleville	19 Oct 95	: Ajout du traitement des types de peripheriques
*	concernant les imprimantes NB et Couleur V1.5
* T.Milleville	19 Oct 95	: Recupeartion du type de l'entite
*	dans le msg XDM_ADM_ARC_PER
* P. Niepceron	16 Jan 1996	: Ajout de l'init. de vs_ResSynt à 1 avant la revue de l'etat des machines v1.5
* P. Niepceron	12 Fev 1996	: Correction de l'init. de vs_ResSynt à 1 avant la revue de l'etat des machines v1.6
* P. Niepceron	28 Aou 1996	: Ajout de traitement pc simpl pour chargement des vue arch gen et synt v1.8
* P. Niepceron  29 Aou 1996     : Ajout de SSS_PCS.v v1.9
* P. Niepceron  18 Dec 1997	: Modif pour PC2 + ajout type mac poste analyse v1.10
* Guilhou	16 mar 1998	: re ajout type mac poste analyse 1.11
* P. Niepceron  01 Jul 1998	: Ajout du type de machine Historique v1.12
* P. Niepceron  06 dec 2006	: Création d'une nouvelle table de symbole pour les variables de traces " TRA_* " v1.13 DEM594
* P. Niepceron  23 Aou 2007	: Suppression de l'ajout des variables de trace pour les taches telmi.xs v1.14 DEM691
* P. Niepceron  23 Aou 2007	: Suppression de l'ajout des variables de trace pour les taches *.xs et tel* v1.15 DEM691
* GR            10/07/2020      : EMS v1.16  
*******************************************************/
 
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/

/* fichiers inclus */

#include "gmot_ser.h"
#include "gmot_var.h"
#include "dvtools.h"


#include "gmot_adm_dv.h"

/* definitions de constantes */

/* constantes DataViews */
#define cm_DEFAULT_SCR_VP  (RECTANGLE *) NULL
#define cm_DEFAULT_DRW_VP  (RECTANGLE *) NULL
#define cm_DEFAULT_CLUT    (char *) NULL

/* definitions de types locaux */

typedef struct {
  DRAWPORT drawport;
  VIEW     view;
} tm_HELP_INFO;

typedef struct {
  BOOL         MagicFlag;  /* Pour ignorer l'embarassant tout 1er Expose event. */
  OBJECT       screen;
  DRAWPORT     drawport;
  VIEW         view;
  Widget       widget;
  tg_TYP_SYN   type;
  tm_HELP_INFO help;
} tm_SCR_INFO;

typedef struct {
  char* buffer;
  int   size;
} tm_BUFFERED_VIEW;

typedef struct {
  int              *buffer;
  int              currIndex;
  int              maxIndex;
  XZSCT_NomSite    site;
  XZSCT_NomMachine machine;
} tm_SYMTAB_BUILD_ARGS;

typedef struct {
  tg_TYP_SYN       syn;
  char*            (*color )();
  char*            (*rebind)();
  XZSCT_NomMachine machine;
  XZSCT_NomTache   task;
  XZSCT_NomSite    site;
} tm_OTHER_TRAVERSE_ARGS;

typedef char* ( *tm_COLOR_FUNCTION  )( OBJECT, char*,   char* );
typedef char* ( *tm_REBIND_FUNCTION )( OBJECT, VARDESC, tm_OTHER_TRAVERSE_ARGS* );

typedef struct {
  tm_COLOR_FUNCTION  color;
  tm_REBIND_FUNCTION rebind;
} tm_CONFIG_FUNCTIONS;

typedef struct {
  int           lineNum;
  int           pointNum;
  OBJECT        drawing;
  XZSCT_NomSite site;
} tm_SSS_TRAVERSE_ARGS;


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : gmot_adm_dv.c";

static vs_init_AG = False;

static VIEW  vue_arch = (VIEW)NULL;
static VIEW  vue_synt = (VIEW)NULL;

static OBJECT loc = NULL;

static BOOL vm_ColorsLoaded = NO;
static OBJECT vm_fond,
              vm_contexte,
              vm_gris,
              vm_vert,
              vm_rouge,
              vm_jaune,
              vm_bleu,
              vm_turquoise;

static char *vm_ViewFileTab[cm_MAX_SYN_FILE] = { cm_ARC_syn_v,
						 cm_SYNT_GEN_v,
						 cm_SSS_syn_v,
						 cm_TAC_ST_syn_v,
						 cm_TAC_SC_syn_v,
						 cm_TAC_SD_syn_v,
						 cm_TAC_LT_syn_v,
						 cm_TAC_SP_syn_v,
						 cm_TAC_PCS_syn_v,
						 cm_TAC_PC2_syn_v,
						 cm_TAC_PC2LT_syn_v,
						 cm_TAC_STA_syn_v,
						 cm_TAC_HIS_syn_v,
						 cm_TRA_ST_syn_v,
						 cm_TRA_SC_syn_v,
						 cm_TRA_SD_syn_v,
						 cm_TRA_LT_syn_v,
						 cm_TRA_SP_syn_v,
						 cm_TRA_PCS_syn_v,
						 cm_TRA_PC2_syn_v,
						 cm_TRA_PC2LT_syn_v,
						 cm_TRA_STA_syn_v,
						 cm_TRA_HIS_syn_v};

static char *vm_HelpFileTab[cg_NB_TYP_SYN] = {  cm_ARC_help_v,
						cm_SSS_help_v,
						cm_TAC_help_v,
						cm_TRA_help_v,
						cm_SYNT_help_v};


static char *vm_SSSsubView[XZSAC_NB_TYPEM] = {  cm_SSS_Station_v,
						cm_SSS_ServerC_v,
						cm_SSS_ServerD_v,
						cm_SSS_LT_v,
						cm_SSS_ServerP_v,
						cm_SSS_PCS_v,
						cm_SSS_PC2_v,
						cm_SSS_PC2LT_v,
						cm_SSS_StationA_v,
						cm_SSS_HIS_v};
static VIEW vm_SSSView[XZSAC_NB_TYPEM];


static char *vm_DebugTab[cm_MAX_DEBUG] = { cm_Debug_FO,
					   cm_Debug_ME,
					   cm_Debug_IN,
					   cm_Debug_BD,
					   cm_Debug_D1,
					   cm_Debug_D2,
					   cm_Debug_D3,
					   cm_Debug_INTER,
					   cm_Debug_REGLE,
					   cm_Debug_Ecr };


static tm_BUFFERED_VIEW vm_ViewSet[cm_MAX_SYN_FILE];
static tm_BUFFERED_VIEW vm_HelpSet[cg_NB_TYP_SYN];

static tm_SCR_INFO      vm_ScreenSet[cm_MAX_SCREENS];

static int             *vm_DataBuffer;
static SYMTABLE         vm_DataSymTable;
static SYMTABLE         vm_DataSymTable_T;

static RECTANGLE        vm_WholeWorld = { -16384, -16384, 16383, 16383 };


/* Variables memorisant les etats de Syntheses Machines
*	et Reseau */
int	vs_MachSynt		= 0xffff;
int	vs_ResSynt		= 0xffff;

int vs_NumVar	= 0;


/* declaration de fonctions internes */

static void  GMloadColors( void );

static int  GMcntMachine( XZSCT_NomMachine, int, int* );
static void  GMcntTache( XZSCT_NomTache, int, int );
static void  GMcntEntity( XZSCT_NomEntite, int, int, int* );
static void  GMcntSite( XZSCT_NomSite, int, int* );

static void  GMbuildSSS( XZSCT_NomSSysteme, int, tm_SYMTAB_BUILD_ARGS* );
static void  GMbuildTACandTRA( XZSCT_NomTache, int, tm_SYMTAB_BUILD_ARGS* );
static int  GMaddMachine( XZSCT_NomMachine, int, tm_SYMTAB_BUILD_ARGS* );
static void  GMaddEntity( XZSCT_NomEntite, int, int, tm_SYMTAB_BUILD_ARGS* );
static void  GMaddSite( XZSCT_NomSite, int, tm_SYMTAB_BUILD_ARGS* );

static void  GMeraseScreen( int );
static void  GMupdateTTable( OBJECT, int, ... );

static char *GMhelpSSScolor( OBJECT, char*, char* );
static void  GMinitHelpSyn( tm_SCR_INFO*, int );
static void  GMhelpToSyn( tm_SCR_INFO* );
static int   GMinitSyn( tm_SCR_INFO* , int, tg_TYP_SYN, char* );
static char *GMtraverse( OBJECT, char*, tm_OTHER_TRAVERSE_ARGS* );

static char *GMcolorSYNT( OBJECT, char*, char* );
static char *GMcolorARC( OBJECT, char*, char* );
static char *GMcolorTAC( OBJECT, char*, char* );
static char *GMcolorTRA( OBJECT, char*, char* );
static char *GMcolorHelp( OBJECT, char*, char* );

static char *GMrebindSYNT( OBJECT, VARDESC, tm_OTHER_TRAVERSE_ARGS* );
static char *GMrebindARC( OBJECT, VARDESC, tm_OTHER_TRAVERSE_ARGS* );
static char *GMrebindTAC( OBJECT, VARDESC, tm_OTHER_TRAVERSE_ARGS* );
static char *GMrebindTRA( OBJECT, VARDESC, tm_OTHER_TRAVERSE_ARGS* );

static int   GMinitSynSSS( tm_SCR_INFO*, XZSCT_NomSite );
static int  GMcomposeSSS( XZSCT_NomMachine, int, tm_SSS_TRAVERSE_ARGS* );
static char *GMcolorSSS( OBJECT, char*, char* );
static char *GMrebindSSS( OBJECT, VARDESC, char* );

static int   GMhandlePicking( OBJECT, EVENT_REQUEST, int, OBJECT, tm_SCR_INFO* );
static BOOL  GMhandleEvent( Widget, tm_SCR_INFO*, XEvent* );
static void  GMinputCallback( Widget, tm_SCR_INFO*, caddr_t );
static void  GMresizeCallback( Widget, tm_SCR_INFO*, caddr_t );
static void  GMexposeCallback( Widget, tm_SCR_INFO*, caddr_t );


static void  TESTprintNode( char*, int*, char* );
static void  GM_LibereClesSymTable( char*, int*, char* );

void MajAdm_Mach_Synt(
	XZSCT_NomMachine	va_NomMachine,
	int 			va_EtatMach);

void MajAdm_Ent_Synt(
	XZSCT_NomEntite		va_NomEntite,
	int 			va_EtatEnt,
	int 			va_TypeEnt);

void MajAdm_Res_Synt(
	XZSCT_NomMachine	va_NomMachine,
	int 			va_EtatMach);

void MajSynt_Mach_Synop(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	void             **pa_DonneesMsg);

void MajSynt_Ent_Synop(
	XZSCT_NomEntite pa_NomEntite,
	int              va_NumOrdre,
	void             **pa_DonneesMsg);

void MajSynt_Res_Synop(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	void             **pa_DonneesMsg);

void InitAllSynop();




/* declaration du tableau statique des fonctions de configuration */
/* ( couleurs et rebind de pointeurs de descripteurs de variables ). */

static tm_CONFIG_FUNCTIONS vm_Config[cg_NB_TYP_SYN] = {
  { GMcolorARC, GMrebindARC },
  { NULL,       NULL        },
  { GMcolorTAC, GMrebindTAC },
  { GMcolorTRA, GMrebindTRA },
  { GMcolorSYNT, GMrebindSYNT }
};


/* les 2 fonctions suivantes prennent les sous-dessins de la vue v       */
/* et les incluent dans la vue (de "references", ils deviennent "inclus" */

BOOL TrRefInc( OBJECT o, CHAR *arg)
{
   if(VOobType(o) == OT_SUBDRAWING)
     VOsdViKeep(o,YES);
   return V_CONTINUE_TRAVERSAL;
}

void Ref_Inc( VIEW v)
{
OBJECT dr,dq;
   dr = TviGetDrawing(v);
   dq = VOdrGetObjectDeque(dr);
   VOdqTraverse(dq,TrRefInc,NULL);
}


/* definition de fonctions externes */
extern int vg_TypeMAchine;

/*X*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
* 1- Chargement des couleurs.
* 2- Charge les vues DataViews generiques en memoire
* 3- Construit le buffer de donnees ENTIER pour
*    alimenter tous les synoptiques de supervision.
*    (Ce buffer prend la forme d'une table de symboles
*    DataViews).
* renvoie XDC_OK si tout s'est bien passe.
******************************************************
* SEQUENCE D'APPEL :
*/

int InitAdm( int va_Flag_Reinit)

/*
* CONDITION D'ERREUR :
* XDC_NOK: - Pb lors du chargement initial des vues.
*          - Pb lors de l'allocation du buffer de
             donnees pour les variables dynamiques.
*
* CONDITIONS LIMITES :
* rien
*
******************************************************/
{
  int vl_n, vl_n_vue, dummy;
  int vl_NumVar = 0;
  VIEW vl_v;
  tm_SYMTAB_BUILD_ARGS vl_Args;
  int             vl_NbElt        = 0;
  XZSCT_NomSite   vl_NomSite      = "";
  char   	  vl_NomVue[81]      = "";
  char   	  vl_DebNomVue[81]      = "";


	XZST_03EcritureTrace(XZSTC_FONCTION,"InitAdm : IN \n");


if (va_Flag_Reinit == XDC_VRAI)
{
  /* 1- Chargement des couleurs. */
  GMloadColors();


  /* 2- Charge les vues DataViews generiques en memoire (synoptiques et aides). */
  vl_n_vue = XZSAC_NB_TYPEM;
  while(vl_n_vue--)
    {
      vm_SSSView[vl_n_vue] = TviLoad( vm_SSSsubView[vl_n_vue] );
    }

  vl_n = cm_MAX_SYN_FILE;
  while(vl_n--)
    {
      if (( vl_n < 2 ) && ( vg_TypeMachine == XZSCC_TYPEM_PCS ))
        {
        strncpy( vl_DebNomVue, vm_ViewFileTab[vl_n], strlen(vm_ViewFileTab[vl_n])-2 );
        vl_DebNomVue[strlen(vm_ViewFileTab[vl_n])-2]='\0';
        sprintf( vl_NomVue,"%s_%s.v",vl_DebNomVue,vg_NomSite);
        
      	vl_v = TviLoad( vl_NomVue );
      	}
      else
      	vl_v = TviLoad( vm_ViewFileTab[vl_n] );

      Ref_Inc(vl_v); 

      /* si c'est la vue d'architecture generale, on la stocke dans une var globale */
      if ( vl_n == 0 )
      {
        vue_arch = vl_v;
      }
      else
      {
		if (vl_n == 1 )
		{
			 vue_synt = vl_v;
		}
      if ( !TviMemSave( vl_v, &vm_ViewSet[vl_n].buffer, &vm_ViewSet[vl_n].size ) )
	{
	  /* Erreur */
	  XZST_03EcritureTrace( XZSTC_WARNING,
	  	"gmot_adm_dv, InitAdm: Impossible de charger les synoptiques Generales en memoire" );
	  return XDC_NOK;
	}
      (void) TviDestroy( vl_v );
      }
    }
  vl_n = cg_NB_TYP_SYN;
  while(vl_n--)
    {
      vl_v = TviLoad( vm_HelpFileTab[vl_n] );
      Ref_Inc(vl_v); 
      if ( !TviMemSave( vl_v, &vm_HelpSet[vl_n].buffer, &vm_HelpSet[vl_n].size ) )
	{
	  /* Erreur */
	  XZST_03EcritureTrace( XZSTC_WARNING,
		"gmot_adm_dv, InitAdm: Impossible de charger les synoptiques d'Aide en memoire" );
	  return XDC_NOK;
	}
      (void) TviDestroy( vl_v );
    }



}



  /* 3- Construction du buffer de donnees et de la table de */
  /*    symboles dont les noeuds pointent sur les entrees du buffer. */
XZST_03EcritureTrace(XZSTC_WARNING,"VTstsizecreate....");
  vm_DataSymTable = (SYMTABLE) VTstsizecreate( cm_DATA_TABLE, NULL,1000000);
  vm_DataSymTable_T = (SYMTABLE) VTstsizecreate( "Table de symboles T", NULL,1000000);
XZST_03EcritureTrace(XZSTC_WARNING,"....VTstsizecreate....");

  /* Calcule le nombre total de variables dynamiques d'etat. */
  XZSA_01ListeMachine( GMcntMachine, &dummy, NULL );
	XZST_03EcritureTrace(XZSTC_WARNING,"\n\n Nb de machines -- \
		d'entites et de sites %d \n",vs_NumVar );
  XZSA_18ListeSite( GMcntSite, &dummy, NULL );
	XZST_03EcritureTrace(XZSTC_WARNING,"\n\n Nb de machines -- \
		d'entites et de sites %d \n",vs_NumVar );
  XZSA_24ListeEntite( GMcntEntity, &dummy, NULL );

	XZST_03EcritureTrace(XZSTC_WARNING,"\n\n Nb de machines -- \
		d'entites et de sites %d \n",vs_NumVar );


  /* Allocation du buffer de donnees. Sa taille est */
  /* egale a vl_NumVar plus une marge de securite. */
  vm_DataBuffer = (int *) calloc( vs_NumVar + cm_BUF_SECU_MRG, sizeof(int) );
  if ( vm_DataBuffer == NULL )
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	  	"gmot_adm_dv, InitAdm: Impossible d'allouer la memoire pour le buffer de donnees" );
      return XDC_NOK;
    }

  /* Initialisation de la structure passee en argument */
  /* aux fonctions de traversee. */
  vl_Args.maxIndex = vs_NumVar + cm_BUF_SECU_MRG;
  vl_Args.buffer = vm_DataBuffer;
  vl_Args.currIndex = 0;
  vl_Args.machine[0] = vl_Args.site[0] = '\0';

  /* Traversee des listes de machines et d'entites pour construire */
  /* la table des symboles. */
  XZSA_01ListeMachine( &GMaddMachine, &dummy, &vl_Args );
  XZSA_18ListeSite( &GMaddSite, &dummy, &vl_Args );
  XZSA_24ListeEntite( &GMaddEntity, &dummy, &vl_Args );



  /*
  fprintf( stderr, "************************\n" );
  fprintf( stderr, "*** Affichage des clefs:\n" );
  VTsttraverse( vm_DataSymTable, TESTprintNode, (char *)NULL );
  */

if (va_Flag_Reinit == XDC_VRAI)
{
	/*A Initialisation Vue Synthese */
  GMinitSYNT( vue_synt );

	/*A Initialisation Vue General */
  GMinitARC( vue_arch );


	/*A Demande d'initialisation des Synoptiques */
	InitAllSynop();
}
else
{        /*A Initialisation Vue Synthese */
           GMinitSYNT( vue_synt );

	  /*A Initialisation Vue General */
	    GMinitARC( vue_arch );

	  /*A Demande d'initialisation des Synoptiques */
	  InitAllSynop();
          
	  /*A Demande d'initialisation de la synthese du reseau  */
	  MajAdm_Mach_Synt(XDC_ACTIF_CI,XZSIC_ETAT_VEILLE);
	  MajAdm_Mach_Synt(XDC_ACTIF_DN,XZSIC_ETAT_VEILLE);
	  MajAdm_Mach_Synt(XDC_ACTIF_DM,XZSIC_ETAT_VEILLE);
	  MajAdm_Res_Synt(XDC_ACTIF_CI,XZSIC_ETAT_VEILLE);
	  MajAdm_Res_Synt(XDC_ACTIF_DN,XZSIC_ETAT_VEILLE);
	  MajAdm_Res_Synt(XDC_ACTIF_DM,XZSIC_ETAT_VEILLE);

}


	XZST_03EcritureTrace(XZSTC_FONCTION,"InitAdm : OUT %d",vl_Args.currIndex );

  return XDC_OK;

}	/* Fin InitAdm */



/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
* 1- Recherche les infos DataViews relatives au widget
*    argument dans la structure locale vm_ScreenSet.
* 2- Controle des arguments.
* 3- Efface la fenetre concernee dans le cas ou un
*    synoptique est deja present dans la drawing area.
* 4- Ouverture de l'ecran DataViews (screen) si celui-
*    ci ne l'a pas encore ete.
* 5- Initialisation de la vue DataViews (au besoin de
*    ses sous-vues), et des structures DataViews co-
*    rrespondantes.
* 6- Creation du drawport et affichage de la vue.
******************************************************
* SEQUENCE D'APPEL :
*/

int AfficheVueAdm(
	     Widget           va_widget,
	     tg_TYP_SYN       va_type,
	     char*            pa_name
          )

/*
* PARAMETRES EN ENTREE :
* va_widget: widget de type DrawingArea sur lequel doit
*            s'afficher le synoptique.
* va_type:   type du synoptique a afficher.
* pa_name:   information complementaire relative au type
*            de synoptique:
*            - NULL pour ARC.
*            - nom du site pour SSS.
*            - nom de la machine pour TAC et TRA.
*
* PARAMETRES EN SORTIE :
* aucun changement.
*
* VALEUR(S) DE RETOUR :
* XDC_OK      si tout s'est bien passe.
* XDC_ARG_INV si les parametres passes ne sont pas
*             coherents.
* XDC_NOK     si des Pbs apparaissent lors de l'ouverture
*             de l'ecran DataViews ou de l'initialisation
*             de la vue du synoptique.
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
* Aucune: la coherence des parametres en entree est
*         systematiquement verifiee.
*
******************************************************/
{
  int  vl = cm_MAX_SCREENS;
  int  vl_index = -1;
  int  vl_numView;
  int  vl_machine;
  int  vl_return;
  BOOL vl_alreadyOpened = NO;

      XZST_03EcritureTrace( XZSTC_FONCTION,
	 "AfficheVueAdm  : IN -- Type Synop %d \n",va_type);


  /* Recherche les infos DataViews relatives au widget argument. */
  while(vl--)
    {
      XZST_03EcritureTrace( XZSTC_DEBUG1," Info Dataviews \
	-- indice ecran %d -- Widget ecran %d -- Widget param %d \n",
	vl,vm_ScreenSet[vl].widget,va_widget);


   if (vm_ScreenSet[vl].widget == (Widget) NULL)
	{
	  vl_index = vl;
	}
      if (vm_ScreenSet[vl].widget == va_widget)
	{
	  vl_index = vl;
	  vl_alreadyOpened = YES;
	  break;
	}
    }
  if (vl_index == -1)
    {
      XZST_03EcritureTrace( XZSTC_WARNING,
	 "gmot_adm_dv, AfficheVueAdm: Tous les ecrans internes sont occuppes (mauvais widget ou tableau interne trop petit) "); 
      return XDC_ARG_INV;
    }

  /* Controle des arguments. */
  switch(va_type)
    {
    case cg_NO_SYN:
      GMeraseScreen( vl_index );
      return XDC_OK;
    case cg_TYP_ARC:
      break;
    case cg_TYP_SSS:
      if ( XZSA_22ExisterSite( pa_name ) != XDC_OK )
	{
	  /* Erreur */
	  return XDC_ARG_INV;
	}
      break;
    case cg_TYP_TAC:
    case cg_TYP_TRA:
      if ( XZSA_05ExisterMachine( pa_name ) != XDC_OK )
	{
	  /* Erreur */
	  return XDC_ARG_INV;
	}
      break;
    case cg_TYP_SYNT:
	break;
    default:
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	  			"gmot_adm_dv, AfficheVueAdm: Type de synoptique inconnu." ); 
      return XDC_ARG_INV;
    }

  /* Efface, si necessaire, la fenetre concernee. */
  GMeraseScreen( vl_index );

  /* Ouverture de l'ecran DataViews et positionnement */
  /* des callbacks Expose, Resize et Input. */


      XZST_03EcritureTrace( XZSTC_DEBUG1,
	" Valeur vl_alreadyOpened %d -- YES %d -- Index ecran %d \n",
		vl_alreadyOpened,YES, vl_index);

  if ( vl_alreadyOpened == NO )
    {
      char vl_device[80];
      sprintf( vl_device, "%s", DisplayString( XtDisplay( va_widget ) ) );


	XZST_03EcritureTrace(XZSTC_DEBUG1,"Ouverture Ecran !! \n");

      vm_ScreenSet[vl_index].screen = TscOpenSet( NULL, cm_DEFAULT_CLUT,
						  V_X_DRAW_WIDGET,
						  va_widget,
						  V_X_EXPOSURE_BLOCK, YES,
						  V_X_RESIZE_BLOCK,   YES,
						  V_ACTIVE_CURSOR,
						  V_END_OF_LIST );
      if ( vm_ScreenSet[vl_index].screen )
	{
	  XtAddCallback( va_widget, XmNexposeCallback,
                         &GMexposeCallback, &vm_ScreenSet[vl_index]);
          XtAddCallback( va_widget, XmNresizeCallback,
                         &GMresizeCallback, &vm_ScreenSet[vl_index]);
          XtAddCallback( va_widget, XmNinputCallback,
                         &GMinputCallback, &vm_ScreenSet[vl_index]);
	  vm_ScreenSet[vl_index].widget = va_widget;
	  vm_ScreenSet[vl_index].MagicFlag = YES;
	}
      else
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	     "gmot_adm_dv, AfficheVueAdm: Impossible d'ouvrir le screen DataViews" ); 
	  return XDC_NOK;
	}
    }

  /* Initialisation des structures DataViews. */
  vm_ScreenSet[vl_index].type = va_type;
  switch(va_type)
    {
    case cg_TYP_ARC:
      vl_return = GMinitSyn( &vm_ScreenSet[vl_index], 0, cg_TYP_ARC, NULL );
      break;

    case cg_TYP_SSS:
      vl_return = GMinitSynSSS( &vm_ScreenSet[vl_index], pa_name );
      break;

    case cg_TYP_TAC:
      (void) XZSA_17TypeMachine( pa_name, &vl_machine );
      vl_numView = cg_TYP_TAC + vl_machine -1;
      vl_return = GMinitSyn( &vm_ScreenSet[vl_index], vl_numView, 
						cg_TYP_TAC, pa_name );
      break;

    case cg_TYP_TRA:
      (void) XZSA_17TypeMachine( pa_name, &vl_machine );
      vl_numView = cg_TYP_TRA + vl_machine +8;
      vl_return = GMinitSyn( &vm_ScreenSet[vl_index], vl_numView, 
						cg_TYP_TRA, pa_name );
      break;

    case cg_TYP_SYNT:
      vl_return = GMinitSyn( &vm_ScreenSet[vl_index], 1, 
					cg_TYP_SYNT, NULL );
      break;

    default:
      return;
    }
  if ( vl_return != XDC_OK )
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	     "gmot_adm_dv, AfficheVueAdm: Impossible de charger et initialiser le synoptique" ); 
      return XDC_NOK;
    }

  /* Creation du Drawport et affichage de la vue. */
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Creation Drawport et Affichage Vue !!\n");

  	vm_ScreenSet[vl_index].drawport = 
		TdpCreateStretch( vm_ScreenSet[vl_index].screen,
		      vm_ScreenSet[vl_index].view, cm_DEFAULT_SCR_VP,
			(RECTANGLE *)NULL);

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Creation DrawPort %d \n",vm_ScreenSet[vl_index].drawport);

  	TscSetCurrentScreen( vm_ScreenSet[vl_index].screen );
  	vl_return = TdpDraw( vm_ScreenSet[vl_index].drawport );
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Affichage dessin  DrawPort %d \n",vl_return);

  /* Initialisation des etats */ 
  switch(va_type)
    {
    case cg_TYP_ARC:
	/*A Appel Synoptique General */
  	XZSE_20InitSynoptiqueAG();
      break;

    case cg_TYP_SSS:
        XZSE_21InitSynoptiqueSS( pa_name );
      break;

    case cg_TYP_TAC:
	XZSE_22InitSynoptiqueTache( pa_name );
      break;

    case cg_TYP_TRA:
	XZST_09InitSynoptiqueTrace( pa_name );
      break;

    case cg_TYP_SYNT:
	/*A Appel Synoptique General */
  	XZSE_20InitSynoptiqueAG();
      break;

    default:
      return;
    }

      XZST_03EcritureTrace( XZSTC_FONCTION,
	 "AfficheVueAdm  : OUT \n");


  return XDC_OK;
}


/*X/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
******************************************************
* SEQUENCE D'APPEL :
*/

void FreeVuesAdm( void )
/*
* CONDITION D'ERREUR :
* aucune.
*
* CONDITIONS LIMITES :
* aucune.
g*
******************************************************/
{
  int vl = cm_MAX_SCREENS;

  /* Liberation des structures DataViews relatives aux ecrans. */
  while(vl--)
    {
      GMeraseScreen( vl );
    }


  /* Liberation du buffer contenant la memorisation des vues */
  /* chargees a partir du disque. */
  vl = cm_MAX_SYN_FILE;
  while( vl-- )
    {
	/* Test si les Vues sont definies */
	if ( vm_ViewSet[vl].buffer != NULL)
	{
      		free( vm_ViewSet[vl].buffer );
	}
    }
  vl = cg_NB_TYP_SYN;
  while( vl-- )
    {
	/* Test si les Vues sont definies */
	if ( vm_HelpSet[vl].buffer != NULL)
	{
      		free( vm_HelpSet[vl].buffer );
	}
    }

  /* Liberation du buffer de donnees et destruction de la table */
  /* de symboles associee (et de ses cles ). */
  VTsttraverse( vm_DataSymTable, GM_LibereClesSymTable, NULL );
  (void) VTstdestroy( vm_DataSymTable );
  free( vm_DataBuffer );

  /* Reinitialisation des flags globaux */
  vm_ColorsLoaded = NO ;
}


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
* Rafraichit tous les ecrans contenant une vue
* DataViews.
******************************************************
* SEQUENCE D'APPEL :
*/

void RafraichitVuesAdm( void )

/*
* CONDITION D'ERREUR :
* aucune.
*
* CONDITIONS LIMITES :
* aucune.
*
******************************************************/
{
  int vl = cm_MAX_SYN_FILE;


	XZST_03EcritureTrace(XZSTC_FONCTION, "RafraichitVuesAdm : IN \n");


  /* Rafraichissement de tous les ecrans contenant une vue DataViews */
  while(vl--)
  {
    if ( ( vm_ScreenSet[vl].screen != NULL ) && ( vm_ScreenSet[vl].drawport != NULL ) )
    {
      TscSetCurrentScreen( vm_ScreenSet[vl].screen );
      (VOID) TdpDrawNext( vm_ScreenSet[vl].drawport );
    }
  }
	XZST_03EcritureTrace(XZSTC_FONCTION,"RafraichitVuesAdm : OUT \n");

}	/* Fin RafraichitVuesAdm */



/* Definition des fonctions internes */


/*
|  Fonction court-circuitant le copyright DataViews (sorry !!).
*/
void VUcopyright()
{
}


/*
|  Chargement des couleurs.
*/
static void GMloadColors( void )
{
  VIEW       vl_view;
  OBJECT     vl_drawing;
  ATTRIBUTES vl_attributes;

  vl_view = TviLoad( cm_COLORS_VIEW );
  if ( !vl_view )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	"gmot_adm_dv, GMloadColors: Impossible de charger la vue DataViews contenant les couleurs, utilisation des couleurs par default...");
      return;
    }
  vl_drawing = TviGetDrawing( vl_view );

  /* Initialisation des couleurs a partir de la vue cm_COLORS_VIEW */
  VOobAtGet( TdrGetNamedObject( vl_drawing, "fond" ), &vl_attributes );
  vm_fond = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "contexte" ), &vl_attributes );
  vm_contexte = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "gris" ), &vl_attributes );
  vm_gris = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "vert" ), &vl_attributes );
  vm_vert = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "rouge" ), &vl_attributes );
  vm_rouge = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "jaune" ), &vl_attributes );
  vm_jaune = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "bleu" ), &vl_attributes );
  vm_bleu = vl_attributes.background_color;

  VOobAtGet( TdrGetNamedObject( vl_drawing, "turquoise" ), &vl_attributes );
  vm_turquoise = vl_attributes.background_color;

  vm_ColorsLoaded = YES;

  /* Liberation de la memoire allouee pour vl_view */
  (void) TviDestroy( vl_view );
}


/*
|  Incremente le compteur *pa_NumVar du nombre de variables dynamiques
|  associees a la machine va_nom.
*/
static int GMcntMachine( 
	XZSCT_NomMachine  va_nom,
	int               va_nb,
	int              *pa_NumVar )
{
  int vl_type;

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMcntMachine : IN \n");

	XZSA_03ListeTacheMachine(va_nom,GMcntTache,&va_nb, pa_NumVar );

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMcntMachine : OUT -- NOMBRE %d \n"	,*pa_NumVar);
}



/*
|  Incremente le compteur *pa_NumVar du nombre de variables dynamiques
|  associees a une tache 
*/
static void GMcntTache( 
	XZSCT_NomTache  va_nom,
	int              va_nb,
	int              va_NumVar )
{

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMcntTache : IN \n");

	/**pa_NumVar += cm_NB_TACHES;*/
	vs_NumVar += cm_NB_TACHES;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"GMcntTache : OUT -- NOMBRE %d \n",vs_NumVar);

}	/* Fin GMcntTache */



/*
|  Incremente le compteur *pa_NumVar du nombre de variables dynamiques
|  associees a l'entite va_nom.
*/
static void GMcntEntity( XZSCT_NomEntite  va_nom,
			 int              va_type,
			 int              va_nb,
			 int             *pa_NumVar )
{
	XZST_03EcritureTrace(XZSTC_FONCTION,	
		"GMcntEntite : IN \n");


  switch( va_type )
    {
    case XZSAC_TYPE_PERI:
    case XZSAC_TYPE_IMP_NB:
    case XZSAC_TYPE_IMP_CL:
      vs_NumVar += cm_PERI_NVARS;
      break;
    case XZSAC_TYPE_INTE:
      vs_NumVar += cm_INTE_NVARS;
      break;
    default:
      break;
    }
	XZST_03EcritureTrace(XZSTC_FONCTION,	
		"GMcntEntite : OUT   -- Nombre %d \n", vs_NumVar);

}	/* Fin GMcntEntite */ 

/*X*/
/*
|  Incremente le compteur *pa_NumVar du nombre de variables dynamiques
|  associees a l'entite va_nom.
*/
static void GMcntSite( XZSCT_NomSite  va_nom,
			 int              va_nb,
			 int             *pa_NumVar )
{
	XZST_03EcritureTrace(XZSTC_FONCTION,"GMcntSite : IN \n");

      vs_NumVar += cm_SITE_NVARS;

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMcntSite : OUT -- Nb %d \n",
		vs_NumVar);

}	/* Fin GMcntSite */



/*
|  Cree les entree dans la table de symboles correspondant
|  au variables dynamiques de la machine va_nom.
*/
static int GMaddMachine( XZSCT_NomMachine      va_nom,
			  int                   va_nb,
			  tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  char 		vl_key[50]	= "";
  XZSCT_NomSite vl_site		= "";
  int 		vl_type		= 0;
  int 		dummy		= 0;
  int 		vl_Ind		= XDC_FAUX;

XZST_03EcritureTrace(XZSTC_FONCTION,"GMaddMachine : IN -- Nom %s Nb %d ",
		pa_Args->machine,pa_Args->currIndex);

  strcpy( pa_Args->machine, va_nom );
  (void) XZSA_23SiteMachine( va_nom, pa_Args->site );
  if ( (vg_TypeMachine == XZSCC_TYPEM_PCS) && ( strcmp( pa_Args->site , vg_NomSite)))
     return XDC_OK;
     
  /* Cree les entree des variables concernant le synoptique ARC. */
  (void) XZSA_17TypeMachine( va_nom, &vl_type );
  switch( vl_type )
    {
    case XZSAC_TYPEM_ST:
    case XZSAC_TYPEM_STA:
    case XZSAC_TYPEM_HIS:
      if (pa_Args->currIndex + cm_ST_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	    "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );

      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_modeI" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    case XZSAC_TYPEM_SP : 
      if (pa_Args->currIndex + cm_SP_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	    "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );

      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_modeI" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    case XZSAC_TYPEM_LT :
      if (pa_Args->currIndex + cm_LT_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	    "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );

      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_modeI" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    case XZSAC_TYPEM_SD:
      if (pa_Args->currIndex + cm_SD_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ), 
			   &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    case XZSAC_TYPEM_PC2:
    case XZSAC_TYPEM_PC2LT:
    case XZSAC_TYPEM_SC:
      if (pa_Args->currIndex + cm_SC_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ), 
			   &pa_Args->buffer[pa_Args->currIndex++] );

/*      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_modeI" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ), 
			   &pa_Args->buffer[pa_Args->currIndex++] );*/
      break;
    case XZSAC_TYPEM_PCS:
      if (pa_Args->currIndex + cm_ST_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	    "gmot_adm_dv, GMaddMachine: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      (void) VTstsninsert( vm_DataSymTable,
			   VUstrClone( vl_key ),
			   &pa_Args->buffer[pa_Args->currIndex++] );

      break;
    default:
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddMachine: %d = Type de machine inconnu. ", vl_type );
      return;
    }
XZST_03EcritureTrace(XZSTC_FONCTION,"GMaddMachine : -- Nom %s Nb %d ",
		pa_Args->machine,pa_Args->currIndex);

  /* Cree les entree des varables concernant le synoptique SSS... */
  (void) XZSA_02ListeSSysteme( va_nom, GMbuildSSS, &dummy, pa_Args );

  /* ...et les synoptiques TAC et TRA. */
  (void) XZSA_03ListeTacheMachine( va_nom, GMbuildTACandTRA, &dummy, pa_Args );

XZST_03EcritureTrace(XZSTC_FONCTION,"GMaddMachine : OUT -- Nom %s Nb %d ",
		pa_Args->machine,pa_Args->currIndex);

}	/* Fin GMaddMachine */




/*X*/
/*
|  Cree dans la table de symboles, l'entree de la variable dynamique
|  correspondant a l'etat du sous-systemes va_ssyst de la machine pa_Args->machine. 
*/
static void GMbuildSSS( XZSCT_NomSSysteme     va_ssyst,
		        int                   va_nb,
		        tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  char  vl_key[50];

XZST_03EcritureTrace(XZSTC_WARNING, "Insert cle SSS:%s machine:%s",va_ssyst,pa_Args->machine);
  sprintf( vl_key, "%s%s%c%s%c%s%s", "SSS_",
	                             pa_Args->site,
	                             '_',
	                             pa_Args->machine,
	                             '_',
	                             va_ssyst,
	                             "_etat" );
  (void) VTstsninsert( vm_DataSymTable,
		       VUstrClone( vl_key ),
		       &pa_Args->buffer[pa_Args->currIndex++] );
}


/*
|  Cree dans la table de symboles, l'entree des variables dynamiques
|  correspondant a l'etat de la tache va_tac de la machine pa_Args->machine et
|  au positionnement de ses 8 flags de Trace (synoptiques TAC et TRA).
*/
static void GMbuildTACandTRA( XZSCT_NomTache        va_tac,
			      int                   va_nb,
			      tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  char 	vl_key[50];
  int 	vl_n 		= cm_MAX_DEBUG;

  XZST_03EcritureTrace(XZSTC_FONCTION, "GMbuildTACandTRA : IN \n");

  sprintf( vl_key, "%s%s%c%s%s", "TAC_",
	                         pa_Args->machine,
	                         '_',
	                         va_tac,
	                         "_etat" );

XZST_03EcritureTrace(XZSTC_DEBUG1, "Insert cle");
XZST_03EcritureTrace(XZSTC_DEBUG1, "Insert cle %s -- %d \n",
	vl_key,pa_Args->currIndex);
XZST_03EcritureTrace(XZSTC_DEBUG1, "Insert cle");

XZST_03EcritureTrace(XZSTC_WARNING, "Insert cle tache:%s machine:%s",va_tac,pa_Args->machine);


  (void) VTstsninsert( vm_DataSymTable,
		       VUstrClone( vl_key ),
		       &pa_Args->buffer[pa_Args->currIndex++] );



	/* si la tache est telmxx.xs alors on la memorise pas ==> gain de place dans la table PNI DEM691 */
/*  if (!((!strncmp(va_tac,"tel",3) && (va_tac[strlen(va_tac)-1]=='s'))))*/
  if (!(((va_tac[strlen(va_tac)-1]=='s')||(!strncmp(va_tac,"tele",4)))))
  {
XZST_03EcritureTrace(XZSTC_WARNING, "Insert cle tache:%s machine:%s %d %d",va_tac,pa_Args->machine,pa_Args->currIndex, vl_n);
  while(vl_n--)
    {
 XZST_03EcritureTrace(XZSTC_DEBUG1,"Insert cle %s%s%c%s%c.%d","TRAV_",
	                               pa_Args->machine,
	                               '_',
	                               va_tac,
	                               '_',vl_n);
      sprintf( vl_key, "%s%s%c%s%c%s", "TRAV_",
	                               pa_Args->machine,
	                               '_',
	                               va_tac,
	                               '_',
	                               vm_DebugTab[vl_n] );


  XZST_03EcritureTrace(XZSTC_DEBUG1, "Insert cle %s -- %d %xh\n",
	vl_key,pa_Args->currIndex,pa_Args->currIndex);
	

      VTstsninsert( vm_DataSymTable_T,
		    VUstrClone( vl_key ),
		    &pa_Args->buffer[pa_Args->currIndex++] );
  XZST_03EcritureTrace(XZSTC_DEBUG1, "  Insert cle %s -- %d \n",
	vl_key,pa_Args->currIndex);
    }
  }
  else
	XZST_03EcritureTrace(XZSTC_WARNING, "Pas Insert cle tache:%s machine:%s",va_tac,pa_Args->machine);
 
  XZST_03EcritureTrace(XZSTC_FONCTION, "GMbuildTACandTRA : IN ???\n");

} 	/*Fin GMbuildTACandTRA */



/*X*/
/*
|  Cree les entree dans la table de symboles correspondant
|  au variables dynamiques de l'entite va_nom.
*/
static void GMaddEntity( XZSCT_NomEntite       va_nom,
			 int                   va_type,
			 int                   va_nb,
			 tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  char 	vl_key[50];

  switch( va_type )
    {
/* Modif V1.5 */
    case XZSAC_TYPE_PERI:
    case XZSAC_TYPE_IMP_NB:
    case XZSAC_TYPE_IMP_CL:
      if (pa_Args->currIndex + cm_PERI_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddEntity: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      VTstsninsert( vm_DataSymTable,
		    VUstrClone( vl_key ), 
		    &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    case XZSAC_TYPE_INTE:
      if (pa_Args->currIndex + cm_INTE_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddEntity: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      sprintf( vl_key, "%s%s%s", "ARC_", va_nom, "_etat" );
      VTstsninsert( vm_DataSymTable,
		    VUstrClone( vl_key ),
		    &pa_Args->buffer[pa_Args->currIndex++] );
      break;
    default:
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddEntity: %d = Type d'entite inconnu.", va_type );
      return;
    }
}	/* Fin GMaddEntity */

/*X*/
/*
|  Cree les entree dans la table de symboles correspondant
|  au variables dynamiques de l'entite va_nom.
*/
static void GMaddSite( XZSCT_NomSite       va_nom,
			 int                   va_nb,
			 tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  char 	vl_key[50];

  if ( (vg_TypeMachine == XZSCC_TYPEM_PCS) && ( strcmp( va_nom , vg_NomSite)))
     return ;
     
      	if (pa_Args->currIndex + cm_SITE_NVARS > pa_Args->maxIndex)
	{
	  /* Erreur */
          XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMaddSite: Buffer de donnees trop petit. Recensement des variables incorrect.");
	  return;
	}
      
	sprintf( vl_key, "%s%s%s", "SYNT_", va_nom, "_stat_mach" );
      	VTstsninsert( vm_DataSymTable,
		    VUstrClone( vl_key ), 
		    &pa_Args->buffer[pa_Args->currIndex++] );

      	sprintf( vl_key, "%s%s%s", "SYNT_", va_nom, "_stat_res" );
      	VTstsninsert( vm_DataSymTable,
		    VUstrClone( vl_key ),
		    &pa_Args->buffer[pa_Args->currIndex++] );

}	/* Fin GMaddSite */



static void TESTprintNode( char *key, int *value, char* args)
{
  fprintf( stderr, "\tkey = %s\n", key );
}

/*
|  Libere la cle de la table de symboles DataSymTable 
*/
static void GM_LibereClesSymTable( char *key, int *value, char* args)
{
  free( key );
}

/*
|  Efface la fenetre d'index va dans le tableau vm_ScreenSet.
*/
static void GMeraseScreen( int va )
{
  (void) TscSetCurrentScreen(vm_ScreenSet[va].screen);

  if (vm_ScreenSet[va].help.view != (VIEW)NULL )
    {
      (void) TdpDestroy( vm_ScreenSet[va].help.drawport );
      (void) TviDestroy( vm_ScreenSet[va].help.view );
      vm_ScreenSet[va].help.drawport = (DRAWPORT) NULL;
      vm_ScreenSet[va].help.view = (VIEW) NULL;
    }
  if (vm_ScreenSet[va].view != (VIEW)NULL)
    {
      (void) TdpDestroy( vm_ScreenSet[va].drawport );
      if ( vm_ScreenSet[va].view != vue_arch ) 
         (void) TviDestroy( vm_ScreenSet[va].view );
      else
	 vg_nb_vues_arch_ecran = vg_nb_vues_arch_ecran - 1 ;

      TscErase( vm_ScreenSet[va].screen );
      vm_ScreenSet[va].drawport = (DRAWPORT) NULL;
      vm_ScreenSet[va].view = (VIEW) NULL;

      VUerClearAll( vm_ScreenSet[va].screen );
    }
}


/*
|  Modifie les couleurs de la threshold table
|  passee en parametres.
*/
static void  GMupdateTTable( OBJECT va_ttable, int va_nb, ... )
{
  va_list    vl_ap;
  int        vl;
  int        vl_thresh;
  DATUM      vl_datum;
  DATUM_DESC vl_dd;

  if ( va_nb <= 0 )
    {
      return;
    }
  va_start( vl_ap, va_nb );

  for( vl=0; vl < va_nb; vl++ )
    {
      VOttGetThresh( va_ttable, vl, &vl_thresh, &vl_datum );
      vl_dd.O = va_arg( vl_ap, OBJECT );
      VOttSetDatum( va_ttable, vl_thresh, vl_dd.DATUM_alias );
    }

  va_end( vl_ap );
}


/*
|  Initialisation du synoptique SSS (sous-systemes)
|  sur l'ecran pa_info->screen.
*/
static int GMinitSynSSS( tm_SCR_INFO   *pa_info,
			  XZSCT_NomSite  va_site)
{
  tm_SSS_TRAVERSE_ARGS vl_args;
  OBJECT               vl_vtitle, vl_drawing;
  OBJECT               vl_bckColor, vl_freColor;
  int                  vl_nb;

  /* Chargement de la vue mere du synoptique. */
  pa_info->view = TviMemLoad( vm_ViewSet[cg_TYP_SSS ].buffer,
			      vm_ViewSet[cg_TYP_SSS ].size );
  if ( !pa_info->view )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMinitSynSSS: Chargement de vue impossible: TviLoad echoue.");
      return XDC_NOK;
    }
  vl_drawing = TviGetDrawing( pa_info->view );

  /* Positionne les couleurs de l'objet drawing */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
    }

  /* Positionnement du titre du sous-dessin. */
  vl_vtitle = VOdrGetNamedObject( vl_drawing, "title" );
  VOvtSetString( vl_vtitle, va_site );

  /* Positionnement de l'event handler */
  (void) TscSetCurrentScreen( pa_info->screen );
  VUerRectEdgePost( pa_info->screen, GMhandlePicking,
		    (char *)pa_info, 0, NULL, V_INSIDE, "\001\002\003", cg_TYP_SSS );

  /* Initialisation des parametres de traversee */
  vl_args.pointNum = 0;
  vl_args.lineNum = 1;
  vl_args.drawing = vl_drawing;
  strcpy( vl_args.site, va_site );

  /* Pour chaque machine du site, insere le bon sous-dessin. */
  XZSA_19ListeMachineSite( va_site, &GMcomposeSSS, &vl_nb, &vl_args );

  return XDC_OK;
}


/*
|  Insere le sous-dessin correspondant a la machine va_name
|  dans le synoptique SSS. Les deux points de controle de chaque
|  objet ligne present dans la vue mere, servent de points d'ancrage
|  pour deux sous-dessins representant deux machines du reseau.
|  ( les objets lignes possedent un dynamisme de visibilite relie
|  a une constante -> ils sont toujours invisibles )
*/
static int GMcomposeSSS( XZSCT_NomMachine      va_name,
			  int                   va_nb,
			  tm_SSS_TRAVERSE_ARGS *pa_args )
{
  char       vl_lineName[20];
  char       vl_dynVarPrefix[40];
  OBJECT     vl_line;
  OBJECT     vl_anchorPt;
  OBJECT     vl_subDrawing;
  OBJECT     vl_drawing;
  OBJECT     vl_vtitle;
  VIEW	     vl_view;
  int        vl_type;

  /* Test si la capacite maximale d'affichage est atteinte */
  /* (on peut afficher jusqu'a cm_SSS_SD_MAX machines). */
  if ( 2*pa_args->lineNum == cm_SSS_SD_MAX+2 && pa_args->pointNum == 0 )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMcomposeSSS: Depassement des capacites d'affichage pour %s (plus de 30 machines)", pa_args->site );
      return;
    }

  /* Recherche du point d'insertion du sous-dessin. */
  sprintf( vl_lineName, "%s%d", "CTRL_line", pa_args->lineNum );
  vl_line = TdrGetNamedObject( pa_args->drawing, vl_lineName );
  vl_anchorPt = VOobPtGet( vl_line, pa_args->pointNum+1 );

  /* Creation du sous-dessin. */
  (void) XZSA_17TypeMachine( va_name, &vl_type );
  vl_view = TviClone( vm_SSSView[vl_type-1] );
  vl_subDrawing = VOsdCreate( NULL, vl_view, vl_anchorPt, 1.0, (ATTRIBUTES*)NULL );
  /* Cota: vl_subDrawing = VOsdCreate( vm_SSSsubView[vl_type-1], NULL, vl_anchorPt, 1.0, (ATTRIBUTES*)NULL ); */
  if ( !vl_subDrawing )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMcomposeSSS: Impossible de creer le sous-dessin pour la machine  %s.", va_name );
      return;
    }

  /* Permet les dynamismes internes du sous-dessin. */
  VOsdSetDynamicFlag( vl_subDrawing, SD_DYN_ENABLED );

  /* Insertion du sous-dessin dans la vue mere du synoptique. */
  vl_drawing = TviGetDrawing( VOsdViGet( vl_subDrawing ) );
  if (VOdrObAdd( pa_args->drawing, vl_subDrawing ) == NO)
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMcomposeSSS: Impossible d'inserer un sous-dessin: VOdrObAdd echoue." );
      (void) TviDestroy( VOsdViGet( vl_subDrawing ) );
      return;
    }

  /* Mise a jour des couleurs */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrBackcolor( vl_drawing, vm_fond );
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      TdrForEachNamedObject( vl_drawing, GMcolorSSS, NULL );
    }

  /* Positionnement du titre du sous-dessin. */
  vl_vtitle = VOdrGetNamedObject( vl_drawing, "SSS_NomMachine" );
  VOvtSetString( vl_vtitle, va_name );

  /* Rebind des variables dynamiques du sous-dessin sur les
     entrees correspondantes dans la table de symboles. */
  sprintf( vl_dynVarPrefix, "%s%s%c%s%c", "SSS_", pa_args->site, '_', va_name, '_' );
  (void) TobForEachVdp( vl_drawing, GMrebindSSS, vl_dynVarPrefix );

  /* Incrementation des indices d'iteration: */
  /* lineNum varie de 1 a cg_SSS_SD_MAX, */
  /* et pointNum vaut alternativement 0 et 1. */
  pa_args->lineNum += pa_args->pointNum;
  pa_args->pointNum = (pa_args->pointNum+1) % 2;
}


/*
|  Modifie les couleurs des dynamismes de l'objet courant situe
|  dans un sous-dessin de la vue mere.
*/
static char *GMcolorSSS( OBJECT  va_object,
			 char   *pa_name,
			 char   *pa_nil )
{
  OBJECT     vl_dynamic;
  OBJECT     vl_ttable;

  /* Recherche de l'objet threshold table associe */
  /* au dynamisme de couleur de fond de l'objet courant. */
  vl_dynamic = VOobDyGet( va_object );
  if ( VOdyValid( vl_dynamic ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }
  vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
  if ( VOttValid( vl_ttable ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Mise a jour des couleurs. */
  GMupdateTTable( vl_ttable, 3, vm_fond, vm_rouge, vm_vert );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Redirige le pointeur du descripteur de variable va_vdp sur la bonne
|  entree dans le buffer (par l'intermediaire de la table de symboles).
*/
static char *GMrebindSSS( OBJECT   va_vd,
			  VARDESC  va_vdp,
			  char    *pa_prefix )
{
  char    *pl_ssyst = NULL;
  char    vl_key[50];
  SYMNODE vl_node;

  /* Recherche du nom de la variable dynamique. */
  /* Celle-ci doit etre le nom du sous-systeme correspondant. */
  pl_ssyst = VGvdvarname( va_vdp );
  if (!pl_ssyst)
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMrebindSSS: Data object non nomme-> impossible de composer la cle (prefix= %s).", pa_prefix );
      return V_CONTINUE_TRAVERSAL;
    }

  /* Construction de la clef d'acces dans la table de symboles. */
  sprintf( vl_key, "%s%s%s", pa_prefix, pl_ssyst, "_etat" );

  /* Redirection du descripteur de variable sur l'entree voulue. */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMrebindSSS: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return V_CONTINUE_TRAVERSAL;
    }
  (void) TvdPutBuffer( va_vdp, (char *) VTsnvalue( vl_node ) );

  return V_CONTINUE_TRAVERSAL;

}	/* Fin GMrebindSSS */


/*
|  Initialise un synoptique qui peut etre des trois types
|  ARC (architecture generale), TAC (taches) ou TRA (traces)
*	et SYNT (synthese).
*/
static int GMinitSyn( tm_SCR_INFO *pa_info,
		       int          va,
		       tg_TYP_SYN   va_type,
		       char        *pa_name )
{
  OBJECT                 vl_drawing;
  OBJECT                 vl_vtitle;
  tm_OTHER_TRAVERSE_ARGS vl_args;

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMinitSyn : IN \
		-- Num Synop %d -- Type Synop %d -- \
		Name %s \n", va,va_type,*pa_name );


  if ( ( va==0 ) && (vue_arch!=NULL) )
  {
     if ( vg_nb_vues_arch_ecran > 0 )
        pa_info->view = TviClone( vue_arch );
     else
	pa_info->view = vue_arch;
     
     vg_nb_vues_arch_ecran = vg_nb_vues_arch_ecran + 1;

     /* Positionnement de l'event handler */
     (void) TscSetCurrentScreen( pa_info->screen );
     VUerRectEdgePost( pa_info->screen, GMhandlePicking,
   		       (char *)pa_info, 0, NULL, V_INSIDE, "\001\002\003", va_type );
     return(XDC_OK);
  }


  /* Chargement de la vue du synoptique. */
  pa_info->view = TviMemLoad( vm_ViewSet[va].buffer,
			      vm_ViewSet[va].size );

  if  ( va==0 )
     vue_arch = pa_info->view;

  if ( !pa_info->view )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, GMinitSyn: Chargement de vue impossible: TviLoad echoue" );
      return XDC_NOK;
    }
  vl_drawing = TviGetDrawing( pa_info->view );

  /* Positionne les couleurs de l'objet drawing */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
    }

  /* Positionnement du titre du sous-dessin */
  /* ( = NULL pour ARC, et nom de la machine pour TAC et TRA). */
  if ( pa_name )
    {
      vl_vtitle = VOdrGetNamedObject( vl_drawing, "title" );
      VOvtSetString( vl_vtitle, pa_name );
    }

  /* Positionnement de l'event handler */
  (void) TscSetCurrentScreen( pa_info->screen );
  VUerRectEdgePost( pa_info->screen, GMhandlePicking,
		    (char *)pa_info, 0, NULL, V_INSIDE, "\001\002\003", va_type );

  /* Traverse les objets de la vue et redirige les */
  /* pointeurs des descripteurs de variables dynamiques */
  /* (modifie les couleurs, au besoin). */
  vl_args.syn    = va_type;
  vl_args.color  = vm_Config[va_type - 1].color;
  vl_args.rebind = vm_Config[va_type - 1].rebind;
  if ( va_type == cg_TYP_TAC || va_type == cg_TYP_TRA )
    {
      (void) strcpy( vl_args.machine, pa_name );
    }

  return ( (int) TdrForEachNamedObject( vl_drawing, GMtraverse, (char*)&vl_args ) );

	XZST_03EcritureTrace(XZSTC_FONCTION,"GMinitSyn : OUT \n");
 
}	/* Fin GMinitSyn */


/*
|  Appliquee a chaque sous-dessin de la vue mere d'un synoptique
|  ARC, TAC ou TRA, modifie au besoin les couleurs et redirige
|  les pointeurs des descripteurs de variables dynamiques.
*/
static char *GMtraverse( OBJECT                 va_object,
			 char                   *pa_name,
			 tm_OTHER_TRAVERSE_ARGS *pa_args )
{
  OBJECT vl_drawing;

  if ( VOobType( va_object ) != OT_SUBDRAWING )
    {
      return V_CONTINUE_TRAVERSAL;
    }
  vl_drawing = TviGetDrawing( VOsdViGet( va_object ) );

  /* Mise a jour des couleurs */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrBackcolor( vl_drawing, vm_fond );
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      TdrForEachNamedObject( vl_drawing, pa_args->color, NULL );
    }

  /* Redirection des pointeurs des descripteurs de variables */
  /* (va_name doit etre le nom de la machine ou de l'entite). */
  if ( pa_args->syn == cg_TYP_TAC || pa_args->syn == cg_TYP_TRA )
    {
      (void) strcpy( pa_args->task, pa_name );
    }
  else if ( pa_args->syn == cg_TYP_ARC)
    {
      (void) strcpy( pa_args->machine, pa_name );
    }
    else if ( pa_args->syn == cg_TYP_SYNT)
    {
	(void) strcpy( pa_args->site,pa_name );
    }

  (void) TobForEachVdp( vl_drawing, pa_args->rebind, (char*)pa_args );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Redirige le pointeur du descripteur de variable va_vdp associe
|  au data object va_vd. Le pointeur en question doit alors pointer
|  sur l'emplacement prevu dans le buffer de donnees pour contenir
|  la valeur courante de la variable associee au dynamisme de l'objet.
*/
static char *GMrebindSYNT( OBJECT                  va_vd,
			  VARDESC                 va_vdp,
			  tm_OTHER_TRAVERSE_ARGS *pa_args)
{
  char    *pl_name = NULL;
  char    vl_key[50];
  SYMNODE vl_node;
  int	*pl_valeur;


  /* Recherche du nom de la variable dynamique. */
  /* Celle-ci doit etre le nom du parametre d'etat correspondant. */
  pl_name = VGvdvarname( va_vdp );
  if (!pl_name)
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindSYNT: Data object non nomme-> impossible de composer la cle (site= %s)", pa_args->site );
      return V_CONTINUE_TRAVERSAL;
    }

  /* Construction de la clef d'acces dans la table de symboles. */
  sprintf( vl_key, "%s%s%c%s", "SYNT_", pa_args->site, '_', pl_name );

	XZST_03EcritureTrace(XZSTC_DEBUG1,"Insert Cle SYNT %s \n",
		vl_key);

  /* Redirection du descripteur de variable sur l'entree voulue. */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindSYNT: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return V_CONTINUE_TRAVERSAL;
    }


	/*A On foce la valeur a TOUT OK */
	pl_valeur = VTsnvalue( vl_node );
	*pl_valeur = 1;


  (void) TvdPutBuffer( va_vdp, (char *) (pl_valeur));

  return V_CONTINUE_TRAVERSAL;

}	/* Fin GMrebindSYNT */

/*
|  Modifie les couleurs des dynamismes de l'objet courant situe
|  dans un sous-dessin de la vue mere.
*/
static char *GMcolorSYNT( OBJECT  va_object,
			 char   *va_type,
			 char   *va_nil)
{
  OBJECT vl_dynamic;
  OBJECT vl_ttable;

  /* Recherche de l'objet threshold table associe */
  /* au dynamisme de couleur de fond de l'objet courant. */
  vl_dynamic = VOobDyGet( va_object );
  if ( VOdyValid( vl_dynamic ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Mise a jour des couleurs. */
  if ( strcmp( va_type, "MACH_SYNT" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 2, vm_rouge, vm_vert );
    }
  else if ( strcmp( va_type, "RES_SYNT" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable,2 , vm_fond, vm_vert);
    }


  return V_CONTINUE_TRAVERSAL;

}	/* Fin GMcolorSYNT */




/*
|  Redirige le pointeur du descripteur de variable va_vdp associe
|  au data object va_vd. Le pointeur en question doit alors pointer
|  sur l'emplacement prevu dans le buffer de donnees pour contenir
|  la valeur courante de la variable associee au dynamisme de l'objet.
*/
static char *GMrebindARC( OBJECT                  va_vd,
			  VARDESC                 va_vdp,
			  tm_OTHER_TRAVERSE_ARGS *pa_args)
{
  char    *pl_name = NULL;
  char    vl_key[50];
  SYMNODE vl_node;

  /* Recherche du nom de la variable dynamique. */
  /* Celle-ci doit etre le nom du parametre d'etat correspondant. */
  pl_name = VGvdvarname( va_vdp );
  if (!pl_name)
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindARC: Data object non nomme-> impossible de composer la cle (machine= %s)", pa_args->machine );
      return V_CONTINUE_TRAVERSAL;
    }

  /* Construction de la clef d'acces dans la table de symboles. */
  sprintf( vl_key, "%s%s%c%s", "ARC_", pa_args->machine, '_', pl_name );

  /* Redirection du descripteur de variable sur l'entree voulue. */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindARC: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return V_CONTINUE_TRAVERSAL;
    }
  (void) TvdPutBuffer( va_vdp, (char *) VTsnvalue( vl_node ) );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Modifie les couleurs des dynamismes de l'objet courant situe
|  dans un sous-dessin de la vue mere.
*/
static char *GMcolorARC( OBJECT  va_object,
			 char   *va_type,
			 char   *va_nil)
{
  OBJECT vl_dynamic;
  OBJECT vl_ttable;

  /* Recherche de l'objet threshold table associe */
  /* au dynamisme de couleur de fond de l'objet courant. */
  vl_dynamic = VOobDyGet( va_object );
  if ( VOdyValid( vl_dynamic ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Mise a jour des couleurs. */
  if ( strcmp( va_type, "ST" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 5, vm_fond, vm_rouge, vm_bleu, vm_vert, vm_gris );
	vl_ttable = VOdyGetDataObj( vl_dynamic, FOREGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 2, vm_contexte, vm_jaune );
    }
  else if ( strcmp( va_type, "IN" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 3, vm_fond, vm_rouge, vm_vert );
    }
  else if ( strcmp( va_type, "PE" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 3, vm_fond, vm_rouge, vm_vert );
    }
  else if ( strcmp( va_type, "SE" ) == 0 )
    {
        vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
	GMupdateTTable( vl_ttable, 5, vm_fond, vm_rouge, vm_bleu, vm_vert, vm_gris );
    }

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Redirige le pointeur du descripteur de variable va_vdp associe
|  au data object va_vd. Le pointeur en question doit alors pointer
|  sur l'emplacement prevu dans le buffer de donnees pour contenir
|  la valeur courante de la variable associee au dynamisme de l'objet.
*/
static char *GMrebindTAC( OBJECT                  va_vd,
			  VARDESC                 va_vdp,
			  tm_OTHER_TRAVERSE_ARGS *pa_args)
{
  char    vl_key[50];
  SYMNODE vl_node;

  /* Construction de la clef d'acces dans la table de symboles. */
  sprintf( vl_key, "%s%s%c%s%s", "TAC_", pa_args->machine, '_', pa_args->task, "_etat" );

  /* Redirection du descripteur de variable sur l'entree voulue. */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindARC: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return V_CONTINUE_TRAVERSAL;
    }
  (void) TvdPutBuffer( va_vdp, (char *) VTsnvalue( vl_node ) );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Modifie les couleurs des dynamismes de l'objet courant situe
|  dans un sous-dessin de la vue mere.
*/
static char *GMcolorTAC( OBJECT  va_object,
			 char   *pa_name,
			 char   *pa_nil )
{
  OBJECT     vl_dynamic;
  OBJECT     vl_ttable;
  int        vl = 0;
  int        vl_thresh;
  DATUM      vl_datum;
  DATUM_DESC vl_dd;

  if ( strcmp( pa_name, "TASK" ) != 0 )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Recherche de la threshold table associee au dynamisme */
  /* de la couleur d'avant plan. */
  vl_ttable = VOdyGetDataObj( VOobDyGet( va_object ), FOREGROUND_COLOR, 1 );

  /* Mise a jour des couleurs. */
  GMupdateTTable( vl_ttable, 3, vm_fond, vm_rouge, vm_vert );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Redirige le pointeur du descripteur de variable va_vdp associe
|  au data object va_vd. Le pointeur en question doit alors pointer
|  sur l'emplacement prevu dans le buffer de donnees pour contenir
|  la valeur courante de la variable associee au dynamisme de l'objet.
*/
static char *GMrebindTRA( OBJECT                  va_vd,
			  VARDESC                 va_vdp,
			  tm_OTHER_TRAVERSE_ARGS *pa_args)
{
  char    *pl_debugMode = NULL;
  char     vl_key[50];
  SYMNODE  vl_node;

      XZST_03EcritureTrace( XZSTC_FONCTION,"GMrebindTRA : IN \n");


  /* Recherche du nom de la variable dynamique. */
  /* Celle-ci doit etre le nom du parametre d'etat correspondant. */
  pl_debugMode = VGvdvarname( va_vdp );
  if (!pl_debugMode)
    {
      /* Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindTRA: Data object non nomme-> impossible de composer la cle (machine= %s)", pa_args->machine );
      return V_CONTINUE_TRAVERSAL;
    }

  /* Construction de la clef d'acces dans la table de symboles. */
  sprintf( vl_key, "%s%s%c%s%c%s", "TRAV_", pa_args->machine, '_',
	                         pa_args->task, '_', pl_debugMode );


      XZST_03EcritureTrace( XZSTC_DEBUG1,"Cle %s \n",vl_key);
	

  /* Redirection du descripteur de variable sur l'entree voulue. */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMrebindTRA: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return V_CONTINUE_TRAVERSAL;
    }
  (void) TvdPutBuffer( va_vdp, (char *) VTsnvalue( vl_node ) );

      XZST_03EcritureTrace( XZSTC_FONCTION,"GMrebindTRA : OUT \n");

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Modifie les couleurs des dynamismes de l'objet courant situe
|  dans un sous-dessin de la vue mere.
*/
static char *GMcolorTRA( OBJECT  va_object,
			 char   *pa_name,
			 char   *pa_nil )
{
  OBJECT     vl_ttable;

  if ( strcmp( pa_name, "TRA" ) != 0 )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Recherche de la threshold table associee au dynamisme */
  /* de la couleur d'avant plan. */
  vl_ttable = VOdyGetDataObj( VOobDyGet( va_object ), BACKGROUND_COLOR, 1 );

  /* Mise a jour des couleurs. */
  GMupdateTTable( vl_ttable, 2, vm_fond, vm_vert );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Positionne la couleur de fond de l'objet Ss-Systeme courant
*/
static char *GMhelpSSScolor( OBJECT va_obj, char *pa_name, char *pa_dummy )
{
  OBJECT     vl_dynamic;
  OBJECT     vl_ttable;

  /* Recherche de l'objet threshold table associe */
  /* au dynamisme de couleur de fond de l'objet courant. */
  vl_dynamic = VOobDyGet( va_obj );
  if ( VOdyValid( vl_dynamic ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }
  vl_ttable = VOdyGetDataObj( vl_dynamic, BACKGROUND_COLOR, 1 );
  if ( VOttValid( vl_ttable ) == NO )
    {
      return V_CONTINUE_TRAVERSAL;
    }

  /* Mise a jour des couleurs. */
  GMupdateTTable( vl_ttable, 1, vm_fond );

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Change la couleur de fond de l'objet (rectangle)
*/
static char *GMcolorHelp( OBJECT va_obj, char *pa_name, char *pa_dummy )
{
  ATTRIBUTES vl_att;
  OBJECT     vl_drawing;

  if ( VOobType( va_obj ) == OT_SUBDRAWING )
    {
      vl_drawing = TviGetDrawing( VOsdViGet( va_obj ) );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      TdrForEachNamedObject( vl_drawing, GMhelpSSScolor, NULL );

      return V_CONTINUE_TRAVERSAL;
    }

  VOuAtInit( &vl_att );
  if ( strcmp( pa_name, "gris" ) == 0 )
    {
      vl_att.background_color = vm_gris;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "vert" ) == 0 )
    {
      vl_att.background_color = vm_vert;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "bleu" ) == 0 )
    {
      vl_att.background_color = vm_bleu;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "rouge" ) == 0 )
    {
      vl_att.background_color = vm_rouge;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "fond" ) == 0 )
    {
      vl_att.background_color = vm_fond;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "turquoise" ) == 0 )
    {
      vl_att.background_color = vm_turquoise;
      VOobAtSet( va_obj, &vl_att );
    }
  else if ( strcmp( pa_name, "jaune" ) == 0 )
    {
      vl_att.background_color = vm_jaune;
      VOobAtSet( va_obj, &vl_att );
    }

  return V_CONTINUE_TRAVERSAL;
}


/*
|  Initialise et affiche la vue d'aide
|  correspondant a l'index passe.
*/
static void  GMinitHelpSyn( tm_SCR_INFO *pa_info, int va_index )
{
  OBJECT vl_drawing;

  pa_info->help.view = TviMemLoad( vm_HelpSet[va_index].buffer,
		                   vm_HelpSet[va_index].size );
  if ( vm_ColorsLoaded )
    {
      vl_drawing = TviGetDrawing( pa_info->help.view );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      TdrForEachNamedObject( vl_drawing, GMcolorHelp, (char *)NULL );
    }

  pa_info->help.drawport = TdpCreateStretch( pa_info->screen,
					     pa_info->help.view,
					     cm_DEFAULT_SCR_VP,
					     &vm_WholeWorld );

  (void) TscSetCurrentScreen( pa_info->screen );
  (void) TscErase( pa_info->screen );
  (void) TdpDraw( pa_info->help.drawport );
}


/*
|  Efface la vue d'aide courante et restore le synoptique.
*/
static void  GMhelpToSyn( tm_SCR_INFO* pa_info )
{
  OBJECT vl_drawing;

  (void) TdpDestroy( pa_info->help.drawport );
  (void) TviDestroy( pa_info->help.view );


  (void) TscSetCurrentScreen( pa_info->screen );
  (void) TscErase( pa_info->screen );

  vl_drawing = TviGetDrawing( pa_info->view );
  (void) VOdrForecolor( vl_drawing, vm_contexte );
  (void) VOdrBackcolor( vl_drawing, vm_fond );

  (void) TdpRedraw( pa_info->drawport, NULL, YES );

  pa_info->help.drawport = (DRAWPORT)NULL;
  pa_info->help.view = (VIEW)NULL;
}


/*
|  Service routine DataViews pour traiter les evenements de
|  clickages de souris sur les synoptiques de supervision.
*/
static int GMhandlePicking( OBJECT         va_client,
			    EVENT_REQUEST  va_request,
			    int            va_label,
			    OBJECT         va_location,
			    tm_SCR_INFO   *pa_info )
{
  int     vl_result = XDC_OK;
  int     vl_type;
  int     vl_button;
  OBJECT  vl_drawing;
  OBJECT  vl_SelectedObject;
  OBJECT  vl_vtext;
  char    vl_nom[20];
  char   *pl_pt;
  int		vl_numscreen	= 0;

  vl_button = VOloButton( va_location );

  /* Traiter le cas simple de l'affichage des aides. */
  if ( pa_info->help.drawport != NULL )
    {
      if ( vl_button == 2 )
	{
          GMhelpToSyn( pa_info );
	}
      return XDC_OK;
    }

  if ( vl_button == 2 )
    {
      GMinitHelpSyn( pa_info, va_label - 1 );

      return XDC_OK;
    }
      
  /* Traiter le reste des cas (synoptique de supervision affiche). */
  switch( va_label )
    {
    /*
    |  Le synoptique est du type ARC
    */
    case cg_TYP_ARC:
      switch( vl_button )
	{
	case 3:   /* Bouton droit: ne rien faire */
	  break;
	case 1:   /* Bouton gauche: ouvrir SSS si cliquage sur un nom de site, */
                                 /* ouvrir TAC si cliquage sur une machine. */

	  vl_type = VOobType( TloGetSelectedObject( va_location ) );
	  if ( vl_type == OT_SUBDRAWING )
	    {
	      strcpy( vl_nom, TloGetSelectedObjectName( va_location ) );
	      vl_result = AfficheVueAdm( pa_info->widget, cg_TYP_TAC, vl_nom );
	    }
	  else if ( vl_type == OT_VTEXT)
	    {
	      pl_pt = TloGetSelectedObjectName( va_location );
	      if ( pl_pt != NULL )
		{
		  strcpy( vl_nom, pl_pt );
		  vl_result = AfficheVueAdm( pa_info->widget, cg_TYP_SSS, vl_nom );
		}
	    }
	  break;
	default:
	  break;
	}
      break;

	/*A Le synoptique est du type SYNT */
	case cg_TYP_SYNT :
      		switch( vl_button )
		{
			/* Bt droit: ne rien faire (Popup menu) */
		case 3:   
	  	break;

		/* Bouton gauche: ouvrir le synoptique SSS 
		*	du sous_systeme clique */
		case 1:   
	  	vl_SelectedObject = 	
			TloGetSelectedObject( va_location );
	  	vl_type = VOobType( vl_SelectedObject );
	  	if ( vl_type == OT_SUBDRAWING)  
		{
	      	  strcpy( vl_nom, 
			  TloGetSelectedObjectName(va_location));

			/*A Affichage Sous_Systeme */
			GMAffich_SSSYT(
			   XtParent(pa_info->widget),vl_nom,
			   XDC_VRAI);

		}
		break;

		default:
	  	break;

		}

	break;


    /*
    |  Le synoptique est du type SSS
    */
    case cg_TYP_SSS:
      switch( vl_button )
	{
	case 3:   /* Bouton droit: ne rien faire (Popup menu) */
	  break;
	case 1:   /* Bouton gauche: ouvrir le synoptique TAC de la machine cliquee */
	  vl_SelectedObject = TloGetSelectedObject( va_location );
	  vl_type = VOobType( vl_SelectedObject );
	  if ( vl_type == OT_SUBDRAWING)  
	  {
	      vl_drawing = TviGetDrawing( VOsdViGet( vl_SelectedObject ) );
	      vl_vtext   = TdrGetNamedObject( vl_drawing, "SSS_NomMachine" );
	      strcpy( vl_nom, VOvtGetString( vl_vtext ) );
	      vl_result = AfficheVueAdm( pa_info->widget, cg_TYP_TAC, vl_nom );
	  }
	  break;
	default:
	  break;
	}
      break;

    /* Le synoptique est du type TAC */
    case cg_TYP_TAC:
      switch( vl_button )
	{
	case 3:   /* Bouton droit: ne rien faire (Popup menu) */
          break;
	case 1:   /* Bouton gauche: ne rien faire */ 
	  break;
        default:
	  break;
        }
	break;

    /* Le synoptique est du type TRA */
    case cg_TYP_TRA:
      break;

    /* Synoptique inconnu... */
    default:
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
           "gmot_adm_dv, GMhandlePicking: label %d inconnu (normalement egal au type du synoptique).", va_label );
      return XDC_ARG_INV;
      break;
    }

  return vl_result;
}


/*
|  Traduit l'evenement X en evenement DataViews (objet Location)
|  puis passe ce dernier au handler d'evenements DataViews.
*/
static BOOL GMhandleEvent( Widget va_widget, tm_SCR_INFO* pa_info, XEvent* pa_xevent )
{
  WINEVENT w_event;

  if (!(pa_info->view))
    return NO;

  if ( ! loc )
    loc = VOloCreate();

  GRwe_convert ( pa_xevent, &w_event );
  TloWinEventSetup ( loc, &w_event, pa_info->screen, pa_info->drawport );

  if ( VUerHandleLocEvent ( loc ) == INPUT_UNUSED)
    return YES;

  return NO;
}


/*
|  Appelle GMeventHandler sur reception d'un evenement
|  concernant la callback resource XmNinputCallback
*/
static void  GMinputCallback( Widget va_widget, tm_SCR_INFO* pa_info, caddr_t va_calldata )
{
  (void) GMhandleEvent ( va_widget, pa_info, (XEvent*) ((XmDrawingAreaCallbackStruct *)va_calldata)->event );
}


/*
|  Sur reception d'un evenement Resize, recalcule les dimensions de l'ecran
|  et redimensionne la vue et les objets graphiques a l'interieur.
*/
static void  GMresizeCallback( Widget va_widget, tm_SCR_INFO* pa_info, caddr_t va_calldata )
{
  if (!(pa_info->view))
    return;

  (void) TscSetCurrentScreen(pa_info->screen);
  (void) TscReset(pa_info->screen);
}


/*
|  Sur reception d'un evenement Expose,
|  redessine la region concernee de la vue
*/
static void  GMexposeCallback( 
	Widget va_widget, 
	tm_SCR_INFO* pa_info, 
	caddr_t va_calldata )
{
  WINEVENT w_event;

  if (!(pa_info->view))
    return;

  /* Get rid of the first Expose event (Great Bidouille comes back, yeah!). */
  if ( pa_info->MagicFlag == YES )
    {
      pa_info->MagicFlag = NO;
      return;
    }

  /* Redessine la partie de l'ecran nouvellement exposee. */
  GRwe_convert ((XEvent *)(((XmDrawingAreaCallbackStruct *)va_calldata)->event), &w_event);
  (void) TscSetCurrentScreen(pa_info->screen);
  (void) TdpRedraw(pa_info->drawport, &(w_event.region), YES);
}


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Les fonctions qui suivent sont appelees par 
* XZSA_01ListeMachine et XZSA_18ListeSite
* Elles servent a demander a la supervision l'etat
* initial des taches et sous-systemes
*
******************************************************
*/

/*
|  Demande a la supervision l'etat des taches 
|  de la machine dont le nom est passe en parametre
*/
static void GMInitVarTac( XZSCT_NomMachine      va_nom,
			  int                   va_nb,
			  tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  XZSE_22InitSynoptiqueTache( va_nom );
}

/*
|  Demande a la supervision l'etat des sous-systemes 
|  du site dont le nom est passe en parametre
*/
static void GMInitVarSSys( XZSCT_NomSite        va_nom,
			  int                   va_nb,
			  tm_SYMTAB_BUILD_ARGS *pa_Args )
{
  XZSE_21InitSynoptiqueSS( va_nom );
}

/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* ------------------------------------------
* FONCTION NON UTILISEE EN VERSION 1       |
* CAR LES SYNOPTIQUES SONT INITIALISES AU  |
* MOMENT OU ON DEMANDE LEUR AFFICHAGE      |
* ------------------------------------------
*
* cette fonction demande a la supervision d'initialiser
* tous les synoptiques:
*   - celui d'architecture generale;
*   - ceux de tache pour chaque machine existante;
*   - ceux de sous-systeme pour chaque site existant.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void InitAllSynop()

/*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{

  int   vl_cr;
  int   vl_dummy;
  void  *pl_dummy;


  /* initialisation du synoptique d'architecture generale */
  XZSE_20InitSynoptiqueAG();

  /* initialisation des synoptiques de tache */
  /*vl_cr = XZSA_01ListeMachine( GMInitVarTac, &vl_dummy, pl_dummy );
  if ( vl_cr != XDC_OK )
	XZST_03EcritureTrace( XZSTC_WARNING, 
		"InitAllSynop: pb dans XZSA_01ListeMachine" );

  vl_cr = XZSA_18ListeSite( GMInitVarSSys, &vl_dummy, pl_dummy );
  if ( vl_cr != XDC_OK )
	XZST_03EcritureTrace( XZSTC_WARNING, 
		"InitAllSynop: pb dans XZSA_18ListeSite" );*/
}



/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette  fonction renseigne les niveaux de trace 
* d'une tache sur une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void GetStateTraceTache( XZSCT_NomMachine	va_NomMac,
			XZSCT_NomTache		va_NomTache, 
			tg_EtatTache		*pa_EtatTache )
/*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  char	 vl_RacineVar[80];
  char	 vl_key[80];
  SYMNODE vl_node;
  int	*pl_valeur;

  sprintf( vl_RacineVar, "TRAV_%s_%s", va_NomMac, va_NomTache );

  /*---------- Niveau ECRAN ------------------------*/
  sprintf( vl_key, "%s_Ecr" , vl_RacineVar ); 

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );

  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->Ecr = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau FONCTION ---------------------*/
  sprintf( vl_key, "%s_FO" , vl_RacineVar ); 

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->FO = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau MESSAGE ----------------------*/
  sprintf( vl_key, "%s_ME" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->ME = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau INFO -------------------------*/
  sprintf( vl_key, "%s_IN" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->IN = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau SGBD ---------------------*/
  sprintf( vl_key, "%s_BD" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->BD = (*pl_valeur == 1 ) ? 1 : 0;


  /*---------- Niveau DEBUG1 ---------------------*/
  sprintf( vl_key, "%s_D1" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->D1 = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau DEBUG2 ---------------------*/
  sprintf( vl_key, "%s_D2" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->D2 = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau DEBUG3 ---------------------*/
  sprintf( vl_key, "%s_D3" , vl_RacineVar );

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->D3 = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau INTERFACE ---------------------*/
  sprintf( vl_key, "%s_INTER" , vl_RacineVar ); 

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->INTER = (*pl_valeur == 1 ) ? 1 : 0;

  /*---------- Niveau REGLE ---------------------*/
  sprintf( vl_key, "%s_REGLE" , vl_RacineVar ); 

  /* recuperation de la valeur de cette variable */ 
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  pl_valeur = VTsnvalue( vl_node );
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"GetStateTraceTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
     /* valorisation de pa_EtatTache */
     pa_EtatTache->REGLE = (*pl_valeur == 1 ) ? 1 : 0;
}




/* --------------------------------- */
/*  --------   2e partie  --------   */
/* --------------------------------- */


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_SSS (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmSsSys( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	     T_CB_ARG    	        pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{

#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomSSys ;
#else
  XDY_NomMachine vl_NomMac ;
  XDY_NomSysteme vl_NomSSys ;
#endif
 
  T_INT4        vl_EtatSSys;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZSCT_NomSite vl_NomSite;
  int		vl_cr;

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmSsSys : Pb lecture message XDM_ADM_SSS" );
    return;
  }

  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomSSys))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmSsSys : Pb lecture message XDM_ADM_SSS" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatSSys))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmSsSys : Pb lecture message XDM_ADM_SSS" );
    return;
  }

  /* site auquel appartient la machine vl_NomMac ?? */
  vl_cr = XZSA_23SiteMachine( vl_NomMac, vl_NomSite );
  if (vl_cr == XDC_NOK) return ; 

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "SSS_%s_%s_%s_etat", vl_NomSite, vl_NomMac, vl_NomSSys );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmSsSys: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );

  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmSsSys: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
  	switch (vl_EtatSSys)
  	{
	case XZSIC_OK:
			*pl_valeur = 2;
                        break;

	case XZSIC_NOK:
			*pl_valeur = 1;
                        break;

  	case XZSIC_SANS_ETAT:
			*pl_valeur = 0;
                        break;
  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmSsSys : Mauvais parametre d'entree " );
                        break;
  	}
  }
  /* met a jour les vues */
  RafraichitVuesAdm();

  return ;
}



/*X*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_TAC (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmTache( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	     T_CB_ARG    	        pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*  XDC_OK:  ok
*  XDC_NOK: probleme de lecture de message RT-Works
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{

 #ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomTache ;
  #else
  XDY_NomMachine vl_NomMac ;
  XDY_NomTache   vl_NomTache ;
  #endif
  
  T_INT4        vl_EtatTache;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmTache" );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTache : Pb lecture message XDM_ADM_TAC" );
    return;
  }

  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomTache))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTache : Pb lecture message XDM_ADM_TAC" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatTache))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTache : Pb lecture message XDM_ADM_TAC" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TAC_%s_%s_etat", vl_NomMac, vl_NomTache );

  XZST_03EcritureTrace( XZSTC_DEBUG1, 
	" Maj de la tache %s sur la machine %s \n", vl_NomTache,vl_NomMac);


  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTache: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );

  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTache: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {

  	switch (vl_EtatTache)
  	{
  	case XZSIC_OK:
			*pl_valeur = 2;
                        break;

  	case XZSIC_NOK:
			*pl_valeur = 1;
                        break;

  	case XZSIC_SANS_ETAT:
			*pl_valeur = 0;
                        break;
  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmTache : Mauvais parametre d'entree " );
                        break;
  	}
  }
  /* met a jour les vues */
  RafraichitVuesAdm();
  return;
}


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_SER (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmServeur( T_IPC_CONN                 va_Cnx,
	       T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	       T_CB_ARG        	          pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*  XDC_OK:  ok
*  XDC_NOK: probleme de lecture de message RT-Works
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
#else
  XDY_NomMachine vl_NomMac ;
#endif  

  T_INT4        vl_EtatMac;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;
  int		vl_TypeMaj_Synt 	= XZSIC_ETAT_NACTIF;




  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmServeur" );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmServeur : Pb lecture message XDM_ADM_ARC_SER" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmServeur : Pb lecture message XDM_ADM_ARC_SER" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomMac );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmServeur: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmServeur: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
  	switch (vl_EtatMac)
  	{
  	case XZSIC_ETAT_VEILLE:        
		*pl_valeur = 4;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

  	case XZSIC_ETAT_NACTIF:
		*pl_valeur = 3;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

  	case XZSIC_ETAT_NPASSIF:
		*pl_valeur = 2;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

  	case XZSIC_ETAT_DEGRADE:
		*pl_valeur = 1;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

  	case XZSIC_SANS_ETAT:
		*pl_valeur = 0;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmServeur : Mauvais parametre d'entree " );
                        break;
  	}
  }
	/*B On force a 0xffff */
	vs_MachSynt = 0xffff;

	/*B On force a 0xffff */
	vs_ResSynt = 0xffff;

  	/* met a jour les vues */
  	RafraichitVuesAdm();

  return;
}




/*X*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_PER (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmPeriph(T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	     T_CB_ARG    	        pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
#ifndef _TIBCO_EMS  
  T_STR         vl_NomPeriph ;  /* variables de lecture de message */
#else
  XDY_NomMachine vl_NomPeriph ;
#endif

  T_INT4        vl_EtatPeriph	= 0;
  T_INT4        vl_TypeEntite	= 0;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmPeriph" );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomPeriph))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmPeriph : Pb lecture message XDM_ADM_ARC_PER" );
    return;
  }

/* Modif V1.19 */
  if (!TipcMsgNextInt4(pa_Data->msg,&vl_TypeEntite))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmPeriph : Pb lecture message XDM_ADM_ARC_PER" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatPeriph))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmPeriph : Pb lecture message XDM_ADM_ARC_PER" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomPeriph );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmPeriph: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmPeriph: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {

  	switch (vl_EtatPeriph)
  	{
  	case XZSIC_OK:        
		*pl_valeur = 2;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Ent_Synt(vl_NomPeriph,vl_EtatPeriph,vl_TypeEntite);
                break;

  	case XZSIC_NOK:
		*pl_valeur = 1;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Ent_Synt(vl_NomPeriph,vl_EtatPeriph,vl_TypeEntite);
                 break;

  	case XZSIC_SANS_ETAT:
			*pl_valeur = 0;
                        break;
  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmPeriph : Mauvais parametre d'entree " );
                        break;
  	}
  }

	/*B On force a 0xffff */
	vs_MachSynt = 0xffff;

  /* met a jour les vues */
  RafraichitVuesAdm();
  return;
}



/*X*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_INT (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmInterf(T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	     T_CB_ARG    	        pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  #ifndef _TIBCO_EMS
  T_STR         vl_NomInterf ;  /* variables de lecture de message */
  #else
  XDY_Nom       vl_NomInterf ;
  #endif
  
  T_INT4        vl_EtatInterf;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmInterf" );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomInterf))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmInterf : Pb lecture message XDM_ADM_ARC_INT" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatInterf))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmInterf : Pb lecture message XDM_ADM_ARC_INT" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomInterf );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmInterf: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmInterf: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {

  	switch (vl_EtatInterf)
  	{
  	case XZSIC_OK:        
		*pl_valeur = 2;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Ent_Synt(vl_NomInterf,vl_EtatInterf,XZSAC_TYPE_INTE);

                break;

  	case XZSIC_NOK:
		*pl_valeur = 1;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Ent_Synt(vl_NomInterf,vl_EtatInterf,XZSAC_TYPE_INTE);
                break;

  	case XZSIC_SANS_ETAT:
			*pl_valeur = 0;
                        break;
  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmInterf : Mauvais parametre d'entree " );
                        break;
  	}
  }
  /* met a jour les vues */
  RafraichitVuesAdm();
  return;
}



/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_TRA (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmTrace( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	     T_CB_ARG    	        pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  #ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomTache ;
  #else
  XDY_NomMachine vl_NomMac ;
  XDY_NomTache   vl_NomTache ;
  #endif
  
  T_INT4        vl_NivTrace;
  T_INT4        vl_VisuTrace;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmTrace" );

  /* decodage du message */
  if (!TipcMsgNextInt4(pa_Data->msg,&vl_NivTrace))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTrace : Pb lecture message XDM_ADM_TRA" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_VisuTrace))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTrace : Pb lecture message XDM_ADM_TRA" );
    return;
  }

  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomTache))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTrace : Pb lecture message XDM_ADM_TRA" );
    return;
  }

  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmTrace : Pb lecture message XDM_ADM_TRA" );
    return;
  }

  /*---------- VISUALISATION -----------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_Ecr", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( vl_VisuTrace == XZSTC_ECRAN )
       *pl_valeur = 1;  /* visualisation ecran ON */
    else
       *pl_valeur = 0;  /* visualisation ecran OFF */
  }

  /*---------- Niveau FONCTION ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_FO", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_FONCTION) != 0 )
       *pl_valeur = 1;  /* Niveau FONCTION on */
    else
       *pl_valeur = 0;  /* Niveau FONCTION off */
  }

  /*---------- Niveau MESSAGE ----------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_ME", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_MESSAGE) != 0 )
       *pl_valeur = 1;  /* Niveau MESSAGE on */
    else
       *pl_valeur = 0;  /* Niveau MESSAGE off */
  }

  /*---------- Niveau INFO -------------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_IN", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_INFO) != 0 )
       *pl_valeur = 1;  /* Niveau INFO on */
    else
       *pl_valeur = 0;  /* Niveau INFO off */
  }

  /*---------- Niveau SGBD ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_BD", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_SGBD) != 0 )
       *pl_valeur = 1;  /* Niveau SGBD on */
    else
       *pl_valeur = 0;  /* Niveau SGBD off */
  }

  /*---------- Niveau DEBUG1 ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_D1", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_DEBUG1) != 0 )
       *pl_valeur = 1;  /* Niveau DEBUG1 on */
    else
       *pl_valeur = 0;  /* Niveau DEBUG1 off */
  }

  /*---------- Niveau DEBUG2 ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_D2", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_DEBUG2) != 0 )
       *pl_valeur = 1;  /* Niveau DEBUG2 on */
    else
       *pl_valeur = 0;  /* Niveau DEBUG2 off */
  }

  /*---------- Niveau DEBUG3 ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_D3", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_DEBUG3) != 0 )
       *pl_valeur = 1;  /* Niveau DEBUG3 on */
    else
       *pl_valeur = 0;  /* Niveau DEBUG3 off */
  }

  /*---------- Niveau INTERFACE ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_INTER", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_INTERFACE) != 0 )
       *pl_valeur = 1;  /* Niveau FONCTION on */
    else
       *pl_valeur = 0;  /* Niveau FONCTION off */
  }

  /*---------- Niveau REGLE ---------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_REGLE", vl_NomMac, vl_NomTache );

  /*  valorisation de cette variable avec la valeur recue */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable_T, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmTrace: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmTrace: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {
    if ( (vl_NivTrace & XZSTC_REGLE) != 0 )
       *pl_valeur = 1;  /* Niveau FONCTION on */
    else
       *pl_valeur = 0;  /* Niveau FONCTION off */
  }

  /* met a jour les vues */
  RafraichitVuesAdm();

  XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : MajAdmTrace" );

	return;

}	/* Fin MajAdmTrace */


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_STA_M (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmStatiM( T_IPC_CONN                 va_Cnx,
	      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	      T_CB_ARG        	         pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
#else
  XDY_NomMachine vl_NomMac ;
#endif  

  T_INT4        vl_ModeInstr;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmStatiM " );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmStatiM : Pb lecture message XDM_ADM_ARC_STA_M" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_ModeInstr))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmStatiM : Pb lecture message XDM_ADM_ARC_STA_M" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_modeI", vl_NomMac );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmStatiM: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmStatiM: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {

  	switch (vl_ModeInstr)
  	{
  	case XZSIC_MODE_INS:        
			*pl_valeur = 1;
                        break;

  	case XZSIC_MODE_NOR:
			*pl_valeur = 0;
                        break;
  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmStatiM : Mauvais parametre d'entree " );
                        break;
  	}
  }

  /* met a jour les vues */
  RafraichitVuesAdm();
  return;
}


/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_STA_E (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmStatiE( T_IPC_CONN                 va_Cnx,
	      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	      T_CB_ARG        	         pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
#else
  XDY_NomMachine vl_NomMac ;
#endif  

  T_INT4        vl_EtatStation;

  char          vl_key[50];  /* variables DV-Draw */
  SYMNODE       vl_node;
  int           *pl_valeur;
  int		vl_TypeMaj_Synt 	= XZSIC_ETAT_NACTIF;



  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmStatiE " );

  /* decodage du message */
  if (!TipcMsgNextStr(pa_Data->msg,&vl_NomMac))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmStatiE : Pb lecture message XDM_ADM_ARC_STA_E" );
    return;
  }

  if (!TipcMsgNextInt4(pa_Data->msg,&vl_EtatStation))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"MajAdmStatiE : Pb lecture message XDM_ADM_ARC_STA_E" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomMac );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdmStatiE: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmStatiE: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  else
  {

  	switch (vl_EtatStation)
  	{
  	case XZSIC_ETAT_VEILLE:        
		*pl_valeur = 4;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

  	case XZSIC_ETAT_NACTIF:
		*pl_valeur = 3;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

  	case XZSIC_ETAT_NPASSIF:
		*pl_valeur = 2;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

  	case XZSIC_ETAT_DEGRADE:
		*pl_valeur = 1;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

  	case XZSIC_SANS_ETAT:
		*pl_valeur = 0;

		/*A Mis a jour de la vue de Synthese */
		MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

  	default:
			XZST_03EcritureTrace( XZSTC_WARNING,
			         "MajAdmStatiE : Mauvais parametre d'entree " );
                        break;
  	}
  }

	/*B On force a 0xffff */
	vs_MachSynt = 0xffff;

	/*B On force a 0xffff */
	vs_ResSynt = 0xffff;

  /* met a jour les vues */
  RafraichitVuesAdm();

  return;
}


/*X*/
/* Fichier : $Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $		        Release : $Revision: 1.16 $              Date : $Date: 2020/11/03 17:48:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction s'execute a la reception du message
* XDM_ADM_RAF envoye periodiquement par le timer
* Elle effectue un rafraichissement de tous les 
* synoptiques d'administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
RafraichitAdm( 	T_IPC_CONN                 va_Cnx,
	      	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	      	T_CB_ARG        	   pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*  va_cnx :        connection avec RTServer
*  pa_Data :       acces aux donnees du msg
*  pa_Arg :        Prive
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  /* appelle la fonction qui rafraichit les synoptiques */
  RafraichitVuesAdm();
}


static int GMinitARC( VIEW va_view)
		       
{
  OBJECT                 vl_drawing;
  OBJECT                 vl_vtitle;
  tm_OTHER_TRAVERSE_ARGS vl_args;

  vl_drawing = TviGetDrawing( va_view );

  /* Positionne les couleurs de l'objet drawing */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
    }

  /* Traverse les objets de la vue et redirige les */
  /* pointeurs des descripteurs de variables dynamiques */
  /* (modifie les couleurs, au besoin). */
  vl_args.syn    = cg_TYP_ARC;
  vl_args.color  = vm_Config[cg_TYP_ARC - 1].color;
  vl_args.rebind = vm_Config[cg_TYP_ARC - 1].rebind;

  return ( (int) TdrForEachNamedObject( vl_drawing, GMtraverse, (char*)&vl_args ) );

}	/* Fin GMinitARC */




static int GMinitSYNT( VIEW va_view)
		       
{
  OBJECT                 vl_drawing;
  OBJECT                 vl_vtitle;
  tm_OTHER_TRAVERSE_ARGS vl_args;

  vl_drawing = TviGetDrawing( va_view );

  /* Positionne les couleurs de l'objet drawing */
  if ( vm_ColorsLoaded )
    {
      (void) VOdrForecolor( vl_drawing, vm_contexte );
      (void) VOdrBackcolor( vl_drawing, vm_fond );
    }

  /* Traverse les objets de la vue et redirige les */
  /* pointeurs des descripteurs de variables dynamiques */
  /* (modifie les couleurs, au besoin). */
  vl_args.syn    = cg_TYP_SYNT;
  vl_args.color  = vm_Config[cg_TYP_SYNT - 1].color;
  vl_args.rebind = vm_Config[cg_TYP_SYNT - 1].rebind;

  return ( (int) TdrForEachNamedObject( vl_drawing, GMtraverse, (char*)&vl_args ) );

}	/* Fin GMinitSYNT */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le synoptique de Synthese de Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajAdm_Mach_Synt(
	XZSCT_NomMachine	va_NomMachine,
	int 			va_EtatMach)

/*
* ARGUMENTS EN ENTREE :
*   va_EtatMach : Etat de la machine modifiee
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucune
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Calcul l'etat de synthese pour le mode degrade te 
*	le mode reseau
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajAdm_Mach_Synt " ;
	XZSCT_NomSite	vl_NomSite	= "";
		/* variables DV-Draw */
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_NbElt	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"MajAdm_Mach_Synt : IN \
		-- Nom Mach %s -- Etat mac %d \n",
		va_NomMachine, va_EtatMach);

	/*A Recuperation du Nom du Site associe a la
	*	machine */
	XZSA_23SiteMachine(va_NomMachine, vl_NomSite);

  	/* construction du nom de variable de synoptique */
  	sprintf( vl_key, "SYNT_%s_stat_mach", vl_NomSite );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdm_Mach_Synt: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
  {
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmServeur: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  }
  else
  {
	/*B Teste si au moins une machine degrade */
	if (va_EtatMach == XZSIC_ETAT_DEGRADE)
	{
		/*A MAJ etat de Synthes Machine */
		vs_MachSynt = 0;
	}
	else
	{
		/*B MAJ de l'etat de Synthese 
		* en parcourant etat de toutes 
		* les machines du Site */
		XZSA_19ListeMachineSite(vl_NomSite,
			   MajSynt_Mach_Synop,&vl_NbElt,NULL);

		/*B MAJ de l'etat de Synthese 
		* en parcourant etat de toutes 
		* les Peripheriques du Site */
		XZSA_29ListeEntiteSite(vl_NomSite,
			   MajSynt_Ent_Synop,&vl_NbElt,NULL);
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Maj Synthese cle %s -- MachSynt %d \n",
		vl_key,vs_MachSynt);


	/*A Affectation de la Valeur Synoptique */
	*pl_valeur = vs_MachSynt;
  }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"MajAdm_Mach_Synt : OUT \
	--cle %s -- Valeur SyntMAch %d \n",
		vl_key,*pl_valeur);

}	/* Fin MajAdm_Mach_Synt */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le synoptique de Synthese de Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajAdm_Ent_Synt(
	XZSCT_NomEntite		va_NomEntite,
	int 			va_EtatEnt,
	int			va_TypeEnt)

/*
* ARGUMENTS EN ENTREE :
*   va_EtatEnt : Etat de l'entite modifiee
*   va_TypeEnt : Type  de l'entite modifiee
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucune
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Calcul l'etat de synthese pour le mode degrade 
*	des entites
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajAdm_Ent_Synt " ;
	XZSCT_NomSite	vl_NomSite	= "";
		/* variables DV-Draw */
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_NbElt	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"MajAdm_Ent_Synt : IN \
		-- Nom Mach %s -- Etat ent %d \n",
		va_NomEntite, va_EtatEnt);

	/*A Recuperation du Nom du Site associe a la
	*       machine */
	XZSA_28SiteEntite(va_NomEntite, va_TypeEnt,vl_NomSite);

  	/* construction du nom de variable de synoptique */
  	sprintf( vl_key, "SYNT_%s_stat_mach", vl_NomSite );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdm_Mach_Synt: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
  {
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmServeur: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  }
  else
  {
	/*B Teste si au moins une machine degrade */
	if (va_EtatEnt == XZSIC_NOK) 
	{
		/*A MAJ etat de Synthes Machine */
		vs_MachSynt = 0;
	}
	else
	{
		/*B MAJ de l'etat de Synthese 
		* en parcourant etat de toutes 
		* les Peripheriques du Site */
		XZSA_29ListeEntiteSite(vl_NomSite,
			   MajSynt_Ent_Synop,&vl_NbElt,NULL);

		/*B MAJ de l'etat de Synthese 
		* en parcourant etat de toutes 
		* les machines du Site */
		XZSA_19ListeMachineSite(vl_NomSite,
			   MajSynt_Mach_Synop,&vl_NbElt,NULL);
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"Maj Synthese cle %s -- MachSynt %d \n",
		vl_key,vs_MachSynt);


	/*A Affectation de la Valeur Synoptique */
	*pl_valeur = vs_MachSynt;
  }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"MajAdm_Ent_Synt : OUT \
	--cle %s -- Valeur SyntMAch %d \n",
		vl_key,*pl_valeur);

}	/* Fin MajAdm_Ent_Synt */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le synoptique de Synthese de Reseau
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajAdm_Res_Synt(
	XZSCT_NomMachine	va_NomMachine,
	int 			va_EtatMach)

/*
* ARGUMENTS EN ENTREE :
*   va_EtatMach : Etat de la machine modifiee
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucune
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Calcul l'etat de synthese pour le mode degrade te 
*	le mode reseau
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajAdm_Res_Synt " ;
	XZSCT_NomSite	vl_NomSite	= "";
		/* variables DV-Draw */
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_NbElt	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"MajAdm_Res_Synt : IN \
		-- Nom Mach  %s -- Etat mac %d  \n",
		va_NomMachine, va_EtatMach);


	/*A Recuperation du Nom du Site associe a la
	*	machine */
	XZSA_23SiteMachine(va_NomMachine, vl_NomSite);

  	/* construction du nom de variable de synoptique */
  	sprintf( vl_key, "SYNT_%s_stat_res", vl_NomSite );

  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
  vl_node = (SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  if ( !vl_node )
    {
      /* Error */
      XZST_03EcritureTrace( XZSTC_WARNING,
	      "gmot_adm_dv, MajAdm_Synt: SYMNODE non trouve: la cle %s est invalide.", vl_key );
      return;
    }
  pl_valeur = VTsnvalue( vl_node );
  
  if ( pl_valeur == NULL )
  {
     XZST_03EcritureTrace (XZSTC_WARNING, 
		"MajAdmServeur: VTsnvalue a retourne NULL pour vl_key = %s", vl_key );
  }
  else
  {
	/*B Teste si au moins une machine degrade */
        /* Modif PN v1.5 */vs_ResSynt = XZSEC_ETAT_OK;
	if (va_EtatMach == XZSIC_SANS_ETAT)
	{
		/*A MAJ etat de Synthes Machine */
		vs_ResSynt = XZSIC_SANS_ETAT;
	}
	else
	{
		/*B MAJ de l'etat de Synthes en 
		* parcourant etat de toutes les 
		*	machines du Site */
		XZSA_19ListeMachineSite(vl_NomSite,
			MajSynt_Res_Synop,&vl_NbElt,NULL);
	}

	/*A Affectation de la Valeur Synoptique */
	*pl_valeur = vs_ResSynt;

   }
	XZST_03EcritureTrace(XZSTC_FONCTION,
	"MajAdm_Res_Synt : OUT --cle %s -- SyntRes %d \n",
		vl_key,*pl_valeur);

}	/* Fin MajAdm_Res_Synt */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de calculer l'etat de Synthese des Machines
*	du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajSynt_Mach_Synop(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	void             **pa_DonneesMsg)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine : Nom de la Machine
*   va_NumOrdre : Numero d'ordre
*   pa_DonneesMsg : Arguments a transmettre
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucne
*
* FONCTION 
*   Calcul l'etat de synthese dse machines du Site
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajSynt_Mach_Synop " ;
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_MASK		= 0x0007;



	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Mach_Synop  : IN -- machine %s \n",
		pa_NomMachine);

	sprintf( vl_key, "ARC_%s_etat", pa_NomMachine);

  	/*  valorisation de cette variable avec la valeur */
  	/*  recue (cette variable se situe dans la table */
  	/*  de symboles globale vm_DataSymTable ) */
  	vl_node =(SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  	if ( !vl_node )
    	{
      		/* Error */
      		XZST_03EcritureTrace( XZSTC_WARNING,
	      		"gmot_adm_dv, MajAdm_Synt: SYMNODE \
			non trouve: la cle %s est invalide.", 
			vl_key );
      		return;
    	}
  	pl_valeur = VTsnvalue( vl_node );

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Etat de Synthese %d -- %d \n",
		*pl_valeur,vs_MachSynt);

	/*A Memorisation de l'etat de Synthese */
	/*? On effectue un & logique avec la valeur recupere 
	* de la machine pour obtenir soit 0 si une machine 
	* est en Mode degrade (valeur = 1) ou superieur a
	* 2 si Actif,Passif ou Veille (valeur = 3,2 et 1) */

	/*B On calcule l'etat de Synthese , seulement si
	*	l'etat de la machine est different de 
	*	SANS_ETAT */
	if (*pl_valeur != XZSIC_SANS_ETAT)
	{
		if (*pl_valeur == 1)
		{
			/*A Mode Degrade */
			vs_MachSynt = 0;
		}
		else
		{
			/* Mode Normal */
			vs_MachSynt = vs_MachSynt & 
				( (*pl_valeur) | vl_MASK) ;
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Mach_Synop  : OUT  --  Valeur %d \n",
		vs_MachSynt);

}	/* Fin MajSynt_Mach_Synop */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de calculer l'etat de Synthese des Entites
*	du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajSynt_Ent_Synop(
	XZSCT_NomEntite pa_NomEntite,
	int              va_NumOrdre,
	void             **pa_DonneesMsg)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomEntite : Nom de la Entite
*   va_NumOrdre : Numero d'ordre
*   pa_DonneesMsg : Arguments a transmettre
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucne
*
* FONCTION 
*   Calcul l'etat de synthese dse machines du Site
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajSynt_Mach_Synop " ;
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_MASK		= 0x0007;



	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Ent_Synop  : IN -- Entite %s \n",
		pa_NomEntite);

	sprintf( vl_key, "ARC_%s_etat", pa_NomEntite);

  	/*  valorisation de cette variable avec la valeur */
  	/*  recue (cette variable se situe dans la table */
  	/*  de symboles globale vm_DataSymTable ) */
  	vl_node =(SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  	if ( !vl_node )
    	{
      		/* Error */
      		XZST_03EcritureTrace( XZSTC_WARNING,
	      		"gmot_adm_dv, MajAdm_Synt: SYMNODE \
			non trouve: la cle %s est invalide.", 
			vl_key );
      		return;
    	}
  	pl_valeur = VTsnvalue( vl_node );

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Etat de Synthese %d -- %d \n",
		*pl_valeur,vs_MachSynt);

	/*A Memorisation de l'etat de Synthese */
	/*? On effectue un & logique avec la valeur recupere 
	* de la machine pour obtenir soit 0 si une machine 
	* est en Mode degrade (valeur = 1) ou superieur a
	* 2 si Actif,Passif ou Veille (valeur = 3,2 et 1) */

	/*B On calcule l'etat de Synthese , seulement si
	*	l'etat de la machine est different de 
	*	SANS_ETAT */
	if (*pl_valeur != XZSIC_SANS_ETAT)
	{
		if (*pl_valeur == 1)
		{
			/*A Mode Degrade */
			vs_MachSynt = 0;
		}
		else
		{
			/* Mode Normal */
			vs_MachSynt = vs_MachSynt & 
				( (*pl_valeur) | vl_MASK) ;
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Ent_Synop  : OUT  --  Valeur %d \n",
		vs_MachSynt);

}	/* Fin MajSynt_Ent_Synop */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de calculer l'etat de Synthese des Machines
*	du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajSynt_Res_Synop(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	void             **pa_DonneesMsg)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine : Nom de la Machine
*   va_NumOrdre : Numero d'ordre
*   pa_DonneesMsg : Arguments a transmettre
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucne
*
* FONCTION 
*   Calcul l'etat de synthese dse machines du Site
*
------------------------------------------------------*/
{
	static char *version="$Id: gmot_adm_dv.c,v 1.16 2020/11/03 17:48:40 pc2dpdy Exp $ : MajSynt_Res_Synop " ;
  	char          vl_key[50]	= "";  
  	SYMNODE       vl_node;
  	int           *pl_valeur	= NULL;
	int		vl_MASK		= 0x0007;



	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Res_Synop  : IN -- machine %s \n",
		pa_NomMachine);

	sprintf( vl_key, "ARC_%s_etat", pa_NomMachine);

  	/*  valorisation de cette variable avec la valeur */
  	/*  recue (cette variable se situe dans la table */
  	/*  de symboles globale vm_DataSymTable ) */
  	vl_node =(SYMNODE) VTstkeyfind( vm_DataSymTable, vl_key );
  	if ( !vl_node )
    	{
      		/* Error */
      		XZST_03EcritureTrace( XZSTC_WARNING,
	      		"gmot_adm_dv,  SYMNODE \
			non trouve: la cle %s est invalide.", 
			vl_key );
      		return;
    	}
  	pl_valeur = VTsnvalue( vl_node );

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		" Etat de Synthese %d -- %d \n",
		*pl_valeur,vs_ResSynt);

	/*A Mode Degrade */
	vs_ResSynt = vs_ResSynt * ((*pl_valeur == 0 ) ? 0 : 1); /* 1.6 */

	XZST_03EcritureTrace(XZSTC_FONCTION,
		" MajSynt_Res_Synop  : OUT  --  Valeur %d \n",
		vs_ResSynt);

}	/* Fin MajSynt_Res_Synop */

