/*E*/
/*Fichier : $Id: xzao826.c,v 1.1 2017/02/10 09:05:02 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2017/02/10 09:05:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao826.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	27/09/00	creation   
* JPL		19/11/10 : Prototype fonction conforme au modele declare (DEM 961)  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao826.h"
static char *version = "$Id: xzao826.c,v 1.1 2017/02/10 09:05:02 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2017/02/10 09:05:02 $ : xzao826.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO826SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO826_Rechercher_les_seconds_PMV_SAGA_amont (	XDY_Autoroute	va_autoroute_in,
						XDY_Sens	va_sens_in,
						XDY_PR		va_pr_in,
						XDY_PR		va_pr_tete_in,
						XDY_Entier	va_Dmax_in,
						XDY_Entier	va_Dmax_bifurc_in,
						XDY_PR		va_pr_bifamont_in,
						XDY_PR		va_pr_bifamont_tete_in,
						XDY_Autoroute	va_aut_bifamont_in,
						XDY_Autoroute	va_aut_bifamont_tete_in,
						XDY_PR		va_pr_debut_bifamont_in,
						XDY_PR		va_pr_debut_bifamont_tete_in,
						XDY_District	va_district_in,
						XDY_Entier	va_priorite_in,
						XDY_Horodate	va_horodate_in,
						char		*va_perturbation_in,
						XDY_FonctionInt pa_FonctionUtilisateurZDP_in,
						XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des zones de parcours 
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
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_ZDP
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR *rpc_name = XZAOC_XZAO826_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;
	int 		vl_cr=0;

	/*A
	** Definition des variables locales
	*/
	
	XDY_ZDP	vl_Eqt;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont aut=%d, sens = %d,PR=%d, PR tete=%d, Dmax=%d, Dmaxbifurc=%d,PR bifamont=%d,PR bifamont tete=%d,aut bifamont=%d,aut bifamont tete=%d, PR deb bifamont=%d, PR deb bifamont tete=%d",
						va_autoroute_in,
						va_sens_in,
						va_pr_in,
						va_pr_tete_in,
						va_Dmax_in,
						va_Dmax_bifurc_in,
						va_pr_bifamont_in,
						va_pr_bifamont_tete_in,
						va_aut_bifamont_in,
						va_aut_bifamont_tete_in,
						va_pr_debut_bifamont_in,
						va_pr_debut_bifamont_tete_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_tinyint_input(va_Autoroute_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_tinyint_input(va_Sens_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }


	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_in,"@va_PR_evt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);

		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_tete_in,"@va_PR_tete_evt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Dmax_in,"@va_Dmax_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_distance_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_Dmax_bifurc_in,"@va_Dmax_bifurc_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_Dmax_bifurc_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_bifamont_in,"@va_pr_bifamont_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_pr_bifamont_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_bifamont_tete_in,"@va_pr_bifamont_tete_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_pr_bifamont_tete_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_aut_bifamont_in,"@va_aut_bifamont_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_tinyint_input(va_aut_bifamont_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_aut_bifamont_tete_in,"@va_aut_bifamont_tete_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_tinyint_input(va_aut_bifamont_tete_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_debut_bifamont_in,"@va_pr_debut_bifamont_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_pr_debut_bifamont_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_debut_bifamont_tete_in,"@va_pr_debut_bifamont_tete_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: asql_ctparam_int_input(va_pr_debut_bifamont_tete_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao826: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao826 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao826 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao826 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao826: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao826: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao826: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur pour chaque UGTP 
	*/
	
	*va_Resultat_out = 0;
	
	XZST_03EcritureTrace(XZSTC_INFO,"xzao826 : nb de PMV = %d",vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( pa_FonctionUtilisateurZDP_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurZDP_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurZDP_in) ( 
					* (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value
					,* (XDY_District *) vl_Tab_Ligne[i][1].pt_value
					,* (XDY_Autoroute *) vl_Tab_Ligne[i][2].pt_value
					,* (XDY_PR *) vl_Tab_Ligne[i][3].pt_value
					,* (XDY_Entier *) vl_Tab_Ligne[i][4].pt_value
					,* (XDY_Sens *) vl_Tab_Ligne[i][5].pt_value
					,(char *) vl_Tab_Ligne[i][6].pt_value
					,* (XDY_Entier *) vl_Tab_Ligne[i][7].pt_value
					,va_district_in
					,va_priorite_in
					,va_horodate_in
					,va_perturbation_in
					, &vl_cr ) ) != XDC_OK ) 
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurZDP_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return (XDC_NOK);
	      }
	      *va_Resultat_out = *va_Resultat_out || vl_cr;
	   }	
	   
	}
	
	if (vl_Nb_Ligne_Lue>0)
	  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO826_Rechercher_les_seconds_PMV_SAGA_amont retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




