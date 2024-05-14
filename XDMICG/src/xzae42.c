/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae42.c
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
* JMG   11/10/08        CREATION        DEM 835
* JMG   29/09/09        SECTO           DEM 887
* ABE	11/09/20	DEM-SAE130 Datex2 Récuperation du code matiere avec le curseur (pb récuperation par asql_ctparam_char_output) V1.3
* ABE 22/11/21 Fix cas ou le plan d'action est vide DEM-SAE130
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae42.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae42.c 1.5" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE42_Lire_Fiche_Delestage : recupere les parametres
* de sortie de xzae;42
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE42_Lire_Fiche_Delestage (    XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
					char       *va_nomPlan_out,
					XDY_Entier       *va_numPlan_out,
					char       *va_hierarchie_out,
					XDY_PR	*va_PR_out,
					XDY_PR	*va_PR2_out)


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
	CS_CHAR 	*pl_rpc_name = XZAE42C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE42C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE42C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE42_Lire_Fiche_Delestage : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_nomPlan_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_nomPlan_out,"@va_NomPlan_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae42: asql_ctparam_char_output(nom plan) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numPlan_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_numPlan_out,"@va_NumPlan_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae42: asql_ctparam_int_output(numero plan) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_hierarchie_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_hierarchie_out,"@va_Hierarchie_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae42: asql_ctparam_char_output(hierarchie) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
       
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PR_out,"@va_PR_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae42: asql_ctparam_int_output(PR) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR2_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PR2_out,"@va_PR2_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae42: asql_ctparam_int_output(PR2) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae42: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE42 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE42 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE42 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE42_Lire_Fiche_Delestage retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE42: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
//	if (vl_Nb_Ligne_Lue>0)
//	{
//		strcpy(va_nomPlan_out,  (char *)vl_Tab_Ligne[0][0].pt_value);
//	}


	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE42_Lire_Fiche_Delestage retourne %d", vl_retcode);
 	return(XDC_OK);
}



