/*E*/
/*Fichier : @(#)xzac80.c	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzac80.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Insere une action DATEX si la FMC associee existe dans MIGRAZUR
*  (insertion dans les tables ACT_GEN et ACT_ADA)
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      04 Mar 2004     : Creation (SAGA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac80.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzac80.c	1.1 02/26/99 : xzac80" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAC80_Ajouter_Action_DATEX :
*  Insere une action DATEX si la FMC associee existe dans MIGRAZUR
*  (insertion dans les tables ACT_GEN et ACT_ADA)
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAC80_Ajouter_Action_DATEX (char *va_emetteur_in,
                                 char *va_reference_in,
                                 char *va_lnk_in,
				 XDY_Horodate va_horodate_debut_in,
				 XDY_Horodate va_horodate_fin_in,
				 char *va_libelle_in,
				 int  *va_numeroAction_out)
/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in         code du partenaire qui a emis le message
* char *va_reference_in        reference datex associee
* char *va_lnk_in              numero de FMC cause
* XDY_Horodate va_horodate_debut_in  horodate de debut
* XDY_Horodate va_horodate_fin_in  horodate de fin
* va_libelle_in                commentaires
*				
* PARAMETRES EN SORTIE :
* va_numeroAction_out          numero de l'action creee
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
	static char *version = "@(#)xzac80.c	1.1 02/26/99 : XZAC80_Ajouter_Action_DATEX" ;
	
	CS_CHAR 	*pl_rpc_name = XZAC80C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int             vl_num_Tab_donnees2 = XZAC80C_NB_PARM_RETURN;
	tg_Colonne_donnees2 vl_Tab_donnees2[XZAC80C_NB_PARM_RETURN];
	XDY_Entier      vl_SPstatus;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	CS_DATETIME     vl_Horodate_debut;
	CS_DATETIME     vl_Horodate_fin;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC80_Ajouter_Action_DATEX : Debut d execution \nva_emetteur_in:%s, va_reference_in:%s, va_libelle_in:%s",
	va_emetteur_in,va_reference_in,va_libelle_in);

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_emetteur_in,"@va_emetteur_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_char_input(@va_emetteur_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_reference_in,"@va_reference_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_char_input(@va_reference_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_lnk_in,"@va_lnk_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_char_input(@va_lnk_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_date_Ux2Sybase(va_horodate_debut_in,&vl_Horodate_debut))!= XDC_OK)
	{
  	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_date_Ux2Sybase(va_horodate_debut_in,&vl_Horodate_debut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
        }
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate_debut,"@va_horodate_debut_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_date_input(@va_horodate_debut_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_date_Ux2Sybase(va_horodate_fin_in,&vl_Horodate_fin))!= XDC_OK)
	{
  	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_date_Ux2Sybase(va_horodate_fin_in,&vl_Horodate_fin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
        }
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate_fin,"@va_horodate_fin_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_date_input(@va_horodate_fin_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_libelle_in,"@va_libelle_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_char_input(@va_libelle_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

	vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_numeroAction_out;
        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_numeroAction_out,"@va_numeroAction_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: asql_ctparam_char_input(@va_numeroAction_out) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC80 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC80 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC80 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC80 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAC80_Ajouter_Action_DATEX retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAC80: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}


	XZST_03EcritureTrace(	XZSTC_FONCTION, "OUT : XZAC80_Ajouter_Action_DATEX");
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC80_Ajouter_Action_DATEX retourne %d", vl_retcode);
 	return(XDC_OK);
}



