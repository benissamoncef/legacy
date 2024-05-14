/*E*/
/* Fichier : $Id: egab_cyc.h,v 1.1 2018/06/19 17:00:48 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/06/19 17:00:48 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER egab_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de traitements cycliques
* de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
* JMG	15/05/18	creation 1.1
*
------------------------------------------------------ */

#ifndef egab_cyc
#define egab_cyc

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"egab_cfg.h"


/* definitions de constantes exportees */
#define EGABC_REVEIL_1MN     60
#define EGABC_TIMER_1MN               "GAB_Timer_1mn"

/* definitions de types exportes */

/* definition de macro exportees */
extern char                  vg_idpassw[18];

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
