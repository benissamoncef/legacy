/*E*/
/* Fichier : $Id: ebad_don.h,v 1.2 2007/11/19 18:01:08 pc2dpdy Exp $     Release : $Revision: 1.2 $       Date : $Date: 2007/11/19 18:01:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAD * FICHIER ebad_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
* JMG   31/10/07        : ajout site de gestion 12.
------------------------------------------------------ */


#ifndef ebad_don
#define ebad_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "ebad_cfg.h"
#include "ebad_cyc.h"
#include "ex_mlcr.h"

/* definitions de constantes exportees */

#define EBADC_REVEIL_10S	10
#define EBADC_TIMER_10S		"BAD_Timer_10s"
#define EBADC_REVEIL_1MN	60
#define EBADC_TIMER_1MN		"BAD_Timer_1mn"
#define EBADC_REVEIL_5MN	300
#define EBADC_TIMER_5MN		"BAD_Timer_5mn"


#define EBADC_EQT_LIBRE		1
#define EBADC_EQT_OCCUP		2
#define EBADC_EQT_PANNE		3


#define	EBADC_PATH_CONFIG_BAD	"../../nav"

/* definitions de types exportes */


typedef struct  {
   		   XDY_Eqt		Numero;
		   XDY_Mot		NoModule;
   		   char			Module[5];
		   int			Etat_Service;
		}	EBAD_LISTE_MODULE;
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_Mot		Type;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NumeroLS;
   		   XDY_Rgs		AdresseRGS;
   		   EBAD_LISTE_MODULE	Liste_Module[XZEXC_nb_module_max];
		   XDY_District		SiteGestion;
		}EBAD_CONFIG_BAD;
		
typedef struct	{
		   XDY_NomPicto		Module[XZEXC_nb_module_max];
		   XDY_Scenario		Scenario;
		}EBAD_ETAT_BAD;
		
typedef struct	{
		   EBAD_CONFIG_BAD	Config;
		   EBAD_ETAT_BAD	Etat;
		   EBAD_ETAT_BAD	EtatCmd;
		   int			Etat_Service;
		   XZEXT_TRAME_PS_BAD	TramePS;
		   XZEXT_TRAME_PS_BAD	TrameST;
		   int			Contraste;
		   int			ContrasteCmd;
		   int			ContrasteEtat;
 		   int			Mode[4];
  		   int			Socket;
		   XDY_Mot		StatusTR;
		   XDY_Mot		EtatTrans;
		   XDY_Mot		Colision;
   		   int			NumCmd;
   		   int			ArretUrg;
	           void *		Suivant;
		}EBAD_DONNEES_BAD;

typedef struct  {
	short		NoSCEN;
	short		AutoFic;
	XDY_Scenario	Nom_SCEN;
	void *		Suivant;
		}	EBAD_LISTE_SCEN;

typedef struct  {
		   char	PseudoScenar[5];
   		   char	NomScenar[10];
		}COMP;

/* definition de macro exportees */

/* declarations de donnees globales */

extern EBAD_DONNEES_BAD		*pg_debutBAD;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern INFO_EQT_FEN_LCR		*pg_DebutListeBADFLcr;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
extern COMP			vg_CompScenar[XZEXC_NB_SCENAR_PREDEF];
char			vg_idpassw[18];
char			vg_clignolent[15];
char			vg_clignorapide[15];
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EBAD_DONNEES_BAD, ed_ins_liste_BAD )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_BAD )		

#endif
