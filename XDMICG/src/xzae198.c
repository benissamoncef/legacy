/*E*/
/*Fichier : $Id: xzae198.c,v 1.3 2020/12/09 09:01:29 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/12/09 09:01:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae198.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les coordonnée GPS d'un PR
* avec son n° d'axe et son PR depuis la talbe LOC_GPS
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 24/06/2020 : Creation DEM 130 V1.0 
* ABE 08/12/2020 : Fix asql_clean commenté non release V1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae198.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae198.c,v 1.3 2020/12/09 09:01:29 pc2dpdy Exp $ : XZAE198" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE198_Lire_LOC_GPS : recupere les infos 
*  de coordonee en fonction du PR/axe
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern int XZAE198_Lire_LOC_GPS (     XDY_Autoroute      va_autoroute_in ,
                                        XDY_PR		 va_pr_in,
                                        char*         va_lat_out,
                                        char*     va_lon_out)



/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		va_autoroute_in
* XDY_PR		va_pr_in
*				
* PARAMETRES EN SORTIE :
* XDY_Mot      *va_lat_out
* XDY_Mot      *va_lon_out

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
* - va_pr_in, va_pr_in sont obligatoires
*

* FONCTION :
*
* Extraire les coordonnées de la talbe LOC_GPS
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae198.c,v 1.3 2020/12/09 09:01:29 pc2dpdy Exp $ : XZAE198_Lire_LOC_GPS" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE198C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE198C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE198C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
    int             vl_Nb_Col = 0;
    tg_row_result   vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE198_Lire_LOC_GPS : Debut d execution" );

	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198:va_autoroute_in=%d,va_pr_in=%d",va_autoroute_in,va_pr_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A		
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_autoroute_in,"@va_autoroute_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: asql_ctparam_tinyint_input(va_autoroute_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
     
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_pr_in,"@va_pr_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: asql_ctparam_tinyint_input(va_pr_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_lat_out;

	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_lat_out,"@va_lat_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: asql_ctparam_char_output(va_lat_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}

	i = i +1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_lon_out;

	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_lon_out,"@va_lon_out")) != CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: asql_ctparam_char_output(va_lon_out) a echoue ");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}
		
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE198: ROW RESULTS : %d", vl_Nb_Ligne_Lue);		                      
						break;

					case  CS_PARAM_RESULT:

						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAE198_Lire_LOC_GPS : PARAMETER RESULTS\n");		
						break;

					case  CS_STATUS_RESULT:

						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE198 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE198 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE198_Lire_LOC_GPS retourne 1 : %d",(int)vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}	

	strcpy(va_lat_out,  (char *)vl_Tab_Ligne[0][0].pt_value);
	strcpy(va_lon_out,  (char *)vl_Tab_Ligne[0][1].pt_value);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE198_Lire_LOC_GPS :\nautoroute=%d\nPR=%d\nLatitude=%s\nLongitude=%s",
				va_autoroute_in,
				va_pr_in,
				va_lat_out,
				va_lon_out);

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	if (vl_Nb_Ligne_Lue>0)
	{
		vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
		if (vl_retcode != XDC_OK)
		{
		   XZST_03EcritureTrace(XZSTC_WARNING," XZAE197_get_pat_radio_date_depart: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		   asql_clean_cmd(pl_connection, pl_cmd);
		   return (XDC_NOK);
		}
	}
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE198_Lire_LOC_GPS retourne %d", vl_retcode);
 	return(XDC_OK);
}




