/*E*/
/* Fichier : $Id: xzao444.c,v 1.1 2017/01/10 11:23:42 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2017/01/10 11:23:42 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao444.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Recherche des equipements BRA dont le panneau d'alerte est a piloter.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	15/12/16 : Creation (pour DEM 1193)  1.1
----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzao444.h"

static char *version = "$Id: xzao444.c,v 1.1 2017/01/10 11:23:42 devgfi Exp $ : xzao444.c" ;

/* Declaration de variables internes */


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne la liste des BRA situes sur une autoroute et dans un sens donnes,
*  autour d'un point donne et dont le panneau d'alerte est a activer
*  ou a eteindre en cas de fin d'evenement ou sortie de zone d'activation.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO444_BRA_En_Perturbation (	XDY_Autoroute	va_Autoroute_in,
					XDY_Sens	va_Sens_in,
					XDY_PR		va_PR_in,
					XDY_District	va_District_in,
					XDY_Horodate	va_Horodate_in,
					XDY_PR		va_PR_Tete_in,
					XDY_Entier	va_Numero_Fmc_in,
					XDY_Octet	va_Cle_Fmc_in,
					XDY_FonctionInt	pa_FonctionUtilisateur )

/*
------------------------------------------------------------------------------
*
* PARAMETRES EN ENTREE :
*
*  XDY_Autoroute	va_Autoroute_in
*  XDY_Sens		va_Sens_in
*  XDY_PR		va_PR_in
*  XDY_District		va_District_in
*  XDY_Horodate		va_Horodate_in
*  XDY_PR		va_PR_Tete_in
*  XDY_Entier		va_Numero_Fmc_in
*  XDY_Octet		va_Cle_Fmc_in
*  XDY_FonctionInt	pa_FonctionUtilisateur
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
*  Liste des equipements avec leur etat et date de changement d'etat.
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
* Lit en base les equipements et appelle la fonction utilisateur pour chacun.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao444.c,v 1.1 2017/01/10 11:23:42 devgfi Exp $ : XZAO444_BRA_En_Perturbation" ;

	CS_CHAR *rpc_name = XZAOC_XZAO444_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	/*int 		num_Tab_donnees2 = XZAOC_XZAO444_NB_PARM_RETURN;*/
	/*tg_Colonne_donnees2	Tab_donnees2[XZAOC_XZAO444_NB_PARM_RETURN];*/
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne = NULL;
	int		i;
	tg_status_result	SPstatus;
	int		vl_cr;

	/*A
	** Definition des variables locales
	*/

	CS_TINYINT	vl_Autoroute_in;
	CS_TINYINT	vl_Sens_in;
	CS_INT		vl_PR_in;
	CS_DATETIME	vl_Horodate_in;
	CS_INT		vl_PR_Tete_in;
	CS_INT		vl_Numero_Fmc_in;
	CS_TINYINT	vl_Cle_Fmc_in;


	XZST_03EcritureTrace( XZSTC_INTERFACE, "IN : XZAO444_BRA_En_Perturbation : Auto=%d, Sens=%d, PR=%d, Fmc=%d, Cle=%d",
	                                          va_Autoroute_in, va_Sens_in, va_PR_in,
	                                          va_Numero_Fmc_in, va_Cle_Fmc_in );

	/*A
	** Verification de la validite des parametres d'entree
	*/

	if ( pa_FonctionUtilisateur == NULL )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "pa_FonctionUtilisateur est nulle");
		return (XDC_NOK);
	}


	/*A
	** Conversion des dates en entree du format Unix au format Sybase
	*/

	if ((vl_cr = asql_date_Ux2Sybase (va_Horodate_in, &vl_Horodate_in)) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "xzat01: asql_date_Ux2Sybase() a echoue");
		return (XDC_NOK);
	}

	vl_Autoroute_in		= va_Autoroute_in;
	vl_Sens_in		= va_Sens_in;
	vl_PR_in		= va_PR_in;
	vl_PR_Tete_in		= va_PR_Tete_in;
	vl_Numero_Fmc_in	= va_Numero_Fmc_in;
	vl_Cle_Fmc_in		= va_Cle_Fmc_in;

	/*A
	** Initialisation de la commande
	*/

	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao444: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &vl_Autoroute_in, "@va_Autoroute_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_tinyint_input (Autoroute) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &vl_Sens_in, "@va_Sens_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_tinyint_input (Sens) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_PR_in, "@va_PR_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_int_input (PR) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_date_input (cmd, (CS_DATETIME *) &vl_Horodate_in, "@va_Horodate_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_date_input (Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_PR_Tete_in, "@va_PR_Tete_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_int_input (PR_Tete) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_Numero_Fmc_in, "@va_Numero_Fmc_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_int_input (Numero_Fmc) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &vl_Cle_Fmc_in, "@va_Cle_Fmc_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzao444: asql_ctparam_int_input (Cle_Fmc) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao444: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int) vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int) vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao444: %d ROW RESULTS", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						/*vl_retcode = asql_fetch_param_result (cmd, num_Tab_donnees2, Tab_donnees2);*/
						XZST_03EcritureTrace(XZSTC_INFO,"xzao444 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao444 : CS_STATUS_RESULT = %d", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao444 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne NOK");
 						   asql_clean_cmd(connection, cmd);
 						   asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
						   return (XDC_NOK);
						}
						break;
				}

				break;


			case CS_MSG_RESULT:
				/*
				**
				*/
				vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao444: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne NOK");
					asql_clean_cmd(connection, cmd);
					asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao444: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne NOK");
				asql_clean_cmd(connection, cmd);
				asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao444: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne NOK");
				asql_clean_cmd(connection, cmd);
				asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
				return (XDC_NOK);
		}
	}


	/*B
	** Appel de la fonction utilisateur pour chaque ligne retournee par la procedure stockee
	*/

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
		vl_cr = (*pa_FonctionUtilisateur) (
				* (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value,
				* (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value,
				* (XDY_District *) vl_Tab_Ligne[i][1].pt_value,
				* (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value,
				* (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value,
				va_District_in,
				va_Horodate_in);
		if ( vl_cr != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "appel de pa_FonctionUtilisateur s'est mal deroule");
		}
	}


	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	*/

	vl_retcode =  asql_Free_Tab_Ligne_Results (vl_Nb_Ligne_Lue, vl_Nb_Col, &vl_Tab_Ligne);
	if (vl_retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Retourne le resultat final
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO444_BRA_En_Perturbation retourne %d", vl_retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}
