/*E*/
/*Fichier : $Id: xzao17.c,v 1.13 2012/07/26 11:53:37 gesconf Exp $      Release : $Revision: 1.13 $        Date : $Date: 2012/07/26 11:53:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao17.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche la "numero" sortie en amont (>0) ou
* en aval (<0).
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Sep 1994	: Creation
* volcic	28 Nov 1994	: Ajout du free(pl_Tab_param_retour) (v1.11)
* guilhou	18 nov 1997 	: suppression  asql_Free_Tab_Ligne_Results 1.12
* JPL		26/06/2012	: Degroupee; ajout parametres 'sortie usager' et 'indicateur bifurcation' (DEM 1033)  1.13
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao17.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao17.c,v 1.13 2012/07/26 11:53:37 gesconf Exp $ : xzao17" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

int XZAO17_Sortie_Amont(XDY_Autoroute 	va_Autoroute_In, 
			XDY_PR 		va_PR_In, 
			XDY_Sens 	va_Sens_In, 
			XDY_Mot		va_Numero_In,
			 
			XDY_Octet 	*pa_NumSortie_Out,		
			XDY_Nom		va_NomSortie_Out,
			XDY_Abrev	va_AbrevSortie_Out, 
			XDY_Sortie	va_SortieUsager_Out, 
			XDY_Octet 	*pa_Bifurcation_Out,		
			XDY_Autoroute 	*pa_Autoroute_Out, 
			XDY_PR 		*pa_PR_Out, 
			XDY_Sens 	*pa_Sens_Out, 
			XDY_Entier 	*pa_Distance_Out)


/*
* ARGUMENTS EN ENTREE :
*  va_Autoroute_In 
*  va_PR_In
*  va_Sens_In
*  va_Numero_In
*
* ARGUMENTS EN SORTIE :

*  pa_NumSortie_Out
*  va_NomSortie_Out
*  va_AbrevSortie_Out
*  va_SortieUsager_Out
*  pa_Bifurcation_Out
*  pa_Autoroute_Out
*  pa_PR_Out
*  pa_Sens_Out
*  pa_Distance_Out
*
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO17 appelle SP XZAO17
* XZAO17 select dans les tables ECHANGEUR et PORTION
* XZAO17 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR *rpc_name 			= XZAOC_XZAO17_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode		= 0;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAO17_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour; 
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
        tg_row_result   vl_Tab_Ligne;
        XDY_Entier 	vl_SPstatus 		= XDC_NOK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,
			"IN : XZAO17_Sortie_Amont : va_Autoroute_In=%d va_PR_In=%d va_Sens_In=%d va_Numero_In=%d",
			va_Autoroute_In,va_PR_In,va_Sens_In,va_Numero_In);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_param_retour = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_param_retour);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_In,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_In,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_In,"@va_Sens_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}	
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Numero_In,"@va_Numero_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_smallint_input(va_Numero_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		printf("error de ct_param pour va_Numero_In = %d\n",va_Numero_In);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	
	pl_Tab_param_retour[0].pt_value = (CS_VOID *)pa_NumSortie_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_NumSortie_Out,"@va_NumSortie_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_output(NumSortie) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[1].pt_value = (CS_VOID *)va_NomSortie_Out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_NomSortie_Out,"@va_NomSortie_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_char_output(NomSortie) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	pl_Tab_param_retour[2].pt_value = (CS_VOID *)va_AbrevSortie_Out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_AbrevSortie_Out,"@va_AbrevSortie_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_char_output(AbrevSortie) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	pl_Tab_param_retour[3].pt_value = (CS_VOID *)va_SortieUsager_Out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_SortieUsager_Out,"@va_SortieUsager_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_char_output(SortieUsager) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[4].pt_value = (CS_VOID *)pa_Bifurcation_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Bifurcation_Out,"@va_Bifurcation_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_output(Bifurcation) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	pl_Tab_param_retour[5].pt_value = (CS_VOID *)pa_Autoroute_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Autoroute_Out,"@va_Autoroute_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_output(Autoroute) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[6].pt_value = (CS_VOID *)pa_PR_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_PR_Out,"@va_PR_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_int_input(PR) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[7].pt_value = (CS_VOID *)pa_Sens_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Sens_Out,"@va_Sens_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_tinyint_output(Sens) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[8].pt_value = (CS_VOID *)pa_Distance_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_Distance_Out,"@va_Distance_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO17_Sortie_Amont: asql_ctparam_int_input(Distance) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
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
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						
						XZST_03EcritureTrace(XZSTC_INFO,"xzao17: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao17 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao17 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao17 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO17_Sortie_Amont retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_param_retour);
							return (XDC_NOK);
                                                }
						break;
				}
				 if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
                                        asql_clean_cmd (pl_connection,pl_cmd);
                                        free(pl_Tab_param_retour);
					return (XDC_NOK);
                                }

				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao17: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao17 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao17 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao17: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
	}
	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO17_Sortie_Amont retourne %d", XDC_OK);
 	asql_clean_cmd (pl_connection,pl_cmd);
 	free(pl_Tab_param_retour);
	return(XDC_OK);
}



