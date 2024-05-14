
								/*E*/
/*Fichier :  $Id: xzat.h,v 1.18 2021/06/10 08:49:58 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/06/10 08:49:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER XZAT.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	10 Oct 1994	: Creation
* volcic	03 Nov 1994	: Ajout include (v1.3)
*				xzit05.h xzit06.h xzit07.h xzit08.h
* guilhou	27 aou 1996	: interface  xzat22, 23, 24, 25 (DEM/1172) v1.5
* Mismer	29 Jan 1998 	: Ajout compte rendu PAL (DEM/1539) v1.6
* JMG	27/04/04	ajout xzat280	1.7
* JMG	13/09/05	ajout PMVA BAF BAD 1.8
* JMG   31/10/07        : ajout site de gestion 1.9
* AAZ   30/08/2007 ajout SIG, ECL, VEN, ENR, INC DEM 662
* JMG	12/12/10	: portage linux 1.11
* VR	08/12/11	: Ajout equipement PRV 1.12 (DEM/1016)
* JMG	16/04/16 	: ajout xzat251 SAGA 1.14 
* LCL	22/02/18	: ajout CTRL_FEUX 1.15 DEM1284
* JMG	12/01/20	: ajout IMU 1.16
* ABE	02/04/21	: Ajout PAU SONO DEM-SAE244 1.18
------------------------------------------------------ */

#ifndef XZAT
#define XZAT

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */
#ifdef _HPUX_SOURCE
	#include        <ctpublic.h>
	/*#include        "oserror.h"*/
	 #include "asql_com.h"
#endif
	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsc.h"
	 #include "xzat05.h"
	 #include "xzat06.h"
	 #include "xzat07.h"
	 #include "xzat08.h"
	 #include "xzat51.h"
	 #include "xzat511.h"
	 #include "xzat280.h"
	 #include "xzat100.h"
	 #include "xzat150.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
												
/* delaration de fonctions externes */

#ifdef _HPUX_SOURCE
extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    *XZAGV_Context;
#endif

extern int xzat22_CompteRendu_Tube 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat23_CompteRendu_DemiEch (XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat24_CompteRendu_Nav 	(XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat25_CompteRendu_PMV 	(XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat26_Init_Etats_Eqts 	(XDY_Octet,XDY_Entier,XDY_NomMachine);
extern int xzat27_Init_Etats_EqtsIhm 	(XDY_Octet,XDY_Entier,XDY_Datagroup);
extern int xzat28_CompteRendu_Pal 	(XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);

extern int xzat250_CompteRendu_PMVA       (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat30_CompteRendu_BAF       (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat31_CompteRendu_BAD       (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat50_CompteRendu_PRV       (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat251_CompteRendu_Eqt_SAGA       (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat252_CompteRendu_CFE      (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat253_CompteRendu_IMU      (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District);
extern int xzat551_CompteRendu_RAU_SONO      (XDY_Entier,XDY_Horodate,XDY_Booleen,XDY_District,XDY_Mot,XDY_Mot);

/* AAZ 30/08/2007*/
extern int xzat40_CompteRendu_SIG 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat41_CompteRendu_ECL 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat42_CompteRendu_VEN 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat43_CompteRendu_ENR 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat44_CompteRendu_INC 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
extern int xzat45_CompteRendu_EXP 	(XDY_Entier,XDY_Horodate,XDY_Booleen);
/* AAZ */

#endif
