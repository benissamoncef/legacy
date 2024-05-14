/*E*/
/*Fichier :  $Id: xzao1006.c,v 1.3 2008/10/14 15:44:54 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2008/10/14 15:44:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1006.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       28 Nov 2007     : Creation DEM 662
* JBL      13 Oct 2008      : Ajout PRInfluence
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao1006.c,v 1.3 2008/10/14 15:44:54 devgtie Exp $ : xzao1006" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO1006SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO1006_Ajouter_LTN ( XDY_Basedd        va_Basedd_in,
                           XZAOT_ConfLTNG    va_LTN_in,
                           XDY_ResConf       *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_NomEqt        va_NomLtn_in
* XDY_NomAuto       va_Autoroute_in
* XDY_PR            va_PR_in
* XDY_PR            va_PRInfluence_in
* XDY_Sens          va_Sens_in
* XDY_Mot           va_AdresseEtat_in
* XDY_Mot           va_AdresseCommande_in
* XDY_Mot           va_AdresseDiscordance_in
* XDY_Mot           va_AdresseTelealarme_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute d'un locaux techniques en base
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
* Insertion ou modification dans la table EQT_LTN (EQUIPEMENT_LOCAL_TECHNIQUE)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao1006.c,v 1.3 2008/10/14 15:44:54 devgtie Exp $ : XZAO1006_Ajouter_LTN" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO1006_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO1006_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    
    /*A
    ** Definition des variables locales
    */
    
    XDY_NomEqt      vl_NomLtn;
    XDY_NomAuto     vl_Autoroute;
    XDY_NomEqt      vl_NomServeur;
    XDY_PR          vl_PR;
    XDY_PR          vl_PRInfluence;
    XDY_Sens        vl_Sens;
    XDY_Mot         vl_AdresseEtat;
    XDY_Mot         vl_AdresseCommande;
    XDY_Mot         vl_AdresseDiscordance;
    XDY_Mot         vl_AdresseTelealarme;
    
       
    /*A
    ** Definition des variables locales
    */
    
    strcpy( vl_NomLtn,        va_LTN_in.NomLtn );
    strcpy( vl_Autoroute,     va_LTN_in.Autoroute );
    strcpy( vl_NomServeur,    va_LTN_in.NomServeur );
    vl_PR                   = va_LTN_in.PR;
    vl_PR                   = va_LTN_in.PRInfluence;
    vl_Sens                 = va_LTN_in.Sens;
    vl_AdresseEtat          = va_LTN_in.AdresseEtat;
    vl_AdresseCommande      = va_LTN_in.AdresseCommande;
    vl_AdresseDiscordance   = va_LTN_in.AdresseDiscordance;
    vl_AdresseTelealarme    = va_LTN_in.AdresseTelealarme;
    
    
    
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO1006_Ajouter_LTN : Nom = %s\tAutoroute = %s\tPR = %d\tPRInfluence = %d\tSens = %d\tAdresseEtat = %d\tAdresseCommande = %d\tAdresseDisc. = %d\t AdresseTeleal.= %d\n",
                          va_LTN_in.NomLtn,
                          va_LTN_in.Autoroute,
                          va_LTN_in.NomServeur,
                          va_LTN_in.PR,
                          va_LTN_in.PRInfluence,
                          va_LTN_in.Sens,
                          va_LTN_in.AdresseEtat,
                          va_LTN_in.AdresseCommande,
                          va_LTN_in.AdresseDiscordance,
                          va_LTN_in.AdresseTelealarme );
            
            
    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
    
    
    
    
    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
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
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_char_input() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
        
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomLtn,"@va_NomLtn_in"))!= CS_SUCCEED)
    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_char_input(NomLtn) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_char_input(Autoroute) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
        
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_char_input(NomServeur) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_int_input(PR) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRInfluence,"@va_PRInfluence_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_int_input(PRInfluence) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_tinyint_input(Sens) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseEtat,"@va_AdresseEtat_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_smallint_input(AdresseEtat) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseCommande,"@va_AdresseCommande_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_smallint_input(AdresseCommande) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseDiscordance,"@va_AdresseDiscordance_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_smallint_input(AdresseDiscordance) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_AdresseTelealarme,"@va_AdresseTelealarme_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_smallint_input(AdresseTelealarme) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    
    
    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    

    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: asql_ctparam_int_output(Resultat) a echoue "); 
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    


    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1006: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne 1");
                            asql_clean_cmd(connection, cmd);
                              free(Tab_donnees2);
                              return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao1006 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao1006: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1006_Ajouter_LTN retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return(XDC_OK);
}




