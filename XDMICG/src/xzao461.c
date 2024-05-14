/*E*/
/*  Fichier : 	@(#)xzao461.c	1.2	Release : 1.2        Date : 08/11/11
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao461.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       11 Nov 2011   1.1  : Creation (DEM/1014)
* VR       27 Jui 2012   1.2  : Lecture Nom de zones au lieu des Id
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao461.c	1.0 08/11/11 : xzao461" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO461SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO461_Ajouter_Destination_TDP (	XDY_Basedd		va_Basedd_in,
					XZAOT_ConfDestPMV	va_DestTDP_in,
					XDY_ResConf  		*va_Resultat_out )

   
/*
* PARAMETRES EN ENTREE :
*
*   XDY_Basedd		    va_Basedd_in
*   XZAOT_ConfDestPMV       va_DestTDP_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une destination (Destination Temps de parcours) en base
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
* Insertion ou modification dans la table RES_DTP (Destination Temps de parcours)
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzao461.c	1.2 06/05/97 : XZAO461_Ajouter_Destination_TDP" ;

	CS_CHAR *rpc_name = XZAOC_XZAO461_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO461_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Nom			vl_NomDest;
	XDY_NomAuto		vl_Autoroute;
    XDY_PR          vl_PR;
	XDY_Sens		vl_Sens;
    XDY_Octet     	vl_Type_Destination;
	XDY_Sens		vl_SensDest;
    XDY_Booleen		vl_Inhibition;
	XDY_Entier		vl_init;
	XDY_Nom			vl_Nom_ZDP;
	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomDest, va_DestTDP_in.NomDest );
	strcpy( vl_Autoroute, va_DestTDP_in.Autoroute );
	strcpy( vl_Nom_ZDP, va_DestTDP_in.Nom_ZDP );
	vl_PR	= va_DestTDP_in.PR;
	vl_Sens		= va_DestTDP_in.Sens;
	vl_SensDest	= va_DestTDP_in.SensDest;
	vl_Type_Destination		= va_DestTDP_in.TypeDest;
	vl_Inhibition	= va_DestTDP_in.Inhibition;
	vl_init		= va_DestTDP_in.Init;
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO461_Ajouter_Destination_TDP : Autoroute = %s\tPR = %d\tSens = %d\tDestination: %s\tType: %d\tSensDest = %d\tNom ZDP=%s\tinhibition:%d\tinit:%d\n",
										va_DestTDP_in.Autoroute,
										va_DestTDP_in.PR,
										va_DestTDP_in.Sens,
										va_DestTDP_in.NomDest,
										va_DestTDP_in.TypeDest,
										va_DestTDP_in.SensDest,
										va_DestTDP_in.Nom_ZDP,
										va_DestTDP_in.Inhibition,
										va_DestTDP_in.Init);

	XZST_03EcritureTrace( XZSTC_WARNING,"BASEDD %s",va_Basedd_in);

	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO461_Ajouter_Destination_TDP : init %d",vl_init);

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
	
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
		
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_NomDest,"@va_Nom_Dest_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_char_input(NomDest) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type_Destination,"@va_Type_Dest_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461:asql_ctparam_tinyint_input(Type_Destination)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_SensDest,"@va_Sens_Dest_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_tinyint_input(SensDest) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Inhibition,"@va_Inhibition_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461:asql_ctparam_bit_input(Inhibition)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_Nom_ZDP,"@va_Nom_Zdp_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461:asql_ctparam_char_input(Nom_ZDP)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
 	}

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_init,"@va_droptable_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461:asql_ctparam_int_input(Init table)  a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao461: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao461 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao461 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao461: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes


	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO461_Ajouter_Destination_TDP retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




