/*E*/
/*Fichier : $Id: xzao19.c,v 1.12 2012/07/30 13:45:08 gesconf Exp $      Release : $Revision: 1.12 $        Date : $Date: 2012/07/30 13:45:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao19.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
* d'autoroute
*pl_Tab_param_retour
* XZAO19_NAV_amont : Recherche la "numero" NAV non inhibee en amont (>0) ou
* en aval (<0).
********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Sep 1994	: Creation
* MERCIER Ol	17 Nov 1994	: Mise a jour des 	V 1.5
*  noms des variables, appel a asql_clean_cmd et asql_cmd_alloc.
* volcic	28 Nov 1994	: Ajout du free(pl_Tab_param_retour) (v1.7)
* C.T.		13 Jan 1995	: Ajout de la date en entree (V1.8)
*                                 non teste
* B.G.		17 Jan 1995	: Ajout de la date en entree XDC_OK(V1.9)
* JMG		14 nov 1997 	: ajout type de nav 1.10
* JMG		18 nov 1997	: suppression asql_Free_Tab_Ligne_Results 1.11
* PNI		30/07/2012	: XDY_Octet *pa_TypeNav_Out 1.12 DEM1048
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao19.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao19.c,v 1.12 2012/07/30 13:45:08 gesconf Exp $ : xzao19" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* XZAO19_NAV_amont : Recherche la "numero" NAV non inhibee en amont (>0) ou
* en aval (<0).
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO19_NAV_amont(XDY_Autoroute 	va_Autoroute_In, 
			XDY_PR 		va_PR_In, 
			XDY_Sens 	va_Sens_In, 
			XDY_Mot 	va_Numero_In, 
			
			
			XDY_Mot 	*pa_NumNAV_Out, 
			XDY_District 	*pa_District_Out, 
			XDY_Autoroute 	*pa_Autoroute_Out, 
			XDY_PR 		*pa_PR_Out, 
			XDY_Sens 	*pa_Sens_Out, 
			XDY_Entier 	*pa_Distance_Out,
			XDY_Entier 	*pa_PRInfluence_Out,
			XDY_Octet	*pa_TypeNav_Out,
			XDY_Horodate va_Horodate_In)


/*
* ARGUMENTS EN ENTREE :
*  va_Autoroute_In 
*  va_PR_In
*  va_Sens_In
*  va_Numero_In
*  va_Horodate_In	: date apres les arguments de sortie
*
* ARGUMENTS EN SORTIE :
*  pa_NumNAV_Out
*  pa_District_Out
*  pa_Autoroute_Out
*  pa_PR_Out
*  pa_Sens_Out
*  pa_Distance_Out
*  pa_PRInfluence_Out
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO19 appelle XZAO;19
* AOSP19 select dans les tables EQUIPEMENT et PORTION
* XZAO19 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR 	*pl_rpc_name 		= XZAOC_XZAO19_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode		= XDC_NOK;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAO19_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
        tg_row_result   vl_Tab_retour_select;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_Horodate;
	
	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : XZAO19_NAV_amont : va_Autoroute_In='%d' va_PR_In=%d va_Sens_In=%d va_Numero_In=%d va_Horodate_In=%f",
			va_Autoroute_In,va_PR_In,va_Sens_In,va_Numero_In, va_Horodate_In);
			
	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_In,&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao19: asql_date_Ux2Sybase(va_Horodate_In,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d",vl_retcode);
		return (XDC_NOK);
	}

	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_param_retour = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_param_retour);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_In,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_In,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_In,"@va_Sens_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}	
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Numero_In,"@va_Numero_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_smallint_input(va_Numero_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		printf("error de ct_param pour va_Numero_In = %d\n",va_Numero_In);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

	pl_Tab_param_retour[0].pt_value = (CS_VOID *)pa_NumNAV_Out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)pa_NumNAV_Out,"@va_NumNAV_out")) != CS_SUCCEED)	

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_smallint_output(va_NumNAV_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[1].pt_value = (CS_VOID *)pa_District_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_District_Out,"@va_District_out")) != CS_SUCCEED)	

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_output(va_District_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[2].pt_value = (CS_VOID *)pa_Autoroute_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Autoroute_Out,"@va_Autoroute_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_output(pa_Autoroute_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	
	pl_Tab_param_retour[3].pt_value = (CS_VOID *)pa_PR_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_PR_Out,"@va_PR_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_int_output(pa_PR_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[4].pt_value = (CS_VOID *)pa_Sens_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Sens_Out,"@va_Sens_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_output(pa_Sens_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	
	pl_Tab_param_retour[5].pt_value = (CS_VOID *)pa_Distance_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_Distance_Out,"@va_Distance_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_int_output(pa_Distance_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}
	
	pl_Tab_param_retour[6].pt_value = (CS_VOID *)pa_PRInfluence_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_PRInfluence_Out,"@va_PRInfluence_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_int_output(pa_PRInfluence_Out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}

        pl_Tab_param_retour[7].pt_value = (CS_TINYINT *)pa_TypeNav_Out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_TypeNav_Out,"@va_Type_out"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAO19: asql_ctparam_tinyint_output(pa_TypeNav_Out) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK );
                asql_clean_cmd (pl_connection,pl_cmd);
                free(pl_Tab_param_retour);
                return (XDC_NOK);
        }
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_date_input(datetime) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO19_NAV_amont retourne %d",vl_retcode);
		printf("error de ct_param pour va_Horodate_In = %f\n",va_Horodate_In);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_param_retour);
		return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
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
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_retour_select);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao19: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						
						XZST_03EcritureTrace(XZSTC_INFO,"xzao19 : PARAMETER RESULTS\n");
						
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAO19_NAV_amont : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAO19_NAV_amont : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO16_Aire_Amont retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_param_retour);
							return (XDC_NOK);
                                                }

						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                	XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: asql_fetch_* a echoue");
                                	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_res_info(CS_MSGTYPE) a echoue");
						XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_INFO,"xzao19: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao19 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao19 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"xzao19: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
		
	}
	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO19_NAV_amont retourne %d", XDC_OK);
 	free(pl_Tab_param_retour);
	return(XDC_OK);
}
