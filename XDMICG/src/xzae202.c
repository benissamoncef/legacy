/*E*/
/*Fichier : $Id: xzae202.c,v 1.2 2020/12/09 09:01:36 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/12/09 09:01:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae202.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tada2
* permet de récuperer le type et la cause pour un numero et une cle
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 21/10/2020 : Creation SAE-DEM130 V1.1
* ABE 08/12/2020 : Fix asql_clean commenté non release V1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae202.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae202.c,v 1.2 2020/12/09 09:01:36 pc2dpdy Exp $ : XZAE202" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE202_Get_FMC_Type : recupere le type et la cause d'une FMC pour un numero et une cle
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern int XZAE202_Get_FMC_Type(XDY_Entier      va_numero_in ,
                                XDY_Octet       va_cle_in,
				XDY_Entier	*va_numero_cause_out,
				XDY_Octet	*va_cle_cause_out,
                                XDY_Octet       *va_type_out)


/*
*
* PARAMETRES EN ENTREE :
* XDY_Entier      va_numero_in
* XDY_Octet       va_cle_in	
*
*				
* PARAMETRES EN SORTIE :
* XDY_Mot         *va_type_out

* VALEUR RENDUE :
* EXP..FMC_GEN.Type
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* 2     XDC_ARG_INV
*
* CONDITIONS D'UTILISATION :
*
*
* FONCTION :
*
*  XZAE202_Get_FMC_Type : recupere le type de FMC et la FMC cause pour un numero et cle
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae202.c,v 1.2 2020/12/09 09:01:36 pc2dpdy Exp $ : XZAE202_Get_FMC_Type" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE202C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE202C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE202C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE202_Get_FMC_Type : Debut d execution" );

	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202:va_numero_in=%d, va_cle_in=%d",va_numero_in,va_cle_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A		
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_numero_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_numero_cause_out;
	  if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_numero_cause_out,"@va_numero_cause_out"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: asql_ctparam_smallint_input(va_numero_cause_out) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	i = i + 1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_cle_cause_out;	
	
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_cle_cause_out,"@va_cle_cause_out"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: asql_ctparam_tinyint_input(va_cle_cause_out) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	i = i + 1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_type_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_type_out,"@va_type_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae202: asql_ctparam_tinyint_output(type) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE202: ROW RESULTS : %d", vl_Nb_Ligne_Lue);		                      
						break;

					case  CS_PARAM_RESULT:

						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAE202_Get_FMC_Type : PARAMETER RESULTS\n");		
						break;

					case  CS_STATUS_RESULT:

						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE202 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE202 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE202_Get_FMC_Type retourne 1 : %d",(int)vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE202: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}	

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
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE202_Get_FMC_Type retourne %d", vl_retcode);
 	return(XDC_OK);
}



				
