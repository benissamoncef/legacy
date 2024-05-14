/*E*/
/* Fichier : $Id: xzac900.c,v 1.2 2018/08/31 18:28:31 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/08/31 18:28:31 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac900.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	26/02/18	creation CTRL_FEUX 1.1 DEM1284
* JMG	31/08/18	correction warning compil 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac900.h"

static char *version = "@(#)xzac900.c	1.1 1.1 12/05/06 : xzac900" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC900SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC900_Commande_CFE ( 	 XDY_Entier    		va_NumEvt_in,
                                 XDY_Octet     		va_CleEvt_in,
                                 XDY_Operateur 		va_Operateur_in,
                                 XDY_Eqt       		va_NumEqt_in,
                                 XDY_Horodate  		va_HeureLancement_in,
                                 XDY_Octet      	va_Site_in,
				 XDY_Octet		va_PlanDeFeux_in,
				 XDY_LibellePlanDeFeux	va_LibellePlanDeFeux_in,
                                 XDY_Octet     		va_Priorite_in,
                                 XDY_Entier    		va_DistanceEvt_in,
                                 XDY_Octet      	va_site_origine_in,
                                 XDY_Entier		*va_NumeroAction_out)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de CFE
* 
* Sequence d'appel
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* 
------------------------------------------------*/


{

	static char *version = "@(#)xzac900.c	1.1 1.1 12/05/06 :  XZAC900_Commande_CFE" ;

	CS_CHAR *rpc_name = XZACC_XZAC900_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC900_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZACC_XZAC900_NB_PARM_RETURN];
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	int		i = 0;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAC900_Commande_CFE: asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,
		"IN : XZAC900_Commande_CFE NumEvt: (%d)\n Cle(%d)\n  Operateur(%d)\n Site(%d)\n PlanDeFeux(%d)\n LibellePlanDeFeux(%s)\n Priorite(%d)\n Distance(%d)\n Site (%d)\n ",
				va_NumEvt_in,
				va_CleEvt_in,
				va_Operateur_in,
				va_Site_in,
				va_PlanDeFeux_in,
				va_LibellePlanDeFeux_in,
				va_Priorite_in,
				va_DistanceEvt_in,
				va_site_origine_in);
			

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_Operateur_in,"@va_Operateur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_tinyint_input(va_Site_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

     
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PlanDeFeux_in,"@va_PlanDeFeux_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_char_input(va_PlanDeFeux_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %s", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_LibellePlanDeFeux_in,"@va_LibellePlanDeFeux_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_char_input(va_LibellePlanDeFeux_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %s", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}		


	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Priorite_in,"@va_Priorite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_tinyint_input(va_Priorite_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_DistanceEvt_in,"@va_DistanceEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_int_input(va_DistanceEvt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
       

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_site_origine_in,"@va_site_origine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_tinyint_input(va_site_origine_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        

	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumeroAction_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumeroAction_out,"@va_NumeroAction_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: asql_ctparam_tinyint_input(va_NumeroAction_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac900 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzac900 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac900: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC900_Commande_CFE retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




