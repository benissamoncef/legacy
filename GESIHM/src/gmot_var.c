/*E*/
/* Fichier : $Id: gmot_var.c,v 1.2 1996/08/29 10:39:46 milleville Exp $		        Release : $Revision: 1.2 $              Date : $Date: 1996/08/29 10:39:46 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_var * FICHIER gmot_var.c
******************************************************
* DESCRIPTION DU MODULE :
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  14 Nov 94 	: Creation
* P.Niepceron	28 Aou 96	: Ajout des fichiers uid specifiques PC simplifie v1.2
*
*******************************************************/

/******************************************************
 * TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

#include "gmot_ser.h"
#include "gmot_var.h"

static char *version = "$Id: gmot_var.c,v 1.2 1996/08/29 10:39:46 milleville Exp $ : gmot_var" ;

/**********************************************/
/* declaration de variables globales a TGMOTI */

/*XDY_NomFic505           *pg_vues_Berne;
XDY_NomFic505		*pg_vues_locales;*/

/* Tableau de correspondance des classes d'autorisations et des boutons */
tg_Autorisation    vg_Autorisation = {True};


tg_EltFenetre      	vg_TabFenetres[50];
tg_InfosFenetres  	*vg_PtrSurFenACreer;
tg_Infos_Dialogues  	*vg_PtrSurDiaACreer;
tg_InfosMenu  		*vg_PtrSurMenuACreer;

/* variables pour Mrm */
MrmHierarchy       vg_mrm_hierarchy;
MrmOsOpenParamPtr  vg_mrm_open_param;
MrmCode            vg_class;

int   		   vg_NumScreens;   /* nombre d'ecrans de la station */

#define		vg_File_MigAdmin 	XDC_NSUP_PATHEXEC"/MigAdmin.uid"
#define         vg_File_MigAdminpcs     XDC_NSUP_PATHEXEC"/MigAdminpcs.uid"
#define		vg_File_SurfDivers_fr 	XDC_NSUP_PATHEXEC"/SurfDivers_fr.uid"
#define		vg_File_SurfBoites_fr 	XDC_NSUP_PATHEXEC"/SurfBoites_fr.uid"
#define		vg_SurfMasqStat_fr 	XDC_NSUP_PATHEXEC"/SurfMasqStat_fr.uid"
#define		vg_SurfMasqDyn_fr 	XDC_NSUP_PATHEXEC"/SurfMasqDyn_fr.uid"


char *vg_mrm_filename_list_fr[] = { vg_File_MigAdmin ,
				    vg_File_SurfDivers_fr,
				    vg_File_SurfBoites_fr,
				    vg_SurfMasqStat_fr,
				    vg_SurfMasqDyn_fr };

char *vg_mrm_filename_list_frpcs[] = { vg_File_MigAdminpcs,
				    vg_File_SurfDivers_fr,
				    vg_File_SurfBoites_fr,
				    vg_SurfMasqStat_fr,
				    vg_SurfMasqDyn_fr };

pid_t 		   vg_pid_term;


/* mode de travail de la station (normal ou instruction */
tg_mode_courant    vg_mode_courant = cg_MODE_NORMAL;  

char          	   *vg_NewPassword;     

/* droit d'acces, restreint ou non, a la base de donnees */
tg_droit_base	   vg_droit_base;    

XZSCT_NomSite      vg_NomSite;
int		  vg_TypeMachine;

tg_InfosMenu vg_MENU[5];		/* widget menu d'administration */

/* widgets nommes dans les hierarchies          */
/* on peut les visualiser sous XDesigner        */
/* Les regles de nommage sont les suivantes:    */
/* MB = Menu Bar, FM = ForM, TO = TOggle Button */
/* DA = Drawing Area, LI = LIste, LA = LAbel    */
/* CH = CHamp de saisie                         */

Widget vg_GRSYN[5];            /* widget Affichage Synoptique */
Widget vg_GRSYN_FM[5];            /* widget Affichage Synoptique */
Widget vg_GRTRA[5];            /* widget Affichage Trace */
Widget vg_GRSYNT[5];           /* widget Affichage Synthese */
Widget vg_GRMACH_SYNT_FM[5];           /* widget Affichage Synthese */

