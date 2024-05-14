/*E*/
/*Fichier :  $Id: xzao0203.c,v 1.1 2008/02/13 13:24:46 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0203.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ         09 Nov 1997     : Creation DEM 662
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao0203.c    1.1 11/09/07 : xzao0203.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO0203SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE ( XDY_Basedd                    va_Basedd_in,
                                          XDY_NomEqt                    va_NomMachine_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurOuv_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSIG_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurECL_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurVEN_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurENR_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurINC_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEXP_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSYS_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurLTN_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTSIG_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTECL_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTVEN_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTENR_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTINC_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTEXP_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTSYS_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurEQTLTN_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSeqSIG_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSeqECL_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSeqVEN_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSeqENR_in,
                                          XDY_FonctionInt               va_FonctionUtilisateurSeqINC_in,
                                          XZAOT_GTC_NICE_RENOVEE        *va_GTC_NICE_RENOVEE_out )

/*
*
* PARAMETRES EN ENTREE :
*
*  XDY_FonctionInt      va_FonctionUtilisateurOUV_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSIG_in,
*  XDY_FonctionInt      va_FonctionUtilisateurECL_in,
*  XDY_FonctionInt      va_FonctionUtilisateurVEN_in,
*  XDY_FonctionInt      va_FonctionUtilisateurENR_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEXP_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEXP_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSYS_in,
*  XDY_FonctionInt      va_FonctionUtilisateurLTN_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTSIG_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTECL_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTVEN_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTENR_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTEXP_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTEXP_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTSYS_in,
*  XDY_FonctionInt      va_FonctionUtilisateurEQTLTN_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSeqSIG_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSeqECL_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSeqVEN_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSeqENR_in,
*  XDY_FonctionInt      va_FonctionUtilisateurSeqEXP_in,
*                
* PARAMETRES EN SORTIE :
*
* XZAOT_GTC_NICE_RENOVEE    va_GTC_NICE_RENOVEE_out
*
* VALEUR RENDUE :
*
* Retourne la GTC Nice de la Machine 
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
* Pour chaque Domaine Signalisation appel de la fonction utilisateur avec XZAOT_OUV
* Pour chaque Domaine Signalisation appel de la fonction utilisateur avec XZAOT_SIG
* Pour chaque Domaine Eclairage appel de la fonction utilisateur avec XZAOT_ECL
* Pour chaque Domaine Ventilation appel de la fonction utilisateur avec XZAOT_VEN
* Pour chaque Domaine Energie appel de la fonction utilisateur avec XZAOT_ENR
* Pour chaque Domaine Protection Incendie appel de la fonction utilisateur avec XZAOT_EXP
* Pour chaque Domaine Exploitation appel de la fonction utilisateur avec XZAOT_EXP
* Pour chaque Domaine Systeme appel de la fonction utilisateur avec XZAOT_SYS
* Pour chaque Domaine Local Technique appel de la fonction utilisateur avec XZAOT_LTN
* Pour chaque Domaine Signalisation appel de la fonction utilisateur avec XZAOT_EQTSIG
* Pour chaque Domaine Eclairage appel de la fonction utilisateur avec XZAOT_EQTECL
* Pour chaque Domaine Ventilation appel de la fonction utilisateur avec XZAOT_EQTVEN
* Pour chaque Domaine Energie appel de la fonction utilisateur avec XZAOT_EQTENR
* Pour chaque Domaine Protection Incendie appel de la fonction utilisateur avec XZAOT_EQTEXP
* Pour chaque Domaine Exploitation appel de la fonction utilisateur avec XZAOT_EQTEXP
* Pour chaque Domaine Systeme appel de la fonction utilisateur avec XZAOT_EQTSYS
* Pour chaque Domaine Local Technique appel de la fonction utilisateur avec XZAOT_EQTLTN
* Pour chaque SeqSIG appel de la fonction utilisateur avec XZAOT_SeqSIG
* Pour chaque SeqECL appel de la fonction utilisateur avec XZAOT_SeqECL
* Pour chaque SeqVEN appel de la fonction utilisateur avec XZAOT_SeqVEN
* Pour chaque SeqENR appel de la fonction utilisateur avec XZAOT_SeqENR
* Pour chaque SeqEXP appel de la fonction utilisateur avec XZAOT_SeqEXP
*
* MECANISMES :
*
------------------------------------------------------*/

