/*E*/
/*  Fichier : 	$Id: xzao830.c,v 1.1 2017/02/10 09:05:03 pc2dpdy Exp $	Release : $Revision: 1.1 $        Date : $Date: 2017/02/10 09:05:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao830.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	14/02/11 : Creation d'apres xzao429.c ; ajout "distance" (DEM 953)  1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao830.h"

static char *version = "$Id: xzao830.c,v 1.1 2017/02/10 09:05:03 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2017/02/10 09:05:03 $ : xzao830" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO830SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO830_Ajouter_Zon_PMVA_SAGA ( XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_pmv_in,
                                XDY_Sens                va_sens_pmv_in,
                                XDY_PR                  va_pr_pmv_in,
                                char               	*va_site_pmv_in,
                                XDY_Entier              va_distance_pmv_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
                                char                    *va_direction_zone_in,
                                XDY_Sens		va_type_zone_in,
                                XDY_Booleen             va_inhibition_zone_in,
                                XDY_ResConf             *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une zone d'influence PMVA en base
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
* Insertion ou modification dans la table EQT_PMA (Zone de couverture PMVA)
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR *rpc_name = XZAOC_XZAO830_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO830_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_autoroute_pmv_in,"@va_AutoroutePMV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_char_input(va_autoroute_pmv_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_pmv_in,"@va_SensPMV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_tinyint_input(va_sens_pmv_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_pmv_in,"@va_PrPMV_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_int_input(va_pr_pmv_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_site_pmv_in,"@va_SitePMV_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_char_input(va_site_pmv_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_distance_pmv_in,"@va_DistancePMV_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_int_input(va_distance_pmv_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_autoroute_zone_in,"@va_AutorouteZone_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_char_input(va_autoroute_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_zone_in,"@va_SensZone_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_tinyint_input(va_sens_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_debut_zone_in,"@va_PrDebutZone_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_int_input(va_debut_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_fin_zone_in,"@va_PrFinZone_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_int_input(va_fin_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_direction_zone_in,"@va_DirectionZone_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_char_input(va_direction_zone_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_type_zone_in,"@va_TypeZone_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_tinyint_input(va_type_zone_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
        if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_inhibition_zone_in,"@va_Inhibition_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao830:asql_ctparam_bit_input(Inhibition)  a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao830: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : STATUS RESULT PROCEDURE STOCKEE = %ld", (XDY_LongEntier) SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao830 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : ct_result retourne CS_MSG_RESULT l id du message est : %ld.\n", (XDY_LongEntier) msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao830 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao830: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzao830_Ajouter_Zon_PMVA_SAGA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}
