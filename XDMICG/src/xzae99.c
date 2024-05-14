/*E*/
/* Fichier : $Id: xzae99.c,v 1.1 2012/07/04 10:30:40 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae99.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	20/01/22	: Creation SAE-DEM367
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae99.h"
static char *version = "@(#)xzae99.c	1.2 1.2 07/03/07 : xzae99.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle xzae99SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzae99_Rechercher_NumFMC_NumAction_par_idAVA (XDY_Texte	va_id_ava_in,
                                            XDY_Entier	*va_numEvt_out,
                                            XDY_Octet	*va_cleEvt_out,
											XDY_Entier	*va_numEvtCause_out,
                                            XDY_Octet	*va_cleEvtCause_out,
                                            XDY_Entier *va_numAction_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Texte	va_id_ava_in
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	va_numEvt_out
* XDY_Octet	    va_cleEvt_out
* XDY_Entier	va_numEvtCause_out
* XDY_Octet	    va_cleEvtCause_out
* XDY_Entier    va_numAction_out
*
* VALEUR RENDUE :
*
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
	CS_CHAR *rpc_name = XZAE99C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		vl_num_Tab_donnees2 = XZAE99C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE99C_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;
	int 		vl_cr=0;

	/*A
	** Definition des variables locales
	*/
	
	XDY_ZDP	vl_Eqt;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : xzae99_Rechercher_NumFMC_NumAction_par_idAVA va_id_ava_in=%s",va_id_ava_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_id_ava_in,"@va_id_ava_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_char_input(va_id_ava_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);

		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numEvt_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_numEvt_out,"@va_numEvt_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_int_output(va_numEvt_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}

	i = i +1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_cleEvt_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_cleEvt_out,"@va_cleEvt_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_int_output(va_cle_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}

	i = i +1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numEvtCause_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_numEvtCause_out,"@va_numEvtCause_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_int_output(va_numEvtCause_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}

	i = i +1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_cleEvtCause_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_cleEvtCause_out,"@va_cleEvtCause_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_int_output(va_cle_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}
	
	i= i +1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numAction_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_numAction_out,"@va_numAction_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: asql_ctparam_int_output(va_numAction_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae99: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae99 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae99 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae99 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d",SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae99: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae99: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae99: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur pour chaque UGTP 
	*/
		
	XZST_03EcritureTrace(XZSTC_INFO,"xzae99 : NumFMC:%d cle:%d",*va_numEvt_out,*va_cleEvt_out);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae99_Rechercher_NumFMC_NumAction_par_idAVA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




