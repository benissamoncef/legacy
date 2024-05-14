/*E*/
/* Fichier : $Id: xzaq.c,v 1.3 1995/09/20 19:26:38 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/09/20 19:26:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAQ * FICHIER xzaq01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface indice qualite
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit       18/01/95     : Creation
* gaborit       04/05/95	: modif trace + test booleen (1.2)
* gaborit       20/09/95	: modif etat_in en XDY_Octet avec :
				 XZSEC_SANS_ETAT et XZSEC_ETAT_OK -> XDC_OK
				 XZSEC_ETAT_NOK -> XDC_NOK
				 (une tache sans etat doit etre OK pour le calcul) (1.3)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaq.h"
#include "xzse.h"

static char *version = "$Id: xzaq.c,v 1.3 1995/09/20 19:26:38 gesconf Exp $ : xzaq01" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  met a jour l'indice qualite
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAQ01_Mise_A_Jour_Etat (	XDY_Nom		va_NomSite_in,
				XDY_Nom		va_NomMachine_in,
				XDY_Nom		va_NomTache_in,
				XDY_Octet 	va_Etat_in)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Nom	@va_NomSite_in		: Nom du site
* XDY_Nom       @va_NomMachine_in       : Nom de la machine
* XDY_Nom       @va_NomTache_in         : Nom de la tache 
* XDY_Octet	@va_Etat_in		: Etat de la tache : XZSEC_ SANS_ETAT ETAT_OK ETAT_NOK
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaq.c,v 1.3 1995/09/20 19:26:38 gesconf Exp $ : xzaq01" ;
	
	CS_CHAR *rpc_name =XZAQC_XZAQ01_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,"IN : %s site %s machine %s tache %s etat %d",version,va_NomSite_in,va_NomMachine_in,va_NomTache_in,va_Etat_in );

	/*
	** calcul de l'etat booleen a partir de l'etat XZEC
	*/

	if (va_Etat_in==XZSEC_ETAT_NOK)
	{
		va_Etat_in=XDC_NOK;
	}
	else
	{
		va_Etat_in=XDC_OK;
	}
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_cmd_alloc() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d", version,retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomSite_in,"@va_NomSite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_ctparam_char_input a echoue",version);
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_ctparam_char_input a echoue",version);
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomTache_in,"@va_NomTache_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_ctparam_char_input a echoue",version);
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
         
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_Etat_in,"@va_Etat_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_ctparam_bit_input a echoue",version);
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_send() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d", version,retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"%s: ROW RESULTS",version);
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"%s : PARAMETER RESULTS\n",version);
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_STATUS_RESULT = %ld",version, retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : STATUS REUSULT PROCEDURE STOCKEE = %ld",version, SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne 1",version);
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_fetch_data* a echoue",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_res_info(msgtype) a echoue",version);
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
						asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version ,retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}
	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  met a jour l'indice qualite
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAQ02_Arret_SUPERV ()
				

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaq.c,v 1.3 1995/09/20 19:26:38 gesconf Exp $ : xzaq02" ;
	
	CS_CHAR *rpc_name =XZAQC_XZAQ02_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,"IN : %s",version);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_cmd_alloc() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d", version,retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_send() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d", version,retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"%s: ROW RESULTS",version);
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"%s : PARAMETER RESULTS\n",version);
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_STATUS_RESULT = %ld",version, retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : STATUS REUSULT PROCEDURE STOCKEE = %ld",version, SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne 1",version);
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_fetch_data* a echoue",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_res_info(msgtype) a echoue",version);
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
						asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version ,retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}
	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne %d",version, retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}

