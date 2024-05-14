/*Fichier : $Id: xzap821.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap821.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
*  GGY	04/12/23	: Creation (DEM483)
------------------------------------------------------*/
/* fichiers inclus */
#include "xzap821.h"

static char *version = "xzap821.c : xzap821" ;

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP82SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAP821_Ajout_Prop_PIC (	XDY_Mot 			NumPA_In,
								XDY_Mot 			Ordre_In,
								XDY_District		District_In,
								XDY_Phrase			Remarque_In,
								XDY_Octet 			PrioriteEvt_In,
								XDY_Eqt 			NoEqt_In,
								XDY_Commentaire		Explication_In,
								XDY_Nom				Libelle_In,
								XDY_Bandeau_PIC		Bandeau_In,
								XDY_Booleen			Flash_In,
								XDY_Picto_PIC		Picto_In,
								XDY_Booleen			Realisable_In )
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 			NumPA_In
* XDY_Mot 			Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 			NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_Bandeau_PIC	Bandeau_In
* XDY_Booleen		Flash_In
* XDY_Picto_PIC		Picto_In
* XDY_Nom			Libelle_In
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

	static char *version = "xzap821.c : XZAP821_Ajout_Prop_PIC";

	CS_CHAR 		*rpc_name = XZAOC_XZAP821_RPC_NAME;
	CS_CONNECTION   *connection = XZAGV_Connexion;
	CS_COMMAND		*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier		SPstatus;	

	XZST_03EcritureTrace (XZSTC_INTERFACE, "IN : XZAP821_Ajout_Prop_PIC : NumPA_In=%d Ordre_In=%d District_In=%d PrioriteEvt_In=%d NoEqt_In=%d Explication=%s Bandeau=%s Flash=%d Picto=%s Libelle=%s",
																		NumPA_In,
																		Ordre_In,
																		District_In,
																		PrioriteEvt_In,
																		NoEqt_In,
																		Explication_In,
																		Bandeau_In, 
																		Flash_In, 
																		Picto_In, 
																		Libelle_In);

	/* Initialisation de la commande */
	if ((retcode = asql_cmd_alloc (connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((retcode = ct_command (cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc. */

	strcpy(datafmt.name ,"@NumPA_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&NumPA_In, CS_SIZEOF (CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(@NumPA_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Ordre_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&Ordre_In, CS_SIZEOF (CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(@Ordre_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@District_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&District_In, CS_SIZEOF (CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(District_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Remarque_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Remarque_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.format    = CS_FMT_PADBLANK; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)Remarque_In, strlen (Remarque_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Remarque_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@PrioriteEvt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&PrioriteEvt_In, CS_SIZEOF (CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(PrioriteEvt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@NoEqt_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&NoEqt_In, CS_SIZEOF (CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(NoEqt_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Explication_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Explication_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)Explication_In, strlen (Explication_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap82: ct_param(Explication_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Bandeau_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Bandeau_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)Bandeau_In, strlen (Bandeau_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Bandeau_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Flash_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&Flash_In, CS_SIZEOF (CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Flash_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Picto_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Picto_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)Picto_In, strlen (Picto_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Picto_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
		
	strcpy(datafmt.name ,"@Libelle_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = strlen(Libelle_In);
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)Libelle_In, strlen (Libelle_In), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Libelle_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@Realisable_In");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;
	if ((retcode = ct_param (cmd, &datafmt, (CS_VOID *)&Realisable_In, CS_SIZEOF (CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_param(Realisable_In) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/* Envoie de la commande au Sql server */
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Traitement du resultat */
	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/* Impression de l entete en fonction du type de resultat. */
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzap821: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzap821 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap821 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzap821 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				break;

				case CS_MSG_RESULT:
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/* Pas de colonnes retournees. */
					break;

				case CS_CMD_DONE:
					/* Commande effectuee : positionnement des parametres de retour. */
					break;

				case CS_CMD_FAIL:
					/* Le serveur SQL a detecte une ereeur en executant notre commande. */
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
					break;

				default:
					/* Il s est passe quelque chose d inattendu. */
					XZST_03EcritureTrace(XZSTC_WARNING,"xzap821: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/* Retourne le resultat et les parametres de sorties renseignes */
 	XZST_03EcritureTrace (XZSTC_INTERFACE, "OUT : XZAP821_Ajout_Prop_PIC retourne %d", retcode);
 	asql_clean_cmd (connection, cmd);
	return (XDC_OK);
}
