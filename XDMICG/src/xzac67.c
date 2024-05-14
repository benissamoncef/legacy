/*E*/
/*  Fichier : 	@(#)xzac67.c	1.1  Release : 1.1 Date : 11/16/04
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac67.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/11/04	creation
* PFE   17/11/06        DEM 591 : on supprime les free si pas de alloc pb en version HPUX 11.11
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac67.h"

static char *version = "@(#)xzac67.c	1.1 11/16/04 : xzac67" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC67SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC67_Commande_Fin_PAL ( XDY_Eqt       va_NumEqt_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Horodate  va_HeureLancementFin_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 char       va_Scenario_in[6],
                                 char      va_NomScenario_in[50])

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de PAL
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

	static char *version = "@(#)xzac67.c	1.1 11/16/04 : XZAC67_Commande_Fin_PAL" ;

	CS_CHAR *rpc_name = XZACC_XZAC67_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC67_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_NomSite     pl_NomSite;

	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_HeureLancementFin_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_HeureLancementFin_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PALretourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC67_Commande_Fin_PAL ");
			
        /*A
        ** Recuperation du site de la machine
        */
        if ( (retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzac67 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzac67 : Recuperation du Nom du site %s.",pl_NomSite);

			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumeroPAL_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: asql_ctparam_tinyint_input(va_Site_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_HeureLancementFin_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_Manuelle_in,"@va_Manuelle_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: asql_ctparam_bit_input(va_Manuelle_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Scenario_in,"@va_Scenario_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAS62: asql_ctparam_char_input(va_Scenario_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS62_Ajout_Mesure_DATEX retourne %d", retcode);
                asql_clean_cmd (connection,cmd);
                return (XDC_NOK);
        }

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomScenario_in,"@va_NomScenario_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAS62: asql_ctparam_char_input(va_NomScenario_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS62_Ajout_Mesure_DATEX retourne %d", retcode);
                asql_clean_cmd (connection,cmd);
                return (XDC_NOK);
        }
 
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac67: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac67 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac67 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac67: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC67_Commande_Fin_PAL retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




