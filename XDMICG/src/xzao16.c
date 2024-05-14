/*E*/
/*Fichier : $Id: xzao16.c,v 1.7 1994/12/15 14:55:57 volcic Exp $      Release : $Revision: 1.7 $        Date : $Date: 1994/12/15 14:55:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao16.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
* XZAO16_Aire_Amont : Recherche la "numero" aire en amont (>0) ou
* en aval (<0).
*
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Sep 1994	: Creation
* MERCIER	17 Nov 1994	: Modification			V 1.4
*	Mise en conformite des arguments
*	appel a asql_clean_cmd et a asql_cmd_alloc
* volcic	28 Nov 1994	: Ajout du free(pl_Tab_donnees2) (v1.7)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao16.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao16.c,v 1.7 1994/12/15 14:55:57 volcic Exp $ : xzao16" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
* XZAO16_Aire_Amont : Recherche la "numero" aire en amont (>0) ou
* en aval (<0).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO16_Aire_Amont(XDY_Autoroute 	va_Autoroute_In, 
			XDY_PR 		va_PR_In, 
			XDY_Sens 	va_Sens_In, 
			XDY_Mot 	va_Numero_In,
			 
			XDY_Octet 	*pa_NumAire_Out,		
			XDY_Nom		va_NomAire_Out,
			XDY_Abrev	va_AbrevAire_Out, 
			XDY_Autoroute 	*pa_Autoroute_Out, 
			XDY_PR 		*pa_PR_Out, 
			XDY_Sens 	*pa_Sens_Out, 
			XDY_Entier 	*pa_Distance_Out)


/*
* ARGUMENTS EN ENTREE :
*  va_Autoroute_In 
*  va_PR_In
*  Sens_In
*  va_Numero_In
*
* ARGUMENTS EN SORTIE :

*  pa_NumAire_Out
*  AbrevAire_Out
*  Autoroute_Out
*  PR_Out
*  Sens_Out
*  Distance_Out
*
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO16 appelle AOSP16
* AOSP16 select dans les tables AIRE et PORTION
* XZAO16 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR 	*pl_rpc_name 	= XZAOC_XZAO16_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection	= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_param_retour = XZAOC_XZAO16_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2; 
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
        tg_row_result   vl_Tab_retour_select;
	
	XDY_Entier      vl_SPstatus;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,
			"IN : XZAO16_Aire_Amont : va_Autoroute_In=%d va_PR_In=%d va_Sens_In=%d va_Numero_In=%d",
			va_Autoroute_In,va_PR_In,va_Sens_In,va_Numero_In);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_param_retour);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d",XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d",XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_In,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_In,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_In,"@va_Sens_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}	
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Numero_In,"@va_Numero_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_smallint_input(va_Numero_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		printf("error de ct_param pour va_Numero_In = %d\n",va_Numero_In);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}

	pl_Tab_donnees2[0].pt_value = (CS_VOID *)pa_NumAire_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_NumAire_Out,"@va_NumAire_out")) != CS_SUCCEED)	

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_tinyint_output() a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	pl_Tab_donnees2[1].pt_value = (CS_VOID *)va_NomAire_Out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_NomAire_Out,"@va_NomAire_out")) != CS_SUCCEED)	

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_char_output(va_NomAire_Out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}

	pl_Tab_donnees2[2].pt_value = (CS_VOID *)va_AbrevAire_Out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_AbrevAire_Out,"@va_AbrevAire_out")) != CS_SUCCEED)	

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_char_output(va_AbrevAire_Out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	pl_Tab_donnees2[3].pt_value = (CS_VOID *)pa_Autoroute_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Autoroute_Out,"@va_Autoroute_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_tinyint_output() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	pl_Tab_donnees2[4].pt_value = (CS_VOID *)pa_PR_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_PR_Out,"@va_PR_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_int_input(pa_PR_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	pl_Tab_donnees2[5].pt_value = (CS_VOID *)pa_Sens_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Sens_Out,"@va_Sens_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_tinyint_output(pa_Sens_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	pl_Tab_donnees2[6].pt_value = (CS_VOID *)pa_Distance_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_Distance_Out,"@va_Distance_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: asql_ctparam_int_input(pa_Distance_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d",XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
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
						vl_retcode = asql_fetch_data (pl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzao16: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao16 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao16 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao16 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_donnees2);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao16: CS_CMD_SUCCEED");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao16: CS_CMD_DONE");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d", XDC_NOK);
					
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao16: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_donnees2);
					return (XDC_NOK);
				}
	}
	
	asql_clean_cmd (pl_connection,pl_cmd);
	free(pl_Tab_donnees2);
					
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO16_Aire_Amont retourne %d", XDC_OK);
 	return(XDC_OK);
}


