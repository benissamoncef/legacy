/*E*/
/*Fichier : @(#)xzae120.c	1.1      Release : 1.1        Date : 11/25/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE120 * FICHIER xzae120.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tacli
* permet de mettre en base les donnees meteo
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	10 oct 1996	: Creation (dem/1232)
* PFE   17/11/06        DEM 591 : on supprime les free si pas de alloc pb en version HPUX 11.11
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae120.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae120.c	1.1 11/25/96 : xzae120" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE120_Creation_Evt_ADA : met en base les .
*  Appelle XZAE120SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE120_Creation_Evt_ADA (	char		va_reference_in[50] ,
				int		va_version_in,
				XDY_Horodate	va_date_in,
				char		va_texte_in[250],
				char		va_type_in[50]);





/*int XZAE120_Creation_Evt_ADA (	XDY_Eqt  	va_version_in ,
					XDY_Horodate	va_date_in,
						XDY_Booleen	va_Dispo_in,
						XDY_Octet	va_Code_in,
						XDY_Horodate    va_Horo_debut_in,
						XDY_Horodate    va_Horo_fin_in) */

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot               va_version_in   : numero de station
* XDY_Sens              va_Sens_in
* XDY_Octet             va_Voie_in
* XDY_Datetime          va_Horodate_in
* XDY_Booleen		va_Dispo
* XDY_Octet		va_Code
*

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
*
* FONCTION :
*
* Inserer ou Modifier dans la table MES_MET
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzae120.c	1.1 11/25/96 : XZAE120_Creation_Evt_ADA" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE120C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE120C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	
	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate, vl_Horo_debut, vl_Horo_fin;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE120_Creation_Evt_ADA : Debut d execution" );
			
	/*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        if ((vl_retcode = asql_date_Ux2Sybase(va_date_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_date_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
		return (XDC_NOK);
        }
        
        
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/        
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_reference_in,"@va_reference_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae120: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           /* FPE 
	   free(Tab_donnees2);
           */
	   return (XDC_NOK);
        }


       if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_texte_in,"@va_texte_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae120: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           /* FPE 
	   free(Tab_donnees2);
           */
	   return (XDC_NOK);
        }

       if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_type_in,"@va_type_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae120: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           /* FPE 
	   free(Tab_donnees2);
           */
	   return (XDC_NOK);
        }




        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_version_in,"@va_version_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: asql_ctparam_smallint_input(va_version_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Jour_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: asql_ctparam_date_input(va_Horodate_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	
		
	
		
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE120: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE120 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE120 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE120 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE120_Creation_Evt_ADA retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE120: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE120_Creation_Evt_ADA retourne %d", vl_retcode);
 	return(XDC_OK);
}



