/* Fichier : $Id: xzac3031.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac3031.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	: Creation (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzac.h"
#include "xzac3031.h"

static char *version = "@(#)xzac3031.c	: xzac3031" ;

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC3031SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC3031_Commande_Fin_PIC (	XDY_Horodate		va_HeureLancementFin_in,
									XDY_Entier			va_NumEvt_in,
									XDY_Octet			va_CleEvt_in,
									XDY_Operateur		va_Operateur_in,
									XDY_Eqt				va_NumEqt_in,
									XDY_Site			va_Site_in,
									XDY_Bandeau_PIC		va_Bandeau_in,
									XDY_Booleen			va_Flash_in,
									XDY_Picto			va_Picto_in,
									XDY_Octet			va_Priorite_in,
									XDY_Site			va_Site_Origine_in,
									XDY_Entier			va_NumeroAction_out)

/*-----------------------------------------------------
* Service rendu
* commande de PIC
* 
* Sequence d'appel
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* 
------------------------------------------------*/
{

	static char *version = "@(#)xzac3031.c	1.1 11/16/04 : XZAC3031_Commande_Fin_PIC" ;

	CS_CHAR 			*rpc_name = XZACC_XZAC3031_RPC_NAME;
	CS_CONTEXT      	*context;
	CS_CONNECTION   	*connection=XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT      	datafmt;
	CS_RETCODE      	retcode = 0;
	CS_INT          	res_type;
	CS_SMALLINT     	msg_id;
	int 				num_Tab_donnees2 = XZACC_XZAC3031_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZACC_XZAC3031_NB_PARM_RETURN];
	XDY_Entier 			SPstatus;
	CS_DATETIME			vl_HorodateFin;
	
	/* Conversion des chaines de caracteres en entree du format Unix au format Sybase */
	if ((retcode = asql_date_Ux2Sybase(va_HeureLancementFin_in,&vl_HorodateFin))!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC retourne: asql_date_Ux2Sybase(va_HeureLancementFin_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		return (XDC_NOK);
	}
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC3031_Commande_Fin_PIC ");

	/* Initialisation de la commande */
	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Initialisation de la structure pour chacun des parametres d I/O a faire passer a la rpc. */
	if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_HorodateFin,"@va_HorodateFin_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_date_input(va_HorodateFin_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_int_input(NumEvt) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_tinyint_input(CleEvt) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_Operateur_in,"@va_Operateur_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_tinyint_input(va_Site_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Bandeau_in,"@va_Bandeau_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_char_input(va_Bandeau_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %s", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)va_Flash_in,"@va_Flash_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_bit_input(va_Flash_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %s", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Picto_in,"@va_Picto_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_char_input(va_Picto_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %s", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Priorite_in,"@va_Priorite_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_tinyint_input(va_Priorite_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_Origine_in,"@va_site_origine_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_tinyint_input(va_site_origine_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[0].pt_value = (CS_VOID *)va_NumeroAction_out;
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumeroAction_out,"@va_NumeroAction_out")) != CS_SUCCEED)	
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: asql_ctparam_tinyint_output(NumeroAction) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	/* Envoie de la commande au Sql server */
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC3031_Commande_Fin_PIC: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	return (XDC_NOK);
	}

	/* Traitement du resultat */
	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/* Impression de l entete en fonction du type de resultat. */
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne 1");
							asql_clean_cmd(connection, cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac3031 : asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
				break;

			case CS_MSG_RESULT:
			/*	**	*/
				retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac3031: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
					
				XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
				break;

			case CS_CMD_SUCCEED:
				/* Pas de colonnes retournees. */
				XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : CS_CMD_SUCCEED \n");
				break;

			case CS_CMD_DONE:
				/* Commande effectuee : positionnement des parametres de retour. */
				XZST_03EcritureTrace(XZSTC_DEBUG1,"xzac3031 : CS_CMD_DONE \n");
				break;

			case CS_CMD_FAIL:
				/* Le serveur SQL a detecte une ereeur en executant notre commande. */
				XZST_03EcritureTrace(XZSTC_WARNING,"xzac3031: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
	   			return (XDC_NOK);

			default:
				/* Il s est passe quelque chose d inattendu. */
				XZST_03EcritureTrace(XZSTC_WARNING,"xzac3031: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
	   			return (XDC_NOK);
		}
	}

	/* Retourne le resultat et les parametres de sorties renseignes */
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC3031_Commande_Fin_PIC retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
