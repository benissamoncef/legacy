/*E*/
/* Fichier : $Id: xzae15.c,v 1.1 2012/07/04 10:30:40 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae15.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	20/01/22	: Creation DEM-SAE367
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae15.h"
static char *version = "@(#)xzae15.c	1.2 1.2 07/03/07 : xzae15.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle xzae15SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzae15_Transmettre_Droit_Enrichir (	 XDY_Entier	va_NumEvt_in,
                                         XDY_Octet	va_CleEvt_in,
                                         XDY_Octet	va_Site_in,
                                         XDY_Mot va_PosteOperateur_in,
                                         XDY_Entier	va_Sans_Rafraichir_in,
                                         XDY_Entier	va_Sans_Reveil_in,
										 XDY_NomSite va_NomSiteLocal_in,
                                         XDY_Entier	*va_Resultat_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Octet	va_Site_in
* XDY_Mot	va_PosteOperateur_in
* XDY_Entier	va_Sans_Rafraichir_in
* XDY_Entier	va_Sans_Reveil_in
* XDY_NomSite va_NomSiteLocal_in
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	va_Resultat_out
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
* Transmet les droits d'enrichir à un autre site ou un autre operateur
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR *rpc_name = XZAE15C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		vl_num_Tab_donnees2 = XZAE15C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE15C_NB_PARM_RETURN];
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
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : xzae15_Transmettre_Droit_Enrichir FMC:%d/%d, site:%d, operateur:%d sans_rafraichir:%d, sans_reveil:%d, Nomsiteappelant:%s",va_NumEvt_in,va_CleEvt_in,va_Site_in,va_PosteOperateur_in,va_Sans_Rafraichir_in,va_Sans_Reveil_in,va_NomSiteLocal_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
		
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
       XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_NumEvt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_Site_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PosteOperateur_in,"@va_PosteOperateur_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_PosteOperateur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Sans_Rafraichir_in,"@va_Sans_Rafraichir_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_Sans_Rafraichir_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Sans_Reveil_in,"@va_Sans_Reveil_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_input(va_Sans_Reveil_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomSiteLocal_in,"@va_NomSiteLocal_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_char_input(va_NomSiteLocal_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);

		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_Resultat_out,"@va_Resultat_out"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: asql_ctparam_int_output(va_Resultat_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae15: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae15 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae15 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae15 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d",SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae15: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae15: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae15: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir resultat out %d", va_Resultat_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae15_Transmettre_Droit_Enrichir retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




