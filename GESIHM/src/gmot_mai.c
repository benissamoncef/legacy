/*E*/
/* Fichier : @(#)gmot_mai.c	1.4		Release : 1.4              Date : 07/31/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mai * FICHIER gmot_mai.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient le main de gmot dans lequel
* on initialise l'environnement MOTIF, RT-Works,
* l'administration systeme, ...
* le main contient la boucle d'attente d'evenements
* MOTIF a laquelle on a ajoute la reception de 
* messages RT-Works
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  14 Nov 94	: Creation
* T.Milleville  15 Mar 95	: Ajout fonctionnalite d'impression
*	du BL V1.2
* T.Milleville  16 Mar 95	: Ajout fonctionnalite d'impression
*	du VL V1.2
* T.Milleville  03 Avr 95	: Modification de la boucle de
*	recuperation des msg RTworks V1.3
* P.Niepceron	25 Jan 95	: Ajout de synchro_disques_cb ds le tableau reglist v1.5
* P.Niepceron   30 Jan 96	: deplacement de la decl de XZSEV_NomSDActif en dehors du main v1.6
* P.Niepceron   13 Fev 96	: Arret et relance de tigmot pour la prise en compte des fic de adm v1.7
* P.Niepceron   04 Mar 96	: Ajout bouton pour lancement tsconf v1.8
				  Ajout svg manuelle (577) 
				  Ajout vidage des bilan (576) v1.8
* P.Niepceron   28 Aou 96	:  Recuperation NomSite et Type MAchine v1.10
* JPL		19/11/10	: Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.11
*******************************************************/


/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/

/* fichiers inclus */
    
#include "gmot_ser.h"
#include "gmot_var.h"

#include "gmot_mai.h"
#include "gmot_adm.h"
#include "gmot_adm_dv.h"

/* definitions de constantes */
/* definitions de types locaux */

typedef struct {
		  int NumMsg;      /* Numero message RTW */
		  void (*FoncCB)(); /* Fonction callback */
} tm_callback;

/* definition de macro locales */

/* definition de variables locales */

tm_callback pm_TabCB[] = {
			   { XDM_ADM_SSS, (void (*)())MajAdmSsSys },
			   { XDM_ADM_TAC, (void (*)())MajAdmTache },
			   { XDM_ADM_ARC_SER, (void (*)())MajAdmServeur },
			   { XDM_ADM_ARC_PER, (void (*)())MajAdmPeriph },
			   { XDM_ADM_ARC_INT, (void (*)())MajAdmInterf },
			   { XDM_ADM_TRA, (void (*)())MajAdmTrace },
			   { XDM_ADM_ARC_STA_M, (void (*)())MajAdmStatiM },
			   { XDM_ADM_ARC_STA_E, (void (*)())MajAdmStatiE },
			   { XDM_ADM_RAF, (void (*)())RafraichitAdm },
			   { XDM_ADM_REINIT, (void (*)())InitAdm_cb }
};

