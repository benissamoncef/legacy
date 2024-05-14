/*E Fichier : $Id: drol_fax.h,v 1.3 2015/09/01 08:46:14 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2015/09/01 08:46:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_FAX * FICHIER drol_fax.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fonctions de creation des propositions de fax.
*
------------------------------------------------------
* HISTORIQUE :
* JMG	31/01/01	creation
* JPL	31/03/09 : Ajout arguments d'evenement a 'drol_fax_elabore' (DEM 411) 1.2
* JMG	14/08/15 DEM1131 interface drol_fax_elabore 1.3
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
#include "xzap07.h"
#include "xzap09.h"
#include "drol_appel.h"

/* definition de macro exportees */

/* declarations de donnees globales */

extern T_NOEUD *vg_racine;
extern XDY_Mot	vg_numero_plan;
extern XDY_Mot	vg_ordre_fax;
extern XDY_District	vg_site;

/* delaration de fonctions externes */
extern int drol_fax_init();
extern char *drol_fax_genere(T_NOEUD *);
extern char *drol_fax_genere_police(T_NOEUD *);
extern char *drol_fax_elabore(XDY_District, XDY_Evt, int, char *);
extern void drol_fax_elabore_police(T_NOEUD *);
