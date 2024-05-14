/*E*/
/*Fichier : $Id: xzao802.c,v 1.1 2021/06/01 07:43:47 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/01 07:43:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao802.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	11/05/2021	Création DEM-SAE244 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao802.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO802SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO802_Tunnel_Oppose(	XDY_Autoroute  	va_autoroute_in,
				XDY_Sens va_sens_in,
				XDY_PR	va_PR_in,
				XDY_PR * va_PR_Debut_out,
				XDY_PR * va_PR_Fin_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute va_autoroute_in
* XDY_Sens 	va_sens_in
* XDY_PR	va_PR_in
* XDY_PR	va_PR_Debut_out
* XDY_PR	va_PR_Fin_out 
*				
* PARAMETRES EN SORTIE :
* XDY_PR	va_PR_Debut_out,
* XDY_PR	va_PR_Fin_out 
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
* Récupère le tunnel opposé à partir d'une autoroute sens pr
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao802.c	$Id: xzao802.c,v 1.1 2021/06/01 07:43:47 pc2dpdy Exp $ %D% : XZAO802_Tunnel_Oppose" ;
	
	CS_CHAR *rpc_name = XZAO802_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	int 		vl_num_Tab_donnees2 = XZAO802_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAO802_NB_PARM_RETURN];
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO802_Tunnel_Oppose : Autoroute = %d, PR:%d, Sens:%d", va_autoroute_in, va_PR_in, va_sens_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);												
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: asql_ctparam_tinyint_input(Autoroute) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}      
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: asql_ctparam_int_input(va_PR_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: asql_ctparam_tinyint_input(va_sens_int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
        
        i = 0;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_Debut_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PR_Debut_out,"@va_PR_Debut_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: asql_ctparam_char_output(va_PR_Debut_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_Fin_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PR_Fin_out,"@va_PR_Fin_out")) != CS_SUCCEED)
        {					
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: asql_ctparam_char_output(va_PR_Fin_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao802: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao802 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao802 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao802 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne 1");
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
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao802: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace( XZSTC_INTERFACE,"OUT : XZAO802_Tunnel_Oppose : PR tunnel oppose: Debut=%d, Fin=:%d", *va_PR_Debut_out, *va_PR_Fin_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO802_Tunnel_Oppose retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




