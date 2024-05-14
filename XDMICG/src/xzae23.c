/*E*/
/*  Fichier : 	$Id: xzae23.c,v 1.7 2010/12/06 18:06:18 gesconf Exp $     Release : $Revision: 1.7 $        Date : $Date: 2010/12/06 18:06:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae23.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       31 Aug 1994     : Creation
* niepceron     03 Dec 1998     : XZAOC_XZAP23_... devient XZAOC_XZAE23_... v1.6
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : Cte. pour arg. 5 de 'ct_param' pour Sybase 15.x  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae23.h"

static char *version = "$Id: xzae23.c,v 1.7 2010/12/06 18:06:18 gesconf Exp $ : xzae23" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE23SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE23_Decrire_Bouchon_PA (	XDY_Horodate 	va_Horodate,
				XDY_Evt 	va_NumEvt,
				XDY_PR		*va_PRQueue_out,
				XDY_Octet	*va_Type_out,
				XDY_Entier	*va_Longueur_out,
				XDY_Evt		*va_NumEvtQueue )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate 	va_Horodate_in
* XDY_Evt 	va_NumEvt_in
*
* ARGUMENTS EN SORTIE :
*
* XDY_PR	va_PRQueue_out
* XDY_Entier	va_Longueur_out
* XDY_Octet	va_Type_out
* XDY_Evt	va_NumEvtQueue
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzae23.c,v 1.7 2010/12/06 18:06:18 gesconf Exp $ : E23_Decrire_Bouchon_PA" ;

	CS_CHAR *rpc_name =XZAOC_XZAE23_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAE23_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	XDY_Entier	va_NumEvt_out;
	XDY_Octet	va_CleEvt_out;
	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines au format Sybase
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;
	
	
	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : XZAE23_Decrire_Bouchon_PA : va_Horodate_in_days=%d va_Horodate_in_time=%d va_NumEvt_in=%d va_CleEvt_in=%d",va_Horodate_in.dtdays,va_Horodate_in.dttime,va_NumEvt_in,va_CleEvt_in);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	strcpy(datafmt.name ,"@va_Horodate_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_DATETIME_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Horodate_in,
			CS_SIZEOF(CS_DATETIME), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(int) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
        
        strcpy(datafmt.name ,"@va_NumEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_NumEvt_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(@va_NumEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_CleEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_CleEvt_in,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(@va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
        
        strcpy(datafmt.name ,"@va_PRQueue_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[0].pt_value = (CS_VOID *)va_PRQueue_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_PRQueue_out, 
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(va_PRQueue_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Type_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[1].pt_value = (CS_VOID *)va_Type_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Type_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(va_Type_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Longueur_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[2].pt_value = (CS_VOID *)va_Longueur_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Longueur_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(va_Longueur_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_NumEvt_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[3].pt_value = (CS_VOID *)&va_NumEvt_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_NumEvt_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(va_NumEvt_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_CleEvt_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[4].pt_value = (CS_VOID *)&va_CleEvt_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_CleEvt_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_param(va_CleEvt_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	/*A
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzae23: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae23 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae23 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae23 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE23_Decrire_Bouchon_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				

				/*
				** Les trois types de resultats sont fetchables.
				** Since the result model for rpcs and rows have
				** been unified in the New Client-Library, we
				** will use the same routine to display them
				*/
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						free(Tab_donnees2);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae23: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvtQueue->numero = va_NumEvt_out;
 	va_NumEvtQueue->cle    = va_CleEvt_out;
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE23_Decrire_Bouchon_PA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
