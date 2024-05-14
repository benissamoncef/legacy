/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae127.c
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
* Guilhou	02/12/97	: ajout zz et sens en sortie 1.4
* Niepceron 	14/01/99	: ajout trafic et boies op 1.5 dem/1624
* Niepceron     04/03/99	: ajout gravite 1.6
* F.Lizot       07/05/2004      : Ajout de la reference datex (SAGA) v1.7
* PNI		13/11/2008	: Modif niv des traces WARNING->INFO v1.8
* JMG		22/04/15	: linux chaines sortie 1.9
* JPL		21/02/24	: Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae127.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae127.c 1.10" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE127_Infos_obligatoires : recupere les parametres
* de sortie de xzae;127
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE127_Infos_obligatoires (XDY_Entier  	va_numero_in ,
				XDY_Octet	va_cle_in,
				XDY_Horodate	*va_debut,
				XDY_Horodate	*va_fin,
				XDY_Nom		va_fausse_alerte_out,
				XDY_Entier	*va_duree_out,
				XDY_Nom		va_fin_evt_out,
				XDY_Horodate	*va_validation,
				XDY_Entier	*va_numero_cause_out,
				XDY_Octet	*va_cle_cause_out,
				XDY_Nom		va_type_cause_out,
				XDY_Octet	*va_veracite_out,
				XDY_Nom		va_prevision_out,
				XDY_Nom		va_localisation_out,
				XDY_Entier	*va_zz_out,
				XDY_Octet	*va_sens_out,
				XDY_Entier	*va_pr_out,
				XDY_Octet	*va_autoroute_out,
				XDY_Octet	*va_trafic_out,
				XDY_Octet       *va_gravite_out,
                                XDY_Octet       *va_nbvoiesop_out,
				XDY_Nom         va_reference_out)


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
	CS_CHAR 	*pl_rpc_name = XZAE127C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE127C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE127C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE127_Infos_obligatoires : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_numero_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
        Tab_donnees2[i].pt_value = (CS_VOID *)&va_debut_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&va_debut_out,"@va_debut_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_date_output(debut) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_fin_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&va_fin_out,"@va_fin_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_date_output(fin) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_fausse_alerte_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_fausse_alerte_out,"@va_fausse_alerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(fausse alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_duree_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_duree_out,"@va_duree_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_int_output(duree) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_evt_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_fin_evt_out,"@va_fin_evt_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(fin evt) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_validation_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&va_validation_out,"@va_validation_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_date_output(validation) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numero_cause_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_numero_cause_out,"@va_numero_cause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_int_output(numero cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_cle_cause_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_cle_cause_out,"@va_cle_cause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(cle cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_type_cause_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_type_cause_out,"@va_type_cause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(type cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_veracite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_veracite_out,"@va_veracite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(veracite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_prevision_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_prevision_out,"@va_prevision_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(prevision) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
          
        i = i + 1;


        Tab_donnees2[i].pt_value = (CS_VOID *)va_localisation_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_localisation_out,"@va_localisation_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(localisation) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_zz_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_zz_out,"@va_zz_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_int_output(zz) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_sens_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_sens_out,"@va_sens_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(sens) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_pr_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_pr_out,"@va_pr_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_int_output(pr) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_autoroute_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_autoroute_out, "@va_autoroute_out" )) != CS_SUCCEED)     
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(autoroute) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_trafic_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_trafic_out,"@va_trafic_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(trafic) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_gravite_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_gravite_out,"@va_gravite_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(gravite) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_nbvoiesop_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_nbvoiesop_out,"@va_nbvoiesop_out")) != CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_tinyint_output(nbvoiesop) a echoue ");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
	  asql_clean_cmd(pl_connection, pl_cmd);
	  return (XDC_NOK);
	}
	i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_reference_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_reference_out,"@va_reference_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_ctparam_char_output(va_reference_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	/*A
	** Envoie de la commande au Sql server
	*/
	if ((vl_retcode = ct_send(pl_cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae127: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE127 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE127 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE127 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE127_Infos_obligatoires retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE127: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	/*A
	** Conversion des dates du format Sybase au format Unix
	*/
	
	if ((vl_retcode = asql_date_Sybase2Ux(va_debut_out,va_debut))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_date_Sybase2Ux(va_debut_out,va_debut) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_date_Sybase2Ux(va_fin_out,va_fin))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_date_Sybase2Ux(va_fin_out,va_fin) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_date_Sybase2Ux(va_validation_out,va_validation))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae127: asql_date_Sybase2Ux(va_validation_out,va_validation) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
	
	
	strcpy(va_fausse_alerte_out, (char *)vl_Tab_Ligne[0][0].pt_value);
	strcpy(va_fin_evt_out, (char *)vl_Tab_Ligne[0][1].pt_value);
	strcpy(va_type_cause_out, (char *)vl_Tab_Ligne[0][2].pt_value);
	strcpy(va_prevision_out, (char *)vl_Tab_Ligne[0][3].pt_value);
	strcpy(va_localisation_out, (char *)vl_Tab_Ligne[0][4].pt_value);
	strcpy(va_reference_out, (char *)vl_Tab_Ligne[0][5].pt_value);

	
	XZST_03EcritureTrace(	XZSTC_INFO,
			"OUT : XZAE127_Infos_obligatoires :; debut=%lf; fin=%lf; fausse_alerte=%s; duree=%d; fin_evt=%s; validation=%lf; numero_cause=%d; cle_cause=%d; type_cause=%s; veracite=%d; precision=%s; localisation=%s; zz=%d; sens=%d; pr=%d; autoroute=%d; trafic=%d; gravite=%d; reference=%s",
			*va_debut,
			*va_fin,
			va_fausse_alerte_out,
			*va_duree_out,
			va_fin_evt_out,
			*va_validation,
			*va_numero_cause_out,
			*va_cle_cause_out,
			va_type_cause_out,
			*va_veracite_out,
			va_prevision_out,
			va_localisation_out,
			*va_zz_out,
			*va_sens_out,
			*va_pr_out,
			*va_autoroute_out,
			*va_trafic_out,
			*va_gravite_out,
			va_reference_out);


	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE127_Infos_obligatoires retourne %d", vl_retcode);
 	return(XDC_OK);
}



