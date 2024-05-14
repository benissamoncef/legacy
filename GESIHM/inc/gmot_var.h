/*E*/
/* Fichier : $Id: gmot_var.h,v 1.2 1996/08/29 10:27:39 milleville Exp $		        Release : $Revision: 1.2 $              Date : $Date: 1996/08/29 10:27:39 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_var * FICHIER gmot_var.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* mule	15 Feb 1994	: Cration
* niepcerom	29 Aou 1996	: Ajout vg_NomSite et vg_TypeMachine v1.2
******************************************************/

#ifndef gmot_var
#define gmot_var

/* Fichiers Inclus */
#include "gmot_ser.h"

/* Nombre de hierarchies par langue */
#define cg_nb_hierarchies	5

/*extern XDY_NomFic505           *pg_vues_Berne;
extern XDY_NomFic505           *pg_vues_locales;*/

/* Table de correspondances des autorisations/ boutons menu1 */
extern tg_Autorisation 		vg_Autorisation;

extern tg_EltFenetre  		vg_TabFenetres[50];
extern tg_InfosFenetres*       	vg_PtrSurFenACreer;
extern tg_Infos_Dialogues*     	vg_PtrSurDiaACreer;
extern tg_InfosMenu*     	vg_PtrSurMenuACreer;

extern MrmHierarchy  		vg_mrm_hierarchy;
extern MrmOsOpenParamPtr  	vg_mrm_open_param;
extern MrmCode  		vg_class;

extern int			vg_NumScreens;

extern char * 			vg_mrm_filename_list_fr[];
extern char * 			vg_mrm_filename_list_frpcs[];

extern char * 			vg_mrm_filename_list_al[];

extern pid_t 			vg_pid_term;

extern tg_mode_courant    	vg_mode_courant;

extern  char              *vg_NewPassword;

extern tg_droit_base      vg_droit_base;  
extern XZSCT_NomSite      vg_NomSite;
extern int		  vg_TypeMachine;

extern tg_InfosMenu vg_MENU[5];             /* widget menu d'administration */

extern Widget vg_GRSYN[5];            /* widget Affichage Synoptique */
extern Widget vg_GRSYN_FM[5];            /* widget Affichage Synoptique */
extern Widget vg_GRTRA[5];            /* widget Affichage Trace */
extern Widget vg_GRSYNT[5];           /* widget Affichage Synthese */
extern Widget vg_GRMACH_SYNT_FM[5];           /* widget Affichage Synthese */



