/* Fichier : $Id: xzaa30.c,v 1.1 2012/07/04 10:21:32 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:21:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA30 * FICHIER xzaa30.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzaa30.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	03/04/2012	: Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaa30.h"
#include "xzag.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzaa30.c	1.9 04/04/11 : xzaa30" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
XZAT_Connexion *XZAGV_Connexion;
XZAT_Context   *XZAGV_Context;
/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne les informations detaillees d'un alerte Regulation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAA30_Liste_Infos_Alerte_Regulation ( XDY_Basedd 	va_Base,
										   XDY_Entier	va_NumAlerte_in,
										   XDY_Entier	*va_Context_out,
										   XDY_Entier	*va_Scenario_out,
										   XDY_Entier	*va_Indicateur_Horaire_out
										 )


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Basedd		va_Base
* XDY_Alerte		va_NumAlerte_in
*
* ARGUMENTS EN SORTIE :
* XDY_Entier	va_Context_out
* XDY_Entier	va_Scenario_out
* XDY_Entier	va_Indicateur_Horaire_out
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
* Appelle XZAA30SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "@(#)xzaa30.c	1.3 26/01/12 XZAA30_Liste_Infos_Alerte_Regulation";

	CS_CHAR 	*vl_RpcName =XZAAC_XZAA30_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	tg_row_result	vl_Tab_Ligne;
	int		i,
			vl_Nb_Ligne_Lue,
			vl_Nb_Col,
			vl_Status;
	XDY_Entier	SPstatus;



	/*A
	** Initialisation de la commande
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d",
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d",
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)va_Base,
						"@va_Basedd_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d",
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_NumAlerte_in,
						"@va_NumAlerte_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: asql_ctparam_smallint_input(NumAlerte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d",
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}



	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d",
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
							*va_Context_out = *((XDY_Eqt *)vl_Tab_Ligne[0][0].pt_value);
							*va_Scenario_out = *((XDY_Octet *)vl_Tab_Ligne[0][1].pt_value);
							*va_Indicateur_Horaire_out = *((XDY_Sens *)vl_Tab_Ligne[0][2].pt_value);

							if (vl_Status != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,
									"xzaa30 : fonction utilisateur retourne %d", vl_Status);

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
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa30 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa30 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaa30 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d", SPstatus);
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (SPstatus);
						}
						break;
				}


				if (vl_Retcode != CS_SUCCEED && ((int)vl_ResType != CS_ROW_RESULT))
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: asql_fetch a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d", vl_Retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d", vl_Retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaa30: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}


	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAA30_Liste_Infos_Alerte_Regulation retourne %d\n", SPstatus);


 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}
