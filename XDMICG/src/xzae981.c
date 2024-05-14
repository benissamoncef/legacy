/*E*/
/*  Fichier : 	@(#)xzae981.c	1.2     Release : 1.2        Date :12/06/10
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae981.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* sdu       14/02/12     : Creation (DEM/1015)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae981.h"

static char *version = "@(#)xzae981.c	1.2 1.2 12/06/10";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE981SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE981_Update_Communautaire (
				XDY_Evt 	       va_NumEvt,
				XDY_Synthese_Commu va_Synthese,
				XDY_Tendance_Commu va_Tendance)
				

/*-----------------------------------------------------
* Arguments en entree
* XDY_Evt          va_NumEvt : FMC
* XDY_Tendance_Comm   va_Tendance  : tendance
* XDY_Synthese_Commu  va_Synthese  : synthese
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
*
* Conditions d'utilisation
*
-------------------------------------------------------*/
{

	static char *version = "@(#)xzae981.c	1.2 1.2 12/06/10" ;

	CS_CHAR *rpc_name =XZAEC_XZAE981_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection = XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE981_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines au format Sybase
	*/
	
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	
	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : XZAE981_Update_Communautaire : va_NumEvt=(%d/%d), va_Synthese=%d, va_Tendance=%d", va_NumEvt.numero, va_NumEvt.cle, va_Synthese, va_Tendance);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	**
	** @va_NumeroEvt_in   int ,
	** @va_CleEvt_in	tinyint	,
	** @va_tendance_in    int ,
	** @va_synthese_in    int
	**
	*/

	strcpy(datafmt.name ,"@va_NumeroEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&(va_NumEvt.numero),
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_param(@va_NumeroEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	strcpy(datafmt.name ,"@va_CleEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&(va_NumEvt.cle),
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_param(@va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	strcpy(datafmt.name ,"@va_tendance_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Tendance,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_param(@va_tendance_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	strcpy(datafmt.name ,"@va_synthese_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Synthese,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_param(@va_synthese_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae981: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae981 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae981 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae981 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE981_Update_Communautaire retourne 1");
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae981: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE981_Update_Communautaire retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
