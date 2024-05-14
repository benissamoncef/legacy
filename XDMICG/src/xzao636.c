/*E*/
/*Fichier :  $Id: xzao636.c,v 1.1 2008/02/13 13:24:53 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao636.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       24 Septembre 2007     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao636.c    1.1 09/24/07 : xzao636.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO636SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO636_Rech_SYS ( XDY_Autoroute    va_Autoroute_in,
                       XDY_PR           va_PR_in,
                       XDY_Octet        va_Sens_in,
                       XDY_FonctionInt  pa_FonctionUtil_in,
                       XDY_Horodate     va_Horodate_in,
                       XDY_PR           va_PRTete_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute  Autoroute_In
* XDY_PR         PR_In
* XDY_Sens       Sens_In
* XDY_Fonction   FonctionUtilisateur_In
* XDY_Horodate   Horodate_In    
* XDY_PR         PRTete_In
*
* PARAMETRES EN SORTIE :    
*
* VALEUR RENDUE :
*
* Recherche tous les equipements du domaine systeme 
* non inhibee dont la zone de competence a une intersection non vide avec le segment [PR, PRTete]
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_GTC_SYS (Systeme)
*
* MECANISMES :
*
------------------------------------------------------*/

{

    static char         *version = "@(#)xzao636.c    1.5 09/18/95 : xzao636_Rech_SYS" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO636_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          vl_datafmt;
    CS_RETCODE          vl_retcode;
    CS_INT              vl_res_type;
    CS_SMALLINT         vl_msg_id;
    CS_DATETIME         vl_Horodate;
    int                 vl_num_pl_Tab_donnees2 = XZAOC_XZAO636_NB_PARM_RETURN;
    tg_Colonne_donnees2 *pl_Tab_donnees2;
    int                 vl_Nb_Ligne_Lue;
    int                 vl_Nb_Col;
    tg_row_result       vl_Tab_Ligne;
    int                 i;
    XDY_Entier          vl_SPstatus;
    XDY_Entier          vl_SYS;

    /*A
    ** Definition des variables locales
    */

    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO636_Rech_SYS : Autoroute = %d\tPR = %dtSens = %d\tFonction = %ld\tHorodate = %f\tPR Tete = %d\n",
                          va_Autoroute_in,
                          va_PR_in,
                          va_Sens_in,
                          pa_FonctionUtil_in,
                          va_Horodate_in,
                          va_PRTete_in);

    /*
     *A conversion de la date au format sybase
     */
    if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate)) != XDC_OK)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        return ( XDC_NOK );
    }

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */

    pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_pl_Tab_donnees2);

    /*A
    ** Initialisation de la commande                
    */

    if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_cmd_alloc() a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        return ( XDC_NOK );
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */

    if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao636: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode);
        asql_clean_cmd(connection, cmd);
        return (XDC_NOK);
    }

    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_ctparam_tinyint_input(Autoroute) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        asql_clean_cmd( connection, cmd );
        return ( XDC_NOK );
    }

    if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_ctparam_int_input(PR) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        asql_clean_cmd( connection, cmd );
        return ( XDC_NOK );
    }

    if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_ctparam_tinyint_input(Sens) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        asql_clean_cmd( connection, cmd );
        return ( XDC_NOK );
    }

    if ((vl_retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_ctparam_date_input(datetime) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d",vl_retcode );
        printf( "error de ct_param pour va_Horodate_in = %f\n", va_Horodate_in );
        asql_clean_cmd ( connection, cmd );
        return ( XDC_NOK );
    }    

    if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PRTete_in,"@va_PR_tete_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: asql_ctparam_int_input(PRTete) a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        asql_clean_cmd( connection, cmd );
        return ( XDC_NOK );
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: ct_send() a echoue" );
        XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
        asql_clean_cmd( connection, cmd );
        return (XDC_NOK);
    }

    /*A
    ** Traitement du resultat
    */

    while ((vl_retcode = ct_results(cmd, &vl_res_type)) == CS_SUCCEED)
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
                        vl_retcode = asql_fetch_row_result( cmd, &vl_Nb_Ligne_Lue, &vl_Nb_Col, &vl_Tab_Ligne );
                        XZST_03EcritureTrace( XZSTC_INFO, "xzao636: ROW RESULTS" );
                        break;

                    case  CS_PARAM_RESULT:
                        vl_retcode = asql_fetch_param_result ( cmd , vl_num_pl_Tab_donnees2, pl_Tab_donnees2 );
                        XZST_03EcritureTrace( XZSTC_INFO, "xzao636 : PARAMETER RESULTS\n" );
                        break;

                    case  CS_STATUS_RESULT:
                        vl_retcode = asql_fetch ( cmd, &vl_SPstatus );
                        XZST_03EcritureTrace( XZSTC_INFO, "xzao636 : CS_STATUS_RESULT = %ld", vl_retcode );
                        XZST_03EcritureTrace( XZSTC_INFO, "xzao636 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus );

                        if (vl_SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne 1" );
                            asql_clean_cmd( connection, cmd );
                            return ( XDC_NOK );
                        }
                        break;
                }

                break;

            case CS_MSG_RESULT:
                /*
                **
                */
                vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                if (vl_retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao636: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode);
                    asql_clean_cmd(connection, cmd);
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
                XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: ct_results retourne CS_CMD_FAIL." );
                XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
                asql_clean_cmd( connection, cmd );
                return ( XDC_NOK );

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace( XZSTC_WARNING, "xzao636: ct_results retourne un resultat de type inattendu" );
                XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
                asql_clean_cmd( connection, cmd );
                return ( XDC_NOK );
        }
    }

    /*A
    ** Appel de la fonction utilisateur FonctionUtilisateur pour
    ** chaque equipement du domaine Systeme
    ** Affectation des parametres de sortie
    */

    for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
    {
        /*B
        ** Remplissage de la structure XZAOT_Capteur coorrespondant a la ligne resultat
        ** du select de la procadure stockee XZAO;20
        */

        vl_SYS    = * (XDY_Mot *) vl_Tab_Ligne[i][0].pt_value;

        /*B
        ** Appel de la fonction utilisateur FonctionUtilisateur
        */

        if ( pa_FonctionUtil_in == NULL)
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "xzao636 : pa_FonctionUtilisateur_in est nulle" );
            asql_clean_cmd( connection, cmd );
            return ( XDC_NOK );
        }
        else
        {
            if ( ( vl_retcode = (int) (*pa_FonctionUtil_in) ( vl_SYS ) ) != XDC_OK )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzao636 : appel de pa_FonctionUtilisateur_in s'est mal deroule" );
                asql_clean_cmd( connection, cmd );
                return ( XDC_NOK );
            }
        }    
    }

    /*A
    ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
    ** si il y a lieu
    */ 

    vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

    if (vl_retcode != XDC_OK)
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "xzao636 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire" );
        asql_clean_cmd( connection, cmd );
        return ( XDC_NOK );
    }

    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */

    XZST_03EcritureTrace( XZSTC_INTERFACE, "OUT : XZAO636_Rech_SYS retourne %d", vl_retcode );
    asql_clean_cmd( connection, cmd );
    return( XDC_OK );
}

