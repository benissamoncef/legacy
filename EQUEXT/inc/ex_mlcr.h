/*Fichier : $Id: ex_mlcr.h,v 1.16 2021/05/04 13:25:09 pc2dpdy Exp $      Release : $Revision: 1.16 $        Date : $Date: 2021/05/04 13:25:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mlcr.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  fenetre LCR.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	26 Oct 1994	: Creation
* Nagiel.E	version 1.2	08 Dec 1994	:
* Nagiel.E	version 1.3	16 Dec 1994	:
* Mismer.D	version 1.4	06 Jan 1995	:
* Mismer.D	version 1.5	10 Jan 1995	:
* Mismer.D	version 1.6	17 Jan 1995	:
* Mismer.D	version 1.7	22 Mai 1995	: Ajout ecriture fichier trace des commandes LCR
* Mismer.D	version 1.8	09 Jun 1995	: Modif ecriture fichier trace des commandes LCR
* Mismer.D	version 1.9	16 Aou 1995	: Ajout fonction
* Mismer.D      version 1.10    25 Mar 1997     : Modif pour TDP (DEM/1396)
* JMG		version 1.11	09 Jun 2009	: Acceptation de 9car pour le nom du LT DEM 887 
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : declaration fonction 'EnvoyerTrame'  1.12
* JMG		15/05/18 : ajout IP LCR 1.13
* JMG		27/02/19 : port devient portIP 1.14
* JMG		12/01/20 : ajoput IMU 1.15
* ABE		16/02/21 : Modification IMU DEM-SAE155 1.16
------------------------------------------------------ */

#ifndef ex_mlcr
#define ex_mlcr

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <xdy.h>
#include <xdc.h>
#include <xdf.h>
#include <xdl.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzex.h>
#include <xzec.h>
#include <xzss.h>
#include <xzsm.h>
#include "ex_mdon.h"

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

/* definitions de constantes exportees */

#define XZEXC_TRAME_FENETRE_LCR		20

/* definitions de types exportes */

typedef	struct	
{
	int		Numero;
	XDY_Nom		NomEqt;
	XDY_Nom		NomMachine;
	void *		Suivant;
}	EXT_NOM_EQT_LCR;

typedef struct	{
  		   XDY_TypeEqt		TypeEqt;
  		   XDY_Eqt		NoEqt;
		   char   		NomFichCmdesLCR[21]; 
		   char   		NomFichResLCR[21];
  		   XDY_Entier		Periode;
  		   XDY_Horodate		HrDebut;
  		   XDY_Horodate		HrFin;
  		   XDY_NomMachine	NomMachine;
  		   XDY_Entier		Socket;
  		   XDY_Rgs		AdresseRGS;
  		   XDY_Entier		Position;
  		   XDY_Mot		Indiv;
  		   XDY_Texte		TexteCmd;
  		   XZEXT_TRAME_PS_NAV   TexteReponse;
		   void *		Suivant;
		}INFO_EQT_FEN_LCR;

typedef struct	{
  		   XDY_TypeEqt		TypeEqt;
  		   XDY_Eqt		NoEqt;
		   char   		NomFichCmdesLCR[21]; 
		   char   		NomFichResLCR[21];
  		   XDY_Entier		Periode;
  		   XDY_Horodate		HrDebut;
  		   XDY_Horodate		HrFin;
  		   XDY_NomMachine	NomMachine;
  		   XDY_Entier		Socket;
  		   XDY_Rgs		AdresseRGS;
  		   XDY_Entier		Position;
  		   XDY_Mot		Indiv;
  		   XDY_Texte		TexteCmd;
  		   XZEXT_TRAME_PS_NAV   TexteReponse;
		   XDY_AdresseIP	AdresseIP;
		   XDY_PortIP		Port;
		   void *		Suivant;
		}INFO_EQT_FEN_LCR_IP;

/* definition de macro exportees */

/* declarations de donnees globales */

extern INFO_EQT_FEN_LCR	*pg_DebutListeEqt;
extern INFO_EQT_FEN_LCR_IP *pg_DebutListeEqt_IP;

/* declaration de fonctions externes */

extern int ex_cmd_LCR ( 		char  *, 
					XDY_NomMachine,
					XDY_TypeEqt,
					XDY_Eqt,
					XDY_Rgs,
					int,
					char  *,
					XDY_Entier *,
					char  * );
extern int ex_cmd_LCR_IP ( 		char  *, 
					XDY_NomMachine,
					XDY_TypeEqt,
					XDY_Eqt,
					XDY_Rgs,
					int,
					char  *,
					XDY_Entier *,
					char  *,
					XDY_AdresseIP,
					XDY_PortIP);

extern int EnvoyerTrame ( 		XZEXT_Trame,
					int,
					char *,
					int,
					int,
					int);

extern int EnvoyerTrameIP ( 		XZEXT_Trame,
					int,
					char *,
					int,
					char *,
					int,
					int,
					int);
extern int EnvoyerTrameOPC ( 		XZEXT_Trame,
					int,
					int,
					int,
					int,
					int);

extern int ex_ret_LCR ( 		char  *,
					XDY_Rgs,
					INFO_EQT_FEN_LCR **,
					int );
extern int ex_ret_LCR_IP ( 		char  *,
					XDY_Rgs,
					INFO_EQT_FEN_LCR_IP **,
					int );

extern int ex_ins_eqt_liste (		INFO_EQT_FEN_LCR * , INFO_EQT_FEN_LCR ** ) ;
extern int ex_ins_eqt_liste_IP (	INFO_EQT_FEN_LCR_IP * , INFO_EQT_FEN_LCR_IP ** ) ;

extern int ex_relance_lcr ( 		XDY_Eqt, 
					char,
			 		INFO_EQT_FEN_LCR **,
			 		XDY_Horodate, 
			 		XZEXT_CLE_PROG,
			 		int ); 
			 	
extern int ex_test_autorisation_eqt ( 	INFO_EQT_FEN_LCR **,
					INFO_EQT_FEN_LCR *,
					int );
				
extern void ex_init_lcr ( 		INFO_EQT_FEN_LCR **,
					INFO_EQT_FEN_LCR,
					int,
					XDY_Rgs,
					XDY_Eqt,
					char *,
					int );

extern int ex_nom_equipement (  	XDY_Eqt,
					XDY_Eqt,
					XDY_NomMachine,
					char *,
					char * ); 

extern int ex_ecriture_trace_cmd ( 	XDY_Eqt,
                            		XDY_Eqt,
                            		char *,
                            		char *,
                            		int );

extern int ex_ecriture_trace_alerte(      XDY_Eqt,
			XDY_Eqt,
			char *);
#endif
