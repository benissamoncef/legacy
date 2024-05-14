/*E*/
/*Fichier :  $Id: xzao620.c,v 1.1 2008/02/13 13:24:51 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao620.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       06 Sep 2007     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao620.c    1.1 06/09/07 : xzao620" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO620SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO620_Ajouter_SeqAut_SIG ( XDY_Basedd            va_Basedd_in,
                                 XZAOT_ConfSeqAutSIG   va_Seq_Aut_SIG_in,
                                 XDY_ResConf           *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_Nom           va_NomSequence_in
* XDY_Nom           va_NomSIG_in
* XDY_Mot           va_Adresse_in
* XDY_Octet         va_BitComm_in
* XDY_Octet         va_BitDiscordance_in
* XDY_Booleen       va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un eqt SIGNALISATION en base
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
* Insertion ou modification dans la table EQT_SQA_GTC_SIG (EQUIPEMENT_SQA_SIG)
*
* MECANISMES :
*------------------------------------------------------*/
{

    static char         *version = "@(#)xzao620.c    1.2 12/15/94 : XZAO620_Ajouter_Seq_Aut_SIG" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO620_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO620_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;

    /*A
    ** Definition des variables locales
    */

    XDY_Nom             vl_NomSequence;
    XDY_NomEqt          vl_NomSIG;
    XDY_Mot             vl_Adresse;
    XDY_Octet           vl_BitComm;
    XDY_Octet           vl_BitDiscordance;
    XDY_Booleen         vl_Creation;

    /*A
    ** Definition des variables locales
    */

    strcpy( vl_NomSequence, va_Seq_Aut_SIG_in.NomSequence );
    strcpy( vl_NomSIG,      va_Seq_Aut_SIG_in.NomSIG );
    vl_Adresse            = va_Seq_Aut_SIG_in.Adresse;
    vl_BitComm            = va_Seq_Aut_SIG_in.BitComm;
    vl_BitDiscordance     = va_Seq_Aut_SIG_in.BitDiscordance;
    vl_Creation           = va_Seq_Aut_SIG_in.Creation;

    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO620_Ajouter_Seq_Aut_SIG : NomSequence = %s\tNomSeqSIG = %s\tAdresse = %d\tBitComm = %d\tBitDiscordance = %d\tCreation= %d\n",
                          va_Seq_Aut_SIG_in.NomSequence,
                          va_Seq_Aut_SIG_in.NomSIG,
                          va_Seq_Aut_SIG_in.Adresse,
                          va_Seq_Aut_SIG_in.BitComm,
                          va_Seq_Aut_SIG_in.BitDiscordance,
                          va_Seq_Aut_SIG_in.Creation );

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */

    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */

    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao620: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
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
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_char_input() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSequence,"@va_NomSequence_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_char_input(NomSequence) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSIG,"@va_NomSIG_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_char_input(NomSIG) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Adresse,"@va_Adresse_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_smallint_input(Adresse) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_BitComm,"@va_BitComm_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_tinyint_input(BitComm) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_BitDiscordance,"@va_BitDiscordance_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_tinyint_input(BitDiscordance) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_bit_input(Creation) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;

    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: asql_ctparam_int_output(Resultat) a echoue "); 
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO, "xzao620: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO, "xzao620 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO, "xzao620 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO, "xzao620 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne 1");
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
                    XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
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
                ** Le serveur SQL a detecte une erreur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,   "xzao620: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */

    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO620_Ajouter_Seq_Aut_SIG retourne %d", retcode);
    asql_clean_cmd(connection, cmd);
    free(Tab_donnees2);
    return(XDC_OK);
}

