/*E*/
/* Fichier : $Id: xzac840.c,v 1.4 2020/11/03 17:56:38 pc2dpdy Exp $        Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 17:56:38 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzac840.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Mise au neutre d'un equipement PMV.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	25/02/13 : Creation (pour DEM 1061)  1.1
* JPL	14/03/13 : Envoi du parametre "Fin action" a la procedure stockee  1.2
* JMG	16/01/20 : ajout va_Attente_EQT_FIL DEM1347 1.3
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac840.h"


static char *version = "$Id: xzac840.c,v 1.4 2020/11/03 17:56:38 pc2dpdy Exp $ : xzac840.c" ;

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

int XZAC840_Mise_Au_Neutre_PMVA (	XDY_Eqt		va_NumEqt_in,
					XDY_District	va_Site_in,
					XDY_Octet	va_Fin_Action_in,
					XDY_Ligne_PMV	va_Ligne1_TDP_in,
					XDY_Ligne_PMV	va_Ligne2_TDP_in,
					XDY_Ligne_PMV	va_Ligne3_TDP_in,
					XDY_Ligne_PMV	va_Ligne4_TDP_in,
					XDY_Ligne_PMV	va_Ligne5_TDP_in,
					XDY_Mot		va_Dest1_Proche_in,
					XDY_Mot		va_Dest1_Loin_in,
					XDY_Mot		va_Dest2_Proche_in,
					XDY_Mot		va_Dest2_Loin_in,
					XDY_Entier	*va_Num_Action_out,
					XDY_Octet	*va_Type_Neutre_out, 
					XDY_Octet	va_Attente_EQT_FIL)


/*
------------------------------------------------------------------------------
*
* PARAMETRES EN ENTREE :
*
*  XDY_Eqt		va_NumEqt_in
*  XDY_District		va_Site_in
*  XDY_Octet		va_Fin_Action_in
*  XDY_Ligne_PMV	va_Ligne1_TDP_in
*  XDY_Ligne_PMV	va_Ligne2_TDP_in
*  XDY_Ligne_PMV	va_Ligne3_TDP_in
*  XDY_Ligne_PMV	va_Ligne4_TDP_in
*  XDY_Ligne_PMV	va_Ligne5_TDP_in
*  XDY_Mot		va_Dest1_Proche_in
*  XDY_Mot		va_Dest1_Loin_in
*  XDY_Mot		va_Dest2_Proche_in
*  XDY_Mot		va_Dest2_Loin_in
*
* PARAMETRES EN SORTIE :
*
*  XDY_Entier		va_Num_Action_out
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
*  Execute la procedure de mise au neutre de l'equipements.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzac840.c,v 1.4 2020/11/03 17:56:38 pc2dpdy Exp $ : XZAC840_Mise_Au_Neutre_PMVA" ;

	CS_CHAR *rpc_name = XZACC_XZAC840_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC840_NB_PARM_RETURN;
	tg_Colonne_donnees2	Tab_donnees2[XZACC_XZAC840_NB_PARM_RETURN];
	int		i;
	XDY_Entier	SPstatus;
	int		vl_cr;
	XDY_NomSite     pl_NomSite;

	CS_INT		vl_Num_Action = 0;
	CS_TINYINT	vl_Type_Neutre = 0;


	XZST_03EcritureTrace (XZSTC_INTERFACE, "IN : XZAC840_Mise_Au_Neutre_PMVA : Eqt=%d, Site=%d, Msg Tdp %s/%s/%s/%s/%s",
	                         va_NumEqt_in, va_Site_in,
	                         va_Ligne1_TDP_in, va_Ligne2_TDP_in, va_Ligne3_TDP_in, va_Ligne4_TDP_in, va_Ligne5_TDP_in);

	/*A
	** Verification de la validite des parametres d'entree
	*/

        /*A
        ** Recuperation du site de la machine
        */
        if ( (retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzac840 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzac840 : Recuperation du Nom du site %s.",pl_NomSite);


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_NumEqt_in, "@va_NumEqt_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_smallint_input (NumEqt) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Site_in, "@va_Site_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_tinyint_input (Site) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Fin_Action_in, "@va_Fin_Action_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_tinyint_input (Fin_Action) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne1_TDP_in, "@va_Ligne1_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_char_input (Ligne1_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne2_TDP_in, "@va_Ligne2_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_char_input (Ligne2_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne3_TDP_in, "@va_Ligne3_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_char_input (Ligne3_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne4_TDP_in, "@va_Ligne4_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_char_input (Ligne4_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) va_Ligne5_TDP_in, "@va_Ligne5_TDP_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_char_input (Ligne5_TDP) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_Dest1_Proche_in, "@va_Dest1_Proche_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_smallint_input (Dest1_Proche) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_Dest1_Loin_in, "@va_Dest1_Loin_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_smallint_input (Dest1_Loin) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_Dest2_Proche_in, "@va_Dest2_Proche_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_smallint_input (Dest2_Proche) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_Dest2_Loin_in, "@va_Dest2_Loin_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_smallint_input (Dest2_Loin) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	Tab_donnees2[0].pt_value = (CS_VOID *) va_Num_Action_out;

	if ((retcode = asql_ctparam_int_output (cmd, (CS_INT *) va_Num_Action_out, "@va_Num_Action_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_int_output (Numero_Action) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[1].pt_value = (CS_VOID *) va_Type_Neutre_out;

	if ((retcode = asql_ctparam_tinyint_output (cmd, (CS_TINYINT *) va_Type_Neutre_out, "@va_Type_Neutre_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_tinyint_output (Type_Neutre) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Attente_EQT_FIL, "@va_Attente_EQT_FIL")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzac840: asql_ctparam_tinyint_input (va_Attente_EQT_FIL) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)pl_NomSite,"@va_"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: asql_ctparam_char_input(va_NomSiteLocal_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac840: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd, num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac840 : PARAMETER RESULTS %d, %d",
						                        (int) *va_Num_Action_out, (int) *va_Type_Neutre_out);
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus); 
						XZST_03EcritureTrace(XZSTC_INFO,"xzac840 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac840 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac840 : asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac840: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
		}
	}


	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC840_Mise_Au_Neutre_PMVA retourne %d", retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}
