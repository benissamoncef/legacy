/*E*/
/*Fichier : $Id: xzat067.c,v 1.1 2021/06/01 07:44:09 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/06/01 07:44:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat067.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	18/05/2021	Création DEM-SAE244 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"
#include "xzat067.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT067SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT067_Utilisation_PAU(	XDY_Horodate	va_Horodate_in,
				XDY_District	va_Site_in,
				XDY_Octet	*va_DispoPAU_out,
				XDY_Entier	*va_NumEvt_out,
				XDY_Octet	*va_CleEvt_out,
				XDY_Octet	*va_Priorite_out,
				XDY_Entier	*va_DistanceEvt_out,
				XDY_Entier	*va_Action_out,
				XDY_District	*va_SiteAction_out,
				XDY_MessageSONO	*va_Message_out,
				XDY_Autoroute	*va_Autoroute_out,
				XDY_Sens	*va_Sens_out,
				XDY_PR		*va_PrDebut_out,
				XDY_PR		*va_PrFin_out)

/*
*
* PARAMETRES EN ENTREE :
*
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Site	va_Site_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPAU_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* XDY_Entier	va_Action_out
* XDY_Site	va_SiteAction_out
* 		va_Message_out
*		va_Autoroute_out
*		va_Sens_out
*		va_PrDebut_out
*		va_PrFin_out
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne l'utilisation reelle d'un PAU a une horodate donnee.
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzat067.c	$Id: xzat067.c,v 1.1 2021/06/01 07:44:09 pc2dpdy Exp $ %D% : XZAT067_Utilisation_PAU" ;
	
	CS_CHAR *rpc_name = XZAT067_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	int 		vl_num_Tab_donnees2 = XZAT067_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAT067_NB_PARM_RETURN];
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	CS_DATETIME     vl_Horodate;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT067_Utilisation_PAU : va_Site_in:%d, horodate:%lf",
				va_Site_in,
				va_Horodate_in);

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);												
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        /*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD05_Dernier_TDP retourne %d", retcode);
                return (XDC_NOK);
        }
        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,@va_Horodate_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP25_Trafic retourne %d", retcode);
	   	asql_clean_cmd (connection,cmd);
	   	return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_smallint_input(va_Site_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}  
 
        i = 0;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_DispoPAU_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPAU_out,"@va_DispoPAU_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_DispoPAU_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }
	
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumEvt_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEvt_out,"@va_NumEvt_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_int_output(@va_NumEvt_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

       i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_CleEvt_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_CleEvt_out,"@va_CleEvt_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_CleEvt_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

       i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Priorite_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_Priorite_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_DistanceEvt_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceEvt_out,"@va_DistanceEvt_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_int_output(@va_DistanceEvt_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_Action_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Action_out,"@va_Action_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_int_output(@va_Action_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_SiteAction_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_SiteAction_out,"@va_SiteAction_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_SiteAction_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Message_out;

        if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Message_out,"@va_Message_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_char_output(@va_Message_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Autoroute_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Autoroute_out,"@va_Autoroute_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_Autoroute_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Sens_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Sens_out,"@va_Sens_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_tinyint_output(@va_Sens_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_PrDebut_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrDebut_out,"@va_PrDebut_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_int_output(@va_PrDebut_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

        i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_PrFin_out;

        if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrFin_out,"@va_PrFin_out")) != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: asql_ctparam_int_output(@va_PrFin_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzat067: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat067 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat067 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat067 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat067: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU evt_out:%d, cle_out:%d",*va_NumEvt_out,*va_CleEvt_out);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT067_Utilisation_PAU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




