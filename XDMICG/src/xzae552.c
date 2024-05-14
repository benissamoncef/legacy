/*E*/
/*Fichier : @(#)xzae552.c	1.1      Release : 1.1        Date : 05/15/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE552 * FICHIER xzae552.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Cree une FMC à partir d'un evennement DATEX recu.
*
------------------------------------------------------
* HISTORIQUE :
*
*  F. lizot      12 Fev 2004     : Creation (SAGA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae552.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae552.c	1.1 05/15/97 : xzae552" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE552_Creation_FMC:
*  Cree une FMC à partir d'un evennement DATEX recu.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE552_Creation_FMC(char *va_emetteur_in,
			 char *va_reference_in,
			 XDY_Octet   va_cle_in,
			 XDY_Octet   va_site_in,
			 char *va_texte_alt_in,
			 int  *va_FMC_out)


/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in          code du partenaire qui a emis le message
* char *va_reference_in         reference datex associee
* int   va_cle_in               cle a utiliser pour creer la FMC
* int   va_site_in              site a utiliser pour creer la FMC
* char *va_texte_alt_in         texte a utiliser pour creer la FMC
*
* PARAMETRES EN SORTIE :
*
* int  *va_FMC_out              numero de la FMC creee
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
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzae552.c	1.1 05/15/97 : XZAE552_Creation_FMC" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE552C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2=XZAE552C_NB_PARM_RETURN;
	tg_Colonne_donnees2 vl_Tab_donnees2[XZAE552C_NB_PARM_RETURN];
	
	XDY_Entier	vl_SPstatus;
	int		vl_FMC_out;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE552_Creation_FMC: Debut d execution" );

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_emetteur_in,"@va_emetteur_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: asql_ctparam_char_input(@va_emetteur_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_reference_in,"@va_reference_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: asql_ctparam_char_input(@va_reference_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_site_in,"@va_site_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: asql_ctparam_tinyint_input(va_site_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_texte_alt_in,"@va_texte_alt_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: asql_ctparam_char_input(@va_texte_alt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}

	/*
	** 1 parametre en output
	*/
	vl_Tab_donnees2[0].pt_value = (CS_VOID *)&vl_FMC_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)&vl_FMC_out,"@va_FMC_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552 : asql_ctparam_int_output(va_FMC_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
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
						vl_retcode = asql_fetch_data (pl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE552: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE552 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE552 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE552 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE552_Creation_FMC retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE552: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	*va_FMC_out = vl_FMC_out;
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE552_Creation_FMC retourne %d", XDC_OK);
 	return(XDC_OK);
}



