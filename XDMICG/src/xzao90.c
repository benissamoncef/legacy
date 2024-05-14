/*E*/
/*  Fichier : 	$Id: xzao90.c,v 1.2 1994/12/15 14:57:59 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/15 14:57:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao90.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       23 Sep 1994     : Creation
* volcic       27 Oct 1994     : Ajout retour status SP (v1.2)
* volcic       01 Nov 1994     : Modification entete (v1.3)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao90.c,v 1.2 1994/12/15 14:57:59 volcic Exp $ : xzao90" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO90SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO90_Ajouter_PS (	XDY_Basedd	va_Basedd_in,
			XZAOT_ConfPS	va_PS_in,
			XDY_ResConf  	*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomPS_in
* XDY_Nom		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Mot		GabaritSens1;
* XDY_Mot		GabaritSens2;
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PS en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans la table PASSAGE SUPERIEUR 
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao90.c,v 1.2 1994/12/15 14:57:59 volcic Exp $ : XZAO90_Ajouter_PS" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO90_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO90_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Nom		vl_NomPS;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_PR		vl_PR;
   	XDY_Mot		vl_GabaritSens1;
   	XDY_Mot		vl_GabaritSens2;
   	XDY_Booleen	vl_Creation;
	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomPS, va_PS_in.NomPS );
	strcpy( vl_Autoroute, va_PS_in.Autoroute );
	vl_PR		= va_PS_in.PR;
	vl_GabaritSens1	= va_PS_in.GabaritSens1;
	vl_GabaritSens2	= va_PS_in.GabaritSens2;
	vl_Creation	= va_PS_in.Creation;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO90_Ajouter_PS : PS = %s\tAutoroute = %s\tPR = %d\tGbS1 = %d\tGbS2 = %d\tCreation = %d\n",
			va_PS_in.NomPS,
			va_PS_in.Autoroute,
			va_PS_in.PR,
			va_PS_in.GabaritSens1,
			va_PS_in.GabaritSens2,
			va_PS_in.Creation );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomPS,"@va_NomPS_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_char_input(NomPS) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_GabaritSens1,"@va_GabaritSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_smallint_input(GabaritSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_GabaritSens2,"@va_GabaritSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_smallint_input(GabaritSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_bit_input(Creation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao90: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao90 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_ retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
						   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao90 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao90 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao90 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao90 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao90: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO90_Ajouter_PS retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




