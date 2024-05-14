/*E*/
/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao960.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GR  30/07/2020     : Creation TDP_FCD DEM-SAE152 1.1
* ABE 24/09/2020     : Appel par callback acli DEM-SAE152 1.2
* LCL 27/05/2021     : Fix liberation memoire 1.3
* LCL 05/07/2021     : Fix liberation memoire pl_Tab_donnees2 DEM-SAE303 1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
static char *version = "$Id: xzao960.c,v 1.4 2021/07/05 16:39:34 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2021/07/05 16:39:34 $: xzao22.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO960SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO960_insert_TDP_FCD (	XDY_Horodate va_Horodate_in,
				XDY_PR va_PR_debut_in,
				XDY_PR va_PR_fin_in,
				XDY_Sens va_Sens_in,
				XDY_Entier va_Tdp_in,
				XDY_Octet va_Validite_in,
				XDY_NomAuto va_Autoroute_in)
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

	static char *version = "$Id: xzao960.c,v 1.4 2021/07/05 16:39:34 pc2dpdy Exp $ : xzao22_Distance" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO960_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_pl_Tab_donnees2 = XZAOC_XZAO960_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_Entier	vl_Tunnel;
	CS_DATETIME     vl_Horodate_sybase_in;
	
	/*A
	** Definition des variables locales
	*/
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO960_insert_TDP_FCD : Horodate = %f\tPR1 = %d\tPR2 = %d\tSens = %d\tTdp = %d\tValidite = %d\tAutoroute = %s\n",
			va_Horodate_in,
			va_PR_debut_in,
			va_PR_fin_in,
			va_Sens_in,
			va_Tdp_in,
			va_Validite_in,
			va_Autoroute_in);
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_pl_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/
	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        /*A
        ** Transformation de l horodate de double vers le structure Sybase
        */
        if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate_sybase_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"xzao960 : asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate_sybase_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
                asql_clean_cmd(connection, cmd);
		free(pl_Tab_donnees2);
                return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate_sybase_in,"@va_Horodate_in"))!= CS_SUCCEED)
        {                
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_datetime_input(Horodate) a echoue");	
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
                asql_clean_cmd(connection, cmd);
		free(pl_Tab_donnees2);
                return (XDC_NOK);
        }

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_debut_in,"@va_PR_debut_in"))!= CS_SUCCEED)		
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_int_input(PR_debut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(pl_Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_fin_in,"@va_PR_fin_in"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_int_input(PR_fin) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  free(pl_Tab_donnees2);
	  return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(pl_Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Tdp_in,"@va_Tdp_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_int_input(Tdp) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(pl_Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Validite_in,"@va_Validite_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_tinyint_input(Validite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(pl_Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(pl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		free(pl_Tab_donnees2);
	        return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_data(cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao960: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , vl_num_pl_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao960 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao960 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao960 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne 1");
 						   asql_clean_cmd(connection, cmd);
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
					vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
						asql_clean_cmd(connection, cmd);
						free(pl_Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
					free(pl_Tab_donnees2);
	        			return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao960: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
					free(pl_Tab_donnees2);
	        			return (XDC_NOK);
				}
	}

	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/

	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

	if (vl_retcode != XDC_OK)
	{
        	XZST_03EcritureTrace( XZSTC_WARNING, "xzao960 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire" );
	        asql_clean_cmd( connection, cmd );
		free(pl_Tab_donnees2);
	        return ( XDC_NOK );
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	asql_clean_cmd(connection, cmd);
	free(pl_Tab_donnees2);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO960_insert_TDP_FCD retourne %d", vl_retcode);

	return(XDC_OK);
}
