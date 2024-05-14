/*E*/
/*Fichier : $Id: xzae197.c,v 1.2 2020/07/20 14:26:24 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/07/20 14:26:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae197.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recupère la date de départ et la radio des patrouilleurs en action
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 02/07/2020 : Creation DEM 130 V1.1
* ABE 20/07/2020 : DEM 130 V1.2 correction malloc pour radio
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae197.h"

/* definitions de constantes */

/* definitions de types locau		x */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae197.c,v 1.2 2020/07/20 14:26:24 pc2dpdy Exp $ : XZAE197" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE197_get_pat_radio_date_depart : Recupère la date de départ et la radio des patrouilleurs en action
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern int XZAE197_get_pat_radio_date_depart (XZAE_LOC_GPS *loc_gps_out)



/*
*
* PARAMETRES EN ENTREE :
*
* XZAE_LOC_GPS *	loc_gps_out
*				
* PARAMETRES EN SORTIE :
*
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
*

* FONCTION :
*
* Recupere la date de depart, la radio, l'autoroute, le pr et le sens des patrouilleurs en action
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae197.c,v 1.2 2020/07/20 14:26:24 pc2dpdy Exp $ : XZAE197_get_pat_radio_date_depart" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE197C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE197C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE197C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = -1;
   	int             vl_Nb_Col = 0;
    	tg_row_result   vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE197_get_pat_radio_date_depart : Debut d execution" );
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A		
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
		
		
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE197: ROW RESULTS : %d", vl_Nb_Ligne_Lue);		
	
						break;

					case  CS_PARAM_RESULT:

						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAE197_get_pat_radio_date_depart : PARAMETER RESULTS\n");		
						break;

					case  CS_STATUS_RESULT:

						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE197 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE197 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE197_get_pat_radio_date_depart retourne 1 : %d",(int)vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart Fin d execution retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart Fin d execution retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}	

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	/* Set taille des tableaux de donnée aux nombre de lignes retournées */
	loc_gps_out->size=vl_Nb_Ligne_Lue;

	/* Allocation de la mémoire à la taille des tableaux */
	loc_gps_out->heure_succes= (XDY_Horodate*) malloc (loc_gps_out->size * sizeof(XDY_Horodate));
	loc_gps_out->autoroute= (XDY_Autoroute*) malloc (loc_gps_out->size * sizeof(XDY_Autoroute));
	loc_gps_out->pr= (XDY_PR*) malloc (loc_gps_out->size * sizeof(XDY_PR));
	loc_gps_out->sens= (XDY_Sens*) malloc (loc_gps_out->size * sizeof(XDY_Sens));
	loc_gps_out->radio=malloc (loc_gps_out->size * sizeof(loc_gps_out->radio));
	/* Si malloc échoue */
	if (loc_gps_out->heure_succes == NULL || loc_gps_out->autoroute == NULL || loc_gps_out->pr == NULL || loc_gps_out->sens == NULL) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: malloc retourne NULL pour allocation mémoire de taille =%d",loc_gps_out->size);
		return XDC_NOK;
	}

	/* pour chaque ligne retournée */
	for (i=0; i<loc_gps_out->size; i++)
	{
		/* Allocation de la mémoire à la taille des tableaux */
		loc_gps_out->radio[i]=(char*) malloc (sizeof(char)*RADIO_LENGTH);
		/* Si malloc échoue */
		if (loc_gps_out->radio[i] == NULL) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAE197: malloc retourne NULL pour allocation radio index=%d, taille=%d",i,loc_gps_out->size);
			return XDC_NOK;
		}
		
		/* Remplissage des tableaux de la structure */
		strcpy(loc_gps_out->radio[i], vl_Tab_Ligne[i][0].pt_value);		

		if ((vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME *)vl_Tab_Ligne[i][1].pt_value,&loc_gps_out->heure_succes[i]))!= XDC_OK)
		{

			XZST_03EcritureTrace(XZSTC_WARNING,"xzae197: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_out) a echoue");
			XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart retourne %d", vl_retcode);
		}
		loc_gps_out->autoroute[i]= *(XDY_Autoroute*) vl_Tab_Ligne[i][2].pt_value;
		loc_gps_out->pr[i]=*(XDY_PR*) vl_Tab_Ligne[i][3].pt_value;
		loc_gps_out->sens[i]=*(XDY_Sens*) vl_Tab_Ligne[i][4].pt_value;			
	}

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (vl_retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING," XZAE197_get_pat_radio_date_depart: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE197_get_pat_radio_date_depart retourne %d", vl_retcode);
 	return(XDC_OK);
}

extern int XZAE197_free_memory (XZAE_LOC_GPS *loc_gps_out) {
	int i=0;
	for (i=0; i<loc_gps_out->size; i++)
	{
		free(loc_gps_out->radio[i]);
		loc_gps_out->radio[i]=NULL;

	}
	free(loc_gps_out->heure_succes);
	free(loc_gps_out->pr);
	free(loc_gps_out->sens);
	free(loc_gps_out->autoroute);
	free(loc_gps_out->radio);
	loc_gps_out->heure_succes=NULL;
	loc_gps_out->pr=NULL;
	loc_gps_out->sens=NULL;
	loc_gps_out->autoroute=NULL;
	loc_gps_out->size=0;
}
