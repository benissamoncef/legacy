/*E*/
/* Fichier : $Id: xzao262.c,v 1.1 2018/01/05 20:55:34 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2018/01/05 20:55:34 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao262.c
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		07/10/2010 : Creation  (DEM 948)  1.1
----------------------------------------------------------------------------- */

/* fichiers inclus */

#include "xzao.h"
#include "xzao262.h"

static char *version = "$Id: xzao262.c,v 1.1 2018/01/05 20:55:34 devgfi Exp $ : xzao262";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO262SP
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO262_Ajouter_Station_DIT (	XDY_Basedd		va_Basedd_in,
					XZAOT_Conf_Station_DIT	va_Station_DIT_in,
					XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		Nom_Station_DIT
* XDY_NomEqt		Nom_Station_RADT
* XDY_Sens		Sens_RADT
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute le point de mesure DIT en base
*  si la station RADT existe et fournit des mesures dans le sens indique.
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans la table RES_DIT
*
* MECANISMES :
*
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao262.c,v 1.1 2018/01/05 20:55:34 devgfi Exp $ : XZAO262_Ajouter_Station_DIT";


	CS_CHAR *rpc_name = XZAOC_XZAO262_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO262_NB_PARM_RETURN;
	tg_Colonne_donnees2	Tab_donnees2[XZAOC_XZAO262_NB_PARM_RETURN];
	XDY_Entier	SPstatus;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO262_Ajouter_Station_DIT : Nom = %s, Station = %s, Sens = %d\n",
				va_Station_DIT_in.Nom_Station_DIT,
				va_Station_DIT_in.Nom_Station_RADT,
				va_Station_DIT_in.Sens_RADT );


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}



	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd, (CS_CHAR *) va_Basedd_in, "@va_Basedd_in")) != CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_char_input(cmd, (CS_CHAR *) va_Station_DIT_in.Nom_Station_DIT, "@va_Nom_DIT_in")) != CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: asql_ctparam_char_input(Nom) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_char_input(cmd, (CS_CHAR *) va_Station_DIT_in.Nom_Station_RADT, "@va_Station_RADT_in")) != CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: asql_ctparam_char_input(Station RADT) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_tinyint_input(cmd, (CS_TINYINT *) &va_Station_DIT_in.Sens_RADT, "@va_Sens_RADT_in")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	Tab_donnees2[0].pt_value = (CS_VOID *) va_Resultat_out;

	if ((retcode = asql_ctparam_int_output(cmd, (CS_INT *) va_Resultat_out, "@va_Resultat_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: asql_ctparam_int_output(Resultat) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	/*A
	** Envoie de la commande au Sql server
	*/

	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao262: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao262 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao262 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao262 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao262 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao262 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao262 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao262: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}


	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO262_Ajouter_Station_DIT retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);

	return(XDC_OK);
}
