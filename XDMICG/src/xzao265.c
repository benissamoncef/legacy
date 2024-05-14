/*E*/
/* Fichier : $Id: xzao265.c,v 1.2 2018/11/07 19:19:57 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/11/07 19:19:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao265.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	14/05/18	: Creation GABEB 1.1 DEM1283
* JPL	07/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao265.h"

static char *version = "@(#)xzao265.c	1.1 20/02/18 : xzao265" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO265SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO265_Ajouter_CAP (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfCAP	 	va_CAP_in,
				XDY_ResConf  		*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	 va_Basedd_in        		
* XDY_NomEqt     va_NomCAP_in
* XDY_NomAuto    va_Autoroute_in
* XDY_PR         va_PR_in
* XDY_Sens       va_Sens_in
* XDY_Entier     va_Type_in
* XDY_Rgs        va_Rgs_in
* XDY_AdresseIP  va_AdresseIP_in
* XDY_Port       va_PortMaitre_in
* XDY_Port       va_PortEsclave_in
* XDY_Octet      va_PointCaracteristique_in
* XDY_NomEqt	 va_NomServeur_in
* XDY_NomSite    va_NomSite_in
* XDY_Booleen    va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un controleur de feu CAP en base
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
* Insertion ou modification dans les tables EQT_CAP (C_CAP) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao265.c	1.1 20/02/18 : XZAO265_Ajouter_CAP" ;

	CS_CHAR *rpc_name = XZAOC_XZAO265_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO265_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	

	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO265_Ajouter_CAP : CAP = %s\tAutoroute = %s\tPR = %d\tSens = %d\tType = %d\tAdresseRgs = %s\tAdresseIP = %s\tPortMaitre = %d\tPortEsclave = %d\tPointCaracteristique = %d\tSiteGestion = %s\tNomServeur = %s\tBASEDD=%s\n",
			va_CAP_in.NomCAP,
			va_CAP_in.Autoroute,
			va_CAP_in.PR,
			va_CAP_in.Sens,
			va_CAP_in.Type,
			va_CAP_in.AdresseRgs,
			va_CAP_in.AdresseIP,
			va_CAP_in.PortMaitre,
			va_CAP_in.PortEsclave,
			va_CAP_in.PointCaracteristique,
			va_CAP_in.NomSite,
			va_CAP_in.NomServeur,
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
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.NomCAP,"@va_NomCAP_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_CAP_in.PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CAP_in.Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_CAP_in.Type,"@va_Type_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_smallint_input(smallint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.AdresseRgs,"@va_Rgs_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.AdresseIP,"@va_AdresseIP_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_CAP_in.PortMaitre,"@va_PortMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_CAP_in.PortEsclave,"@va_PortEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CAP_in.PointCaracteristique,"@va_PointCaracteristique_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_CAP_in.Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_CAP_in.NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao265: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao265 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao265 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao265 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao265: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO265_Ajouter_CAP retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




