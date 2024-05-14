/*E*/
/*Fichier :  $Id: xzao911.c,v 1.1 2016/09/29 15:35:04 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:35:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao911.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	31/08/16	creation 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao911.c    1.1 11/21/07 : xzao911" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO911SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO911_Ecrire_Zone_RAU ( char *va_zone ,
                                 char       *va_etat )

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

    static char         *version = "@(#)xzao911.c    1.1 11/21/07 : XZAO911_Ecrire_Zone_RAU" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO911_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO911_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    
    /*A
    ** Definition des variables locales
    */
    
    char      vl_zone[30];
    char     vl_etat[5];
    
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO911_Ecrire_Zone_RAU : Zone = %s\tEtat = %s",
            va_zone,
            va_etat );
    
    sprintf( vl_zone,   "%s", va_zone);
    sprintf( vl_etat,    "%s", va_etat);
    

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "xzao911: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_zone,"@va_Zone_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: asql_ctparam_char_input(Zone_in) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_etat,"@va_Etat_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: asql_ctparam_char_input(Etat_in) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao911: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao911 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao911 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao911: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO911_Ecrire_Zone_RAU retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
    return(XDC_OK);
}




