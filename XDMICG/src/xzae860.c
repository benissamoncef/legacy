/*E*/
/*  Fichier : 	$Id: xzae860.c,v 1.1 2009/10/22 15:53:14 pc2dpdy Exp $  Release : $Revision: 1.1 $        Date : $Date: 2009/10/22 15:53:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae860.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	30/11/08	creation DEM895
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae860.h"

static char *version = "@(#)xzae860.c	1.1 1.1 08/22/96";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE860SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE860_Lire_Config_Terrain (	XDY_Autoroute 	va_NumAutoroute_in,
				XDY_PR 	va_PRDebut_in,
				XDY_Sens	va_Sens_in,
				XDY_PR	va_PRFin_in,
				XDY_Octet	*va_VR_out,
				XDY_Octet       *va_VM2_out,
				XDY_Octet       *va_VM1_out,
				XDY_Octet       *va_VL_out,
				XDY_Octet       *va_BAU_out,
				XDY_Octet       *va_VR_I_out,
				XDY_Octet       *va_VM2_I_out,
				XDY_Octet       *va_VM1_I_out,
				XDY_Octet       *va_VL_I_out,
				XDY_Octet       *va_BAU_I_out,
				XDY_Octet       *va_Nombre_ZI_out )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate 	va_Horodate_in
* XDY_Evt 	va_NumEvt_in
*
* ARGUMENTS EN SORTIE :
*
* XDY_Evt	va_NumEvtQueue
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
	CS_CHAR *rpc_name =XZAOC_XZAP860_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAP860_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	int		i;


	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : XZAE860_Lire_Config_Terrain (%d/%d/%d/%d)",
			va_NumAutoroute_in,va_PRDebut_in,va_Sens_in,va_PRFin_in);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
        if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_NumAutoroute_in,"@va_NumAutoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_input(va_NumAutoroute_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRDebut_in,"@va_PRDebut_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_int_input(va_PRDebut_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


        if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_input(va_Sens_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRFin_in,"@va_PRFin_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_int_input(va_PRFin_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VR_out,"@va_VR_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VR) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VM2_out,"@va_VM2_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VM2) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VM1_out,"@va_VM1_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VM1) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VL_out,"@va_VL_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VL) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_BAU_out,"@va_BAU_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(BAU) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VR_I_out,"@va_VR_I_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VR_I) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VM2_I_out,"@va_VM2_I_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VM2_I) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VM1_I_out,"@va_VM1_I_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VM1_I) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_VL_I_out,"@va_VL_I_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(VL_I) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_BAU_I_out,"@va_BAU_I_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae860: asql_ctparam_tinyint_output(BAU_I) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	i = i + 1;




	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae860: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae860 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae860 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae860 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE860_Lire_Config_Terrain retourne 1");
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzae860: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne la configuration :\nVR     : %d\nVM2    : %d\nVM1    : %d\nVL    : %d\nBAU    : %d\nVR_I   : %d\nVM2_I  : %d\nVM1_I  : %d\nVL_I   : %d\nBAU_I  : %d\nNbre ZI : %d\n",
		*va_VR_out,
		*va_VM2_out,
		*va_VM1_out,
		*va_VL_out,
		*va_BAU_out,
		*va_VR_I_out,
		*va_VM2_I_out,
		*va_VM1_I_out,
		*va_VL_I_out,
		*va_BAU_I_out,
		*va_Nombre_ZI_out );

 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE860_Lire_Config_Terrain retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
