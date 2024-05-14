/* Fichier : ecfe_don.h
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
* JPL   11/07/23 :      Ajout mode de fonctionnement aux donnees CFE (SAE-440)  1.2
*
------------------------------------------------------ */

#ifndef ecfe_don
#define ecfe_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzeb.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "ecfe_cfg.h"
#include "ecfe_cyc.h"
#include "ex_mlcr.h"


/* definitions de constantes exportees */

#define ECFEC_REVEIL_10S	9
#define ECFEC_TIMER_10S		"CFE_Timer_10s"
#define ECFEC_REVEIL_1MN	60
#define ECFEC_TIMER_1MN		"CFE_Timer_1mn"

#define ECFEC_EQT_LIBRE		1
#define ECFEC_EQT_OCCUP		2
#define ECFEC_EQT_PANNE		3


#define	ECFEC_PATH_CONFIG_CFE	"../../nav"
#define ECFEC_POS_MODULE	5

/* definitions de types exportes */


typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_AdresseIP		AdresseIP;
   		   XDY_Port		Port;
		XDY_District         SiteGestion;
		}ECFE_CONFIG_CFE;
		
		
typedef struct	{
		   ECFE_CONFIG_CFE	Config;
		   int			Etat;
		   XDY_Octet		Mode_Fct;
		   int	EtatCmd;
		   int			Etat_Service;
  		   int			Socket;
		   XDY_Mot		StatusTR;
		   XDY_Mot		EtatTrans;
   		   int			NumCmd;
	           void *		Suivant;
		   XDY_LibellePlanDeFeux		Libelle;
		}ECFE_DONNEES_CFE;

typedef struct {
	int	Numero;
	XDY_LibellePlanDeFeux	Libelle;
} T_Plan;

/* definition de macro exportees */

/* declarations de donnees globales */

extern ECFE_DONNEES_CFE		*pg_debutCFE;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern INFO_EQT_FEN_LCR		*pg_DebutListeCFEFLcr;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ECFE_DONNEES_CFE, ed_ins_liste_CFE )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_CFE )	

#endif
