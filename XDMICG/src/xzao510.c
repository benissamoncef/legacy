/*  Fichier : 	$Id: xzao510.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao510.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	16/02/24	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */
#include "xzao.h"

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO510SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO510_Ajouter_Batiment (	XDY_Basedd		va_Basedd_in,
								XZAOT_ConfBat	va_Batiment_in,
								XDY_ResConf		*va_Resultat_out )
/*
* PARAMETRES EN ENTREE :
*
*	XDY_Basedd			va_Basedd_in
*	XZAOT_ConfBat		va_Batiment_in
*		XDY_Octet			Numero
*		XDY_Nom				NomBatiment
*		XDY_Abrev			Abreviation
*		XDY_NomAuto			Autoroute
*		XDY_PR				PR
*		XDY_Sens			Sens
*		XDY_Booleen			Creation
*
* PARAMETRES EN SORTIE :
*
*	XDY_ResConf			*va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute un batiment en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table RES_BAT (Batiment)
*
* MECANISMES :
*
------------------------------------------------------*/
{
	CS_CHAR 				*rpc_name = XZAOC_XZAO510_RPC_NAME;
	CS_CONNECTION   		*vl_connection=XZAGV_Connexion;
	CS_COMMAND				*vl_cmd;
	CS_RETCODE      		vl_retcode;
	CS_INT          		vl_res_type;
	CS_SMALLINT     		vl_msg_id;
	int 					vl_num_Tab_donnees2 = XZAOC_XZAO510_NB_PARM_RETURN;
	tg_Colonne_donnees2		*vl_Tab_donnees2;
	XDY_Entier				vl_SPstatus;
	
	/* Definition des variables locales */
	XDY_Octet		vl_Numero;
	XDY_Nom			vl_NomBatiment;
	XDY_Abrev		vl_Abreviation;
	XDY_NomAuto		vl_Autoroute;
	XDY_PR			vl_PR;
	XDY_Sens		vl_Sens;
	XDY_Booleen		vl_Creation;

	/* Initialisation des variables locales */
	vl_Numero 	= va_Batiment_in.Numero;
	strcpy (vl_NomBatiment, va_Batiment_in.NomBatiment);
	strcpy (vl_Abreviation, va_Batiment_in.Abreviation);
	strcpy (vl_Autoroute, va_Batiment_in.Autoroute);
	vl_PR 		= va_Batiment_in.PR;
	vl_Sens		= va_Batiment_in.Sens;
	vl_Creation	= va_Batiment_in.Creation;	

	XZST_03EcritureTrace( XZSTC_FONCTION,"IN : XZAO510_Ajouter_Batiment : Numero = %d\tBatiment = %s\tAbrev = %s\tAutoroute = %s\tPR = %d\tSens = %d\tCreation = %d\n",
			va_Batiment_in.Numero,
			va_Batiment_in.NomBatiment,
			va_Batiment_in.Abreviation,
			va_Batiment_in.Autoroute,
			va_Batiment_in.PR,
			va_Batiment_in.Sens,
			va_Batiment_in.Creation );

	/* Allocation de la structure permettant la recuperation des parametres */
	vl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_Tab_donnees2);

	/* Initialisation de la commande */
	if ((vl_retcode = asql_cmd_alloc(vl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzao510: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao510: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
	  	free(vl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	/* Initialisation de la structure pour chacun des parametres d I/O a faire passer a la rpc. */
	if ((vl_retcode = asql_ctparam_char_input (vl_cmd,(CS_CHAR *)va_Basedd_in, "@va_Basedd_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_char_input() a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
		free (vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input (vl_cmd,(CS_TINYINT *)&vl_Numero, "@va_Numero_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_tinyint_input(Numero) a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO88_Ajouter_Peage retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
		free (vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input (vl_cmd,(CS_CHAR *)vl_NomBatiment, "@va_NomBatiment_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao510: asql_ctparam_char_input(NomBatiment) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
		free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input (vl_cmd,(CS_CHAR *)vl_Abreviation, "@va_Abreviation_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_char_input(Abreviation) a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
		free (vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_Autoroute, "@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_char_input(Autoroute) a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
		free (vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao510: asql_ctparam_tinyint_input(Sens) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
		free(vl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_int_input(PR) a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
		free (vl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_Creation, "@va_Creation_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_bit_input(Creation) a echoue");
	   XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free (vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	if ((vl_retcode = asql_ctparam_int_output(vl_cmd,(CS_INT *)va_Resultat_out, "@va_Resultat_out")) != CS_SUCCEED)	
	{
	   XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: asql_ctparam_smallint_output() a echoue "); 
	   XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
	   asql_clean_cmd (vl_connection, vl_cmd);
	   free (vl_Tab_donnees2);
	   return (XDC_NOK);
	}

	/* Envoie de la commande au Sql server */
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace (XZSTC_FATAL,"xzao510: ct_send() a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
		asql_clean_cmd (vl_connection, vl_cmd);
	   	free (vl_Tab_donnees2);
	   	return (XDC_NOK);
	}

	/* Traitement du resultat */
	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/* Impression de l entete en fonction du type de resultat. */
				switch ((int) vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd);
						XZST_03EcritureTrace (XZSTC_INFO,"xzao510: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (vl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace (XZSTC_INFO,"xzao510 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace (XZSTC_INFO,"xzao510 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace (XZSTC_INFO,"xzao510 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace (XZSTC_INTERFACE, "OUT : XZAO510_Ajouter_Batiment retourne 1");
 							asql_clean_cmd(vl_connection, vl_cmd);
							free(vl_Tab_donnees2);
							return (XDC_NOK);
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace (XZSTC_WARNING,"xzao510 : asql_fetch_* a echoue");
					XZST_03EcritureTrace (XZSTC_INTERFACE, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
					free(vl_Tab_donnees2);
					return (XDC_NOK);
				}
				break;

				case CS_MSG_RESULT:
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao510: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
						asql_clean_cmd(vl_connection, vl_cmd);
						free(vl_Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao510 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/* Pas de colonnes retournees. */
					XZST_03EcritureTrace(XZSTC_INFO,"xzao510 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:
					/* Commande effectuee : positionnement des parametres de retour. */
					XZST_03EcritureTrace(XZSTC_INFO,"xzao510 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/* Le serveur SQL a detecte une ereeur en executant notre commande. */
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao510: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/* Il s est passe quelque chose d inattendu. */
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao510: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);
		}
	}

	/* Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO510_Ajouter_Batiment retourne %d", vl_retcode);
	asql_clean_cmd(vl_connection, vl_cmd);
	free(vl_Tab_donnees2);
	return(XDC_OK);
}
