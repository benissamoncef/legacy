/*E*/
/* Fichier : $Id: xzao466.c,v 1.1 2012/11/08 09:52:05 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:52:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao466.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       05 Juin 2012     : Creation (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao466.c	1.0 08/11/11 : xzao466" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO466SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO466_Ajouter_Destination_TDP_PMA (	XDY_Basedd		va_Basedd_in,
					XZAOT_ConfDestPMA	va_DestTDP_in,
					XDY_ResConf  		*va_Resultat_out )


/*
* PARAMETRES EN ENTREE :
*
*   XDY_Basedd		    va_Basedd_in
*   XZAOT_ConfDestPMVA       va_DestTDP_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajout d'une destination (Destination Temps de parcours) en base
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
* Insertion ou modification dans la table RES_DTP (Destination Temps de parcours)
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzao466.c	1.2 06/05/97 : XZAO466_Ajouter_Destination_TDP_PMA" ;

	CS_CHAR *rpc_name = XZAOC_XZAO466_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO466_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;

	/*A
	** Definition des variables locales
	*/


	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO466_Ajouter_Destination_TDP_PMA : NomDest = %s\tAutoroute = %s\tPR = %d\tSens = %d\tType: %d\tSensDest:%d\tNom ZDP=%s\tInhibition : %d\tInit:%d\n",
			va_DestTDP_in.NomDest,
			va_DestTDP_in.Autoroute,
			va_DestTDP_in.PR,
			va_DestTDP_in.Sens,
			va_DestTDP_in.TypeDest,
			va_DestTDP_in.SensDest,
			va_DestTDP_in.ZDP,
			va_DestTDP_in.Inhibition,
			va_DestTDP_in.Init);
	XZST_03EcritureTrace( XZSTC_WARNING,"BASEDD %s",va_Basedd_in);

	XDY_Nom		vl_NomDest;
	XDY_NomAuto	vl_Autoroute;
    XDY_PR          vl_PR;
	XDY_Sens	vl_Sens;
    XDY_Octet     vl_Type_Destination;
    XDY_Booleen	vl_Inhibition;
	XDY_Entier	vl_init;
	XDY_Nom		vl_ZDP;
	XDY_Sens	vl_SensDest;

	strcpy( vl_NomDest, va_DestTDP_in.NomDest);
	strcpy( vl_Autoroute, va_DestTDP_in.Autoroute );
	vl_PR				= va_DestTDP_in.PR;
	vl_Sens				= va_DestTDP_in.Sens;
	vl_Type_Destination	= va_DestTDP_in.TypeDest;
	vl_Inhibition		= va_DestTDP_in.Inhibition;
	vl_init				= va_DestTDP_in.Init;
	strcpy( vl_ZDP, va_DestTDP_in.ZDP);
	vl_SensDest			= va_DestTDP_in.SensDest;

	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_NomDest,"@va_Nom_Dest_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_char_input(NomDest) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type_Destination,"@va_Type_Dest_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466:asql_ctparam_tinyint_input(Type_Destination)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_SensDest,"@va_Sens_Dest_in"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao466:asql_ctparam_tinyint_input(va_Sens_Dest_in)  a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	  asql_clean_cmd(connection, cmd);
	  return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Inhibition,"@va_Inhibition_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466:asql_ctparam_bit_input(Inhibition)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_ZDP,"@va_Nom_Zdp_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466:asql_ctparam_char_input(Nom_ZDP)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
 	}

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_init,"@va_droptable_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466:asql_ctparam_int_input(Init table)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: asql_ctparam_int_output(Resultat) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	/*A
	** Envoie de la commande au Sql server
	*/

	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao466: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao466 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}

					XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao466 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao466: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	/*!
	** Retourne le resultat et les parametres de sorties renseignes


	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO466_Ajouter_Destination_TDP_PMA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




