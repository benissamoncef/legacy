/*X Fichier : $Id: XZAG.h,v 1.1 1994/09/19 17:37:54 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/19 17:37:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER XZAG.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	16 Aug 1994	: Creation
------------------------------------------------------ */

#ifndef XZAG
#define XZAG

/* Constantes de correspondance entre nom machine et nom de SQL_Serveur */
/* present sur la machine						*/
#define DISTRIC_NICE_HOSTNAME "SDDN1"
#define XZAGC_SDDN1_SQL_SRV "SQL_DN"
#define XZAGC_SDCI1_SQL_SRV "SQL_CI"

#define XZAGC_USER	"MIGRAZUR"
#define XZAGC_PASSWD	"MIGRAZUR"

/*#define CS_STATIC CS_CONTEXT
#define CS_STATIC CS_CONNECTION
*/

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctpublic.h>

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* definition de macro exportees */
/* XZAGC_Hostname_SqlSrvName est un tableau de correspondance entre       */
/* le Hostname des machines et le nom du sql_Srv present sur le serveur   */
/* en question . En cas de rajoue d un site, NE PAS OUBLIER D INCREMENTER */
/* LA CONSTANTE XZAGC_NB_HostNme_SqlSrvNme				  */

#define XZAGC_NB_HostNme_SqlSrvNme 2
char *XZAGC_Hostname_SqlSrvName[][2]= {{"SDDN1","SQL_DN"},
				 {"SDCI1","SQL_CI"}}
#define XZAGC_USE_PRC_BASE "use prc\n"
#endif
