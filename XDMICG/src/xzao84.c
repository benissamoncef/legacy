/*E*/
/*  Fichier : 	$Id: xzao84.c,v 1.3 2012/06/20 18:04:51 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2012/06/20 18:04:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao84.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       23 Sep 1994     : Creation
* volcic       26 Oct 1994     : Ajout retour status SP (v1.2)
* volcic       01 Nov 1994     : Modification entete (v1.2)
* JPL		14/06/2012     : Ajout parametres Nom sortie et Indicateur bifurcation (DEM 1033)  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao84.c,v 1.3 2012/06/20 18:04:51 gesconf Exp $ : xzao84" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO84SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO84_Ajouter_Echangeur (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfEchang	 va_Echangeur_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_NomEchangeur_in
* XDY_Abrev		va_Abreviation_in
* XDY_Nom		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_PR		va_PRentreeSens1_in	
* XDY_Octet		va_VoiesEntreeSens1_in
* XDY_Booleen		va_PeageEntreeSens1_in
* XDY_PR		va_PRentreeSens2_in	
* XDY_Octet		va_VoiesEntreeSens2_in
* XDY_Booleen		va_PeageEntreeSens2_in
* XDY_PR		va_PRsortieSens1_in	
* XDY_Octet		va_VoiesSortieSens1_in
* XDY_Booleen		va_PeageSortieSens1_in
* XDY_PR		va_PRsortieSens2_in	
* XDY_Octet		va_VoiesSortieSens2_in
* XDY_Booleen		va_PeageSortieSens2_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un echangeur en base
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
* Insertion ou modification dans la table RES_ECH (ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao84.c,v 1.3 2012/06/20 18:04:51 gesconf Exp $ : XZAO84_Ajouter_Echangeur" ;

	CS_CHAR *rpc_name = XZAOC_XZAO84_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO84_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Octet	vl_Numero;
   	XDY_Nom		vl_NomEchangeur;
   	XDY_Abrev	vl_Abreviation;
   	XDY_Sortie	vl_NomSortie;
	XDY_Booleen	vl_Bifurcation;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_PR		vl_PR;	
	XDY_PR		vl_PRentreeSens1;	
	XDY_Octet	vl_VoiesEntreeSens1;
	XDY_Booleen	vl_PeageEntreeSens1;
	XDY_PR		vl_PRentreeSens2;	
	XDY_Octet	vl_VoiesEntreeSens2;
	XDY_Booleen	vl_PeageEntreeSens2;
	XDY_PR		vl_PRsortieSens1;	
	XDY_Octet	vl_VoiesSortieSens1;
	XDY_Booleen	vl_PeageSortieSens1;
	XDY_PR		vl_PRsortieSens2;	
	XDY_Octet	vl_VoiesSortieSens2;
	XDY_Booleen	vl_PeageSortieSens2;
	XDY_Booleen	vl_Creation;
	
	/*A
	** Definition des variables locales
	*/
	
	vl_Numero 		= va_Echangeur_in.Numero;
	strcpy( vl_NomEchangeur, va_Echangeur_in.NomEchangeur );
	strcpy( vl_Abreviation, va_Echangeur_in.Abreviation );
	strcpy( vl_NomSortie, va_Echangeur_in.NomSortie );
	vl_Bifurcation		= va_Echangeur_in.Bifurcation;
	strcpy( vl_Autoroute, va_Echangeur_in.Autoroute );
	vl_PR			= va_Echangeur_in.PR;
	vl_PRentreeSens1	= va_Echangeur_in.PRentreeSens1;
	vl_VoiesEntreeSens1	= va_Echangeur_in.VoiesEntreeSens1;
	vl_PeageEntreeSens1	= va_Echangeur_in.PeageEntreeSens1;
	vl_PRentreeSens2	= va_Echangeur_in.PRentreeSens2;
	vl_VoiesEntreeSens2	= va_Echangeur_in.VoiesEntreeSens2;
	vl_PeageEntreeSens2	= va_Echangeur_in.PeageEntreeSens2;
	vl_PRsortieSens1	= va_Echangeur_in.PRsortieSens1;
	vl_VoiesSortieSens1	= va_Echangeur_in.VoiesSortieSens1;
	vl_PeageSortieSens1	= va_Echangeur_in.PeageSortieSens1;
	vl_PRsortieSens2	= va_Echangeur_in.PRsortieSens2;
	vl_VoiesSortieSens2	= va_Echangeur_in.VoiesSortieSens2;
	vl_PeageSortieSens2	= va_Echangeur_in.PeageSortieSens2;
	vl_Creation		= va_Echangeur_in.Creation;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO84_Ajouter_Echangeur : Base = %s\tNumero = %d\tEchangeur = %s\tAbrev = %s\tNomSortie = %s\tBifurcation = %d\tAutoroute = %s\tPR = %d\tPRes1 = %d\tVes1 = %d\tPes1 = %d\tPres2 = %d\tVes2 = %d\tPes2 = %d\tPRss1 = %d\tVss1 = %d\tPss1 = %d\tPrss2 = %d\tVss2 = %d\tPss2 = %d\tCreation = %d\n",
			va_Basedd_in,
			va_Echangeur_in.Numero,
			va_Echangeur_in.NomEchangeur,
			va_Echangeur_in.Abreviation,
			va_Echangeur_in.NomSortie,
			va_Echangeur_in.Bifurcation,
			va_Echangeur_in.Autoroute,
			va_Echangeur_in.PR,
			va_Echangeur_in.PRentreeSens1,
			va_Echangeur_in.VoiesEntreeSens1,
			va_Echangeur_in.PeageEntreeSens1,
			va_Echangeur_in.PRentreeSens2,
			va_Echangeur_in.VoiesEntreeSens2,
			va_Echangeur_in.PeageEntreeSens2,
			va_Echangeur_in.PRsortieSens1,
			va_Echangeur_in.VoiesSortieSens1,
			va_Echangeur_in.PeageSortieSens1,
			va_Echangeur_in.PRsortieSens2,
			va_Echangeur_in.VoiesSortieSens2,
			va_Echangeur_in.PeageSortieSens2,
			va_Echangeur_in.Creation );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Numero,"@va_Numero_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_tinyint_input(Numero) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomEchangeur,"@va_NomEchangeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_char_input(NomEchangeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Abreviation,"@va_Abreviation_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_char_input(Abreviation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSortie,"@va_NomSortie_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_char_input(NomSortie) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Bifurcation,"@va_Bifurcation_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(Bifurcation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRentreeSens1,"@va_PRentreeSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_input(PRentreeSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_VoiesEntreeSens1,"@va_VoiesEntreeSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_tinyint_input(VoiesEntreeSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_PeageEntreeSens1,"@va_PeageEntreeSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(PeageEntreeSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRentreeSens2,"@va_PRentreeSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_input(PRentreeSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_VoiesEntreeSens2,"@va_VoiesEntreeSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_tinyint_input(VoiesEntreeSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_PeageEntreeSens2,"@va_PeageEntreeSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(PeageEntreeSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRsortieSens1,"@va_PRsortieSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_input(PRsortieSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_VoiesSortieSens1,"@va_VoiesSortieSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_tinyint_input(VoiesSortieSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_PeageSortieSens1,"@va_PeageSortieSens1_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(PeageSortieSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PRsortieSens2,"@va_PRsortieSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_input(PRsortieSens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_VoiesSortieSens2,"@va_VoiesSortieSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_tinyint_input(VoiesSortieSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_PeageSortieSens2,"@va_PeageSortieSens2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(PeageSortieSens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Creation,"@va_Creation_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(Creation) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_ retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	    					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao84 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	  					free(Tab_donnees2);
	    					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao84 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	    				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	    				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




