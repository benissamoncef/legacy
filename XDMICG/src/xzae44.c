/*E*/
/*Fichier : $Id: xzae44.c,v 1.1 2008/11/17 11:01:08 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 11:01:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae44.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les infos obligatoires pour
* generer le fichier des evts strada
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae44.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae44.c	1.2 12/03/97 : xzae44" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE44_Lire_Fiche_Meteo : recupere les parametres
* de sortie de xzae;44
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE44_Lire_Fiche_Meteo (    XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
					XDY_PR		*va_PRfin_out,
					XDY_Octet	*va_perturbation_out,
					XDY_Mot	*va_visibilite_out,
					XDY_Entier	*va_longueur_out,
					XDY_Octet	*va_nature_out,
					XDY_Nom		va_libelle_nature_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_numero_in
* XDY_Octet		va_cle_in
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
* - va_numero_in, va_cle_in sont obligatoires
*

* FONCTION :
*
* Extraire les infos obligatoires sur les evts
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzae44.c	1.2 12/03/97 : XZAE44_Lire_Fiche_Meteo" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE44C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE44C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE44C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;


	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE44_Lire_Fiche_Meteo : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PRfin_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PRfin_out,"@va_PRFin_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_smallint_output(longueur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_perturbation_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_perturbation_out,"@va_Perturbation_out")) != CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_tinyint_output(VR) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
		    return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_visibilite_out;

	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_visibilite_out,"@va_Visibilite_out")) != CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_tinyint_output(VR) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
		    return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_longueur_out;

	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_longueur_out,"@va_Longueur_out")) != CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_tinyint_output(VR) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
		    return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_nature_out;

	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_nature_out,"@va_nature_out")) != CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_tinyint_output(VR) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
		    return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_libelle_nature_out;

	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_libelle_nature_out,"@va_libelle_nature_out")) != CS_SUCCEED)
	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"xzae44: asql_ctparam_tinyint_output(VR) a echoue ");
	      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
		 asql_clean_cmd(pl_connection, pl_cmd);
		    return (XDC_NOK);
	}

	i = i + 1;

        
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae44: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE44 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE44 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE44 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE44_Lire_Fiche_Meteo retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE44: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_WARNING,
			"OUT : XZAE44_Lire_Fiche_Meteo V=%d; L=%d;N=%d",
					*va_visibilite_out,
					*va_longueur_out,
					*va_perturbation_out);
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE44_Lire_Fiche_Meteo retourne %d", vl_retcode);
 	return(XDC_OK);
}



