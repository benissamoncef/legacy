/*E*/
/* Fichier : $Id: xzac640.c,v 1.3 2020/11/03 17:56:36 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:56:36 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzac640.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Mise au neutre d'un equipement SAGA (PMV, PMVA).
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/03/17 : Creation (DEM 1221)  1.1
* JPL	09/04/18 : Ajout parametres: texte affiche et fin d'action en cours; en sortie Action et Type neutre  1.2
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac640.h"


static char *version = "$Id: xzac640.c,v 1.3 2020/11/03 17:56:36 pc2dpdy Exp $ : xzac640.c" ;

/* Declaration de variables internes */


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue la mise au neutre d'un equipement donne d'un site donne,
*  en indiquant les donnees de Temps de Parcours a afficher s'il y en a.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC640_Mise_Au_Neutre_PMVx_SAGA (	XDY_Eqt		va_NumEqt_in,
					XDY_District	va_Site_in,
					char		*va_Texte_Affiche_in,
					XDY_Octet	va_Fin_Action_in,
					char		*va_Ligne1_TDP_in,
					char		*va_Ligne2_TDP_in,
					char		*va_Ligne3_TDP_in,
					char		*va_Ligne4_TDP_in,
					char		*va_Ligne5_TDP_in,
					char		*va_Alternat1_in,
					char		*va_Alternat2_in,
					char		*va_Alternat3_in,
					char		*va_Alternat4_in,
					char		*va_Alternat5_in,
					XDY_Entier	*va_NumeroAction_out,
					XDY_Octet	*va_Type_Neutre_out )


/*
------------------------------------------------------------------------------
*
* PARAMETRES EN ENTREE :
*
*  XDY_Eqt		va_NumEqt_in
*  XDY_District		va_Site_in
*  XDY_Texte		va_Texte_Affiche_in
*  XDY_Octet		va_Fin_Action_in
*  XDY_Ligne_PMV_SAGA	va_Ligne1_TDP_in
*  XDY_Ligne_PMV_SAGA	va_Ligne2_TDP_in
*  XDY_Ligne_PMV_SAGA	va_Ligne3_TDP_in
*  XDY_Ligne_PMV_SAGA	va_Ligne4_TDP_in
*  XDY_Ligne_PMV_SAGA	va_Ligne5_TDP_in
*  XDY_Ligne_PMV_SAGA	va_Alternat1_in
*  XDY_Ligne_PMV_SAGA	va_Alternat2_in
*  XDY_Ligne_PMV_SAGA	va_Alternat3_in
*  XDY_Ligne_PMV_SAGA	va_Alternat4_in
*  XDY_Ligne_PMV_SAGA	va_Alternat5_in
*
* PARAMETRES EN SORTIE :
*
*  XDY_Entier		va_NumeroAction_out
*  XDY_Octet		va_Type_Neutre_out
*
* VALEUR RENDUE :
*
*  Aucune
*
* CODE RETOUR :
*
*   XDC_OK	En cas d'execution correcte
*   XDC_NOK	Une erreur s'est produite
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Execute la procedure de mise au neutre de l'equipement.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzac640.c,v 1.3 2020/11/03 17:56:36 pc2dpdy Exp $ : XZAC640_Mise_Au_Neutre_PMVx_SAGA" ;

	CS_CHAR *rpc_name = XZACC_XZAC640_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC640_NB_PARM_RETURN;
	tg_Colonne_donnees2	Tab_donnees2[XZACC_XZAC640_NB_PARM_RETURN];
	int		i;
	XDY_Entier	SPstatus;
	int		vl_cr;
	XDY_NomSite     pl_NomSite;

	XZST_03EcritureTrace (XZSTC_INTERFACE, "IN : XZAC640_Mise_Au_Neutre_PMVx_SAGA : Eqt=%d, Aff=%s, Msg Tdp %s/%s/%s/%s/%s",
	                         va_NumEqt_in, va_Texte_Affiche_in,
	                         va_Ligne1_TDP_in, va_Ligne2_TDP_in, va_Ligne3_TDP_in, va_Ligne4_TDP_in, va_Ligne5_TDP_in);

	/*A
	** Verification de la validite des parametres d'entree
	*/

        /*A
        ** Recuperation du site de la machine
        */
        if ( (retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzac640 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzac640 : Recuperation du Nom du site %s.",pl_NomSite);


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_NumEqt_in, "@va_NumEqt_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_smallint_input (NumEqt) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Site_in, "@va_Site_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_tinyint_input (Site) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Texte_Affiche_in, "@va_Texte_Affiche_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Texte_Affiche) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Fin_Action_in, "@va_Fin_Action_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_tinyint_input (Fin_Action) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne1_TDP_in, "@va_Ligne1_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Ligne1_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne2_TDP_in, "@va_Ligne2_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Ligne2_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne3_TDP_in, "@va_Ligne3_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Ligne3_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne4_TDP_in, "@va_Ligne4_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Ligne4_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne5_TDP_in, "@va_Ligne5_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Ligne5_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Alternat1_in, "@va_Alternat1_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Alternat1) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Alternat2_in, "@va_Alternat2_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Alternat2) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Alternat3_in, "@va_Alternat3_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Alternat3) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Alternat4_in, "@va_Alternat4_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Alternat4) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Alternat5_in, "@va_Alternat5_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_char_input (Alternat5) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[0].pt_value = (CS_VOID *) va_NumeroAction_out;

	if ((retcode = asql_ctparam_int_output (cmd, (CS_INT *) va_NumeroAction_out, "@va_NumeroAction_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_int_output (Numero_Action) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[1].pt_value = (CS_VOID *) va_Type_Neutre_out;

	if ((retcode = asql_ctparam_tinyint_output (cmd, (CS_TINYINT *) va_Type_Neutre_out, "@va_Type_Neutre_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac640: asql_ctparam_tinyint_output (Type_Neutre) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac640: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd, num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac640 : PARAMETER RESULTS -");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus); 
						XZST_03EcritureTrace(XZSTC_INFO,"xzac640 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac640 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac640 : asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
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
				** Le serveur SQL a detecte une erreur en executant notre commande.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzac640: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				return (XDC_NOK);
		}
	}


	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC640_Mise_Au_Neutre_PMVx_SAGA retourne %d", retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}
