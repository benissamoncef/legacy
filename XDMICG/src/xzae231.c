/*E*/
/*  Fichier : 	$Id: xzae231.c,v 1.1 2020/11/03 17:56:51 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2020/11/03 17:56:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae231.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/12/18	creation AVA 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae231.h"

static char *version = "@(#)xzae231.c	1.2 1.2 12/06/10";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE231SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE231_Ajoute_FMC_AVA (
				char * 	   va_identifiant_in,
				XDY_Entier va_numero_in,
				XDY_Octet va_cle_in)
				

/*
* Arguments en entree
* Arguments en sortie
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{

	static char *version = "@(#)xzae231.c	1.2 1.2 12/06/10" ;

	CS_CHAR *rpc_name =XZAEC_XZAE231_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection = XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE231_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	CS_INT vl_numero;
	CS_TINYINT vl_cle;

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
	
	
	XZST_03EcritureTrace(	XZSTC_INFO,
			"IN : XZAE231_Ajoute_FMC_AVA %s : va_identifiant_in=%s %d %d", va_identifiant_in, va_numero_in, va_cle_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	**
	**
	*/

	if ((retcode = asql_ctparam_char_input(cmd, (CS_CHAR *)va_identifiant_in, "@va_identifiant_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_param(@va_identifiant_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        

	if ((retcode = asql_ctparam_int_input(cmd, (CS_INT *)&va_numero_in, "@va_numero_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_param(@va_identifiant_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd, (CS_TINYINT *)&va_cle_in, "@va_cle_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_param(@va_identifiant_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae231: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzae231 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae231 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae231 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE231_Ajoute_FMC_AVA retourne 1");
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae231: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/

 	XZST_03EcritureTrace(XZSTC_DEBUG1, "XZAE231_Ajoute_FMC_AVA %d %d ", 
			va_numero_in, va_cle_in);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 		
 	XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : XZAE231_Ajoute_FMC_AVA retourne %d ", retcode );
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
