/*E*/
/*Fichier : $Id: xzag15.c,v 1.18 2020/01/21 16:52:08 gesconf Exp $      Release : $Revision: 1.18 $        Date : $Date: 2020/01/21 16:52:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG15 * FICHIER xzag15.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a BASEDD, module XZAG
*
* XZAG15 permet de purger la base d exploitation.
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	20 Jun 1995	: Creation (V 1.5)
* Torregrossa	11 Aou 1995	: Correction erreur test code retour (V 1.8)
* Torregrossa	23 Aou 1996	: Continuer la purge si pb sur purge evts (V 1.9)
*                                 ajout de traces d'execution de proc
* Torregrossa	09 Sep 1996	: Ajout cas de purge sur pc simplifie ( RADT V 1. 10)
* Niepceron	09 Oct 1996	: Ajout des mesure pesage ( dem/1127) V 1. 11
* Niepceron     27 Fev 1997	: Ajout des tdp (dem/1396) v 1.13
* Niepceron     30 Sep 1997     : Ajout remontee des erreurs v1.14
* Torregrossa	12 Jan 1998	: Ajout cas de purge alerte sur pc 3 (1532) ( V 1. 15)
* Niepceron     30 Sep 1998	: pas de XZAG54 sur PCS 1.16
* PNI           06/09/2019      : DEM1359 - Optimisation Purge                  1.17
* PNI           06/09/2019      : DEM1359 - Optimisation Purge - planification                 1.18
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag15.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzag15.c,v 1.18 2020/01/21 16:52:08 gesconf Exp $  $Revision: 1.18 $  Date : $Date: 2020/01/21 16:52:08 $: XZAG15_Purge_Base_Exploitation";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG15_Purge_Evts_Base_Exploitation : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG15SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG15_Purge_Evts_Base_Exploitation (	XDY_Horodate	va_Horodate_in, 
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
*  On fait appel a XZAG;15 et XZAG;10
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag15.c,v 1.18 2020/01/21 16:52:08 gesconf Exp $ $Revision: 1.18 $   Date : $Date: 2020/01/21 16:52:08 $: XZAG15_Purge_Evts_Base_Exploitation";
	
	CS_CHAR 	*	pl_rpc_name15 = XZAG15C_RPC_NAME;
	CS_CHAR 	*	pl_rpc_name10 = XZAG10C_RPC_NAME;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int 			vl_num_Tab_donnees1 = XZAG15C_NB_PARM_RETURN;
	int 			vl_num_Tab_donnees2 = XZAG10C_NB_PARM_RETURN;
	tg_Colonne_donnees2 	vl_Tab_donnees2[XZAG10C_NB_PARM_RETURN];
	int			vl_Nb_Ligne_Lue;
	int			vl_Nb_Col;
	int			i;
	tg_row_result		vl_Tab_Ligne;
	
	XDY_Entier		vl_SPstatus;
	XDY_Booleen		vl_Suppression;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG15_Purge_Evts_Base_Exploitation : Debut d execution" );

	*va_Suppression_out = XDC_FAUX;
	vl_Nb_Ligne_Lue = 0;
			
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name15, 
					CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: asql_ctparam_date_input(Horodate) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG15: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG15 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG15 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG15 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
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
	XZST_03EcritureTrace( XZSTC_INTERFACE,"XZAG15_Purge_Evts_Base_Exploitation : Debut d execution de XZAG10" );
	for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	sleep(0.1);
		/*A
		** Initialisation de la commande				
		*/
	
		if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
		{
		 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: asql_cmd_alloc() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		/*A
		** Envoie de la commande RPC pour notre stored procedure.
		*/
		
		if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name10, 
						CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: ct_command() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
	
		/*A 
		** Initialisation de la structure pour chacun des parametres d I/O	
		** A faire passer a la rpc.
		*/
	
	        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)vl_Tab_Ligne[i][0].pt_value,"@va_NumEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)vl_Tab_Ligne[i][1].pt_value,"@va_CleEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		vl_Tab_donnees2[0].pt_value = (CS_BIT *)&vl_Suppression;

	        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)&vl_Suppression,"@va_Resultat_out"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		/*A
		** Envoie de la commande au Sql server
		*/
		if (ct_send(pl_cmd) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: ct_send() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
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
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG10: ROW RESULTS");
							break;
	
						case  CS_PARAM_RESULT:
							vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG10 : PARAMETER RESULTS \n");
							break;
	
						case  CS_STATUS_RESULT:
							vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG10 : CS_STATUS_RESULT = %ld", vl_retcode);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG10 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
							
							if (vl_SPstatus != XDC_OK)
							{
							   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne 1");
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
							XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: ct_res_info(msgtype) a echoue");
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: ct_results retourne CS_CMD_FAIL.");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
	
					default:
						/*
						** Il s est passe quelque chose d inattendu.
						*/
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG10: ct_results retourne un resultat de type inattendu");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
					}
		}
		
		if (vl_Suppression == XDC_VRAI)
			*va_Suppression_out = XDC_VRAI;

 		asql_clean_cmd (pl_connection,pl_cmd);
	}
 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"XZAG15_Purge_Evts_Base_Exploitation XZAG10 retourne %d", XDC_OK);

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par
	** la SP XZAG15
	*/
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	if (vl_retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG15_Purge_Evts_Base_Exploitation retourne %d", XDC_OK);

 	return(XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_Purge_Base_Exploitation : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG15SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG_Purge_Base_Exploitation (XDY_Horodate	va_Horodate_in, XDY_Entier va_TypeMachine_in )

/*
*
* PARAMETRES EN ENTREE :
*
*   va_Horodate_in est l horodate au dela de la quelle on doit effectuer
*   la purge.
*   va_TypeMachine_in est le type de pc				
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
*  On fait appel a XZAG;16, XZAG;17, XZAG;18, XZAG;19 , XZAG;20, XZAG;21, XZAG;22
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag15.c,v 1.18 2020/01/21 16:52:08 gesconf Exp $ $Revision: 1.18 $   Date : $Date: 2020/01/21 16:52:08 $: XZAG_Purge_Base_Exploitation";
	
	CS_CHAR 	*	pl_rpc_name ;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int			vl_Status,vl_code_retour;
	XDY_Booleen		vl_Suppression;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG_Purge_Base_Exploitation : Debut d execution" );

	vl_Suppression = XDC_FAUX;
	vl_code_retour = XDC_OK;
	
	/*A
	** Purge des evenements
	*/
	vl_Status = XZAG15_Purge_Evts_Base_Exploitation(va_Horodate_in, &vl_Suppression);
	while (vl_Suppression == XDC_VRAI && vl_Status == XDC_OK)
	{
		sleep(120); /*attente de 2mn entre 2 passes*/
		vl_Status = XZAG15_Purge_Evts_Base_Exploitation(va_Horodate_in, &vl_Suppression);
	}
	if ( vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG: XZAG15_Purge_Evts_Base_Exploitation a echoue");
		vl_code_retour = XDC_NOK;
	}
			
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG15: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Base_Exploitation retourne %d", vl_retcode);
		return (XDC_NOK);
	}
	

	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	if (va_TypeMachine_in != XZSCC_TYPEM_PCS)
	{
		/*A
		** purge des plans d'action
		*/
		if ( XZAG_Purge_Morceau(XZAG16C_RPC_NAME, XZAG16C_NB_PARM_RETURN, "XZAG16", &vl_Horodate) == XDC_NOK )
			vl_code_retour = XDC_NOK;

		sleep(120); /*attente de 2mn entre 2 passes*/
		/*A
		** purge des appels fax
		*/
		if ( XZAG_Purge_Morceau(XZAG19C_RPC_NAME, XZAG19C_NB_PARM_RETURN, "XZAG19", &vl_Horodate) == XDC_NOK )
			vl_code_retour = XDC_NOK;
		sleep(120); /*attente de 2mn entre 2 passes*/
	}

	/*A
	** purge des alertes
	*/
	if ( XZAG_Purge_Morceau(XZAG17C_RPC_NAME, XZAG17C_NB_PARM_RETURN, "XZAG17", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		sleep(300); /*attente de 5mn entre 2 passes*/
	/*A
	** purge des etats eqts
	*/
	if ( XZAG_Purge_Morceau(XZAG18C_RPC_NAME, XZAG18C_NB_PARM_RETURN, "XZAG18", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		/*sleep(300);*/ /*attente de 5mn entre 2 passes*/
	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	/*A
	** purge des mesures trafic
	*/
	if ( XZAG_Purge_Morceau(XZAG20C_RPC_NAME, XZAG20C_NB_PARM_RETURN, "XZAG20", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		sleep(300); /*attente de 5mn entre 2 passes*/
	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	/*A
	** purge des mesures niveau
	*/
	if ( XZAG_Purge_Morceau(XZAG21C_RPC_NAME, XZAG21C_NB_PARM_RETURN, "XZAG21", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		sleep(300); /*attente de 5mn entre 2 passes*/
	sleep(900); /*attente de 15mn avant purge morceaux de la base */
	/*A
	** purge des mesures poids
	*/
	if ( XZAG_Purge_Morceau(XZAG22C_RPC_NAME, XZAG22C_NB_PARM_RETURN, "XZAG22", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		sleep(120); /*attente de 5mn entre 2 passes*/
	/*A
	** purge des mesures pesage KCS
	*/
	if ( XZAG_Purge_Morceau(XZAG51C_RPC_NAME, XZAG_SANS_PARM_RETURN, "XZAG251", &vl_Horodate) == XDC_NOK )
		vl_code_retour = XDC_NOK;

		sleep(120); /*attente de 2mn entre 2 passes*/
	if (va_TypeMachine_in != XZSCC_TYPEM_PCS)
	{

		/*A
		** purge des tdp (mesure + frn)
		*/
		if ( XZAG_Purge_Morceau(XZAG54C_RPC_NAME, XZAG_SANS_PARM_RETURN, "XZAG54", &vl_Horodate) == XDC_NOK )
			vl_code_retour = XDC_NOK;
	}
	/*A
	** Message en sortie de la fonction
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG_Purge_Base_Exploitation retourne %d", XDC_OK);

 	return(vl_code_retour);

}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_Purge_Morceau : Purger avec la procedure donnee en entree
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZAG_Purge_Morceau(	CS_CHAR 	*va_rpc_name_in, 
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
	static char *version = "$Id: xzag15.c,v 1.18 2020/01/21 16:52:08 gesconf Exp $ $Revision: 1.18 $   Date : $Date: 2020/01/21 16:52:08 $: XZAG_Purge_Base_Exploitation";
	
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
				"IN : XZAG_Purge_Morceau : Debut d execution pour %s", 
				va_titre_commentaire_in );

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_cmd_alloc() a echoue", 
						va_titre_commentaire_in);
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG_Purge_Morceau retourne %d", vl_retcode);
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
				"OUT : XZAG_Purge_Morceau retourne %d", vl_retcode);
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
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Morceau retourne %d", 
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
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Morceau retourne %d", 
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
								"XZAG_Purge_Morceau: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , va_Nb_Param_Return_in, vl_Tab_donnees1);
						XZST_03EcritureTrace(XZSTC_INFO,
							"XZAG_Purge_Morceau : PARAMETER RESULTS\n");
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
								"OUT : XZAG_Purge_Morceau retourne 1");
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
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Morceau retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Morceau retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu", va_titre_commentaire_in);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Purge_Morceau retourne %d", vl_retcode);
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
			"OUT : XZAG_Purge_Morceau retourne %d", XDC_OK);

 	return(XDC_OK);
}