static MRMRegisterArg reglist[] = {
  {"lija_qui_cb",(XtPointer) lija_qui_cb},
  {"livl_fil_cb",(XtPointer) livl_fil_cb},
  {"livl_qui_cb",(XtPointer) livl_qui_cb},
  {"livl_imp_cb",(XtPointer) livl_imp_cb},
  {"bderr_qui_cb",(XtPointer) bderr_qui_cb},
  {"bdaca_qui_cb",(XtPointer) bdaca_qui_cb},
  {"bdaca_val_cb",(XtPointer) bdaca_val_cb},
  {"bdbsd_qui_cb",(XtPointer) bdbsd_qui_cb},
  {"bdbsd_val_cb",(XtPointer) bdbsd_val_cb},
  {"bdbsd_to_chg_cb",(XtPointer) bdbsd_to_chg_cb},
  {"bdbsc_val_cb",(XtPointer) bdbsc_val_cb},
  {"bdbsc_qui_cb",(XtPointer) bdbsc_qui_cb},
  {"bdbsc_to_chg_cb",(XtPointer) bdbsc_to_chg_cb},
  {"bdass_li_mac_cb",(XtPointer) bdass_li_mac_cb},
  {"bdass_val_cb",(XtPointer) bdass_val_cb},
  {"bdass_qui_cb",(XtPointer) bdass_qui_cb},
  {"grtra_qui_cb",(XtPointer) grtra_qui_cb},
  {"grtra_val_cb",(XtPointer) grtra_val_cb},
  {"grtra_selec_cb",(XtPointer) grtra_selec_cb},
  {"bdsta_qui_cb",(XtPointer) bdsta_qui_cb},
  {"bdsta_val_cb",(XtPointer) bdsta_val_cb},
  {"bdsit_qui_cb",(XtPointer) bdsit_qui_cb},
  {"bdsit_val_cb",(XtPointer) bdsit_val_cb},
  {"bdata_li_mac_cb",(XtPointer) bdata_li_mac_cb},
  {"bdata_val_cb",(XtPointer) bdata_val_cb},
  {"bdata_qui_cb",(XtPointer) bdata_qui_cb},
  {"bdasu_val_cb",(XtPointer) bdasu_val_cb},
  {"bdasu_qui_cb",(XtPointer) bdasu_qui_cb},
  {"menu2_sss_cb",(XtPointer) menu2_sss_cb},
  {"menu2_sta_cb",(XtPointer) menu2_sta_cb},
  {"menu2_sar_cb",(XtPointer) menu2_sar_cb},
  {"menu2_bsc_cb",(XtPointer) menu2_bsc_cb},
  {"menu2_ars_cb",(XtPointer) menu2_ars_cb},
  {"menu2_aca_cb",(XtPointer) menu2_aca_cb},
  {"menu2_ass_cb",(XtPointer) menu2_ass_cb},
  {"menu2_ata_cb",(XtPointer) menu2_ata_cb},
  {"menu2_ico_cb",(XtPointer) menu2_ico_cb},
  {"menu2_ijo_cb",(XtPointer) menu2_ijo_cb},
  {"menu2_gdt_cb",(XtPointer) menu2_gdt_cb},
  {"menu2_cvl_cb",(XtPointer) menu2_cvl_cb},
  {"menu2_arc_cb",(XtPointer) menu2_arc_cb},
  {"menu2_sav_cb",(XtPointer) menu2_sav_cb},
  {"menu2_uni_cb",(XtPointer) menu2_uni_cb},
  {"menu2_aid_cb",(XtPointer) menu2_aid_cb},
  {"menu2_jas_cb",(XtPointer) menu2_jas_cb},
  {"menu2_qui_cb",(XtPointer) menu2_qui_cb},
  {"sauve_a_verif_cb",(XtPointer) sauve_a_verif_cb},
  {"restore_a_verif_cb",(XtPointer) restore_a_verif_cb},
  {"verif_reseau_cb",(XtPointer) verif_reseau_cb},
  {"verif_equip_cb",(XtPointer) verif_equip_cb},
  {"verif_SAIDEC_cb",(XtPointer) verif_SAIDEC_cb},
  {"verif_globale_cb",(XtPointer) verif_globale_cb},
  {"verif_defaut_cb",(XtPointer) verif_defaut_cb},
  {"verif_seuil_tf_cb",(XtPointer) verif_seuil_tf_cb},
  {"verif_equ_al_cb",(XtPointer) verif_equ_al_cb},
  {"modif_reseau_cb",(XtPointer) modif_reseau_cb},
  {"modif_equip_cb",(XtPointer) modif_equip_cb},
  {"lecture_bc_cb",(XtPointer) lecture_bc_cb},
  {"lecture_bl_cb",(XtPointer) lecture_bl_cb},
  {"livrer_cb",(XtPointer) livrer_cb},
  {"diffuser_bl_cb",(XtPointer) diffuser_bl_cb},
  {"tests_1_cb",(XtPointer) tests_1_cb},
  {"tests_2_cb",(XtPointer) tests_2_cb},
  {"tests_3_cb",(XtPointer) tests_3_cb},
  {"tests_4_cb",(XtPointer) tests_4_cb},
  {"sauve_archiv_cb",(XtPointer) sauve_archiv_cb},
  {"restaure_archiv_cb",(XtPointer) restaure_archiv_cb},
  {"lecture_catalogue_cb",(XtPointer) lecture_catalogue_cb},
  {"configurer_equt_cb",(XtPointer) configurer_equt_cb},
  {"list_bc_qui_cb",(XtPointer) list_bc_qui_cb},
  {"list_bl_qui_cb",(XtPointer) list_bl_qui_cb},
  {"list_bl_imp_cb",(XtPointer) list_bl_imp_cb},
  {"list_arc_qui_cb",(XtPointer) list_arc_qui_cb},
  {"list_brbh_qui_cb",(XtPointer) list_brbh_qui_cb},
  {"list_brbh_vider_cb",(XtPointer) list_brbh_vider_cb},
  {"list_bsbh_vider_cb",(XtPointer) list_bsbh_vider_cb},
  {"list_hebdo_vider_cb",(XtPointer) list_hebdo_vider_cb},
  {"list_BC_vider_cb",(XtPointer) list_BC_vider_cb},
  {"list_bsbh_qui_cb",(XtPointer) list_bsbh_qui_cb},
  {"lecture_brbh_cb",(XtPointer) lecture_brbh_cb},
  {"lecture_bsbh_cb",(XtPointer) lecture_bsbh_cb},
  {"list_hebdo_qui_cb",(XtPointer) list_hebdo_qui_cb},
  {"sauvegarde_manuel_cb",(XtPointer) sauvegarde_manuel_cb},
  {"sauvegarde_hebdo_cb",(XtPointer) sauvegarde_hebdo_cb},
  {"lecture_hebdo_cb",(XtPointer) lecture_hebdo_cb},
  {"synchro_disques_cb",(XtPointer) synchro_disques_cb},
  {"menu_tsconf",(XtPointer) menu_tsconf},
};

