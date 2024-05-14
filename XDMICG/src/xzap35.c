/*E*/
/*  Fichier : 	$Id: xzap35.c,v 1.1 2021/06/01 07:44:01 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/01 07:44:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap35.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/05/21	Creation DEM-SAE244 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzap35.h"

static char *version = "$Id: xzap35.c,v 1.1 2021/06/01 07:44:01 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2021/06/01 07:44:01 $ : xzap35" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP35SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAP35_ajout_Prop_SONO(XDY_Mot 		va_NumPA_in,
					XDY_Mot 		va_Ordre_in,
					XDY_District		va_District_in,
					XDY_Eqt 		va_NoEqt_in,
					XDY_Octet		va_PrioriteEvt_in,
					XDY_Commentaire		va_Remarque_in,
					XDY_Texte		va_Explication_in,
					XDY_Booleen		va_Realisable_in,
					XDY_Texte		va_Message_in,
					XDY_Autoroute		va_Autoroute_in,
					XDY_Sens		va_Sens_in,
					XDY_PR			va_PR_Debut_in,
					XDY_PR			va_PR_Fin_in,
					XDY_Nom			va_Libelle_in )


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Mot 		va_Ordre_in
* XDY_District		va_District_in
* XDY_Eqt 		va_NoEqt_in
* XDY_Octet		va_PrioriteEvt_in
* XDY_Commentaire	va_Remarque_in
* XDY_Texte		va_Explication_in
* XDY_Booleen		va_Realisable_in
* XDY_Texte		va_Message_in
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PR_Debut_in
* XDY_PR		va_PR_Fin_in
* XDY_Nom		va_Libelle_in
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
	CS_CHAR *rpc_name =XZAOC_XZAP35_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAP35_ajout_Prop_SONO : va_NumPA_in:%d\nva_Ordre_in:%d\nva_District_in:%d\nva_NoEqt_in:%d\nva_PrioriteEvt_in:%d\nva_Remarque_in:%s\nva_Explication_in:%s\nva_Realisable_in:%d\nva_Message_in:%s\nva_Autoroute_in:%d\nva_Sens_in:%d\nva_PR_deb_in:%d\nva_PR_fin_in:%d\nva_Libelle_in:%s",
						va_NumPA_in,
						va_Ordre_in,
						va_District_in,
						va_NoEqt_in,
						va_PrioriteEvt_in,
						va_Remarque_in,
						va_Explication_in,
						va_Realisable_in,
						va_Message_in,
						va_Autoroute_in,
						va_Sens_in,
						va_PR_Debut_in,
						va_PR_Fin_in,
						va_Libelle_in);	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);					
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/


        strcpy(datafmt.name ,"@va_NumPA_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;												
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_NumPA_in,
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(@va_NumPA_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Ordre_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Ordre_in,
			CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(@va_Ordre_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	

	strcpy(datafmt.name ,"@va_District_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_District_in,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_District_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_NoEqt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_SMALLINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_NoEqt_in,
		CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_NoEqt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}	
	
	strcpy(datafmt.name ,"@va_PrioriteEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PrioriteEvt_in,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_PrioriteEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	strcpy(datafmt.name ,"@va_Remarque_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 50;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Remarque_in,
			strlen(va_Remarque_in), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Remarque_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d",retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	strcpy(datafmt.name ,"@va_Explication_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 250;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Explication_in,
		strlen(va_Explication_in), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Explication_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	strcpy(datafmt.name ,"@va_Realisable_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Realisable_in,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Realisable_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Message_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 31;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Message_in,
		strlen(va_Message_in), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Message_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Autoroute_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Autoroute_in,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Autoroute_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Sens_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Sens_in,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Sens_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_PR_Debut_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PR_Debut_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(@va_PR_Debut_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	}

	strcpy(datafmt.name ,"@va_PR_Fin_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PR_Fin_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(@va_PR_Fin_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	}

	strcpy(datafmt.name ,"@va_Libelle_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 25;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Libelle_in,
		strlen(va_Libelle_in), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_param(va_Libelle_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap35: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP35_Ajout_Prop_SONO: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP35_Ajout_Prop_SONO : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP35_Ajout_Prop_SONO : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP35_Ajout_Prop_SONO : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAP35_Ajout_Prop_SONO: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAP35_Ajout_Prop_SONO: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAP35_Ajout_Prop_SONO: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP35_Ajout_Prop_SONO retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
