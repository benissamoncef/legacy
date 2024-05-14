/*E*/
/*  Fichier : 	$Id: asql_com.h,v 1.24 2010/12/03 17:16:38 gesconf Exp $      Release : $Revision: 1.24 $        Date : $Date: 2010/12/03 17:16:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER asql_com.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	18 Aug 1994	: Creation
* volcic	07 Nov 1994	: Ajout ctparam_bit_output (v1.20)
***********************
* MERCIER	10 Nov 1994	: Ajout de asql_cmd_alloc	V 1.21
*			et de asql_clean_cmd
* JPL		01/12/10 : Migration architecture HP ia64 (DEM 961) : declaration asql_fetch et type de status  1.23
* JPL		03/12/10 : Migration HP ia64 (DEM 961) : Ctes. pour arg 5 de ct_param (SMALLINT en V15, int avant) 1.24
------------------------------------------------------ */

#ifndef asql_com
#define asql_com

/* fichiers inclus */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <limits.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzen.h"
#include "xzep.h"


/* definitions de constantes exportees */

#define cg_Sp_Autoroute		CS_TINYINT_TYPE
#define cg_Sp_Pr		CS_FLOAT_TYPE
/*#define cg_Sp_Pr		CS_NUMERIC_TYPE*/
#define cg_Sp_Sens		CS_TINYINT_TYPE
#define cg_Sp_Numero		CS_SMALLINT_TYPE
#define cg_Sp_Eqt 		CS_SMALLINT_TYPE
#define cg_Sp_District		CS_TINYINT_TYPE
#define cg_Sp_Distance		CS_INT_TYPE
#define cg_MaxSizeBuf		1024

/* A partir de la version 15, le parametre "indicator" de ct_param est de type CS_SMALLINT     */
/* (conformement a la documentation), alors qu'il etait de type "int" auparavant.              */
/* De ce fait la valeur CS_UNUSED utilisee jusqu'alors (conformement aux exemples des manuels) */
/* etant de type CS_INT, devient trop grande. Utiliser la valeur 0 semble simple et approprie. */
/* CETTE DECLARATION SERAIT PRIVEE A asql_com SI AUCUN SOURCE N'UTILISAIT DIRECTEMENT ct_param */

#if defined (CS_VERSION_150)
#define	CGS_PARAM_INDIC		((CS_SMALLINT) 0)
#else
#define	CGS_PARAM_INDIC		(CS_UNUSED)
#endif


/* definitions de types exportes */

typedef CS_TINYINT		tg_Sp_Autoroute;
typedef CS_FLOAT		tg_Sp_Pr;
typedef CS_TINYINT 		tg_Sp_Sens;
typedef CS_SMALLINT	 	tg_Sp_Numero;
typedef CS_SMALLINT 		tg_Sp_Eqt;
typedef CS_TINYINT 		tg_Sp_District;
typedef CS_INT 			tg_Sp_Distance;

typedef CS_CONNECTION 		tg_Connexion;
typedef CS_CONTEXT    		tg_Context;

/* Le status de 'fetch' a vocation a etre un entier 4 octets comme les entiers Sybase */
/* soit un "int" en mode 64 bits, mais un "long" ailleurs pour compatibilite */
typedef CS_INT			tg_status_result;


/*
** Define structure where row data is bound.
*/
typedef struct _ex_column_data
{
	CS_SMALLINT     indicator;
	CS_CHAR         *value;
	CS_INT          valuelen;
} tg_Colonne_donnees;

typedef struct column_data2
{
	CS_SMALLINT     indicator;
	void		*pt_value;
	CS_INT 		valuelen;
}tg_Colonne_donnees2;

typedef tg_Colonne_donnees2 **tg_row_result;


/* definition de macro exportees */
#ifndef MAX
#define MAX(X,Y)      (((X) > (Y)) ? (X) : (Y))
#define MIN(X,Y)      (((X) < (Y)) ? (X) : (Y))
#endif


/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int asql_fetch_param_result(CS_COMMAND *,int ,tg_Colonne_donnees2 *);
extern int asql_fetch_data(CS_COMMAND *);
extern int asql_fetch(CS_COMMAND *, tg_status_result *);
extern int asql_display_dlen(CS_DATAFMT *);
extern int asql_display_header(CS_INT numcols,CS_DATAFMT columns[]);
extern int asql_date_Ux2Sybase(double , CS_DATETIME *);
extern int asql_date_Sybase2Ux(CS_DATETIME , double *);
extern int asql_char_Ux2Sybase_varchar(char *,CS_VARCHAR *);
extern int asql_Sybase_varchar2char_Ux(CS_VARCHAR,int,char *);
extern int asql_fetch_row_result(CS_COMMAND *,int *,int *,tg_row_result  *);
extern int asql_Free_Tab_Ligne_Results(int, int,tg_row_result *);
extern int asql_Affiche_Tab_Ligne_Results(int, int,tg_row_result);
extern int asql_Affiche_Col_Fct_Type(int, int, tg_Colonne_donnees2 );

extern CS_RETCODE asql_ctparam_bit_input(CS_COMMAND *, CS_BIT *,char *);
extern CS_RETCODE asql_ctparam_tinyint_input(CS_COMMAND *, CS_TINYINT *,char *);
extern CS_RETCODE asql_ctparam_smallint_input(CS_COMMAND *, CS_SMALLINT *,char *);
extern CS_RETCODE asql_ctparam_int_input(CS_COMMAND *, CS_INT *,char *);
extern CS_RETCODE asql_ctparam_date4_input(CS_COMMAND *, CS_DATETIME4 *,char *);
extern CS_RETCODE asql_ctparam_date_input(CS_COMMAND *, CS_DATETIME *,char *);
extern CS_RETCODE asql_ctparam_char_input(CS_COMMAND *, CS_CHAR *,char *);

extern CS_RETCODE asql_ctparam_tinyint_output(CS_COMMAND *, CS_TINYINT *,char *);
extern CS_RETCODE asql_ctparam_smallint_output(CS_COMMAND *, CS_SMALLINT *,char *);
extern CS_RETCODE asql_ctparam_int_output(CS_COMMAND *, CS_INT *,char *);
extern CS_RETCODE asql_ctparam_bit_output(CS_COMMAND *, CS_BIT *,char *);
extern CS_RETCODE asql_ctparam_date4_output(CS_COMMAND *, CS_DATETIME4 *,char *);
extern CS_RETCODE asql_ctparam_date_output(CS_COMMAND *, CS_DATETIME *,char *);
extern CS_RETCODE asql_ctparam_char_output(CS_COMMAND *, CS_CHAR *,char *);
extern CS_RETCODE asql_servermsg_cb(CS_CONTEXT *,CS_CONNECTION *,CS_SERVERMSG *);
extern CS_RETCODE asql_clientmsg_cb(CS_CONTEXT *,CS_CONNECTION *,CS_CLIENTMSG *);
extern CS_RETCODE asql_cmd_alloc(CS_CONNECTION *,CS_COMMAND **);
extern void 	  asql_clean_cmd(CS_CONNECTION *,CS_COMMAND *);
#endif
