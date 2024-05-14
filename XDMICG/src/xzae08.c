/*E*/
/*Fichier : $Id: xzae08.c,v 1.3 2019/01/29 11:24:16 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/01/29 11:24:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae08.c
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

#include "xzae08.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae08.c	1.2 12/03/97 : xzae08" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE08_Creer_Fiche : recupere les parametres
* de sortie de xzae;08
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE08_Creer_Fiche ( XDY_Octet       va_TypeEvenement_in ,
                                        XDY_Octet       va_MachineOperateur_in,
                                        XDY_Booleen     va_Prevision_ATraiter_in,
                                        XDY_Horodate    va_HoroDebut_in,
                                        XDY_Horodate    va_HoroDebutPrevu_in,
                                        XDY_Octet       va_Site_creation_in,
                                        XDY_Octet       va_Degrade_in,
                                        XDY_Entier      *va_NumEvenement_out,
                                        XDY_Octet       *va_CleEvenement_out,
                                        XDY_Entier      *va_NumFicheAppel_out,
					int		va_NumAlerte_in,
					XDY_Octet	va_SiteAlerte_in)
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
	static char *version = "@(#)xzae08.c	1.2 12/03/97 : XZAE08_Creer_Fiche" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE08C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE08C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE08C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_DATETIME     va_Horodate;


	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE08_Creer_Fiche : Debut d execution" );
	XZST_03EcritureTrace( XZSTC_INTERFACE,"va_TypeEvenement_in  %d, va_MachineOperateur_in %d, va_Prevision_ATraiter_in %d, va_HoroDebut_in %lf, va_HoroDebutPrevu_in %lf, va_Site_creation_in %d, va_Degrade_in %d, va_NumAlerte_in %d, va_SiteAlerte_in %d",
			va_TypeEvenement_in,va_MachineOperateur_in,va_Prevision_ATraiter_in,va_HoroDebut_in,va_HoroDebutPrevu_in,va_Site_creation_in,va_Degrade_in,va_NumAlerte_in,va_SiteAlerte_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_TypeEvenement_in,"@va_TypeEvenement_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_MachineOperateur_in,"@va_MachineOperateur_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_bit_input(pl_cmd,(CS_BIT *)&va_Prevision_ATraiter_in,"@va_Prevision_ATraiter_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
        if ((vl_retcode = asql_date_Ux2Sybase(va_HoroDebut_in,&va_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_date_Ux2Sybase(va_Horodate, &va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d ", vl_retcode);
                return (XDC_NOK);
        }
	if (va_HoroDebut_in !=-1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroDebut_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_ctparam_date_input(va_Horodate_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
        if ((vl_retcode = asql_date_Ux2Sybase(va_HoroDebutPrevu_in,&va_Horodate))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_date_Ux2Sybase(va_Horodate, &va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d ", vl_retcode);
                return (XDC_NOK);
        }
	if (va_HoroDebutPrevu_in != -1) {
        if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&va_Horodate,"@va_HoroDebutPrevu_in")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_ctparam_date_input(va_Horodate_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Site_creation_in,"@va_Site_creation_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Degrade_in,"@va_Degrade_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumEvenement_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_NumEvenement_out,"@va_NumEvenement_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae08: asql_ctparam_smallint_output(longueur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_CleEvenement_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_CleEvenement_out,"@va_CleEvenement_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae08: asql_ctparam_int_output(niveau) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_NumFicheAppel_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_NumFicheAppel_out,"@va_NumFicheAppel_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae08: asql_ctparam_tinyint_output(VR) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_NumAlerte_in,"@va_NumAlerte_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_NumAlerte_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_SiteAlerte_in,"@va_SiteAlerte_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae08: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE08 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE08 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE08 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE08_Creer_Fiche retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE08: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE08_Creer_Fiche :\nva_NumEvenement_out=%d\nva_CleEvenement_out=%d\nficheappel=%d",
			*va_NumEvenement_out,*va_CleEvenement_out,*va_NumFicheAppel_out);
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE08_Creer_Fiche retourne %d", vl_retcode);
 	return(XDC_OK);
}



