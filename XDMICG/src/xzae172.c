/*E*/
/*Fichier : $Id: xzae172.c,v 1.2 2013/11/08 13:38:15 pc2dpdy Exp $    Release : $Revision: 1.2 $        Date : $Date: 2013/11/08 13:38:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae172.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
* JMG	26/02/13	extension TDP
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae172.h"

static char *version = " $Id: xzae172.c,v 1.2 2013/11/08 13:38:15 pc2dpdy Exp $ $Revision: 1.2 $ $Date: 2013/11/08 13:38:15 $ ";
	
/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE172_Recup_TDP (  XDY_FonctionInt va_FonctionUtilisateur_in)


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
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
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = "XZAE1720";
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAE172C_NB_PARM_RETURN;
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	CS_DATETIME     va_Horodate;

	/*A
	** Definition des variables locales
	*/
	char vl_date[50];
	char vl_tdp11[50];
	char vl_tdp12[50];
	char vl_tdp21[50];
	char vl_tdp22[50];
	char vl_tdp31[50];
	char vl_tdp32[50];
	char vl_tdp41[50];
	char vl_tdp42[50];
	char vl_tdp51[50];
	char vl_tdp52[50];
	char vl_tdp53[50];
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE172_Recup_TDP ");
			
			
   	/*A
        **  test les parametres d'entree
        */

	if ( va_FonctionUtilisateur_in == NULL)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur_in est nulle");
	      return (XDC_NOK);
	}

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
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
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae172: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae172 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae172 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae172 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
			 			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae172: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
				}
	}

	
	XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE172_Lire_Infos_TFM %d lignes", vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	           
		/*B
	         * recup des parametres de retour
	         */
	        strcpy(vl_date, (char *) vl_Tab_Ligne[i][0].pt_value);
	        strcpy(vl_tdp11, (char *) vl_Tab_Ligne[i][1].pt_value);
	        strcpy(vl_tdp12, (char *) vl_Tab_Ligne[i][2].pt_value);
	        strcpy(vl_tdp21, (char *) vl_Tab_Ligne[i][3].pt_value);
	        strcpy(vl_tdp22, (char *) vl_Tab_Ligne[i][4].pt_value);
	        strcpy(vl_tdp31, (char *) vl_Tab_Ligne[i][5].pt_value);
	        strcpy(vl_tdp32, (char *) vl_Tab_Ligne[i][6].pt_value);
	        strcpy(vl_tdp41, (char *) vl_Tab_Ligne[i][7].pt_value);
	        strcpy(vl_tdp42, (char *) vl_Tab_Ligne[i][8].pt_value);
	        strcpy(vl_tdp51, (char *) vl_Tab_Ligne[i][9].pt_value);
	        strcpy(vl_tdp52, (char *) vl_Tab_Ligne[i][10].pt_value);
	        strcpy(vl_tdp53, (char *) vl_Tab_Ligne[i][11].pt_value);
	        /*B
		 ** Appel de la fonction utilisateur FonctionUtilisateur
		 */
		  
		if (( retcode = (*va_FonctionUtilisateur_in) (vl_date,
							      vl_tdp11,
							      vl_tdp12,
							      vl_tdp21,
							      vl_tdp22,
							      vl_tdp31,
							      vl_tdp32,
							      vl_tdp41,
							      vl_tdp42,vl_tdp51,vl_tdp52,vl_tdp53) != XDC_OK ))
		{
		        XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_FonctionUtilisateur_in s'est mal deroule");
		        asql_clean_cmd(connection, cmd);
			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
			return (XDC_NOK);
		}	
	}

	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE172_Lire_Infos_TFM retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
