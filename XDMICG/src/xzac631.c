/*E*/
/*Fichier :  $Id: xzac631.c,v 1.2 2020/11/03 17:56:29 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 17:56:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac631.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* J.B.	10/12/07	: Creation a partir xzac63 DEM 662
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac631.h"

static char *version = "@(#)xzac631.c	1.1 1.1 12/05/06 : xzac631" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC631SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC631_Commande_Fin_Signalisation ( XDY_Eqt       va_NumEqt_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Horodate  va_HeureLancementFin_in,
                                 XDY_Booleen   va_Manuelle_in )

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de Tunnel
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

	static char *version = "@(#)xzac631.c	1.1 1.1 12/05/06 : XZAC631_Commande_Fin_Tunnel" ;

	CS_CHAR *rpc_name = XZACC_XZAC631_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC631_NB_PARM_RETURN;
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
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnelretourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC631_Commande_Fin_Tunnel ");
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        /*A
        ** Recuperation du site de la machine
        */
        if ( (retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzac631 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzac631 : Recuperation du Nom du site %s.",pl_NomSite);

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumeroTunnel_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: asql_ctparam_tinyint_input(va_Site_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_HeureLancementFin_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_Manuelle_in,"@va_Manuelle_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: asql_ctparam_bit_input(va_Manuelle_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac100: asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac631: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac631 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac631 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac631: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC631_Commande_Fin_Tunnel retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




