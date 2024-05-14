								/*E*/
/* Fichier : $Id: xzao257.c,v 1.2 2018/11/07 18:38:43 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/11/07 18:38:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao257.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	22/12/11	: Creation (DEM/1016)
* JPL	07/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.2
* ABE	31/05/21 : PRV suur IP DEM-SAE283 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao257.h"

static char *version = "@(#)xzao257.c	1.3 12/15/94 : xzao257" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO257SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO257_Ajouter_PRV (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfPRV	 	va_PRV_in,
				XDY_ResConf  		*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomPRV_in
* XDY_Nom       va_Autoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonPRVPRV_in
* XDY_Rgs       va_RgsPRV_in
* XDY_Octet     va_Type_in
* XDY_NomSite   va_NomSite_in
* XDY_Mot       va_VitesseNominale_in
* char(50)      va_NomZone_in
* XDY_Booleen   va_Critique_in
* XDY_Booleen   va_BretelleEntree_in
* XDY_Booleen   va_RappelNominal_in
* XDY_Adresse	va_Adresse_IP_in
* XDY_Port	va_Port_in
* XDY_Booleen   va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PRV et un Picto en base
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
* Insertion ou modification dans les tables EQT_PRV (C_PRV), EQT_PCT (C_PICTO) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao257.c	1.3 12/15/94 : XZAO257_Ajouter_PRV" ;

	CS_CHAR *rpc_name = XZAOC_XZAO257_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO257_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_PortIP	vl_Port;
	vl_Port	  = va_PRV_in.Port;

	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO257_Ajouter_PRV : PRV = %s\tAutoroute = %s\tPR = %d\tSens = %d\tServeur = %s\tLiaisonPRV = %d\tRgsPRV = %s\tSiteGestion = %s\tType= %d\tVitesseNominale=%d\tNomZone=%s\tCritique=%d\tBretelleEntree=%d\tRappelNominal=%d\tAdresseIP=%s\tPort=%d\tBASEDD=%s\n",
			va_PRV_in.NomPRV,
			va_PRV_in.Autoroute,
			va_PRV_in.PR,
			va_PRV_in.Sens,
			va_PRV_in.NomServeur,
			va_PRV_in.LiaisonPRV,
			va_PRV_in.RgsPRV,
			va_PRV_in.NomSite,
			va_PRV_in.TypePRV,
			va_PRV_in.VitesseNominale,
			va_PRV_in.NomZone,
			va_PRV_in.Critique,
			va_PRV_in.BretelleEntree,
			va_PRV_in.RappelNominal,
			va_PRV_in.Adresse_IP,
			va_PRV_in.Port,
			va_Basedd_in);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.NomPRV,"@va_NomPRV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRV_in.PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PRV_in.Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PRV_in.LiaisonPRV,"@va_Liaison_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.RgsPRV,"@va_Rgs_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_PRV_in.TypePRV,"@va_Type_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_PRV_in.VitesseNominale,"@va_VitesseNominale_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_smallint_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_PRV_in.NomZone,"@va_NomZone_in"))!=CS_SUCCEED)	
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_PRV_in.Critique,"@va_Critique_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_bit_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_PRV_in.BretelleEntree,"@va_BretelleEntree_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_bit_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_PRV_in.RappelNominal,"@va_RappelNominal_in"))!=CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_bit_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_PRV_in.Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *) va_PRV_in.Adresse_IP,"@va_Adresse_IP_in"))!= CS_SUCCEED) 
	{ 
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_char_input() a echoue"); 
		   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257 retourne %d", retcode); 
		   asql_clean_cmd(connection, cmd); 
		   free(Tab_donnees2); 
		   return (XDC_NOK); 
	} 
	 
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port,"@va_Port_in"))!= CS_SUCCEED) 
	{ 				
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_int_input(int) a echoue"); 
		   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257 retourne %d", retcode); 
		   asql_clean_cmd(connection, cmd); 
		   free(Tab_donnees2); 
		   return (XDC_NOK); 
	} 

	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao257: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao257 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao257 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao257 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao257: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO257_Ajouter_PRV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




