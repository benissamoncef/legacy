/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao963.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	23/01/2024 : Creation, d'apres XZAO962
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO963SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO963_Calcul_TDP_FCD_Pur (	XDY_Autoroute	va_autoroute_in_o,
					XDY_PR		va_PR_in_o,
					XDY_Sens        va_sens_in_o,		
					XDY_Autoroute	va_autoroute_in_d,
					XDY_PR		va_PR_in_d,
					XDY_Sens        va_sens_in_d,
					XDY_Entier      va_numero_in,
					XDY_Octet       va_cle_in,
					XDY_TDP	*	va_TDP_FCD_out,
					XDY_Octet *     va_validite_out,
					XDY_Booleen *	va_validite_faible_out)
/*
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
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
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAOC_XZAO963_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_pl_Tab_donnees2 = XZAOC_XZAO963_NB_PARM_RETURN;
	tg_Colonne_donnees2 pl_Tab_donnees2[XZAOC_XZAO963_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_Entier	vl_Tunnel;
	CS_DATETIME     vl_Horodate_sybase_in;
	int	vl_TDP_FCD_out;

	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO963_Calcul_TDP_FCD_Pur : Numero d'autoroute d'origine = %d\tPR d'origine = %d\tSens d'origine = %d\tNumero d'autoroute de destination = %d\tPR de destination = %d\tSens de destination = %d\tNumero FMC = %d\tCle FMC = %d",
			va_autoroute_in_o,
			va_PR_in_o,
 			va_sens_in_o,
 			va_autoroute_in_d,
 			va_PR_in_d,
 			va_sens_in_d,
			va_numero_in,
			va_cle_in
			);


	/*
	** Initialisation de la commande
	*/
	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	/*
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

    if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_autoroute_in_o,"@va_autoroute_in_o"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_input(autoroute_origine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in_o,"@va_PR_in_o"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_int_input(PR_origine) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_in_o,"@va_sens_in_o"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_input(sens_origine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_autoroute_in_d,"@va_autoroute_in_d"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_input(autoroute_destination) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in_d,"@va_PR_in_d"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_int_input(PR_destination) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_in_d,"@va_sens_in_d"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_input(sens_destination) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_numero_in,"@va_numero_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_int_input(va_numero_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

        i = 0;

        pl_Tab_donnees2[i].pt_value = (CS_VOID *)&vl_TDP_FCD_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_TDP_FCD_out,"@va_TDP_FCD_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_int_output(TDP) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	i = i + 1;

        pl_Tab_donnees2[i].pt_value = (CS_VOID *)va_validite_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_validite_out,"@va_validite_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_tinyint_output(validite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	i = i + 1;

        pl_Tab_donnees2[i].pt_value = (CS_VOID *)va_validite_faible_out;

	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_validite_faible_out,"@va_validite_faible_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: asql_ctparam_bit_output(validite_faible) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	/*
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	        return (XDC_NOK);
	}

	/*
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_data(cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao963 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , vl_num_pl_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao963 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao963 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao963 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne 1");
 						   asql_clean_cmd(connection, cmd);
	        				   return (XDC_NOK);
						}
						break;
				}

				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
						asql_clean_cmd(connection, cmd);
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
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao963: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);
				}
	}

	*va_TDP_FCD_out=(XDY_TDP)vl_TDP_FCD_out;
	XZST_03EcritureTrace( XZSTC_WARNING,"OUT : XZAO963_Calcul_TDP_FCD_Pur : Numero d'autoroute d'origine = %d\tPR d'origine = %d\tSens d'origine = %d\tNumero d'autoroute de destination = %d\tPR de destination = %d\tSens de destination = %d\tNumero FMC = %d\tCle FMC = %d\tTDP = %d\tValidite = %d\tValidite faible = %d\n",
			va_autoroute_in_o,
			va_PR_in_o,
 			va_sens_in_o,
 			va_autoroute_in_d,
 			va_PR_in_d,
 			va_sens_in_d,
			va_numero_in,
			va_cle_in,
 			*va_TDP_FCD_out,
			*va_validite_out,
			*va_validite_faible_out);

	/*
	** Retourne le resultat et les parametres de sorties renseignes
	*/

 	asql_clean_cmd(connection, cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO963_Calcul_TDP_FCD_Pur retourne %d", vl_retcode);

	return(XDC_OK);
}
