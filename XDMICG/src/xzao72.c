/*E*/
/*  Fichier : $Id: xzao72.c,v 1.6 2019/01/24 16:22:09 devgfi Exp $        $Revision: 1.6 $        $Date: 2019/01/24 16:22:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao72.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* JMG   13/11/07	:	ajout site de gestion DEM715
* JMG	02/10/18	:	suppression SAGA 1.4
* JPL	24/01/19 	:	Gestion du parametre supplementaire 'Supprime' (DEM 1305)  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao72.c,v 1.6 2019/01/24 16:22:09 devgfi Exp $ : xzao72" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO72SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO72_Ajouter_PMVPicto (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfPMV	 	va_PMVPicto_in,
				XDY_ResConf  		*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomPMV_in
* XDY_Nom       va_Autoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonPMV_in
* XDY_Rgs       va_RgsPMV_in
* XDY_Nom       va_TypePMV_in
* XDY_Octet     va_LiaisonPicto_in
* XDY_Rgs       va_RgsPicto_in
* XDY_Nom       va_TypePicto_in			
* XDY_NomSite   va_NomSite_in
* XDY_AdresseIP va_AdresseIP_in
* XDY_Entier    va_PortIP_in
* XDY_Octet     va_TypeLCR_in
* char(50)      va_IdLCR_in
* XDY_Booleen   va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PMV et un Picto en base
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
* Insertion ou modification dans les tables EQT_PMV (C_PMV), EQT_PCT (C_PICTO) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao72.c,v 1.6 2019/01/24 16:22:09 devgfi Exp $ : XZAO72_Ajouter_PMVPicto" ;

	CS_CHAR *rpc_name = XZAOC_XZAO72_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO72_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomPMV;
	XDY_NomSite	vl_NomSite;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_PR		vl_PR;
   	XDY_Sens	vl_Sens;
   	XDY_NomEqt	vl_NomServeur;
   	XDY_Octet	vl_LiaisonPMV;
   	XDY_Rgs		vl_RgsPMV;
   	XDY_Nom		vl_TypePMV;
   	XDY_Octet	vl_LiaisonPicto;
   	XDY_Rgs		vl_RgsPicto;
   	XDY_Nom		vl_TypePicto;
   	XDY_NomFich	vl_Seuils;
   	XDY_AdresseIP	vl_adr;
   	int	vl_port;
	XDY_Octet	vl_TypeLCR;
	char		vl_IdLCR[50];
	XDY_Booleen	vl_Supprime;

	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomPMV, va_PMVPicto_in.NomPMV );
	strcpy( vl_NomSite, va_PMVPicto_in.NomSite );
	strcpy( vl_Autoroute, va_PMVPicto_in.Autoroute );
	vl_PR         = va_PMVPicto_in.PR;
	vl_Sens       = va_PMVPicto_in.Sens;
	strcpy( vl_NomServeur, va_PMVPicto_in.NomServeur );
	vl_LiaisonPMV    = va_PMVPicto_in.LiaisonPMV;
	strcpy( vl_RgsPMV, va_PMVPicto_in.RgsPMV );
	strcpy( vl_TypePMV, va_PMVPicto_in.TypePMV );
	vl_LiaisonPicto    = va_PMVPicto_in.LiaisonPicto;
	strcpy( vl_RgsPicto, va_PMVPicto_in.RgsPicto );
	strcpy( vl_TypePicto, va_PMVPicto_in.TypePicto );
	strcpy( vl_adr, va_PMVPicto_in.AdresseIP );
	vl_port = va_PMVPicto_in.PortIP;
	vl_TypeLCR = va_PMVPicto_in.TypeLCR;
	strcpy( vl_IdLCR, va_PMVPicto_in.IdLCR );
	vl_Supprime = va_PMVPicto_in.Supprime;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO72_Ajouter_PMVPicto : PMVPicto = %s\tAutoroute = %s\tPR = %d\tSens = %d\tServeur = %s\tLiaisonPMV = %d\tRgsPMV = %s\tTypePMV = %s\tLiaisonPicto = %d\tRgsPicto = %s\tTypePicto = %s\tSiteGestion = %s\tSupprime = %d\tAdresse = %s\nPort = %d\nTypeLCR = %d\nIdLCR = %s.\n",
			va_PMVPicto_in.NomPMV,
			va_PMVPicto_in.Autoroute,
			va_PMVPicto_in.PR,
			va_PMVPicto_in.Sens,
			va_PMVPicto_in.NomServeur,
			va_PMVPicto_in.LiaisonPMV,
			va_PMVPicto_in.RgsPMV,
			va_PMVPicto_in.TypePMV,
			va_PMVPicto_in.LiaisonPicto,
			va_PMVPicto_in.RgsPicto,
			va_PMVPicto_in.TypePicto ,
			va_PMVPicto_in.NomSite,
			va_PMVPicto_in.Supprime,
			va_PMVPicto_in.AdresseIP,
			va_PMVPicto_in.PortIP,
			va_PMVPicto_in.TypeLCR,
			va_PMVPicto_in.IdLCR);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomPMV,"@va_NomPMV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_NomAutoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonPMV,"@va_LiaisonPMV_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_RgsPMV,"@va_RgsPMV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_TypePMV,"@va_TypePMV_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonPicto,"@va_LiaisonPicto_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_RgsPicto,"@va_RgsPicto_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_TypePicto,"@va_TypePicto_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_adr,"@va_AdresseIP_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input(Adresse IP) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_port,"@va_PortIP_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_int_input(Port IP) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_TypeLCR,"@va_TypeLCR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_tinyint_input(Type LCR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_IdLCR,"@va_IdLCR_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_char_input(Id LCR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
		
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao72: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao72 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao72 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao72 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao72: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO72_Ajouter_PMVPicto retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




