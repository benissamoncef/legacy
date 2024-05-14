/*E*/
/* Fichier : @(#)gmot_com.h	1.1	      Release : 1.1        Date : 11/17/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun * FICHIER gmot_commun.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Declaration des constantes utiles aux modules gmot_XXX
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	17 Nov 94	: Creation
******************************************************/

#ifndef gmot_commun
#define gmot_commun

/* fichiers inclus */

/* definitions de constantes exportees */
typedef enum BoutonImage 
{
  cg_BOUTON_IMAGE1=1, cg_BOUTON_IMAGE2, cg_BOUTON_IMAGE3, 
  cg_BOUTON_IMAGE4, cg_BOUTON_IMAGE5, cg_BOUTON_IMAGE6, 
  cg_BOUTON_IMAGE7, cg_BOUTON_IMAGE8, cg_BOUTON_IMAGE9, 
  cg_BOUTON_IMAGE10, cg_BOUTON_IMAGE11, cg_BOUTON_IMAGE12, 
  cg_BOUTON_IMAGE13, cg_BOUTON_IMAGE14, cg_BOUTON_IMAGE15, 
  cg_BOUTON_IMAGE16, cg_BOUTON_IMAGE17, cg_BOUTON_IMAGE18, 
  cg_BOUTON_IMAGE19, cg_BOUTON_IMAGE20, cg_BOUTON_IMAGE21, 
  cg_BOUTON_IMAGE22, cg_BOUTON_IMAGE23, cg_BOUTON_IMAGE24
} tg_BoutonImage;

/* indices permettant d'acceder aux boutons du menu principal */
typedef enum BoutonMenu1
{
IND_MENU1_MIGRAZUR = 	0,
IND_MENU1_ACCES = 	1,
IND_MENU1_CHGT = 	2,
IND_MENU1_PASS = 	3,
IND_MENU1_INSTR = 	4,
IND_MENU1_SYS = 	5,
IND_MENU1_ADMIN = 	6,
IND_MENU1_QUIT = 	7,
IND_MENU1_LISTE = 	8,
IND_MENU1_COMM = 	9,
IND_MENU1_INFO = 	10,
IND_MENU1_SYNOP = 	11,
IND_MENU1_ONLINE = 	12,
IND_MENU1_REGU = 	13,
IND_MENU1_STAT = 	14,
IND_MENU1_ARCH = 	15,
IND_MENU1_AIDE = 	16
} tg_BoutonMenu1;
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#define TAILLE_NOM_MASQUE	6

typedef char	GMOT_NomMasque [TAILLE_NOM_MASQUE];

extern int affiche_liste_dans_widget (Widget 		va_Widget, 
				      char**		va_Tableau,
				      int		va_DimTab,
				      int 		va_TypeTab);

extern int ouvre_hierarchie	( tg_ouverture	va_FlagClose);
/*XDY_Langue 	va_LangueUtil,*/ 

extern void busy_cursor( Widget );
extern void unbusy_cursor( Widget );

extern int met_image_dans_menu (tg_InfosMenu* 	pa_Menu, 
				char** 		va_TableauImage);

extern int grise_menu_principal	(tg_ClasseAutorisation 	va_ClasseAutorisation,
				 tg_InfosMenu          	va_menu1 );

extern void grise_tout_menu_principal( tg_InfosMenu          	va_menu1 );

extern void active_tout_menu_principal( tg_InfosMenu          	va_menu1 );

extern int construit_message	(Widget va_Parent,
				 int va_NumeroMessage,
				 ...);

extern int affiche_message	(Widget 	va_Parent,
				 char* 		va_Message);

extern int get_fenetre_mere (Widget 		va_Widget, 
			     tg_InfosFenetres** 	pa_Infos);

 extern void get_display_unix (	Widget                    va_Widget,
    				XDY_DisplayUnix           pa_Display);

extern void PostPopup( 	Widget          va_Widget, 
			XtPointer       va_client_data,
			XButtonEvent    *va_event );

#endif
