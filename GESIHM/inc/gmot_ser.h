/*E*/
/* Fichier : @(#)gmot_ser.h	1.5	Release : 1.5        Date : 01/25/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_service * FICHIER gmot_service.h
******************************************************
* DESCRIPTION DU MODULE :
*
* 
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 29 Nov 94 	: Creation
* T.Milleville 08 Mar 95 	: Remplacement de xzga.h par xzsi.h V1.3
* P.Niepceron  14 Nov 03	: Mise en commentaire de l'include std.h
******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/

#ifndef gmot_service
#define gmot_service

#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <rtworks/ipc_proto.h>

/*PN UpgradeDV : Mise en commentaire de l'include std.h */
/*#include "std.h"   */      /* <stdio.h> etc., scalar & macro definitions */
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>


#include <Mrm/MrmAppl.h>
#include <Mrm/MrmPublic.h>


#include <Xm/XmStrDefs.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/Separator.h>
#include <Xm/MessageB.h>

#include "dvstd.h" 
#include "dvtools.h" 
#include "dvGR.h"    
#include "VOstd.h"  
#include "dvparams.h" 


#include "Tfundecl.h"    /* T routines (screens, drawports & views) */
#include "VOfundecl.h"   /* VO routines (objects) */
#include "VUerfundecl.h" /* VUer routines (event handling routines) */
#include "VGfundecl.h"   /* VG routines (data grp and var descriptor routines) */
#include "VTfundecl.h"   /* VT routines */ 
#include "VUfundecl.h"   /* VU routines */ 
#include "VPfundecl.h"   /* VU routines */ 

#include "xdc.h"
#include "xdl.h"
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzsa.h"
#include "xzse.h"
#include "xzsp.h"
#include "xzsm.h"
#include "xzss.h"
#include "xzsi.h"

/* definitions de constantes exportees */

/* Maxima utilises */
#define cg_MAX_CLASSE		17
#define cg_MAX_BOUTON_MENU1	150
#define cg_MAX_WIDGETS		65 
#define cg_MAX_BDIAL_W          30	/* nombre max de widget par boite */
#define cg_MAX_NOM_BOITE        9	/* longueur d'un nom de boite */

/* Nombre de fichiers UID a ouvrir pour une langue donnee */
#define cg_NB_HIERARCHIES	5

#define cg_VIDE  		0
#define cg_NONVIDE  		1

/* A REMPLACER PAR UN ENUM ... */
#define cg_NB_TYP_SYN  		5    
#define cg_NO_SYN      		0
#define cg_TYP_ARC     		1
#define cg_TYP_SSS     		2
#define cg_TYP_TAC     		3
#define cg_TYP_TRA     		4
#define cg_TYP_SYNT     	5

/* nom des masques de saisies */
#define cg_MABPS	"MABPS"
#define cg_MADPS	"MADPS"
#define cg_MAEPS	"MAEPS"
#define cg_MAABS	"MAABS"
#define cg_MAADV	"MAADV"
#define cg_MADAB	"MADAB"
#define cg_MAPAC	"MAPAC"
#define cg_MAIGA	"MAIGA"
#define cg_MASSS	"MASSS"
#define cg_MASYN	"MASYN"
#define cg_MATPA	"MATPA"
#define cg_MABDP	"MABDP"
#define cg_MAEPD	"MAEPD"
#define cg_MACPR	"MACPR"
#define cg_MACAT	"MACAT"
#define cg_MAGEN	"MAGEN"
#define cg_MAPUT	"MAPUT"
#define cg_MAAUT	"MAAUT"
#define cg_MASUR	"MASUR"
#define cg_MAJDI	"MAJDI"
#define cg_MAANN	"MAANN"
#define cg_MAMES	"MAMES"
#define cg_MARAD	"MARAD"
#define cg_MAFEN	"MAFEN"
#define cg_MAPAR	"MAPAR"

