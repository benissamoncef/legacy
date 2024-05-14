/*E*/
/*Fichier : $Id: xzar03.c,v 1.1 2016/09/29 15:37:50 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:37:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzar03.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les infos obligatoires pour
* generer le fichier des evts strada
*
------------------------------------------------------
* HISTORIQUE :
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

/* fichiers inclus */

#include "xzar03.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzar03.c	1.2 12/03/97 : xzar03" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAR03_Reveil : recupere les parametres
* de sortie de xzae;53
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAR03_Reveil (       XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Mot      va_PosteOperateur_in,
                                        char     *va_texte_in,
                                        XDY_Mot va_site_in)

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
	static char *version = "@(#)xzar03.c	1.2 12/03/97 : XZAR03_Reveil" ;
	
	CS_CHAR 	*pl_rpc_name = XZAR03C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAR03C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAR03C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_DATETIME     va_Horodate_in;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAR03_Reveil : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_PosteOperateur_in,"@va_PosteOperateur_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: asql_ctparam_date_input(va_Cond_Meteo_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_texte_in,"@va_TexteReveil_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: asql_ctparam_date_input(va_TexteReveil_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}



	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: asql_ctparam_date_input(va_Site_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzar03: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAR03 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAR03 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAR03 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAR03_Reveil retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAR03: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAR03_Reveil");
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAR03_Reveil retourne %d", vl_retcode);
 	return(XDC_OK);
}



