/*E*/
/*Fichier : $Id: xzae158.h,v 1.1 2021/05/03 14:51:16 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 14:51:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE158 * FICHIER XZAE158.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	15/04/21	: Creation DEM-SAE130 1.1
------------------------------------------------------ */

#ifndef XZAE158
#define XZAE158

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
#define XZAE158C_RPC_NAME "XZAE158"

#define XZAE158C_NB_PARM_RETURN 2


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE158_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE158_Date_prevue (	XDY_Entier  	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Horodate	*va_debut_prevu_out,
					XDY_Horodate	*va_fin_prevu_out);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
*				
* PARAMETRES EN SORTIE :
* XDY_Horodate		va_debut_prevu_out
* XDY_Horodate		va_fin_prevu_out
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
