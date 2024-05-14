/*E*/
/* Fichier : $Id: xzae560.c,v 1.2 2020/11/03 17:56:54 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 17:56:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae560.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* VR  	26/01/12     : Creation (DEM/1016)
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------*/

/* fichiers inclus */
#include "xzae560.h"

static char *version = "@(#)xzae560.c	1.0 02/02/12 XZAE560_Creer_FMC_Regulation";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree une FMC Regulation sur une zone donnee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE560_Creer_FMC_Regulation ( XDY_Autoroute	va_Autoroute_in,
				   XDY_Sens	 		va_Sens_in,
				   XDY_PR			va_PR_Debut_in,
				   XDY_PR			va_Pr_Fin_in,
				   XDY_Eqt		va_Contexte_in,
				   XDY_Eqt	va_Scenario_in,
				   XDY_Eqt		va_Scenario_cible_in,
				   XDY_Booleen		va_IndicateurHoraire_in,
				   XDY_Eqt		va_Mesure_in,
				   char *			va_Transitoire_in,
				   XDY_Eqt		va_Zone_in,
				   XDY_Entier	va_numAlerte_in,
				   XDY_Entier		*va_Num_FMC_out,
				   XDY_Octet		*va_Cle_FMC_out
				 )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Autoroute	va_Autoroute_in
* XDY_Entier	va_Sens_in
* XDY_Entier	va_PR_Debut_in
* XDY_Entier	va_Pr_Fin_in
* XDY_Entier	va_Contexte_in
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier	va_Num_FMC_out
* XDY_Entier	va_Cle_FMC_out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*  XDC_ARG_INV	: argument invalide
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAE560SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "@(#)xzae560.c	1.3 26/01/12 XZAE560_Creer_FMC_Regulation";
	
	CS_CHAR 	*vl_RpcName =XZAEC_XZAE560_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int             vl_num_Tab_donnees2 = XZAE560C_NB_PARM_RETURN;
	tg_Colonne_donnees2 vl_Tab_donnees2[XZAE560C_NB_PARM_RETURN];
	tg_row_result	vl_Tab_Ligne;
	int		i,
			vl_Nb_Ligne_Lue,
			vl_Nb_Col,
			vl_Status;
	XDY_Entier	SPstatus;
	XDY_NomSite     pl_NomSite;

	
        /*A
        ** Recuperation du site de la machine
        */
        if ( (vl_Retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzae560 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzae560 : Recuperation du Nom du site %s.",pl_NomSite);

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Autoroute_in,
						"@va_Autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(NumAlerte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Sens_in,
						"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(NumAlerte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_PR_Debut_in,
						"@va_PR_Debut_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_int_input(pr debut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Pr_Fin_in,
						"@va_Pr_Fin_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(NumAlerte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Contexte_in,
						"@va_Contexte_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(NumAlerte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Scenario_in,
						"@va_Scenario_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_Scenario_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Scenario_cible_in,
							"@va_Scenario_cible_in"))!= CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_Scenario_cible_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d",
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_bit_input(vl_Cmd,(CS_BIT *)&va_IndicateurHoraire_in,
						"@va_IndicateurHoraire_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_IndicateurHoraire_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Mesure_in,
						"@va_Mesure_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_Mesure_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)va_Transitoire_in,
						"@va_Transitoire_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_Transitoire_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Zone_in,
						"@va_Zone_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_Zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d",
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_numAlerte_in,
						"@va_numAlerte_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_smallint_input(va_numAlerte_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d",
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_Num_FMC_out;
	if ((vl_Retcode = asql_ctparam_int_output(vl_Cmd,(CS_INT *)va_Num_FMC_out,
							"@va_Num_FMC_out"))!= CS_SUCCEED)
	{
	    XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_int_output(va_Num_FMC_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d",
		vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	vl_Tab_donnees2[1].pt_value = (CS_VOID *)va_Cle_FMC_out;
		if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_Cle_FMC_out,
							"@va_Cle_FMC_out"))!= CS_SUCCEED)
	{
	    XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_ctparam_tinyint_output(va_Cle_FMC_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d",
		vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

        if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae560 : asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", vl_Retcode);
           asql_clean_cmd(vl_Connection, vl_Cmd);
           return (XDC_NOK);
        }

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						vl_Retcode = asql_fetch_row_result(vl_Cmd , &vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);  

						vl_Retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
									vl_Nb_Col,&vl_Tab_Ligne);
						if (vl_Retcode != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_WARNING,
									"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
							asql_clean_cmd(vl_Connection, vl_Cmd);
							return XDC_NOK;
						}
						break;

					case  CS_PARAM_RESULT:
						vl_Retcode = asql_fetch_param_result (vl_Cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae560 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae560 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae560 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", SPstatus);
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (SPstatus);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && ((int)vl_ResType != CS_ROW_RESULT))
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: asql_fetch a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae560: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE560_Creer_FMC_Regulation retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE560_Creer_FMC_Regulation retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}


