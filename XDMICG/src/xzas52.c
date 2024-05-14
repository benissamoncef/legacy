/*E*/
/*Fichier : $Id: xzas52.c,v 1.1 1996/10/09 17:34:35 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:34:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS52 * FICHIER xzas52.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tacli
* permet de mettre en base les donnees de debits
* horaires par classe de silhouette
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas52.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas52.c,v 1.1 1996/10/09 17:34:35 gaborit Exp $ : xzas52" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS52_Ajout_Mesure_PC : met en base les donnees de comptages.
*  Appelle XZAS52SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS52_Ajout_Mesure_PC (	XDY_Mot  	va_Station_in ,
				XDY_Sens 	va_Sens_in,
				XDY_Octet 	va_Voie_in,
				XDY_Horodate	va_Horodate_in,
				XDY_Mot		*pa_tPC_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot               va_Station_in   : numero de station
* XDY_Sens              va_Sens_in
* XDY_Octet             va_Voie_in
* XDY_Datetime          va_Horodate_in
* XDY_Mot               pa_tPC_in : tableau des donnees PC
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
* - si pa_PC_in[i] est non value, utiliser XDC_RADT_Q_INCONNU
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
	static char *version = "$Id: xzas52.c,v 1.1 1996/10/09 17:34:35 gaborit Exp $ : XZAS52_Ajout_Mesure_PC" ;
	
	CS_CHAR 	*pl_rpc_name = XZAS52C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAS52C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	
	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS52_Ajout_Mesure_PC : Debut d execution" );
			
	/*A
        ** Conversion des chaines de caracteres en entree du format Unix au format Sybase
        */

        if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
		return (XDC_NOK);
        }
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Station_in,"@va_Station_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_Station_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_tinyint_input(va_Sens_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Voie_in,"@va_Voie_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_tinyint_input(va_Voie_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_date_input(va_Horodate_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[0],"@va_PC1_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC1_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[1],"@va_PC2_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC2_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[2],"@va_PC3_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC3_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[3],"@va_PC4_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC4_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[4],"@va_PC5_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC5_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&pa_tPC_in[5],"@va_PC6_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: asql_ctparam_smallint_input(va_PC6_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS52: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS52 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS52 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS52 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS52: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS52_Ajout_Mesure_PC retourne %d", vl_retcode);
 	return(XDC_OK);
}



