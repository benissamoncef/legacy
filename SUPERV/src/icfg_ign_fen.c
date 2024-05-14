
/* fichiers inclus */
#include <stdio.h>
#include "icfg_ign_fen.h"    

OBJECT vm_screen;
DRAWPORT vm_drawport_en_cours;

/* definition de macro locales */
#define         cm_MAX_SCREENS	5
#define cm_DEFAULT_CLUT    (char *) NULL
#define cm_DEFAULT_SCR_VP  (RECTANGLE *) NULL
#define cg_NB_HIERARCHIES  1



/* definition de variables locales */

typedef struct {
  DRAWPORT drawport;
    VIEW     view;
    } tm_HELP_INFO;


typedef struct {
  BOOL         MagicFlag;  /* Pour ignorer l'embarassant tout 1er Expose event.
  */
  OBJECT       screen;
  DRAWPORT     drawport;
  VIEW         view;
  Widget       widget;
  int   	type;
  tm_HELP_INFO help;
} tm_SCR_INFO;


extern void quit_cb (Widget, XtPointer, XtPointer );
static MRMRegisterArg reglist[] = { 
	{"quit_cb", (XtPointer)quit_cb } };

static int reglist_num = (sizeof(reglist)/sizeof( MRMRegisterArg));

static Widget	vs_GRMACH_SYNT_FM[]={NULL, NULL, NULL ,
					NULL, NULL, NULL};
static Widget	vs_GRMACH_SYNT_DA[]={NULL, NULL, NULL ,
					NULL, NULL, NULL};


/* declaration de fonctions internes */
int GMinitSyn(tm_SCR_INFO   *pa_info);

void create_MENU (
	Display         *display,
	Screen          *screen,
	char            *app_name,
	int             app_argc,
	char            **app_argv,
	MrmHierarchy    hierarchy_id,
	MrmCode         *mrm_class);

int ouvre_hierarchie     ( int  va_FlagClose);

int cree_ou_manage_GRMACH_SYNT(Widget w,int p_num_screen,VIEW va_view);

void GMeraseScreen();

int AfficheVueAdm(
	     	Widget           va_widget,
		char*            pa_name,
		VIEW 		 va_view);

void create_GRMACH_SYNT (
	Widget parent,
	MrmHierarchy hierarchy_id,
	MrmCode *mrm_class);

BOOL  GMhandleEvent( Widget, tm_SCR_INFO*, XEvent* );
void  GMinputCallback( Widget, tm_SCR_INFO*, caddr_t );
void  GMresizeCallback( Widget, tm_SCR_INFO*, caddr_t );
void  GMexposeCallback( Widget, tm_SCR_INFO*, caddr_t );

void GRSYN_exposeCb(
	Widget w,
	XtPointer client_data, 
	XtPointer xt_call_data);


/* definition de fonctions externes */

/* declaration de variables locales */

XtAppContext    vl_app_context;
Display 	*pl_Display;
Screen          *pl_ScreenSet;
MrmHierarchy       vg_mrm_hierarchy;
MrmOsOpenParamPtr  vg_mrm_open_param;
MrmCode            vg_class;

/*#define         vg_File_ign        "/users/milleville/migrazur/SUPERV/bin/icfg_ign.uid"*/
#define         vg_File_ign        "/users/milleville/migrazur/SUPERV/bin/icfg_ign.uid"
char *vg_mrm_filename_list_fr[] = { vg_File_ign };

Widget vg_GRSYNT[cm_MAX_SCREENS];           /* widget Affichage Synthese */


tm_SCR_INFO      vm_ScreenSet[cm_MAX_SCREENS];
OBJECT loc = NULL;

Widget toplevel;








void quit_cb (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )
{
int vl_iter;

	
for( vl_iter=0; vl_iter < vg_NumScreens; vl_iter++ )
{
	XtUnmanageChild(vs_GRMACH_SYNT_FM[vl_iter]);
	XtUnmanageChild(vg_MENU[vl_iter].w_menu_fen);
	/*XtDestroyWidget(vg_MENU[vl_iter].w_menu_fen);*/
}

}






/*main (int argc, char **argv)
{
VIEW Vue;

	MTCG06_Ouvrir_Fenetre(Vue,argc,argv);

}
*/



/*int main (int argc, char **argv)*/
/*int MTCG06_Ouvrir_Fenetre (
                           VIEW va_view,int argc, char **argv)*/

