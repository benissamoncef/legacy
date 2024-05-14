/*E*/
/*Fichier :  $Id: xzao892.c,v 1.1 2008/02/13 13:24:55 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:55 $
------------------------------------------------------
* GTIE *  AdresseOJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao892.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	10/08/2022	: Creation DEM-SAE406
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao892.c    1.1 10/08/2022 : xzao892" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO892SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO892_Ajouter_SeqRet_SIG ( XDY_Basedd           va_Basedd_in,
                                 XZAOT_ConfSeqRetSIG  va_Seq_Ret_SIG_in,
                                 XDY_ResConf          *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd         va_Basedd_in
* XDY_Nom            va_NomSequence_in
* XDY_Nom            va_NomSIG_in
* XDY_Booleen        va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un tunnel en base
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
* Insertion ou modification dans la table EQT_SQR_GTC_SIG
*
* MECANISMES :
*------------------------------------------------------*/

{

    static char *version = "@(#)xzao892.c    1.1 10/08/2022 : XZAO892_Ajouter_SeqRet_SIG" ;
    
    CS_CHAR             *rpc_name = XZAOC_XZAO892_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO892_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    /*A
    ** Definition des variables locales
    */
    
    XDY_Entier       vl_NumeroEquipment;
    XDY_Entier       vl_ScenarioTerrain;
    XDY_Entier       vl_ScenarioRetourNormal;
    
    /*A
    ** Definition des variables locales
    */
    
    vl_NumeroEquipment = va_Seq_Ret_SIG_in.NumeroEquipment;
    vl_ScenarioTerrain = va_Seq_Ret_SIG_in.ScenarioTerrain;
    vl_ScenarioRetourNormal = va_Seq_Ret_SIG_in.ScenarioRetourNormal;
    

    XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO892_Ajouter_SeqRet_SIG : NumeroEquipment = %d\nScenarioTerrain= %d\nScenario_Retour_Normal = %d",
    va_Seq_Ret_SIG_in.NumeroEquipment,
    va_Seq_Ret_SIG_in.ScenarioTerrain,
    va_Seq_Ret_SIG_in.ScenarioRetourNormal);

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
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
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: asql_ctparam_char_input() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_NumeroEquipment,"@va_NumeroEquipment_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: asql_ctparam_tinyint_input(va_NumeroEquipment_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_ScenarioTerrain,"@va_ScenarioTerrain_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: asql_ctparam_tinyint_input(va_ScenarioTerrain_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_ScenarioRetourNormal,"@va_ScenarioRetourNormal_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: asql_ctparam_tinyint_input(va_ScenarioRetourNormal_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;

    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: asql_ctparam_int_output(Resultat) a echoue "); 
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao892: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao892 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao892 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao892 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                break;

            case CS_MSG_RESULT:
                /*
                **
                */
                retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao892: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */

    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO892_Ajouter_SeqRet_SIG retourne %d", retcode);
    asql_clean_cmd(connection, cmd);
    free(Tab_donnees2);
    return(XDC_OK);
}




