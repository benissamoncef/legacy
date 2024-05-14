/*E Fichier : $Id: xzcg.h,v 1.10 1999/02/26 11:14:41 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1999/02/26 11:14:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE XZCG * FICHIER XZCG.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d interface de la tache tcgcd.x permettant d effectuer
* la gestion de conf (voir DCG-002/0.1)
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	12 Oct 1994	: Creation
*************
* MERCIER O.	05 Nov 1994	: Modif de XZCGC_TYPEF_GLOBAL 	V1.3
* "" -> "glob"
*************
* MERCIER	02 Jan 1995	: Ajout de xzcg_ChgmtBaseCour 	V 1.4
*
* T. Milleville	09 Jan 1995	: Modification da la structure 
*					XZCGT_ComposantBL V1.5
* P.Niepceron   08 Jun 1995	: Ajout de XZCGC_TYPEF_FINF v 1.6
* P.Niepceron   29 Jan 1996     : Ajout de XZCGC_TYPEF_DIE v 1.7
* P.Niepceron   05 Fev 1996     : Ajout de XZCGC_TYPEF_MOD v 1.8
* P.niepceron	27 Jan 1999	: Ajout de  XZCGC_TYPEF_STRA 1.9 dem/1728
------------------------------------------------------ */

#ifndef XZCG
#define XZCG

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* Include service migrazur */
	 #include "xdc.h"
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xdf.h"
	 #include "xzst.h"
	 #include "xzsc.h"

/* definitions de constantes exportees */

#define XZCGC_ERR_LECTURE	-1
#define XZCGC_ERR_TYPEF		-1
#define XZCGC_ERR_VERIF		-1

#define XZCGC_TYPEF_SCNT	"SCNT"
#define XZCGC_TYPEF_SNAV	"SNAV"
#define XZCGC_TYPEF_LPIC	"LPIC"
#define XZCGC_TYPEF_MPMV	"MPMV"
#define XZCGC_TYPEF_EQAL	"EQAL"
#define XZCGC_TYPEF_SYNP	"SYNP"
#define XZCGC_TYPEF_RSAD	"RSAD"
#define XZCGC_TYPEF_FAID	"FAID"
#define XZCGC_TYPEF_PIGN	"PIGN"
#define XZCGC_TYPEF_DIE         "FDIE"
#define XZCGC_TYPEF_MOD         "FMOD"
#define XZCGC_TYPEF_MFBO	"MFBO"
#define XZCGC_TYPEF_FADM	"FADM"
#define XZCGC_TYPEF_DORA	"DORA"
#define XZCGC_TYPEF_EQUI	"EQUI"
#define XZCGC_TYPEF_DEFT	"DEFT"
#define XZCGC_TYPEF_FINF        "FINF"
#define XZCGC_TYPEF_OPER	"OPER"
#define XZCGC_TYPEF_UTIL	"UTIL"
#define XZCGC_TYPEF_NTFM	"NTFM"
#define XZCGC_TYPEF_STRA	"STRA"

#define XZCGC_TYPEF_EXEC_BASEDD		"EXEC_BASEDD"
#define XZCGC_TYPEF_EXEC_EQUEXT		"EXEC_EQUEXT"
#define XZCGC_TYPEF_EXEC_GECONF		"EXEC_GECONF"
#define XZCGC_TYPEF_EXEC_GESIHM		"EXEC_GESIHM"
#define XZCGC_TYPEF_EXEC_MAINTE		"EXEC_MAINTE"
#define XZCGC_TYPEF_EXEC_SAIDEC		"EXEC_SAIDEC"
#define XZCGC_TYPEF_EXEC_SIMTST		"EXEC_SIMTST"
#define XZCGC_TYPEF_EXEC_SUPERV		"EXEC_SUPERV"
#define XZCGC_TYPEF_EXEC_XDMICG		"EXEC_XDMICG"
#define XZCGC_TYPEF_TRANSFERT_BASE	"TRS_CFG_CFT"


#define XZCGC_TYPEF_GLOBAL	"GLOBAL"
#define XZCGC_NOMF_GLOBAL	"GLOBAL"

/* Tableau de verification */

#define XZCGC_VERIF_SCNT		1
#define XZCGC_VERIF_SNAV		2
#define XZCGC_VERIF_DEFT		3
#define XZCGC_VERIF_SIHM		4
#define XZCGC_VERIF_RESE		5
#define XZCGC_VERIF_TYPE		6
#define XZCGC_VERIF_EQAL		7
#define XZCGC_VERIF_EQUI		8
#define XZCGC_VERIF_RSAD		9
#define XZCGC_VERIF_SGBD		10
#define XZCGC_VERIF_MACH		11
#define XZCGC_VERIF_GLOBAL		12
#define XZCGC_VERIF_TRANSFERTBASE 	13

#define XZCGC_NB_VERIF		13	/* doit contenir le nbre de type */
					/* de fichier a verifier : afin d allouer*/
					/* le tableau checklist en global correctement*/
/* Tableau des modifications */

#define XZCGC_MODIF_SGBD	20
#define XZCGC_MODIF_MACH	21	
#define XZCGC_MODIF_RESE	22	
#define XZCGC_MODIF_EQUI	23	
#define XZCGC_MODIF_TYPE	24	
#define XZCGC_MODIF_SIHM	25	

/* type d action restauration sauvegarde */

#define XZCGC_SAUVEGARDE_CFG		1
#define XZCGC_RESTAURATION_DANS_CFT	2

/* definitions de types exportes */

typedef struct {
	char TypeFic[20];		/*Type de fichier 	 */
	XDY_NomMachine NomMachine;	/*Nom de la machine	 */
	char NomFichier[20];		/*Nom du fichier	 */
	char NumVersion[7];		/*Numero de version	 */
	XDY_Horodate Horodate;		/*Horodate de mise a jour*/
} XZCGT_ComposantBL;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int xzcg01_Livraison ();
extern int xzcg02_Lecture_BL(XDY_FonctionInt, int *);
extern int xzcg03_Diffusion_Fichiers ();
extern int xzcg04_Verification_Coherence(int);
extern int xzcg05_ModificationBD(int);
extern int xzcg06_DuplicationBD(int);
extern int xzcg07_EnvoieNomMachineLocal () ;
extern int xzcg_ActivChecklist (int ); 
extern int xzcg_ChgmtBaseCour (char *);


#endif
