/*E*/
/*Fichier : $Id: xzan37.c,v 1.1 1994/11/30 12:36:46 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/30 12:36:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN37 * FICHIER xzan37.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module interface Annuaire 
*********************************
*  XZAN37_Nom_Fonction : Retourne le nom de l'interlocuteur 
*  associ≈ » une fonction.
*
*  Appelle XZAN37SP
*********************************  
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	30 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzan37.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzan37.c,v 1.1 1994/11/30 12:36:46 mercier Exp $ : xzan37" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAN37_Nom_Fonction : Retourne le nom de l'interlocuteur 
*  associ≈ » une fonction.
*  Appelle XZAN37SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAN37_Nom_Fonction (XDY_Octet va_NumTypeAstreinte_in,XDY_Nom va_Nom_out)

/*
*
* PARAMETRES EN ENTREE : va_NumTypeAstreinte_in
*			
* PARAMETRES EN SORTIE : va_Nom_out
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
* Retourne le nom de l'interlocuteur associ≈ » une fonction
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzan37.c,v 1.1 1994/11/30 12:36:46 mercier Exp $ : XZAN37_Nom_Fonction" ;
	
	CS_CHAR 	*pl_rpc_name = XZAN37C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAN37C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	
	XDY_Entier	vl_SPstatus;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAN37_Nom_Fonction a recu %d",va_NumTypeAstreinte_in);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_Tab_donnees2);
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_NumTypeAstreinte_in,"@va_NumTypeAstreinte_in"))!= CS_SUCCEED)
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"xzan37: asql_ctparam_tinyint_input(va_NumTypeAstreinte_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	free(pl_Tab_donnees2);
	   	return (XDC_NOK);
	}
	
	pl_Tab_donnees2[0].pt_value = (CS_VOID *)va_Nom_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,va_Nom_out,"@va_Nom_out")) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzan37 : asql_ctparam_char_output(va_Nom_out) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAN37_Nom_Fonction retourne %d",XDC_NOK );
                asql_clean_cmd (pl_connection,pl_cmd);
	   	free(pl_Tab_donnees2);
	   	return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
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
						vl_retcode = asql_fetch_data (pl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN37: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN37 : PARAMETER RESULTS\n");
						
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN37 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN37 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
 					free(pl_Tab_donnees2);
 					return ( XDC_OK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAN37: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_donnees2);
					return ( XDC_OK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAN37 a trouve pour Num astreinte =%d le nom : %s",va_NumTypeAstreinte_in,va_Nom_out);
 	
 	asql_clean_cmd (pl_connection,pl_cmd);
 	free(pl_Tab_donnees2);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN37_Nom_Fonction retourne %d", vl_retcode);
	return(XDC_OK);
}




