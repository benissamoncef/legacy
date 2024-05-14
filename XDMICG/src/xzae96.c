/*E*/
/*Fichier : $Id: xzae96.c,v 1.1 2016/09/29 15:29:43 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:29:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae96.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les infos obligatoires pour
* generer le fichier des evts strada
*
------------------------------------------------------
* HISTORIQUE :
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae96.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae96.c	1.2 12/03/97 : xzae96" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE96_Ecrire_Fiche_Vehicule : recupere les parametres
* de sortie de xzae;96
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE96_Ecrire_Fiche_Vehicule (       XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Octet       va_Type_in,
                                        XDY_Octet       va_Passagers_in,
										XDY_Booleen		va_Bebe_in,
										XDY_Booleen		va_PersAgee_in,
                                        XDY_Booleen     va_Animaux_in,
                                        XDY_Booleen     va_VehEscota_in,
                                        XDY_Octet       va_Chargement_in,
                                        char            *va_CodeMat_in,
										XDY_Entier		va_Depannage_in,
										XDY_District	va_SiteDepannage_in,
                                        char            *va_Texte_in,
                                        XDY_Mot         va_NumeroVehicule_in,
										XDY_Eqt			va_Poste_operateur_in,
										XDY_Texte       va_marque_in,
										XDY_Texte		va_type_in,
										XDY_Texte		va_couleur_in,
										XDY_Texte		va_immatriculation_in,
										XDY_Mot			va_code_pays_in,
										XDY_Texte		va_pays_in,
										XDY_Texte		va_nom_in,
										XDY_Texte		va_ville_in,
										XDY_Texte		va_adresse_in,
										XDY_Texte		va_code_danger_in,
										XDY_Mot			va_code_nature_panne_in,
										XDY_Texte		va_nature_panne_in,
										XDY_Texte		va_positionnement_in,
										XDY_Octet		va_vehicule_roulant_in,
										XDY_Texte		va_enlev_dem_par_in,
										XDY_Entier		va_numero_fiche_appel_in,
										XDY_Octet		va_pres_mat_dangereuse_in,
										XDY_Octet		va_fax_envoye_in,
										XDY_Mot			va_code_marque_in,
										XDY_Mot			va_code_couleur_in,
										XDY_Mot			va_code_enlev_demande_par_in,
										XDY_Entier		va_sit_in,
										XDY_Octet		va_site_creation_in,
                                        XDY_Mot         va_nb_bebes_in,
                                        XDY_Mot         va_nb_agees_in,										
										XDY_Mot			va_flag_association_in,
										XDY_Texte		va_id_ava_in,
										XDY_Entier		va_dangerosite_in
										)

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
	static char *version = "@(#)xzae96.c	1.2 12/03/97 : XZAE96_Ecrire_Fiche_Vehicule" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE96C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE96C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE96C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_DATETIME     va_Horodate_in;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE96_Ecrire_Fiche_Vehicule : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvenement_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvenement_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Type_in,"@va_Type_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Passagers_in,"@va_Passagers_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_VL_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_bit_input(pl_cmd,(CS_BIT *)&va_Bebe_in,"@va_Bebe_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_Bebe_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_bit_input(pl_cmd,(CS_BIT *)&va_PersAgee_in,"@va_PersAgee_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_PL_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(pl_cmd,(CS_BIT *)&va_Animaux_in,"@va_Animaux_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_PL_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(pl_cmd,(CS_BIT *)&va_VehEscota_in,"@va_VehEscota_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_PL_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Chargement_in,"@va_Chargement_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_date_input(va_Remorque_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
/*CODEMAT*/
	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_CodeMat_in,"@va_CodeMat_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_emetteur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

    if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_Depannage_in,"@va_Depannage_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_int_input(va_Depannage_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_SiteDepannage_in,"@va_SiteDepannage_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_SiteDepannage_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

/*TEXTE*/
	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_Texte_in,"@va_Texte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_emetteur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_NumeroVehicule_in,"@va_NumeroVehicule_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_NumeroVehicule_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Poste_operateur_in,"@va_PosteOperateur_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_PosteOperateur_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_marque_in,"@va_marque_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_emetteur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_type_in,"@va_type_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_type_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_couleur_in,"@va_couleur_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_couleur_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_immatriculation_in,"@va_immatriculation_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_immatriculation_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_code_pays_in,"@va_code_pays_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_code_pays_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_pays_in,"@va_pays_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_pays_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_nom_in,"@va_nom_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_nom_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_ville_in,"@va_ville_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_ville_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_adresse_in,"@va_adresse_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_ville_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_code_danger_in,"@va_code_danger_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_ville_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_code_nature_panne_in,"@va_code_nature_panne_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_code_pays_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_positionnement_in,"@va_positionnement_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_positionnement_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_vehicule_roulant_in,"@va_vehicule_roulant_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_vehicule_roulant_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_enlev_dem_par_in,"@va_enlev_dem_par_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(@va_enlev_dem_par_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection,pl_cmd);
		return (XDC_NOK);
	}

    if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_fiche_appel_in,"@va_numero_fiche_appel_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_numero_fiche_appel_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_pres_mat_dangereuse_in,"@va_pres_mat_dangereuse_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_pres_mat_dangereuse_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_fax_envoye_in,"@va_fax_envoye_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_fax_envoye_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_code_marque_in,"@va_code_marque_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_code_marque_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_code_couleur_in,"@va_code_couleur_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_code_couleur_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_code_enlev_demande_par_in,"@va_code_enlev_demande_par_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_code_enlev_demande_par_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

    if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_sit_in,"@va_sit_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_smallint_input(va_sit_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_site_creation_in,"@va_site_creation_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_site_creation_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_nb_bebes_in,"@va_nb_bebes_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_nb_bebes_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_nb_agees_in,"@va_nb_agees_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_nb_agees_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_flag_association_in,"@va_flag_association_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_tinyint_input(va_flag_association_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE96: va_id_ava_in = [%s]",va_id_ava_in);
	if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_id_ava_in,"@va_id_ava_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_char_input(va_id_ava_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAE96: va_dangerosite_in = [%d]",va_dangerosite_in);
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_dangerosite_in,"@va_dangerosite_in"))!= CS_SUCCEED)
	{
    	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: asql_ctparam_int_input(va_dangerosite_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae96: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE96 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE96 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE96 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE96: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE96_Ecrire_Fiche_Vehicule");
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE96_Ecrire_Fiche_Vehicule retourne %d", vl_retcode);
 	return(XDC_OK);
}



