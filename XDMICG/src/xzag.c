/*X  Fichier : @(#)xzag.c	$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $      Release :  $Revision: 1.16 $        Date : $Date: 2012/05/29 10:12:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER xzag.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  	XZAG02_Connexion_Base :Prend en argument le nom du site, 
*  en deduit le nom du sql Serveur
*  sur le quel on doit se connecter en tant que l utilisateur migrazur,
*  effectue un use PRC.
***************************
*  	XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase.
***************************
*	XZAG03_Deconnexion_Base : Arret de la connexion a la base 
***************************
*	XZAG04_Arret_Base : recoit en argument le nom du site
*  et lance un script shell permettant l arret des bases en local.
***************************
*  	XZAG_execute_cmd : Execute la commande sur la pl_connection ouverte
***************************

------------------------------------------------------
* HISTORIQUE :
*
* mercier	16 Aug 1994	: Creation
***********************
* mercier	06 Oct 1994	: Modification	                V 1.7
*	Correction des codes de retour de XZAG_connexion au format
*	migrazur.
***********************
* MERCIER	14 Oct 1994	: Modification de XZAG_connexion V 1.8
*				Rajoue de cs_setnull .
***********************
* MERCIER	02 Dec 1994	: Modification de XZAG_connexion V 1.9
*				Rajoue de charset a iso1 lang a french.
***********************
* MERCIER	10 Jan 1995	: Ajout de xzag01			V 1.10
* TORREGROSSA	12 Sep 1995	: Modif de xzag01			V 1.11
*                                 et ajout de xzag03_Recuperer_Evt_Mois, 
*                                 xzag_Executer_Proc_Transfert
* Niepceron	09 Oct 1996	: Ajout de XZAG50 (dem/1227) v1.11
* Niepceron	10 Dec 1997	: ajout du script pour XZAG04 v1.12
* JMG		29/05/12 : V15 1.13
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag.h"


#define	XZAGC_ARRET_BASE	"nohup /produits/migrazur/appliSD/exec/arret_base.sh %s 2>>/dev/null >>/dev/null &"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */


static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG" ;

/* declaration de fonctions internes */

int XZAG02_Connexion_Base (char * );
CS_RETCODE XZAG02_context_init(CS_CONTEXT **);
int XZAG03_Recuperer_Evt_Mois (XDY_Mot *, XDY_Mot *);
int XZAG_Executer_Proc_Transfert (CS_CHAR *, XDY_Octet , char *, XDY_Mot *, XDY_Mot *);

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG03_Recuperer_Evts_Mois : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAG03SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG03_Recuperer_Evts_Mois (XDY_Mot	*va_Mois_in, XDY_Mot       *va_Annee_in)

