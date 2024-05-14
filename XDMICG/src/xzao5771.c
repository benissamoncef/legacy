/* Fichier : $Id: xzao5771.c
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao5771.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	08/12/23 : Creation (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzao.h"

/* definitions de constantes */
/* definitions de types locaux */
/* definition de macro locales */
/* declaration de variables locales */
static char *version = "xzao5771.c : XZAO5771_PIC_basculement" ;

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAO5771_PIC_basculement (	XDY_Autoroute 	va_Autoroute_In,
								XDY_Sens 		va_Sens_In, 
								XDY_PR 			va_PR_In,
								XDY_PR 			va_PR_tete_In,
								XDY_District	va_District_In,
								XDY_Entier		va_Priorite_In,
								XDY_Horodate	va_Horodate_In,
								char*			va_Perturbation_in,
								XDY_FonctionInt pa_FonctionUtilisateur_in,
								XDY_Entier      *va_Resultat_out )
/*
* ARGUMENTS EN ENTREE :CFG
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO5771 appelle XZAO5771
*
------------------------------------------------------*/
{
	CS_CHAR			*pl_rpc_name = XZAOC_XZAO5771_RPC_NAME;
	CS_CONTEXT		*pl_context;
	CS_CONNECTION   *pl_connection = XZAGV_Connexion;
	CS_COMMAND		*pl_cmd;

	CS_RETCODE      	vl_retcode		= XDC_NOK;
	CS_INT          	vl_res_type		= 0;
	CS_SMALLINT     	vl_msg_id		= 0;
	int 				vl_num_param_retour 	= XZAOC_XZAO5771_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;

	int					vl_Nb_Ligne_Lue = 0;
	int					vl_Nb_Col = 0;
	int					i, vl_cr;
	tg_row_result		vl_Tab_Ligne;

	XDY_Entier			vl_SPstatus;
	CS_DATETIME			vl_Horodate;

	/* Initialisation de la commande */
	if ((vl_retcode = asql_cmd_alloc (pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_In,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO5771_PIC_basculement: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_In,"@va_Sens_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO5771_PIC_basculement: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_In,"@va_PR_Debut_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO5771_PIC_basculement: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_tete_In,"@va_PR_Fin_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO5771_PIC_basculement: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_Resultat_out,"@va_Resultat_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO5771_PIC_basculement: asql_ctparam_int_input(va_Resultat_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/* Envoie de la commande au Sql server */
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/* Traitement du resultat */
	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int) vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/* Impression de l entete en fonction du type de resultat. */
				switch ((int) vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result (pl_cmd, &vl_Nb_Ligne_Lue, &vl_Nb_Col, &vl_Tab_Ligne);
						XZST_03EcritureTrace (XZSTC_INFO, "XZAO5771_PIC_basculement: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

						if (vl_SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne 1");
							asql_clean_cmd (pl_connection,pl_cmd);
							free(pl_Tab_param_retour);
							return (XDC_NOK);
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
				break;

				case CS_MSG_RESULT:
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_res_info(CS_MSGTYPE) a echoue");
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/* Pas de colonnes retournees. */
					XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:
					/* Commande effectuee : positionnement des parametres de retour. */
					XZST_03EcritureTrace(XZSTC_INFO,"XZAO5771_PIC_basculement : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/* Le serveur SQL a detecte une ereeur en executant notre commande. */
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/* Il s est passe quelque chose d inattendu. */
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAO5771_PIC_basculement: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO5771_PIC_basculement retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
	}
	
	*va_Resultat_out = 0;
	XZST_03EcritureTrace (XZSTC_INFO,"xzao5771 : nb de PIC = %d",vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
		/* Appel de la fonction utilisateur FonctionUtilisateur */		
		if ( pa_FonctionUtilisateur_in == NULL)
		{
		   XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateur_in est nulle");
		   asql_clean_cmd(pl_connection, pl_cmd);
		   return (XDC_NOK);
		}
		else
		{
			XZST_03EcritureTrace (XZSTC_INFO, "xzao5771 : %d/%d/%d/%d/%d",
										* (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value,
										* (XDY_District *) vl_Tab_Ligne[i][1].pt_value,
										* (XDY_Autoroute *) vl_Tab_Ligne[i][2].pt_value,
										* (XDY_PR *) vl_Tab_Ligne[i][3].pt_value,
										* (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value);

            if ((vl_retcode = (*pa_FonctionUtilisateur_in) ( * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value
															,* (XDY_District *) vl_Tab_Ligne[i][1].pt_value
															,* (XDY_Autoroute *) vl_Tab_Ligne[i][2].pt_value
															,* (XDY_PR *) vl_Tab_Ligne[i][3].pt_value
															,* (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value
															, va_District_In
															, va_Priorite_In
															, va_Horodate_In
															, va_Perturbation_in
															, &vl_cr ) ) != XDC_OK )
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);
			}
			*va_Resultat_out = *va_Resultat_out || vl_cr;
		}
	}

	if (vl_Nb_Ligne_Lue>0)
		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

	/* Retourne le resultat et les parametres de sorties renseignes */
	XZST_03EcritureTrace (XZSTC_INTERFACE, "OUT : XZAO5771_PIC_basculement retourne  d", vl_retcode);
	asql_clean_cmd (pl_connection, pl_cmd);
	return (XDC_OK);
}
