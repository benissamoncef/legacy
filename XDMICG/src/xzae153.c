/*E*/
/*Fichier : $Id: xzae153.c,v 1.1 1999/02/26 14:40:30 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:40:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae153.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	converti la localisation
*  RDS en Autoroute PR sens
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	23 Dec 1998	:creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae153.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae153.c,v 1.1 1999/02/26 14:40:30 gaborit Exp $ : xzae153" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE153_Localisation_PR : converti la localisation
*  RDS en Autoroute PR sens
*  XZAE153_Localisation_PR : recupere les parametres
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE153_Localisation_PR (	char		*va_l1_in,
					char		*va_l2_in,
					XDY_Octet       va_l3_in,
					int             va_l4_in,
					char            *va_l5_in,
					char            *va_l6_in,
					char            *va_l7_in,
					int             va_l8_in,
					int             va_l9_in,
					char            *va_l10_in,
					int             va_l11_in,
					int             va_l12_in,
					char		*va_libelle_out,
					XDY_Octet	*va_Autoroute_out,
					XDY_PR		*va_PR_out,
					XDY_Sens	*va_Sens_out,
					XDY_Octet       *va_Autoroutetete_out,
					XDY_PR          *va_PRtete_out)



/*
*
* PARAMETRES EN ENTREE :
* char		*va_L1_in,
  char		*va_l2_in,
* tinyint         va_l3_in,
* int             va_l4_in,
* char            *va_l5_in,
* char            *va_l6_in,
* char            *va_l7_in,
* int             va_l8_in,
* int             va_l9_in,
* char            *va_l10_in,
* int             va_l11_in,
* int             va_l12_in,
*				
* PARAMETRES EN SORTIE :
* char		*va_libelle_out,
* XDY_Octet	*va_Autoroute_out,
* XDY_PR		*va_PR_out,
* XDY_Sens	*va_Sens_out,
* XDY_Octet       *va_Autoroutetete_out,
* XDY_PR          *va_PRtete_out
*
*
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
	static char *version = "$Id: xzae153.c,v 1.1 1999/02/26 14:40:30 gaborit Exp $ : XZAE153_Localisation_PR" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE153C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE153C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE153C_NB_PARM_RETURN+2];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
        XDY_Octet	vl_a,vl_at,vl_s;
        XDY_PR		vl_pr,vl_prt;
        char 		vl_lib[300];


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE153_Localisation_PR : Debut d execution \nL1:%sL2:%sL3:%dL4:%dL5:%sL6:%sL7:%sL8:%dL9:%dL10:%sL11:%dL12:%d ",
	va_l1_in,va_l2_in,va_l3_in,va_l4_in,va_l5_in,va_l6_in,va_l7_in,va_l8_in,va_l9_in,va_l10_in,va_l11_in,va_l12_in);

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l1_in,"@va_L1_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L1_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l2_in,"@va_L2_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L2_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_l3_in,"@va_L3_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_date_input(va_L3_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
        
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_l4_in,"@va_L4_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_smallint_input(va_L4_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l5_in,"@va_L5_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L5_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l6_in,"@va_L6_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L6_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l7_in,"@va_L7_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L7_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }
        
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_l8_in,"@va_L8_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_smallint_input(va_L8_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_l9_in,"@va_L9_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_smallint_input(va_L9_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}


        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_l10_in,"@va_L10_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzap09: asql_ctparam_char_input(@va_L10_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAP09_Ajout_Dest_Fax retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }
        
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_l11_in,"@va_L11_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_smallint_input(va_L11_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        
        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_l12_in,"@va_L12_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: asql_ctparam_smallint_input(va_L12_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	
		
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_libelle_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_libelle_out,"@va_libelle_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(libelles) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Autoroute_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Autoroute_out,"@va_Autoroute_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(Autoroute) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PR_out,"@va_PR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(PR) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_Sens_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Sens_out,"@va_Sens_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(Sens) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Autoroutetete_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Autoroutetete_out,"@va_Autoroutetete_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(Autoroutetete_) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_PRtete_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PRtete_out,"@va_PRtete_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae153: asql_ctparam_char_output(PRtete_) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
                  
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae153: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE153 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE153 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE153 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE153_Localisation_PR retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE153: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}


	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"OUT : XZAE153_Localisation_PR :lib:%s,autoroute:%d,PR:%d,sens:%d,tete : auttroute:%d,Pr:%d",
					va_libelle_out,
					*va_Autoroute_out,
					*va_PR_out,
					*va_Sens_out,
					*va_Autoroutetete_out,
					*va_PRtete_out);
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE153_Localisation_PR retourne %d", vl_retcode);
 	return(XDC_OK);
}



