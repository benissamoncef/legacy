/*E*/
/* Fichier : $Id: xzae159.c,v 1.1 2012/07/04 10:30:40 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae159.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	20/01/22	: Creation DEM-SAE367
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae159.h"
static char *version = "@(#)xzae159.c	1.2 1.2 07/03/07 : xzae159.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle xzae159SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzae159_Rechercher_Veh_dans_rayon (	XDY_NomAuto   	va_NomAutoroute_in ,
                                        XDY_PR		    va_pr_in,
				                        XDY_Sens		va_sens_in,
                                        XDY_Entier      va_rayon_in,
                                        XDY_FonctionInt pa_FonctionUtilisateur_in,
						                XDY_Entier 	    *va_Resultat_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute   va_autoroute_in
* XDY_PR		  va_pr_in
* XDY_Sens		  va_sens_in
* XDY_Entier      va_rayon_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier 	va_Resultat_out
*
* VALEUR RENDUE :
*
* 
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
	CS_CHAR *rpc_name = XZAE159C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;
	int 		vl_cr=0;

	/*A
	** Definition des variables locales
	*/
	
	XDY_ZDP	vl_Eqt;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : xzae159_Rechercher_Veh_dans_rayon localisation=%s-%u-%d, rayon=%d",va_NomAutoroute_in, va_sens_in, va_pr_in, va_rayon_in);
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
		
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomAutoroute_in,"@va_NomAutoroute_in"))!= CS_SUCCEED)
	{
       XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: asql_ctparam_char_input(va_NomAutoroute_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_pr_in,"@va_PR_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: asql_ctparam_int_input(va_PR_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: asql_ctparam_tinyint_input(va_Sens_in) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
  	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
    }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_rayon_in,"@va_Rayon_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: asql_ctparam_int_input(va_Rayon_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae159: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae159 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae159 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae159 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d",SPstatus);
 						   asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae159: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae159: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae159: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur pour chaque UGTP 
	*/
		
	XZST_03EcritureTrace(XZSTC_INFO,"xzae159 : nb de FMC concernee = %d",vl_Nb_Ligne_Lue);

	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
        /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( pa_FonctionUtilisateur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateur_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateur_in) ( 
					* (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value //NumEvt
					,* (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value //CleEvt
					,* (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value //TypeEvt
					,* (XDY_PR *) vl_Tab_Ligne[i][3].pt_value //Pr
					,* (XDY_NomAuto *) vl_Tab_Ligne[i][4].pt_value //Nom autoroute
					,* (XDY_Sens *) vl_Tab_Ligne[i][5].pt_value //Sens
					, &vl_cr ) ) != XDC_OK ) 
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return (XDC_NOK);
	      }
	      *va_Resultat_out = *va_Resultat_out || vl_cr;
	   }	
	   
	}
	
	if (vl_Nb_Ligne_Lue>0)
	  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : xzae159_Rechercher_Veh_dans_rayon retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




