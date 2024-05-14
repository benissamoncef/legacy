/*E*/
/*Fichier : $Id: epma_don.h,v 1.5 2020/11/03 18:06:40 pc2dpdy Exp $  Release : $Revision: 1.5 $      Date : $Date: 2020/11/03 18:06:40 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG	25/10/05	ajout Trame dans structure PMVA 1.2
* JMG   31/10/07        : ajout site de gestion 1.3
* JMG	03/10/18	: ajuot IP suppression SAGA DEM1306 1.4
------------------------------------------------------ */

#ifndef epma_don
#define epma_don

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

#define EPMVAC_REVEIL_10S	10
#define EPMVAC_TIMER_10S		"PMVA_Timer_10s"
#define EPMVAC_REVEIL_1MN	60
#define EPMVAC_TIMER_1MN		"PMVA_Timer_1mn"

#define EPMVAC_EQT_LIBRE		1
#define EPMVAC_EQT_OCCUP		2
#define EPMVAC_EQT_PANNE		3

#define EPMVAC_CONTRASTE		'S'

#define EPMVAC_TYPE_ST		1

/* definitions de types exportes */

/* A remettre dans xdy.h */
typedef XDY_Mot	XDY_TypePMVA;


typedef struct	{
	XDY_Num_Eqt	Numero;
	XDY_NomST	NumeroST;
	XDY_NoLS	NumeroLS;
	XDY_Rgs		AdresseRGS;
		XDY_District         SiteGestion;
		XDY_AdresseIP		AdresseIP;
		XDY_PortIP	Port;
		XDY_Octet	Type;
		char		IdLCR[50];
		}	EPMVA_CONFIG_PMVA;
		
typedef struct	{
	EPMVA_CONFIG_PMVA	Config;
	int		Contraste;
	int		ContrasteCmd;
	int		NumCmd;
	int		NumDerCmd;
	XDY_Etat_PMVA	Etat;
	XDY_Etat_PMVA	Etat_Commande;
	int		Etat_Service;
	int		Mode;
	int		Socket;
	XDY_Mot		StatusTR;
	XDY_Mot		EtatTrans;
	XZEXT_TRAME_PS_NAV	Trame;
	XDY_Ligne_PMVA	AffDefaut[5];
	void *		Suivant;
		}	EPMVA_DONNEES_PMVA;

		

/* definition de macro exportees */

/* declarations de donnees globales */

extern EPMVA_DONNEES_PMVA		*pg_debutPMVA;
extern int			vg_Mode_Fonct;
extern INFO_EQT_FEN_LCR		*pg_DebutListePMVAFLcr;
extern int			vg_SockTIM;
extern int			vg_Simulation;
extern int			vg_Contexte;
extern XDY_NomSite		vg_NomSite;

char				vg_idpassw[18];
char				vg_clignolent[15];
char				vg_clignorapide[15];

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EPMVA_DONNEES_PMVA, ed_ins_liste_PMVA )
EX_DECLA_SUPPRIMER ( ed_sup_liste_PMVA )	

#endif