Widget MENU = (Widget) NULL;
Widget MENU_MB = (Widget) NULL;
Widget GRMACH_SYNT = (Widget) NULL;
Widget GRMACH_SYNT_FM = (Widget) NULL;
Widget GRMACH_SYNT_SCROLL = (Widget) NULL;
Widget GRMACH_SYNT_DA = (Widget) NULL;
Widget GRTRA = (Widget) NULL;
Widget GRTRA_FM = (Widget) NULL;
Widget GRTRA_SCROLL = (Widget) NULL;
Widget GRTRA_DA = (Widget) NULL;
Widget GRTRA_LI = (Widget) NULL;
Widget GRTRA_TO_FO = (Widget) NULL;
Widget GRTRA_TO_ME = (Widget) NULL;
Widget GRTRA_TO_IN = (Widget) NULL;
Widget GRTRA_TO_BD = (Widget) NULL;
Widget GRTRA_TO_D1 = (Widget) NULL;
Widget GRTRA_TO_D2 = (Widget) NULL;
Widget GRTRA_TO_D3 = (Widget) NULL;
Widget GRTRA_TO_EC = (Widget) NULL;
Widget GRTRA_TO_INTER = (Widget) NULL;
Widget GRTRA_TO_REGLE = (Widget) NULL;
Widget GRSYN = (Widget) NULL;
Widget GRSYN_FM = (Widget) NULL;
Widget GRSYN_SCROLL = (Widget) NULL;
Widget GRSYN_DA = (Widget) NULL;
Widget BDSIT = (Widget) NULL;
Widget BDSIT_FM = (Widget) NULL;
Widget BDSIT_LI_SIT = (Widget) NULL;
Widget BDSTA = (Widget) NULL;
Widget BDSTA_FM = (Widget) NULL;
Widget BDSTA_LI_MAC = (Widget) NULL;
Widget BDATA = (Widget) NULL;
Widget BDATA_FM = (Widget) NULL;
Widget BDATA_LI_MAC = (Widget) NULL;
Widget BDATA_LI_TAC = (Widget) NULL;
Widget BDATA_TO_AR = (Widget) NULL;
Widget BDATA_TO_LA = (Widget) NULL;
Widget BDASS = (Widget) NULL;
Widget BDASS_FM = (Widget) NULL;
Widget BDASS_LI_MAC = (Widget) NULL;
Widget BDASS_LI_SSS = (Widget) NULL;
Widget BDASS_TO_AR = (Widget) NULL;
Widget BDASS_TO_LA = (Widget) NULL;
Widget BDACA = (Widget) NULL;
Widget BDACA_FM = (Widget) NULL;
Widget BDACA_LI_MAC = (Widget) NULL;
Widget BDACA_TO_AVE = (Widget) NULL;
Widget BDACA_TO_SAN = (Widget) NULL;
Widget BDERR = (Widget) NULL;
Widget BDERR_FM = (Widget) NULL;
Widget BDERR_LA = (Widget) NULL;
Widget BDBSD = (Widget) NULL;
Widget BDBSD_FM = (Widget) NULL;
Widget BDBSD_LA_1 = (Widget) NULL;
Widget BDBSD_LA_2 = (Widget) NULL;
Widget BDBSD_CH_PER = (Widget) NULL;
Widget BDBSD_TO_CYC = (Widget) NULL;
Widget BDBSD_TO_NCY = (Widget) NULL;
Widget BDBSC = (Widget) NULL;
Widget BDBSC_FM = (Widget) NULL;
Widget BDBSC_LI_SIT = (Widget) NULL;
Widget BDBSC_LA_1 = (Widget) NULL;
Widget BDBSC_LA_2 = (Widget) NULL;
Widget BDBSC_CH_PER = (Widget) NULL;
Widget BDBSC_TO_CYC = (Widget) NULL;
Widget BDBSC_TO_NCY = (Widget) NULL;
Widget BDASU = (Widget) NULL;
Widget BDASU_FM = (Widget) NULL;
Widget BDASU_TO_AR = (Widget) NULL;
Widget BDASU_TO_LA = (Widget) NULL;
Widget BDASU_LI_MAC = (Widget) NULL;
Widget LIVL = (Widget) NULL;
Widget LIVL_FM = (Widget) NULL;
Widget LIVL_LI = (Widget) NULL;
Widget LIVL_CH_MAC = (Widget) NULL;
Widget LIJA = (Widget) NULL;
Widget LIJA_FM = (Widget) NULL;
Widget LIJA_LI = (Widget) NULL;
Widget BILAN_CONFIG = (Widget) NULL;
Widget BILAN_CONFIG_FM = (Widget) NULL;
Widget LIST_BC = (Widget) NULL;
Widget LIST_BC_FM = (Widget) NULL;
Widget LIST_BC_LI = (Widget) NULL;
Widget LIST_BL = (Widget) NULL;
Widget LIST_BL_FM = (Widget) NULL;
Widget LIST_BL_LI = (Widget) NULL;
Widget LIST_ARC = (Widget) NULL;
Widget LIST_ARC_FM = (Widget) NULL;
Widget LIST_ARC_LI = (Widget) NULL;
Widget LIST_BRBH = (Widget) NULL;
Widget LIST_BRBH_FM = (Widget) NULL;
Widget LIST_BRBH_LI = (Widget) NULL;
Widget LIST_BSBH = (Widget) NULL;
Widget LIST_BSBH_FM = (Widget) NULL;
Widget LIST_BSBH_LI = (Widget) NULL;
Widget LIST_HEBDO = (Widget) NULL;
Widget LIST_HEBDO_FM = (Widget) NULL;
Widget LIST_HEBDO_LI = (Widget) NULL;

