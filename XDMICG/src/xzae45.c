/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae45.c
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
* LCL	28 Aou 2008	Creation  DEM/835
* ABE	10/09/2020	Correction variable de sortie char sur curseur pour tagen/tada2 DEM-SAE130 V1.2
* ABE 22/11/21 Fix cas ou le plan d'action est vide DEM-SAE130
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae45.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae45.c 1.4" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE45_Lire_Fiche_Vehicule : recupere les parametres
* de sortie de xzae45
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE45_Lire_Fiche_Vehicule ( XDY_Entier    va_numero_in ,
                                        XDY_Octet     va_cle_in,
                                        XDY_Octet    *va_Type_out,
                                        XDY_Octet    *va_passagers_out,
                                        XDY_Booleen  *va_bebe_out,
                                        XDY_Booleen  *va_persAgee_out,
                                        XDY_Booleen  *va_animaux_out,
                                        XDY_Booleen  *va_type_veh_escota_out,
                                        XDY_Octet    *va_chargement_out,
                                        XDY_Nom      va_codemat_out,
                                        XDY_Entier   *va_depannage_out,
                                        XDY_Octet    *va_site_depannage_out,
                                        XDY_Nom         va_texte_out,
                                        XDY_Mot      *va_numveh_out,
                                        XDY_Nom         va_marque_out,
                                        XDY_Nom         va_type_out,
                                        XDY_Nom         va_couleur_out,
                                        XDY_Nom         va_immatriculation_out,
                                        XDY_Mot      *va_code_pays_out,
                                        XDY_Nom         va_pays_out,
                                        XDY_Nom         va_nom_out,
                                        XDY_Nom         va_ville_out,
                                        XDY_Nom         va_adresse_out,
                                        XDY_Nom         va_code_danger_out,
                                        XDY_Mot      *va_code_nature_panne_out,
                                        XDY_Nom         va_nature_panne_out,
                                        XDY_Nom         va_positionnement_out,
                                        XDY_Octet    *va_vehicule_roulant_out,
                                        XDY_Nom         va_enlev_dem_par_out,
                                        XDY_Entier   *va_numero_fiche_appel_out,
                                        XDY_Octet    *va_pres_mat_dangereuse_out,
                                        XDY_Octet    *va_fax_envoye_out,
                                        XDY_Mot      *va_code_marque_out,
                                        XDY_Mot      *va_code_couleur_out,
                                        XDY_Mot      *va_code_enlev_dem_par_out,
                                        XDY_Octet    *va_site_creation_out,
                                        XDY_Mot      *va_nb_bebes_out,
                                        XDY_Mot      *va_nb_agees_out)


