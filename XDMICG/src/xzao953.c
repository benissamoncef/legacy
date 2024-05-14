/*E*/
/*  Fichier : 	@(#)xzao953.c	1.0      Release : 1.0        Date : 26/06/2017
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao953.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL          26 Jun 2017     : Creation (convergence)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao953.c	1.0 26/06/17 : xzao953" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO953SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO953_Ajouter_ZoneUrbaine (    XDY_Basedd		va_Basedd_in,
				XZAOT_ConfZoneUrbaine	va_ZoneUrbaine_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRdebut
* XDY_PR		va_PRfin	
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une aire en base
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
* FONCTION :
*
* Insertion ou modification dans la table ZON_URB (ZONES URBAINES)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao953.c	1.0 26/06/17 : XZAO953_Ajouter_ZoneUrbaine" ;

	CS_CHAR *rpc_name = XZAOC_XZAO953_RPC_NAME;
	
	CS_CONNECTION   *vl_connection=XZAGV_Connexion;
	CS_COMMAND	*vl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAOC_XZAO953_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	XDY_Entier	vl_SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Octet	vl_Numero;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_Sens	vl_Sens;
   	XDY_PR		vl_PRdebut;
	XDY_PR		vl_PRfin;
	XDY_Booleen	vl_Creation;
	
	/*A
	** Definition des variables locales
	*/
	
	vl_Numero 	= va_ZoneUrbaine_in.Numero;
	strcpy( vl_Autoroute, va_ZoneUrbaine_in.Autoroute );
	vl_Sens		= va_ZoneUrbaine_in.Sens;
	vl_PRdebut	= va_ZoneUrbaine_in.PRdebut;
	vl_PRfin	= va_ZoneUrbaine_in.PRfin;
	
	
	
	XZST_03EcritureTrace( XZSTC_WARNING , "XZAO953_Ajouter_ZoneUrbaine : Numero = %d\tAutoroute = %s\tSens = %d\tPRdebut = %d\tPRfin = %d\n",
			va_ZoneUrbaine_in.Numero,
			va_ZoneUrbaine_in.Autoroute,
			va_ZoneUrbaine_in.Sens,
			va_ZoneUrbaine_in.PRdebut,
			va_ZoneUrbaine_in.PRfin);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	vl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(vl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
		free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
	  	free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
           free(vl_Tab_donnees2);
	   return (XDC_NOK);
        }

	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Numero,"@va_Numero_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)


	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_PRdebut,"@va_Debut_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_int_input(PRdebut) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_PRfin,"@va_Fin_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_int_input(PRfin) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((vl_retcode = asql_ctparam_int_output(vl_cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: asql_ctparam_smallint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao953: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
	   	free(vl_Tab_donnees2);
	   	return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao953: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (vl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne 1");
 						   asql_clean_cmd(vl_connection, vl_cmd);
	   					   free(vl_Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao953 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
                                        asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao953: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
						asql_clean_cmd(vl_connection, vl_cmd);
	   					free(vl_Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao953 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao953: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao953: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO953_Ajouter_ZoneUrbaine retourne %d", vl_retcode);
 	asql_clean_cmd(vl_connection, vl_cmd);
 	free(vl_Tab_donnees2);
	return(XDC_OK);
}