static int reglist_num = (sizeof(reglist)/sizeof( MRMRegisterArg));
static char *version = "@(#)gmot_mai.c	1.4 07/31/95 : gmot_mai.c" ;

void fonc_sortie_gmot( );

extern  XZSCT_NomMachine        XZSEV_NomSDActif;


/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet sur reception du message XDM_ADM_REINIT de reinitialiser
*	les variables dynamiques necessaires a Dataviews
*	pour construire ses synoptiques
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void InitAdm_cb(
	T_IPC_CONN                 va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG                   pa_Arg )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Auncun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*	Libere les ressoures Dataviews et reinitialise
*	les donnees dynamiques
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mai.c	1.4 07/31/95 : InitAdm_cb " ;
	int	i = 0;
	XZSCT_NomMachine   vl_machine="";
	char 	vm_cmd[80]="";

	XZST_03EcritureTrace(XZSTC_FONCTION,"InitAdm_cb : IN \n");

	system("echo 'sleep 2;tigmot.x migrazur' | cat >/tmp/tigmot.sh");
	system("chmod +x /tmp/tigmot.sh");

	system("nohup /tmp/tigmot.sh >>/dev/null 2>>/dev/null &");

	
	exit(0);


	/* Fermeture hierarchie Mrm */
	MrmCloseHierarchy( vg_mrm_hierarchy );

	/* destruction du xterm console */
	/*kill( vg_pid_term, SIGTERM );*/

	/* on detruit les menus MENU */
	for ( i=0; i< vg_NumScreens; i++ )
	{
		XtDestroyWidget( vg_MENU[i].w_menu_fen);
	}

	/* liberation des ressources Dataviews */
	FreeVuesAdm();

	/* liberation des ressources d'administration  */
	XZSA_21LibereRessourcesAdmin(); 

	/* desabonnement des Datagroupes */
	XDG_QuitterDG( XDG_IHM_ADM );
	/*XZSC_07NomMachine(vl_NomMachine);
	XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomMachine);
	XDG_QuitterDG( vl_datagroup);
	XZSC_04NomSite(vl_NomSite);
	XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomSite);
	XDG_QuitterDG( vl_datagroup);*/


	/*A Liberation des donnes dataviews */
