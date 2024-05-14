/*E Fichier : $Id: xzah03.h,v 1.2 1996/09/06 11:22:54 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/06 11:22:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH03 * FICHIER XZAH03.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	02 Nov 1994	: Creation
* niepceron	27 Aou 1996	: Ajout du site en arg  v1.2
------------------------------------------------------ */

#ifndef XZAH03
#define XZAH03

/* fichiers inclus */
	/* Include systeme */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <memory.h>
	
	/* Include Sybase */
	#include <ctpublic.h>
	#include <cstypes.h>

	/* Include applicatif Migrazur */
	#include "xdy.h"
	#include "xdc.h"
	#include "xzst.h"
	#include "xzsc.h"
	#include "xzag.h"
	#include "asql_com.h"
	#include "xzis.h"
	#include "xzahc.h"

/* definitions de constantes exportees */
#define XZAH03C_RPC_NAME "XZAH;03"
#define XZAH03C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH03_Envoyer_Voies_Neutralisees_Segment(XDY_Booleen,XDY_Nom);
#endif