/*
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
	CS_CHAR 	*pl_rpc_name = XZAE45C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE45C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE45C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE45_Lire_Fiche_Vehicule : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Type_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Type_out,"@va_Type_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(type) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_passagers_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_passagers_out,"@va_Passagers_ou")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(passagers) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_bebe_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_bebe_out,"@va_Bebe_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_bit_output(bebes) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_persAgee_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_persAgee_out,"@va_PersAgee_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_bit_output(personnes agees) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_animaux_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_animaux_out,"@va_Animaux_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_bit_output(animaux) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_type_veh_escota_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_type_veh_escota_out,"@va_TypeVehEscota_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_bit_output(vehicule ESCOTA) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_chargement_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_chargement_out,"@va_Chargement_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(chargement) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_codemat_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_codemat_out,"@va_CodeMat_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(code matiere dang) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_depannage_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_depannage_out,"@va_Depannage_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_int_output(depannage) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_site_depannage_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_site_depannage_out,"@va_SiteDepannage_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(site depannage) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_texte_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_texte_out,"@va_Texte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(commentaire) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_numveh_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_numveh_out,"@va_numveh_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(numero vehicule) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_marque_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_marque_out,"@va_marque_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(marque) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_type_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_type_out,"@va_type_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(type vehicule) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_couleur_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_couleur_out,"@va_couleur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(couleur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_immatriculation_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_immatriculation_out,"@va_immatriculation_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(immatriculation) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_pays_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_code_pays_out,"@va_code_pays_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(code pays) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_pays_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_pays_out,"@va_pays_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(pays) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_nom_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_nom_out,"@va_nom_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(nom proprietaire) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_ville_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_ville_out,"@va_ville_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(ville) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_adresse_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_adresse_out,"@va_adresse_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(adresse) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_danger_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_code_danger_out,"@va_code_danger_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(code danger) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_nature_panne_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_code_nature_panne_out,"@va_code_nature_panne_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(code panne) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_nature_panne_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_nature_panne_out,"@va_nature_panne_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(nature panne) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_positionnement_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_positionnement_out,"@va_positionnement_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(positionnement) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_vehicule_roulant_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_vehicule_roulant_out,"@va_vehicule_roulant_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(vehicule roulant) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_enlev_dem_par_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_enlev_dem_par_out,"@va_enlev_dem_par_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_char_output(demandeur enlevement) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_numero_fiche_appel_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_numero_fiche_appel_out,"@va_numero_fiche_appel_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_int_output(numero fiche appel) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_pres_mat_dangereuse_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_pres_mat_dangereuse_out,"@va_pres_mat_dangereuse_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(presence matiere dang) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fax_envoye_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_fax_envoye_out,"@va_fax_envoye_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(fax envoye) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_marque_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_code_marque_out,"@va_code_marque_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(code marque) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_couleur_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_code_couleur_out,"@va_code_couleur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(code couleur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_code_enlev_dem_par_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_code_enlev_dem_par_out,"@va_code_enlev_dem_par_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(code demandeur enlevement) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_site_creation_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_site_creation_out,"@va_site_creation_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_tinyint_output(site creation) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

       i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_nb_bebes_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_nb_bebes_out,"@va_nb_bebes_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(nb bebes) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


       i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_nb_agees_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_nb_agees_out,"@va_nb_agees_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae45: asql_ctparam_smallint_output(nb personnes agees) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae45: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE45 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE45 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE45 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE45_Lire_Fiche_Vehicule retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE45: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
/*	
	XZST_03EcritureTrace(	XZSTC_WARNING,
			"OUT : XZAE45_Lire_Fiche_Vehicule :\npassagers=%d\nbebe=%d\npersonne agee=%d\nanimaux=%d\ntype vehicule escota=%d\nchargement=%d\ncodemat=%s\ndepannage=%d\nsite depannage=%d\ntexte=%s\nnumveh=%d\nmarque=%s\ntype=%s\ncouleur=%s\nimmatriculation=%s\ncode_payse=%d\npays=%s\nnom=%s\nville=%s\nadresse=%s\ncode_danger=%s\ncode_nature_panne=%d\nnature_panne=%s\npositionnement=%s\nvehicule_roulant=%d\nenlev_dem_par=%s\nnumero_fiche_appel=%d\npres_mat_dangereuse=%d\nfax_envoye=%d\ncode_marque=%d\ncode_couleur=%d\ncode_enlev_dem_par=%d\nsite_creation=%d\nnb_bebes=%d\nnb_agees=%d\n",
                                        *va_Type_out,
                                        *va_passagers_out,
                                        *va_bebe_out,
                                        *va_persAgee_out,
                                        *va_animaux_out,
                                        *va_type_veh_escota_out,
                                        *va_chargement_out,
                                        va_codemat_out,
                                        *va_depannage_out,
                                        *va_site_depannage_out,
                                        va_texte_out,
                                        *va_numveh_out,
                                        va_marque_out,
                                        va_type_out,
                                        va_couleur_out,
                                        va_immatriculation_out,
                                        *va_code_pays_out,
                                        va_pays_out,
                                        va_nom_out,
                                        va_ville_out,
                                        va_adresse_out,
                                        va_code_danger_out,
                                        *va_code_nature_panne_out,
                                        va_nature_panne_out,
                                        va_positionnement_out,
                                        *va_vehicule_roulant_out,
                                        va_enlev_dem_par_out,
                                        *va_numero_fiche_appel_out,
                                        *va_pres_mat_dangereuse_out,
                                        *va_fax_envoye_out,
                                        *va_code_marque_out,
                                        *va_code_couleur_out,
                                        *va_code_enlev_dem_par_out,
                                        *va_site_creation_out,
                                        *va_nb_bebes_out,
                                        *va_nb_agees_out);
*/

	//Récupération de la varialbe de sortie en char ave cle curseur		
   if (vl_Nb_Ligne_Lue>0)
	{
	   strcpy(va_codemat_out,  (char *)vl_Tab_Ligne[0][0].pt_value);
   }


	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE45_Lire_Fiche_Vehicule retourne %d", vl_retcode);
 	return(XDC_OK);
}