main (int argc, char **argv)
{
T_INT4    			vl_source;
T_BOOL    			vl_cr;
int       			vl_nCB;
int       			vl_numMsg;
pid_t     			vl_pid;
T_IPC_MT			vl_mt;
int             		vl_iter;
Arg args[10];
VIEW va_view;


vg_mrm_open_param = (MrmOsOpenParamPtr)XtMalloc(sizeof(MrmOsOpenParam) );


/* initialisation Mrm    */

/* initialisation Motif  */
MrmInitialize();
XtToolkitInitialize ();
vl_app_context = XtCreateApplicationContext ();


/*toplevel = XtAppInitialize(&vl_app_context,"XIcfg_ign",NULL,0,&argc,argv,NULL,args,0);*/

/* ouverture du display  */
pl_Display = XtOpenDisplay (	vl_app_context, 
		     		NULL, 
		     		argv[0], 
		     		"XIcfg_ign",
		     		NULL, 
		     		0, 
		     		&argc, 
		     		argv);

/* probleme d'ouverture ... */
if (!pl_Display)
   printf("main de gmot: ouverture du display impossible");

vg_mrm_open_param->version = MrmOsOpenParamVersion;
vg_mrm_open_param->display = pl_Display;

/* ouverture hierachie francaise */
ouvre_hierarchie( FALSE );

if ( MrmRegisterNames ( reglist, reglist_num ) != MrmSUCCESS )
   printf("main de gmot: pb a l'appel de MrmRegisterNames");


/* initialisation Dataviews */
(void) TInit( (char *)NULL, (char *)NULL ); 
va_view = TviLoad("TAC_help_syn.v");

/* calcul du nombre d'ecrans disponibles sur la station */
vg_NumScreens =  ScreenCount( pl_Display ); 
pl_ScreenSet = ScreenOfDisplay( pl_Display, 0 );

/* -----------------------------------------------------------*/
/* --------- boucle de creation d'une arborescence ---------- */
/* --------- pour chaque ecran du display          ---------- */
/* creation de l'objet MENU, menu principal MIGRAZUR en francais */
/* -----------------------------------------------------------*/
for( vl_iter=0; vl_iter <vg_NumScreens ; vl_iter++ )
{
     vg_PtrSurMenuACreer = (tg_InfosMenu *) calloc (1, (unsigned int) sizeof (tg_InfosMenu));

     create_MENU( pl_Display, 
		   &pl_ScreenSet[vl_iter], 
		   argv[0], 
		   argc, 
		   argv , 
		   vg_mrm_hierarchy, 
		   &vg_class);

     vg_MENU[vl_iter] = *vg_PtrSurMenuACreer;

     free( vg_PtrSurMenuACreer );


     XtRealizeWidget ( vg_MENU[vl_iter].w_menu_fen );

	/*A Creer ou Manager le Synoptique de Synthese */
	cree_ou_manage_GRMACH_SYNT(vg_MENU[vl_iter].w_menu_fen,vl_iter,va_view );
	
}


/* boucle principale d'attente d'evenements  */
XtAppMainLoop (vl_app_context);

exit (0);
 
}


void create_MENU (       
	Display         *display,
	Screen          *screen,
	char            *app_name,
	int             app_argc,
	char            **app_argv,
	MrmHierarchy    hierarchy_id,
	MrmCode         *mrm_class)
{
	Arg al[64];
	register int ac = 0;
	int cr;


	XtSetArg(al[ac],XmNallowShellResize, TRUE);ac++;
	XtSetArg(al[ac],XmNargc, app_argc);ac++;
	XtSetArg(al[ac],XmNargv, app_argv);ac++;
	XtSetArg(al[ac],XmNx, 0);ac++;
	XtSetArg(al[ac],XmNy, 200);ac++;


	toplevel = XtAppCreateShell ( app_name, "XIcfg_ign",
				applicationShellWidgetClass,
				display, al, ac);


	MENU = XmCreateDialogShell ( toplevel, "MENU",
					al, ac);

	if ((cr = MrmFetchWidget (      hierarchy_id,
				"GRMACH_SYNT_FM",
				MENU,
				&GRMACH_SYNT_FM,
				mrm_class )) != MrmSUCCESS)
	{
		printf ( "gmot_  : Erreur de fetch de la hierarchie");
	}
	else
	{


	  XtManageChild(GRMACH_SYNT_FM);

	   vg_PtrSurMenuACreer->w_menu_bar = MENU_MB;
	   vg_PtrSurMenuACreer->w_menu_fen = MENU;
	 }

}       /* Fin create_MENU */


