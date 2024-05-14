/*E*/
/*Fichier : $Id: xzae196.c,v 1.3 2020/12/09 09:01:21 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/12/09 09:01:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae196.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tada2
* permet d'extraire le code SIREDO d'une station pour 
* avec son numero et le sens
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 31/07/2020 : Creation DEM-SAE130 V1.1
* ABE 15/10/2020 : Ajout colonne version_radt DEM-SAE130 V1.2
* ABE 08/12/2020 : Fix asql_clean commenté non release V1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae196.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae196.c,v 1.3 2020/12/09 09:01:21 pc2dpdy Exp $ : XZAE196" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE196_Lire_Code_SIREDO : recupere les infos 
*  de coordonee en fonction du PR/axe
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern int XZAE196_Lire_Code_SIREDO (XDY_Eqt va_numero_station_in ,
                                     XDY_Sens va_sens_in,
					char * va_code_siredo_out,
					char * va_version_station_out)


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_numero_station_in
* XDY_Sens 	va_sens_in
*				
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
* CFG..RES_DIT.nom_pt_mesure
* CFG..RES_DIT.version_radt
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* - va_numero_station, va_sens sont obligatoires
*

* FONCTION :
*
* Retourne le code SIREDO et la version de la station pour un numero de station et un sens donnée
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae196.c,v 1.3 2020/12/09 09:01:21 pc2dpdy Exp $ : XZAE196_Lire_Code_SIREDO" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE196C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE196C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE196C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE196_Lire_Code_SIREDO : Debut d execution" );

	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196:va_numero_station_in=%d, va_sens_in=%d",va_numero_station_in,va_sens_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
XZST_03EcritureTrace( XZSTC_INTERFACE,"1");
	/*A		
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_numero_station_in,"@va_numero_station_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: asql_ctparam_tinyint_input(va_numero_sration_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
     
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_sens_in,"@va_sens_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: asql_ctparam_tinyint_input(va_sens_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE196: ROW RESULTS : %d", vl_Nb_Ligne_Lue);		                      
						break;

					case  CS_PARAM_RESULT:

						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAE196_Lire_Code_SIREDO : PARAMETER RESULTS\n");		
						break;

					case  CS_STATUS_RESULT:

						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE196 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE196 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE196_Lire_Code_SIREDO retourne 1 : %d",(int)vl_retcode);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				break;				


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}	

	

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	if (vl_Nb_Ligne_Lue>0)
	{
		strcpy(va_code_siredo_out,  (char *)vl_Tab_Ligne[0][0].pt_value);
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE196_Lire_Code_SIREDO :code siredo =%s",va_code_siredo_out);
		if(vl_Nb_Col==2)strcpy(va_version_station_out,  (char *)vl_Tab_Ligne[0][1].pt_value);

		
		vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
		if (vl_retcode != XDC_OK)
		{
		   XZST_03EcritureTrace(XZSTC_WARNING," XZAE196_Lire_Code_SIREDOt: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		   asql_clean_cmd(pl_connection, pl_cmd);
		   return (XDC_NOK);
		}
	}
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE196_Lire_Code_SIREDO retourne %d", vl_retcode);
 	return(XDC_OK);
}




