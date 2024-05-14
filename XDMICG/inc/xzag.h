/*X Fichier :  $Id: xzag.h,v 1.10 1997/04/11 10:33:22 vathelot Exp $	      Release : $Revision: 1.10 $       Date : $Date: 1997/04/11 10:33:22 $
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
**********************
* mercier	06 Oct 1994	: modification 
*		mise a jour des fichiers d include et des
*		flags sccs.
******************
* MERCIER	10 Jan 1995 	: Ajout de xzag01 	V 1.7
* MERCIER	13 Sep 1995 	: Modif de xzag01 	V 1.8
*                                 et ajout xzag03, xzag04, 
*                                 xzag07, xzag08, xzag09, xzag40
* Niepceron	09 Oct 1996	: Ajout XZAG_SANS_PARM_RETURN et XZAG;50(dem/1227) v1.9
* Vathelot	11 Avr 1997	: Ajout xzag100
------------------------------------------------------ */

#ifndef XZAG
#define XZAG


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
	#include "asql_com.h"

	#include "xzagc.h"

/* definitions de constantes exportees */

#define XZAG_SANS_PARM_RETURN	0

#define XZAG50C_RPC_NAME  	"XZAG;50"


#define XZAG02C_RPC_NAME  	"XZAG;02"
#define XZAG02C_NB_PARM_RETURN  0
#define XZAG03C_RPC_NAME  	"XZAG;03"
#define XZAG03C_NB_PARM_RETURN  0
#define XZAG04C_RPC_NAME  	"XZAG;04"
#define XZAG04C_NB_PARM_RETURN  0
#define XZAG07C_RPC_NAME  	"XZAG;07"
#define XZAG07C_NB_PARM_RETURN  0
#define XZAG08C_RPC_NAME  	"XZAG;08"
#define XZAG08C_NB_PARM_RETURN  0
#define XZAG09C_RPC_NAME  	"XZAG;09"
#define XZAG09C_NB_PARM_RETURN  0
#define XZAG12C_RPC_NAME  	"XZAG;12"
#define XZAG12C_NB_PARM_RETURN  0
#define XZAG40C_RPC_NAME  	"XZAG;40"
#define XZAG40C_NB_PARM_RETURN  0
#define XZAG100C_RPC_NAME  	"XZAG;100"
#define XZAG100C_NB_PARM_RETURN  0


/* definitions de types exportes */

typedef char	XZAGT_Volume[7];
typedef char	XZAGT_Bande[18];

typedef CS_CONNECTION XZAT_Connexion;
typedef CS_CONTEXT    XZAT_Context;

/* definition de macro exportees */

/* declarations de donnees globales */

XZAT_Connexion *XZAGV_Connexion;
XZAT_Context   *XZAGV_Context;

/* delaration de fonctions externes */
extern int XZAG01_Transfert_Mois (XDY_Mot , XDY_Mot);
extern int XZAG02_Connexion_Base (char *);
extern int XZAG03_Deconnexion_Base();
extern int XZAG04_Arret_Base(XDY_NomSite);
extern int XZAG08_Sauvegarder_Base  (XDY_NomSite , char *, XZAGT_Volume ,
				char *, char *, char *, XDY_Entier, 
				XZAGT_Bande, char *);
extern int XZAG09_Restaurer_Base  (XDY_NomSite , char *, XZAGT_Volume ,
				char *, char *, char *, 
				XZAGT_Bande, char *);
extern int XZAG_execute_cmd(CS_CONNECTION *,CS_CHAR *);
extern int XZAG100_Calcul_RES_SEG_PR_Fin(XDY_Basedd);

/* definition de macro exportees */
/* XZAGC_Hostname_SqlSrvName est un tableau de correspondance entre       */
/* le Hostname des machines et le nom du sql_Srv present sur le serveur   */
/* en question . En cas de rajoue d un site, NE PAS OUBLIER D INCREMENTER */
/* LA VARIABLE XZAGC_NB_HostNme_SqlSrvNme se trouvant dans xzag.c*/

#define XZAGC_PREFIXE_SQLSRVNAME "SQL_"
#define XZAGC_USE_PRC_BASE "use PRC\n"
#define XZAGC_USER   "MIGRAZUR"
#define XZAGC_PASSWD "MIGRAZUR"


#endif