extern Widget 			MENU ;
extern Widget  		 	MENU_MB ;
extern Widget  		 	GRMACH_SYNT ;
extern Widget  		 	GRMACH_SYNT_FM ;
extern Widget  		 	GRMACH_SYNT_SCROLL ;
extern Widget  		 	GRMACH_SYNT_DA ;
extern Widget 			GRTRA ;
extern Widget 			GRTRA_FM ;
extern Widget 			GRTRA_SCROLL ;
extern Widget 			GRTRA_DA ;
extern Widget 			GRTRA_LI;
extern Widget 			GRTRA_TO_FO;
extern Widget 			GRTRA_TO_ME;
extern Widget 			GRTRA_TO_IN;
extern Widget 			GRTRA_TO_BD;
extern Widget 			GRTRA_TO_D1;
extern Widget 			GRTRA_TO_D2;
extern Widget 			GRTRA_TO_D3;
extern Widget 			GRTRA_TO_EC;
extern Widget 			GRTRA_TO_INTER;
extern Widget 			GRTRA_TO_REGLE;
extern Widget 			GRSYN ;
extern Widget 			GRSYN_FM ;
extern Widget 			GRSYN_SCROLL ;
extern Widget 			GRSYN_DA ;
extern Widget 			BDSIT;
extern Widget 			BDSIT_FM;
extern Widget 			BDSIT_LI_SIT;
extern Widget 			BDSTA;
extern Widget 			BDSTA_FM;
extern Widget 			BDSTA_LI_MAC;
extern Widget 			BDATA;
extern Widget 			BDATA_FM;
extern Widget 			BDATA_LI_MAC;
extern Widget 			BDATA_LI_TAC;
extern Widget 			BDATA_TO_AR;
extern Widget 			BDATA_TO_LA;
extern Widget 			BDASS;
extern Widget 			BDASS_FM;
extern Widget 			BDASS_LI_MAC;
extern Widget 			BDASS_LI_SSS;
extern Widget 			BDASS_TO_AR;
extern Widget 			BDASS_TO_LA;
extern Widget 			BDBSC;
extern Widget 			BDBSC_FM;
extern Widget 			BDBSC_LI_SIT;
extern Widget 			BDBSC_LA_1;
extern Widget 			BDBSC_LA_2;
extern Widget 			BDBSC_CH_PER;
extern Widget 			BDBSC_TO_CYC;
extern Widget 			BDBSC_TO_NCY;
extern Widget 			BDBSD;
extern Widget 			BDBSD_FM;
extern Widget 			BDBSD_LA_1;
extern Widget 			BDBSD_LA_2;
extern Widget 			BDBSD_CH_PER;
extern Widget 			BDBSD_TO_CYC;
extern Widget 			BDBSD_TO_NCY;
extern Widget 			BDERR;
extern Widget 			BDERR_FM;
extern Widget 			BDERR_LA;
extern Widget 			BDACA;
extern Widget 			BDACA_FM;
extern Widget 			BDACA_LI_MAC;
extern Widget 			BDACA_TO_AVE;
extern Widget 			BDACA_TO_SAN;
extern Widget 			BDASU;
extern Widget 			BDASU_FM;
extern Widget 			BDASU_LI_MAC;
extern Widget 			BDASU_LI_TAC;
extern Widget 			BDASU_TO_AR;
extern Widget 			BDASU_TO_LA;
extern Widget 			BDASU_LI_MAC;
extern Widget 			LIVL;
extern Widget 			LIVL_LI;
extern Widget 			LIVL_FM;
extern Widget 			LIVL_CH_MAC;
extern Widget 			LIJA;
extern Widget 			LIJA_LI;
extern Widget 			LIJA_FM;
extern Widget 			BILAN_CONFIG;
extern Widget 			BILAN_CONFIG_FM;
extern Widget 			LIST_BC;
extern Widget 			LIST_BC_FM;
extern Widget 			LIST_BC_LI;
extern Widget 			LIST_BL;
extern Widget 			LIST_BL_FM;
extern Widget 			LIST_BL_LI;
extern Widget 			LIST_ARC;
extern Widget 			LIST_ARC_FM;
extern Widget 			LIST_ARC_LI;
extern Widget 			LIST_BRBH;
extern Widget 			LIST_BRBH_FM;
extern Widget 			LIST_BRBH_LI;
extern Widget 			LIST_BSBH;
extern Widget 			LIST_BSBH_FM;
extern Widget 			LIST_BSBH_LI;
extern Widget 			LIST_HEBDO;
extern Widget 			LIST_HEBDO_FM;
extern Widget 			LIST_HEBDO_LI;

/* Affichage des masques */

extern int		vg_affiche_BP;
extern int		vg_affiche_DP;
extern int		vg_affiche_EP;
extern int		vg_affiche_ABS;
extern int		vg_affiche_Rela;
extern int		vg_affiche_DABS;
extern int		vg_affiche_Pac;
extern int		vg_affiche_InfoG;
extern int		vg_affiche_SectSousSect;
extern int		vg_affiche_Image;
extern int		vg_affiche_Occup;
extern int		vg_affiche_ConvBP;
extern int		vg_affiche_ConvEP;
extern int		vg_affiche_CatPrev;
extern int		vg_affiche_CatTrain;
extern int		vg_affiche_GenreTrain;
extern int		vg_affiche_Utilisateur;
extern int		vg_affiche_Classe;
extern int		vg_affiche_Fonction;
extern int		vg_affiche_Justif;
extern int		vg_affiche_Annuaire;
extern int		vg_affiche_MsgCode;
extern int		vg_affiche_Radio;
extern int		vg_affiche_ParamFen;
extern int		vg_affiche_ParamGlob;

extern int		vg_affiche_BP2;
extern int		vg_affiche_DP2;
extern int		vg_affiche_EP2;
extern int		vg_affiche_ABS2;
extern int		vg_affiche_Rela2;
extern int		vg_affiche_DABS2;
extern int		vg_affiche_Pac2;
extern int		vg_affiche_InfoG2;
extern int		vg_affiche_SectSousSect2;
extern int		vg_affiche_Image2;
extern int		vg_affiche_Occup2;
extern int		vg_affiche_ConvBP2;
extern int		vg_affiche_ConvEP2;
extern int		vg_affiche_CatPrev2;
extern int		vg_affiche_CatTrain2;
extern int		vg_affiche_GenreTrain2;
extern int		vg_affiche_Utilisateur2;
extern int		vg_affiche_Classe2;
extern int		vg_affiche_Fonction2;
extern int		vg_affiche_Justif2;
extern int		vg_affiche_Annuaire2;
extern int		vg_affiche_MsgCode2;
extern int		vg_affiche_Radio2;
extern int		vg_affiche_ParamFen2;


/* MODIFICATION MIGRAZUR -> MIGRAZUR */
extern int vg_nb_appel_arch;
extern int vg_nb_vues_arch_ecran;

#endif
