/*E*/
/*  Fichier : 	$Id: xzao86.c,v 1.5 1995/03/21 17:04:16 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/03/21 17:04:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao86.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       23 Sep 1994     : Creation
* volcic       26 Oct 1994     : Ajout recup status SP (v1.2)
* volcic       01 Nov 1994     : Modification entete (v1.3)
* volcic       15 Jan 1995     : Modification Param Resultat (v1.4)
* C.T.	       10/03/95        : Ajout capacite VL et PL (V 1.5)
* C.T.	       21/03/95        : Modif capacite VL et PL (V 1.6)
*	Mise en conformite PPL.
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao86.c,v 1.5 1995/03/21 17:04:16 gaborit Exp $ : xzao86" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO86SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO86_Ajouter_Aire (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfAire		va_Aire_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_NomAire_in
* XDY_Abrev		va_Abreviation_in
* XDY_Nom		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRentree
* XDY_PR		va_PRsortie	
* XDY_Booleen		va_Creation_in
* XDY_Mot               va_CapaciteVL_in
* XDY_Mot               va_CapacitePL_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une aire en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table RES_AIR (AIRE)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao86.c,v 1.5 1995/03/21 17:04:16 gaborit Exp $ : XZAO86_Ajouter_Aire" ;

	CS_CHAR *rpc_name = XZAOC_XZAO86_RPC_NAME;
	
	CS_CONNECTION   *vl_connection=XZAGV_Connexion;
	CS_COMMAND	*vl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int 		vl_num_Tab_donnees2 = XZAOC_XZAO86_NB_PARM_RETURN;
	tg_Colonne_donnees2 *vl_Tab_donnees2;
	XDY_Entier	vl_SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Octet	vl_Numero;
   	XDY_Nom		vl_NomAire;
   	XDY_Abrev	vl_Abreviation;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_Sens	vl_Sens;
   	XDY_PR		vl_PRentree;
	XDY_PR		vl_PRsortie;
	XDY_Booleen	vl_Creation;
	
	/*A
	** Definition des variables locales
	*/
	
	vl_Numero 	= va_Aire_in.Numero;
	strcpy( vl_NomAire, va_Aire_in.NomAire );
	strcpy( vl_Abreviation, va_Aire_in.Abreviation );
	strcpy( vl_Autoroute, va_Aire_in.Autoroute );
	vl_Sens		= va_Aire_in.Sens;
	vl_PRentree	= va_Aire_in.PRentree;
	vl_PRsortie	= va_Aire_in.PRsortie;
	vl_Creation	= va_Aire_in.Creation;
	
	
	
	XZST_03EcritureTrace( XZSTC_FONCTION,"IN : XZAO86_Ajouter_Aire : Numero = %d\tAire = %s\tAbrev = %s\tAutoroute = %s\tSens = %d\tPRentree = %d\tPRsortie = %d\tCreation = %d\n",
			va_Aire_in.Numero,
			va_Aire_in.NomAire,
			va_Aire_in.Abreviation,
			va_Aire_in.Autoroute,
			va_Aire_in.Sens,
			va_Aire_in.PRentree,
			va_Aire_in.PRsortie,
			va_Aire_in.Creation );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	vl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(vl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
		free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
	  	free(vl_Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Numero,"@va_Numero_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao88: asql_ctparam_tinyint_input(Numero) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO88_Ajouter_Peage retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_NomAire,"@va_NomAire_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_char_input(NomAire) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_Abreviation,"@va_Abreviation_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_char_input(Abreviation) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_PRentree,"@va_PRentree_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_int_input(PRentree) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_PRsortie,"@va_PRsortie_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_int_input(PRsortie) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_bit_input(Creation) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&(va_Aire_in.CapaciteVL),"@va_CapaciteVL_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_smallint_input(CapaciteVL) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&(va_Aire_in.CapacitePL),"@va_CapacitePL_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_smallint_input(CapacitePL) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	vl_Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((vl_retcode = asql_ctparam_int_output(vl_cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: asql_ctparam_smallint_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
	   asql_clean_cmd(vl_connection, vl_cmd);
	   free(vl_Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao86: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
		asql_clean_cmd(vl_connection, vl_cmd);
	   	free(vl_Tab_donnees2);
	   	return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_data (vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao86: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (vl_cmd , vl_num_Tab_donnees2, vl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO86_Ajouter_Aire retourne 1");
 						   asql_clean_cmd(vl_connection, vl_cmd);
	   					   free(vl_Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao86 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
                                        asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao86: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
						asql_clean_cmd(vl_connection, vl_cmd);
	   					free(vl_Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao86 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao86: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao86: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
					asql_clean_cmd(vl_connection, vl_cmd);
	   				free(vl_Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO86_Ajouter_Aire retourne %d", vl_retcode);
 	asql_clean_cmd(vl_connection, vl_cmd);
 	free(vl_Tab_donnees2);
	return(XDC_OK);
}




