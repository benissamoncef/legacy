/*E*/
/* Fichier : $Id: xzao184.c,v 1.4 2018/11/07 17:26:12 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/07 17:26:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao184.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic       27 Sep 1994     : Creation
* Volcic       01 Nov 1994     : Modification entete (v1.2)
* Mismer       05 Fev 1997     : Ajout colonne voie et inverse (v1.3)
* JPL		06/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao184.c,v 1.4 2018/11/07 17:26:12 devgfi Exp $ : xzao184" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO184SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO184_Ajouter_PnGTC (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfPnGTC		va_PnGTC_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomPnGTC_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_NomMaitre_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Voie		va_Voie_in
* XDY_Octet		va_Inverse_in
* XDY_Octet		va_RangComm_in
* XDY_Octet		va_RangComm2_in
* XDY_Octet		va_RangEtat_in
* XDY_Octet		va_RangEtat2_in
* XDY_Octet		va_BitDiscordance_in
* XDY_Octet		va_BitDiscordance2_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute d'un panneau GTC en base
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
* Insertion ou modification dans la table EQT_PAN (EQUIPEMENT_PANNEAU)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao184.c,v 1.4 2018/11/07 17:26:12 devgfi Exp $ : XZAO184_Ajouter_PnGTC" ;

	CS_CHAR *rpc_name = XZAOC_XZAO184_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO184_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomPnGTC;
	XDY_NomAuto	vl_Autoroute;
	XDY_NomEqt	vl_NomMaitre;
	XDY_PR		vl_PR;
	XDY_Sens	vl_Sens;
	XDY_Voie	vl_Voie;
	XDY_Octet	vl_Inverse;
	XDY_Octet	vl_RangComm;
	XDY_Octet	vl_RangComm2;
	XDY_Octet	vl_RangEtat;
	XDY_Octet	vl_RangEtat2;
	XDY_Octet	vl_BitDiscordance;
	XDY_Octet	vl_BitDiscordance2;
	XDY_Booleen	vl_Supprime;
	
	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomPnGTC, va_PnGTC_in.NomPnGTC );
	strcpy( vl_Autoroute, va_PnGTC_in.Autoroute );
	strcpy( vl_NomMaitre, va_PnGTC_in.NomMaitre );
	vl_PR		 	= va_PnGTC_in.PR;
	vl_Sens		 	= va_PnGTC_in.Sens;
	vl_Voie		 	= va_PnGTC_in.Voie;
	vl_Inverse		= va_PnGTC_in.Inverse;
	vl_RangComm		= va_PnGTC_in.RangComm;
	vl_RangComm2  		= va_PnGTC_in.RangComm2;
	vl_RangEtat		= va_PnGTC_in.RangEtat;
	vl_RangEtat2		= va_PnGTC_in.RangEtat2;
	vl_BitDiscordance	= va_PnGTC_in.BitDiscordance;
	vl_BitDiscordance2 	= va_PnGTC_in.BitDiscordance2;
	vl_Supprime		= va_PnGTC_in.Supprime;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO184_Ajouter_PnGTC : Nom = %s\tAutoroute = %s\tPR = %d\tSens = %d\tRangComm = %d\tRangComm2 = %d\tRangEtat = %d\tRangEtat2 = %d\tBitDiscordance = %d\tBitDiscordance2 = %d\n",
			va_PnGTC_in.NomPnGTC,
			va_PnGTC_in.Autoroute,
			va_PnGTC_in.NomMaitre,
			va_PnGTC_in.PR,
			va_PnGTC_in.Sens,
			va_PnGTC_in.RangComm,
			va_PnGTC_in.RangComm2,
			va_PnGTC_in.RangEtat,
			va_PnGTC_in.RangEtat2,
			va_PnGTC_in.BitDiscordance,
			va_PnGTC_in.BitDiscordance2 );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomPnGTC,"@va_NomPnGTC_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_char_input(NomPnGTC) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomMaitre,"@va_NomMaitre_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_char_input(NomMaitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Voie,"@va_Voie_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(Voie) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Inverse,"@va_Inverse_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(Inverse) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_RangComm,"@va_RangComm_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(RangComm) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_RangComm2,"@va_RangComm2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(RangComm2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_RangEtat,"@va_RangEtat_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(RangEtat) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_RangEtat2,"@va_RangEtat2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(RangEtat2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_BitDiscordance,"@va_BitDiscordance_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(BitDiscordance) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_BitDiscordance2,"@va_BitDiscordance2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_tinyint_input(BitDiscordance2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao184: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao184 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao184 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao184: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO184_Ajouter_PnGTC retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




