/*E*/
/*Fichier : @(#)xzae175.c        1.0     Release : 1.0        Date : 06/02
------------------------------------------------------
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*       Appartient a BASEDD tache tami2
*       Module xza
*
------------------------------------------------------
* HISTORIQUE :
* ETR : creation juin 2002
* JMG	1.2 linux
------------------------------------------------------*/
/* fichiers inclus */

#include "xzae175.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae175.c    1.0 06/02 : xzae175" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
* Retourne pour chaque station son nom, debit moyen, vitesse moyenne, taux moyen
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE175_Get_6Min_Sequence (XDY_FonctionInt pa_FonctionUtilisateur_in)


/*
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
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

	static char *version =  "@(#)xzae175.c    1.0 06/2002  XZAE175_Get_6Min_Sequence";

	CS_CHAR         *pl_rpc_name = XZAE175_RPC_NAME;

	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND      *pl_cmd;

	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int             vl_num_Tab_donnees2 = XZAE175_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE175_NB_PARM_RETURN];
	tg_row_result   vl_Tab_Ligne;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	int             i;
	XDY_Entier      vl_SPstatus;
	int             retcode;
	
	/* declaration des valeurs retournees */

	char vl_nom_station[9]; /* nom du point de mesure format frgdd.sx*/ 
	int vl_val_QT = 0 ; 	/*debits tous vehicules */
	int vl_val_TT = 0; 	/* taux tous vehicules */
	int vl_val_VT = 0; 	/* vitesses moyennes tous vehicules */

	 /*A
	 ** Initialisation de la commande
	 */

	  if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
		{
		 	 XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_cmd_alloc() a echoue");
			 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne %d", vl_retcode);
			 asql_clean_cmd (pl_connection,pl_cmd);
			 return (XDC_NOK);
		}
	

	 /** Envoie de la commande RMET pour notre stored procedure.
	 */

	 if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
		 {
			 XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_command() a echoue");
			 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne %d", vl_retcode);
			 asql_clean_cmd (pl_connection,pl_cmd);
			 return (XDC_NOK);
		 }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_send() a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne %d", vl_retcode);
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
						 XZST_03EcritureTrace(XZSTC_INFO,"xzae175: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						 break;

					 case  CS_STATUS_RESULT:
						 vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						 XZST_03EcritureTrace(XZSTC_INFO,"XZAE175 : CS_STATUS_RESULT = %ld", vl_retcode);
						 XZST_03EcritureTrace(XZSTC_INFO,"XZAE175 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

						 if (vl_SPstatus != XDC_OK)
						 {
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne 1");
						 	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE175_Get_6Min_Sequence retourne 1 : %d", (int) vl_SPstatus);
							asql_clean_cmd (pl_connection,pl_cmd);
						 	return (XDC_NOK);
						}
						break;
					
					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO, "XZAE175 : PARAMETER RESULTS\n");
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
						 XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_res_info(msgtype) a echoue");
						 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne %d", vl_retcode);
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
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE175_Get_6Min_Sequence retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
																						}

	 }


	/*A
	 ** Appel de la fonction utilisateur pour chaque FMC
	 */
	
	/* check valeur de vl_Ligne_Lue */
	if (vl_Nb_Ligne_Lue == 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175: vl_Nb_Ligne_Lue vaut 0");
		asql_clean_cmd (pl_connection,pl_cmd);
		return ( XDC_NOK );

	}



	 for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	 {
		 /*B
		 * recup des parametres de retour
		 */

		strcpy(vl_nom_station, (char *)vl_Tab_Ligne[i][0].pt_value);
		vl_val_QT = * (CS_SMALLINT *) vl_Tab_Ligne[i][1].pt_value;
		vl_val_TT = * (CS_TINYINT *) vl_Tab_Ligne[i][2].pt_value;
		vl_val_VT = * (CS_TINYINT *) vl_Tab_Ligne[i][3].pt_value;
		
		/*B
	    	** Appel de la fonction utilisateur FonctionUtilisateur
		*/
		
		if ( pa_FonctionUtilisateur_in == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAE175 : pa_FonctionUtilisateur_in est nulle");
			asql_clean_cmd(pl_connection, pl_cmd);
	       		return (XDC_NOK);
		 }

		 else
	    	{
		  
		  XZST_03EcritureTrace(XZSTC_INFO,"XZAE175 : Appel fonction utilisateur");
		  if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_nom_station,
								  vl_val_QT,
								  vl_val_TT,
								  vl_val_VT ) ) != XDC_OK )
			{
			 XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
			 asql_clean_cmd(pl_connection, pl_cmd);
		 	 return (XDC_NOK);
		 	}
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
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	 }

	 /*A
	 ** Retourne le resultat et les parametres de sorties renseignes
	 */

	 asql_clean_cmd(pl_connection, pl_cmd);
	 return(XDC_OK);
}

