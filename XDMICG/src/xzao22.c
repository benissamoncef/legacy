/*E*/
/*Fichier : $Id: xzao22.c,v 1.1 1998/02/03 14:02:51 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/02/03 14:02:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao22.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       16/12/97     : Creation (dem/1534)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao22.c,v 1.1 1998/02/03 14:02:51 verdier Exp $ $Revision: 1.1 $ $Date: 1998/02/03 14:02:51 $: xzao22.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO22SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO22_Distance (	XDY_Autoroute	va_Autoroute_in,
				XDY_PR		va_PR_in,
				XDY_Autoroute	va_Autoroute2_in,
				XDY_PR		va_PR2_in,
				XDY_Entier	*va_distance_out)

/*
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* distance entre 2 PR
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
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_ECH (DEMI-ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao22.c,v 1.1 1998/02/03 14:02:51 verdier Exp $ : xzao22_Distance" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO22_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_pl_Tab_donnees2 = XZAOC_XZAO22_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_Entier	vl_Tunnel;
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO22_Distance : Autoroute = %d\tPR = %d\tautoroute2 = %d\t PR2 = %d\n",
			va_Autoroute_in,
			va_PR_in,
			va_Autoroute2_in,
			va_PR2_in);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_pl_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Autoroute_in,"@va_NumeroAutorouteDeb_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_ctparam_tinyint_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in,"@va_PRDebut_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Autoroute2_in,"@va_NumeroAutorouteFin_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_ctparam_tinyint_input(Autoroute2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR2_in,"@va_PRFin_in"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_ctparam_int_input(PR2) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  return (XDC_NOK);
	}
	
	pl_Tab_donnees2[0].pt_value = (CS_VOID *)va_distance_out;
	
	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_distance_out,"@va_Distance_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: asql_ctparam_int_output(distance) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao22: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , vl_num_pl_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao22 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao22 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao22 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne 1");
 						   asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
						asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao22: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO22_Distance retourne %d", vl_retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




