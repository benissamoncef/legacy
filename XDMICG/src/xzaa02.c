/*E*/
/* Fichier : $Id: xzaa02.c,v 1.1 2012/07/04 10:30:40 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzaa02.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	04/04/22	: Creation SAE-DEM367
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaa02.h"
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle xzaa02SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzaa02_Archive_Fiche_Alerte (XDY_Entier va_NumAlerte_in,
                                XDY_Octet va_Qualification_in,
                                XDY_Entier va_NumEvt_in,
                                XDY_Octet va_CleEvt_in,
                                XDY_NomSite va_NomSiteLocal_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier va_NumAlerte_in,
* XDY_Octet va_Qualification_in,
* XDY_Entier va_NumEvt_in,
* XDY_Octet va_CleEvt_in,
* XDY_NomSite va_NomSiteLocal_in
*				
* PARAMETRES EN SORTIE :
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
	CS_CHAR *rpc_name = XZAA02C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	XDY_Entier	SPstatus;
	int 		vl_cr=0;

	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : xzaa02_Archive_Fiche_Alerte va_NumAlerte_in=%d, va_Qualification_in:%d, va_NumEvt_in:%d, va_CleEvt_in:%d, va_NomSiteLocal_in:%s",
                                 va_NumAlerte_in,
                                 va_Qualification_in,
                                 va_NumEvt_in,
                                 va_CleEvt_in,
                                 va_NomSiteLocal_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumAlerte_in,"@va_NumAlerte_in")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: asql_ctparam_int_input(va_NumAlerte_in) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}
	

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Qualification_in,"@va_Qualification_in")) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: asql_ctparam_tinyint_input(@va_Qualification_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA02_Archive_Fiche_ALERTE retourne %d", retcode);
        asql_clean_cmd(connection,cmd);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: asql_ctparam_int_input(va_NomSiteLocal_in) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
	   asql_clean_cmd (connection,cmd);
	   return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in")) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: asql_ctparam_tinyint_input(@va_CleEvt_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA02_Archive_Fiche_ALERTE retourne %d", retcode);
        asql_clean_cmd(connection,cmd);
        return (XDC_NOK);
    }

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomSiteLocal_in,"@va_NomSiteLocal_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: asql_ctparam_char_input(va_NomSiteLocal_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);

		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa02: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa02 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa02 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa02 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d",SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzaa02: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzaa02: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaa02: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzaa02_Archive_Fiche_Alerte retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




