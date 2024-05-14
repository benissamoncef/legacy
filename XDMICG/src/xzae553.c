/*E*/
/*Fichier : @(#)xzae553.c	1.1      Release : 1.1        Date : 01/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae553.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  01/04/2004 :  Creation (SAGA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae553.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae553.c	1.1 02/26/99 : xzae553" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE553_Creer_Evenement_DATEX :
*  Cree un evennement DATEX associe a une FMC ESCOTA
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE553_Creer_Evenement_DATEX (char *va_emetteur_in,
   				   char *va_reference_in,
				   XDY_Entier  va_evenement_in,
				   XDY_Octet   va_cle_in)
/*
*
* PARAMETRES EN ENTREE :
* char *va_emetteur_in         code du partenaire qui a emis le message
* char *va_reference_in        reference datex associee
* XDY_Entier va_evenement_in   numero de la FMC associee
* XDY_Octet  va_cle_in         cle de la FMC associee
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
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzae553.c	1.1 02/26/99 : XZAE553_Creer_Evenement_DATEX" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE553C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int             vl_num_Tab_donnees2 = XZAE553C_NB_PARM_RETURN;
	XDY_Entier      vl_SPstatus;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	CS_DATETIME     vl_Horodate;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE553_Creer_Evenement_DATEX : Debut d execution \nva_emetteur_in:%s, va_reference_in:%s",
	va_emetteur_in,va_reference_in);

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_emetteur_in,"@va_emetteur_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: asql_ctparam_char_input(@va_emetteur_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_reference_in,"@va_reference_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: asql_ctparam_char_input(@va_reference_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_evenement_in,"@va_evenement_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: asql_ctparam_int_input(@va_evenement_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: asql_ctparam_tinyint_input(@va_cle_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection,pl_cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE553 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE553 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE553 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE553 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE553_Creer_Evenement_DATEX retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE553: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}


	XZST_03EcritureTrace(	XZSTC_FONCTION, "OUT : XZAE553_Creer_Evenement_DATEX");
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE553_Creer_Evenement_DATEX retourne %d", vl_retcode);
 	return(XDC_OK);
}



