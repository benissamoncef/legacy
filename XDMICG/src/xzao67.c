/*E*/
/*  Fichier : $Id: xzao67.c,v 1.7 2019/01/24 12:47:07 devgfi Exp $        $Revision: 1.7 $        $Date: 2019/01/24 12:47:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao67.c
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
*JMG   13/11/07        : ajout site de gestion DEM715
* JMG	12/11/18 : ajout IP DEM1306 1.6
* JPL	24/01/19 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao67.c	1.3 12/15/94 : xzao67.c" ;
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO67SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO67_Ajouter_Station_RADT(XDY_Basedd		va_Basedd_in,
				XZAOT_ConfRADT	 	va_Station_in,
				XDY_ResConf  		*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomStation_in
* XDY_Nom	va_Autoroute_in	
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Eqt	va_NomServeur_in
* XDY_Octet	va_Liaison_in
* XDY_Rgs	va_RGS_in
* XDY_NomFich	va_Seuils_in
* XDY_NomFich	va_Alertes_in
* XDY_Booleen	va_DonneesIndividuelles_in				
* XDY_NomSite	va_NomSite_in
* XDY_Booleen	va_Supprime_in
* XDY_Octet	va_type_in
* XDY_AdresseIP	va_adresseIP_in
* XDY_Entier	va_port_in
* char(20)	va_idLCR_in
* XDY_Entier	va_port_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une station de comptage en base
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
* Insertion ou modification dans la table EQT_RAD (STATION RADT)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao67.c	1.3 12/15/94 : XZAO67_Ajouter_Station" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO67_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO67_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomStation;
	XDY_NomSite	vl_NomSite;
   	XDY_NomAuto	vl_NomAutoroute;
   	XDY_PR		vl_PR;
   	XDY_Sens	vl_Sens;
   	XDY_NomEqt	vl_NomServeur;
   	XDY_Octet	vl_Liaison;
   	XDY_Rgs		vl_RGS;
   	XDY_NomFich	vl_Seuils;
   	XDY_NomFich	vl_Alertes;
   	XDY_Booleen	vl_DonneesIndividuelles;
   	XDY_Octet	vl_type;
	int		vl_port;
	char	vl_adresseIP[20];
	char	vl_idLCR[20];
	XDY_Booleen	vl_Supprime;

	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomStation, va_Station_in.NomStation );
	strcpy( vl_NomAutoroute, va_Station_in.NomAutoroute );
	strcpy( vl_NomSite, va_Station_in.NomSite );
	vl_PR         = va_Station_in.PR;
	vl_Sens       = va_Station_in.Sens;
	strcpy( vl_NomServeur, va_Station_in.NomServeur );
	vl_Liaison    = va_Station_in.Liaison;
	strcpy( vl_RGS, va_Station_in.RGS );
	strcpy( vl_Seuils, va_Station_in.Seuils );
	strcpy( vl_Alertes, va_Station_in.Alertes );
	vl_DonneesIndividuelles = va_Station_in.DonneesIndividuelles;
	strcpy( vl_adresseIP, va_Station_in.AdresseIP );
	strcpy( vl_idLCR, va_Station_in.IdLCR );
	vl_port = va_Station_in.Port;
	vl_type = va_Station_in.Type;
	vl_Supprime = va_Station_in.Supprime;
	
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO67_Ajouter_Station : Station = %s\tAutoroute = %s\tPR = %d\tSens = %d\nServeur = %s\tRGS = %s\tSeuils = %s\tAlertes = %s\tDonneesInd = %d\tSiteGestion = %s, Type=%d, IdLCR=%s,\n",
			va_Station_in.NomStation,
			va_Station_in.NomAutoroute,
			va_Station_in.PR,
			va_Station_in.Sens,
			va_Station_in.NomServeur,
			/*va_Station_in.Liaison,*/
			va_Station_in.RGS,
			va_Station_in.Seuils,
			va_Station_in.Alertes,
			va_Station_in.DonneesIndividuelles ,
			va_Station_in.NomSite,
			va_Station_in.Type,
			va_Station_in.IdLCR);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomStation,"@va_NomStation_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(NomStation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomAutoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(NomAutoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(NomServeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Liaison,"@va_Liaison_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_tinyint_input(Liaison) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_RGS,"@va_RGS_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(RGS) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Seuils,"@va_Seuils_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(Seuils) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Alertes,"@va_Alertes_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(Alertes) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_DonneesIndividuelles,"@va_DonneesIndividuelles_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_bit_input(DonneesIndividuelles) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(NomSite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_type,"@va_type_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_tinyint_input(Type) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_adresseIP,"@va_adresseIP_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(AdresseIP) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_port,"@va_port_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_int_input(Port) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_idLCR,"@va_idLCR_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_char_input(IdLCR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Aire retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao67: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao67 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station_RADT retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station_RADT retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao67 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao67: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO67_Ajouter_Station retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




