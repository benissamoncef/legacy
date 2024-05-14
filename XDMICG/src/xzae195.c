/*E*/
/*Fichier : $Id: xzae195.c,v 1.4 2021/05/04 13:28:02 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/05/04 13:28:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae195.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a BASEDD tache tada2
* permet de test si la FMC a été retypee
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 30/11/2020 : Creation DEM-SAE130 V1.1
* ABE 08/12/2020 : Fix asql_clean commenté non release V1.2
* ABE 05/02/2021 : Ajout recuperation si route coupe sur version précedente de la fmc DEM-SAE130 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae195.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae195.c,v 1.4 2021/05/04 13:28:02 pc2dpdy Exp $ : XZAE195" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE195_Lire_FMC_Retypee : permet de test si la FMC a été retypee
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern int XZAE195_Lire_FMC_Retypee (XDY_Entier va_numero_in,
					XDY_Octet va_cle_in,
					XDY_Booleen *	va_retypage_out,
					XDY_Octet * va_type_anterieur_out,
					XDY_Booleen *	va_coupure_route_version_precedente,
					XDY_Booleen *	va_coupure_route_inverse_version_precedente)
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_numero_in
* XDY_Sens 	va_cle_in
*				
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
* va_retypage_out
* EXP..FMC_HIS_DER.type_anterieur
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*				
* - va_numero_in, va_cle_in sont obligatoires
*

* FONCTION :
*
* Permet de test si la FMC a été retypee
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzae195.c,v 1.4 2021/05/04 13:28:02 pc2dpdy Exp $ : XZAE195_Lire_FMC_Retypee" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE195C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE195C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE195C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE195_Lire_FMC_Retypee : Debut d execution" );

	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195:va_numero_in=%d, va_cle_in=%d",va_numero_in,va_cle_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
				
	/*A		
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_numero_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: asql_ctparam_int_input(va_numero_sration_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
     
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	i = 0;

    	Tab_donnees2[i].pt_value = (CS_VOID *)va_retypage_out;	
	if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_retypage_out,"@va_retypage_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao195: asql_ctparam_bit_output(va_retypage_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}


	i = i+1;

    	Tab_donnees2[i].pt_value = (CS_VOID *)va_type_anterieur_out;	

	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_type_anterieur_out,"@va_type_anterieur_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao195: asql_ctparam_smallint_output(va_type_anterieur_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	i = i+1;

    	Tab_donnees2[i].pt_value = (CS_VOID *)va_coupure_route_version_precedente;

	if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_coupure_route_version_precedente,"@va_coupure_route_version_precedente"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao195: asql_ctparam_bit_output(va_coupure_route_version_precedente) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}

	i = i+1;

    	Tab_donnees2[i].pt_value = (CS_VOID *)va_coupure_route_inverse_version_precedente;

	if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_coupure_route_inverse_version_precedente,"@va_coupure_route_inverse_version_precedente"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao195: asql_ctparam_bit_output(va_coupure_route_inverse_version_precedente) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection,pl_cmd);
	   return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE195: ROW RESULTS : %d", vl_Nb_Ligne_Lue);		                      
						break;

					case  CS_PARAM_RESULT:

						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"XZAE195_Lire_FMC_Retypee : PARAMETER RESULTS\n");		
						break;

					case  CS_STATUS_RESULT:

						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE195 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE195 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE195_Lire_FMC_Retypee retourne 1 : %d",(int)vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}	

	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE195:va_retypage_out=%d,va_type_anterieur_out=%d",*va_retypage_out,*va_type_anterieur_out);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE195_Lire_FMC_Retypee retourne %d", vl_retcode);
 	return(XDC_OK);
}




