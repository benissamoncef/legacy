/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae41.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
* JPL	10/11/11 : Ajout parametres conditions d'accident a l'interface proc. stockee (DEM 1006)  1.2
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae41.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae41.c 1.3" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE41_Lire_Fiche_Accident : recupere les parametres
* de sortie de XZAE41
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE41_Lire_Fiche_Accident (    XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Octet       *va_vehicules_out,
                                        XDY_Octet       *va_PL_out,
                                        XDY_Octet       *va_cars_out,
                                        XDY_Octet       *va_rem_out,
                                        XDY_Octet       *va_motos_out,
                                        XDY_Octet       *va_blessesLegers_out,
                                        XDY_Octet       *va_blessesGraves_out,
                                        XDY_Octet       *va_morts_out,
                                        XDY_Booleen     *va_matieres_out,
                                        XDY_Booleen     *va_veh_escota_out,
                                        XDY_Booleen     *va_homme_escota_out,
                                        XDY_Booleen     *va_perso_out,
					XDY_Booleen	*va_degats_out)



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
	CS_CHAR 	*pl_rpc_name = XZAE41C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE41C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE41C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;

	XDY_Mot		vl_cond_meteo;
	XDY_Mot		vl_cond_visibilite;
	XDY_Mot		vl_cond_etat_chaussee;
	XDY_Mot		vl_cond_chantier;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE41_Lire_Fiche_Accident : Debut d execution %d %d ",
					va_numero_in,va_cle_in);

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: asql_ctparam_int_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_vehicules_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_vehicules_out,"@va_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(vehicules) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PL_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_PL_out,"@va_PL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(PL) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_rem_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_rem_out,"@va_Remorque_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(rem) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_cars_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_cars_out,"@va_Car_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(cars) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_motos_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_motos_out,"@va_Moto_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(moto) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_blessesLegers_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_blessesLegers_out,"@va_BlesseLeg_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(blessesLegers) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_blessesGraves_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_blessesGraves_out,"@va_BlesseGra_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(blessesGraves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_morts_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_morts_out,"@va_Mort_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_tinyint_output(morts) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_veh_escota_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_veh_escota_out,"@va_VehEscota_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_bit_output(veh escota) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_matieres_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_matieres_out,"@va_MatDang_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_bit_output(matiere) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_homme_escota_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_homme_escota_out,"@va_HomEscota_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_bit_output(homme escota) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_perso_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_perso_out,"@va_Personalite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_bit_output(personnalite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
          
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_degats_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_degats_out,"@va_DegatsDom_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_bit_output(degats) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *) &vl_cond_meteo;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *) &vl_cond_meteo,"@va_Cond_Meteo_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_smallint_output(cond_meteo) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *) &vl_cond_visibilite;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *) &vl_cond_visibilite,"@va_Cond_Visibilite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_smallint_output(cond_visibilite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *) &vl_cond_etat_chaussee;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *) &vl_cond_etat_chaussee,"@va_Cond_Etat_Chau_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_smallint_output(cond_etat_chaussee) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *) &vl_cond_chantier;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *) &vl_cond_chantier,"@va_Cond_Chantier_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae41: asql_ctparam_smallint_output(cond_chantier) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae41: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE41 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE41 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE41 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE41_Lire_Fiche_Accident retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE41: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"OUT : XZAE41_Lire_Fiche_Accident :\nvehicules=%d\ncars=%d\nblesses=%d\nmorts=%d\nmatieres=%d\nveh escota=%d\nhomme escota=%d\nperso=%d\nnblesses graves=%d",
			*va_vehicules_out,
			*va_cars_out,
			*va_blessesLegers_out,
			*va_morts_out,
			*va_matieres_out,
			*va_veh_escota_out,
			*va_homme_escota_out,
			*va_perso_out,
			*va_blessesGraves_out);
	
	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE41_Lire_Fiche_Accident retourne %d", vl_retcode);
 	return(XDC_OK);
}