/*	FreeVuesAdm();
*/
	/* initialisation Dataviews */
	/*(void) TInit( (char *)NULL, (char *)NULL );*/

	/*A Reinitialisation des donnees dynamiques */
/*	if ( InitAdm(XDC_FAUX) != XDC_OK)*/
	if ( InitAdm(XDC_FAUX) != XDC_OK)
	{
   		XZST_03EcritureTrace( XZSTC_WARNING, 
			"Pb a l'init des synoptiques" );
	}

	/* on detruit les menus MENU */
/*	for ( i=0; i< vg_NumScreens; i++ )
	{
		XtUnmanageChild( vg_GRSYN_FM[i]);
	}*/


	XZST_03EcritureTrace(XZSTC_FONCTION,"InitAdm_cb : OUT \n");
	
}	/* Fin InitAdm_cb */




/*X*/
/* Fichier : @(#)gmot_mai.c	1.4		Release : 1.4              Date : 07/31/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE smot_mai
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction appelee sur reception d'un evenement 
* RTWorks (reception de message)
*
******************************************************
* SEQUENCE D'APPEL :
*/

my_xt_func( XtPointer client_data, int *source, XtInputId *id )

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
  Widget		w;
  XtPointer		cli_data;
  XtPointer		call_data;
  T_INT4		vl_ErrNum;
  T_IPC_MSG		vl_msg;


  /*A passage en Mode Blockant */
  TipcSrvSetBlockMode(TRUE);

  while ( (vl_msg = TipcSrvMsgNext( 0.0 )) != NULL )
  {
    if ( !TipcSrvMsgProcess( vl_msg ) )
       	XZST_03EcritureTrace (XZSTC_WARNING, 
			"gmot_main: pb dans TipcSrvMsgNext" );

    if ( !TipcMsgDestroy( vl_msg ) )
	XZST_03EcritureTrace (XZSTC_WARNING, 
			"gmot_main: pb dans TipcMsgDestroy" );
  }

  /* erreur de connexion */
  XZST_03EcritureTrace(XZSTC_DEBUG1,"Recuperation de l'erreur RTWorks \n");

  vl_ErrNum = TutErrNumGet();

  if (vl_ErrNum > T_ERR_IPC_START )
  {
    /* on deconnecte les IT RT-Works */
    XtRemoveInput(*id);

    XZST_03EcritureTrace (XZSTC_WARNING, 
	"gmot se tue sur erreur RT-Works num %d ", vl_ErrNum );

	/*A Teste si l'erreur est 501  */
	/*if (vl_ErrNum == 501)
	{
		fonc_sortie_gmot();	
	}*/
  }

  TipcSrvFlush();

}	/* Fin my_xt_func */





/* definition de fonctions externes */

/* declaration de variables locales */

XtAppContext    vl_app_context;
Display 	*pl_Display;
Screen          *pl_ScreenSet;


/*X*/
/* Fichier : @(#)gmot_mai.c	1.4		Release : 1.4              Date : 07/31/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE smot_mai
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* main du module gmot (tache tgmoti)
* - initialisation de la tache et connexion au RTServer
* - initialisations Mrm et Motif
* - recuperation du contexte, display
* - ouverture de la hierarchie Mrm francaise 
* - initialisation de la supervision
* - abonnement au Datagroup XDG_IHM_ADM, XDG_IHM_ADM_Site
*   et XDG_IHM_ADM_Machine
* - initialisation des synoptiques d'administration
* - creation et affichage du menu principal MENU
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int main (int argc, char **argv)

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
T_INT4    			vl_source;
T_BOOL    			vl_cr;
int       			vl_nCB;
int       			vl_numMsg;
pid_t     			vl_pid;
T_IPC_MT			vl_mt;
int             		vl_iter;
XZSCT_Datagroup      		vl_datagroup;
XZSCT_NomMachine     		vl_NomMachine;
XZSCT_NomSite        		vl_NomSite;



vg_mrm_open_param = (MrmOsOpenParamPtr)XtMalloc(sizeof(MrmOsOpenParam) );

if ( ( argc == 3 ) && ( strcmp ( argv[2], "no" ) == 0 ) )
{
   printf( "sans synchro...\n" );
   /* on force le serveur actif */
   strcpy(XZSEV_NomSDActif,"SDCI1");
   if (XZSC_01InitCnxSup(  argv[1],
			   argv[0],
			   XZSCC_INTERDIRE,
			   XZSCC_AUTORISER,
			   XZSCC_INTERDIRE,
			   fonc_sortie_gmot,
			   "/dev/null" ) == XDC_NOK )
   {
     XZST_03EcritureTrace( XZSTC_FATAL, "main: pb a la connexion au RTServer ");
   }
}
else
{
   printf( "avec synchro...\n" );
   /* initialisation de la tache et connexion au RTServer */
   if (XZSC_01InitCnxSup(  	argv[1], 
				argv[0], 
				XZSCC_INTERDIRE, 
				XZSCC_AUTORISER, 
				XZSCC_INTERDIRE, 
				fonc_sortie_gmot, 
				"/dev/null" ) == XDC_NOK ) 
   {
     XZST_03EcritureTrace( XZSTC_FATAL, "main: pb a la connexion au RTServer ");
   }
}
  (void) XZSC_05TypeMachine(&vg_TypeMachine);
  (void) XZSC_04NomSite(vg_NomSite);

