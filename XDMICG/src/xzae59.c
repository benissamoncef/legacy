/*E*/
/* Fichier : $Id: xzae59.c,v 1.1 2012/07/04 10:30:40 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae59.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	10/01/23	: Creation DEM-SAE431
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae59.h"
static char *version = "@(#)xzae59.c	1.2 1.2 07/03/07 : xzae59.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle xzae59SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzae59_Rechercher_EQT_TDP_INHIB (   XDY_Octet   va_NumEqt_in,
                                        XDY_Mot     va_TypeEqt_in,
				                        XDY_Mot		*va_EQT_TDP_INHIB_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumEqt_in Numero d equipement recherche
* XDY_Octet 	va_TypeEqt_in Type d equipement recherche
* 				
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_EQT_TDP_INHIB_out
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
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR *rpc_name = XZAE59C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
    int 		vl_num_Tab_donnees2 = XZAE59C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE59C_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;

	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : xzae59_Rechercher_EQT_TDP_INHIB EQT=%d, Type=%d",va_NumEqt_in, va_TypeEqt_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
		
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	{
       XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_TypeEqt_in,"@va_TypeEqt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: asql_ctparam_tinyint_input(va_TypeEqt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	i = 0;

    Tab_donnees2[i].pt_value = (CS_VOID *)va_EQT_TDP_INHIB_out;	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_EQT_TDP_INHIB_out,"@va_EQT_TDP_INHIB_out"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: asql_ctparam_tinyint_output(va_EQT_TDP_INHIB_out) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
    }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae59: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae59 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae59 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae59 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d",SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae59: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae59: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae59: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

 	XZST_03EcritureTrace(XZSTC_INFO, "xzae59_Rechercher_EQT_TDP_INHIB INHIB =%d", *va_EQT_TDP_INHIB_out);
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae59_Rechercher_EQT_TDP_INHIB retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




