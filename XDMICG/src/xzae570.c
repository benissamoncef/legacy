/*E*/
/* Fichier : $Id: xzae570.c,v 1.2 2018/08/31 16:24:18 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/08/31 16:24:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzae570.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* XZAE570_Bouchons_Proches : recherche bouchons proches echageur
********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	31/08/18 correction en etes GABEB 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae570.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae570.c	1.5 1.5 12/09/10 : XZAE570_Bouchons_Proches" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE570_Bouchons_Proches( XDY_Horodate 	va_Horodate_in, 
									 XDY_Autoroute 	va_Autoroute_in,
									 XDY_PR 		va_PR_in,
									 XDY_Sens 		va_Sens_in, 
									 XDY_Eqt		va_ech_in,
									 XDY_FonctionInt pa_FonctionUtilisateur_in
									)


/*
* ARGUMENTS EN ENTREE :CFG
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO19 appelle XZAO;19
* AOSP19 select dans les tables EQUIPEMENT et PORTION
* XZAO19 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR 	*pl_rpc_name 		= XZAEC_XZAE570_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode		= XDC_NOK;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAE570_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
	int	i,vl_cr;
        tg_row_result   vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_Horodate;

		/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE570_Bouchons_Proches: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne %d",vl_retcode);
		return (XDC_NOK);
	}
		
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE562_Evts_impactants_En_Cours: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE562_Evts_impactants_En_Cours retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

		if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE570_Bouchons_Proches: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	
		if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE570_Bouchons_Proches: asql_ctparam_tinyint_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
		if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_in,"@va_pr_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE570_Bouchons_Proches: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
		if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_ech_in,"@va_ech_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE570_Bouchons_Proches: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK  );		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
		/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
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
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
						
						XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches : PARAMETER RESULTS\n");
						
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Bouchons_Proches retourne 1");
                                                   	asql_clean_cmd (pl_connection,pl_cmd);
                                                   	free(pl_Tab_param_retour);
							return (XDC_NOK);
                                                }

						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                	XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: asql_fetch_* a echoue");
                                	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
                                        asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
                                }

				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_res_info(CS_MSGTYPE) a echoue");
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
						asql_clean_cmd (pl_connection,pl_cmd);
						free(pl_Tab_param_retour);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE570_Bouchons_Proches : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
					free(pl_Tab_param_retour);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAE570_Bouchons_Proches: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE570_Bouchons_Proches retourne %d", XDC_NOK);
					asql_clean_cmd (pl_connection,pl_cmd);
					free(pl_Tab_param_retour);
					return (XDC_NOK);
				}
		
	}
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"xzae570 : nb de bouchons = %d",vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */

           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }

	else
           {
              if ( ( vl_retcode = (*pa_FonctionUtilisateur_in) (
                                        * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value /*num et cle Q*/
                                        ,* (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value
					,* (XDY_Entier *) vl_Tab_Ligne[i][2].pt_value /*num et cle T*/
					,* (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value
                                        ,* (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value /*autorouteQ*/
					,* (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value /*sensQ*/
										,* (XDY_PR *) vl_Tab_Ligne[i][6].pt_value /*PR Q*/
										,* (XDY_PR *) vl_Tab_Ligne[i][7].pt_value /*PR T*/
										,* (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value /*PT CAR Q*/
										,* (XDY_Octet *) vl_Tab_Ligne[i][9].pt_value /*PT CAR T*/
										,* (XDY_Entier *) vl_Tab_Ligne[i][10].pt_value /*num et cle cause*/
										,* (XDY_Eqt *) vl_Tab_Ligne[i][11].pt_value
										,* (XDY_Eqt *) vl_Tab_Ligne[i][12].pt_value /*autorouteT*/
										,* (XDY_Octet *) vl_Tab_Ligne[i][13].pt_value /*sensT*/
										, &vl_cr ) ) != XDC_OK )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
                 asql_clean_cmd(pl_connection, pl_cmd);
                 return (XDC_NOK);
              }

           }
	}
	if (vl_Nb_Ligne_Lue>0)
          asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE570_Rechercher_PAL_amont retourne  d", vl_retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);

}
