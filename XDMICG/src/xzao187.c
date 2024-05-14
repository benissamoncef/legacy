/*E*/
/* Fichier : $Id: xzao187.c,v 1.4 2018/11/07 10:40:45 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/07 10:40:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao187.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       04 Nov 1996     : Creation (DEM/1232)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression de "free" avant "malloc"  1.3
* JPL	06/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao187.c,v 1.4 2018/11/07 10:40:45 devgfi Exp $ : xzao187" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO187SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO187_Ajouter_Eqt_generique_LCR (	XDY_Basedd		va_Basedd_in,
					XZAOT_ConfEqtGenLCR	va_EqtLCR_in,
					XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomEqt_in
* XDY_NomAuto		va_Autoroute_in
* XDY_NomEqt		va_Serveur_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_LiaisonMaitre_in
* XDY_Octet		va_LiaisonEsclave_in
* XDY_Rgs		va_RGS_in
* XDY_NomSite		va_NomSite_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'un Åquipement generique LCR en base
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
* Insertion ou modification dans la table EQT_LCR (EQT_GENERIQUE_LCR)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao187.c	1.1 12/05/96 : XZAO187_Ajouter_Eqt_generique_LCR" ;

	CS_CHAR *rpc_name = XZAOC_XZAO187_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO187_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomEqtLCR;
	XDY_NomSite	vl_NomSite;
	XDY_NomAuto	vl_Autoroute;
	XDY_PR		vl_PR;
	XDY_Sens	vl_Sens;
	XDY_Octet	vl_Type;
   	XDY_NomEqt	vl_NomServeur;
   	XDY_Rgs		vl_Rgs;
   	XDY_Octet	vl_LiaisonMaitre;
   	XDY_Octet	vl_LiaisonEsclave;
   	XDY_Octet	vl_Code;
	XDY_Booleen	vl_Supprime;
	
	
   	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomEqtLCR, va_EqtLCR_in.Nom );
	strcpy( vl_NomSite, va_EqtLCR_in.NomSite );
	strcpy( vl_Autoroute, va_EqtLCR_in.Autoroute );
	strcpy( vl_NomServeur, va_EqtLCR_in.NomServeur );
	strcpy( vl_Rgs, va_EqtLCR_in.Rgs );
	vl_LiaisonMaitre  = va_EqtLCR_in.LiaisonMaitre;
	vl_LiaisonEsclave = va_EqtLCR_in.LiaisonEsclave;
	vl_PR		  = va_EqtLCR_in.PR;
	vl_Sens		  = va_EqtLCR_in.Sens;
	vl_Type 	  = va_EqtLCR_in.Type;
	vl_Code 	  = va_EqtLCR_in.Code;
	vl_Supprime	  = va_EqtLCR_in.Supprime;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO187_Ajouter_Eqt_generique_LCR : Nom = %s\tAutoroute = %s\tPR = %d\tSens = %d\tType = %d\tServeur:%s\tLMaitre = %d\tLMaitre = %d\tRGRS:%s\tSiteGestion=%s\n",
			va_EqtLCR_in.Nom,
			va_EqtLCR_in.Autoroute,
			va_EqtLCR_in.PR,
			va_EqtLCR_in.Sens,
			va_EqtLCR_in.Type,
			va_EqtLCR_in.NomServeur,
			va_EqtLCR_in.LiaisonMaitre,
			va_EqtLCR_in.LiaisonEsclave,
			va_EqtLCR_in.Rgs,
			va_EqtLCR_in.NomSite);
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomEqtLCR,"@va_NomEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(NomEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_Serveur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(NomServeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type,"@va_Type_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_tinyint_input(Type) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonMaitre,"@va_LiaisonMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_tinyint_input(LiaisonMaitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonEsclave,"@va_LiaisonEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_tinyint_input(LiaisonEsclave) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Rgs,"@va_RGS_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(Rgs) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Code,"@va_Code_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_tinyint_input(Code) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_char_input(NomSite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao187: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao187 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao187 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao187: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO187_Ajouter_Eqt_generique_LCR retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




