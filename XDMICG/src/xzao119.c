/*E*/
/*  Fichier : 	$Id: xzao119.c,v 1.2 1994/12/15 14:55:16 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/15 14:55:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao119.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic        23 Sep 1994     : Creation
* volcic	28 Oct 1994	: Modification entete (v1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao119.c,v 1.2 1994/12/15 14:55:16 volcic Exp $ : xzao119" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO119SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO119_Ajouter_Troncon (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfTroncon	va_Troncon_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomTroncon_in
* XDY_Nom		va_NomAutoroute_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Troncon en base
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
* Insertion ou modification dans la table RES_TRC (VUE_TRONCON)
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao119.c,v 1.2 1994/12/15 14:55:16 volcic Exp $ : XZAO119_Ajouter_Troncon" ;

	CS_CHAR *rpc_name = XZAOC_XZAO119_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO119_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Nom		vl_NomTroncon;		
	XDY_NomAuto	vl_NomAutoroute;
	XDY_PR		vl_PRdebut;
	XDY_PR		vl_PRfin;
	XDY_Booleen	vl_Creation;
	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomTroncon, va_Troncon_in.NomTroncon );
	strcpy( vl_NomAutoroute, va_Troncon_in.NomAutoroute );
	vl_PRdebut	= va_Troncon_in.PRdebut;
	vl_PRfin	= va_Troncon_in.PRfin;
	vl_Creation	= va_Troncon_in.Creation;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO119_Ajouter_Troncon : Troncon = %s\tAutoroute = %s\tPRdebut = %d\tPRfin = %d\tCreation = %d\n",
			va_Troncon_in.NomTroncon,
			va_Troncon_in.NomAutoroute,
			va_Troncon_in.PRdebut,
			va_Troncon_in.PRfin,
			va_Troncon_in.Creation );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomTroncon,"@va_NomTroncon_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_char_input(NomTroncon) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomAutoroute,"@va_NomAutoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_char_input(NomAutoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRdebut,"@va_PRdebut_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_int_input(PRdebut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRfin,"@va_PRfin_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_int_input(PRfin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_bit_input(Creation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao119: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Noeud retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao119 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	     					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao119 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
					free(Tab_donnees2);
	   				return(XDC_NOK);
				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao119: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
					free(Tab_donnees2);
	   				return(XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO119_Ajouter_Troncon retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




