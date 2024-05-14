/*E*/
/*Fichier :  $Id: xzao317.c,v 1.1 2008/02/13 13:24:49 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao317.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ           21 Sep 2007     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "" ;
    
/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO317SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO317_Liste_EQT_Synoptique ( XDY_Basedd      va_Basedd_in,
                                   XDY_Nom         va_NomTroncon_in,
                                   XDY_Booleen     va_Tous_in,
                                   XDY_FonctionInt va_FonctionUtilisateurEQTSyn_in,
                                   XDY_Entier      *va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier     va_Basedd_in            
* XDY_Nom        va_NomTroncon_in
* XDY_Booleen    va_Tous_in
* XDY_Fonction   va_FonctionUtilisateurEQT_Synoptique_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier     va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des EQT d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
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
* Pour chaque EQT_Synoptique appel de la fonction utilisateur avec XZAOT_EQT_Synoptique
*
* MECANISMES :
*
------------------------------------------------------*/

{

    static char *version = "@(#)xzao317.c    1.1 21/09/07 : XZAO317_Liste_EQT_Synoptique" ;
    
    CS_CHAR             *rpc_name        = XZAOC_XZAO317_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection      = XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO317_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    int                 vl_Nb_Ligne_Lue  = 0;
    int                 vl_Nb_Col        = 0;
    tg_row_result       vl_Tab_Ligne;
    int                 i;
    XDY_Entier          SPstatus;
    
    /*A
    ** Definition des variables locales
    */
    
    XZAOT_EQTSyn        vl_EQTSyn;
    
    XZST_03EcritureTrace ( XZSTC_INTERFACE,"IN : XZAO317_Liste_EQT_Synoptique : Basedd = %s\tFonction = %ld\n",
                           va_Basedd_in,
                           va_FonctionUtilisateurEQTSyn_in );
            
            
    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
    
    
    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: ct_cmd_alloc() a echoue" );
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
        free ( Tab_donnees2 );
        return ( XDC_NOK );
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: ct_command() a echoue");
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
        asql_clean_cmd ( connection, cmd );
        free ( Tab_donnees2 );
        return ( XDC_NOK );
    }

    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: asql_ctparam_char_input(Basedd) a echoue" );
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
        asql_clean_cmd ( connection, cmd);
        free ( Tab_donnees2 );
        return ( XDC_NOK );
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomTroncon_in,"@va_NomTroncon_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: asql_ctparam_char_input(NomTroncon) a echoue");
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
        asql_clean_cmd ( connection, cmd );
        free ( Tab_donnees2 );
        return ( XDC_NOK );
    }

    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&va_Tous_in,"@va_Tous_in"))!= CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: asql_ctparam_bit_input(Tous) a echoue");
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
        asql_clean_cmd ( connection, cmd );
        free ( Tab_donnees2 );
        return ( XDC_NOK );
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace ( XZSTC_WARNING, "xzao317: ct_send() a echoue" );
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode );
        asql_clean_cmd ( connection, cmd );
        free ( Tab_donnees2 );
        return ( XDC_NOK );
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
                        retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao317: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace (XZSTC_INFO, "xzao317 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao317 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao317 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);

                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne 1");
                            asql_clean_cmd( connection, cmd);
                            free( Tab_donnees2 );
                            return ( XDC_NOK );
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao317: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao317: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao317: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    /*A
    ** Appel de la fonction utilisateur FonctionUtilisateurEQT_Synoptique pour
    ** chaque type de EQT_Synoptique
    ** Affectation des parametres de sortie
    */

    va_Resultat_out = 0;

    for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
    {
        va_Resultat_out ++;
       
        /*B
        ** Remplissage de la structure XZAOT_EQTSyn coorrespondant a la ligne resultat
        ** du select de la procadure stockee XZAO;317
        */

        vl_EQTSyn.NumeroEQT        = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
        vl_EQTSyn.NumeroAutoroute  = * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
        vl_EQTSyn.PR               = * (XDY_PR *) vl_Tab_Ligne[i][2].pt_value;
        vl_EQTSyn.Sens             = * (XDY_Sens *) vl_Tab_Ligne[i][3].pt_value;
        vl_EQTSyn.Voie             = * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
        vl_EQTSyn.Inverse          = * (XDY_Booleen *) vl_Tab_Ligne[i][5].pt_value;

        /*B
        ** Appel de la fonction utilisateur FonctionUtilisateurEQTSyn
        */
      
        if ( va_FonctionUtilisateurEQTSyn_in == NULL)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"va_FonctionUtilisateurEQTSyn_in est nulle");
            asql_clean_cmd(connection, cmd);
            free(Tab_donnees2);
            return (XDC_NOK);
        }
        else
        {
            if ( ( retcode = (*va_FonctionUtilisateurEQTSyn_in) ( vl_EQTSyn ) ) != XDC_OK )
            {
                XZST_03EcritureTrace(XZSTC_FATAL,"appel de va_FonctionUtilisateurEQTSyn s'est mal deroule");
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
            }
        }
    }

    /*A
    ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
    ** si il y a lieu
    */ 

    retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

    if (retcode != XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }    

    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */

    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO317_Liste_EQT_Synoptique retourne %d", retcode);
    asql_clean_cmd(connection, cmd);
    free(Tab_donnees2);
    return(XDC_OK);
}




