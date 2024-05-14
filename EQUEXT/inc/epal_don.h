/*E*/
/* Fichier : $Id: epal_don.h,v 1.4 2020/02/13 09:27:29 pc2dpdy Exp $     Release : $Revision: 1.4 $      Date :  $Date: 2020/02/13 09:27:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d≈crivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Jan 1998	: Creation
* Mismer.D      version 1.2     29 avr 2004     : Ajout pilotage par scenario
* JMG   31/10/07        : ajout site de gestion 1.3
* JMG	14/06/19	: IP 1.4
------------------------------------------------------ */

#ifndef epal_don
#define epal_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzeb.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "epal_cfg.h"
#include "epal_cyc.h"
#include "ex_mlcr.h"


/* definitions de constantes exportees */

#define EPALC_REVEIL_10S	10
#define EPALC_TIMER_10S		"PAL_Timer_10s"
#define EPALC_REVEIL_1MN	60
#define EPALC_TIMER_1MN		"PAL_Timer_1mn"

#define EPALC_EQT_LIBRE		1
#define EPALC_EQT_OCCUP		2
#define EPALC_EQT_PANNE		3


#define	EPALC_PATH_CONFIG_PAL	"../../nav"
#define EPALC_POS_MODULE	5

/* definitions de types exportes */


typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NumeroLS;
   		   XDY_Rgs		AdresseRGS;
   		   XDY_Mot		TypePAL;
   		   XDY_Etat_Module_PAL	Module[XDC_Lon_Nb_Mod_Pal];
		XDY_District         SiteGestion;
		XDY_AdresseIP		AdresseIP;
		XDY_PortIP		Port;
		}EPAL_CONFIG_PAL;
		
		
typedef struct	{
		   EPAL_CONFIG_PAL	Config;
		   XDY_Etat_Module_PAL	Etat[XDC_Lon_Nb_Mod_Pal];
		   XDY_Etat_Module_PAL	EtatCmd;
		   int			Etat_Service;
		   XZEXT_TRAME_PS_PAL	TramePS;
		   XZEXT_TRAME_PS_PAL	TrameST;
		   int			Contraste;
		   int			ContrasteCmd;
		   int			ContrasteEtat;
  		   int			Socket;
		   XDY_Mot		StatusTR;
		   XDY_Mot		EtatTrans;
   		   int			NumCmd;
	           void *		Suivant;
		}EPAL_DONNEES_PAL;



/* definition de macro exportees */

/* declarations de donnees globales */

extern EPAL_DONNEES_PAL		*pg_debutPAL;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern INFO_EQT_FEN_LCR		*pg_DebutListePALFLcr;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
char			vg_idpassw[18];
char			vg_clignolent[15];
char			vg_clignorapide[15];
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EPAL_DONNEES_PAL, ed_ins_liste_PAL )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_PAL )	

#endif
