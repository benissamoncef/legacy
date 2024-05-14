/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae565.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* VR  	30/01/12     : Creation
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae565.h"

static char *version = "xzae565.c 1.4";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lecture d'une FMC regulation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE565_Lire_FMC_Regulation (XDY_Entier	va_Numero_in,
									XDY_Octet  		va_Cle_in,
									XDY_Horodate	va_Horodate_in,
									XDY_Entier		*va_Contexte_out,
									XDY_Entier		*va_Scenario_out,
									XDY_Entier		*va_Scenario_cible_out,
									XDY_Entier		*va_Mesure_out,
									XDY_Entier		*va_Indicateur_Horaire_out,
									XDY_Entier		*va_Zone_out,
									char*			va_Transitoire_out)



/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_numero_in
* XDY_Octet		va_cle_in
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
* - va_numero_in, va_cle_in sont obligatoires
*

* FONCTION :
*
* Extraire les infos obligatoires sur les evts
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR 	*pl_rpc_name = XZAE565C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE565C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE565C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
    int             vl_Nb_Col = 0;
    tg_row_result   vl_Tab_Ligne = NULL;
    CS_DATETIME	vl_Horodate;
	XDY_NomSite     pl_NomSite;


	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAE565_Lire_FMC_Regulation : Debut d execution %d %d %lf",
					va_Numero_in,va_Cle_in, va_Horodate_in);

	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565_Lire_FMC_Regulation: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d",vl_retcode);
		return (XDC_NOK);
	}
	
        /*A
        ** Recuperation du site de la machine
        */
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzae565 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzae565 : Recuperation du Nom du site %s.",pl_NomSite);


	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/


        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_Numero_in,"@va_Numero_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565_Lire_FMC_Regulation: asql_ctparam_int_input(va_Numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Cle_in,"@va_Cle_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565_Lire_FMC_Regulation: asql_ctparam_tinyint_input(va_Cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
/*	
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565_Lire_FMC_Regulation: asql_ctparam_date_input(va_Horodate_in) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
*/	
        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae565 : asql_ctparam_char_input(@va_NomSiteLocal_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if ((vl_retcode = ct_send(pl_cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE565: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE565 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE565 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE565 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE565_Lire_FMC_Regulation retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565 retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565 Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE565: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565 retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	if (vl_Nb_Ligne_Lue > 0) {
		*va_Contexte_out			= * (XDY_Entier *) vl_Tab_Ligne[0][0].pt_value;
		*va_Scenario_out			= * (XDY_Entier *) vl_Tab_Ligne[0][1].pt_value;
		*va_Scenario_cible_out			= * (XDY_Entier *) vl_Tab_Ligne[0][2].pt_value;
		*va_Mesure_out			= * (XDY_Entier *) vl_Tab_Ligne[0][3].pt_value;
		*va_Indicateur_Horaire_out			= * (XDY_Entier *) vl_Tab_Ligne[0][4].pt_value;
		strcpy(va_Transitoire_out			, (char *) vl_Tab_Ligne[0][5].pt_value);

	
		XZST_03EcritureTrace(	XZSTC_WARNING,
			"OUT : XZAE565_Lire_FMC_Regulation :\nContexte=%d\nScenario=%d\nMesure=%d\nIndicateur_Horaire=%d\nTransitoire=%s",
			*va_Contexte_out,
			*va_Scenario_out,
			*va_Scenario_cible_out,
			*va_Mesure_out,
			*va_Indicateur_Horaire_out,
			*va_Transitoire_out);
	
	}


	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	

 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE565_Lire_FMC_Regulation retourne %d", vl_retcode);
 	return(XDC_OK);
}
