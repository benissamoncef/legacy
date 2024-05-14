/*E Fichier : $Id: asrv_rpcom.h,v 1.5 1994/12/16 10:00:44 mercier Exp $      Release : $Revision: 1.5 $        Date : $Date: 1994/12/16 10:00:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE RP * FICHIER ASRV_RPCOM.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	31 Aug 1994	: Creation
------------------------------------------------------ */

#ifndef RP
#define RP

/* fichiers inclus */
	#include        <stdio.h>
	#include        "ospublic.h"
	#include        "oserror.h"

	/* Include service migrazur */
	#include 	"xzst.h" /* Pour les traces */
	#include	"asrv_crp.h"
	#include        "asrv_arp.h"
	#include        "asrv_mrp.h"
	#include        "asrv_prp.h"
	#include        "asrv_rrp.h"
	#include        "asrv_trp.h"
	

/* definitions de constantes exportees */
#define RPC_FAIL        (CS_INT)SRV_MAXERROR+1  /* rpc failed           */
#define RPC_INFO        (CS_INT)SRV_MAXERROR+2  /* rpc failed           */
#define SHUTDOWN        (CS_INT)SRV_MAXERROR+3  /* shutdown du server   */
/*
** Shutdown password.
*/
#define DEFAULT_PASSWD          "yes"
#define c_OPSRV_NAME            "TASRV"

#define c_ERRMSG       "\tCT-Lib erreur: erreur(%d) severite(%d) OS erreur(%d)\n"
#define c_OSERRMSG	"\tCT-Lib OS Erreur: erreur(%d)\n"

#define c_ATTENTE_RTMSG 0.01 	/* Temps d attente de lecture de msg RTWks */
				/* sur chaque rp			   */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

CS_CHAR         Name[1024]; 		/*
					**  Name : Nom de l Open Serveur
					*/
CS_CHAR         SqlServername[1024];	/*
					**  SqlServername : Nom du SQL Server
					**  au quel on se connect.
					*/

CS_CONTEXT      *Context ;
					/*
					** Context         Open Server context
					*/

CS_INT          Ctcflags;               /* Context ct_debug flags. */
CS_INT          Conflags;               /* Connect ct_debug flags. */

/* delaration de fonctions externes */
extern CS_RETCODE asrv_rp_affiche   	(SRV_PROC        *);
extern CS_RETCODE asrv_start_handler	(SRV_SERVER      *);
extern int asrv_rp_shutdown         	(SRV_PROC        *);
extern int asrv_err_handler         	(CS_CONTEXT      *, CS_CLIENTMSG    *);
extern CS_INT asrv_syntax_error	(CS_CHAR   *);
extern CS_RETCODE asrv_server_err_handler (SRV_SERVER      	*,
					SRV_PROC        *,
					CS_INT,
					CS_INT,
					CS_INT,
					CS_INT,
					CS_CHAR 	*,
					CS_INT,
					CS_CHAR 	*,
					CS_INT);

extern CS_INT asrv_cterr (	CS_CONTEXT *,
				CS_CONNECTION   *,
				CS_CLIENTMSG    *);

extern CS_INT asrv_regparam_char(SRV_PROC        *,
				char 		*);
extern CS_INT asrv_regparam_int(SRV_PROC        *,
				char 		*);
extern CS_INT asrv_regparam_smallint(SRV_PROC   *,
				char 		*);
extern CS_INT asrv_regparam_tinyint(SRV_PROC    *,
				char 		*);
extern CS_INT asrv_regparam_date(SRV_PROC    *,
				char 		*);
extern CS_INT asrv_lire_smallint(SRV_PROC 	*,
				int		,
				CS_SMALLINT 	*);

extern CS_INT asrv_lire_tinyint(SRV_PROC 	*,
				int		,
				CS_TINYINT 	*);

extern CS_INT asrv_lire_int(	SRV_PROC 	*,
				int		,
				CS_INT 		*);

extern CS_INT asrv_lire_char(	SRV_PROC    	*,
				int             ,
				CS_CHAR 	*,
				int		);
extern int asrv_lire_RTSRV_MSG();
#endif
