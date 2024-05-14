/*E*/
/*Fichier : $Id: xzae49.c,v 1.4 2020/06/02 21:21:58 devgfi Exp $      $Revision: 1.4 $        $Date: 2020/06/02 21:21:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae49.c
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
* VR	07/10/11	Ajout Vitesse		(DEM/1015)
* JMG	08/03/18	ajout personnel 1.3
* JPL	06/09/19	Ajout bouchon (mobile pour Basculement pour travaux) (DEM 1346)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae49.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae49.c	1.2 12/03/97 : xzae49" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  XZAE49_Lire_Fiche_Travaux : recupere les parametres
* de sortie de xzae49
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE49_Lire_Fiche_Travaux (    XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Booleen     *va_nature_out,
                                        XDY_Octet      *va_type_out,
                                        XDY_Booleen     *va_continu_out,
                                        XDY_Horodate    *va_horodebc_out,
                                        XDY_Horodate    *va_horofinc_out,
                                        XDY_Booleen       *va_mobile_out,
                                        XDY_Autoroute       *va_autoroute_out,
                                        XDY_PR       *va_prtetem_out,
                                        XDY_PR       *va_prqueuem_out,
                                        XDY_Sens       *va_sensm_out,
                                        XDY_PR       *va_prtete_out,
                                        XDY_Booleen       *va_voiesret_out,
                                        XDY_Entier       *va_longueur_out,
                                        XDY_Entier       *va_longueurm_out,
                                        XDY_Octet       *va_VR_out,
                                        XDY_Octet       *va_VM2_out,
                                        XDY_Octet       *va_VM1_out,
                                        XDY_Octet       *va_VL_out,
                                        XDY_Octet       *va_BAU_out,
                                        XDY_Octet       *va_VR_I_out,
                                        XDY_Octet       *va_VM2_I_out,
                                        XDY_Octet       *va_VM1_I_out,
                                        XDY_Octet       *va_VL_I_out,
                                        XDY_Octet       *va_BAU_I_out,
                                        XDY_PR          *va_prdebut_vr_out,
                                        XDY_PR          *va_prfin_vr_out,
                                        XDY_Horodate    *va_debut_vr_out,
                                        XDY_Horodate    *va_fin_vr_out,
                                        XDY_PR          *va_prdebut_vm2_out,
                                        XDY_PR          *va_prfin_vm2_out,
                                        XDY_Horodate    *va_debut_vm2_out,
                                        XDY_Horodate    *va_fin_vm2_out,
                                        XDY_PR          *va_prdebut_vm1_out,
                                        XDY_PR          *va_prfin_vm1_out,
                                        XDY_Horodate    *va_debut_vm1_out,
                                        XDY_Horodate    *va_fin_vm1_out,
                                        XDY_PR          *va_prdebut_vl_out,
                                        XDY_PR          *va_prfin_vl_out,
                                        XDY_Horodate    *va_debut_vl_out,
                                        XDY_Horodate    *va_fin_vl_out,
                                        XDY_PR          *va_prdebut_bau_out,
                                        XDY_PR          *va_prfin_bau_out,
                                        XDY_Horodate    *va_debut_bau_out,
                                        XDY_Horodate    *va_fin_bau_out,
                                        XDY_PR          *va_prdebut_vr_i_out,
                                        XDY_PR          *va_prfin_vr_i_out,
                                        XDY_Horodate    *va_debut_vr_i_out,
                                        XDY_Horodate    *va_fin_vr_i_out,
                                        XDY_PR          *va_prdebut_vm2_i_out,
                                        XDY_PR          *va_prfin_vm2_i_out,
                                        XDY_Horodate    *va_debut_vm2_i_out,
                                        XDY_Horodate    *va_fin_vm2_i_out,
                                        XDY_PR          *va_prdebut_vm1_i_out,
                                        XDY_PR          *va_prfin_vm1_i_out,
                                        XDY_Horodate    *va_debut_vm1_i_out,
                                        XDY_Horodate    *va_fin_vm1_i_out,
                                        XDY_PR          *va_prdebut_vl_i_out,
                                        XDY_PR          *va_prfin_vl_i_out,
                                        XDY_Horodate    *va_debut_vl_i_out,
                                        XDY_Horodate    *va_fin_vl_i_out,
                                        XDY_PR          *va_prdebut_bau_i_out,
                                        XDY_PR          *va_prfin_bau_i_out,
                                        XDY_Horodate    *va_debut_bau_i_out,
                                        XDY_Horodate    *va_fin_bau_i_out,
					XDY_Entier	*va_vitesse_out,
					XDY_Octet	*va_personnel_out,
					XDY_Octet	*va_bouchon_out)


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
	static char *version = "@(#)xzae49.c	1.2 12/03/97 : XZAE49_Lire_Fiche_Travaux" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE49C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE49C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE49C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_TINYINT	vl_vit;

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_nature_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_nature_out,"@va_Nature_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_smallint_output(longueur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_type_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_type_out,"@va_Type_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_int_output(longueur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_continu_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_continu_out,"@va_Continu_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VR) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_horodebc_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_horodebc_out,"@va_HoroDebC_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VM2) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_horofinc_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_horofinc_out,"@va_HoroFinC_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VM1) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_mobile_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_mobile_out,"@va_Mobile_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VL) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_autoroute_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_autoroute_out,"@va_Autoroute_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prtetem_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prtetem_out,"@va_PRteteM_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VR_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prqueuem_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prqueuem_out,"@va_PRqueueM_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VM2_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_sensm_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_I_out,"@va_SensM_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VM1_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prtete_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prtete_out,"@va_PRtete_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(VL_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_voiesret_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_voiesret_out,"@va_VoiesRet_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_longueur_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_longueur_out,"@va_Longueur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_longueurm_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_longueurm_out,"@va_LongueurM_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_out,"@va_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_out,"@va_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_out,"@va_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_out,"@va_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_out,"@va_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_I_out,"@va_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_I_out,"@va_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_I_out,"@va_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_I_out,"@va_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_out,"@va_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vr_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vr_out,"@va_PRdebut_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vr_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vr_out,"@va_PRfin_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vr_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vr_out,"@va_debut_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vr_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vr_out,"@va_fin_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vm2_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vm2_out,"@va_PRdebut_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vm2_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vm2_out,"@va_PRfin_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vm2_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vm2_out,"@va_debut_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vm2_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vm2_out,"@va_fin_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vm1_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vm1_out,"@va_PRdebut_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vm1_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vm1_out,"@va_PRfin_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vm1_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vm1_out,"@va_debut_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vm1_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vm1_out,"@va_fin_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vl_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vl_out,"@va_PRdebut_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vl_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vl_out,"@va_PRfin_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vl_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vl_out,"@va_debut_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vl_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vl_out,"@va_fin_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_bau_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_bau_out,"@va_PRdebut_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_bau_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_bau_out,"@va_PRfin_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_bau_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_bau_out,"@va_debut_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_bau_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_bau_out,"@va_fin_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vr_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vr_i_out,"@va_PRdebut_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vr_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vr_i_out,"@va_PRfin_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vr_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vr_i_out,"@va_debut_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vr_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vr_i_out,"@va_fin_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vm2_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vm2_i_out,"@va_PRdebut_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vm2_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vm2_i_out,"@va_PRfin_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vm2_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vm2_i_out,"@va_debut_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vm2_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vm2_i_out,"@va_fin_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vm1_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vm1_i_out,"@va_PRdebut_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vm1_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vm1_i_out,"@va_PRfin_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vm1_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vm1_i_out,"@va_debut_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vm1_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vm1_i_out,"@va_fin_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_vl_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_vl_i_out,"@va_PRdebut_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_vl_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_vl_i_out,"@va_PRfin_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_vl_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_vl_i_out,"@va_debut_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_vl_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_vl_i_out,"@va_fin_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prdebut_bau_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prdebut_bau_i_out,"@va_PRdebut_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_prfin_bau_i_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_prfin_bau_i_out,"@va_PRfin_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_debut_bau_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_debut_bau_i_out,"@va_debut_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_fin_bau_i_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)va_fin_bau_i_out,"@va_fin_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


	i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)&vl_vit;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)&vl_vit,"@va_vitesse_out")) != CS_SUCCEED)
        {
          
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);

           return (XDC_NOK);
        }

	i = i + 1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_personnel_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_personnel_out,"@va_personnel_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(personnel) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	i = i + 1;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_bouchon_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_bouchon_out,"@va_bouchon_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae49: asql_ctparam_tinyint_output(bouchon) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	/*A
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae49: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE49 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE49 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE49 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE49_Lire_Fiche_Travaux retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE49: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	

	
	*va_vitesse_out=vl_vit;
	XZST_03EcritureTrace(	XZSTC_WARNING,
			"OUT : XZAE49_Lire_Fiche_Travaux %d",*va_vitesse_out);
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE49_Lire_Fiche_Travaux retourne %d", vl_retcode);
 	return(XDC_OK);
}



