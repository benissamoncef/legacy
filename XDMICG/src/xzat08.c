/*E*/
/*  Fichier : 	$Id: xzat08.c,v 1.2 1994/12/19 16:16:47 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/12/19 16:16:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat08.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       03 Nov 1994     : Creation
* volcic       30 Nov 1994     : Ajout parametres sorties (v1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

static char *version = "$Id: xzat08.c,v 1.2 1994/12/19 16:16:47 volcic Exp $ : xzat08" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT08SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT08_Utilisation_Echangeur ( 	XDY_Horodate	va_Horodate_in,
					XDY_Mot		va_NumEqt_in,
					XDY_Octet 	*va_DispoEchangeur_out,
					XDY_Octet 	*va_Sequence_out,
					XDY_Entier 	*va_NumEvt_out,
					XDY_Octet 	*va_CleEvt_out,
					XDY_Octet 	*va_Priorite_out,
					XDY_Entier 	*va_DistanceEvt_out )

/*X*/
/*-----------------------------------------------------
* S≈quence d'appel
* CL	XZAT08_Utilisation_Echangeur
* 
* Arguments en entr≈e
* XDY_Horodate	va_Horodate_in : date comprise entre le succes et la 
*                                fin de l'action correspondante
* XDY_Eqt	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet	va_DispoEchangeur_out
* XDY_Octet	va_Sequence_out
* XDY_Evt	va_NumEvt_out
* XDY_Evt	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* select dans les tables EQUIPEMENT, AFFICHAGE_ECHANGEUR, 
* ACTION et INDISPONIBILITE
* 
------------------------------------------------*/


{

	static char *version = "$Id: xzat08.c,v 1.2 1994/12/19 16:16:47 volcic Exp $ : XZAT08_Utilisation_Echangeur" ;

	CS_CHAR *rpc_name = XZATC_XZAT08_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZATC_XZAT08_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier	vl_Action;
	XDY_Octet	vl_SiteAction;
	
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT08_Utilisation_Echangeur : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_smallint_input(NumEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_DispoEchangeur_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoEchangeur_out,"@va_DispoEchangeur_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_tinyint_output(DispoEchangeur) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_Sequence_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Sequence_out,"@va_Sequence_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_tinyint_output(Sequence) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[2].pt_value = (CS_VOID *)va_NumEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEvt_out,"@va_NumEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_int_output(NumEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_CleEvt_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_CleEvt_out,"@va_CleEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[4].pt_value = (CS_VOID *)va_Priorite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_tinyint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_DistanceEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceEvt_out,"@va_DistanceEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_int_output(DistanceEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[6].pt_value = (CS_VOID *)&vl_Action;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_Action,"@va_Action_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_int_output(Action) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)&vl_SiteAction;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAction,"@va_SiteAction_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: asql_ctparam_tinyint_output(SiteAction) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzat08: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzat08 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzat08 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat08: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT08_Utilisation_Echangeur retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}