{

    static char         *version = "@(#)xzao0203.c    1.10 01/22/98 : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE" ;
    
    CS_CHAR             *rpc_name = XZAOC_XZAO0203_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO0203_NB_PARM_RETURN;
    tg_Colonne_donnees2 Tab_donnees2[XZAOC_XZAO0203_NB_PARM_RETURN];
    int                 vl_Nb_Ligne_Lue = -1;
    int                 vl_Nb_Col = 0;
    tg_row_result       vl_Tab_Ligne;
    int                 i, j;
    XDY_Entier          SPstatus;
    XDY_Entier          vl_Num = 0;
    XDY_Entier          vl_Resultat = 0;
    
    
    /*A
    ** Definition des variables locales
    */

    XZAOT_OUV           vl_OUV;

    XZAOT_DOM           vl_DOM;

    XZAOT_EQT           vl_EQT;

    XZAOT_SEQ           vl_SEQ;
    
    XDY_Eqt             vl_Numero;
    XDY_NomEqt          vl_NomServeur;

    
    
    XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE : Basedd = %s\tMachine = %s",
            va_Basedd_in,
            va_NomMachine_in );
                    
            
    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
         XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        return (XDC_NOK);
    }
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        return (XDC_NOK);
    }

    
    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
    
    {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: asql_ctparam_char_input(Basedd) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       return (XDC_NOK);
    }
        
    
    
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
    
    {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: asql_ctparam_char_input(NomMachine) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       return (XDC_NOK);
    }
    
    Tab_donnees2[0].pt_value = (CS_VOID *)&va_GTC_NICE_RENOVEE_out->Numero;
    
    if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_RENOVEE_out->Numero,"@va_Numero_out")) != CS_SUCCEED)    

    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: asql_ctparam_smallint_output(Numero) a echoue "); 
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       return (XDC_NOK);
    }

    
    Tab_donnees2[1].pt_value = (CS_VOID *)&va_GTC_NICE_RENOVEE_out->NomServeur;
    
    if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)&va_GTC_NICE_RENOVEE_out->NomServeur,"@va_NomServeur_out")) != CS_SUCCEED)    

    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: asql_ctparam_char_output(NomServeur) a echoue "); 
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande au Sql server
    */
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
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
                        vl_Num ++;
                        retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0203: ROW RESULTS");
                        vl_Resultat = 0;
                                         
                        switch (vl_Num)
                        {
                        
						
						   case 1 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
             							** Si le nom de l'ouvrage est different du precedent
              							** Remplissage de la structure XZAOT_OUV
              							** Appel de la fonction utilisateur FonctionUtilisateurOuv
              							*/
           
              							vl_Resultat ++;
              							
              							strcpy(vl_OUV.Ouvrage, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                 						strcpy(vl_OUV.NomOuvrage, * (XDY_NomEqt *) vl_Tab_Ligne[i][1].pt_value);
                                        /* Remplacement des espaces du nom de l'ouvrage par des soulignes */
                                        for (j=0;j < XDC_Lon_Nom; j++)
                                        {
                                           if (vl_OUV.NomOuvrage[j] == ' ')
                                               vl_OUV.NomOuvrage[j] = '_';
                                        }
 
              
	         						if ( va_FonctionUtilisateurOuv_in == NULL)
	         						{
	           							XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurOuv est nulle");
	            							asql_clean_cmd(connection, cmd);
	   								return (XDC_NOK);
	         						}
	         						else
	         						{
	            							if ( ( retcode = (*va_FonctionUtilisateurOuv_in) ( vl_OUV, vl_Resultat ) ) != XDC_OK )
	            							{
	               								XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurOUV s'est mal deroule");
	               								asql_clean_cmd(connection, cmd);
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   								return (XDC_NOK);
								}	
 							     
 							     break;

                            case 2 :  
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Si le numero de SIG est different du precedent
                                    ** Remplissage de la structure XZAOT_DOM
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSIG
                                    */
                                    /*
                                        XDY_NomEqt	   Domaine;
                                        XDY_Octet      Type;
                                        XDY_Eqt		   Numero;
                                        XDY_Etat_Eqt   Etat;

                                    */

                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSIG_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSIG est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSIG_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSIG s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }    
                                  
                                break;

                            case 3 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_ECL
                                    ** Appel de la fonction utilisateur FonctionUtilisateurECL
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;
                                          
                                    if ( va_FonctionUtilisateurECL_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurECL est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurECL_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurECL s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 4 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_VEN
                                    ** Appel de la fonction utilisateur FonctionUtilisateurVEN
                                    */ 
                                       
                                    vl_Resultat ++;
                                       

                                    strcpy(vl_DOM.Domaine,  * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;                                          
                                          
                                    if ( va_FonctionUtilisateurVEN_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurVEN est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurVEN_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurVEN s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 5 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_ENR
                                    ** Appel de la fonction utilisateur FonctionUtilisateurENR
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurENR_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurENR est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurENR_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurENR s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 6 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EXP
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEXP
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurEXP_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEXP est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEXP_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEXP s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 7 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EXP
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEXP
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurEXP_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEXP est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEXP_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEXP s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 8 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SYS
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSYS
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSYS_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSYS est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSYS_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSYS s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 9 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_LTN
                                    ** Appel de la fonction utilisateur FonctionUtilisateurLTN
                                    */ 
                                       
                                    vl_Resultat ++;

                                    strcpy(vl_DOM.Domaine, * (XDY_NomEqt *)   vl_Tab_Ligne[i][0].pt_value);
                                    vl_DOM.Type      = * (XDY_Octet *)    vl_Tab_Ligne[i][1].pt_value;
                                    vl_DOM.Numero    = * (XDY_Eqt *)      vl_Tab_Ligne[i][2].pt_value;
                                    vl_DOM.Etat      = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurLTN_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurLTN est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurLTN_in) ( vl_DOM, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurLTN s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 10 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTSIG
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTSIG
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT,  * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTSIG_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTSIG est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTSIG_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTSIG s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 11 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTECL
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTECL
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTECL_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTECL est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTECL_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTECL s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 12 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTVEN
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTVEN
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTVEN_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTVEN est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTVEN_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTVEN s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 13 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTENR
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTENR
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTENR_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTENR est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTENR_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTENR s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 14 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTEXP
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTEXP
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTEXP_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTEXP est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTEXP_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTEXP s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 15 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTEXP
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTEXP
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTEXP_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTEXP est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTEXP_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTEXP s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 16 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTSYS
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTSYS
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTSYS_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTSYS est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTSYS_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTSYS s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 17 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_EQTLTN
                                    ** Appel de la fonction utilisateur FonctionUtilisateurEQTLTN
                                    */ 

                                    vl_Resultat ++;
                                    strcpy(vl_EQT.EQT, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_EQT.TypeMaitre  = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][1].pt_value;
                                    vl_EQT.Maitre      = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
                                    vl_EQT.Type        = * (XDY_TypeEqt *)   vl_Tab_Ligne[i][3].pt_value;
                                    vl_EQT.Numero      = * (XDY_Eqt *) vl_Tab_Ligne[i][4].pt_value;
                                    strcpy(vl_EQT.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][5].pt_value);

                                    if ( va_FonctionUtilisateurEQTLTN_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurEQTLTN est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurEQTLTN_in) ( vl_EQT, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurEQTLTN s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 18 :
								XZST_03EcritureTrace( XZSTC_WARNING, "XZAO0203 : Recuperation des sequences autorisees SIGNALISATION" );
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SeqSIG
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSeqSIG
                                    */

                                    vl_Resultat ++;
                                    strcpy(vl_SEQ.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_SEQ.TypeEquipement = * (XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
                                    vl_SEQ.Numero         = * (XDY_Eqt *)    vl_Tab_Ligne[i][2].pt_value;
                                    vl_SEQ.NumSequence    = * (XDY_Octet *)  vl_Tab_Ligne[i][3].pt_value;
									XZST_03EcritureTrace( XZSTC_WARNING, "XZAO0203 : domaine=%s, typeEqt = %d, numero = %d, numero sequence = %d", 
												vl_SEQ.Domaine, 
												vl_SEQ.TypeEquipement,
												vl_SEQ.Numero,
												vl_SEQ.NumSequence);

                                    if ( va_FonctionUtilisateurSeqSIG_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSeqSIG est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSeqSIG_in) ( vl_SEQ, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSeqSIG s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 19 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SeqECL
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSeqECL
                                    */

                                    vl_Resultat ++;
                                    strcpy(vl_SEQ.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_SEQ.TypeEquipement = * (XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
                                    vl_SEQ.Numero         = * (XDY_Eqt *)    vl_Tab_Ligne[i][2].pt_value;
                                    vl_SEQ.NumSequence    = * (XDY_Octet *)  vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSeqECL_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSeqECL est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSeqECL_in) ( vl_SEQ, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSeqECL s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 20 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SeqVEN
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSeqVEN
                                    */
                                    /*
                                	XDY_NomEqt	Domaine;
                                	XDY_Octet   TypeEquipement;
                                	XDY_Eqt		Numero;
                                	XDY_Octet	NumSequence;

                                    */

                                    vl_Resultat ++;
                                    strcpy(vl_SEQ.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_SEQ.TypeEquipement = * (XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
                                    vl_SEQ.Numero         = * (XDY_Eqt *)    vl_Tab_Ligne[i][2].pt_value;
                                    vl_SEQ.NumSequence    = * (XDY_Octet *)  vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSeqVEN_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSeqVEN est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSeqVEN_in) ( vl_SEQ, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSeqVEN s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 21 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SeqENR
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSeqENR
                                    */
                                    /*
                                	XDY_NomEqt	Domaine;
                                	XDY_Octet   TypeEquipement;
                                	XDY_Eqt		Numero;
                                	XDY_Octet	NumSequence;

                                    */

                                    vl_Resultat ++;
                                    strcpy(vl_SEQ.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_SEQ.TypeEquipement = * (XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
                                    vl_SEQ.Numero         = * (XDY_Eqt *)    vl_Tab_Ligne[i][2].pt_value;
                                    vl_SEQ.NumSequence    = * (XDY_Octet *)  vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSeqENR_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSeqENR est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSeqENR_in) ( vl_SEQ, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSeqENR s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                            case 22 :
                                for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
                                {
                                    /*B
                                    ** Remplissage de la structure XZAOT_SeqINC
                                    ** Appel de la fonction utilisateur FonctionUtilisateurSeqINC
                                    */

                                    vl_Resultat ++;
                                    strcpy(vl_SEQ.Domaine, * (XDY_NomEqt *) vl_Tab_Ligne[i][0].pt_value);
                                    vl_SEQ.TypeEquipement = * (XDY_Octet *)  vl_Tab_Ligne[i][1].pt_value;
                                    vl_SEQ.Numero         = * (XDY_Eqt *)    vl_Tab_Ligne[i][2].pt_value;
                                    vl_SEQ.NumSequence    = * (XDY_Octet *)  vl_Tab_Ligne[i][3].pt_value;

                                    if ( va_FonctionUtilisateurSeqINC_in == NULL)
                                    {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : va_FonctionUtilisateurSeqINC est nulle");
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                    }
                                    else
                                    {
                                        if ( ( retcode = (*va_FonctionUtilisateurSeqINC_in) ( vl_SEQ, vl_Resultat ) ) != XDC_OK )
                                        {
                                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : appel de va_FonctionUtilisateurSeqINC s'est mal deroule");
                                            asql_clean_cmd(connection, cmd);
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
                                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
                                    asql_clean_cmd(connection, cmd);
                                    return (XDC_NOK);
                                }

                                break;

                        }
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0203 : PARAMETER RESULTS");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0203 : CS_STATUS_RESULT = %d", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao0203 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne 1");
                            asql_clean_cmd(connection, cmd);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);
        }
    }

    /*A
    ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
    ** si il y a lieu
    */ 
    
    if (vl_Nb_Ligne_Lue >= 0 )
    {
        retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

        if (retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao0203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
            asql_clean_cmd(connection, cmd);
            return (XDC_NOK);
        }
    }    
     
     
    /*A
    ** Retourne le resultat et les parametres de sorties renseignes
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO0203_Liste_Eqt_GTC_NICE_RENOVEE retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
    return(XDC_OK);
}




