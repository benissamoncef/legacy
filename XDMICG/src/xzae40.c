/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae40.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
* JMG	29/11/16	majeur lot 23 1.2
* JPL	04/04/19	Retour de l'indicateur d'écoulement (DEM 1326)  1.3
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae40.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzae40.c 1.4" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE40_Lire_Fiche_MC : recupere les champs generiques d'une fiche main courante 
* de sortie de XZAE40
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE40_Lire_Fiche_MC (   XDY_Horodate     va_Horodate_in,
                                    XDY_Entier      va_numero_in ,
                                    XDY_Octet       va_cle_in,                                    
        XDY_Entier   *va_Numero_out,
        XDY_Octet    *va_Cle_out,
        XDY_Octet    *va_Site_out     ,
        XDY_Octet    *va_Type_out,
        XDY_Horodate *va_DebutPrevu_out  ,
        XDY_Horodate *va_Debut_out      ,
        XDY_Horodate *va_FinPrevu_out  ,
        XDY_Horodate *va_Fin_out      ,
        XDY_Horodate *va_Validation_out,
        XDY_Entier   *va_NumCause_out   ,
        XDY_Octet    *va_CleCause_out  ,
        XDY_Mot      *va_TypeCause_out,
        XDY_Entier   *va_NumAlerte_out ,
        XDY_Horodate *va_HoroAlerte_out,
        XDY_Nom      va_TypeAlerte_out  ,
        XDY_Nom      va_Origine_out    ,
        XDY_Booleen  *va_Confirme_out,
        XDY_Octet    *va_Autoroute_out ,
        XDY_PR       *va_PR_out            ,
        XDY_Octet    *va_Sens_out    ,
        XDY_Octet    *va_PointCar_out,
        XDY_Nom      va_NomPtCar_out   ,
        XDY_Octet    *va_VR_out    ,
        XDY_Octet    *va_VM2_out  ,
        XDY_Octet    *va_VM1_out  ,
        XDY_Octet    *va_VL_out  ,
        XDY_Octet    *va_BAU_out,
        XDY_Octet    *va_VR_I_out   ,
        XDY_Octet    *va_VM2_I_out ,
        XDY_Octet    *va_VM1_I_out,
        XDY_Octet    *va_VL_I_out,
        XDY_Octet    *va_BAU_I_out ,
        XDY_Octet    *va_AutorouteCause_out ,
        XDY_PR       *va_PRCause_out        ,
        XDY_Octet    *va_SensCause_out,
        XDY_Horodate *va_HoroCause_out ,
        XDY_Octet    *va_Position_out  ,
        XDY_Octet    *va_Degats_out   ,
        XDY_Mot      *va_TypeAnterieur_out,
        XDY_Octet    *va_FausseAlerte_out,
        XDY_Octet    *va_BAU_Etroite_out   ,
        XDY_Octet    *va_BAU_I_Etroite_out ,
        XDY_Entier   *va_Longueur_out,
        XDY_Nom      va_AbrPtCar_out     ,
        XDY_Octet    *va_NumPtCar_out    ,
        XDY_Octet    *va_Degrade_out    ,                
        XDY_Entier        *va_NumEvtInit_out,
        XDY_Octet         *va_CleEvtInit_out,
        XDY_Commentaire   va_ComEvtInit_out,
        XDY_Octet         *va_TypeEvtInit_out,
        XDY_Octet         *va_AutorouteEvtInit_out,
        XDY_PR            *va_PREvtInit_out,
        XDY_Octet         *va_SensEvtInit_out,
        XDY_Horodate      *va_HoroEvtInit_out,
        XDY_Octet    *va_SiteAlerte_out,
        XDY_Octet    *va_Datex_out    ,
        XDY_Octet    *va_Trafic_out  ,
        XDY_Octet    *va_Gravite_out,
        XDY_Booleen  *va_Bloquant_out,
        XDY_Octet    *va_Duree_out,
      XDY_Octet  *va_Majeur_out,
      XDY_Octet    *va_Ecoulement_out)




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
	CS_CHAR 	*pl_rpc_name = XZAE40C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE40C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE40C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne = NULL;
   CS_DATETIME vl_Horodate;
   CS_DATETIME	vl_DebutPrevu_out;
   CS_DATETIME	vl_Debut_out;
   CS_DATETIME	vl_FinPrevu_out;
   CS_DATETIME	vl_Fin_out;
   CS_DATETIME	vl_Validation_out;
   CS_DATETIME vl_HoroAlerte_out;
   CS_DATETIME	vl_HoroCause_out;
   CS_DATETIME	vl_HoroEvtInit_out;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE40_Lire_Fiche_MC %d %d ", va_numero_in,va_cle_in);

	        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Ux2Sybase(va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

   if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in")) != CS_SUCCEED)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_input(va_Horodate_in) a echoue ");
      XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
      asql_clean_cmd(pl_connection, pl_cmd);
      return (XDC_NOK);
   }
        

   if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&va_numero_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_smallint_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	
		
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_input(va_cle_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Numero_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_Numero_out,"@va_Numero_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(numero out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Cle_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Cle_out,"@va_Cle_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(cle out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_Site_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Site_out,"@va_Site_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(site) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_Type_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Type_out,"@va_Type_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(type) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        Tab_donnees2[i].pt_value = (CS_VOID *)&vl_DebutPrevu_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_DebutPrevu_out,"@va_DebutPrevu_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate debut prevu) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Debut_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_Debut_out,"@va_Debut_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate debut) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_FinPrevu_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_FinPrevu_out,"@va_FinPrevu_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate fin prevue) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Fin_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_Fin_out,"@va_Fin_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate fin) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Validation_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_Validation_out,"@va_Validation_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate validation) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumCause_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_NumCause_out,"@va_NumCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_bit_output(cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_CleCause_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_CleCause_out,"@va_CleCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(cle cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
        i = i + 1;
        
        
        Tab_donnees2[i].pt_value = (CS_VOID *)va_TypeCause_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_TypeCause_out,"@va_TypeCause_out")) != CS_SUCCEED)
       {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_smallint_output(type cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
          
        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumAlerte_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_NumAlerte_out,"@va_NumAlerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_HoroAlerte_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_HoroAlerte_out,"@va_HoroAlerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(horodate alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_TypeAlerte_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_TypeAlerte_out,"@va_TypeAlerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(type alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Origine_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_Origine_out,"@va_Origine_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(origine) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Confirme_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_Confirme_out,"@va_Confirme_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_bit_output(confirme) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Autoroute_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Autoroute_out,"@va_Autoroute_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(autoroute) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PR_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PR_out,"@va_PR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(PR) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Sens_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Sens_out,"@va_Sens_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(sens) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PointCar_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_PointCar_out,"@va_PointCar_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(ptCar) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NomPtCar_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_NomPtCar_out,"@va_NomPtCar_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(nom ptCar) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_out,"@va_VR_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VR) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_out,"@va_VM2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VM2) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_out,"@va_VM1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VM1) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_out,"@va_VL_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VL) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_out,"@va_BAU_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(BAU) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VR_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VR_I_out,"@va_VR_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VR_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM2_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM2_I_out,"@va_VM2_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VM2_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VM1_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VM1_I_out,"@va_VM1_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VM1_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_VL_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_VL_I_out,"@va_VL_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(VL_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_out,"@va_BAU_I_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(BAU_I) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_AutorouteCause_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_AutorouteCause_out,"@va_AutorouteCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(Auto cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PRCause_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PRCause_out,"@va_PRCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(PR cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        i = i + 1;

        Tab_donnees2[i].pt_value = (CS_VOID *)va_SensCause_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_SensCause_out,"@va_SensCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(sens cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_HoroCause_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_HoroCause_out,"@va_HoroCause_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(Horodate cause) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Position_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Position_out,"@va_Position_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(position) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Degats_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Degats_out,"@va_Degats_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(degats) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_TypeAnterieur_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_TypeAnterieur_out,"@va_TypeAnterieur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_smallint_output(type anterieur) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_FausseAlerte_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_FausseAlerte_out,"@va_FausseAlerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(fausse alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_Etroite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_Etroite_out,"@va_BAU_Etroite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(BAU etroite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_BAU_I_Etroite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_BAU_I_Etroite_out,"@va_BAU_I_Etroite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(BAU_I etroite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
      i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Longueur_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_Longueur_out,"@va_Longueur_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(va_Longueur_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_AbrPtCar_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_AbrPtCar_out,"@va_AbrPtCar_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(abrev PtCar) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumPtCar_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_NumPtCar_out,"@va_NumPtCar_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(numero PtCar) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Degrade_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Degrade_out,"@va_Degrade_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(degrade) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_NumEvtInit_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_NumEvtInit_out,"@va_NumEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(va_NumEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_CleEvtInit_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_CleEvtInit_out,"@va_CleEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(va_CleEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_ComEvtInit_out;

        if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)va_ComEvtInit_out,"@va_ComEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_char_output(va_ComEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_TypeEvtInit_out;

        if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)va_TypeEvtInit_out,"@va_TypeEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_smallint_output(va_TypeEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_AutorouteEvtInit_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_AutorouteEvtInit_out,"@va_AutorouteEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(va_AutorouteEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_PREvtInit_out;

        if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)va_PREvtInit_out,"@va_PREvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_int_output(va_PREvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_SensEvtInit_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_SensEvtInit_out,"@va_SensEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(va_SensEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_HoroEvtInit_out;

        if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_HoroEvtInit_out,"@va_HoroEvtInit_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_date_output(va_HoroEvtInit_out) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_SiteAlerte_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_SiteAlerte_out,"@va_SiteAlerte_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(site alerte) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Datex_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Datex_out,"@va_Datex_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(datex) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Trafic_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Trafic_out,"@va_Trafic_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(trafic) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        


        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Gravite_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Gravite_out,"@va_Gravite_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(gravite) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Bloquant_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_Bloquant_out,"@va_Bloquant_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_bit_output(bloquant) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        

        i = i + 1;
        
        
	Tab_donnees2[i].pt_value = (CS_VOID *)va_Duree_out;

        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Duree_out,"@va_Duree_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(duree) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }
        
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_Majeur_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Majeur_out, "@va_Majeur_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(majeur) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_Ecoulement_out;
        if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)va_Ecoulement_out,"@va_Ecoulement_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: asql_ctparam_tinyint_output(ecoulement) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

   XZST_03EcritureTrace(XZSTC_WARNING,"i=%d",i);

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE40: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE40 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE40 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE40 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE40_Lire_Fiche_MC retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE40: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	/*A
	** Conversion des dates du format Sybase au format Unix
	*/

	if ((vl_retcode = asql_date_Sybase2Ux(vl_DebutPrevu_out,va_DebutPrevu_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_DebutPrevu_out,va_DebutPrevu_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_date_Sybase2Ux(vl_Debut_out,va_Debut_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_Debut_out,va_Debut_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_date_Sybase2Ux(vl_FinPrevu_out,va_FinPrevu_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_FinPrevu_out,va_FinPrevu_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
	
   	if ((vl_retcode = asql_date_Sybase2Ux(vl_Fin_out,va_Fin_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_Fin_out,va_Fin_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
   	if ((vl_retcode = asql_date_Sybase2Ux(vl_Validation_out,va_Validation_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_Validation_out,va_Validation_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
   	if ((vl_retcode = asql_date_Sybase2Ux(vl_HoroAlerte_out,va_HoroAlerte_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_HoroAlerte_out,va_HoroAlerte_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
   	if ((vl_retcode = asql_date_Sybase2Ux(vl_HoroCause_out,va_HoroCause_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_HoroCause_out,va_HoroCause_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}
   	if ((vl_retcode = asql_date_Sybase2Ux(vl_HoroEvtInit_out,va_HoroEvtInit_out))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae40: asql_date_Sybase2Ux(vl_HoroEvtInit_out,va_HoroEvtInit_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return (XDC_NOK);
	}

	/*XZST_03EcritureTrace(	XZSTC_FONCTION,
			"OUT : XZAE40_Lire_Fiche_MC :\nnumero=%d\ncle=%d\nsite=%d\ntype:%d\nnumcause=%d\nclecause=%d\ntypecause=%d\nnumalerte=%d\ntypealerte=%s\norigine=%s\nconfirme=%d\nautoroute=%d\npr=%d\nsens=%d\npointcar=%d\nnomptcar=%s\nvr=%d\nvm2=%d\nvm1=%d\nvl=%d\nbau=%d\nvr_i=%d\nvm2_i=%d\nvm1_i=%d\nvl_i=%d\nbau_i=%d\nautoroutecause=%d\nprcause=%d\nsenscause=%d\nposition=%d\ndegats=%d\ntypeanterieur=%d\nfaussealerte=%d\nbau_etroite=%d\nbau_i_etroite=%d\nabrptcar=%s\nnumptcar=%d\ndegrade=%d\nsitealerte=%d\ndatex=%d\ntrafic=%d\ngravite=%d\nbloquant=%d\nduree=%d\nmajeur=%d\necoulement=%d",
        *va_Numero_out,
        *va_Cle_out,
        *va_Site_out     ,
        *va_Type_out,
        *va_NumCause_out   ,
        *va_CleCause_out  ,
        *va_TypeCause_out,
        *va_NumAlerte_out ,
        //va_TypeAlerte_out  ,
        "test",
        //va_Origine_out    ,
        "test2",
        *va_Confirme_out,
        *va_Autoroute_out ,
        *va_PR_out            ,
        *va_Sens_out    ,
        *va_PointCar_out,
        //va_NomPtCar_out   ,
        "Test3",
        *va_VR_out    ,
        *va_VM2_out  ,
        *va_VM1_out  ,
        *va_VL_out  ,
        *va_BAU_out,
        *va_VR_I_out   ,
        *va_VM2_I_out ,
        *va_VM1_I_out,
        *va_VL_I_out,
        *va_BAU_I_out ,
        *va_AutorouteCause_out ,
        *va_PRCause_out        ,
        *va_SensCause_out,
        *va_Position_out  ,
        *va_Degats_out   ,
        *va_TypeAnterieur_out,
        *va_FausseAlerte_out,
        *va_BAU_Etroite_out   ,
        *va_BAU_I_Etroite_out ,
        //va_AbrPtCar_out     ,
        "test4",
        *va_NumPtCar_out    ,
        *va_Degrade_out    ,
        *va_SiteAlerte_out,
        *va_Datex_out    ,
        *va_Trafic_out  ,
        *va_Gravite_out,
        *va_Bloquant_out,
        *va_Duree_out ,
	*va_Majeur_out,
	*va_Ecoulement_out);*/

   XZST_03EcritureTrace(	XZSTC_FONCTION,"print");
	strcpy(va_TypeAlerte_out, (char *)vl_Tab_Ligne[0][0].pt_value);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_TypeAlerte_out);
	strcpy(va_Origine_out, (char *)vl_Tab_Ligne[0][1].pt_value);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_Origine_out);
	strcpy(va_NomPtCar_out, (char *)vl_Tab_Ligne[0][2].pt_value);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_NomPtCar_out);
	strcpy(va_AbrPtCar_out, (char *)vl_Tab_Ligne[0][3].pt_value);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_AbrPtCar_out );

   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Numero_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Cle_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Site_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Type_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_NumCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_CleCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_TypeCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_NumAlerte_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_TypeAlerte_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_Origine_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Confirme_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Autoroute_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_PR_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Sens_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_PointCar_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_NomPtCar_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VR_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VM2_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VM1_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VL_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_BAU_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VR_I_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VM2_I_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VM1_I_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_VL_I_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_BAU_I_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_AutorouteCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_PRCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_SensCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Position_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Degats_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_TypeAnterieur_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_FausseAlerte_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_BAU_Etroite_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_BAU_I_Etroite_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%s",va_AbrPtCar_out );
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_NumPtCar_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Degrade_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_SiteAlerte_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Datex_out );
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Trafic_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Gravite_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Bloquant_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Duree_out );
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Majeur_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%d",*va_Ecoulement_out);

   XZST_03EcritureTrace(	XZSTC_FONCTION,"horo:");
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_Debut_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_DebutPrevu_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_FinPrevu_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_Fin_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_Validation_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_HoroAlerte_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_HoroCause_out);
   XZST_03EcritureTrace(	XZSTC_FONCTION,"%lf",*va_HoroEvtInit_out);
	
	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

 	/*A
	   Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE40_Lire_Fiche_MC retourne %d", vl_retcode);
 	return(XDC_OK);
}
