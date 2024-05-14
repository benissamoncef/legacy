/*E*/
/*Fichier : $Id: epmv_don.h,v 1.18 2019/01/29 11:07:09 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2019/01/29 11:07:09 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	06 Sep 1994	: Creation
* Mismer.D			28 Sep 1994	: Suppression des #define XDF
* Mismer.D	version 1.2	13 Dec 1994	:
* Nagiel.E	version 1.3	16 Dec 1994	:
* Mismer.D	version 1.4	23 Jan 1995	:
* Mismer.D	version 1.5	09 Fev 1995	:
* Mismer.D	version 1.6	08 Mar 1995	:
* Mismer.D	version 1.7	07 Avr 1995	:
* Volcic.F	version 1.8	19 Avr 1995	: 
* Fontaine.C	version 1.9	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.10	30 Mai 1995	: Modif pour affichage par defaut en fichier
* Mismer.D	version 1.11	16 Aou 1995	: Ajout numero de la derniere commande dans la structure equipement
* Mismer.D      version 1.12    29 Aou 1996     : Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* Mismer.D      version 1.13    13 Sep 1996     : Modif pour eclairage et periode (RADT_W)
* Mismer.D	version 1.14	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D      version 1.15    10 Mar 1998     : Modif pour PMV PICTO avec la meme RGS (DEM/1618)
* JMG   31/10/07        : ajout site de gestion 1.16
* JPL	07/12/17	: Ajout du champ Trame dans la structure des donnees d'un PMV (DEM 1258)  1.17
* JMG	02/08/18	: ajout picto dans typ pmv suppression SAGA DEM1306 1.18
------------------------------------------------------ */

#ifndef epmv_don
#define epmv_don

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

#define EPMVC_REVEIL_10S	10
#define EPMVC_TIMER_10S		"PMV_Timer_10s"
#define EPMVC_REVEIL_1MN	60
#define EPMVC_TIMER_1MN		"PMV_Timer_1mn"

#define EPMVC_EQT_LIBRE		1
#define EPMVC_EQT_OCCUP		2
#define EPMVC_EQT_PANNE		3

#define EPMVC_CONTRASTE		'S'

#define EPMVC_PMV_1LIGNES	1
#define EPMVC_PMV_2LIGNES	2
#define EPMVC_PMV_3LIGNES	3
#define EPMVC_PMV_4LIGNES	4

/* definitions de types exportes */

/* A remettre dans xdy.h */
typedef XDY_Mot	XDY_TypePMV;
typedef XDY_Mot	XDY_TypePicto;

typedef struct	{
	XDY_Num_Eqt	Numero;
	XDY_NomST	NumeroST;
	XDY_NoLS	NumeroLS;
	XDY_Rgs		AdresseRGS;
	XDY_TypePMV	TypePMV;
	XDY_Mot		Eqt_PMV_Picto;
	XDY_District    SiteGestion;
	int		Port;
	XDY_AdresseIP	AdresseIP;
	XDY_Octet	TypeLCR;
	char		IdLCR[50];
		}	EPMV_CONFIG_PMV;
		
typedef struct	{
	EPMV_CONFIG_PMV	Config;
	int		Contraste;
	int		ContrasteCmd;
	int		NumCmd;
	int		NumDerCmd;
	XDY_Etat_PMV	Etat;
	XDY_Etat_PMV	Etat_Commande;
	int		Etat_Service;
	int		Mode;
	int		Socket;
	XDY_Mot		StatusTR;
	XDY_Mot		EtatTrans;
	XZEXT_TRAME_PS_NAV	Trame;
	XDY_Ligne_PMV	AffDefaut[4];
	void *		Suivant;
		}	EPMV_DONNEES_PMV;

typedef struct	{
	XDY_TypePMV	TypePMV;
	int		NbLigne;
	int		NbCaracteres;
	XDY_Booleen	Flash;
	XDY_Booleen	Temperature;
	XDY_Booleen	Picto;
	void *		Suivant;
		}	EPMV_DONNEES_TYPE_PMV;

typedef struct	{
	XDY_Num_Eqt	Numero;
	XDY_NomST	NumeroST;
	XDY_NoLS	NumeroLS;
	XDY_Rgs		AdresseRGS;
	XDY_TypePicto	TypePicto;
	XDY_Mot		Eqt_PMV_Picto;
	XDY_District         SiteGestion;
	int             Port;
       XDY_AdresseIP   AdresseIP;
	XDY_Octet	TypeLCR;
	char		IdLCR[50];
		}	EPMV_CONFIG_Picto;
		
typedef struct	{
	EPMV_CONFIG_Picto	Config;
	int		Contraste;
	int		ContrasteCmd;
	int		NumCmd;
	int		NumDerCmd;
	XDY_Etat_Picto	Etat;
	XDY_Etat_Picto	Etat_Commande;
	int		Etat_Service;
	int		Mode;
	int		Socket;
	XDY_Mot		StatusTR;
	XDY_Mot		EtatTrans;
	void *		Suivant;
		}	EPMV_DONNEES_Picto;

typedef struct  {
	XDY_Picto	Nom_Picto;
	void *		Suivant;
		}	EPMV_LISTE_Picto;

typedef struct	{
	XDY_TypePicto		TypePicto;
	int			NbCaracteres;
	EPMV_LISTE_Picto *	Liste;
	void *			Suivant;
		}	EPMV_DONNEES_TYPE_Picto;
		

/* definition de macro exportees */

/* declarations de donnees globales */

extern EPMV_DONNEES_PMV		*pg_debutPMV;
extern EPMV_DONNEES_TYPE_PMV	*pg_debutTypePMV;
extern EPMV_DONNEES_Picto	*pg_debutPicto;
extern EPMV_DONNEES_TYPE_Picto	*pg_debutTypePicto;
extern int			vg_Mode_Fonct;
extern INFO_EQT_FEN_LCR		*pg_DebutListePMVFLcr;
extern INFO_EQT_FEN_LCR		*pg_DebutListePictoFLcr;
extern int			vg_SockTIM;
extern int			vg_Simulation;
extern int			vg_Contexte;
extern XDY_NomSite		vg_NomSite;

char				vg_idpassw[18];
char				vg_clignolent[15];
char				vg_clignorapide[15];

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EPMV_DONNEES_PMV, ed_ins_liste_PMV )
EX_DECLA_SUPPRIMER ( ed_sup_liste_PMV )	
EX_DECLA_INSERER   ( EPMV_DONNEES_Picto, ed_ins_liste_Picto )
EX_DECLA_SUPPRIMER ( ed_sup_liste_Picto )	
EX_DECLA_INSERER   ( EPMV_DONNEES_TYPE_PMV, ed_ins_liste_TypePMV )
EX_DECLA_SUPPRIMER ( ed_sup_liste_TypePMV )	
EX_DECLA_INSERER   ( EPMV_DONNEES_TYPE_Picto, ed_ins_liste_TypePicto )
EX_DECLA_SUPPRIMER ( ed_sup_liste_TypePicto )	

#endif
