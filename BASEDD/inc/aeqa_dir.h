/*E Fichier : $Id: aeqa_dir.h,v 1.1 1995/01/27 12:50:47 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/27 12:50:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE EQA * FICHIER aeqa_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module directeur de la tache TAEQA:
*  TAEQA est une tache qui se met en reception de message
*  et calcule les equations d'alerte
*
------------------------------------------------------
* HISTORIQUE :
*
* GABORIT B. 	25 jan 1995	: creation (1.1)
------------------------------------------------------ */

#ifndef AEQA
#define AEQA

/* fichiers inclus */

	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */
	#include        <ospublic.h>
	#include        <oserror.h>

	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsm.h"
	 #include "xzsc.h"
	 
	/* Include des CallBacks RTwks */ 
	 #include "aeqa_calc.h"
	 	 
/* definitions de constantes exportees */
		
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
#endif
