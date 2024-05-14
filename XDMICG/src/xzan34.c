/*E*/
/*Fichier : $Id: xzan34.c,v 1.4 1998/01/12 16:06:23 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/01/12 16:06:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN34 * FICHIER xzan34.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module interface Annuaire 
*********************************
*  XZAN34_Epurer_Calendrier_Astreinte : est appele par un callback de tacli
*  sur reception de message XDM_AN_PURGE sur le dg XDG_AN_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAN34SP
*********************************  
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	04 Nov 1994	: Creation
********************************
* MERCIER	10 Nov 1994	: Modification de		V 1.2
*			la gestion de la commande des Open Client
*		    et encapsulation des fonctions Sybase.
********************************
* VOLCIC	30 Nov 1994	: Ajout free(Tab_donnees2)	V 1.3
*			
********************************
********************************
* MERCIER	10 Nov 1994	: Modification de		V 1.2
*			la gestion de la commande des Open Client
*		    et encapsulation des fonctions Sybase.
********************************
* Guilhou	12 jan 1998	: correction free pour hp-ux 10.20 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzan34.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzan34.c,v 1.4 1998/01/12 16:06:23 gesconf Exp $ : xzan34" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAN34_Epurer_Calendrier_Astreinte : est appele par un callback de tacli
*  sur reception de message XDM_AN_PURGE sur le dg XDG_AN_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAN34SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAN34_Epurer_Calendrier_Astreinte ()

/*
*
* PARAMETRES EN ENTREE :
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
* FONCTION :
*
* Purge le calendrier des astreintes 
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzan34.c,v 1.4 1998/01/12 16:06:23 gesconf Exp $ : XZAN34_Epurer_Calendrier_Astreinte" ;
	
	CS_CHAR 	*pl_rpc_name = XZAN34C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XDY_Entier	vl_SPstatus;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAN34_Epurer_Calendrier_Astreinte : Debut d execution" );
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
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
						vl_retcode = asql_fetch_data (pl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN34: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN34 : PARAMETER RESULTS\n");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN34 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAN34 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
 					return ( XDC_OK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAN34: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_OK );
				}
	}

	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAN34_Epurer_Calendrier_Astreinte retourne %d", vl_retcode);
	return(XDC_OK);
}