int cree_ou_manage_GRMACH_SYNT(Widget w,int p_num_screen,VIEW va_view)
{

/* creation de GRSYN la premiere fois uniquement */
  if(!vs_GRMACH_SYNT_FM[p_num_screen])
    {
	   printf( " Creation GRSYN pour la 1er fois \n");

	    create_GRMACH_SYNT(w, vg_mrm_hierarchy,&vg_class);

	    vs_GRMACH_SYNT_FM[p_num_screen] = GRMACH_SYNT_FM;
	    vs_GRMACH_SYNT_DA[p_num_screen] = GRMACH_SYNT_DA;

	    /* affichage du synoptique */
	    printf( " Avant declenchement Callback Widegt %d \n",
				    GRMACH_SYNT_DA);

	  	XtManageChild ( vs_GRMACH_SYNT_FM[p_num_screen] );
		AfficheVueAdm( vs_GRMACH_SYNT_DA[p_num_screen], NULL , va_view);
		} 
	else
	{
		printf( " Creation GRSYN apres la 1er fois \n");

	      /* affichage du synoptique */
	  	XtManageChild ( vs_GRMACH_SYNT_FM[p_num_screen] );
	      AfficheVueAdm( vs_GRMACH_SYNT_DA[p_num_screen], NULL , va_view);

	}

	printf( "cree_ou_manage_GRMACH_SYNT : OUT \n");

}       /* Fin cree_ou_manage_GRMACH_SYNT */


void create_GRMACH_SYNT (
	Widget parent,
	MrmHierarchy hierarchy_id,
	MrmCode *mrm_class)
{
		Arg al[64];                    /* Arg List */
		register int ac = 0;           /* Arg Count */


		printf(" create_GRMACH_SYNT : IN \n");
		GRMACH_SYNT_SCROLL =
			XtNameToWidget(GRMACH_SYNT_FM,"GRMACH_SYNT_SCROLL");
		ac = 0;
		GRMACH_SYNT_DA =
		XmCreateDrawingArea( GRMACH_SYNT_SCROLL,"GRMACH_SYNT_DA",al,ac);

		printf( " create_GRMACH_SYNT : OUT -- \
		Widegt SCROLL %d -- Widget Draw %d \n",
		GRMACH_SYNT_SCROLL,GRMACH_SYNT_DA);

}       /* create_GRMACH_SYNT */





int AfficheVueAdm(
	     	Widget           va_widget,
		char*            pa_name,
		VIEW 		 va_view)
{
int  vl = cm_MAX_SCREENS;
int  vl_index = -1;
int  vl_numView;
int  vl_machine;
int  vl_return;
BOOL vl_alreadyOpened = NO;


/* Recherche les infos DataViews relatives au widget argument. */
while(vl--)
{
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
		return XDC_ARG_INV;
	}

    /* Efface, si necessaire, la fenetre concernee. */
    GMeraseScreen(vl_index);


    if ( vl_alreadyOpened == NO )
    {
    	vm_ScreenSet[vl_index].screen = 
			TscOpenSet( NULL, cm_DEFAULT_CLUT,
				V_X_DRAW_WIDGET,
				va_widget,
				V_X_EXPOSURE_BLOCK, YES,
				V_X_RESIZE_BLOCK,   YES,
				V_ACTIVE_CURSOR,
				V_END_OF_LIST );
				
        vm_screen =vm_ScreenSet[vl_index].screen;
        
	if ( vm_ScreenSet[vl_index].screen )
	{
		XtAddCallback( va_widget, XmNexposeCallback,
			 GMexposeCallback, &vm_ScreenSet[vl_index]);
	   	XtAddCallback( va_widget, XmNresizeCallback,
	    		GMresizeCallback, &vm_ScreenSet[vl_index]);
		XtAddCallback( va_widget, XmNinputCallback,
		       GMinputCallback, &vm_ScreenSet[vl_index]);
		 vm_ScreenSet[vl_index].widget = va_widget;

		 vm_ScreenSet[vl_index].MagicFlag = YES;
	 }
	 else
	 {
		 /* Erreur */
	   printf( "gmot_adm_dv, AfficheVueAdm: Impossible d'ouvrir le screen DataViews" );
										  return XDC_NOK;
									  	}
}

	/*  Chargement de la Vue Synoptique ***/
	/*vl_return = GMinitSyn(&vm_ScreenSet[vl_index]);*/
        vl_return= XDC_OK;
        vm_ScreenSet[vl_index].view = va_view;

	if ( vl_return != XDC_OK )
	{
	  	/* Erreur */
		printf( "gmot_adm_dv, AfficheVueAdm: \
	Impossible de charger et initialiser le synoptique\n" );
	   	return XDC_NOK;
	}

	/* Creation du Drawport et affichage de la vue. */
	printf( "Creation Drawport et Affichage Vue !!\n");

	vm_ScreenSet[vl_index].drawport =
		TdpCreateStretch( vm_ScreenSet[vl_index].screen,
	      vm_ScreenSet[vl_index].view, cm_DEFAULT_SCR_VP,
	      (RECTANGLE *)NULL);

	printf( "Creation DrawPort %d \n",vm_ScreenSet[vl_index].drawport);

	TscSetCurrentScreen( vm_ScreenSet[vl_index].screen );
	vl_return = TdpDraw( vm_ScreenSet[vl_index].drawport );
	printf( "Affichage dessin  DrawPort %d \n",vl_return);
	
        vm_drawport_en_cours = vm_ScreenSet[vl_index].drawport;

	return XDC_OK;
}


