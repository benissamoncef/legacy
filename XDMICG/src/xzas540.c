/*E*/
/* Fichier : $Id: xzas540.c,v 1.1 2012/07/04 10:47:38 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:47:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzas540.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tasts
* permet d'extraire les donnees trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	16 Mai 2012	: Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas540.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzas540.c	1.2 12/10/97 : xzas540" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU :
*  XZAS54_Donnees_RADT : recupere les donnees RADT
*  pour une station et un sens donnes � l'heure courante
*  Appelle XZAS;54SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS540_Liste_Donnees_Trafic (	XDY_Eqt			va_station_in ,
									XDY_Sens		va_sens_in,
									XDY_Horodate	va_horodate_in,
									XDY_Entier		*va_Q_out,
									XDY_Entier		*va_V_out,
									XDY_Entier		*va_TO_out,
									XDY_Octet		*va_validQ_out,
									XDY_Octet		*va_validV_out,
									XDY_Octet		*va_validTO_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_Station_in
* XDY_Entier		va_sens_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* - va_sens_in, va_station_in sont obligatoires
*

* FONCTION :
*
* Extraire les donnees RADT pour la station souhaitee
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzas540.c	1.2 12/10/97 : XZAS540_Liste_Donnees_Trafic" ;

	int		i;
	CS_CHAR 	*pl_rpc_name = XZAS540C_RPC_NAME;

	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;

	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAS540C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAS540C_NB_PARM_RETURN];
	tg_row_result 	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;

	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	XDY_Horodate	vl_HorodateFin;

	int 		vl_indice;
	XDY_Booleen	vl_Bool;


	XZST_03EcritureTrace( XZSTC_DEBUG1,"IN : XZAS540_Liste_Donnees_Trafic : Debut d execution %lf",va_horodate_in );


	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/

	if ((vl_retcode = asql_date_Ux2Sybase(va_horodate_in,&vl_Horodate))!= XDC_OK)
	{


		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_date_Ux2Sybase(va_horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic %d", vl_retcode);

		return (XDC_NOK);
	}


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS540_Liste_Donnees_Trafic : Jours = %d\tHeure = %d" , 
		vl_Horodate.dtdays,vl_Horodate.dttime);


	/*A
	** Initialisation de la commande
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/

	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_station_in,"@va_station"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_smallint_input(va_station_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}


	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_sens_in,"@va_sens"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_date_input(va_sens) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_horodate"))!= CS_SUCCEED)
	{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_date_input(va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Q_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_Q_out,"@va_Q_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_Q_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_V_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_V_out,"@va_V_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_V_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_TO_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_TO_out,"@va_TO_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_TO_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_validQ_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_validQ_out,"@va_validQ_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_validQ_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_validV_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_validV_out,"@va_validV_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_validV_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_validTO_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_validTO_out,"@va_validTO_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: asql_ctparam_int_output(va_validTO_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS540_Liste_Donnees_Trafic: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS540_Liste_Donnees_Trafic : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS540_Liste_Donnees_Trafic : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS540_Liste_Donnees_Trafic : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAS540_Liste_Donnees_Trafic retourne 1 : %d", (int) vl_SPstatus);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
						break;
				}

				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS540_Liste_Donnees_Trafic: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS540_Liste_Donnees_Trafic : Q=%d, V=%d, TO=%d",
			*va_Q_out,
			*va_V_out,
			*va_TO_out);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS540_Liste_Donnees_Trafic retourne %d", vl_retcode);
 	return(XDC_OK);
}