#define cg_MABPS2	"MABPS2"
#define cg_MADPS2	"MADPS2"
#define cg_MAEPS2	"MAEPS2"
#define cg_MAABS2	"MAABS2"
#define cg_MAADV2	"MAADV2"
#define cg_MADAB2	"MADAB2"
#define cg_MAPAC2	"MAPAC2"
#define cg_MAIGA2	"MAIGA2"
#define cg_MASSS2	"MASSS2"
#define cg_MASYN2	"MASYN2"
#define cg_MATPA2	"MATPA2"
#define cg_MABDP2	"MABDP2"
#define cg_MAEPD2	"MAEPD2"
#define cg_MACPR2	"MACPR2"
#define cg_MACAT2	"MACAT2"
#define cg_MAGEN2	"MAGEN2"
#define cg_MAPUT2	"MAPUT2"
#define cg_MAAUT2	"MAAUT2"
#define cg_MASUR2	"MASUR2"
#define cg_MAJDI2	"MAJDI2"
#define cg_MAANN2	"MAANN2"
#define cg_MAMES2	"MAMES2"
#define cg_MARAD2	"MARAD2"
#define cg_MAFEN2	"MAFEN2"

/* constantes definissant le nom des boites de dialogue */
#define cg_BDACC	"BDACC"
#define cg_BDCHM	"BDCHM"
#define cg_BDCOM	"BDCOM"
#define cg_BDAID	"BDAID"
#define cg_BDARC	"BDARC"
#define cg_BDSED	"BDSED"

#define cg_BDRJD	"BDRJD"
#define cg_BDRJB	"BDRJB"

/* constantes definissant le nom des boites de dialogue */
#define cg_LISA         "LISA"
#define cg_LIMO         "LIMO"

/* definitions de types exportes */

/* Enumeres de l'ouverture d'un flag */
typedef enum Ouverture 
{
  cg_FLAG_OUVERT = 0, 
  cg_FLAG_FERME
} tg_ouverture;


/* Enumeres du mode de fonctionnement */
typedef enum ModeCourant
{
  cg_MODE_NORMAL=0, cg_MODE_INSTRUCTION
} tg_mode_courant;

/* Enumeres des classes d'autorisations */
typedef enum ClasseAutorisation 
{
  cg_CLASSE01=0, cg_CLASSE02, cg_CLASSE03, cg_CLASSE04, 
  cg_CLASSE05, cg_CLASSE06, cg_CLASSE07, cg_CLASSE08,
  cg_CLASSE09, cg_CLASSE10, cg_CLASSE11, cg_CLASSE12,
  cg_CLASSE13, cg_CLASSE14, cg_CLASSE15, cg_CLASSE16,
  cg_CLASSE17
} tg_ClasseAutorisation;

/* Enumeres des droits d acces a la base */
typedef enum DroitBase
{
   cg_DROIT_MODIF=0, cg_DROIT_REGARD
} tg_droit_base;

/* nom des masques de saisies: voir les constantes cg_MAxxx */
typedef char tg_nomMasque[6];

/* nom des boites de dialogue (voir constantes definies plus haut)*/
typedef char tg_nom_boite[cg_MAX_NOM_BOITE];

typedef char TG_MessageType[50];

typedef Widget tg_TableauWidget[cg_MAX_WIDGETS];

/* Type du tableau des autorisations */
typedef Boolean tg_Autorisation[cg_MAX_BOUTON_MENU1][cg_MAX_CLASSE];


typedef enum TypeToggle 
{
  cg_TOGGLE_CRE = 0, 
  cg_TOGGLE_MOD,
  cg_TOGGLE_SUP,
  cg_NO_TOGGLE
} tg_typeToggle;
     

/* Widgets Index  */
typedef struct ZoneIndex
{
  Widget 			w_index1;
  Widget			w_index2;
  Widget			w_index3;
  Widget			w_index4;
  Widget			w_index5;
  Widget			w_index6;
  Widget			w_index7;
} tg_zoneIndex;


/* Widgets Toggles de la zone Infos */
typedef struct ZoneInfo
{
  Widget 			w_toggleCreat;
  Widget			w_toggleModif;
  Widget			w_toggleSuppr;
} tg_zoneInfo;


/* Widgets Propres a chaque Masque */
typedef struct ZonePropre
{
  int				nbreWidget;
  Widget			w_tableau [cg_MAX_WIDGETS];
  Widget 			w_horodate;
} tg_zonePropre;


/* Widgets Propres a chaque Tri */
typedef struct ZoneTri
{
  int				nbreWidget;
  Widget			w_tableau [cg_MAX_WIDGETS];
} tg_zoneTri;