/* Affichage des masques */

int		vg_affiche_BP = False;
int		vg_affiche_DP = False;
int		vg_affiche_EP = False;
int		vg_affiche_ABS = False;
int		vg_affiche_Rela = False;
int		vg_affiche_DABS = False;
int		vg_affiche_Pac = False;
int		vg_affiche_InfoG = False;
int		vg_affiche_SectSousSect = False;
int		vg_affiche_Image = False;
int		vg_affiche_Occup = False;
int		vg_affiche_ConvBP = False;
int		vg_affiche_ConvEP = False;
int		vg_affiche_CatPrev = False;
int		vg_affiche_CatTrain = False;
int		vg_affiche_GenreTrain = False;
int		vg_affiche_Utilisateur = False;
int		vg_affiche_Classe = False;
int		vg_affiche_Fonction = False;
int		vg_affiche_Justif = False;
int		vg_affiche_Annuaire = False;
int		vg_affiche_MsgCode = False;
int		vg_affiche_Radio = False;
int		vg_affiche_ParamFen = False;
int		vg_affiche_ParamGlob = False;

int		vg_affiche_BP2 = False;
int		vg_affiche_DP2 = False;
int		vg_affiche_EP2 = False;
int		vg_affiche_ABS2 = False;
int		vg_affiche_Rela2 = False;
int		vg_affiche_DABS2 = False;
int		vg_affiche_Pac2 = False;
int		vg_affiche_InfoG2 = False;
int		vg_affiche_SectSousSect2 = False;
int		vg_affiche_Image2 = False;
int		vg_affiche_Occup2 = False;
int		vg_affiche_ConvBP2 = False;
int		vg_affiche_ConvEP2 = False;
int		vg_affiche_CatPrev2 = False;
int		vg_affiche_CatTrain2 = False;
int		vg_affiche_GenreTrain2 = False;
int		vg_affiche_Utilisateur2 = False;
int		vg_affiche_Classe2 = False;
int		vg_affiche_Fonction2 = False;
int		vg_affiche_Justif2 = False;
int		vg_affiche_Annuaire2 = False;
int		vg_affiche_MsgCode2 = False;
int		vg_affiche_Radio2 = False;
int		vg_affiche_ParamFen2 = False;


/* MODIFICATION    MIGRAZUR -> MIGRAZUR */
int  vg_nb_appel_arch;
int  vg_nb_vues_arch_ecran;