/*
*
* PARAMETRES EN ENTREE :
*
*   va_Mois_in contient le mois de l annee va_Annee_in a transferer dans la
*   base sauvegarde/restauration.
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
*  On fait appel a XZAG;03 et XZAG;12
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG03_Recuperer_Evts_Mois" ;
	
	CS_CHAR 	*	pl_rpc_name03 = XZAG03C_RPC_NAME;
	CS_CHAR 	*	pl_rpc_name12 = XZAG12C_RPC_NAME;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_RETCODE      	vl_retcode;
	CS_INT          	vl_res_type;
	CS_SMALLINT     	vl_msg_id;
	int 			vl_num_Tab_donnees1 = XZAG03C_NB_PARM_RETURN;
	int 			vl_num_Tab_donnees2 = XZAG12C_NB_PARM_RETURN;
	int			vl_Nb_Ligne_Lue;
	int			vl_Nb_Col;
	int			i;
	tg_row_result		vl_Tab_Ligne;
	
	XDY_Entier		vl_SPstatus;
	XDY_Booleen		vl_Suppression;
	
	CS_DATETIME		vl_Horodate;	/* Va contenir la conversion de l horodate*/
						/* recue en parametre au format SYBASE	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG03_Recuperer_Evt_Mois : Debut d execution" );

	vl_Nb_Ligne_Lue = 0;
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name03, 
					CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)va_Mois_in,
							"@va_Mois_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,
					"XZAG03: asql_ctparam_smallint_input(Mois) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", 
					vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)va_Annee_in,
							"@va_Annee_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,
					"XZAG03: asql_ctparam_smallint_input(Annee) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", 
					vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG03: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG03 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG03 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG03 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAG03: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
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
		 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_cmd_alloc() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		/*A
		** Envoie de la commande RPC pour notre stored procedure.
		*/
		
		if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name12, 
						CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: ct_command() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, 
					"OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
			asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
	
		/*A 
		** Initialisation de la structure pour chacun des parametres d I/O	
		** A faire passer a la rpc.
		*/
	
	        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)vl_Tab_Ligne[i][0].pt_value,"@va_NumEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)vl_Tab_Ligne[i][1].pt_value,"@va_CleEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_ctparam_int_input(Numero) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)vl_Tab_Ligne[i][4].pt_value,"@va_ClasseEvt_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_ctparam_tinyint_input(Classe) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)vl_Tab_Ligne[i][2].pt_value,"@va_NumCause_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_ctparam_int_input(NumCause) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
	        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)vl_Tab_Ligne[i][3].pt_value,"@va_CleCause_in"))!= CS_SUCCEED)
		
		{
	    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: asql_ctparam_tinyint_input(CleCause) a echoue");
		   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
		   	asql_clean_cmd (pl_connection,pl_cmd);
			return (XDC_NOK);
		}
		
		
		/*A
		** Envoie de la commande au Sql server
		*/
		if (ct_send(pl_cmd) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: ct_send() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
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
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG12: ROW RESULTS");
							break;
	
						case  CS_PARAM_RESULT:
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG12 : PARAMETER RESULTS \n");
							fprintf(stdout, "\nPARAMETER RESULTS\n");
							break;
	
						case  CS_STATUS_RESULT:
							vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG12 : CS_STATUS_RESULT = %ld", vl_retcode);
							XZST_03EcritureTrace(XZSTC_INFO,"XZAG12 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
							
							if (vl_SPstatus != XDC_OK)
							{
							   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne 1");
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
							XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: ct_res_info(msgtype) a echoue");
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: ct_results retourne CS_CMD_FAIL.");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
	
					default:
						/*
						** Il s est passe quelque chose d inattendu.
						*/
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAG12: ct_results retourne un resultat de type inattendu");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG03_Recuperer_Evts_Mois retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
						return ( XDC_NOK );
					}
		}
		
 		asql_clean_cmd (pl_connection,pl_cmd);
	}

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par
	** la SP XZAG03
	*/
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	if (vl_retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG03_Recuperer_Evts_Mois retourne %d", XDC_OK);

 	return(XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG01_Transfert_Mois : est appele par un callback de tacli
*  sur reception de message XDM_AG_PURGE sur le dg XDG_AG_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG01_Transfert_Mois (XDY_Mot	va_Mois_in, XDY_Mot       va_Annee_in)

/*
*
* PARAMETRES EN ENTREE :
*
*	va_Mois_in contient le mois de l annee va_Annee_in a transferer dans la
* base sauvegarde/restauration.
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
*  On fait appel a XZAG;02, XZAG;07, XZAG;08, XZAG;09 , XZAG;40, XZAG;03, XZAG;04
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG01_Transfert_Mois" ;
	
	CS_CHAR 	*	pl_rpc_name ;
	
	CS_CONNECTION   *	pl_connection=XZAGV_Connexion;
	CS_COMMAND	*	pl_cmd;
	
	CS_SMALLINT     	vl_msg_id;
	int			vl_Status;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG_Transfert_Mois : Debut d execution" );

	/*A
	** vider la base SVG
	*/
	XZAG_Executer_Proc_Transfert(XZAG02C_RPC_NAME, XZAG02C_NB_PARM_RETURN, "XZAG02", NULL, NULL);

	/*A
	** Transferer les evenements du mois
	*/
	vl_Status = XZAG03_Recuperer_Evts_Mois(&va_Mois_in, &va_Annee_in);
	if ( vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG: XZAG03_Recuperer_Evts_Mois a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Transfert_Mois retourne %d", vl_Status);
		return (XDC_NOK);
	}
			
	/*A
	** transfert mesures trafic
	*/
	XZAG_Executer_Proc_Transfert(XZAG04C_RPC_NAME, XZAG04C_NB_PARM_RETURN, "XZAG04",  &va_Mois_in, &va_Annee_in);

	/*A
	** transfert mesures pesage
	*/
	XZAG_Executer_Proc_Transfert(XZAG50C_RPC_NAME, XZAG_SANS_PARM_RETURN, "XZAG50",  &va_Mois_in, &va_Annee_in);

	/*A
	** transfert mesures poids
	*/
	XZAG_Executer_Proc_Transfert(XZAG07C_RPC_NAME, XZAG07C_NB_PARM_RETURN, "XZAG07", &va_Mois_in, &va_Annee_in);

	/*A
	** transfert mesures niveau
	*/
	XZAG_Executer_Proc_Transfert(XZAG08C_RPC_NAME, XZAG08C_NB_PARM_RETURN, "XZAG08", &va_Mois_in, &va_Annee_in);

	/*A
	** transfert mesures disponibilites
	*/
	XZAG_Executer_Proc_Transfert(XZAG09C_RPC_NAME, XZAG09C_NB_PARM_RETURN, "XZAG09", &va_Mois_in, &va_Annee_in);

	/*A
	** transfert appels horaires
	*/
	XZAG_Executer_Proc_Transfert(XZAG40C_RPC_NAME, XZAG40C_NB_PARM_RETURN, "XZAG40", &va_Mois_in, &va_Annee_in);


	/*A
	** Message en sortie de la fonction
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, 
			"OUT : XZAG_Transfert_Mois retourne %d", XDC_OK);

 	return(XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_Executer_Proc_Transfert : executer la procedure donnee en entree
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZAG_Executer_Proc_Transfert(	CS_CHAR 	*va_rpc_name_in, 
					XDY_Octet	va_Nb_Param_Return_in,
					char		*va_titre_commentaire_in,
					XDY_Mot		*va_Mois_in, 
					XDY_Mot       	*va_Annee_in)
/*
*
* PARAMETRES EN ENTREE :
*
*   va_rpc_name_in          : nom de la procedure a executer
*   va_Nb_Param_Return_in   : nombre de parametres de sortie de la procedure a executer
*   va_Mois_in		    : mois 
*   va_Annee_in		    : annee
*   va_titre_commentaire_in : titre du commentaire pour les messages
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
	static char *version = "";
	
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
				"IN : XZAG_Executer_Proc_Transfert : Debut d execution pour %s", 
				va_titre_commentaire_in );

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_cmd_alloc() a echoue", 
						va_titre_commentaire_in);
		XZST_03EcritureTrace(XZSTC_INTERFACE, 
				"OUT : XZAG_Executer_Proc_Transfert retourne %d", vl_retcode);
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
				"OUT : XZAG_Executer_Proc_Transfert retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((va_Mois_in != NULL) && ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)va_Mois_in,
							"@va_Mois_in"))!= CS_SUCCEED))
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,
					"%s: asql_ctparam_smallint_input(Mois) a echoue", 
					va_titre_commentaire_in);
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", 
					vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

        if ((va_Annee_in != NULL) && ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)va_Annee_in,
							"@va_Annee_in"))!= CS_SUCCEED))
	
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,
					"%s: asql_ctparam_smallint_input(Annee) a echoue", 
					va_titre_commentaire_in);
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", 
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
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", 
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
								"XZAG_Executer_Proc_Transfert: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , va_Nb_Param_Return_in, vl_Tab_donnees1);
						XZST_03EcritureTrace(XZSTC_INFO,
							"XZAG_Executer_Proc_Transfert : PARAMETER RESULTS\n");
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
								"OUT : XZAG_Executer_Proc_Transfert retourne 1");
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
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu", va_titre_commentaire_in);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG_Executer_Proc_Transfert retourne %d", vl_retcode);
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
			"OUT : XZAG_Executer_Proc_Transfert retourne %d", XDC_OK);

 	return(XDC_OK);
}

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG02_Connexion_Base :Prend en argument le nom du site, 
*  en deduit le nom du sql Serveur
*  sur le quel on doit se connecter en tant que l utilisateur migrazur,
*  effectue un use PRC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG02_Connexion_Base( char * va_Nom_Site)

/*
* ARGUMENTS EN ENTREE :
*   < va_Nom_Site contient le nom du site sur lequel on cherche a se connecter >
*
*
* ARGUMENTS EN SORTIE :
*   < Aucun >
*
*
* CODE RETOUR : 
*   < XDC_OK: Connexion reussi au SQL_serveur de la machine passee en argument >
*   < XDC_NOK: Connexion a ≈chou≈ >
*
* CONDITION D'UTILISATION
*    Ouverture du fichier trace doit etre effectue 
*    Etre sur une machine possedant open client 
*          (serveur de donnees ou poste de developpement)
*
*
* FONCTION 
*    Ouvre une connexion a la base PRC sur le serveur de donnees du Site
*    Initialise la variable globale au Module XZAGV_Connexion a la connexion 
*    Sybase si tout s est bien passe sinon elle est mise a NULL.
*
------------------------------------------------------*/
{
	static char   	*version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG02_Connexion_Base";
	CS_RETCODE      vl_retcode;
	CS_CONTEXT    	*pl_context;
	CS_CONNECTION 	*pl_connection;
	CS_CHAR       	*username=XZAGC_USER;
	CS_CHAR       	*password=XZAGC_PASSWD;
	CS_CHAR       	vl_nom_sql_srv[80];
	CS_INT		vl_nom_sql_srv_len;
	CS_CHAR         *vl_cmdbuf; 
	CS_COMMAND      *pl_cmd;
	CS_DATAFMT      vl_tmpfmt;
	/*CS_LOCALE	*pl_local;*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : Debut de XZAG02_Connexion_Base \n");
	
	/*A
	** Reconstitution du nom du SQL serveur a partir du nom du site recu
	** en argument.
	*/
	strcpy(vl_nom_sql_srv,XZAGC_PREFIXE_SQLSRVNAME);
	strcat(vl_nom_sql_srv,va_Nom_Site);
	
	XZST_03EcritureTrace(XZSTC_MESSAGE,"Nom Sql_Srv identifie a : %s \n",vl_nom_sql_srv);
	
	/*A
	** allocation de la structure de Contexte et initialisation Client Lib 
	*/
	/*B
	** Positionnement du charset a ISO_1 ainsi que du lang a french effectue dans XZAG02_context_init
	*/
	/*B
	**  Definition de la substitution d une chaine de caractere null pas un espace
	**  : effectue dans XZAG02_context_init
	*/
	
	XZAG02_context_init(&pl_context);

	/*A
	**  Definition de la substitution d une chaine de caractere null pas un espace
	**  : effectue dans XZAG02_context_init
	*/
	/*memset(&vl_tmpfmt, 0, sizeof(vl_tmpfmt));
        vl_tmpfmt.datatype = CS_VARCHAR_TYPE;
	if((vl_retcode = cs_setnull(pl_context, &vl_tmpfmt, "", strlen("")))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG02_Connexion_Base : cs_setnull a echoue");
	}
	*/
	
	/*A
	** Positionnement du charset a ISO_1 ainsi que du lang a french effectue dans XZAG02_context_init
	*/
	
	
	
 	/*A
	** allocation de la structure de connexion.
	*/
	vl_retcode = ct_con_alloc(pl_context, &pl_connection);
	
	if (vl_retcode != CS_SUCCEED)
	 {
		XZST_03EcritureTrace(XZSTC_FATAL,"ct_con_alloc a echoue pour allouer la structure contexte : code d erreur Sybase = %d\n",vl_retcode);
		printf("ct_con_alloc a echoue\n");
	 	return (XDC_NOK);
	 }

	/*     
	** Si l utilisateur est defini, on met a jour la USERNAME property.
	*/
	if (vl_retcode == CS_SUCCEED && username != NULL)
	{
		if ((vl_retcode = ct_con_props(pl_connection, CS_SET, CS_USERNAME,
				username, CS_NULLTERM, NULL)) != CS_SUCCEED)
		{
		 	printf("Definition de %s incorrect pour se connecter a %s\n",username,vl_nom_sql_srv);
			XZST_03EcritureTrace(XZSTC_FATAL,"Definition de %s incorrect pour se connecter a %s\n Code Sybase =%d\n",username,vl_nom_sql_srv,vl_retcode);
			return(XDC_NOK);
		}
	}
 	if (vl_retcode == CS_SUCCEED && password != NULL)
	{
	 	if ((vl_retcode = ct_con_props(pl_connection, CS_SET, CS_PASSWORD,
		 password, CS_NULLTERM, NULL)) != CS_SUCCEED)
	 	{
		 	printf("Definition du passwd dans le context de connexion incorrect\n");
			XZST_03EcritureTrace(XZSTC_FATAL,"Definition du passwd dans le context de connexion incorrect");
			return(XDC_NOK);
		}

	}
	/*
	** Si vl_nom_sql_srv est null alors vl_nom_sql_srv_len=0 sinon =CS_NULLTERM
	*/
	vl_nom_sql_srv_len = (vl_nom_sql_srv == NULL) ? 0 : CS_NULLTERM;

	XZST_03EcritureTrace(XZSTC_MESSAGE,"essai de Connexion a %s\n",vl_nom_sql_srv);
	vl_retcode = ct_connect(pl_connection, vl_nom_sql_srv, vl_nom_sql_srv_len);

	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"ct_connect a echoue :\n Code Sybase = %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_MESSAGE,"Liberation de Connexion a %s\n",vl_nom_sql_srv);
		ct_con_drop(pl_connection);
		pl_connection = NULL;
		return(XDC_NOK);
	}
	XZST_03EcritureTrace(XZSTC_MESSAGE,"Connexion a %s reussi\n",vl_nom_sql_srv);
	XZST_03EcritureTrace(XZSTC_SGBD,"Essai d utilisation de la base PRC\n");

	vl_cmdbuf = (CS_CHAR *) malloc(sizeof(XZAGC_USE_PRC_BASE));
	if (vl_cmdbuf == (CS_CHAR *)NULL)
	{
		 XZST_03EcritureTrace(XZSTC_FATAL,"Creation de la commande d utilisation de la base PRC a echoue: malloc a ≈chou≈\n",NULL);
		 return(XDC_NOK);
	}

	strcpy(vl_cmdbuf,XZAGC_USE_PRC_BASE);

	if ((vl_retcode = XZAG_execute_cmd(pl_connection, vl_cmdbuf)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"Execution de la commande d utilisation de la base PRC a echoue  : \n XZAG_execute_cmd(%s) a ≈chou≈ avec code Sybase %d",vl_cmdbuf,vl_retcode);
		return(XDC_NOK);
	}
	free(vl_cmdbuf);
	/*
	** Initialisation de la variable globale XZAGV_Connexion
	** apres verification que tout ce soit bien passe
	** sinon XZAGV_Connexion reste egale a NULL.
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAG02_Connexion_Base OK : initialisation de XZAGV_Connexion \n");

	XZAGV_Connexion = pl_connection;	
	XZAGV_Context   = pl_context;

	return(XDC_OK);
}



/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE XZAG02_context_init(CS_CONTEXT **pa_context)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   CS_SUCEED : code de ertour OK des procedures sybase encapsulees
*   CS_FAIL ou autre en cas d erreur.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Initialise la structure de contexte de l open client >
*
------------------------------------------------------*/
{	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG02_context_init";

	CS_RETCODE      vl_retcode;
	CS_INT          vl_netio_type = CS_SYNC_IO;
	CS_DATAFMT      vl_tmpfmt;
	CS_LOCALE	*pl_locale;
	char		vl_chaine_locale[200];
	
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG02_context_init: debut");
	
	/*
	** Get a context handle to use.
	*/
#ifdef _HPUX_SOURCE
	vl_retcode = cs_ctx_alloc(CS_VERSION_100, pa_context);
#else
	vl_retcode = cs_ctx_alloc(CS_VERSION_150, pa_context);
#endif
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: cs_ctx_alloc() a ≈chou≈");
		return vl_retcode;
	}

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG02_context_init: avant ct_init");
	/*A
	** Initialise l Open Client.
	*/
	
#ifdef _HPUX_SOURCE
	vl_retcode = ct_init(*pa_context, CS_VERSION_100);
#else
	vl_retcode = ct_init(*pa_context, CS_VERSION_150);
#endif
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG02_context_init: apres ct_init");
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: ct_init() a ≈chou≈");
		cs_ctx_drop(*pa_context);
		*pa_context = NULL;
		return (vl_retcode);

	}
	/*A
	** Installe les handlers de messages du client et du server.
	*/
	
	else
	{
		vl_retcode = ct_callback(*pa_context, NULL, CS_SET, CS_CLIENTMSG_CB,(CS_VOID *)asql_clientmsg_cb);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: ct_callback(asql_clientmsg_cb) a ≈chou≈");
		}
	
	
		if (vl_retcode == CS_SUCCEED)
        	{
                	vl_retcode = ct_callback(*pa_context, NULL, CS_SET, CS_SERVERMSG_CB,
                                (CS_VOID *)asql_servermsg_cb);
               		 if (vl_retcode != CS_SUCCEED)
                	{
                        	XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: ct_callback(asql_servermsg_cb) a ≈chou≈");
                	}
        	}
        }
        
       	/* si on met en place l asynchronisme : 
       	vl_retcode  = ct_callback(Ex_context, NULL, CS_SET, CS_COMPLETION_CB,(CS_VOID *)asql_completion_cb);
        
        if (vl_retcode != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: ct_callback a ≈chou≈");
        }*/

	/*
	** This is an synchronous example so set the input/output type
	** to synchronous (This is the default setting, but show an
	** example anyway).
	*/
	if (vl_retcode == CS_SUCCEED)
	{
		vl_retcode = ct_config(*pa_context, CS_SET, CS_NETIO, &vl_netio_type,
				CS_UNUSED, NULL);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init: ct_config(netio) a ≈chou≈");
		}
	}

	else
	{
		ct_exit(*pa_context, CS_FORCE_EXIT);
		cs_ctx_drop(*pa_context);
		*pa_context = NULL;
	}

	/*A
	**  Definition de la substitution d une chaine de caractere null pas un espace
	*/
	memset(&vl_tmpfmt, 0, sizeof(vl_tmpfmt));
        vl_tmpfmt.datatype = CS_VARCHAR_TYPE;
	if((vl_retcode = cs_setnull(*pa_context, &vl_tmpfmt, "", strlen("")))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_setnull a echoue en renvoyant %d",vl_retcode);
	}
	
	/*A
	** Positionnement du charset a ISO_1 ainsi que du langage a french
	*/
	if((vl_retcode = cs_loc_alloc (*pa_context,&pl_locale))== CS_SUCCEED)
	
	{
#ifdef _HPUX_SOURCE
		strcpy(vl_chaine_locale,"french.iso88591");
#else
		strcpy(vl_chaine_locale,"en");
#endif
		if((vl_retcode = cs_locale(*pa_context, CS_SET, pl_locale, CS_LC_ALL,vl_chaine_locale,CS_NULLTERM,NULL))==CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_INFO,"XZAG02_context_init : cs_local (CS_LC_ALL a french.iso88591, french, iso_1) a reussi");
			
#ifdef _HPUX_SOURCE
			if((vl_retcode = cs_locale(*pa_context, CS_SET, pl_locale, CS_SYB_LANG,"french",CS_NULLTERM,NULL))==CS_SUCCEED)
#else
			if((vl_retcode = cs_locale(*pa_context, CS_SET, pl_locale, CS_SYB_LANG,"us_english",CS_NULLTERM,NULL))==CS_SUCCEED)
#endif
			{
				XZST_03EcritureTrace(XZSTC_INFO,"XZAG02_context_init : cs_local (CS_SYB_LANG a french) a reussi");
				
				if((vl_retcode = cs_locale(*pa_context, CS_SET, pl_locale, CS_SYB_CHARSET,"iso_1",CS_NULLTERM,NULL))==CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_INFO,"XZAG02_context_init : cs_local (CS_SYB_CHARSET a iso_1) a reussi");
					
					
					/*
					if((vl_retcode = cs_locale(*pa_context, CS_SET, pl_locale, CS_SYB_SORTORDER,"binary",CS_NULLTERM,NULL))==CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG02_context_init : cs_local (CS_SYB_SORTORDER a binary) a reussi");
					
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init :  cs_local (CS_SYB_SORTORDER ) a echoue en renvoyant %d",vl_retcode);
					
					}
					
				
					*/
					
					if((vl_retcode = cs_config(*pa_context, CS_SET, CS_LOC_PROP, pl_locale, CS_UNUSED, NULL))== CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_INFO,"XZAG02_context_init : (cs_config de CS_LOC_PROP) a reussi");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init :  (cs_config de CS_LOC_PROP) a echoue en renvoyant %d",vl_retcode);
					}
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_local (CS_SYB_CHARSET a iso_1)  a echoue en renvoyant %d",vl_retcode);
				}
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_local (CS_SYB_LANG a french) a echoue en renvoyant %d",vl_retcode);
			}
		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_local (french.iso88591, french, iso_1) a echoue en renvoyant %d",vl_retcode);
		}
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_loc_alloc a echoue avec %d",vl_retcode);
	}
	/*A
	** Liberation de la structure CS_locale 
	** qui a permi de configurer le contexte (french iso_1)
	*/
	if((vl_retcode = cs_loc_drop (*pa_context, pl_locale)) == CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_INFO,"cs_loc_drop a reussi a droper la structure cs_locale");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG02_context_init : cs_loc_drop a echoue avec %d",vl_retcode);
	}
	return vl_retcode;

}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG_execute_cmd : Execute la commande sur la pl_connection ouverte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG_execute_cmd(CS_CONNECTION 	*pl_connection, 
			CS_CHAR 	*va_cmdbuf)

