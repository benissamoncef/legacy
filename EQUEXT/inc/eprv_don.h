/*E*/
/*Fichier : $Id: eprv_don.h,v 1.1 2012/07/04 14:28:17 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:28:17 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
* ABE	31/05/21		: PRV suur IP DEM-SAE283 1.2
------------------------------------------------------ */

#ifndef eprv_don
#define eprv_don

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

#define EPRVC_REVEIL_10S	10
#define EPRVC_TIMER_10S		"PRV_Timer_10s"
#define EPRVC_REVEIL_1MN	60
#define EPRVC_TIMER_1MN		"PRV_Timer_1mn"

#define EPRVC_EQT_LIBRE		1
#define EPRVC_EQT_OCCUP		2
#define EPRVC_EQT_PANNE		3

#define EPRVC_CONTRASTE		'S'


/* definitions de types exportes */

/* A remettre dans xdy.h */
typedef XDY_Mot	XDY_TypePRV;


typedef struct	{
	XDY_Num_Eqt	Numero;
	XDY_NomST	NumeroST;
	XDY_NoLS	NumeroLS;
	XDY_Rgs		AdresseRGS;
	XDY_Eqt		VitesseNominale;
	XDY_Eqt		Zone;
	XDY_Eqt		Critique;
	XDY_District         SiteGestion;
	XDY_PortIP	Port;
	XDY_AdresseIP	AdresseIP;
		}	EPRV_CONFIG_PRV;
		
typedef struct	{
	EPRV_CONFIG_PRV	Config;
	int		Contraste;
	int		ContrasteCmd;
	int		NumCmd;
	int		NumDerCmd;
	XDY_Etat_PRV	Etat;
	XDY_Etat_PRV	Etat_Commande;
	int		Etat_Service;
	int		Mode;
	int		Socket;
	XDY_Mot		StatusTR;
	XDY_Mot		EtatTrans;
	XZEXT_TRAME_PS_NAV	Trame;
	XDY_Ligne_PRV	AffDefaut[5];
	void *		Suivant;
		}	EPRV_DONNEES_PRV;

		

/* definition de macro exportees */

/* declarations de donnees globales */

extern EPRV_DONNEES_PRV		*pg_debutPRV;
extern int			vg_Mode_Fonct;
extern INFO_EQT_FEN_LCR		*pg_DebutListePRVFLcr;
extern int			vg_SockTIM;
extern int			vg_Simulation;
extern int			vg_Contexte;
extern XDY_NomSite		vg_NomSite;
int vg_SockTEMIP;

char				vg_idpassw[18];
char				vg_clignolent[15];
char				vg_clignorapide[15];

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EPRV_DONNEES_PRV, ed_ins_liste_PRV )
EX_DECLA_SUPPRIMER ( ed_sup_liste_PRV )	

#endif