/* Widgets boutons d'administrations */
typedef struct ZoneAdm
{
  Widget 			w_buttonCreat;
  Widget 			w_buttonModif;
  Widget			w_buttonSuppr;
  
  Widget			w_buttonAide;
  Widget			w_buttonConsu;
  
  Widget			w_buttonPrec;
  Widget			w_buttonSuiv;
  
  Widget			w_buttonValid;
  Widget			w_buttonQuit;
} tg_zoneAdm;


/* Infos sur les elements des fenetres des masques */
typedef struct InfosFenetres
{
  /* Widget Root */
  Boolean			planif;

  tg_nomMasque			nomMasque;  	/* Nom du Masque */

  Widget          	  	w_fen;		/* Widget Root */

  struct InfosFenetres		*masqueOrig;    /* masque origine de l'appel */

  /* Zone d'index */
  tg_zoneIndex			zoneIndex; 	/* Widgets des Index */

  /* zone des toggles */
  tg_zoneInfo			zoneInfo;	/* Widgets Toggles Haut */

  /* primitive xzcs a appeler */
  /*int 				(*primitive_xzcs_bd)(XZCST_Operation, void *);*/

  /* zone libre propre au masque */
  tg_zonePropre			zonePropre;	/* Widgets propres au masque */

  /* zone libre propre au tri */
  tg_zoneTri			zoneTri;	/* Widgets propres au tri */

  /* zone des boutons d'administration */
  tg_zoneAdm			zoneAdm;	/* Widgets Boutons bas */
  
  /* structure d'accueil pour config */
  void*				Donnees;	/* Donnees de Config */

  /* structure d'accueil pour le tri */
  void*				Tri;		/* Donnees de Tri */

} tg_InfosFenetres;


/* infos sur les elements des boites de dialogue */

/* zone des push buttons pour la boite de dialogue */
typedef struct {
	Widget	w_button_valid; 	/* bouton "Valider" */
	Widget	w_button_quit;  	/* bouton "Quitter */
	Widget  w_button_suppl[6];  	/* boutons supplementaires */
} tg_zoneValid;

/* zone propre a chaque boite de dialogue */
typedef struct {
  	int				nbreWidget;
	Widget 	w_tableau[cg_MAX_BDIAL_W]; /* tableau des widgets propres */
} tg_zonePropreDia;


typedef struct {
	Widget 			w_fen;		/* widget root */
	tg_nom_boite		nomBoite;	/* nom de la boite */
  	tg_InfosFenetres	*masqueOrig;    /* masque origine de l appel */
	tg_zonePropreDia 	zonePropreDia;	/* zone propre a chaque boite */
	tg_zoneValid 		zoneValid;	/* push buttons du bas */
	void* 			Donnees;	/* donnees propres a la boite */
} tg_Infos_Dialogues;

typedef struct s_EltFenetre {
	tg_InfosFenetres 	structData;
	struct 			s_EltFenetre  *suiv;
} tg_EltFenetre ;

typedef struct s_Basculement {
	int 			Cyclique;
	int 			Periode;
} tg_Basculement ;

typedef struct s_Executable {
	XZSPT_TypeFichier	typeFic;
	XZSCT_NomMachine   	nomMac;
	XZSPT_NomFichier     	nomFichier;
	XZSCT_NumVersion   	numVersion;
	XZSPT_Horodate     	horodate;
} tg_Executable;


/* MODIFICATION  MIGRAZUR -> MIGRAZUR */
typedef struct s_EtatTache {
	int	  	  	FO;
	int	  	  	ME;
	int 	  	  	IN;
	int 	  	  	BD;
	int	  	  	D1;
	int	  	  	D2;
	int 	  	  	D3;
	int	  	  	Ecr;
	int	  	  	INTER;
	int	  	  	REGLE;
} tg_EtatTache;

typedef struct s_InfosMenu {
	Widget   	  	w_menu_fen;
	Widget   	  	w_menu_bar;
	int			nb_widget;
	Widget   	  	tab_w [200];
} tg_InfosMenu; 

/* Declarations des fonctions externes */

extern void EcritChampsACA( int );
extern void EcritChampsBSD( char *, int );
extern void EcritChampsBSC( char *, int );
extern void EcritChampsASS( int );
extern void EcritChampsATA( int );
extern int LitChampsBSD( tg_Basculement * );
extern int LitChampsBSC( tg_Basculement *);
extern int LitChampsLIVL( XZSCT_NomMachine );

#endif



