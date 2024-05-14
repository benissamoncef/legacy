/*E*/
/*Fichier : $Id: xzae101.c,v 1.6 2010/12/09 00:18:24 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2010/12/09 00:18:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE101 * FICHIER xzae101.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* xzae101.c est un client qui permet de faire l'interface
* entre le serveur de donnees du pc simplifie et une
* fonction utilisatrice.
* xzae101.c interroge donc le serveur de donnes en 
* executant la SP xzae101sp.prc qui retourne la liste des
* fmc locales. Cette liste doit ensuite etre retournee
* a la fonction utilisatrice.
*
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron  	27 Aou 1996	: Creation
* vathelot	04 sep 1996	: remplissage
* vathelot	06 sep 1996	: correction v1.3
* corinne	04 dec 1996	: Modif interface de sortie (680) v1.4
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae101.h"

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE;101 : Liste des FMC Locales pour un site
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE101_Liste_FMC_Locales( XDY_Nom va_NomSite, XDY_FonctionInt va_FonctionUtilisateur)

/*
*
* PARAMETRES EN ENTREE : aucun
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*	Appelle XZAE;101 et pour chaque ligne retournee appelle va_FonctionUtilisateur
*
------------------------------------------------------*/

{
	static char *version 		= "$Id: xzae101.c,v 1.6 2010/12/09 00:18:24 gesconf Exp $ : XZAE101_Liste_FMC_Locales" ;	
	int	vl_retcode 		= XDC_OK;
	
	CS_CHAR *rpc_name = XZAE101C_XZAE101_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2 = XZAE101C_XZAE101_NB_PARAM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	
	/*A
	** Definition des variables locales
	*/	
		
	XZIST_FMC_LOC	vl_Tab_FMC_Locales;
		
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : XZAE101_Liste_FMC_Locales : Debut d execution ");
	
			
   	/*A
        **  test les parametres d'entree
        */

	if ( va_FonctionUtilisateur == NULL)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur est nulle");
	      return (XDC_NOK);
	}

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_Locales retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoi de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_locales retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,va_NomSite,"@va_District_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: asql_ctparam_char_input(district) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_Locales retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}
        
		
	
	/*A
	** Envoi de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_Locales retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
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
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae101: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae101 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae101 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae101 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_locales retourne 1");
 						   asql_clean_cmd(pl_connection, pl_cmd);
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
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_Locales retourne %d", vl_retcode);
						asql_clean_cmd(pl_connection, pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_Locales retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_locales retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateur
	*/

	if (vl_Nb_Ligne_Lue != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzae101: vl_Nb_Ligne_Lue:%d",vl_Nb_Ligne_Lue );
		for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
		{

  			/*A
  			** Conversion des dates du format Sybase au format Unix
  			*/
  		
  			if ((vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][3].pt_value,&vl_Tab_FMC_Locales.horodate))!= XDC_OK)
  			{
  				XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: asql_date_Sybase2Ux(horodate) a echoue");
  				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101 retourne %d", vl_retcode);
  				asql_clean_cmd(pl_connection, pl_cmd);
  				return (XDC_NOK);
  			}
	
  			/*A
  			** Conversion des dates du format Sybase au format Unix
  			*/
  		
  			if ((vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][5].pt_value,&vl_Tab_FMC_Locales.debut))!= XDC_OK)
  			{
  				XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: asql_date_Sybase2Ux(debut) a echoue");
  				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101 retourne %d", vl_retcode);
  				asql_clean_cmd(pl_connection, pl_cmd);
  				return (XDC_NOK);
  			}
	
  			/*A
  			** Conversion des dates du format Sybase au format Unix
  			*/
  		
  			if ((vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][6].pt_value,&vl_Tab_FMC_Locales.fin))!= XDC_OK)
  			{
  				XZST_03EcritureTrace(XZSTC_WARNING,"xzae101: asql_date_Sybase2Ux(fin) a echoue");
  				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101 retourne %d", vl_retcode);
  				asql_clean_cmd(pl_connection, pl_cmd);
  				return (XDC_NOK);
  			}
	
		  	/*B
	           	** Recuperation du select de la procedure stockee XZAE;101
	          	 */
	           
	          	vl_Tab_FMC_Locales.numero 	= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value;
	           	vl_Tab_FMC_Locales.cle	 	= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
	           	vl_Tab_FMC_Locales.type	 	=  (XDY_Octet) (* (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value);
	           	strcpy(vl_Tab_FMC_Locales.localisation,* (XDY_Nom *) vl_Tab_Ligne[i][4].pt_value);
	           	vl_Tab_FMC_Locales.operation	=  (XDY_Booleen) (* (XDY_Booleen *) vl_Tab_Ligne[i][7].pt_value);
			vl_Tab_FMC_Locales.datex	=  (XDY_Booleen) (* (XDY_Booleen *) vl_Tab_Ligne[i][8].pt_value);

	           
	        	/*B
		 	** Appel de la fonction utilisateur FonctionUtilisateur
		 	*/
		  
			if (( vl_retcode = (* va_FonctionUtilisateur) ( vl_Tab_FMC_Locales, vl_Nb_Ligne_Lue ) ) != XDC_OK )
			{
		        	XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_FonctionUtilisateur s'est mal deroule");
		        	asql_clean_cmd(pl_connection, pl_cmd);
				asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
				return (XDC_NOK);
			}	
		}	
		   
	}
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (vl_retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(pl_connection, pl_cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE101_Liste_FMC_locales retourne %d", vl_retcode);
 	asql_clean_cmd(pl_connection, pl_cmd);
	return(XDC_OK);
}
