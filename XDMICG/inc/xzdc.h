/*E Fichier : $Id: xzdc.h,v 1.2 1995/01/11 18:30:27 thomas Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/11 18:30:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZDC_coherence * FICHIER XZDC_coherence.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  verification de coherence entre les donnees utilisees
*  par SAIDEC et les donnees definies dans BASEDD
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Jan 1995	: Creation
* IT	11 Jan 1995	: Ajout de CARACTERE_DE_SEPARATION
------------------------------------------------------ */

#ifndef XZDC
#define XZDC

/* fichiers inclus */
#include "xdc.h"
#include "xdy.h"
#include "xzst.h"

/* definitions de constantes exportees */
#define CARACTERE_DE_SEPARATION		'_'

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZDC01_LireAstreintesSAD();

#endif
