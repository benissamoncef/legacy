/*E*/
/*  Fichier : 	$Id: xzae24.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/06 18:06:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae24.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Plan d'actions
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       31 Aug 1994     : Creation
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : Cte. pour arg. 5 de 'ct_param' pour Sybase 15.x  1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae24.h"

static char *version = "$Id: xzae24.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $ : xzae24" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE24SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE24_Decrire_Travaux_PA (	XDY_Horodate 	va_Horodate,
				XDY_Evt 	va_NumEvt,
				XZAET_Travaux 	*va_Travaux )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate 		va_Horodate_in
* XDY_Evt 		va_HoroDebC_in
*
* ARGUMENTS EN SORTIE :
*
* XZAET_Travaux 	va_Travaux
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

	static char *version = "$Id: xzae24.c,v 1.5 2010/12/06 18:06:19 gesconf Exp $ : XZAE24_Decrire_Travaux_PA" ;

	CS_CHAR *rpc_name =XZAEC_XZAE24_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE24_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt et XZAET_Travaux
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	
	XDY_Booleen	va_Nature_out;
	XDY_Octet	va_Type_out;
	XDY_Booleen	va_Continu_out;
	XDY_Horodate	Nb_Second_DebC;		
	CS_DATETIME	va_HoroDebC;
	XDY_Horodate	Nb_Second_FinC;
	CS_DATETIME	va_HoroFinC;
	XDY_Booleen	va_Mobile_out;
	XDY_Autoroute	va_Autoroute_out;
	XDY_PR		va_PRteteM_out;
	XDY_PR		va_PRqueueM_out;
	XDY_Sens	va_SensM_out;
	XDY_PR		va_PRtete_out;
	XDY_Booleen	va_VoiesRet_out;
	XDY_Entier	va_Longueur_out;
	XDY_Entier	va_LongueurM_out;
	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines du format Sybase au format Unix
	*/
	
	CS_DATETIME	va_Horodate_in;
	CS_DATETIME	va_HoroDebC_out;
	CS_DATETIME	va_HoroFinC_out;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in)) != XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE24_Decrire_Travaux_PA : va_Horodate_in=%d va_NumEvt_in=%d va_CleEvt_in=%d",va_Horodate_in,va_NumEvt_in,va_CleEvt_in);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	strcpy(datafmt.name ,"@va_Horodate_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_DATETIME_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Horodate_in,
			CS_SIZEOF(CS_DATETIME), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
        
        strcpy(datafmt.name ,"@va_NumEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; /* Utiliser des types Sybase */
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; /* Pas un code de retour de rp*/
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_NumEvt_in,
			CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(@va_NumEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	strcpy(datafmt.name ,"@va_CleEvt_in");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	datafmt.status    = CS_INPUTVALUE; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_CleEvt_in,
			CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(@va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
        
        strcpy(datafmt.name ,"@va_Nature_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[0].pt_value = (CS_VOID *)&va_Nature_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Nature_out,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Nature_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Type_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[1].pt_value = (CS_VOID *)&va_Type_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Type_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Type_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Continu_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[2].pt_value = (CS_VOID *)&va_Continu_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Continu_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Type_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	strcpy(datafmt.name ,"@va_HoroDebC_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_DATETIME_TYPE; 
	datafmt.maxlength = 255;
	Tab_donnees2[3].pt_value = (CS_VOID *)&va_HoroDebC_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, NULL, 0, -1)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_HoroDebC_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_HoroFinC_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_DATETIME_TYPE; 
	datafmt.maxlength = 255;
	Tab_donnees2[4].pt_value = (CS_VOID *)&va_HoroFinC_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, NULL, 0, -1)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_HoroFinC_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Mobile_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[5].pt_value = (CS_VOID *)&va_Mobile_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Mobile_out,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Mobile_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Autoroute_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[6].pt_value = (CS_VOID *)&va_Autoroute_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Autoroute_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Autoroute_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_PRteteM_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[7].pt_value = (CS_VOID *)&va_PRteteM_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PRteteM_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_PRteteM_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_PRqueueM_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[8].pt_value = (CS_VOID *)&va_PRqueueM_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PRqueueM_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_PRqueueM_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_SensM_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_TINYINT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[9].pt_value = (CS_VOID *)&va_SensM_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_SensM_out,
		CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_SensM_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_PRtete_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[10].pt_value = (CS_VOID *)&va_PRtete_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_PRtete_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_PRtete_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_VoiesRet_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_BIT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[11].pt_value = (CS_VOID *)&va_VoiesRet_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_VoiesRet_out,
		CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_VoiesRet_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_Longueur_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[12].pt_value = (CS_VOID *)&va_Longueur_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_Longueur_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_Longueur_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	strcpy(datafmt.name ,"@va_LongueurM_out");
	datafmt.namelen   = CS_NULLTERM;
	datafmt.datatype  = CS_INT_TYPE; 
	datafmt.maxlength = CS_UNUSED;
	Tab_donnees2[13].pt_value = (CS_VOID *)&va_LongueurM_out;
	datafmt.status    = CS_RETURN; 
	datafmt.locale    = NULL;

	if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)&va_LongueurM_out,
		CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_param(va_LongueurM_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae24: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae24 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae24 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae24 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				

				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free (Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						free (Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free (Tab_donnees2);
	   				return (XDC_NOK);
				}
	}


	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_Travaux->Nature    = va_Nature_out;
	va_Travaux->Type      = va_Type_out;
	va_Travaux->Continu   = va_Continu_out;
	va_Travaux->Mobile    = va_Mobile_out;
	va_Travaux->Autoroute = va_Autoroute_out;
	va_Travaux->PRqueueM  = va_PRqueueM_out;
	va_Travaux->PRteteM   = va_PRteteM_out;
	va_Travaux->SensM     = va_SensM_out;
	va_Travaux->PRtete    = va_PRtete_out;
	va_Travaux->VoiesRet  = va_VoiesRet_out;
	va_Travaux->Longueur  = va_Longueur_out;
	va_Travaux->LongueurM = va_LongueurM_out;

	
	if ((retcode = asql_date_Sybase2Ux(va_HoroDebC_out,&Nb_Second_DebC))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: asql_date_Sybase2Ux(va_HoroDebC_Out,&Nb_Second_DebC) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	} 
	

	va_Travaux->HoroDebC  = Nb_Second_DebC;
	
	if ((retcode = asql_date_Sybase2Ux(va_HoroFinC_out,&Nb_Second_FinC))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae24: asql_date_Sybase2Ux(va_HoroDebC_Out,&Nb_Second_FinC) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	va_Travaux->HoroFinC  = Nb_Second_FinC;
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE24_Decrire_Travaux_PA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
