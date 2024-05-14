/*E*/
/*  Fichier : 	$Id: xzai07.c,v 1.3 1994/12/19 16:16:31 volcic Exp $  Release : $Revision: 1.3 $ Date : $Date: 1994/12/19 16:16:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai07.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       02 Nov 1994     : Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzai07.h"

static char *version = "$Id: xzai07.c,v 1.3 1994/12/19 16:16:31 volcic Exp $ : xzai07" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAI07SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAI07_Rechercher_Message_A_Diffuser ( 	XDY_Evt 	va_NumEvt_in,
				        	XDY_Horodate 	*va_Horodate_out,
				        	XDY_Booleen 	*va_Confidentiel_out ) 

/*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_Horodate_out
* XDY_Booleen 		va_Confidentiel_out
*
* VALEUR RENDUE :
*
* Recherche l'horodate du message È proposer et s'il est confidentiel ou non
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* SAD CI uniquement
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzai07.c,v 1.3 1994/12/19 16:16:31 volcic Exp $ : XZAI07_Rechercher_Message_A_Diffuser" ;

	CS_CHAR *rpc_name = XZAIC_XZAI07_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAIC_XZAI07_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	
	CS_DATETIME	vl_Horodate;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAI07_Rechercher_Message_A_Diffuser : NumEvt = %d,CleEvt = %d" , 
				va_NumEvt_in.numero,
				va_NumEvt_in.cle );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
		asql_clean_cmd (connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in.numero,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
	   asql_clean_cmd (connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_NumEvt_in.cle,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
	   asql_clean_cmd (connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	Tab_donnees2[0].pt_value = (CS_VOID *)&vl_Horodate;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_ctparam_date_output(Horodate) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
	   asql_clean_cmd (connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_Confidentiel_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Confidentiel_out,"@va_Confidentiel_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_ctparam_bit_output(Confidentiel) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
	   asql_clean_cmd (connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
		asql_clean_cmd (connection, cmd);
	   	free(Tab_donnees2);
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
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzai07: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne 1");
 						   asql_clean_cmd (connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzai07 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
                                        asql_clean_cmd (connection, cmd);
	   				free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
						asql_clean_cmd (connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzai07 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
					asql_clean_cmd (connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
					asql_clean_cmd (connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Conversion de la date parametre sortie de la SP en secondes
	*/
	
	if ((retcode = asql_date_Sybase2Ux(vl_Horodate,va_Horodate_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzai07: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
		asql_clean_cmd (connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}
	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAI07_Rechercher_Message_A_Diffuser retourne %d", retcode);
 	asql_clean_cmd (connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}




