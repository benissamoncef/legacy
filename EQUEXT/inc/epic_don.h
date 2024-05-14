/*E*/
/*Fichier : $Id: epic_don.h,v 1.1 2023/08/17 14:28:17 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2023/08/17 14:28:17 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epic_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK	19/12/11		: Creation (DEM/483)
------------------------------------------------------ */

#ifndef epic_don
#define epic_don

/* fichiers inclus */


#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzez.h>
#include <xzamc.h>
#include <xzaac.h>
#include "ex_mlcr.h"
#include "ex_mala.h"
#include "ex_msta.h"
#include "etim_don.h"

/* definitions de constantes exportees */

/* A remettre dans xdf.h */

#define EPICC_REVEIL_10S	10
#define EPICC_TIMER_10S		"PIC_Timer_10s"
#define EPICC_REVEIL_1MN	60
#define EPICC_TIMER_1MN		"PIC_Timer_1mn"

#define EPICC_EQT_LIBRE		1
#define EPICC_EQT_OCCUP		2
#define EPICC_EQT_PANNE		3

#define EPICC_CONTRASTE		'S'


/* definitions de types exportes */

/* A remettre dans xdy.h */
typedef XDY_Mot	XDY_TypePIC;


typedef struct	{
	XDY_Num_Eqt	Numero;
	XDY_NomST	NumeroST;
	XDY_NoLS	NumeroLS;
	XDY_Rgs		AdresseRGS;
	XDY_District         SiteGestion;
	XDY_PortIP	Port;
	XDY_AdresseIP	AdresseIP;
		}	EPIC_CONFIG_PIC;
		
typedef struct	{
	EPIC_CONFIG_PIC	Config;
	int		Contraste;
	int		ContrasteCmd;
	int		NumCmd;
	int		NumDerCmd;
	XDY_Etat_PIC	Etat;
	XDY_Etat_PIC	Etat_Commande;
	int		Etat_Service;
	int		Mode;
	int		Socket;
	XDY_Mot		StatusTR;
	XDY_Mot		EtatTrans;
	XZEXT_TRAME_PS_NAV	Trame;
	XDY_Picto_PIC	AffDefaut[5];
	void *		Suivant;
		}	EPIC_DONNEES_PIC;

		

/* definition de macro exportees */

/* declarations de donnees globales */

extern EPIC_DONNEES_PIC		*pg_debutPIC;
extern int			vg_Mode_Fonct;
extern INFO_EQT_FEN_LCR		*pg_DebutListePICFLcr;
extern int			vg_SockTIM;
extern int			vg_Simulation;
extern int			vg_Contexte;
extern XDY_NomSite		vg_NomSite;
int vg_SockTEMIP;

char				vg_idpassw[18];
char				vg_clignolent[15];
char				vg_clignorapide[15];

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EPIC_DONNEES_PIC, ed_ins_liste_PIC )
EX_DECLA_SUPPRIMER ( ed_sup_liste_PIC )	

#endif
