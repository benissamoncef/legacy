/*E*/
/*Fichier : $Id: xzao15.c,v 1.29 1998/02/19 11:43:07 gesconf Exp $      Release : $Revision: 1.29 $        Date : $Date: 1998/02/19 11:43:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao15.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao15.h
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier       17 Aug 1994     : Creation
* volcic        13 Oct 1994     : Modification pour bouchon SAIDEC (v1.19)
* MERCIER Ol.	17 Nov 1994	: Modification des noms arguments	V 1.21
*	ajout de asql_clean_cmd et de asql_cmd_alloc
* MERCIER Ol.	17 Nov 1994	: Ajout du SPstatus de retour de proc	V 1.22
* volcic	28 Nov 1994	: Ajout du free(pl_Tab_donnees2) (v1.24)
* C.T.		13 Jan 1995	: Ajout de la date en entree (V1.25)
*                                 non teste
* B.G.		17 Jan 1995	: Ajout de la date en entree XDC_OK(V1.26)
* B.G.		12 Avr 1995	: suppression printf (V1.26)
* B.G.		19 Avr 1995	: suppression malloc tabligresult (V1.28)
* JMG		18 nov 1997	: suppression asql_Free_Tab_Ligne_Results (hp-ux 10.20) 1.29
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao15.h"

static char *version = "$Id: xzao15.c,v 1.29 1998/02/19 11:43:07 gesconf Exp $	: xzao15" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*  
* Recherche le "numero" PMV non inhibe en amont (>0) ou
* en aval (<0)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO15_PMV_amont(	XDY_Autoroute va_Autoroute_In, 
			XDY_PR va_PR_In, 
			XDY_Sens va_Sens_In, 
			XDY_Mot va_Numero_In, 
			XDY_Mot *pa_NumPMV_Out, 
			XDY_District *pa_District_Out, 
			XDY_Autoroute *pa_Autoroute_Out, 
			XDY_PR *pa_PR_Out, 
			XDY_Sens *pa_Sens_Out, 
			XDY_Entier *pa_Distance_Out,
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
*  pa_NumPMV_Out
*  pa_District_Out
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
* INTERFACE 
* XZAO15 appelle AOSP15
* AOSP15 select dans les tables EQUIPEMENT et PORTION
* XZAO15 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR *rpc_name =XZAOC_XZAO15_RPC_NAME;
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2=XZAOC_XZAO15_NB_PARM_RETURN;
	tg_Colonne_donnees2 pl_Tab_donnees2[XZAOC_XZAO15_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue=0;
	int		vl_Nb_Col=0;
	tg_row_result 	vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_Horodate;

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAO15_PMV_amont : va_Autoroute_In='%d' va_PR_In=%d va_Sens_In=%d va_Numero_In=%d",
			va_Autoroute_In,va_PR_In,va_Sens_In,va_Numero_In);
			
	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_In,&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_date_Ux2Sybase(va_Horodate_In,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d",vl_retcode);
		return (XDC_NOK);
	}
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont sortie", vl_retcode);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_In,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_In,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_In,"@va_Sens_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d",vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Numero_In,"@va_Numero_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_smallint_input(va_Numero_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	pl_Tab_donnees2[0].pt_value = (CS_VOID *)pa_NumPMV_Out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)pa_NumPMV_Out,"@va_NumPMV_out")) != CS_SUCCEED)	

	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_smallint_output() a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	pl_Tab_donnees2[1].pt_value = (CS_VOID *)pa_District_Out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_District_Out,"@va_District_out")) != CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_tinyint_output() a echoue"); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
	 	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	 }
	 
	pl_Tab_donnees2[2].pt_value = (CS_VOID *)pa_Autoroute_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Autoroute_Out,"@va_Autoroute_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_tinyint_output() a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	 }
	 
	pl_Tab_donnees2[3].pt_value = (CS_VOID *)pa_PR_Out;
		
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_PR_Out,"@va_PR_out")) != CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_int_output() a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	pl_Tab_donnees2[4].pt_value = (CS_VOID *)pa_Sens_Out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)pa_Sens_Out,"@va_Sens_out")) != CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_tinyint_output() a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	pl_Tab_donnees2[5].pt_value = (CS_VOID *)pa_Distance_Out;
	
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)pa_Distance_Out,"@va_Distance_out")) != CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_int_output() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_date_input(datetime) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d",vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
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
						
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao15: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne 1");
						   asql_clean_cmd (pl_connection,pl_cmd);
	   					   return (XDC_NOK);
                                                }
                                                break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao15 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
                                       
                                	asql_clean_cmd (pl_connection,pl_cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
	   				
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao15 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					
					return (XDC_NOK);
				}
		}
	
	/*A
	** Desallocation de le strucuture de commande ou drop de la commande si 
	** quelque chose s est mal passe.
	*/
	asql_clean_cmd (pl_connection,pl_cmd);
	   				
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO15_PMV_amont retourne %d", vl_retcode);
	return(XDC_OK);
}




