/*E*/
/*Fichier : $Id: xzao907.c,v 1.1 2021/06/01 07:43:55 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/01 07:43:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao907.c
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
#include "xzao907.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO907SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO907_Trouve_PAU(	XDY_District	va_Site_in,
			XDY_Texte	va_Autoroute_in,
			XDY_Sens 	va_Sens_in,
			XDY_PR 		va_PR_Debut_in,
			XDY_PR 		va_PR_Fin_in,
			XDY_District	va_Site_Origine_in,
			XDY_Entier	va_DistanceEvt_in,
			XDY_Entier	*va_NumEqt_out )

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

	static char *version = "@(#)xzao907.c	$Id: xzao907.c,v 1.1 2021/06/01 07:43:55 pc2dpdy Exp $ %D% : XZAO907_Trouve_PAU" ;
	
	CS_CHAR *rpc_name = XZAO907_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	int 		vl_num_Tab_donnees2 = XZAO907_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAO907_NB_PARM_RETURN];
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO907_Trouve_PAU : va_Site_in:%d, va_Autoroute_in :%s, va_Sens_in:%d, va_PR_debut_in%d, va_PR_fin_in:%d, va_site_origine_in:%d, va_DistanceEvt_in:%d",
				va_Site_in,			
				va_Autoroute_in,
				va_Sens_in,
				va_PR_Debut_in,
				va_PR_Fin_in,
				va_Site_Origine_in,
				va_DistanceEvt_in);

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);												
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_tinyint_input(va_site_int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_char_input(Autoroute) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}      

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_tinyint_input(va_sens_int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
                
        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_Debut_in,"@va_PR_debut_in")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_int_input(va_PR_Debut_in) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }        

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_Fin_in,"@va_PR_fin_in")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_int_input(va_PR_Fin_in) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_Origine_in,"@va_site_origine_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_tinyint_input(va_site_int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_DistanceEvt_in,"@va_distant_in")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_int_input(va_distant_in) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }   
        
        i = 0;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumEqt_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEqt_out,"@va_NumEqt_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: asql_ctparam_int_output(@va_NumEqt_out a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao907: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao907 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao907 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao907 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao907: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace( XZSTC_INTERFACE,"OUT : XZAO907_Trouve_PAU : Eqt trouvee :%d", *va_NumEqt_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO907_Trouve_PAU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




