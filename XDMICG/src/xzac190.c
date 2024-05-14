/*E*/
/*X  Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac190.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG 29/11/17	creation
* LCL	02/10/18	: RVA LOT25 DEM1303 1.1
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* LCL	13/07/21	: Fix Memory Leak DEM-SAE303 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac190.h"

static char *version = "%W% %I% %G% : xzac190" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC190SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC190_Envoyer_RVA ( XDY_Entier       va_NumEvt_in,
				XDY_Octet	va_CleEvt_in,
				XDY_Entier	va_NumeroAction_in,
				XDY_Octet	va_Site_in,
				XDY_Octet	va_Niveau_in,
				XDY_Horodate	va_HeureLancement_in,
				XDY_Horodate	va_HeureLancementFin_in,
				XDY_Mot	va_Operateur_in,
				XDY_Entier		*va_NumeroAction_out)

/*X*/
/*-----------------------------------------------------
* Service rendu-
* commande de BAF
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

	static char *version = "%W% %I% %G% : XZAC190_Envoyer_RVA" ;

	CS_CHAR *rpc_name = XZACC_XZAC190_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	int i ;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC190_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Octet vl_prop;
       	XDY_NomSite     pl_NomSite;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC190_Envoyer_RVA %d/%d/%d/%d/%d",
					va_NumEvt_in,va_CleEvt_in,va_Site_in,va_Niveau_in,va_Operateur_in);
			
        /*A
        ** Recuperation du site de la machine
        */
        if ( (retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzac190 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzac190 : Recuperation du Nom du site %s.",pl_NomSite);

			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_int_input(va_NumeroAction_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumeroAction_in,"@va_NumeroAction_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_int_input(va_NumeroAction_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Niveau_in,"@va_Niveau_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
       if ((retcode = asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
                return (XDC_NOK);
        }

	 if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_HeureLancement_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac92: asql_ctparam_date_input(Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }

       if (va_HeureLancementFin_in !=-1) {
       if ((retcode = asql_date_Ux2Sybase(va_HeureLancementFin_in,&vl_Horodate))!= XDC_OK)
        { 

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
                return (XDC_NOK);
        }

         if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_HeureLancementFin_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac92: asql_ctparam_date_input(Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	}

        if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_Operateur_in,"@va_Operateur_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }

	vl_prop = 0;
        if ((retcode= asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_prop,"@va_Proprietes_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
}

        i = 0;
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumeroAction_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumeroAction_out,"@va_NumeroAction_out")) != CS_SUCCEED)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae190: asql_ctparam_int_output(va_numero_out) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE190_Envoyer_RVA retourne %d", retcode);
	      asql_clean_cmd(connection, cmd);
	      free(Tab_donnees2);
	      return (XDC_NOK);
         }

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzac190: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	      					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac190 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	        				free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac190 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	        			free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac190: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	        			free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC190_Envoyer_RVA retourne %d", retcode);
	free(Tab_donnees2);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




