/*E*/
/*  Fichier : 	$Id: xzao51.c,v 1.6 1999/02/26 11:31:07 gaborit Exp $  Release : $Revision: 1.6 $ Date : $Date: 1999/02/26 11:31:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao51.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.4)
* torregrossa  30 oct 1995     : Ajout du sens de l'autoroute (A57) (v1.5)
* niepceron    02 dec 1998     : Ajout de la colonne escota dem/1724 1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao51.c,v 1.6 1999/02/26 11:31:07 gaborit Exp $ : xzao51" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO51SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO51_Ajouter_Autoroute  ( XDY_Basedd	va_Basedd_in,
				XDY_NomAuto 	va_Autoroute_in,
				XDY_Mot 	va_Inversee_in,
				XDY_Octet	va_Escota_in,
				XDY_ResConf  	*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_Autoroute_in
* XDY_Mot		va_Inversee_in
* XDY_Octet		va_Escota_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf		va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajouter une autoroute en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* INTERFACE :
*
* Insertion dans la table RES_AUT (AUTOROUTE)
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzao51.c,v 1.6 1999/02/26 11:31:07 gaborit Exp $ : XZAO51_Ajouter_Autoroute" ;

	CS_CHAR *rpc_name = XZAOC_XZAO51_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO51_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	
	retcode = 0;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO51_Ajouter_Autoroute : Basedd = %s,Autoroute = %s , Inversee = %d", va_Basedd_in,va_Autoroute_in, va_Inversee_in );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
		free (Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free (Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_autoroute retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free (Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Autoroute_in,"@va_NomAutoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_autoroute retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free (Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)(&va_Inversee_in),"@va_Inversee_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_smallint_input(Inversee) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_autoroute retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free (Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)(&va_Escota_in),"@va_Escota_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(Escota) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_autoroute retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free (Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free (Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	        free (Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao51: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao51 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
		                        free (Tab_donnees2);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
		                                free (Tab_donnees2);
	   			 		return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao51 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
		                        free (Tab_donnees2);
	   			 	return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
		                        free (Tab_donnees2);
	   			 	return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO51_Ajouter_Autoroute retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free (Tab_donnees2);
	return(XDC_OK);
}




