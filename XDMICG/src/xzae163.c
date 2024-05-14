/*E*/
/*Fichier : $Id: xzae163.c,v 1.1 2008/11/17 10:57:54 pc2dpdy Exp $    Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:57:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE163 * FICHIER xzae163.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* xzae163  
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae163.h"

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE163 : traiter icones WEB
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE163_Genere_Textes()

/*
*
* PARAMETRES EN ENTREE : aucun
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*	Appelle XZAE163 
*
------------------------------------------------------*/

{
	static char *version 		= "@(#)xzae163.c	1.1 1.1 04/19/01 : XZAE163_Genere_Textes" ;	
	int	vl_retcode 		= XDC_OK;
	int	vl_retcode_general	= XDC_OK;
	
	CS_CHAR *rpc_name = XZAE163C_XZAE163_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
/*	int		vl_num_Tab_donnees2 = XZAE163C_XZAE163_NB_PARAM_RETURN;	*/
/*	tg_Colonne_donnees2 *pl_Tab_donnees2;	*/
/*	int		vl_Nb_Ligne_Lue;	*/
/*	int		vl_Nb_Col;	*/
/*	tg_row_result 	vl_Tab_Ligne;	*/
/*	int		i;	*/
	XDY_Entier	vl_SPstatus;
		
		
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : XZAE163_Genere_Textes : Debut d execution ");
	

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	
	
	/*A
	** Envoi de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	
		
	/*A
	** Envoi de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
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
					case  CS_PARAM_RESULT:
					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae163 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae163 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne 1");
 						   asql_clean_cmd(pl_connection, pl_cmd);
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
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_res_info(msgtype) a echoue");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
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
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae163: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE163_Genere_Textes retourne %d", vl_retcode);
 	asql_clean_cmd(pl_connection, pl_cmd);
	return(XDC_OK);

}
