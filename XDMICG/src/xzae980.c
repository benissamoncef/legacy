/*E*/
/*  Fichier : 	$Id: xzae980.c,v 1.1 2012/05/29 10:12:12 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2012/05/29 10:12:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae980.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU       14/02/12     : DEM 1015 - Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae980.h"

static char *version = "@(#)xzae980.c	1.2 1.2 12/06/10";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE980SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE980_Recherche_FMC_Liee (
				XDY_Evt_Commu 	   va_Id_Commu,
				XDY_Synthese_Commu va_synthese,
				XDY_Tendance_Commu va_tendance,
				XDY_Evt 	       *pa_NumEvt,
				XDY_Booleen	       *pa_Encours,
				XDY_Synthese_Commu *pa_Synthese,
				XDY_Tendance_Commu *pa_Tendance)
				

/*
* Arguments en entree
* XDY_Evt_Commu        va_Evt_Commu_in		: evenement communautaire
*
* Arguments en sortie
* XDY_Evt          va_NumEvt : FMC
* XDY_Booleen         va_Encours   : indicateur en cours
* XDY_Tendance_Comm   va_Tendance  : tendance out
* XDY_Synthese_Commu  va_Syntheset  : synthese out
*
* Code retour
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{

	static char *version = "@(#)xzae980.c	1.2 1.2 12/06/10" ;

	CS_CHAR *rpc_name =XZAEC_XZAE980_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection = XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE980_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	CS_INT vl_numero;
	CS_TINYINT vl_cle;

	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	XDY_Entier	va_NumEvt_out;
	XDY_Octet	va_CleEvt_out;
	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines au format Sybase
	*/
	
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	
	XZST_03EcritureTrace(	XZSTC_INFO,
			"IN : XZAE980_Recharche_FMC_Liee %s : va_Id_Commu=%d", rpc_name, va_Id_Commu);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	**
	** @va_Evt_Commu_in	int	= null,
	** @va_NumeroEvt_out   int = null output,
	** @va_CleEvt_out	tinyint	= null output,
	** @va_encours_out	tinyint	= 0 output,
	** @va_tendance_out    int = null output,
	** @va_synthese_out    int = null output
	**
	*/

	if ((retcode = asql_ctparam_int_input(cmd, (CS_INT *)&va_Id_Commu, "@va_Evt_Commu_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_Evt_Commu_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd, (CS_INT *)&va_synthese, "@va_synthese_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_synthese_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd, (CS_INT *)&va_tendance, "@va_tendance_in") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_tendance_in) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
        
        
	Tab_donnees2[0].pt_value = (CS_VOID *)&vl_numero;
	if ((retcode = asql_ctparam_int_output(cmd, (CS_INT *)&vl_numero, "@va_NumeroEvt_out") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_NumeroEvt_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	Tab_donnees2[1].pt_value = (CS_VOID *)&vl_cle;
	if ((retcode = asql_ctparam_tinyint_output(cmd, (CS_TINYINT *)&vl_cle, "@va_CleEvt_out") != CS_SUCCEED)) 
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_CleEvt_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	Tab_donnees2[2].pt_value = (CS_VOID *)pa_Encours;
	if ((retcode = asql_ctparam_tinyint_output(cmd, (CS_TINYINT *)pa_Encours, "@va_encours_out") != CS_SUCCEED) )
	{

		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(@va_encours_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	Tab_donnees2[3].pt_value = (CS_VOID *)pa_Tendance;
	if ((retcode = asql_ctparam_int_output(cmd, (CS_INT *)pa_Tendance, "@va_tendance_out") != CS_SUCCEED) )
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(va_tendance_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	Tab_donnees2[4].pt_value = (CS_VOID *)pa_Synthese;
	if ((retcode = asql_ctparam_int_output(cmd, (CS_INT *)pa_Synthese, "@va_synthese_out") != CS_SUCCEED) )
	{
	
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_param(va_synthese_out) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae980: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzae980 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae980 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae980 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE980_Recharche_FMC_Liee retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				

				/*
				** Les trois types de resultats sont fetchables.
				** Since the result model for rpcs and rows have
				** been unified in the New Client-Library, we
				** will use the same routine to display them
				*/
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						free(Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae980: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE980_Recharche_FMC_Liee retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/

 	XZST_03EcritureTrace(XZSTC_DEBUG1, "XZAE980_Recharche_FMC_Liee %d %d %d %d %d ", 
			vl_numero, vl_cle,*pa_Encours,*pa_Tendance,*pa_Synthese);
	pa_NumEvt->numero = vl_numero;
	pa_NumEvt->cle = vl_cle;

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 		
 	XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : XZAE980_Recharche_FMC_Liee retourne %d ", retcode );
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
