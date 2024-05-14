/*E*/
/* Fichier : $Id: xzao495.c,v 1.2 2021/06/07 10:17:11 pc2dpdy Exp $        $Revision: 1.2 $        $Date: 2021/06/07 10:17:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao495.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	02/10/20	: Creation 1.1 DEM-SAE155
* ABE	07/06/21	: Fix Numero_in DEM-SAE155 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao495.h"

static char *version = "@(#)xzao495.c	1.1 02/10/20 : xzao495" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO495SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO495_Ajouter_Zon_IMU (	XDY_Basedd		va_Basedd_in,
				XZAOT_Conf_Zon_IMU 	va_ZoneIMU_in,
				XDY_ResConf  		*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	 va_Basedd_in        		
* XDY_Eqt        va_NumeroEqt_in
* XDY_Entier	 va_Id_in
* XDY_Texte	 va_Libelle_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une zone pour les  IMUs en base
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
* Insertion ou modification dans les tables EQT_IMU_ZON
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao495.c	1.1 02/10/20 : XZAO495_Ajouter_Zon_IMU" ;

	CS_CHAR *rpc_name = XZAOC_XZAO495_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO495_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO495_Ajouter_Zon_IMU : NumeroEqt = %d\tId = %d\tLibelle = %s\n",
			va_ZoneIMU_in.NumeroEqt,
			va_ZoneIMU_in.Id,
			va_ZoneIMU_in.Libelle,
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
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_ZoneIMU_in.NumeroEqt,"@va_Numero_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: asql_ctparam_smallint_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_ZoneIMU_in.Id,"@va_Id_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_ZoneIMU_in.Libelle,"@va_Libelle_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao495: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao495 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao495 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao495 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
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
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao495: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO495_Ajouter_Zon_IMU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




