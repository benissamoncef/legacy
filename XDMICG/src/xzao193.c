/*E*/
/* Fichier : $Id: xzao193.c,v 1.1 2010/10/07 18:38:44 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2010/10/07 18:38:44 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao193.c
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

static char *version = "$Id: xzao193.c,v 1.1 2010/10/07 18:38:44 gesconf Exp $ : xzao193";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO193SP
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO193_Ajouter_Zon_Camera (	XDY_Basedd		va_Basedd_in,
					XZAOT_ConfZoneCamera	va_Zone_Camera_in,
					XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
* XDY_Octet		va_Pertinence_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute les zones de couverture des cameras video en base
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
* Insertion ou modification dans la table EQT_CAM_POS (CAMERA)
*
* MECANISMES :
*
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao193.c,v 1.1 2010/10/07 18:38:44 gesconf Exp $ : XZAO193_Ajouter_Zon_Camera";


	CS_CHAR *rpc_name = XZAOC_XZAO193_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO193_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;

	/*A
	** Definition des variables locales
	*/

	XDY_NomEqt	vl_Nom;
   	XDY_NomAuto	vl_Autoroute;
	XDY_Sens	vl_Sens;
	XDY_PR		vl_PRdebut;
	XDY_PR		vl_PRfin;
	XDY_Octet	vl_Pertinence;


	/*A
	** Definition des variables locales
	*/

	strcpy( vl_Nom, va_Zone_Camera_in.Nom );
	strcpy( vl_Autoroute, va_Zone_Camera_in.Autoroute );
	vl_Sens		 = va_Zone_Camera_in.Sens;
	vl_PRdebut 	 = va_Zone_Camera_in.PRdebut;
	vl_PRfin 	 = va_Zone_Camera_in.PRfin;
	vl_Pertinence	 = va_Zone_Camera_in.Pertinence;



	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO193_Ajouter_Zon_Camera : Nom = %s\tAutoroute = %s\tSens = %d\tPRdeb = %d\tPRfin = %d\tPertinence = %d\n",
			va_Zone_Camera_in.Nom,
			va_Zone_Camera_in.Autoroute,
			va_Zone_Camera_in.Sens,
			va_Zone_Camera_in.PRdebut,
			va_Zone_Camera_in.PRfin,
			va_Zone_Camera_in.Pertinence );


	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/

	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);




	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}




	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
		free(Tab_donnees2);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}



	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Nom,"@va_Nom_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_char_input(Nom) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}




	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRdebut,"@va_PRdebut_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_int_input(PRdebut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRfin,"@va_PRfin_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_int_input(PRfin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Pertinence,"@va_Pertinence_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_tinyint_input(Pertinence) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: asql_ctparam_int_output(Resultat) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}




	/*A
	** Envoie de la commande au Sql server
	*/

	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao193: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao193 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}

					XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao193 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao193: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}



	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO193_Ajouter_Zon_Camera retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}
