/*E Fichier : $Id: drol_mail.h,v 1.1 2007/12/13 13:37:45 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 13:37:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DPAC_ARBRE * FICHIER drol_mail.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fonctions de creation et de parcours de l'mail des 
* evenements lies.
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation
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
#include "xzae200.h"
#include "xzit.h"
#include "xzap25.h"
#include "xzap43.h"
#include "xzap44.h"
#include "drol_appel.h"

/* defmailion de macro exportees */

/* declarations de donnees globales */

extern T_NOEUD *vg_racine;
extern XDY_Mot	vg_numero_plan;
extern XDY_Mot	vg_ordre_mail;
extern XDY_District	vg_site;

/* delaration de fonctions externes */
extern int drol_mail_init();
extern char *drol_mail_genere(T_NOEUD *);
extern char *drol_mail_genere_police(T_NOEUD *);
extern void drol_mail_elabore(XDY_District);
extern void drol_mail_elabore_police(T_NOEUD *);
