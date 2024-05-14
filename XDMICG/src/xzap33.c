/*E*/
/*  Fichier : 	$Id: xzap33.c,v 1.3 2013/02/12 11:19:51 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2013/02/12 11:19:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap33.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG       31 Aug 2005     : Creation
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : Cte. pour arg. 5 de 'ct_param' pour Sybase 15.x  1.2
* PNI		12/02/13 : Correction trace plantant tdrol.x v1.3 FI 1822
------------------------------------------------------*/

/* fichiers inclus */

#include "xzap33.h"

static char *version = "$Id: xzap33.c,v 1.3 2013/02/12 11:19:51 pc2dpdy Exp $ $Revision: 1.3 $ $Date: 2013/02/12 11:19:51 $ : xzap33" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP33SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAP33_Ajout_Prop_BAD   (   XDY_Mot 	NumPA_In,
				XDY_Mot 	Ordre_In,
				XDY_District	District_In,
				XDY_Commentaire	Remarque_In,
				XDY_Octet 	PrioriteEvt_In,
				XDY_Eqt 	NoEqt_In,
				XDY_Commentaire	Explication_In,
				XDY_Nom		Libelle_In,
				XDY_Scenario	Scenario_In,
				XDY_Booleen	Realisable_In )


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Commentaire	Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 		NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_Nom		Libelle_In
* XDY_Scenario		Scenario_In
* XDY_Booleen		Realisable_In
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


	CS_CHAR *rpc_name =XZAOC_XZAP33_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAP33_Ajout_Prop_BAD : NumPA_In=%d Ordre_In=%d District_In=%d Remarque_In=%s PrioriteEvt_In=%d NoEqt_In=%d Explication_In=%s Libelle_In=%s Scenario_In=%s Realisable_In=%d",NumPA_In,Ordre_In,District_In,Remarque_In ,PrioriteEvt_In,NoEqt_In,Explication_In ,Libelle_In ,Scenario_In ,Realisable_In);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
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
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(@NumPA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
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
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(@Ordre_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	

	strcpy(datafmt.name ,"@District_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&District_In,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(District_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	strcpy(datafmt.name ,"@Remarque_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 255;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Remarque_In,
			strlen(Remarque_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Remarque_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@PrioriteEvt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&PrioriteEvt_In,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(PrioriteEvt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	strcpy(datafmt.name ,"@NoEqt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&NoEqt_In,
		CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(NoEqt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Explication_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 255;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Explication_In,
		strlen(Explication_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Explication_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	

	strcpy(datafmt.name ,"@Libelle_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 255;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Libelle_In,
		strlen(Libelle_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Libelle_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Scenario_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 255;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Scenario_In,
		strlen(Scenario_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Scenario_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@Realisable_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&Realisable_In,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_param(Realisable_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap03: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzap03: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzap03 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap03 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap03 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP33_Ajout_Prop_BAD retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
