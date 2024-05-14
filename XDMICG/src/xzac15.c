/*E*/
/*Fichier : $Id: xzac15.c,v 1.3 2019/01/29 11:24:16 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/01/29 11:24:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzac15.c
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
* ABE	01/02/22		creation pour traitement depannage AVA DEM-SAE367
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac15.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzac15.c	1.2 12/03/97 : xzac15" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAC15_Creer_Modifier_Action : créer un action dans ACT_GEN
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAC15_Creer_Modifier_Action ( XDY_Entier		va_NumEvt_in,
                            XDY_Octet		va_CleEvt_in,
                            XDY_Entier		va_NumAction_in,
                            XDY_Octet		va_Site_in,
                            XDY_Nom		va_NomDepanneur_in,
                            XDY_Nom		va_Resultat_in,
                            XDY_Horodate		va_HoroAppel_in,
                            XDY_Horodate		va_HoroArrivee_in,
                            XDY_Operateur		va_Operateur_in,
                            XDY_Horodate		va_HoroDepart_in,
                            XDY_Horodate		va_HoroAnnul_in,
                            XDY_Octet             va_renfort_in,
                            XDY_Octet             va_patrouille_presente_in,
                            XDY_Octet             va_garagiste_contact_pc_in,
                            XDY_Octet             va_annul_intervention_in,
                            XDY_Entier            va_code_type_astreinte_in,
                            XDY_Entier            va_interlocuteur_in,
                            XDY_Octet		va_site_responsable_in)
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomDepanneur_in
* XDY_Nom		va_Resultat_in
* XY_Horodate		va_HoroAppel_in
* XY_Horodate		va_HoroArrivee_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroDepart_in
* XY_Horodate		va_HoroAnnul_in
* XDY_Octet             va_renfort_in
* XDY_Octet             va_patrouille_presente_in
* XDY_Octet             va_garagiste_contact_pc_in
* XDY_Octet             va_annul_intervention_in
* XDY_Entier            va_code_type_astreinte_in
* XDY_Entier            va_interlocuteur_in
* XDY_Octet		va_site_responsable_in
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
	static char *version = "@(#)xzac15.c	1.2 12/03/97 : XZAC15_Creer_Modifier_Action" ;
	
	CS_CHAR 	*pl_rpc_name = XZAC15C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_DATETIME     va_Horodate;


	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC15_Creer_Modifier_Action : Debut d execution" );
	XZST_03EcritureTrace( XZSTC_DEBUG1,"%d %d %d %d %s %s %lf %lf %d %lf %lf %d %d %d %d %d %d %d",
								va_NumEvt_in,
								va_CleEvt_in,
								va_NumAction_in,
								va_Site_in,
								va_NomDepanneur_in,
								va_Resultat_in,
								va_HoroAppel_in,
								va_HoroArrivee_in,
								va_Operateur_in,
								va_HoroDepart_in,
								va_HoroAnnul_in,
								va_renfort_in,
								va_patrouille_presente_in,
								va_garagiste_contact_pc_in,
								va_annul_intervention_in,
								va_code_type_astreinte_in,
								va_interlocuteur_in,
								va_site_responsable_in);
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
		
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_int_input(va_NumEvt_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_CleEvt_in,) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_NumAction_in,"@va_NumAction_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_int_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
    if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_Site_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_char_input (pl_cmd, (CS_CHAR *) va_NomDepanneur_in, "@va_NomDepanneur_in")) != CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_char_input(va_NomDepanneur_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_char_input (pl_cmd, (CS_CHAR *) va_Resultat_in, "@va_Resultat_in")) != CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_char_input(va_Resultat_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
    if ((vl_retcode = asql_date_Ux2Sybase(va_HoroAppel_in,&va_Horodate))!= XDC_OK)
    {

            XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_date_Ux2Sybase(va_HoroAppel_in, va_Horodate) a echoue");
            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d ", vl_retcode);
            return (XDC_NOK);
    }
	if (va_HoroAppel_in !=-1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroAppel_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_date_input(va_HoroAppel_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
    if ((vl_retcode = asql_date_Ux2Sybase(va_HoroArrivee_in,&va_Horodate))!= XDC_OK)
    {

            XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_date_Ux2Sybase(va_HoroArrivee_in, va_Horodate) a echoue");
            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d ", vl_retcode);
            return (XDC_NOK);
    }
	if (va_HoroArrivee_in != -1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroArrivee_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_date_input(va_HoroArrivee_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Operateur_in,"@va_Operateur_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
    if ((vl_retcode = asql_date_Ux2Sybase(va_HoroDepart_in,&va_Horodate))!= XDC_OK)
    {

            XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_date_Ux2Sybase(va_HoroDepart_in, va_Horodate) a echoue");
            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d ", vl_retcode);
            return (XDC_NOK);
    }
	if (va_HoroDepart_in !=-1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroDepart_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_date_input(va_HoroDepart_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
    if ((vl_retcode = asql_date_Ux2Sybase(va_HoroAnnul_in,&va_Horodate))!= XDC_OK)
    {

            XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_date_Ux2Sybase(va_HoroAnnul_in, va_Horodate) a echoue");
            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d ", vl_retcode);
            return (XDC_NOK);
    }
	if (va_HoroAnnul_in != -1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroAnnul_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_date_input(va_HoroAnnul_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_renfort_in,"@va_renfort_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_renfort_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_patrouille_presente_in,"@va_patrouille_presente_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_patrouille_presente_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_garagiste_contact_pc_in,"@va_garagiste_contact_pc_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_garagiste_contact_pc_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_annul_intervention_in,"@va_annul_intervention_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_annul_intervention_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}		
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_code_type_astreinte_in,"@va_code_type_astreinte_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_int_input(va_code_type_astreinte_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}		
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_interlocuteur_in,"@va_interlocuteur_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_int_input(va_interlocuteur_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
    if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_site_responsable_in,"@va_site_responsable_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: asql_ctparam_tinyint_input(va_site_responsable_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
    }	

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac15: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC15 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC15 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAC15 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAC15_Creer_Modifier_Action retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAC15: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC15_Creer_Modifier_Action retourne %d", vl_retcode);
 	return(XDC_OK);
}



