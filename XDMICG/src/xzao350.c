/*E*/
/* Fichier : $Id: xzao350.c,v 1.2 2009/10/22 15:52:41 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/10/22 15:52:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzao350.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/10/08 : creation DEM895
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao350.h"

static char *version = "$Id: xzao350.c,v 1.2 2009/10/22 15:52:41 pc2dpdy Exp $ $Revision: 1.2 $  XZAO350_Zone_Tunnel";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des stations RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO350_Zone_Tunnel ( 
			XDY_Autoroute	va_autoroute_in,
			XDY_PR		va_pr_tete_in,
			XDY_PR		va_pr_queue_in,
			XDY_Octet	va_sens_in,
			XDY_Mot		va_tunnel_in,
			XDY_Octet	*va_zone_out,
			XDY_Octet	*va_type_out,
			char	*va_groupe_out,
			XDY_PR		*va_PRPSTT_out	)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Octet		va_NumEch
*
* ARGUMENTS EN SORTIE :
*
* XDY_Octet		va_Sortie
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
* Appelle XZA0350SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "$Id: xzao350.c,v 1.2 2009/10/22 15:52:41 pc2dpdy Exp $ $Revision: 1.2 $ XZAO350_Zone_Tunnel";
	
	CS_CHAR 	*vl_RpcName =XZAOC_XZAO350_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int             num_Tab_donnees2 = XZAOC_XZAO350_NB_PARM_RETURN;
	tg_row_result	vl_Tab_Ligne;
	tg_Colonne_donnees2 Tab_donnees2[XZAOC_XZAO350_NB_PARM_RETURN];
	XZAOT_Station	vl_Station;
	int		i,
			vl_Nb_Ligne_Lue,
			vl_Nb_Col,
			vl_Status;
	XDY_Entier	SPstatus;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAO350_Zone_Tunnel : aut=%d/%d/%d/%d\n",
			va_autoroute_in, va_pr_tete_in, va_pr_queue_in, va_sens_in);

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
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
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_autoroute_in,
						"@va_autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_input(autoroute_in) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_pr_tete_in,
						"@va_pr_tete_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_input(pr_tete_in) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_pr_queue_in,
						"@va_pr_queue_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_input(pr_queue_in) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_sens_in,
						"@va_sens_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_input(sens_in) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_tunnel_in,
					"@va_tunnel_in"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_smallint_input(va_tunnel_in) echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d",
			vl_Retcode);
          asql_clean_cmd(vl_Connection, vl_Cmd);
	  return (XDC_NOK);
	}
	Tab_donnees2[0].pt_value = (CS_VOID *)va_zone_out;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_zone_out,
						"@va_zone_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_output(zone_out) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[1].pt_value = (CS_VOID *)va_type_out;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_type_out,
						"@va_type_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_output(type_out) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[2].pt_value = (CS_VOID *)va_groupe_out;
	if ((vl_Retcode = asql_ctparam_char_output(vl_Cmd,(CS_CHAR *)va_groupe_out,
						"@va_groupe_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_tinyint_output(groupe_out) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[3].pt_value = (CS_VOID *)va_PRPSTT_out;
	if ((vl_Retcode = asql_ctparam_int_output(vl_Cmd,(CS_INT *)va_PRPSTT_out,
						"@va_PRPSTT_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_ctparam_int_output(va_PRPSTT_out) echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", 
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao350: ROW RESULTS \n");
						break;

					case  CS_PARAM_RESULT:
						vl_Retcode = asql_fetch_param_result(vl_Cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao350 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao350 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao350 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", SPstatus);
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (SPstatus);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && ((int)vl_ResType != CS_ROW_RESULT))
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: asql_fetch a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", vl_Retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", vl_Retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao350: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO350_Zone_Tunnel retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAO350_Zone_Tunnel retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

