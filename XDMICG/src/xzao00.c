/*E*/
/*Fichier : $Id: xzao00.c,v 1.1 2023/10/03 11:43:07 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2023/10/03 11:43:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao00.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao15.h
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL   03/10/23    : Creation pour CNA
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao00.h"

static char *version = "$Id: xzao00.c,v 1.0 2023/10/03 11:43:07 gesconf Exp $	: xzao00" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*  
* Recherche le "numero" PMV non inhibe en amont (>0) ou
* en aval (<0)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO00_SitePrincipal(XDY_Mot *pa_Site_out, 
				         XDY_NomSite pa_CodeSite_out)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*  pa_Site_out
*  pa_CodeSite_out
*
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* INTERFACE 
* XZAO00 select dans CFG..RES_DIS
* XZAO00 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR *rpc_name =XZAOC_XZAO00_RPC_NAME;
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2=XZAOC_XZAO00_NB_PARM_RETURN;
	tg_Colonne_donnees2 pl_Tab_donnees2[XZAOC_XZAO00_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue=0;
	int		vl_Nb_Col=0;
	tg_row_result 	vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAO00_SitePrincipal : N/A");
			
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal sortie", vl_retcode);
		
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

    pl_Tab_donnees2[0].pt_value = (CS_VOID *)pa_Site_out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)pa_Site_out,"@va_site_out")) != CS_SUCCEED)	

	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: asql_ctparam_smallint_output() a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	pl_Tab_donnees2[1].pt_value = (CS_VOID *)pa_CodeSite_out;
	
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)pa_CodeSite_out,"@va_nom_site_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: asql_ctparam_char_output(AbrevSortie) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", XDC_NOK );
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
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
						
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao10: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao10 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao00 : CS_STATUS_RESULT = %ld", vl_retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAO00_SitePrincipal : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);

                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne 1");
						   asql_clean_cmd (pl_connection,pl_cmd);
	   					   return (XDC_NOK);
                                                }
                                                break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao00 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
                                       
                                	asql_clean_cmd (pl_connection,pl_cmd);
					return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
	   				
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao00 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao00 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao00 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return(XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao00: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					
					return (XDC_NOK);
				}
		}
	
	/*A
	** Desallocation de le strucuture de commande ou drop de la commande si 
	** quelque chose s est mal passe.
	*/
	asql_clean_cmd (pl_connection,pl_cmd);
	   				
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO00_SitePrincipal retourne %d", vl_retcode);
	return(XDC_OK);
}




