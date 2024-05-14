/*E*/
/*Fichier : $Id: egtc_don.h,v 1.6 1996/12/05 22:34:39 mismer Exp $       Release : $Revision: 1.6 $     Date : $Date: 1996/12/05 22:34:39 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de donnees de la tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	16 Dec 1994	: Creation
* Volcic.F	version 1.2	09 Avr 1995	: Ajout EGTCC_INIT_ETAT_TOR
* Fontaine.C	version 1.3	19 Mai 1995	: Reprise entete
* Mismer.D	version 1.4	08 Aou 1995	: Utilisation de XDC_NBMAX_ETOR
* Mismer.D	version 1.5	10 Oct 1995	: Ajout etat de service ETOR
* Mismer.D	version 1.6 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
*
------------------------------------------------------ */

#ifndef egtc_don
#define egtc_don

/* fichiers inclus */


#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	<xzea.h>
#include 	<xzamc.h>
#include 	<xzaac.h>
#include 	"ex_mlcr.h"
#include 	"egtc_cfg.h"
#include 	"egtc_cmd.h"
#include 	"xzex.h"

/* definitions de constantes exportees */

#define EGTCC_REVEIL_20S	20
#define EGTCC_INIT_ETAT_TOR	2

/* definitions de types exportes */

/* A remettre dans xdy.h */

typedef struct	{
	XDY_Entier		Alarme;
	XDY_Entier		Alerte;
	XDY_Eqt			NoEqtETor;
	char			Nom[XDC_Lon_NomEqt];
	XDY_Entier		Etat;
	XDY_Horodate		Horodate;
	int			Etat_Service;
		}	EGTC_ETAT_GTC;
		
typedef struct	{
	XDY_Eqt			Numero;
	XDY_NomST		NumeroST;
	XDY_Mot			NumeroLS;
	XDY_Rgs			AdresseRGS;
  	int			Socket;
	int			Etat_Service;
	EGTC_ETAT_GTC		ETOR[XDC_NBMAX_ETOR];
		}	EGTC_DONNEES_GTC;
		
/* definition de macro exportees */

/* declarations de donnees globales */

extern EGTC_DONNEES_GTC		vg_donneesGTC;
extern int			vg_Mode_Fonct;
extern INFO_EQT_FEN_LCR		*pg_DebutListeGTCFLcr;
extern int			vg_SockTIM;
extern XDY_NomMachine		vg_NomMachine;
extern T_INT4			vg_SockRT;
extern XDY_NomSite        	vg_NomSite;
extern char			vg_idpassw[18];

/* delaration de fonctions externes */

#endif
