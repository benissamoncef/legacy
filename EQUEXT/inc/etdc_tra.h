/*E*/
/*Fichier : $Id: etdc_tra.h,v 1.8 2009/10/08 17:46:44 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2009/10/08 17:46:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETDC * FICHIER etdc_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de traitement de la
*   tache TETDC.
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	20 Dec 1994	: Creation
* Volcic.F	version 1.2	02 Fev 1995	: Modif valeur des cstes ETDCC_VRAI et ETDCC_FAUX
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete
* Volcic.F	version 1.4	20 Jun 1995	: Ajout definitions
* Volcic.F	version 1.5	27 Jun 1995	: Ajout definitions
* XXX		version 1.6	25 Aou 1995	: Changement nb. de variables
* XXX		version 1.7	27 Nov 1995	: Changement valeurs de types de messages
* JPL		08/10/09 : Dimensions structures donnees par constantes ERDT (pour DEM 909) 1.8
------------------------------------------------------ */

#ifndef etdc_tra
#define etdc_tra

/* fichiers inclus */

/*!#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>*/
#include "xdy.h"
#include "xdc.h"
#include "xdf.h"
#include "xdg.h"
#include "ex_msok.h"
#include "ex_mdon.h"
#include "xzaac.h"
#include "xzst.h"
#include "xzex.h"

#include "erdt_don.h"

#include "etdc_cal.h"

/* definitions de constantes exportees */

#define ETDCC_IMPOSSIBLE        -1
#define ETDCC_VRAI              XDC_VRAI
#define ETDCC_FAUX              XDC_FAUX
#define ETDCC_NB_VAR            248

#define ETDCC_SEMAIME_JOUR	0
#define ETDCC_WEEKEND_JOUR	1
#define ETDCC_FERIE_JOUR	2
#define ETDCC_SEMAIME_NUIT	3
#define ETDCC_WEEKEND_NUIT	4
#define ETDCC_FERIE_NUIT	5

#define ETDCC_MSG_RDT  		1
#define ETDCC_DERN_MSG_RDT  	0
#define ETDCC_MSG_CTXT  	9
#define ETDCC_SEP_FLAG  	"###"

#define ETDCC_pile 		100
#define ETDCC_registre 		10

/* definitions de types exportes */

typedef struct 
{
   char * nom;
   double valeur;

} ETDCT_VAR;

/* declarations de donnees globales */

extern ETDCT_VAR		pg_variables[];
extern XZEXT_MASK *		vg_SockMask;
extern float			vg_DIV[2];
extern int			vg_Cumul_Poids;
extern int			vg_Contexte;
extern int                	vg_Mode_Fonct;
extern XDY_NomSite		vg_NomSite;
extern XDY_EMes_RADT		pg_Mes_RADT[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int    et_traitement_trame ( int );

extern int    et_evaluer ( FILE *, ETDCT_VAR *, int, XDY_Horodate, XDY_TypeEqt, XDY_Eqt, XDY_Booleen );

#endif
