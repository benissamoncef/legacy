/*E*/
/* Fichier : $Id: xzaf10.c,v 1.1 1995/01/14 17:38:44 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/14 17:38:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf10.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       12/01/95     : Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf10.h"

static char *version = "$Id: xzaf10.c,v 1.1 1995/01/14 17:38:44 torregrossa Exp $ XZAF10_Rechercher_Evts_Actions";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Rechercher les evenements et les actions clos en cours a
*  l'heure specifiee pour mettre a jour les tables de formation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAF10_Rechercher_Evts_Actions ( XDY_Horodate	va_Horodate)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAF10SP
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaf10.c,v 1.1 1995/01/14 17:38:44 torregrossa Exp $ XZAF10_Rechercher_Evts_Actions";
	
	CS_CHAR 	*vl_RpcName =XZAFC_XZAF10_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int 		vl_NumTabDonnees2 = XZAFC_XZAF10_NB_PARM_RETURN;
	int		i;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	CS_DATETIME	vl_Horodate_in;
	
	
        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF10_Rechercher_Evts_Actions : vl_Horodate_in=%d va_Time_in=%d ",
			vl_Horodate_in.dtdays,vl_Horodate_in.dttime);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_Horodate_in,
						"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf10: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf10 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf10 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf10 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne 1");
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: asql_fetch_data* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf10: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF10_Rechercher_Evts_Actions retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF10_Rechercher_Evts_Actions \n");
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

