/*E*/
/*Fichier :  $Id: xzao992.c,v 1.1 2008/02/13 13:24:57 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao992.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ          21 Nov 2007     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao992.c    1.1 11/21/07 : xzao992" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO992SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO992_Ajouter_Equipement ( XDY_Basedd        va_Basedd_in,
                                 XZAOT_ConfEqt     va_Equipement_in,
                                 XDY_ResConf       *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_NomEqt        va_TypeEqt_in
* XDY_NomEqt        va_NomEqt_in
* XDY_NomAuto       va_Autoroute_in
* XDY_NomEqt        va_NomMaitre_in
* XDY_PR            va_PR_in
* XDY_Sens          va_Sens_in
* XDY_Voie          va_Voie_in
* XDY_Octet         va_Inverse_in
* XDY_NomEqt        va_Domaine_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf           va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajout d'un equipement GTC en base
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
* Insertion ou modification dans la table EQT_GTC_xxx (EQUIPEMENT_DOMAINE xxx : trigramme domaine)
*
* MECANISMES :
*
------------------------------------------------------*/

{

    static char         *version = "@(#)xzao992.c    1.1 11/21/07 : XZAO992_Ajouter_Equipement" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO992_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO992_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    
    /*A
    ** Definition des variables locales
    */
    
    XDY_Texte      vl_TypeEqt;
    XDY_NomEqt     vl_NomEqt;
    XDY_NomAuto    vl_Autoroute;
    XDY_NomEqt     vl_NomMaitre;
    XDY_PR         vl_PR;
    XDY_Sens       vl_Sens;
    XDY_Voie       vl_Voie;
    XDY_Octet      vl_Inverse;
    XDY_NomEqt     vl_Domaine;

    
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO992_Ajouter_Equipement : Type = %s\tNom = %s\tAutoroute = %s\tMaitre = %s\tPR = %ld\tSens = %d\tVoie = %d\tInverse = %d\tDomaine = %s",
            va_Equipement_in.TypeEqt,
            va_Equipement_in.NomEqt,
            va_Equipement_in.Autoroute,
            va_Equipement_in.NomMaitre,
            va_Equipement_in.PR,
            va_Equipement_in.Sens,
            va_Equipement_in.Voie,
            va_Equipement_in.Inverse,
            va_Equipement_in.Domaine );
    
	vl_TypeEqt[0] = '\0';
    vl_NomEqt[0] = '\0';
    vl_Autoroute[0] = '\0';
    vl_NomMaitre[0] = '\0';
	
    sprintf( vl_TypeEqt,   "%s", va_Equipement_in.TypeEqt );
    sprintf( vl_NomEqt,    "%s", va_Equipement_in.NomEqt );
    sprintf( vl_Autoroute, "%s", va_Equipement_in.Autoroute );
    sprintf( vl_NomMaitre, "%s", va_Equipement_in.NomMaitre );
    vl_PR               = va_Equipement_in.PR;
    vl_Sens             = va_Equipement_in.Sens;
    vl_Voie             = va_Equipement_in.Voie;
    vl_Inverse          = va_Equipement_in.Inverse;
    strcpy( vl_Domaine,   va_Equipement_in.Domaine );
    
    
    

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "xzao992: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
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
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(Basedd) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }


    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_TypeEqt,"@va_TypeEqtGTC_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(NomEqt) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomEqt,"@va_NomEqtGTC_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(NomEqt) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
    
    {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(Autoroute) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomMaitre,"@va_NomMaitre_in"))!= CS_SUCCEED)
    
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(NomMaitre) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_int_input(PR) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_tinyint_input(Sens) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Voie,"@va_Voie_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_tinyint_input(Voie) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Inverse,"@va_Inverse_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_tinyint_input(Inverse) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Domaine,"@va_Domaine_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_char_input(NomMaitre) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    
    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    

    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: asql_ctparam_int_output(Resultat) a echoue "); 
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    


    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao992: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao992 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao992 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao992: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO992_Ajouter_Equipement tout est OK", retcode);
    return(XDC_OK);
}




