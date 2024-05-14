/*E*/
/*Fichier :  $Id: xzao1002.c,v 1.1 2008/02/13 13:24:46 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:46 $
------------------------------------------------------
* GTIE *  AdresseOJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1002.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ           15 Nov 2007     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao1002.c    1.1 11/15/07 : xzao1002" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO1002SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO1002_Ajouter_Seq_INC ( XDY_Basedd           va_Basedd_in,
                              XZAOT_ConfSeqINC     va_Seq_INC_in,
                              XDY_ResConf          *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd         va_Basedd_in
* XDY_Nom            va_NomSequence_in
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
* Insertion ou modification dans la table EQT_SEQ_TUB (EQUIPEMENT_SEQ_TUB)
*
* MECANISMES :
*------------------------------------------------------*/

{

    static char *version = "@(#)xzao1002.c    1.1 11/15/07 : XZAO1002_Ajouter_Seq_INC" ;
    
    CS_CHAR             *rpc_name = XZAOC_XZAO1002_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO1002_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    /*A
    ** Definition des variables locales
    */
    
    XDY_Nom        vl_NomSequence;
    XDY_Octet      vl_Type;
    XDY_Booleen    vl_Creation;
    
    /*A
    ** Definition des variables locales
    */
    
    strcpy( vl_NomSequence, va_Seq_INC_in.NomSequence );
    vl_Type               = va_Seq_INC_in.Type;
    vl_Creation           = va_Seq_INC_in.Creation;

    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO1002_Ajouter_Seq_INC : NomSequence = %s\tType= %d\tCreation= %d\n",
            va_Seq_INC_in.NomSequence,
            va_Seq_INC_in.Type,
            va_Seq_INC_in.Creation );

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
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
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: asql_ctparam_char_input() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSequence,"@va_NomSequence_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: asql_ctparam_char_input(NomSequence) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type,"@va_Type_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: asql_ctparam_tinyint_input(Type) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: asql_ctparam_bit_input(Creation) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;

    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: asql_ctparam_int_output(Resultat) a echoue "); 
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1002: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1002 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1002 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao1002 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne 1");
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao1002: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */

    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO1002_Ajouter_Seq_INC retourne %d", retcode);
    asql_clean_cmd(connection, cmd);
    free(Tab_donnees2);
    return(XDC_OK);
}




