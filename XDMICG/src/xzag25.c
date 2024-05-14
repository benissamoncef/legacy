/*E*/
/*Fichier : $Id: xzag25.c,v 1.8 2010/12/09 00:23:36 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/09 00:23:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG25 * FICHIER xzag25.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a BASEDD, module XZAG
*
* XZAG25 permet de purger la base de l'historique
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	10 Aou 1995	: Creation (V 1.1)
* Torregrossa	11 Aou 1995	: Correction erreur test code retour (V 1.2)
* Torregrossa	04 Sep 1995	: Ajout init nbligne (V 1.3-1.4)
* Niepceron	09 Oct 1996	: Ajout mesures pesage (dem/1227) (V 1.5)
* Niepceron     30 Sep 1997	: Ajout remontee des erreurs v1.6
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.8
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag25.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzag25.c,v 1.8 2010/12/09 00:23:36 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/09 00:23:36 $";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG25_Purge_Evts_Base_Historique : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG25SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG25_Purge_Evts_Base_Historique (	XDY_Horodate	va_Horodate_in, 
				XDY_Booleen	*va_Suppression_out)

/*
*
* PARAMETRES EN ENTREE :
*
*   va_Horodate_in est l horodate au dela de la quelle on doit effectuer
*   la purge.
*				
* PARAMETRES EN SORTIE :
*				
*   va_Suppression_out : indique si au moins un evenement a ete supprime
*
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  On fait appel a XZAG;25 et XZAG;11
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag25.c,v 1.8 2010/12/09 00:23:36 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/09 00:23:36 $";
	
	CS_CHAR 	*	pl_rpc_name25 = XZAG25C_RPC_NAME;
	CS_CHAR 	*	pl_rpc_name11 = XZAG11C_RPC_NAME;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int 			vl_num_Tab_donnees1 = XZAG25C_NB_PARM_RETURN;
	int 			vl_num_Tab_donnees2 = XZAG11C_NB_PARM_RETURN;
	tg_Colonne_donnees2 	vl_Tab_donnees2[XZAG11C_NB_PARM_RETURN];
	int			vl_Nb_Ligne_Lue;
	int			vl_Nb_Col;
	int			i;
	tg_row_result		vl_Tab_Ligne;
	
	XDY_Entier		vl_SPstatus;
	XDY_Booleen		vl_Suppression;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG25_Purge_Evts_Base_Historique : Debut d execution" );

	*va_Suppression_out = XDC_FAUX;
	vl_Nb_Ligne_Lue = 0;
			
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name25, 
					CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: asql_ctparam_date_input(Horodate) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
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
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue, &vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG25: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG25 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG25 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG25 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);

	/*A
	** Purger tous les evenements retournes
	*/
	for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
		/*A
		** Initialisation de la commande				
		*/
	
		if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
		{
		 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: asql_cmd_alloc() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		/*A
		** Envoie de la commande RPC pour notre stored procedure.
		*/
		
		if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name11, 
						CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: ct_command() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
	
		/*A 
		** Initialisation de la structure pour chacun des parametres d I/O	
		** A faire passer a la rpc.
		*/
	
	        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)vl_Tab_Ligne[i][0].pt_value,"@va_NumEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)vl_Tab_Ligne[i][1].pt_value,"@va_CleEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		vl_Tab_donnees2[0].pt_value = (CS_BIT *)&vl_Suppression;

	        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)&vl_Suppression,"@va_Resultat_out"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		/*A
		** Envoie de la commande au Sql server
		*/
		if (ct_send(pl_cmd) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: ct_send() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
	
		/*A
		** Traitement du resultat de la purge d'un evenement
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
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG11: ROW RESULTS");
							break;
	
						case  CS_PARAM_RESULT:
							vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG11 : PARAMETER RESULTS \n");
							fprintf(stdout, "\nPARAMETER RESULTS\n");
							break;
	
						case  CS_STATUS_RESULT:
							vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG11 : CS_STATUS_RESULT = %ld", vl_retcode);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG11 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
							
							if (vl_SPstatus != XDC_OK)
							{
							   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne 1");
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
							XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: ct_res_info(msgtype) a echoue");
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: ct_results retourne CS_CMD_FAIL.");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
	
					default:
						/*
						** Il s est passe quelque chose d inattendu.
						*/
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG11: ct_results retourne un resultat de type inattendu");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
					}
		}
		
		if (vl_Suppression == XDC_VRAI)
			*va_Suppression_out = XDC_VRAI;

 		asql_clean_cmd (pl_connection,pl_cmd);
	}

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par
	** la SP XZAG25
	*/
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	if (vl_retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG25_Purge_Evts_Base_Historique retourne %d", XDC_OK);

 	return(XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_Purge_Base_Historique : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG25SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG_Purge_Base_Historique (XDY_Horodate	va_Horodate_in )

/*
*
* PARAMETRES EN ENTREE :
*
*   va_Horodate_in est l horodate au dela de la quelle on doit effectuer
*   la purge.
*				
* PARAMETRES EN SORTIE :
*				
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  On fait appel a XZAG;25, XZAG;26, XZAG;27, XZAG;28 , XZAG;29, XZAG;31, XZAG;30
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag25.c,v 1.8 2010/12/09 00:23:36 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/09 00:23:36 $";
	
	CS_CHAR 	*	pl_rpc_name ;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode = XDC_NOK;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int			vl_Status,vl_code_retour;
	XDY_Booleen		vl_Suppression;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG_Purge_Base_Historique : Debut d execution" );

	vl_Suppression = XDC_FAUX;
	vl_code_retour = XDC_OK;
	
	/*A
	** Purge des evenements
	*/
	vl_Status = XZAG25_Purge_Evts_Base_Historique(va_Horodate_in, &vl_Suppression);
	while (vl_Suppression == XDC_VRAI && vl_Status == XDC_OK)
	{
		vl_Status = XZAG25_Purge_Evts_Base_Historique(va_Horodate_in, &vl_Suppression);
	}
	if ( vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG: XZAG25_Purge_Evts_Base_Historique a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Base_Historique retourne %d", vl_retcode);
		vl_code_retour = XDC_NOK;
	}
			
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG25: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Base_Historique retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	

	/*A
	** purge des alertes
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG26C_RPC_NAME, XZAG26C_NB_PARM_RETURN, "XZAG26", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des etats des equipements
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG27C_RPC_NAME, XZAG27C_NB_PARM_RETURN, "XZAG27", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des mesures trafic
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG28C_RPC_NAME, XZAG28C_NB_PARM_RETURN, "XZAG28", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des mesures niveau
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG29C_RPC_NAME, XZAG29C_NB_PARM_RETURN, "XZAG29", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des mesures poids
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG30C_RPC_NAME, XZAG30C_NB_PARM_RETURN, "XZAG30", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des appels horaire
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG31C_RPC_NAME, XZAG31C_NB_PARM_RETURN, "XZAG31", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

	/*A
	** purge des mesures pesage
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG52C_RPC_NAME, XZAG_SANS_PARM_RETURN, "XZAG52", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

        /*A
	** purge des mesures tdp
	*/
	if ( XZAG_Purge_MorceauHistorise(XZAG55C_RPC_NAME, XZAG_SANS_PARM_RETURN, "XZAG55", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;


	/*A
	** Message en sortie de la fonction
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG_Purge_Base_Historique retourne %d", XDC_OK);

 	return(vl_code_retour);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_Purge_MorceauHistorise : Purger avec la procedure donnee en entree
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZAG_Purge_MorceauHistorise(	CS_CHAR 	*va_rpc_name_in, 
				XDY_Octet	va_Nb_Param_Return_in,
				char		*va_titre_commentaire_in,
				CS_DATETIME 	*va_Horodate_in)
/*
*
* PARAMETRES EN ENTREE :
*
*   va_rpc_name_in          : nom de la procedure a executer
*   va_Nb_Param_Return_in   : nombre de parametres de sortie de la procedure a executer
*   va_titre_commentaire_in : titre du commentaire pour les messages
*   va_Horodate_in          : l horodate au dela de la quelle on doit effectuer
*                             la purge.
*				
* PARAMETRES EN SORTIE :
*				
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag25.c,v 1.8 2010/12/09 00:23:36 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/09 00:23:36 $";
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	tg_Colonne_donnees2 *	vl_Tab_donnees1;
	int			vl_Nb_Ligne_Lue;
	int			vl_Nb_Col;
	tg_row_result		vl_Tab_Ligne;
	
	XDY_Entier		vl_SPstatus;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,
				"IN : XZAG_Purge_MorceauHistorise : Debut d execution pour %s", 
				va_titre_commentaire_in );

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_cmd_alloc() a echoue", 
						va_titre_commentaire_in);
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG_Purge_MorceauHistorise retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, va_rpc_name_in, 
					CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue",va_titre_commentaire_in);
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG_Purge_MorceauHistorise retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)va_Horodate_in,
							"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,
					"%s: asql_ctparam_date_input(Horodate) a echoue", 
					va_titre_commentaire_in);
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_MorceauHistorise retourne %d", 
					vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_send() a echoue", va_titre_commentaire_in);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_MorceauHistorise retourne %d", 
					vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,
								"XZAG_Purge_MorceauHistorise: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , va_Nb_Param_Return_in, vl_Tab_donnees1);
						XZST_03EcritureTrace(XZSTC_INFO,
							"XZAG_Purge_MorceauHistorise : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,
							"%s : CS_STATUS_RESULT = %ld", 
							va_titre_commentaire_in, vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,
							"%s : STATUS RESULT PROCEDURE STOCKEE = %ld", 
							va_titre_commentaire_in, vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, 
								"OUT : XZAG_Purge_MorceauHistorise retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_res_info(msgtype) a echoue", va_titre_commentaire_in);
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_MorceauHistorise retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne CS_CMD_FAIL.", va_titre_commentaire_in);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_MorceauHistorise retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu", va_titre_commentaire_in);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_MorceauHistorise retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);

	/*A
	** Message en sortie de la fonction
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG_Purge_MorceauHistorise retourne %d", XDC_OK);

 	return(XDC_OK);
}
