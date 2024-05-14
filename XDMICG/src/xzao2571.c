/* Fichier : $Id: xzao2571.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao2571.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABK	18/08/23	: Creation (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzao.h"
#include "xzao2571.h"
static char *version = "@(#)xzao2571.c	1.3 12/15/94 : xzao2571" ;

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO2571SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAO2571_Ajouter_PIC (	XDY_Basedd		va_Basedd_in,
							XZAOT_ConfPIC	va_PIC_in,
							XDY_ResConf  	*va_Resultat_out )
/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_NomEqt	NomPIC
* XDY_NomAuto	Autoroute
* XDY_PR		PR
* XDY_Sens		Sens
* XDY_NomEqt	NomServeur
* XDY_Octet		LiaisonPIC
* XDY_Rgs		RgsPIC
* XDY_NomSite	NomSite
* XDY_Booleen	Supprime
* XDY_AdresseIP	Adresse_IP
* XDY_Entier	Port
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PIC en base
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
* Insertion ou modification dans les tables EQT_PIC (C_PIC), et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
{
	/* Definition des variables locales */
	static char 		*version = "@(#)xzao2571.c	: XZAO2571_Ajouter_PIC" ;
	CS_CHAR 			*rpc_name = XZAOC_XZAO2571_RPC_NAME;
	CS_CONTEXT      	*context;
	CS_CONNECTION   	*connection=XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT      	datafmt;
	CS_RETCODE      	retcode;
	CS_INT          	res_type;
	CS_SMALLINT     	msg_id;
	int 				num_Tab_donnees2 = XZAOC_XZAO2571_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier			SPstatus;
	XDY_PortIP			vl_Port;
	vl_Port	  = va_PIC_in.Port;

	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO2571_Ajouter_PIC");
	
	XZST_03EcritureTrace( XZSTC_WARNING,"PIC = %s\tAutoroute = %s\tPR = %d\tSens = %d\tServeur = %s\tLiaisonPIC = %d\tRgsPIC = %s\tSiteGestion = %s\tSupprime = %s\tAdresseIP=%s \tPort=%d \tBASEDD=%s\n",
			va_PIC_in.NomPIC,
			va_PIC_in.Autoroute,
			va_PIC_in.PR,
			va_PIC_in.Sens,
			va_PIC_in.NomServeur,
			va_PIC_in.LiaisonPIC,
			va_PIC_in.RgsPIC,
			va_PIC_in.NomSite,
			va_PIC_in.Supprime,
			va_PIC_in.Adresse_IP,
			va_PIC_in.Port,
			va_Basedd_in);

	/* Allocation de la structure permettant la recuperation des parametres */
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PIC_in.NomPIC,"@va_NomPIC_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PIC_in.Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PIC_in.PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PIC_in.Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PIC_in.NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}	

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PIC_in.LiaisonPIC,"@va_Liaison_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PIC_in.RgsPIC,"@va_Rgs_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PIC_in.NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_PIC_in.Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *) va_PIC_in.Adresse_IP,"@va_Adresse_IP_in"))!= CS_SUCCEED) 
	{ 
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_char_input() a echoue"); 
		   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571 retourne %d", retcode); 
		   asql_clean_cmd(connection, cmd); 
		   free(Tab_donnees2); 
		   return (XDC_NOK); 
	} 
	 
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port,"@va_Port_in"))!= CS_SUCCEED) 
	{ 				
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_int_input(int) a echoue"); 
		   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571 retourne %d", retcode); 
		   asql_clean_cmd(connection, cmd); 
		   free(Tab_donnees2); 
		   return (XDC_NOK); 
	} 

	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao2571: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao2571 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao2571 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao2571 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao2571: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO2571_Ajouter_PIC retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




