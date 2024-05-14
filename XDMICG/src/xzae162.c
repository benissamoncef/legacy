/*E*/
/*Fichier : $Id: xzae162.c,v 1.1 2008/11/17 10:57:49 pc2dpdy Exp $    Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:57:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE162 * FICHIER xzae162.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* xzae162  
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae162.h"

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE162 : traiter icones WEB
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE162_Calcul_Troncons(XDY_Autoroute va_autoroute,
                        XDY_Octet       va_sens,
                        XDY_PR          va_pr,
                        XDY_Octet       va_type_pt_car,
                        XDY_Octet       va_num_pt_car,
                        XDY_Horodate    va_validation,
                        char         va_nom1[200],
                        XDY_Nom         va_icone,
                        XDY_Octet       va_priorite,
                        XDY_Octet       va_type)

/*
*
* PARAMETRES EN ENTREE : aucun
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*	Appelle XZAE162 
*
------------------------------------------------------*/

{
	static char *version 		= "@(#)xzae162.c	1.1 1.1 04/19/01 : XZAE162_Calcul_Troncons" ;	
	int	vl_retcode 		= XDC_OK;
	int	vl_retcode_general	= XDC_OK;
	
	CS_CHAR *rpc_name = XZAE162C_XZAE162_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	XDY_Entier	vl_SPstatus;
		
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : XZAE162_Calcul_Troncons : Debut d execution ");
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : XZAE162_Calcul_Troncons : %d %d %d %d %d",va_autoroute,va_sens,va_pr,
									va_type_pt_car,va_num_pt_car);
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : XZAE162_Calcul_Troncons : %s %s %d %d",va_nom1,va_icone,va_priorite,va_type);
	

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	
	
	/*A
	** Envoi de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

        /*A
        ** Initialisation de la structure pour chacun des parametres d I/O
        ** A faire passer a la rpc.
        */

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_autoroute,"@va_aut_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_sens,"@va_sens_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_pr,"@va_PR_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_type_pt_car,"@va_pt_car_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_num_pt_car,"@va_num_pt_car_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_validation,"@va_horodate_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_nom1,"@va_texte_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_icone,"@va_icone_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_priorite,"@va_priorite_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_type,"@va_type_web_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
                asql_clean_cmd (pl_connection,pl_cmd);
                return (XDC_NOK);
        }
	
		
	/*A
	** Envoi de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
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
					case  CS_PARAM_RESULT:
					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae162 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae162 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne 1");
 						   asql_clean_cmd(pl_connection, pl_cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				break;


			case CS_MSG_RESULT:
				/*
				**
				*/
			vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
			if (vl_retcode != CS_SUCCEED)
				{
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_res_info(msgtype) a echoue");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
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
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzae162: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, pl_cmd);
				return (XDC_NOK);
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE162_Calcul_Troncons retourne %d", vl_retcode);
 	asql_clean_cmd(pl_connection, pl_cmd);
	return(XDC_OK);

}
