/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae146.c
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
* Niepceron	03 Fev 1999	: Ajout num table dem/1724 v1.3
* PNI		13/11/2008	: Modif niv des traces WARNING->INFO v1.4 1.5
* JMG           15/02/11        : ajout nom autoroute en sortie datex V2 1.6
* JMG		22/04/15 	: portage linux chaines sortie 1.7
* JPL		21/02/24	: Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae146.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae146.c 1.8" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE146_Recherche_Loc_Rds : recupere les parametres
* de sortie de xzae;146
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE146_Recherche_Loc_Rds ( 
				XDY_Entier      va_pr,
                                XDY_Octet      va_autoroute,
                                char            *va_num_table_out,
                                XDY_Entier      *va_pr_inf_out,
                                XDY_Entier      *va_pr_sup_out,
                                char            *ta_nom_inf_out,
                                char            *ta_nom_sup_out,
                                XDY_Entier      *va_num_inf_out,
                                XDY_Entier      *va_num_sup_out,
				char            *va_nom_autoroute_out
                                )



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
	CS_CHAR 	*pl_rpc_name = XZAEC_XZAE146_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE146C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE146C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE146_Recherche_Loc_Rds : Debut d execution %d %d",
			va_pr,va_autoroute);


	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_pr,"@va_pr")) != CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_int_input(va_pr) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }



        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_autoroute,"@va_autoroute")) != CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_tinyint_input(va_autoroute) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	i = 0;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_num_table_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_num_table_out,"@va_num_table")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_char_output(num table) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_pr_inf_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_pr_inf_out,"@va_pr_inf")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_int_output(va_pr_inf_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_pr_sup_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_pr_sup_out,"@va_pr_sup")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_int_output(va_pr_sup_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)ta_nom_inf_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)ta_nom_inf_out,"@va_nom_inf")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_char_output(ta_nom_inf_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)ta_nom_sup_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)ta_nom_sup_out,"@va_nom_sup")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_char_output(ta_nom_sup_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_num_inf_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_num_inf_out,"@va_num_inf")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_int_output(va_num_inf) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_num_sup_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_num_sup_out,"@va_num_sup")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_int_output(va_num_sup_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_nom_autoroute_out;

	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_nom_autoroute_out,"@va_nom_autoroute")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae146: asql_ctparam_char_output(va_nom_autoroute_out) a echoue ");
	     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
	asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
   }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_INFO,"XZAE146: vl_num_Tab_donnees2:%d",vl_num_Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae146: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE146 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE146 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE146 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE146_Recherche_Loc_Rds retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE146: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	strcpy(ta_nom_inf_out, (char *)vl_Tab_Ligne[0][0].pt_value);
	strcpy(ta_nom_sup_out, (char *)vl_Tab_Ligne[0][1].pt_value);
	strcpy(va_nom_autoroute_out, (char *)vl_Tab_Ligne[0][2].pt_value);
	strcpy(va_num_table_out, (char *)vl_Tab_Ligne[0][3].pt_value);

	XZST_03EcritureTrace(	XZSTC_INFO,
			"OUT : XZAE146_Recherche_Loc_Rds : %d/%d/%s/%s/%s/%d/%d/%s/%s",
			*va_pr_inf_out,
			*va_pr_sup_out,
			va_num_table_out,
			ta_nom_inf_out,
			ta_nom_sup_out,
			*va_num_inf_out,
			*va_num_sup_out,
			va_nom_autoroute_out,va_num_table_out);

	
	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE146_Recherche_Loc_Rds retourne %d", vl_retcode);
 	return(XDC_OK);
}
