/*	Fichier :	xzae861.c
------------------------------------------------------
* STERIA *	PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME	BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae861.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	18/01/24	:	Creation (DEM-483)
*
------------------------------------------------------*/

/* fichiers inclus */
#include "xzae861.h"

static char *version = " xzae861.c ";

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
*
*	Appelle XZAE861SP
*	Retourne le parametre
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE861_Basculement_Tunnel (	XDY_Autoroute 	va_NumAutoroute_in,
									XDY_PR 			va_PRDebut_in,
									XDY_PR			va_PRFin_in,
									XDY_Booleen		*va_BasculementTunnel_out)
/*
* ARGUMENTS EN ENTREE :
*
*	XDY_Autoroute	va_NumAutoroute_in
*	XDC_PR			va_PR_in
*
* ARGUMENTS EN SORTIE :
*
*	XDY_Booleen		va_BasculementTunnel_out
*
* CODE RETOUR : 
*
*	XDC_OK 
*	XDC_NOK
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{
	CS_CHAR 			*rpc_name = XZAEC_XZAE861_RPC_NAME;
	CS_CONTEXT			*context;
	CS_CONNECTION	 	*connection = XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT			datafmt;
	CS_RETCODE			retcode;
	CS_INT				res_type;
	CS_SMALLINT		 	msg_id;
	int 				num_Tab_donnees2 = XZAEC_XZAE861_NB_PARM_RETURN;
	tg_Colonne_donnees2	Tab_donnees2[XZAEC_XZAE861_NB_PARM_RETURN+1];
	XDY_Entier			SPstatus;
	int             	vl_Nb_Ligne_Lue = 0;
	int             	vl_Nb_Col = 0;
	tg_row_result   	vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : XZAE861_Basculement_Tunnel (%d/%d/%d/%d)", va_NumAutoroute_in, va_PRDebut_in, va_PRFin_in);

	/* Initialisation de la commande */
	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Parametres d'entree */
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_NumAutoroute_in,"@va_NumAutoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae861: asql_ctparam_tinyint_input(va_NumAutoroute_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRDebut_in,"@va_PRDebut_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae861: asql_ctparam_int_input(va_PRDebut_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRFin_in,"@va_PRFin_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae861: asql_ctparam_int_input(va_PRFin_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Parametre de sortie */
	Tab_donnees2[0].pt_value = (CS_VOID *)va_BasculementTunnel_out;
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_BasculementTunnel_out,"@va_BasculementTunnel_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_bit_output(va_BasculementTunnel_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/* Envoie de la commande au Sql server */
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
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
					case CS_ROW_RESULT:
						//retcode = asql_fetch_data (cmd );
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae861: ROW RESULTS");
						vl_Nb_Ligne_Lue++;
						break;

					case CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae861 : PARAMETER RESULTS\n");
						break;

					case CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae861 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae861 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE861_Basculement_Tunnel retourne 1");
 							asql_clean_cmd(connection, cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				/* Les trois types de resultats sont fetchables.
				** Since the result model for rpcs and rows have
				** been unified in the New Client-Library, we
				** will use the same routine to display them */
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
				break;

			case CS_MSG_RESULT:
				/* */
				retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
				break;

			case CS_CMD_SUCCEED:
				/* Pas de colonnes retournees. */
				break;

			case CS_CMD_DONE:
				/* Commande effectuee : positionnement des parametres de retour. */
				break;

			case CS_CMD_FAIL:
				/* Le serveur SQL a detecte une ereeur en executant notre commande. */
				XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
				asql_clean_cmd (connection, cmd);
				return ( XDC_NOK );
				break;

			default:
				/* Il s est passe quelque chose d inattendu. */
				XZST_03EcritureTrace(XZSTC_FATAL,"xzae861: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				return (XDC_NOK);
		}
	}

 	asql_clean_cmd(connection, cmd);
	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE861_Basculement_Tunnel : va_BasculementTunnel_out = %d.", *va_BasculementTunnel_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE861_Basculement_Tunnel retourne %d", retcode);
	return(XDC_OK);
}