/*
* ARGUMENTS EN ENTREE :
*  pl_connection contient la structure renseignee par la fonction appelante
*  va_cmdbuf contient la chaine a executer sur la pl_connection
*
* CODE RETOUR : 
* On retourne vl_retcode qui contient les codes d erreurs de Sybase
*
* CONDITION D'UTILISATION
* Il faut que les deux parametres pl_connection et va_cmdbuf aient ete renseignes
* correctement par la fonction appelante.
*
* FONCTION 
* Execute la commande contenu dans va_cmdbuf sur la connexion pl_connection ouverte 
* au prealable par la fonction appelante
*
------------------------------------------------------*/
{	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG_execute_cmd";
	CS_RETCODE      vl_retcode;
	CS_INT          vl_restype;
        CS_COMMAND      *pl_cmd;
	CS_RETCODE      vl_query_code;

	/*A
	** Recupere l entete d une commande, charge la chaine dans
	** l entete et l envoie au serveur
	*/
	if ((vl_retcode = ct_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG_execute_cmd: ct_cmd_alloc(%s) a ≈chou≈",va_cmdbuf);
		return (vl_retcode);
	}
	if ((vl_retcode = ct_command(pl_cmd, CS_LANG_CMD, va_cmdbuf, CS_NULLTERM,
		CS_UNUSED)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG_execute_cmd: ct_command(%s) a ≈chou≈",va_cmdbuf);
		(void)ct_cmd_drop(pl_cmd);
		return vl_retcode;
	}

	if ((vl_retcode = ct_send(pl_cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG_execute_cmd: ct_send(%s) a ≈chou≈",va_cmdbuf);
		(void)ct_cmd_drop(pl_cmd);
		return vl_retcode;
	}

	/*
	** Examine le code de retour. Si une erreur apparait, 
	** Le code de retour renvoye a la fonction appelante est mis a FAIL.
	*/
        vl_query_code = CS_SUCCEED;
	while ((vl_retcode = ct_results(pl_cmd, &vl_restype)) == CS_SUCCEED)
	{
		switch((int)vl_restype)
		{
		    	case CS_CMD_SUCCEED:
			case CS_CMD_DONE:
				break;

			case CS_CMD_FAIL:
			     vl_query_code = CS_FAIL;
			     break;

			case CS_STATUS_RESULT:
			     vl_retcode = ct_cancel(NULL, pl_cmd, CS_CANCEL_CURRENT);
			     if (vl_retcode != CS_SUCCEED)
			     {
				  XZST_03EcritureTrace(XZSTC_FATAL,"XZAG_execute_cmd: ct_cancel(%s) a ≈chou≈",va_cmdbuf);
			  	  vl_query_code = CS_FAIL;
			     }
			     break;

			default:
			      /*
			      ** Traitement des codes de retour non attendu.
			      */
			      vl_query_code = CS_FAIL;
			      break;
		}/* Fin du Switch */
                if (vl_query_code == CS_FAIL)
		{
			/*
			** Fin du traitement du code de retour et sortie 
			** de la boucle    
			*/
			vl_retcode = ct_cancel(NULL, pl_cmd, CS_CANCEL_ALL);
			if (vl_retcode != CS_SUCCEED)
			{
        		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG_execute_cmd: ct_cancel(%s) a ≈chou≈",va_cmdbuf);
			}
			break;
		}
	}

    	/*
    	** On nettoie l entete de la commande utilisee
    	*/
    	if (vl_retcode == CS_END_RESULTS)
    	{
   		vl_retcode = ct_cmd_drop(pl_cmd);
		if (vl_retcode != CS_SUCCEED)
		{
			vl_query_code = CS_FAIL;
		}
	}
	else
	{
		(void)ct_cmd_drop(pl_cmd);
		vl_query_code = CS_FAIL;
	}

	return vl_query_code;
}



/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG03_Deconnexion_Base : Arret de la connexion a la base 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG03_Deconnexion_Base()

/*
* ARGUMENTS EN ENTREE :
*   < Aucun >
*
*
* ARGUMENTS EN SORTIE :
*   < Aucun >
*
*
* CODE RETOUR : 
*   XDC_OK si tout est OK
*   sinon XDC_NOK
*
* CONDITION D'UTILISATION
*   < On utilise la variable  XZAGV_Connexion contenant l ID de connexion 
*     a un sql serveur sur la base PRC>
*
*
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{	
	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG03_Deconnexion_Base";
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_RETCODE      vl_status=CS_SUCCEED;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_close_option;

	vl_close_option = (vl_status != CS_SUCCEED) ? CS_FORCE_CLOSE : CS_UNUSED;
	vl_retcode = ct_close(pl_connection, vl_close_option);
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG03_Deconnexion_Base: ct_close() a ≈chou≈");
		return (XDC_NOK);
	}
	vl_retcode = ct_con_drop(pl_connection);
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZAG03_Deconnexion_Base: ct_con_drop() a ≈chou≈");
		return (XDC_NOK);
	}
	return (XDC_OK);
}

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAG04_Arret_Base : Arret de la connexion a la base 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG04_Arret_Base(XDY_NomSite va_NomSite)

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite contient le nom du site sur lequel on veut arreter les bases.
*
*
* ARGUMENTS EN SORTIE :
*   < Aucun >
*
*
* CODE RETOUR : 
*   XDC_OK si tout est OK
*   sinon XDC_NOK
*
* CONDITION D'UTILISATION

*
* FONCTION 
*   < Lance un shell script por arreter les bases su site designe par va_NomSite   >
*
------------------------------------------------------*/
{	
	static char *version = "$Id: xzag.c,v 1.16 2012/05/29 10:12:52 pc2dpdy Exp $ : XZAG04_Arret_Base";
	static char vl_commande[256];

	sprintf( vl_commande,XZAGC_ARRET_BASE, va_NomSite);
	XZST_03EcritureTrace(XZSTC_WARNING,"commande envoyÈe: %",vl_commande);
	system( vl_commande );
	
	return (XDC_OK);
}

