/*E*/
/* Fichier : $Id: gmot_adm.h,v 1.6 1996/03/25 09:56:07 milleville Exp $	1.3	Release : 1.3        Date : 02/08/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm * FICHIER gmot_adm.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	18 Nov 94 	: Creation
* T.Milleville	15 Mar 95 	: Ajout de la fonction list_bl_imp_cb V1.2
* T.Milleville	16 Mar 95 	: Ajout de la fonction livl_imp_cb V1.3
* P.Niepceron   04 Mar 96	: Ajout d'un bt de lancement de tsconf (896) v1.4
* P.Niepceron   07 Mar 96	: Ajout des bt de vidage des bilans et svg manuelle v1.5
******************************************************/

#ifndef gmot_adm
#define gmot_adm

/* fichiers inclus */

/* definitions de constantes exportees */

#define cm_SYN_SSS  0  /* constantes decrivant dans quel */
#define cm_SYN_STA  1  /* mode de travail on se trouve   */
#define cm_SYN_TRA  2
#define cm_BAS_SDO  3
#define cm_BAS_SCO  4
#define cm_ARR_SUR  5
#define cm_DEM_SUR  6
#define cm_ARR_CAL  7
#define cm_SYNC_DISK  8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions extern voides */

extern int GMGetTwoServers(XZSCT_NomMachine, XZSCT_NomMachine, XZSCT_NomMachine);
extern void menu2_sss_cb (Widget, XtPointer, XtPointer );
extern void menu2_sta_cb (Widget, XtPointer, XtPointer );
extern void menu2_sar_cb (Widget, XtPointer, XtPointer );
extern void menu2_bsd_cb (Widget, XtPointer, XtPointer );
extern void menu2_bsc_cb (Widget, XtPointer, XtPointer );
extern void menu2_ars_cb (Widget, XtPointer, XtPointer );
extern void menu2_res_cb (Widget, XtPointer, XtPointer );
extern void menu2_aca_cb (Widget, XtPointer, XtPointer );
extern void menu2_ass_cb (Widget, XtPointer, XtPointer );
extern void menu2_ata_cb (Widget, XtPointer, XtPointer );
extern void menu2_mco_cb (Widget, XtPointer, XtPointer );
extern void menu2_ico_cb (Widget, XtPointer, XtPointer );
extern void menu2_ijo_cb (Widget, XtPointer, XtPointer );
extern void menu2_gdt_cb (Widget, XtPointer, XtPointer );
extern void menu2_vlo_cb (Widget, XtPointer, XtPointer );
extern void menu2_cvl_cb (Widget, XtPointer, XtPointer );
extern void menu2_arc_cb (Widget, XtPointer, XtPointer );
extern void menu2_uni_cb (Widget, XtPointer, XtPointer );
extern void menu2_sav_cb (Widget, XtPointer, XtPointer );
extern void menu2_aid_cb (Widget, XtPointer, XtPointer );
extern void menu2_jas_cb (Widget, XtPointer, XtPointer );
extern void menu2_qui_cb (Widget, XtPointer, XtPointer );
 
extern void bdass_li_mac_cb (Widget, XtPointer, XtPointer );
extern void bdass_val_cb (Widget, XtPointer, XtPointer );
extern void bdass_qui_cb (Widget, XtPointer, XtPointer );

extern void bdsta_qui_cb (Widget, XtPointer, XtPointer );
extern void bdsta_val_cb (Widget, XtPointer, XtPointer );

extern void bdsit_qui_cb (Widget, XtPointer, XtPointer );
extern void bdsit_val_cb (Widget, XtPointer, XtPointer );

extern void bdata_li_mac_cb (Widget, XtPointer, XtPointer );
extern void bdata_val_cb (Widget, int *, XtPointer );
/*extern void bdata_val_cb (Widget, XtPointer, XtPointer );*/
extern void bdata_qui_cb (Widget, XtPointer, XtPointer );

extern void bdasu_val_cb (Widget, XtPointer, XtPointer );
extern void bdasu_qui_cb (Widget, XtPointer, XtPointer );

extern void grsyn_qui_cb (Widget, XtPointer, XtPointer );

extern void grtra_qui_cb (Widget, XtPointer, XtPointer );
extern void grtra_val_cb (Widget, XtPointer, XtPointer );
extern void grtra_selec_cb (Widget, XtPointer, XtPointer );

