/*E*/
/*  Fichier : 	$Id: xzap44.c,v 1.1 2007/12/13 10:25:59 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 10:25:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzap44.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/01/07	creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzap44.h"

static char *version = "$Id: xzap44.c,v 1.1 2007/12/13 10:25:59 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2007/12/13 10:25:59 $";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP44SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAP44_Ajout_Dest_Mail (	XDY_Mot 	NumPA_In,
				XDY_Mot 	Ordre_In,
				XDY_PR		PR_In,
				XDY_Autoroute	NumeroAutoroute_In,
				XDY_Octet	TypeDAstreinte_In,
				XDY_Commentaire	Explication_In )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_PR		PR_In
* XDY_Autoroute		NumeroAutoroute_In
* XDY_Octet		TypeDAstreinte_In
* XDY_Commentaire       Explication_in
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
------------------------------------------------------*/
{

	static char *version = "@(#)xzap44.c	1.12 : XZAP44_Ajout_Dest_Mail" ;

	CS_CHAR *rpc_name =XZAOC_XZAP44_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : %s : NumPA_In=%d Ordre_In=%d PR_In=%d NumeroAutoroute_In=%d TypeDAstreinte_In=%d Explication_In='%s'",version,NumPA_In,Ordre_In,PR_In,NumeroAutoroute_In,TypeDAstreinte_In,Explication_In);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/


        strcpy(datafmt.name ,"@NumPA_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&NumPA_In,
			CS_SIZEOF(CS_SMALLINT), CS_UNUSED)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_param(@NumPA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Ordre_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Ordre_In,
			CS_SIZEOF(CS_SMALLINT), CS_UNUSED)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_param(@Ordre_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
        
        strcpy(datafmt.name ,"@PR_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&PR_In,
			CS_SIZEOF(CS_INT), CS_UNUSED)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_param(@PR_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@NumeroAutoroute_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&NumeroAutoroute_In,
			CS_SIZEOF(CS_TINYINT), CS_UNUSED)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_param(@NumeroAutoroute_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	strcpy(datafmt.name ,"@TypeDAstreinte_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&TypeDAstreinte_In,
			CS_SIZEOF(CS_TINYINT), CS_UNUSED)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_param(@TypeDAstreinte_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)Explication_In,"@Explication_In"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: asql_ctparam_char_input(Explication_In) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzap44: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzap44 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap44 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap44 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap44: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP44_Ajout_Dest_Mail retourne %d", retcode);
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
