/*E*/
/* Fichier : $Id: xzae568.c,v 1.1 2016/09/29 15:32:35 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:32:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae568.c
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

#include "xzae568.h"

static char *version = "@(#)xzae568.c	1.3 03/10/95 XZAE568_Get_FMC_Saga";

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

int XZAE568_Get_FMC_Saga ( char *va_numero_saga, XDY_Entier	*va_Numero_out,
									XDY_Octet  *va_Cle_out,
									XDY_Entier     *va_Numero_Tete_out)
				

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
* Appelle XZAE568SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "@(#)xzae568.c	1.3 26/01/12 XZAE568_Get_FMC_Saga";
	
	CS_CHAR 	*vl_RpcName =XZAE568C_RPC_NAME;
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

    int             vl_num_Tab_donnees2 = XZAE568C_NB_PARM_RETURN;
     tg_Colonne_donnees2 Tab_donnees2[XZAE568C_NB_PARM_RETURN];

     XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae568: numero saga %s",va_numero_saga);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae568: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d",
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae568: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d",
				retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_numero_saga,
						"@va_numero_saga_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae568: asql_ctparam_int_input(va_numero_saga) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d",
			   retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
		
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae568: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d",
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
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga: ROW RESULTS %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE568_Get_FMC_Saga: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAE568_Get_FMC_Saga: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE568_Get_FMC_Saga: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE568_Get_FMC_Saga: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE568_Get_FMC_Saga retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}
	
	*va_Numero_out=0;
	*va_Cle_out=0;
	*va_Numero_Tete_out = 0;
	if (vl_Nb_Ligne_Lue > 0) {
	vl_num			= * (XDY_Entier *) vl_Tab_Ligne[0][0].pt_value;
	vl_cle			= * (XDY_Octet *) vl_Tab_Ligne[0][1].pt_value;
	*va_Numero_out = vl_num;
	*va_Cle_out = vl_cle;

	if (vl_Nb_Ligne_Lue > 1) {
		vl_num                  = * (XDY_Entier *) vl_Tab_Ligne[1][0].pt_value;
		*va_Numero_Tete_out = vl_num;
	}
	}
	XZST_03EcritureTrace(	XZSTC_WARNING,"num=%d %d (%d)",*va_Numero_out,*va_Cle_out,*va_Numero_Tete_out);

	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE568_Get_FMC_Saga retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(connection, cmd);

	return(XDC_OK);
}


