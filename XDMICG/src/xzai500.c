/*E*/
/*Fichier : @(#)xzai500.c	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzai500.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Enregistre en base un message transmis par un partenaire
* pour diffusion directe
* Si ce message est lie a une FMC, il est enregistre
* comme une action TFM via XZAC19 et XZAC20.
* Sinon, le message est enregistre dans la table TFM_DTX.
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      12 Fev 2004     : Creation (SAGA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzai500.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzai500.c	1.1 02/26/99 : xzai500" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAI500_Enregistre_Message_TFM :
* Enregistre en base un message transmis par un partenaire
* pour diffusion directe
* Si ce message est lie a une FMC, il est enregistre
* comme une action TFM via XZAC19 et XZAC20.
* Sinon, le message est enregistre dans la table TFM_DTX.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAI500_Enregistre_Message_TFM (char *va_code_partenaire_in,
				    char *va_reference_datex_in,
				    char *va_lnk_in,
				    XDY_Horodate va_HeureLancement_in,
				    char *va_texte_message_1_in,
				    char *va_texte_message_2_in)

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_code_partenaire_in   code du partenaire qui a emis le message
* char *va_reference_datex_in   reference datex associee
* char *va_lnk_in               reference datex FMC associee
* XDY_Horodate va_HeureLancement_in heure de lancement
* char *va_texte_message_1_in   texte a diffuser (debut du message)
* char *va_texte_message_2_in   texte a diffuser (fin du message)
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
*

* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzai500.c	1.1 02/26/99 : XZAI500_Memoriser_TFM_DATEX" ;
	
	CS_CHAR 	*pl_rpc_name = XZAI500C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int             vl_num_Tab_donnees2 = XZAI500C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	XDY_Entier      vl_SPstatus;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
        CS_DATETIME     vl_Horodate;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAI500_Memoriser_TFM_DATEX : Debut d execution \ncode_partenaire:%s, reference_datex:%s, texte_message:%s%s",
	va_code_partenaire_in,va_reference_datex_in,va_texte_message_1_in,va_texte_message_2_in);

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_code_partenaire_in,"@va_code_partenaire_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_code_partenaire_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_reference_datex_in,"@va_reference_datex_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_reference_datex_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_lnk_in,"@va_lnk_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_lnk_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

	if ((vl_retcode = asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate))!= XDC_OK)
	{
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
	}
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_HeureLancement_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_HeureLancement_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_texte_message_1_in,"@va_texte_message_1_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_texte_message_1_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_texte_message_2_in,"@va_texte_message_2_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: asql_ctparam_char_input(@va_texte_message_2_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/
	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_data (pl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAI500 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAI500 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAI500 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAI500 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAI500_Memoriser_TFM_DATEX retourne 1 : %d", (int) vl_SPstatus);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAI500: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}


	XZST_03EcritureTrace(	XZSTC_FONCTION, "OUT : XZAI500_Memoriser_TFM_DATEX");
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI500_Memoriser_TFM_DATEX retourne %d", vl_retcode);
 	return(XDC_OK);
}