/* initialisation des callbacks de la tache */
for ( vl_nCB = 0; vl_nCB < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_nCB++ )
{
   vl_mt = XDM_IdentMsg( pm_TabCB[vl_nCB].NumMsg );
   if ( vl_mt == NULL ) 
      XZST_03EcritureTrace( XZSTC_WARNING,
          "main: callback arme sur message numero %d global", 
	  pm_TabCB[vl_nCB].NumMsg );

   if (!TipcSrvProcessCbCreate( vl_mt,
			        pm_TabCB[vl_nCB].FoncCB,
			        (T_CB_ARG*)NULL) )
   {
     XZST_03EcritureTrace( XZSTC_FATAL, 
		"main de gmot: pb au TipcSrvProcessCbCreate");
   }
}

/* initialisation Mrm    */
MrmInitialize();

/* initialisation Motif  */
XtToolkitInitialize ();

vl_app_context = XtCreateApplicationContext ();

/* ouverture du display  */
pl_Display = XtOpenDisplay (	vl_app_context, 
		     		NULL, 
		     		argv[0], 
		     		"XMigrazur", 
		     		NULL, 
		     		0, 
		     		&argc, 
		     		argv);

/* probleme d'ouverture ... */
if (!pl_Display)
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: ouverture du display impossible");

vg_mrm_open_param->version = MrmOsOpenParamVersion;
vg_mrm_open_param->display = pl_Display;

/* ouverture hierachie francaise */
ouvre_hierarchie( cg_FLAG_OUVERT );

if ( MrmRegisterNames ( reglist, reglist_num ) != MrmSUCCESS )
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: pb a l'appel de MrmRegisterNames");

/* s'il y a erreur d'execution, on sort ... */
if (!TipcSrvGetXtSource( &vl_source ))
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: erreur au TipcSrvGetXtSource");

/* initialisation de la supervision */
/*if ( XZSA_20InitAdmin() != XDC_OK )
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: pb a l'appel de XZSA_20InitAdmin" );*/

/* abonnement aux Datagroupes XDG_IHM_ADM */ 
if (!XDG_JoindreDG( XDG_IHM_ADM))
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: impossible de s'abonner a XDG_IHM_ADM" );

/* abonnement au Datagroup XDG_IHM_ADM-Machine */
/*XZSC_07NomMachine(vl_NomMachine);
XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomMachine);
if (!XDG_JoindreDG(vl_datagroup))
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: impossible de s'abonner a XDG_IHM_ADM-%s", vl_NomMachine);*/
     
/* abonnement au Datagroup XDG_IHM_ADM-Site */
/*XZSC_04NomSite(vl_NomSite);
XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomSite);
if (!XDG_JoindreDG(vl_datagroup))
   XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: impossible de s'abonner a XDG_IHM_ADM-%s", vl_NomSite);*/

/* initialisation Dataviews */
(void) TInit( (char *)NULL, (char *)NULL );

