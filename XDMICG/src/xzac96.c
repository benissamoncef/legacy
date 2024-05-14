/*E*/
/*X  Fichier : $Id: xzac96.c,v 1.2 2021/06/17 08:40:03 devgfi Exp $        $Revision: 1.2 $        $Date: 2021/06/17 08:40:03 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac96.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	29/11/17 : creation
* JPL	31/05/21 : Gestion des donnees des parametres de sortie (SAE-286)  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac96.h"

static char *version = "$Id: xzac96.c,v 1.2 2021/06/17 08:40:03 devgfi Exp $" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC96SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC96_Lire_FMC_Data ( XDY_Entier       va_NumeroAction_in,
				XDY_Eqt	va_NumEqt_in,
				XDY_Entier	*va_numero_out,
				XDY_Octet	*va_cle_out,
				XDY_PR		*va_pr_queue_out,
				XDY_PR		*va_pr_tete_out)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de BAF
* 
* Sequence d'appel
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* 
------------------------------------------------*/


{
	static char *version = "$Id: xzac96.c,v 1.2 2021/06/17 08:40:03 devgfi Exp $ : XZAC96_Lire_FMC_Data" ;

	CS_CHAR *rpc_name = XZACC_XZAC96_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	int i ;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC96_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZACC_XZAC96_NB_PARM_RETURN];
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC96_Lire_FMC_Data ");
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumeroAction_in,"@va_NumeroAction_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: asql_ctparam_int_input(va_NumeroAction_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
        i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numero_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_numero_out,"@va_numero_out")) != CS_SUCCEED)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae96: asql_ctparam_int_output(va_numero_out) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
         }

        i = 1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_cle_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_cle_out,"@va_cle_out")) != CS_SUCCEED)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae96: asql_ctparam_tinyint_output(va_cle_out) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	      asql_clean_cmd(connection,cmd);
	      return (XDC_NOK);
         }

        i = 2;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_pr_queue_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_pr_queue_out,"@va_pr_queue_out")) != CS_SUCCEED)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae96: asql_ctparam_int_output(va_pr_queue_out) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
         }


        i = 3;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_pr_tete_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_pr_tete_out,"@va_pr_tete_out")) != CS_SUCCEED)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae96: asql_ctparam_int_output(va_pr_tete_out) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
         }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzac96: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac96 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac96 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac96: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC96_Lire_FMC_Data retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




