/*E*/
/* Fichier : $Id: xzao421.c,v 1.4 2020/05/19 12:12:51 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2020/05/19 12:12:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao421.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       21 Jan 1998     : Creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	06/11/18 : Gestion du param�tre suppl�mentaire 'Supprime' (DEM 1305)  1.3
* LCL	11/06/19	: PAL/IP DEM1337 1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao421.c,v 1.4 2020/05/19 12:12:51 pc2dpdy Exp $	xzao421" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO421SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO421_Ajouter_PAL (	XDY_Basedd	va_Basedd_in,
				XZAOT_AjoutPAL	va_PAL_in,
				XDY_ResConf  	*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XZAOT_AjoutPAL	va_PAL_in
* 
* struct XZAOT_AjoutPAL :
* XDY_NomEqt		va_NomPAL_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_NomEqt		va_Serveur_in
* XDY_Octet		va_LiaisonPAL_in
* XDY_Rgs		va_RgsPAL_in
* XDY_Octet		va_TypePAL_in
* XDY_NomSite		va_NomSite_in
* XDY_Booleen		va_Supprime_in
* XDY_AdresseIP         va_Adresse_IP_in
* XDY_Port              va_Port_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une PAL en base
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
* INTERFACE :
*
* Insertion ou modification dans la table EQT_PAL (C_PAL)
*
* MECANISMES :
*------------------------------------------------------*/

{

	static char *version = "@(#)xzao421.c	1.1 02/03/98	XZAO421_Ajouter_PAL" ;

	CS_CHAR *rpc_name = XZAOC_XZAO421_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO421_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomPAL;
	XDY_NomSite	vl_NomSite;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_PR		vl_PR;
	XDY_Sens	vl_Sens;
	XDY_NomEqt	vl_NomServeur;
	XDY_Octet	vl_Liaison;
	XDY_Rgs		vl_Rgs;
	XDY_Octet	vl_Type;
	XDY_Booleen	vl_Supprime;
        XDY_AdresseIP   vl_Adresse_IP;
        XDY_PortIP        vl_Port;
	
   	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomPAL, va_PAL_in.NomPAL );
	strcpy( vl_NomSite, va_PAL_in.NomSite );
	strcpy( vl_Autoroute, va_PAL_in.Autoroute );
	vl_PR		= va_PAL_in.PR;
	vl_Sens		= va_PAL_in.Sens;
	strcpy( vl_NomServeur, va_PAL_in.NomServeur );
	vl_Liaison	= va_PAL_in.Liaison;
	strcpy( vl_Rgs, va_PAL_in.Rgs );
	vl_Type = va_PAL_in.Type ;
	vl_Supprime = va_PAL_in.Supprime;
        strcpy( vl_Adresse_IP, va_PAL_in.AdresseIP );
        vl_Port = va_PAL_in.Port;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO421_Ajouter_PAL : Nom = %s\tAutoroute = %s\tPR = %d\tSens = %d\tNomServeur = %s\tLiaison = %d\tRgs = %s\tType = %d\t SiteGestion = %s\t Adresse IP = %s\t Port = %d\t Supprime = %d\n",
			va_PAL_in.NomPAL,
			va_PAL_in.Autoroute,
			va_PAL_in.PR,
			va_PAL_in.Sens,
			va_PAL_in.NomServeur,
			va_PAL_in.Liaison,
			va_PAL_in.Rgs,
			va_PAL_in.Type,
			va_PAL_in.NomSite,
                        va_PAL_in.AdresseIP,
                        va_PAL_in.Port,
						va_PAL_in.Supprime);
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomPAL,"@va_NomPAL_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(Nom) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_NomAutoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(NomServeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Liaison,"@va_LiaisonPAL_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_tinyint_input(Liaison) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Rgs,"@va_RgsPAL_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(Rgs) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type,"@va_TypePAL_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(Type) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input(NomSite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Adresse_IP,"@va_Adresse_IP_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port,"@va_Port_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_int_input(int) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }

	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao421: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
						   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao421 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao421 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao421: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO421_Ajouter_PAL retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