extern void bdbsd_qui_cb (Widget, XtPointer, XtPointer );
extern void bdbsd_val_cb (Widget, XtPointer, XtPointer );
extern void bdbsd_to_chg_cb (Widget, XtPointer, XtPointer );

extern void bdbsc_qui_cb (Widget, XtPointer, XtPointer );
extern void bdbsc_val_cb (Widget, XtPointer, XtPointer );
extern void bdbsc_to_chg_cb (Widget, XtPointer, XtPointer );

extern void bdaca_qui_cb (Widget, XtPointer, XtPointer );
extern void bdaca_val_cb (Widget, XtPointer, XtPointer );

extern void bderr_qui_cb (Widget, XtPointer, XtPointer );

extern void livl_fil_cb (Widget, XtPointer, XtPointer );
extern void livl_qui_cb (Widget, XtPointer, XtPointer );
extern void livl_imp_cb (Widget, XtPointer, XtPointer );

extern void lija_qui_cb (Widget, XtPointer, XtPointer );

extern void GMAffich_SSSYT(
	Widget va_MenuPere,
	char * va_NomSite,
	int	va_TypeAffich);


extern void sauve_a_verif_cb (Widget, XtPointer, XtPointer ); 
extern void restore_a_verif_cb (Widget, XtPointer, XtPointer ); 
extern void verif_reseau_cb (Widget, XtPointer, XtPointer );
extern void verif_equip_cb (Widget, XtPointer, XtPointer );
extern void verif_SAIDEC_cb (Widget, XtPointer, XtPointer );
extern void verif_globale_cb (Widget, XtPointer, XtPointer );
extern void verif_defaut_cb (Widget, XtPointer, XtPointer );
extern void verif_seuil_tf_cb (Widget, XtPointer, XtPointer );
extern void verif_equ_al_cb (Widget, XtPointer, XtPointer );
extern void modif_reseau_cb (Widget, XtPointer, XtPointer );
extern void modif_equip_cb (Widget, XtPointer, XtPointer );
extern void lecture_bc_cb (Widget, XtPointer, XtPointer );
extern void lecture_bl_cb (Widget, XtPointer, XtPointer );
extern void livrer_cb (Widget, XtPointer, XtPointer );
extern void diffuser_bl_cb (Widget, XtPointer, XtPointer );
extern void tests_1_cb (Widget, XtPointer, XtPointer );
extern void tests_2_cb (Widget, XtPointer, XtPointer );
extern void tests_3_cb (Widget, XtPointer, XtPointer );
extern void tests_4_cb (Widget, XtPointer, XtPointer );

extern void sauve_archiv_cb (Widget, XtPointer, XtPointer );
extern void restaure_archiv_cb (Widget, XtPointer, XtPointer );
extern void lecture_catalogue_cb (Widget, XtPointer, XtPointer );
extern void configurer_equt_cb (Widget, XtPointer, XtPointer );
extern void list_bc_qui_cb (Widget, XtPointer, XtPointer );
extern void list_bl_qui_cb (Widget, XtPointer, XtPointer );
extern void list_bl_imp_cb (Widget, XtPointer, XtPointer );
extern void list_arc_qui_cb (Widget, XtPointer, XtPointer );
extern void list_brbh_qui_cb (Widget, XtPointer, XtPointer );
extern void list_bsbh_qui_cb (Widget, XtPointer, XtPointer );
extern void list_brbh_vider_cb (Widget, XtPointer, XtPointer );
extern void list_bsbh_vider_cb (Widget, XtPointer, XtPointer );
extern void list_hebdo_vider_cb (Widget, XtPointer, XtPointer );
extern void list_BC_vider_cb (Widget, XtPointer, XtPointer );
extern void lecture_brbh_cb (Widget, XtPointer, XtPointer );
extern void lecture_bsbh_cb (Widget, XtPointer, XtPointer );
extern void list_hebdo_qui_cb (Widget, XtPointer, XtPointer );
extern void sauvegarde_hebdo_cb (Widget, XtPointer, XtPointer );
extern void lecture_hebdo_cb (Widget, XtPointer, XtPointer );
extern void sauvegarde_manuel_cb (Widget, XtPointer, XtPointer );

extern void synchro_disques_cb (Widget, XtPointer, XtPointer );

extern void menu_tsconf (Widget, XtPointer, XtPointer );


#endif
