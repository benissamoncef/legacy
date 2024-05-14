/*E*/
/*Fichier : $Id: xzas51.c,v 1.1 1996/10/09 17:34:34 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:34:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS51 * FICHIER xzas51.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tacli
* permet de mettre en base les donnees de Nombre
* d'essieux par classe de poids
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas51.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas51.c,v 1.1 1996/10/09 17:34:34 gaborit Exp $ : xzas51" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS51_Ajout_Mesure_EC : met en base les donnees de comptages.
*  Appelle XZAS51SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS51_Ajout_Mesure_EC (	XDY_Mot  	va_Station_in ,
				XDY_Sens 	va_Sens_in,
				XDY_Octet 	va_Voie_in,
				XDY_Horodate	va_Horodate_in,
				XDY_Mot		*pa_tEC_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot               va_Station_in   : numero de station
* XDY_Sens              va_Sens_in
* XDY_Octet             va_Voie_in
* XDY_Datetime          va_Horodate_in
* XDY_Mot               pa_tEC_in : tableau des donnees EC
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
* - va_Station_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si pa_EC_in[i] est non value, utiliser XDC_RADT_Q_INCONNU
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
*

* FONCTION :
*
* Inserer ou Modifier dans la table MESURE_TRAFIC_6_MINUTES
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzas51.c,v 1.1 1996/10/09 17:34:34 gaborit Exp $ : XZAS51_Ajout_Mesure_EC" ;
	
	CS_CHAR 	*pl_rpc_name = XZAS51C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAS51C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	
	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS51_Ajout_Mesure_EC : Debut d execution" );
			
	/*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
		return (XDC_NOK);
        }
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Station_in,"@va_Station_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_Station_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_tinyint_input(va_Sens_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Voie_in,"@va_Voie_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_tinyint_input(va_Voie_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_date_input(va_Horodate_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[0],"@va_EC1_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC1_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[1],"@va_EC2_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC2_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[2],"@va_EC3_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC3_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[3],"@va_EC4_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC4_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[4],"@va_EC5_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC5_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[5],"@va_EC6_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC6_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[6],"@va_EC7_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC7_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[7],"@va_EC8_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC8_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[8],"@va_EC9_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC9_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[9],"@va_EC10_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC10_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[10],"@va_EC11_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC11_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tEC_in[11],"@va_EC12_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: asql_ctparam_smallint_input(va_EC12_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS51: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS51 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS51 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS51 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS51: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS51_Ajout_Mesure_EC retourne %d", vl_retcode);
 	return(XDC_OK);
}



