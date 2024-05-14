/*E*/
/*Fichier : $Id: xzae213.c,v 1.1 2019/04/05 11:23:39 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/04/05 11:23:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzae213.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tases
* permet d'extraire les infos obligatoires pour
* generer le fichier des evts strada
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL   17/09/18        Creation DEM1283 GABEB
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae213.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzae213.c	1.2 12/03/97 : xzae213" ;

/* declaration de fonctions internes */
CS_STATIC CS_RETCODE CS_INTERNAL
BusyWait(CS_CONNECTION *connection, char *where)
{
        CS_COMMAND      *compcmd;
        CS_INT          compid;
        CS_RETCODE      compstat;
        CS_RETCODE      retstat;

        XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: Waiting [%s]", where);
        

        do
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: .");
                
                retstat = ct_poll(NULL, connection, 100, NULL, &compcmd,
                        &compid, &compstat);
                if (retstat != CS_SUCCEED && retstat != CS_TIMED_OUT &&
                    retstat != CS_INTERRUPT)
                {
						if (retstat == CS_QUIET)
                        	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll returned CS_QUIET",
                                retstat);
						else
                        	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll returned unexpected status of %d ",
                                retstat);
                        break;
                }
        } while (retstat != CS_SUCCEED);

        if (retstat == CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll completed: compid = %d, compstat = %d\n",
                        compid, compstat);
                
        }

        return compstat;
}

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE213_Fermeture_Echangeur : recupere les parametres
* de sortie de xzae213
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE213_Fermeture_Echangeur (XDY_Octet	va_ech_in,
                                        XDY_Booleen     *va_entree1_out,
                                        XDY_Booleen     *va_entree2_out,
                                        XDY_Booleen     *va_sortie1_out,
                                        XDY_Booleen     *va_sortie2_out)


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_ech_in
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* - va_ech_in sont obligatoires
*

* FONCTION :
*
* Extraire les fermetures de bretelles enc ours sur l'echangeur
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzae213.c	1.2 12/03/97 : XZAE213_Fermeture_Echangeur" ;
	
	CS_CHAR 	*pl_rpc_name = XZAE213C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAE213C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE213C_NB_PARM_RETURN];
	XDY_Entier	vl_SPstatus;
	int		i;
	CS_DATETIME	va_debut_out;	
	CS_DATETIME	va_fin_out;
	CS_DATETIME	va_validation_out;
	int             vl_Nb_Ligne_Lue = 0;
        int             vl_Nb_Col = 0;
        tg_row_result   vl_Tab_Ligne;
	CS_TINYINT	vl_vit;

	
	/*A
	** Initialisation de la commande				
	*/

	
	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
		if (vl_retcode==CS_BUSY)
		{ //Wait par ct_poll
			vl_retcode = BusyWait(pl_connection, "ct_cmd_alloc");
        	if (vl_retcode != CS_SUCCEED && vl_retcode != CS_PENDING)
        	{
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_poll() pour ct_cmd_alloc a echoue");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
				asql_clean_cmd(pl_connection, &pl_cmd);
				return (XDC_NOK);
        	}
			else
			{
				if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
				{
	 				XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_cmd_alloc() a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, &pl_cmd);
					return (XDC_NOK);
				}
			}
		}
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}


	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&va_ech_in,"@va_ech_in"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: asql_ctparam_date_input(va_ech_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
	   	asql_clean_cmd (pl_connection,pl_cmd);
	   	return (XDC_NOK);
	}
	

	i = 0;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_entree1_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_entree1_out,"@va_Entree1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae50: asql_ctparam_bit_output(Entree1) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE50_Fermeture_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_entree2_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_entree2_out,"@va_Entree2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae50: asql_ctparam_bit_output(Entree2) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE50_Fermeture_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_sortie1_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_sortie1_out,"@va_Sortie1_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae50: asql_ctparam_bit_output(Sortie1) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE50_Fermeture_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

        i = i + 1;
        Tab_donnees2[i].pt_value = (CS_VOID *)va_sortie2_out;

        if ((vl_retcode = asql_ctparam_bit_output(pl_cmd,(CS_BIT *)va_sortie2_out,"@va_Sortie2_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae50: asql_ctparam_bit_output(Sortie2) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE50_Fermeture_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }

	/*A
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae213: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE213 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE213 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAE213 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE213_Fermeture_Echangeur retourne 1 : %d", (int) vl_SPstatus);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

	

	XZST_03EcritureTrace(	XZSTC_WARNING,
			"OUT : XZAE213_Fermeture_Echangeur entree1 %d entree2 %d sortie 1 %d sortie2",*va_entree1_out,*va_entree2_out,*va_sortie1_out,*va_sortie2_out);
	
	
	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	asql_clean_cmd (pl_connection,pl_cmd);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", vl_retcode);
 	return(XDC_OK);
}



