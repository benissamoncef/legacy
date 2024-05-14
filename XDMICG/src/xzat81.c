/* Fichier : xzat81.c
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / MAGISTRA
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat81.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Ecriture en base de l'etat d'un Controleur de Feux.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	10/07/23 : Creation (SAE-440)
----------------------------------------------------------------------------*/


#include "xzat.h"
#include "xzat81.h"


static char *version = "xzat81.c 1.1" ;

/* Declaration de variables internes */


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue l'ecriture en base de l'etat commande a un Controleur de Feux
*  (le mode de fonctionnement et le Plan de Feux).
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT81_Ecrire_Etat_CFE (	XDY_Eqt		va_NumEqt_in,
				XDY_Horodate	va_Horodate_in,
				XDY_Octet	va_Fonctionnement_in,
				XDY_Octet	va_Plan_Feux_in)


/*
------------------------------------------------------------------------------
*
* PARAMETRES EN ENTREE :
*
*  XDY_Eqt		va_NumEqt_in
*  XDY_Horodate		va_Horodate_in
*  XDY_Octet		va_Fonctionnement_in
*  XDY_Octet		va_Plan_Feux_in
*
* PARAMETRES EN SORTIE :
*
*  Aucun
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
*  Execute la procedure d'ecriture de l'etat courant de l'equipement.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	CS_CHAR		*rpc_name = XZATC_XZAT81_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	XDY_Entier	SPstatus;
	int		vl_cr;
	XDY_NomSite	vl_NomSite;

	CS_DATETIME	vl_Horodate;
	char		vl_Fonctionnement[2];


	XZST_03EcritureTrace (XZSTC_INTERFACE, "IN : XZAT81_Ecrire_Etat_CFE : Eqt=%d, Horodate=%lf, Mode='%c', Plan de feux %d",
	                         va_NumEqt_in, va_Horodate_in, va_Fonctionnement_in, va_Plan_Feux_in);

	/*
	** Verification de la validite des parametres d'entree
	*/

	/* Conversion des horodates en entree du format Unix au format Sybase */
	if ( (retcode = asql_date_Ux2Sybase (va_Horodate_in, &vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "xzat81 : asql_date_Ux2Sybase(va_Horodate_in, &vl_Horodate) a echoue.");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		return (XDC_NOK);
	}


	/*
	** Recuperation du site de la machine
	*/

	if ( (retcode = XZSC_04NomSite (vl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "xzat81 : Recuperation du Nom du site impossible.");
		return (XDC_NOK);
	}
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat81 : Recuperation du Nom du site %s.", vl_NomSite);


	/*
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		return (XDC_NOK);
	}


	/*
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat81: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*
	** Initialisation de la structure des parametres d I/O de la procedure
	*/

	if ((retcode = asql_ctparam_smallint_input (cmd, (CS_SMALLINT *) &va_NumEqt_in, "@va_NumEqt_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat81: asql_ctparam_smallint_input (NumEqt) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_date_input (cmd, (CS_DATETIME *) &vl_Horodate, "@va_Horodate_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat81: asql_ctparam_date_input (Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	sprintf (vl_Fonctionnement, "%c", (int) va_Fonctionnement_in);

	if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) vl_Fonctionnement, "@va_Fonctionnement_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat81: asql_ctparam_char_input (Fonctionnement_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &va_Plan_Feux_in, "@va_Plan_Feux_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat81: asql_ctparam_tinyint_input (Plan_Feux) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_char_input (cmd, (CS_CHAR *) vl_NomSite, "@va_NomSiteLocal_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: asql_ctparam_char_input(va_NomSiteLocal_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }


	/*
	** Envoi de la commande au Sql server
	*/

	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat81: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						/*retcode = asql_fetch_param_result (cmd, num_Tab_donnees2, Tab_donnees2);*/
						XZST_03EcritureTrace(XZSTC_INFO,"xzat81 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat81 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat81 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat81 : asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
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
				XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzat81: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				return (XDC_NOK);
		}
	}


	/*
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT81_Ecrire_Etat_CFE retourne %d", retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}
