/* Fichier : @(#)gmot_mot_bdl.h	2.1      Release : 2.1        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE gmot_mot_bdl * FICHIER gmot_mot_bdl.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* dumont	21 Jun 1994	: Cration
******************************************************/

#ifndef gmot_mot_bdl
#define gmot_mot_bdl

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void create_BDASU (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_LIJA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_LIVL (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDERR (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDACA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDBSD (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDBSC (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDASS (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_GRTRA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDSTA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDSIT (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_BDATA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_GRSYN (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);
extern void create_MENU (Display *display, Screen *screen, char *app_name, int app_argc, char **app_argv, MrmHierarchy hierarchy_id, MrmCode *mrm_class);

extern void create_LIST_BC (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class);


#endif
