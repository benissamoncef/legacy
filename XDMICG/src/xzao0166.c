/*E*/
/*Fichier :  $Id: xzao0166.c,v 1.1 2008/02/13 13:24:45 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:45 $
------------------------------------------------------
* GTIE *  LongueurITOROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0166.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ         27 Sep 1994     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao0166.c    1.1 12/11/07 : xzao0166" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO0166SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO0166_Ajouter_GTC_NICE_RENOVEE ( XDY_Basedd                va_Basedd_in,
                                        XZAOT_ConfGTCNICERENOVEE  va_GTC_NICE_in,
                                        XDY_ResConf               *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomMachine	va_NomMachine_in
* XDY_Mot		va_LongueurITOR_in
* XDY_Mot		va_AdresseITOR_in
* XDY_Mot		va_LongueurETOR_in
* XDY_Mot		va_AdresseETOR_in
* XDY_Mot		va_LongTelealarme_in
* XDY_Mot		va_AdresseTelealarme_in
* XDY_Mot		va_LongTelecomEch_in
* XDY_Mot		va_AdresseTelecomEch_in
* XDY_Mot		va_LongTelecomTub_in
* XDY_Mot		va_AdresseTelecomTub_in
* XDY_Mot		va_LongTelesignEch_in
* XDY_Mot		va_AdresseTelesignEch_in
* XDY_Mot		va_LongTelesignTub_in
* XDY_Mot		va_AdresseTelesignTub_in
* XDY_Mot		va_LongSeqEch_in
* XDY_Mot		va_AdresseSeqEch_in
* XDY_Mot		va_LongSeqTub_in
* XDY_Mot		va_AdresseSeqTub_in
* XDY_Mot		va_LongAlerteTub_in
* XDY_Mot		va_AdresseAlerteTub_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une GTC NICE en base
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans la table EQT_NIX (GTC_NICE RENOVEE)
*
* MECANISMES :
*
*------------------------------------------------------*/

{

    static char *version = "@(#)xzao0166.c    1.1 12/11/07 : XZAO0166_Ajouter_GTC_NICE_RENOVEE" ;
    
    CS_CHAR             *rpc_name = XZAOC_XZAO0166_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection = XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO0166_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    /*A
    ** Definition des variables locales
    */
    
	XDY_NomEqt	vl_Nom;
	XDY_NomMachine	vl_NomMachine;
	XDY_Mot		vl_LongueurITOR;
	XDY_Mot		vl_AdresseITOR;
	XDY_Mot		vl_LongueurETOR;
	XDY_Mot		vl_AdresseETOR;
	XDY_Mot		vl_LongTelealarme;
	XDY_Mot		vl_AdresseTelealarme;
	XDY_Mot		vl_LongTelecomEch;
	XDY_Mot		vl_AdresseTelecomEch;
	XDY_Mot		vl_LongTelecomTub;
	XDY_Mot		vl_AdresseTelecomTub;
	XDY_Mot		vl_LongTelesignEch;
	XDY_Mot		vl_AdresseTelesignEch;
	XDY_Mot		vl_LongTelesignTub;
	XDY_Mot		vl_AdresseTelesignTub;
	XDY_Mot		vl_LongSeqEch;
	XDY_Mot		vl_AdresseSeqEch;
	XDY_Mot		vl_LongSeqTub;
	XDY_Mot		vl_AdresseSeqTub;
	XDY_Mot		vl_LongAlerteTub;
	XDY_Mot		vl_AdresseAlerteTub;

    /*A
    ** Definition des variables locales
    */

	strcpy( vl_Nom,         va_GTC_NICE_in.Nom );
	strcpy( vl_NomMachine,  va_GTC_NICE_in.NomMachine );
	vl_LongueurITOR		  = 0;
	vl_AdresseITOR		  = 0;
	vl_LongueurETOR 	  = 0;
	vl_AdresseETOR		  = 0;
	vl_LongTelealarme	  = 0;
	vl_AdresseTelealarme  = 0;
	vl_LongTelecomEch 	  = 0;
	vl_AdresseTelecomEch  = 0;
	vl_LongTelecomTub 	  = 0;
	vl_AdresseTelecomTub  = 0;
	vl_LongTelesignEch 	  = 0;
	vl_AdresseTelesignEch = 0;
	vl_LongTelesignTub 	  = 0;
	vl_AdresseTelesignTub = 0;
	vl_LongSeqEch 		  = 0;
	vl_AdresseSeqEch 	  = 0;
	vl_LongSeqTub 	 	  = 0;
	vl_AdresseSeqTub 	  = 0;
	vl_LongAlerteTub 	  = 0;
	vl_AdresseAlerteTub   = 0;
	

            
    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
    
    
    
    
    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: asql_ctparam_char_input() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Nom,"@va_Nom_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: asql_ctparam_char_input(Nom) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomMachine,"@va_NomMachine_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: asql_ctparam_char_input(NomMachine) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongueurITOR,"@va_LongueurITOR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongueurITOR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseITOR,"@va_AdresseITOR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseITOR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongueurETOR,"@va_LongueurETOR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongueurETOR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseETOR,"@va_AdresseETOR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseETOR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongTelealarme,"@va_LongTelealarme_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongTelealarme) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelealarme,"@va_AdresseTelealarme_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseTelealarme) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongTelecomEch,"@va_LongTelecomEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongTelecomEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelecomEch,"@va_AdresseTelecomEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseTelecomEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongTelecomTub,"@va_LongTelecomTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongTelecomTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelecomTub,"@va_AdresseTelecomTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseTelecomTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongTelesignEch,"@va_LongTelesignEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongTelesignEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelesignEch,"@va_AdresseTelesignEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseTelesignEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongTelesignTub,"@va_LongTelesignTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongTelesignTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelesignTub,"@va_AdresseTelesignTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseTelesignTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongSeqEch,"@va_LongSeqEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongSeqEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseSeqEch,"@va_AdresseSeqEch_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseSeqEch) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongSeqTub,"@va_LongSeqTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongSeqTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseSeqTub,"@va_AdresseSeqTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseSeqTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_LongAlerteTub,"@va_LongAlerteTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(LongAlerteTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseAlerteTub,"@va_AdresseAlerteTub_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao166: asql_ctparam_smallint_input(AdresseAlerteTub) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: asql_ctparam_int_output(Resultat) a echoue "); 
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande au Sql server
    */

    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Traitement du resultat
    */

    while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
    {
        switch ((int)res_type)
        {
            case CS_ROW_RESULT:
            case CS_PARAM_RESULT:
            case CS_STATUS_RESULT:
                /*B
                ** Impression de l entete en fonction du type de resultat.
                */
                switch ((int)res_type)
                {
                    case  CS_ROW_RESULT:
                        retcode = asql_fetch_data (cmd);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0166: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }

    
                break;


            case CS_MSG_RESULT:
                /*
                **
                */
                retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                    
                XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao0166 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao0166: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0166_Ajouter_GTC_NICE_RENOVEE retourne %d", retcode);
    asql_clean_cmd(connection, cmd);
    free(Tab_donnees2);
    return(XDC_OK);
}




