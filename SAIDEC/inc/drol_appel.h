/*E Fichier : $Id: drol_appel.h,v 1.5 2018/10/19 17:44:00 pc2dpdy Exp $     Release : $Revision: 1.5 $        Date : $Date: 2018/10/19 17:44:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DPAC_ARBRE * FICHIER drol_appel.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fonctions de creation et de parcours de l'appel des 
* evenements lies.
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation
* JMG	17/10/07	ajout mail
* JMG	06/12/16 : lot 23 1.2
* JMG	30/08/18 : SMS LOT25 DEM1301 1.5
* JPL	01/07/21 : Extension du champ "Remarque" a 250 caracteres (SAE-287)  1.6
------------------------------------------------------ */

/* fichiers inclus */
/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include        <stdlib.h>

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzao000.h"
#include "xzit.h"
#include "drol_tfm.h"
#include "xzap08.h"

/* declarations de macro exportees */
/* declarations de donnees globales */

typedef struct S_INTERLOCUTEUR {
	char         	Competence[50];
	XDY_Autoroute	Autoroute;
	XDY_PR          PR;
	int             Type_Astreinte;
	int	Appel_Fax;
	int	Appel_Tel;
	int	Appel_Mail;
	int	Appel_SMS;
	char		Justification[100];
	char		Remarque[251];
	XDY_Booleen	A_Appeler;
	XDY_Sens	Sens;
	struct S_INTERLOCUTEUR  *Suivant;
} T_INTERLOCUTEUR;


#define C_DROL_MAX_INTERL	50;


extern T_INTERLOCUTEUR 	*vg_interlocuteurs_pc;
extern T_INTERLOCUTEUR 	*vg_interlocuteurs_ci;
extern T_CONDITION 	*vg_conditions_ci;
extern T_CONDITION 	*vg_conditions_pc;
extern T_CAS		*vg_cas_ci;
extern T_CAS		*vg_cas_pc;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern XDY_Mot		vg_ordre_appel;

/* delaration de fonctions externes */
extern int drol_appel_init();
extern XDY_Booleen drol_appel_filtre(T_NOEUD *, XDY_District, XDY_Horodate, XDY_Booleen *);
extern void drol_appel_elabore(XDY_District);
extern void drol_appel_reinit();
