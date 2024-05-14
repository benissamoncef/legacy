																/*E*/
/*Fichier : $Id: xzao505.c,v 1.1 2021/07/20 12:24:12 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/07/20 12:24:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao505.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/05/2021	Création DEM-SAE155 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao505.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO505SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO505_Recherche_IMU(XDY_Texte	va_NomBICE_in,
			int	*va_NumEqt_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Texte va_NomBICE_in
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	va_NumEqt_out
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Récupère le tunnel opposé à partir d'une autoroute sens pr
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao505.c	1.1 28/07/20 : XZAO505_Recherche_IMU" ;

	CS_CHAR *rpc_name =XZAOC505_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC505_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO505_Recherche_IMU :va_NomBICE_in :%s.",va_NomBICE_in);
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}								

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomBICE_in,"@va_NomBICE_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        

	Tab_donnees2[0].pt_value = (CS_VOID *)va_NumEqt_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEqt_out,"@va_NumEqt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);								
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_send() a echoue");				
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao505: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao505 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao505 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao505 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao505: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace( XZSTC_INTERFACE,"OUT : XZAO505_Recherche_IMU : Eqt trouvee :%d", *va_NumEqt_out);							
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO505_Recherche_IMU retourne : %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}



							
