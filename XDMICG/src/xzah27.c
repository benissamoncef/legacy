/*E*/
/*Fichier : $Id: xzah27.c,v 1.1 1995/01/17 16:04:31 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 16:04:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah27.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*********************
*  XZAH27_Satir appelle xzes01 pour envoyer les fmc a satir
*********************
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	16 Jan 1995	: creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah27.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah27.c,v 1.1 1995/01/17 16:04:31 gaborit Exp $" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  XZAH27_Satir appelle xzes01 pour envoyer les fmc a satir
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH27_Satir()

/*
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
*
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
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAH27C_XZAH27_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2 = XZAH27C_XZAH27_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	
	/*A
	** Definition des variables locales
	*/	
		
	XZEST_FIC_SAT	vl_Evt;
	CS_DATETIME	vl_Horodate;
		
	static char *version = "$Id: xzah27.c,v 1.1 1995/01/17 16:04:31 gaborit Exp $ xzah27_Satir" ;	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s ",version);			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_cmd_alloc() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_send() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/
	while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"%s: ROW RESULTS",version);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : PARAMETER RESULTS\n",version);
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah27 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzah27 : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAH27_Envoyer_Bouchon_Chaussee_Portion retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel de xzes01 pour chaque evenement
	*/
	
	
        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{         	
           	/*B
           	** Remplissage de la structure XDY_Evt ainsi que des parametres pris par la XZIS03 avec 
           	** coorrespondant a la ligne resultat
           	** du select de la procedure stockee XZAH;27
           	*/
           	if(vl_Tab_Ligne[i][0].indicator == CS_INT_TYPE)
           	vl_Evt.NoEvt		= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value; 
           	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: Probleme sur la colonne 1 sur retour de select de XZAH;27");
           	if(vl_Tab_Ligne[i][1].indicator == CS_TINYINT_TYPE)
       	   	vl_Evt.CleEvt			= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: Probleme sur la colonne 2 sur retour de select de XZAH;27");
           	if(vl_Tab_Ligne[i][2].indicator == CS_DATETIME_TYPE)
       	   	vl_Horodate			= * (CS_DATETIME *) vl_Tab_Ligne[i][2].pt_value;
          	else XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: Probleme sur la colonne 3 sur retour de select de XZAH;27");
         	
         	if (asql_date_Sybase2Ux(vl_Horodate,&(vl_Evt.Horodate))!=XDC_OK)
         	{
         	XZST_03EcritureTrace(XZSTC_WARNING,"xzah27: Probleme sur la conversion horodate");
         	}
         	/*A 
         	** pour chaque ligne retournee par XZAH;27 on appelle XZES01
         	*/
         	/*! SPCI en dur !!! */
         	if (XZES01_Creer_Fichier_SATIR (vl_Evt, "SPCI" )!=XDC_OK)
         	{
         		XZST_03EcritureTrace(XZSTC_WARNING,"xzah27 : XZES01");
         	}
         	else	XZST_03EcritureTrace(XZSTC_DEBUG1,"xzah27 : XZES01 : OK");          
	}
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	if ((vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne))!= XDC_OK)
	{
	   	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           	asql_clean_cmd (pl_connection,pl_cmd);
           	return (XDC_NOK);
	}	
 		
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	asql_clean_cmd (pl_connection,pl_cmd);	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d", version,vl_retcode);
 	return(XDC_OK);
}


