/*E*/
/*Fichier : $Id: xzae138.c,v 1.4 2007/03/26 10:39:35 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2007/03/26 10:39:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae138.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les infos obligatoires pour
* generer le fichier des evts strada
*
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot	12 Mai 1997	: Creation (dem/strada)
* Niepceron	04 Mar 1999	: ajout pr tete 1.3
* Niepceron     17 jan 2007     :  Modif pour recup info metacor v1.4 DEM605
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae138.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae138.c,v 1.4 2007/03/26 10:39:35 gesconf Exp $ : xzae138" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE138_Lire_Fiche_Basculement_Pour_Strada : recupere les parametres
* de sortie de xzae;138
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/



extern int XZAE138_Lire_Fiche_Basculement_Pour_Strada ( XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Entier      *va_longueur_out,
					XDY_Entier      *va_prtete_out,
					XDY_Octet	*va_type_loca_out,
					char		*va_nom_loca_out,
					int 		*va_num_loca_out,
					char		*va_ConfigVoies_out,
					char		*va_ConfigVoiesSuite_out)


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
	static char *version = "$Id: xzae138.c,v 1.4 2007/03/26 10:39:35 gesconf Exp $ : XZAE138_Lire_Fiche_Basculement_Pour_Strada" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE138C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE138C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE138C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;


	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE138_Lire_Fiche_Basculement_Pour_Strada : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_numero_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_longueur_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_longueur_out,"@va_longueur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae138: asql_ctparam_int_output(longueur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
       
	i=i+1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_prtete_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prtete_out,"@va_prtete_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae138: asql_ctparam_int_output(prtete) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
        }

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_type_loca_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_type_loca_out,"@va_type_loc_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(type_loca) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}


	i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_nom_loca_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_nom_loca_out,"@va_nom_loc_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(va_nom_loca_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT :  XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_num_loca_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_num_loca_out,"@va_num_loc_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_int_output(num_loca) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}


	i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_ConfigVoies_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_ConfigVoies_out,"@va_restriction_out_1")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(va_ConfigVoies_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_ConfigVoiesSuite_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_ConfigVoiesSuite_out,"@va_restriction_out_2")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(va_ConfigVoies_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae138: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE138 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE138 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE138 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE138: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada :\nlongueur=%d",
			va_longueur_out);
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE138_Lire_Fiche_Basculement_Pour_Strada retourne %d", vl_retcode);
 	return(XDC_OK);
}



