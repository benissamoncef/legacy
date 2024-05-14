/*E*/
/*Fichier : $Id: xzae213.h,v 1.1 2018/12/21 17:33:49 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/12/21 17:33:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE213 * FICHIER XZAE213.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL   17/09/18        Creation DEM1283 GABEB
------------------------------------------------------ */

#ifndef XZAE213
#define XZAE213

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
	#include "xzae.h"
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAE213C_RPC_NAME "XZAE213"

#define XZAE213C_NB_PARM_RETURN 4 


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE213_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE213_Fermeture_Echangeur (XDY_Octet	va_ech_in ,
					XDY_Booleen	*va_entree1_out,
					XDY_Booleen	*va_entree2_out,
					XDY_Booleen	*va_sortie1_out,
					XDY_Booleen	*va_sortie2_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_ech_in
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