/* initialisation des synoptiques d'administration */
if ( InitAdm(XDC_VRAI) != XDC_OK)
   XZST_03EcritureTrace( XZSTC_WARNING, "main de gmot: pb a l'init des synoptiques" );
	    
/* ajout des receptions d'evenements RTWorks a la boucle */
/* principale d'attente Motif                            */
XtAppAddInput(  vl_app_context, 
		vl_source, 
		(XtPointer) XtInputReadMask,
	        (XtInputCallbackProc) my_xt_func, 
		NULL );

/* calcul du nombre d'ecrans disponibles sur la station */
vg_NumScreens = ScreenCount( pl_Display );
pl_ScreenSet = ScreenOfDisplay( pl_Display, 0 );

/* -----------------------------------------------------------*/
/* --------- boucle de creation d'une arborescence ---------- */
/* --------- pour chaque ecran du display          ---------- */
/* creation de l'objet MENU, menu principal MIGRAZUR en francais */
/* -----------------------------------------------------------*/
for( vl_iter=0; vl_iter < vg_NumScreens; vl_iter++ )
{
     vg_PtrSurMenuACreer = (tg_InfosMenu *) calloc (1, (unsigned int) sizeof (tg_InfosMenu));

     create_MENU( pl_Display, 
		   &pl_ScreenSet[vl_iter], 
		   argv[0], 
		   argc, 
		   argv , 
		   vg_mrm_hierarchy, 
		   &vg_class );

     vg_MENU[vl_iter] = *vg_PtrSurMenuACreer;

     free( vg_PtrSurMenuACreer );


     XtRealizeWidget ( vg_MENU[vl_iter].w_menu_fen );

	/*A Creer ou Manager le Synoptique de Synthese */
	cree_ou_manage_GRMACH_SYNT(vg_MENU[vl_iter].w_menu_fen,vl_iter);
}


/* boucle principale d'attente d'evenements  */
XtAppMainLoop (vl_app_context);

exit (0);
 
}


/*X*/
/* Fichier : @(#)gmot_div.c     2.36      Release : 2.36        Date : 21 Sep
1994
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_div
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* - liberation des ressources Dataviews
* - desabonnement des Datagroupes
* - liberation des ressources d'administration (XZSA_21)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void fonc_sortie_gmot( )

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
******************************************************/
{
	int                  i;
	XZSCT_NomMachine     vl_NomMachine;
	XZSCT_NomSite        vl_NomSite;
	XZSCT_Datagroup      vl_datagroup;

	XZST_03EcritureTrace (XZSTC_FONCTION , 
			"fonc_sortie_gmot : IN \n");

	XZST_03EcritureTrace (XZSTC_WARNING , 
			"fonc_sortie_gmot: SORTIE GMOT.X ");

	/* Fermeture hierarchie Mrm */
	MrmCloseHierarchy( vg_mrm_hierarchy );

	/* destruction du xterm console */
	/*kill( vg_pid_term, SIGTERM );*/

	/* on detruit les menus MENU */
	for ( i=0; i< vg_NumScreens; i++ )
	{
		XtDestroyWidget( vg_MENU[i].w_menu_fen);
	}

	/* liberation des ressources Dataviews */
	FreeVuesAdm();

	/* liberation des ressources d'administration  */
	XZSA_21LibereRessourcesAdmin(); 

	/* desabonnement des Datagroupes */
	XDG_QuitterDG( XDG_IHM_ADM );
	/*XZSC_07NomMachine(vl_NomMachine);
	XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomMachine);
	XDG_QuitterDG( vl_datagroup);
	XZSC_04NomSite(vl_NomSite);
	XDG_EncodeDG2(  vl_datagroup,  XDG_IHM_ADM,  vl_NomSite);
	XDG_QuitterDG( vl_datagroup);*/


	/* Fermeture du fichier de traces */
	XZST_02FermetureTrace();

	XZST_03EcritureTrace (XZSTC_FONCTION , 
			"fonc_sortie_gmot : OUT \n");

	exit(0);

}	/* Fin fonc_sortie_gmot */
