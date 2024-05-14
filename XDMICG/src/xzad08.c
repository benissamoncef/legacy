/*E*/
/*Fichier : $Id: xzad08.c,v 1.1 1997/05/15 09:55:05 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 09:55:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAD08 * FICHIER xzad08.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tacli
* permet de mettre en base les temps de parcours 6mn
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	09 fev 1997	: Creation (dem/tdp)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzad08.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzad08.c,v 1.1 1997/05/15 09:55:05 gaborit Exp $ : xzad08" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD08_Liste_Franchissements : recupere la liste de 
* franchissement pour un site ,une fmc et horodate donn≈s.
*  Appelle XZAD08SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD08_Liste_Franchissements (	
					XDY_FonctionInt va_fct_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_FonctionInt va_fct_in

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
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzad08.c,v 1.1 1997/05/15 09:55:05 gaborit Exp $ : XZAD08_Liste_Franchissements" ;
	
	CS_CHAR 	*pl_rpc_name = XZAD08C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAD08C_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	tg_row_result 	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	
	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	XDY_Horodate	vl_HorodateFin;
	
	XDY_Eqt		vl_zdp;
	int 		vl_indice;
	XDY_Booleen	vl_Bool;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAD08_Liste_Franchissements : Debut d execution" );

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao208: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD08 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD08 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAD08 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAD08_Liste_Franchissements retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAD08: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	if(vl_SPstatus == XDC_OK)
	
	{
		for (vl_indice = 0 ; vl_indice<vl_Nb_Ligne_Lue;vl_indice++)
		{
			vl_zdp = * (XDY_Mot   *) vl_Tab_Ligne[vl_indice][0].pt_value;
			
	   	if ( va_fct_in == NULL)
	   	{
	      		XZST_03EcritureTrace(XZSTC_FATAL,"va_fct_in est nulle");
	      		asql_clean_cmd(pl_connection, pl_cmd);
	      		return (XDC_NOK);
	   	}
	   	else
	   	{
	      		if ( ( vl_retcode = (*va_fct_in) ( vl_zdp ) ) != XDC_OK )
	      		{
	         		XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_fct_in s'est mal deroule");
	         		asql_clean_cmd(pl_connection, pl_cmd);
		 		return (XDC_NOK);
	      		}
	   	}	
	   
			
		}	
	}
	/*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */

        if ((vl_retcode = asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne)) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
        }
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAD08_Liste_Franchissements_Haut retourne %d", vl_retcode);
 	return(XDC_OK);
}



