/*E*/
/* Fichier : $Id: ebaf_don.h,v 1.4 2020/02/13 09:32:53 pc2dpdy Exp $     Release : $Revision: 1.4 $       Date : $Date: 2020/02/13 09:32:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAF * FICHIER ebaf_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
* JMG   31/10/07        : ajout site de gestion 1.2
* PNI	03/02/09	: ajout EDF v1.3 DEM788
* JMG	01/06/19 	: passage IP DEM1332 1.4
------------------------------------------------------ */


#ifndef ebaf_don
#define ebaf_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "ebaf_cfg.h"
#include "ebaf_cyc.h"
#include "ex_mlcr.h"

/* definitions de constantes exportees */

#define EBAFC_REVEIL_10S	10
#define EBAFC_TIMER_10S		"BAF_Timer_10s"
#define EBAFC_REVEIL_1MN	60
#define EBAFC_TIMER_1MN		"BAF_Timer_1mn"
#define EBAFC_REVEIL_5MN	300
#define EBAFC_TIMER_5MN		"BAF_Timer_5mn"


#define EBAFC_EQT_LIBRE		1
#define EBAFC_EQT_OCCUP		2
#define EBAFC_EQT_PANNE		3


#define	EBAFC_PATH_CONFIG_BAF	"../../nav"

/* definitions de types exportes */


typedef struct  {
   		   XDY_Eqt		Numero;
		   XDY_Mot		NoModule;
   		   char			Module[5];
		   int			Etat_Service;
		}	EBAF_LISTE_MODULE;
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_Mot		Type;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NumeroLS;
   		   XDY_Rgs		AdresseRGS;
   		   EBAF_LISTE_MODULE	Liste_Module[XZEXC_nb_module_max];
			XDY_District         SiteGestion;
			XDY_PortIP	Port;
			XDY_AdresseIP	AdresseIP;
		}EBAF_CONFIG_BAF;
		
typedef struct	{
		   XDY_NomPicto		Module[XZEXC_nb_module_max];
		   XDY_Scenario		Scenario;
		}EBAF_ETAT_BAF;
		
typedef struct	{
		   EBAF_CONFIG_BAF	Config;
		   EBAF_ETAT_BAF	Etat;
		   EBAF_ETAT_BAF	EtatCmd;
		   int			Etat_Service;
		   XZEXT_TRAME_PS_BAF	TramePS;
		   XZEXT_TRAME_PS_BAF	TrameST;
		   int			Contraste;
		   int			ContrasteCmd;
		   int			ContrasteEtat;
 		   int			Mode[4];
  		   int			Socket;
		   XDY_Mot		StatusTR;
		   XDY_Mot		EtatTrans;
		   XDY_Mot		Colision;
		   XDY_Mot		EDF;
   		   int			NumCmd;
   		   int			ArretUrg;
	           void *		Suivant;
		}EBAF_DONNEES_BAF;

/* definition de macro exportees */

/* declarations de donnees globales */

extern EBAF_DONNEES_BAF		*pg_debutBAF;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern INFO_EQT_FEN_LCR		*pg_DebutListeBAFFLcr;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
char			vg_idpassw[18];
char			vg_clignolent[15];
char			vg_clignorapide[15];
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EBAF_DONNEES_BAF, ed_ins_liste_BAF )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_BAF )		

#endif
