/*E*/
/*  Fichier : 	$Id: xzae30.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/06 18:06:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae30.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       07 Sep 1994     : Creation
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : Cte. pour arg. 5 de 'ct_param' pour Sybase 15.x  1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae30.h"

static char *version = "$Id: xzae30.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $ : xzae30" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE50SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE30_Decrire_Pb_Concessionnaire_PA (	XDY_Horodate 		va_Horodate,
						XDY_Evt 		va_NumEvt,
						XZAET_SousConcess 	*va_SousConcess )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate 		va_Horodate
* XDY_Evt 		va_NumEvt
*
* ARGUMENTS EN SORTIE :
*
* XZAET_SousConcess 	va_SousConcess
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

	static char *version = "$Id: xzae30.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $ : XZAE30_Decrire_SousConcess_PA" ;

	CS_CHAR *rpc_name =XZAEC_XZAE30_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE30_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt et XZAET_SousConcess
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	
	XDY_Nom		va_Nom_out;
	XDY_Booleen	va_Ferme_out;
	XDY_Booleen	va_Penurie_out;
	XDY_Octet	va_Carburant_out;
	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines du format Sybase au format Unix
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE29_Decrire_Manifestation_PA : va_Horodate_in=%d va_NumEvt_in=%d va_CleEvt_in=%d",va_Horodate_in,va_NumEvt_in,va_CleEvt_in);
	
	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/* Allocation de la structure datafmt				*/

	memset(&datafmt,0,sizeof(CS_DATAFMT));

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

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
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

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(@va_NumEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
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

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(@va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Nom_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 255;
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Nom_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, NULL, 0, -1)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(va_Nom_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Ferme_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[1].pt_value = (CS_VOID *)&va_Ferme_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Ferme_out,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(va_Ferme_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Penurie_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[2].pt_value = (CS_VOID *)&va_Penurie_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Penurie_out,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(va_Penurie_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Carburant_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[3].pt_value = (CS_VOID *)&va_Carburant_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Carburant_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_param(va_Carburant_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae30 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae30 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae30 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae30 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae30: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free (Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	strcpy(va_SousConcess->Nom, va_Nom_out);
	va_SousConcess->Ferme     = va_Ferme_out;
	va_SousConcess->Penurie   = va_Penurie_out;
	va_SousConcess->Carburant = va_Carburant_out;


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE30_Decrire_SousConcess_PA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free (Tab_donnees2);
	return(XDC_OK);
}
