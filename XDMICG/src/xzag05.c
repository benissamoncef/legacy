/*E*/
/*Fichier : $Id: xzag05.c,v 1.2 1995/01/05 11:49:14 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/05 11:49:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG05 * FICHIER xzag05.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a BASEDD, module XZAG
*
* XZAG05 permet de purger la base d exploitation.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
*****************************
* MERCIER	10 Nov 1994	: Modification		V1.2
*	appel a asql_clean_cmd
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag05.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzag05.c,v 1.2 1995/01/05 11:49:14 mercier Exp $ : xzag05" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG05_Purge_Base_Exploitation : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d�clench� de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG05SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG05_Purge_Base_Exploitation (XDY_Horodate	va_Horodate_in)

/*
*
* PARAMETRES EN ENTREE :
*
*   va_Horodate_in est l horodate au dela de la quelle on doit effectuer
*   la purge.
*				
* PARAMETRES EN SORTIE :
*
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
*  On fait appel a XZAG;05
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag05.c,v 1.2 1995/01/05 11:49:14 mercier Exp $ : XZAG05_Purge_Base_Exploitation" ;
	
	CS_CHAR 	*	pl_rpc_name = XZAG05C_RPC_NAME;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int 			vl_num_Tab_donnees2 = XZAG05C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *	vl_Tab_donnees2;
	
	XDY_Entier		vl_SPstatus;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG05_Purge_Base_Exploitation : Debut d execution" );
			
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: asql_ctparam_date_input(Horodate) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
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
						vl_retcode = asql_fetch_data (pl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG05: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG05 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG05 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG05 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne 1");
 						   	asql_clean_cmd (pl_connection,pl_cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG05: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG05_Purge_Base_Exploitation retourne %d", vl_retcode);
 	return(XDC_OK);
}


