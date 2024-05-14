/*E*/
/* Fichier : $Id: enav_don.h,v 1.19 2009/10/05 17:07:25 gesconf Exp $      Release : $Revision: 1.19 $        Date : $Date: 2009/10/05 17:07:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d≈crivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Nov 1994	: Creation
* Mismer.D	version 1.2	02 Dec 1994	:
* Nagiel.E	version 1.3	21 Dec 1994	:
* Nagiel.E	version 1.4	21 Dec 1994	: RAS
* Mismer.D	version 1.5	23 Jan 1995	:
* Mismer.D 	version 1.6	09 Fev 1995	:
* Mismer.D	version 1.7	08 Mar 1995	:
* Volcic.F	version 1.8	06 Avr 1995	:
* Mismer.D	version 1.9	09 Avr 1995	:
* Fontaine.C	version 1.10	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.11	24 Mai 1995	: Memorisation collision pour envoi alerte	
* Mismer.D	version 1.12	22 Sep 1995	: Ajout etat matrice nav	
* Volcic.F	version 1.13	12 Oct 1995	: Ajout timer cyclique 5 min 
* Mismer.D	version 1.14	12 Oct 1995	: Modif structure memo alerte connexion local (DEM/1136)
* Mismer.D	version 1.15	11 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
* Mismer.D	version 1.16	10 Jan 1997	: Modif pour matrice BRA (DEM/1419)
* Mismer.D	version 1.17	10 Jan 1998	: Modif pour FAU et FAC (DEM/1544)
* JMG   31/10/07        : ajout site de gestion 1.18
* PNI	03/02/09	: ajout EDF v1.19 DEM788
*
------------------------------------------------------ */

#ifndef enav_don
#define enav_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "enav_cfg.h"
#include "enav_cyc.h"
#include "ex_mlcr.h"

/* definitions de constantes exportees */

#define ENAVC_REVEIL_10S	10
#define ENAVC_TIMER_10S		"NAV_Timer_10s"
#define ENAVC_REVEIL_1MN	60
#define ENAVC_TIMER_1MN		"NAV_Timer_1mn"
#define ENAVC_REVEIL_5MN	300
#define ENAVC_TIMER_5MN		"NAV_Timer_5mn"


#define ENAVC_EQT_LIBRE		1
#define ENAVC_EQT_OCCUP		2
#define ENAVC_EQT_PANNE		3

#define ENAVC_MATRICE_NEUT	"NEUT"
#define ENAVC_MATRICE_PRES	"PRES"
#define ENAVC_CAR_SEP_MATRICE	","
#define ENAVC_NB_MATRICE	2

#define	ENAVC_PATH_CONFIG_NAV	"../../nav"

/* definitions de types exportes */


typedef struct  {
   		   XZEXT_Trame		Param;
		   XZEXT_Trame		Etat[ENAVC_NB_MATRICE];
        	   char            	Nom[ENAVC_NB_MATRICE][XDC_Lon_Matrice];
        	   XDY_Mot        	Conf[ENAVC_NB_MATRICE][XDC_Lon_Params_Matrice];
		}	ENAV_MATRICE;

typedef struct  {
   		   XDY_Eqt		Numero;
		   XDY_Mot		NoModule;
   		   char			Module[5];
		   int			Etat_Service;
		}	ENAV_LISTE_MODULE;
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_Mot		Type;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NumeroLS;
   		   XDY_Rgs		AdresseRGS;
   		   XDY_Mot		TypeNAV;
   		   ENAV_LISTE_MODULE	Liste_Module[XZEXC_nb_module_max];
		XDY_District         SiteGestion;
		}ENAV_CONFIG_NAV;
		
typedef struct	{
		   XDY_NomPicto		Module[XZEXC_nb_module_max];
		   XDY_Scenario		Scenario;
		}ENAV_ETAT_NAV;
		
typedef struct	{
		   ENAV_CONFIG_NAV	Config;
		   ENAV_ETAT_NAV	Etat;
		   ENAV_ETAT_NAV	EtatCmd;
		   int			Etat_Service;
		   XZEXT_TRAME_PS_NAV	TramePS;
		   XZEXT_TRAME_PS_NAV	TrameST;
		   int			Contraste;
		   int			ContrasteCmd;
		   int			ContrasteEtat;
 		   int			Mode[4];
  		   int			Socket;
		   XDY_Mot		StatusTR;
		   XDY_Mot		EtatTrans;
		   XDY_Mot		Colision;
		   XDY_Mot		EDF;
   		   ENAV_MATRICE		Matrice;
   		   int			NumCmd;
   		   int			ArretUrg;
	           void *		Suivant;
		}ENAV_DONNEES_NAV;

typedef struct  {
	short		NoSCEN;
	short		AutoFic;
	XDY_Scenario	Nom_SCEN;
	void *		Suivant;
		}	ENAV_LISTE_SCEN;

typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_Mot		Flash;
   		   XDY_Mot		NbBarrieres;
		   ENAV_LISTE_SCEN *	Liste;
	           void *		Suivant;
		}ENAV_DONNEES_TYPE_NAV;

typedef struct  {
		   char	PseudoScenar[5];
   		   char	NomScenar[10];
		}COMP;

/* definition de macro exportees */

/* declarations de donnees globales */

extern ENAV_DONNEES_NAV		*pg_debutNAV;
extern ENAV_DONNEES_TYPE_NAV	*pg_debutTypeNAV;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern INFO_EQT_FEN_LCR		*pg_DebutListeNAVFLcr;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
extern COMP			vg_CompScenar[XZEXC_NB_SCENAR_PREDEF];
char			vg_idpassw[18];
char			vg_clignolent[15];
char			vg_clignorapide[15];
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ENAV_DONNEES_NAV, ed_ins_liste_NAV )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_NAV )	
EX_DECLA_INSERER   ( ENAV_DONNEES_TYPE_NAV, ed_ins_liste_type_NAV )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_type_NAV )	

#endif
