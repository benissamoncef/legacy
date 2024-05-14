/*E*/
/* Fichier : $Id: xzat100.c,v 1.1 2012/07/04 10:49:12 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:49:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat100.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		   01 Fevrier : Cr�ation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

static char *version = "@(#)xzat100.c	1.3 04/19/95 : xzat100" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT100SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT100_Utilisation_PRV  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot				va_NumEqt_in,
				XDY_Octet 			*va_DispoPRV_out,
				XDY_Octet 			*va_TypePRV_out,
				XDY_Bandeau			*va_Bandeau_out,
				XDY_VitesseRegulTxt	*va_Vitesse_out,
				XDY_Entier 			*va_NumEvt_out,
				XDY_Octet 			*va_CleEvt_out,
				XDY_Octet 			*va_Priorite_out,
				XDY_Entier 			*va_DistanceEvt_out,
				XDY_Entier			*va_Action_out,
				XDY_NomSite			*va_SiteAction_out)
					
	
	

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'etat de disponibilit� et d'affichage d'un PRV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT100_Utilisation_PRV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPRV_out
* XDY_Octet  	va_TypePRV_out
* XDY_Bandeau	va_Bandeau_out
* XDY_Entier	va_Vitesse_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
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
* select dans les tables EQUIPEMENT, E_PRV, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/


{

	static char *version = "@(#)xzat100.c	1.3 04/19/95 : XZAT100_Utilisation_PRV" ;

	CS_CHAR *rpc_name = XZATC_XZAT100_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZATC_XZAT100_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZATC_XZAT100_NB_PARM_RETURN];
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_Action;
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
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT100_Utilisation_PRV : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_smallint_input(NumEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_DispoPRV_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPRV_out,"@va_DispoPRV_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output(DispoPRV) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_TypePRV_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_TypePRV_out,"@va_TypePRV_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output(TypePRV) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	


	Tab_donnees2[2].pt_value = (CS_VOID *)va_Bandeau_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Bandeau_out,"@va_Bandeau_out")) != CS_SUCCEED)	
	
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output(Bandeau) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_Vitesse_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Vitesse_out,"@va_Vitesse_out")) != CS_SUCCEED)

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_bit_output(Vitesse) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[4].pt_value = (CS_VOID *)va_NumEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEvt_out,"@va_NumEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_int_output(NumEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_CleEvt_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_CleEvt_out,"@va_CleEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[6].pt_value = (CS_VOID *)va_Priorite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)va_DistanceEvt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceEvt_out,"@va_DistanceEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_int_output(DistanceEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[8].pt_value = (CS_VOID *)&vl_Action;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_Action,"@va_Action_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_int_output(Action) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[9].pt_value = (CS_VOID *)&vl_SiteAction;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAction,"@va_SiteAction_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: asql_ctparam_tinyint_output(SiteAction) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzat100 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat100 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat100: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT100_Utilisation_PRV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




