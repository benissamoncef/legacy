/*E*/
/*  Fichier : 	$Id: xzap36.c,v 1.1 2021/07/20 12:25:10 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/07/20 12:25:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap36.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/05/21	Creation DEM-SAE155 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzap36.h"

static char *version = "$Id: xzap36.c,v 1.1 2021/07/20 12:25:10 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2021/07/20 12:25:10 $ : xzap36" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP36SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAP36_ajout_Prop_IMU(		XDY_Mot 		va_NumPA_in,
					XDY_Mot 		va_Ordre_in,
					XDY_District		va_District_in,
					XDY_Eqt 		va_NoEqt_in,
					XDY_Octet		va_PrioriteEvt_in,
					XDY_Commentaire		va_Remarque_in,
					XDY_Texte		va_Explication_in,
					XDY_Booleen		va_Realisable_in,
					XDY_Texte		va_Chaine_in,
					XDY_Entier		va_Intervalle_in,
					XDY_Entier		va_Cycles_in,
					XDY_Entier		va_Duree_in,
					XDY_District		va_Sit_in,
					XDY_Nom			va_Libelle_in )


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in,
* XDY_Mot 		va_Ordre_in,
* XDY_District		va_District_in,
* XDY_Eqt 		va_NoEqt_in,
* XDY_Octet		va_PrioriteEvt_in,
* XDY_Commentaire	va_Remarque_in,
* XDY_Texte		va_Explication_in,
* XDY_Booleen		va_Realisable_in,
* XDY_Texte		va_Chaine_in,
* XDY_Entier		va_Intervalle_in,
* XDY_Entier		va_cycles_in,
* XDY_Entier		va_duree_in,
* XDY_District		va_sit_in,
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
	CS_CHAR *rpc_name =XZAOC_XZAP36_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAP36_Ajout_Prop_IMU : va_NumPA_in:%d\nva_Ordre_in:%d\nva_District_in:%d\nva_NoEqt_in:%d\nva_PrioriteEvt_in:%d\nva_Remarque_in:%s\nva_Explication_in:%s\nva_Realisable_in:%d\nva_Chaine_in:%s\nva_Intervalle_in:%d\nva_Cycles_in:%d\nva_duree_in:%d\nva_sit_in:%d\nva_Libelle_in:%s",
							va_NumPA_in,
							va_Ordre_in,
							va_District_in,
							va_NoEqt_in,
							va_PrioriteEvt_in,
							va_Remarque_in,
							va_Explication_in,
							va_Realisable_in,
							va_Chaine_in,
							va_Intervalle_in,
							va_Cycles_in,
							va_Duree_in,
							va_Sit_in,
							va_Libelle_in);	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);					
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(@va_NumPA_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(@va_Ordre_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_District_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d",retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_NoEqt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_PrioriteEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Remarque_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d",retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Explication_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Realisable_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Chaine_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_CHAR_TYPE; 
	datafmt.maxlength = 31;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)va_Chaine_in,
		strlen(va_Chaine_in), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Chaine_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Intervalle_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Intervalle_in,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Intervalle_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Cycles_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Cycles_in,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Cycles_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_Duree_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Duree_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(@va_Duree_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	}

	strcpy(datafmt.name ,"@va_Sit_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Sit_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(@va_sit_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_param(va_Libelle_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzap36: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP36_ajout_Prop_IMU: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP36_ajout_Prop_IMU : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP36_ajout_Prop_IMU : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAP36_ajout_Prop_IMU : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAP36_ajout_Prop_IMU: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAP36_ajout_Prop_IMU: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAP36_ajout_Prop_IMU: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP36_ajout_Prop_IMU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