void GMeraseScreen(int vl_index)
{
  (void) TscSetCurrentScreen(vm_ScreenSet[vl_index].screen);

    if (vm_ScreenSet[vl_index].help.view != (VIEW)NULL )
    {
	(void) TdpDestroy( vm_ScreenSet[vl_index].help.drawport );
	(void) TviDestroy( vm_ScreenSet[vl_index].help.view );
	vm_ScreenSet[vl_index].help.drawport = (DRAWPORT) NULL;
	vm_ScreenSet[vl_index].help.view = (VIEW) NULL;
    }
    if (vm_ScreenSet[vl_index].view != (VIEW)NULL)
  	{
		(void) TdpDestroy( vm_ScreenSet[vl_index].drawport );
		(void) TviDestroy( vm_ScreenSet[vl_index].view );

		TscErase( vm_ScreenSet[vl_index].screen );
		vm_ScreenSet[vl_index].drawport = (DRAWPORT) NULL;
		vm_ScreenSet[vl_index].view = (VIEW) NULL;

		VUerClearAll( vm_ScreenSet[vl_index].screen );
	}
}



int GMinitSyn( tm_SCR_INFO *pa_info)
{


/* Chargement de la vue du synoptique. */
/*     ?????????????????????????? */
pa_info->view = TviLoad( "TAC_help_syn.v");


if ( !pa_info->view )
{
	  /* Error */
	printf( "gmot_adm_dv, GMinitSynSSS: Chargement de vue impossible: TviLoad echoue.");
	   return XDC_NOK;
}
	return XDC_OK;
}



int ouvre_hierarchie     ( int  va_FlagClose)
{
  register int          valeurRetour = XDC_OK;
  static char*          version = "$Id: icfg_ign_fen.c,v 1.1 1995/01/25 15:59:29 milleville Exp $ : ouvre_hierarchie";

/* si flag fermeture hierarchie courante ... */
	if (va_FlagClose == TRUE)
	{
	  	MrmCloseHierarchy( vg_mrm_hierarchy );
	}

	/* ouverture de la hierarchie utile */
      	if ( MrmOpenHierarchy (cg_NB_HIERARCHIES,
	   	vg_mrm_filename_list_fr,
		&vg_mrm_open_param,
     		&vg_mrm_hierarchy ) != MrmSUCCESS )
	{
       		printf( "ouvre_hierarchie: pb a l'appel de MrmOpenHierarchy");
										  valeurRetour = XDC_NOK;
	}
	return (valeurRetour);
}



BOOL GMhandleEvent( 
	Widget va_widget, 
	tm_SCR_INFO* pa_info, 
	XEvent* pa_xevent )
{
  WINEVENT w_event;

    if (!(pa_info->view))
	return NO;

if ( ! loc )
      loc = VOloCreate();

GRwe_convert ( pa_xevent, &w_event );
TloWinEventSetup ( loc, &w_event, pa_info->screen, pa_info->drawport);

if ( VUerHandleLocEvent ( loc ) == INPUT_UNUSED)
		 return YES;

	   return NO;
}


void  GMexposeCallback( 
	Widget va_widget, 
	tm_SCR_INFO* pa_info,
 	caddr_t va_calldata )
{
   WINEVENT w_event;

     if (!(pa_info->view))
	 return;

/* Get rid of the first Expose event (Great Bidouille comes back, ye
	   ah!). */
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

void  GMinputCallback( 
	Widget va_widget, 
	tm_SCR_INFO* pa_info,
	caddr_t va_calldata )
{
  (void) GMhandleEvent ( va_widget, pa_info, (XEvent*) ((XmDrawingAreaCallbackStruct *)va_calldata)->event );
}



void  GMresizeCallback( 
	Widget va_widget, 
	tm_SCR_INFO* pa_info,
 	caddr_t va_calldata )
{
   if (!(pa_info->view))
       return;

 (void) TscSetCurrentScreen(pa_info->screen);
 (void) TscReset(pa_info->screen);
}



