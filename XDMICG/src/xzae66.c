/*E*/
/*Fichier : $Id: xzae66.c,v 1.1 2017/03/31 17:32:31 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:32:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae66.c
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
* JMG	21/06/16	: creation SAGA 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae66.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzae66.c,v 1.1 2017/03/31 17:32:31 pc2dpdy Exp $ : xzae66" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE66_Lire_Config_Voies : recupere les parametres
* de sortie de xzae;66
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE66_Lire_Config_Voies (    XDY_Autoroute      va_NumAutoroute_in ,
                                        XDY_PR       va_PR_in,
					XDY_Octet	va_Sens_in,
					XDY_Octet	va_PtCaracteristique_in,
					XDY_Octet	va_TypePtCaracteristique_in,
					XDY_Octet	va_Position_in,
					XDY_Mot		va_Type_in,
                                        XDY_Autoroute       *va_NumAutoroute_out,
					XDY_PR		*va_PR_out,
					XDY_Octet	*va_Sens_out,
					XDY_Octet	*va_PtCaracteristique_out,
					XDY_Octet	*va_TypePtCaracteristique_out,
					XDY_Octet	*va_Position_out,
					XDY_Octet	*va_VR_out,
					XDY_Octet	*va_VM2_out,
					XDY_Octet	*va_VM1_out,
					XDY_Octet	*va_VL_out,
					XDY_Octet	*va_BAU_out,
					XDY_Octet	*va_VR_I_out,
					XDY_Octet	*va_VM2_I_out,
					XDY_Octet	*va_VM1_I_out,
					XDY_Octet	*va_VL_I_out,
					XDY_Octet	*va_BAU_I_out,
					XDY_Octet	*va_BAU_Etroite_out,
					XDY_Octet	*va_BAU_I_Etroite_out,
					XDY_Octet	*va_District_out,
					XDY_Octet	*va_DistrictRecouvr_out,
					int		*va_Resultat_out)



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
	static char *version = "$Id: xzae66.c,v 1.1 2017/03/31 17:32:31 pc2dpdy Exp $ : XZAE66_Lire_Config_Voies" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE66C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE66C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE66C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE66_Lire_Config_Voies : Debut d execution");
	XZST_03EcritureTrace( XZSTC_INTERFACE,"va_NumAutoroute_in:%d,va_PR_in:%d,va_Sens_in:%d,va_PtCaracteristique_in:%d,va_TypePtCaracteristique_in:%d,va_Position_in:%d,va_Type_in:%d"
											,va_NumAutoroute_in,va_PR_in,va_Sens_in,va_PtCaracteristique_in,va_TypePtCaracteristique_in,va_Position_in,va_Type_in);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_NumAutoroute_in,"@va_NumAutoroute_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_date_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_PtCaracteristique_in,"@va_PtCaracteristique_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_TypePtCaracteristique_in,"@va_TypePtCaracteristique_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_Position_in,"@va_Position_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}

        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&va_Type_in,"@va_Type_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}


	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumAutoroute_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_NumAutoroute_out,"@va_NumAutoroute_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_char_output(vehicules) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PR_out,"@va_PR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_char_output(PL) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_Sens_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Sens_out,"@va_Sens_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_char_output(cars) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_PtCaracteristique_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_PtCaracteristique_out,"@va_PtCaracteristique_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_char_output(cars) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_TypePtCaracteristique_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_TypePtCaracteristique_out,"@va_TypePtCaracteristique_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Position_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Position_out,"@va_Position_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_int_output(morts) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_out,"@va_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_bit_output(matiere) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_out,"@va_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_bit_output(veh escota) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_out,"@va_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_bit_output(homme escota) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_out,"@va_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_bit_output(personnalite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
          
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_out,"@va_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_I_out,"@va_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_I_out,"@va_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_I_out,"@va_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_I_out,"@va_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_out,"@va_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_Etroite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_Etroite_out,"@va_BAU_Etroite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_Etroite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_Etroite_out,"@va_BAU_I_Etroite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_District_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_District_out,"@va_District_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_DistrictRecouvr_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_DistrictRecouvr_out,"@va_DistrictRecouvr_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Resultat_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae66: asql_ctparam_date_output(blesses_graves) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae66: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE66 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE66 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE66 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE66_Lire_Config_Voies retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE66: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE66_Lire_Config_Voies retourne %d", vl_retcode);
 	return(XDC_OK);
}



