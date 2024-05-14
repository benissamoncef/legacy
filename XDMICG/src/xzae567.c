/*E*/
/* Fichier : $Id: xzae567.c,v 1.1 2012/07/04 17:50:49 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:50:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae567.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* VR  	30/01/12     : Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae567.h"
#include "xzae565.h"

static char *version = "@(#)xzae567.c	1.3 03/10/95 XZAE567_Get_FMC_Regulation";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Permet de recuperer une FMC r�gulation en cours sur une zone donn�e
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE567_Get_FMC_Regulation ( 	XDY_Horodate va_horodate_in,
									XDY_Eqt 	va_zone_in,
									XDY_Entier	*va_Numero_out,
									XDY_Octet  *va_Cle_out,
									XDY_Entier              *va_Contexte_out,
									XDY_Entier              *va_Scenario_out,
									XDY_Entier              *va_Scenario_cible_out,
									XDY_Entier              *va_Mesure_out,
									XDY_Entier              *va_Indicateur_Horaire_out,
									XDY_Entier              *va_Zone_out,
									char*                   va_Transitoire_out)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Basedd	va_Base
* XDY_Entier	va_Numero
* XDY_Entier	va_Cle_in
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*  XDC_ARG_INV	: argument invalide
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAE567SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "@(#)xzae567.c	1.3 26/01/12 XZAE567_Get_FMC_Regulation";
	
	CS_CHAR 	*vl_RpcName =XZAE567C_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	tg_row_result	vl_Tab_Ligne;
	int		i,
			vl_Nb_Ligne_Lue,
			vl_Nb_Col,
			vl_Status;
	XDY_Entier	SPstatus;
	CS_INT vl_num;
	CS_TINYINT	vl_cle;

    int             vl_num_Tab_donnees2 = XZAE567C_NB_PARM_RETURN;
     tg_Colonne_donnees2 Tab_donnees2[XZAE567C_NB_PARM_RETURN];

     XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae567: zone %d",va_zone_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae567: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d",
				retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, vl_RpcName,
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae567: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d",
				retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_zone_in,
						"@va_zone_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae567: asql_ctparam_smallint_input(va_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d",
			   retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
		
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae567: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d",
				retcode);
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
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation: ROW RESULTS %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE567_Get_FMC_Regulation: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE567_Get_FMC_Regulation: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE567_Get_FMC_Regulation: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE567_Get_FMC_Regulation: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE567_Get_FMC_Regulation retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}
	
	if (vl_Nb_Ligne_Lue > 0) {
	vl_num			= * (XDY_Entier *) vl_Tab_Ligne[0][0].pt_value;
	vl_cle			= * (XDY_Octet *) vl_Tab_Ligne[0][1].pt_value;
	*va_Numero_out = vl_num;
	*va_Cle_out = vl_cle;
	XZST_03EcritureTrace(	XZSTC_WARNING,"num=%d %d",vl_num,vl_cle);

	if (XZAE565_Lire_FMC_Regulation(*va_Numero_out, *va_Cle_out,
			va_horodate_in, va_Contexte_out, va_Scenario_out, va_Scenario_cible_out, va_Mesure_out, va_Indicateur_Horaire_out,
			va_Zone_out, va_Transitoire_out) != XDC_OK)
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae567: pb appel a xzae565");
	}

	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE567_Get_FMC_Regulation retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(connection, cmd);

	return(XDC_OK);
}


