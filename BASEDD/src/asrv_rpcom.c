/*E*/
/*Fichier : $Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $      Release : $Revision: 1.36 $        Date : $Date: 2021/06/08 09:50:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE RP * FICHIER asrv_rpcom.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* 	asrv_start_handler : 
*	Permet d'enregister dans le serveur 
*    	les Register Procedures avec le descriptif de leur parametres.	
*********************************
*	asrv_err_handler :
*	La CS-Library appelle CS_ERR_HANDLER quand apparait une erreur
*	On logue une erreur et on revient dans la boucle principale.
*********************************
*	asrv_server_err_handler :
*	Erreur handler pour le serveur : Quand une erreur est detectee par
*  	l Open Serveur, on passe dans cette fct qui logue l'erreur, la decrit,
*  	l'indique dans les traces (exp : mauvais passage de param a une RP).
*********************************
*	asrv_rp_shutdown : Arret de L Open SRV via le isql ou le SQL Srv .
*  	On passe un passwd = yes pour pouvoir l arreter.
*********************************
*	asrv_regparam_in : Encapsule srv_regparam pour un type CS_INT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Utilise dans asrv_start_handler.
*********************************
*	asrv_regparam_char : Encapsule srv_regparam pour un type CS_CHAR
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Utilise dans asrv_start_handler.
*********************************
*	asrv_regparam_smallint : Encapsule srv_regparam pour un type CS_SMALLINT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Utilise dans asrv_start_handler.
*********************************
*	asrv_regparam_tinyint : Encapsule srv_regparam pour un type CS_TINYINT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Utilise dans asrv_start_handler.
********************************** 
*	
*	asrv_regparam_date : Encapsule srv_regparam pour un type CS_DATETIME
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Utilise dans asrv_start_handler.
*
**********************************
*
*	asrv_lire_smallint : Encapsule srv_bind pour un type CS_SMALLINT
*	Prepare la lecture d un parametre recu 
*	dans une Stored Procedure(XZAO....)
*	Utilitaire pour les Open Client.
*
**********************************
*
*	asrv_lire_int : Encapsule srv_bind pour un type CS_INT
*	Prepare la lecture d un parametre recu 
*	dans une Stored Procedure(XZAO....)
*	Utilitaire pour les Open Client.
*
**********************************
*
*	asrv_lire_tinyint : Encapsule srv_bind pour un type CS_TINYINT
*	Prepare la lecture d un parametre recu 
*	dans une Stored Procedure(XZAO....)
*	Utilitaire pour les Open Client.
*
**********************************
*
*	asrv_lire_char : Encapsule srv_bind pour un type CS_CHAR
*	Prepare la lecture d un parametre recu 
*	dans une Stored Procedure(XZAO....)
*	Utilitaire pour les Open Client.
*
**********************************
*
*	asrv_lire_date : Encapsule srv_bind pour un type CS_DATETIME
*	Prepare la lecture d un parametre recu 
*	dans une Stored Procedure(XZAO....)
*	Utilitaire pour les Open Client.
*
**********************************
*
* 	asrv_lire_RTSRV_MSG : Lecture des messages du RTServeur 
*	afin de declancher les CallBack associees.
*	Cette fonction est appelee a chaque fin d'execution d'une RP.
* 	asrv_Cree_ERRMSG : Initialise une structure de type message
*	a renvoyer au SQL Serveur faisant appel a une RP qui a renvoye
*	une erreur.
*
**********************************
* HISTORIQUE :
*
* MERCIER O.	31 Aug 1994	: Creation
********************
* MERCIER O.    29 Sep 1994     : Modification
* Ajout dans srv_server_err_handler() d un renvoie de status = -1
* Si le canal de communication avec le client est toujours actif
* et ce apres lui avoir renvoye le descriptif de l'erreur commise.
********************
* MERCIER O.	29 Sep 1994	: Modification
*	Rajoue des flags  static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_mrp*" ;
********************
* MERCIER O.	25 Oct 1994	: Modification		v1.8
*		des parametres de la arrp03. 0 -> 5 param.
********************
* MERCIER O.	31 Oct 1994	: Ajout			v1.9
*		des parametres de la aarp01.  12 param.
********************
* MERCIER O.	31 Oct 1994	: Modification		v1.10
*		Mise a jout des commentaires
* VERDIER P.	02 Sep 1996	: Ajout param ACRP01 V1.11
* NIEPCERON	03 Mar 1997	: Ajout param ACRP01 V1.12
* TORREGROSSA	09 Jan 1998	: Ajout param AARP01 V1.13
* NIEPCERON     31 Mai 1999	: Modif de asrv_lire_RTSRV_MSG suite a l'installation d'un patch Open Server 10.0.4 v1.14
* NIEPCERON    23 Nov 1999	: Modif de asrv_lire_RTSRV_MSG v1.16
* JMG		11/10/05	: ajout PMVA BAF BAD 1.18
* JBL		Nov 2007	: DEM 663
* JMG		02/10/09	: FACTIS DEM914
* JPL		03/11/09	: Ajout de trois parametres a ACRP19 (DEM 917) 1.22
* JMg		24/03/11 	: ajout aarp11 1.23
* JMG		13/01/12	: tdp etendu DEM/1014 1.24
* VR		05/03/12	: Ajout PRV (DEM/1016)
* JMG		11/11/12	: ajout destination
* LCL		02/03/18	: ajout CEF CTRL_FEUX 1.27 DEM1284
* ABE		27/04/20	: ajout arpDatex2 SAE-DEM130 V1.28
* ABE		24/09/20	: Revert modification precedente (V1.29) SAE-DEM130
* ABE		26/11/20	: Remise de la version 1.29 pour publication datex2 sur ajout FMC au CI SAE-DEM130 v1.31
* ABE		17/12/20	: Ajout ACRP70 pour IMU DEM-SAE155 1.32
* ABE		06/04/20	: Ajout ACRP80 pour SONO DEM-SAE244 1.33
* ABE		08/06/21	: Fix acrp80 pour SONO DEM-SAE244 1.36
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
* GGY		14/11/23	: Ajout ACRP16 (DEM483)
* GGY		15/01/24	: Ajout appel aarp02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_rpcom.h"
#include "xzems.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_rpcom" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'enregister dans le serveur les Register Procedures
*  avec le descriptif de leur parametres.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asrv_start_handler(SRV_SERVER      *server)

/*
* ARGUMENTS EN ENTREE :
*    server :  pointeur sur une structure de controle pour 
*              une application Open Serveur
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*	retourne : CS_SUCCEED | CS_FAIL
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*  Permet d installer les register Procedures ainsi que la gestion
*  des evenements types erreurs.
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_start_handler" ;
	CS_BOOL         vl_ok;
	SRV_PROC        *pl_srvproc;

	/*A Initialization de la valeur de retour.     */
	vl_ok = CS_TRUE;

	/*A Creation d'un service de gestion des RP.*/
	if ((pl_srvproc = srv_createproc(server)) == (SRV_PROC *)NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_createproc a echoue \n");
		XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : srv_createproc sort avec %d",CS_FAIL);
		return (CS_FAIL);
	}
	
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_SGBD|XZSTC_DEBUG1,XZSTC_NECRAN);

        /*A
        **  Enregistrement de la procedure asrv_rp_shutdown de type Gateway.
        */
        if (vl_ok && srv_regdefine(pl_srvproc, "asrv_rp_shutdown", CS_NULLTERM,
                (SRV_EVENTHANDLE_FUNC)asrv_rp_shutdown) != CS_SUCCEED)
	{
                vl_ok = CS_FALSE;
	}
	
	if (vl_ok && asrv_regparam_char(pl_srvproc, "@passwd")!= CS_SUCCEED)
     
	{
               vl_ok = CS_FALSE;
	}
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
		vl_ok = CS_FALSE;
	}
	
	/*A
	** Enregistrement de la rpc acrp01
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp01" est le nom de la procedure a enregistrer
	**
	*/	
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP01", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp01) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}	
	/* 
       	** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
       	*/
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@value1")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
	** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	/*if (vl_ok && srv_regparam(pl_srvproc, "@value2", CS_NULLTERM, CS_CHAR_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/
                
         if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value2")!= CS_SUCCEED)      
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
       	** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/
        /*if (vl_ok && srv_regparam(pl_srvproc, "@value3", CS_NULLTERM, CS_TINYINT_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/
                
        if (vl_ok && asrv_regparam_tinyint (pl_srvproc, "@value3") != CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }       
        /* 
       	** 4 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
       	if (vl_ok && asrv_regparam_tinyint (pl_srvproc, "@value4") != CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }      
        /* 
	** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/	
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value5")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
	** 6 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/	
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value6")!= CS_SUCCEED)       
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
       	** 7 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/
       	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value7")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
       	** 8 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/  	
       	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value8")!= CS_SUCCEED)	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
	** 9 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/	
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value9")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }      
        /* 
	** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value10")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
       	** 11 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/
       	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value11")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }      
        /* 
       	** 12 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
       	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value12")!= CS_SUCCEED)      
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }       
        /* 
	** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value13")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
       	** 14 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
       	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value14")!= CS_SUCCEED)       	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }       
        /* 
        ** Champs de la structure XDY_Etat_Picto
        */        
        /* 
       	** 15 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
       	*/
       	if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@value15")!= CS_SUCCEED)	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }      
        /* 
	** 16 eme champ de XDY_Etat_PMV : type Texte de XDC_Lon_TxtPicto 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value16")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
	** 17 eme champ de XDY_Etat_PMV : type XDY_Picto 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value17")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }        
        /* 
	** 18 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt 
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value18")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 19 eme caracterisant la commande : type int = CS_INT 
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@value19")!= CS_SUCCEED)       
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }       
        /* 
	** 20 eme caracterisant la commande type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value20")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
        	vl_ok = CS_FALSE;
        }
	/*
	** 21 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value21")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}
	/*
	** 22 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value22")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}

	/* 
       	** 23 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value23")!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}

	/* 
       	** 24 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value24")!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}

	/* 
       	** 25 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_regparam_tinyint(pl_srvproc, "@value25")!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}

	/* 
       	** 26 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value26")!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
		vl_ok = CS_FALSE;
	}



        /*
        ** Enregistrement des champs acrp01
        */        
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp01");
       		vl_ok = CS_FALSE;  
	}	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp01 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp01 a echoue");
	}	
	/*A
	** Fin de l enregistrement de acrp01
	*/	
	/*A
	** Enregistrement de la rpc acrpEXEMPLE
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrpEXEMPLE" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "acrpEXEMPLE", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrpEXEMPLE) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrpEXEMPLE");
		vl_ok = CS_FALSE;
	}	
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value1")!= CS_SUCCEED)	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrpEXEMPLE");
        	vl_ok = CS_FALSE;
        }        
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value2")!= CS_SUCCEED)	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrpEXEMPLE");
        	vl_ok = CS_FALSE;
        }       
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrpEXEMPLE");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrpEXEMPLE reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrpEXEMPLE a echoue");
	}	
	/*A
	** Fin de l enregistrement de acrpEXEMPLE
	*/	
	/*A
	** Enregistrement de la rpc AARP01
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "AARP01" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "AARP01", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)aarp01) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp03");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  aarp01: va_numAlerte_in de type XDY_Entier = CS_INT
       	*/
       	if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_numAlerte_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_numAlerte_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 2 eme champ : va_DateTime_in : type  = CS_DATETIME
       	*/
	if (vl_ok &&  asrv_regparam_date (pl_srvproc, "@va_DateTime_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01", "@va_DateTime_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 3 eme champ : va_typeAlerte_in de type  XDY_Octet : tinyint
       	*/
	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_typeAlerte_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_typeAlerte_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 4 eme champ : va_nomType_in type CS_CHAR de long XDC_Lon_NomSysteme
       	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_nomType_in")!= CS_SUCCEED)        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_nomType_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 5 eme champ : va_typeEqt_in de type  XDY_Octet : tinyint
       	*/
	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_typeEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_typeEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 6 eme champ : va_numEqt_in de type  XDY_Mot : smallint
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_numEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_numEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 7 eme champ :va_Nom_Eqt_in type CS_CHAR de long XDC_Lon_NomEqt
       	*/
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Nom_Eqt_in")!= CS_SUCCEED)   
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_Nom_Eqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 8 eme champ :va_texte_in type CS_CHAR de long XDC_Lon_TexteAlerte
       	*/
       	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_texte_in")!= CS_SUCCEED)    
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_texte_in");
        	vl_ok = CS_FALSE;
        }
       	/* 
       	** 9 eme champ : va_autoroute_in de type  XDY_Octet : tinyint
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_autoroute_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_autoroute_in");
        	vl_ok = CS_FALSE;
        }
       	/* 
       	** 10 eme champ : va_pr_in de type  XDY_PR (long) : CS_INT
       	*/
       	if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_pr_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_pr_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 11 eme champ : va_sens_in de type  XDY_Sens (unsigned char) : CS_TINYINT
       	*/
	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_sens_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_sens_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 12 eme champ : va_creation_in de type  XDY_Booleen (unsigned char) : CS_TINYINT
       	*/
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_creation_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_creation_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 13 eme champ : va_SiteAlerte_ine type  XDY_Octet : CS_TINYINT
       	*/
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_SiteAlerte_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP01","@va_SiteAlerte_in");
        	vl_ok = CS_FALSE;
        }

        /*A
        ** Enregistrement dans le protocole de communication de la RP des paramaetres decris ci-dessus.
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur aarp01");
       		vl_ok = CS_FALSE;  
	}
	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp aarp01 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asrv_start_handler : enregistrement de la rp aarp01 a echoue");
	}
	/*A
	** Fin de l enregistrement de AARP01
	*/

	/* Enregistrement de la rpc AARP02
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "AARP02" est le nom de la procedure a enregistrer */
	if (vl_ok && srv_regdefine(pl_srvproc, "AARP02", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)aarp02) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister AARP02");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** AARP02 IN : ***************");

	/* 1 er champ de  AARP02: @vl_numero de type XDY_Entier = CS_INT */
	if (vl_ok && asrv_regparam_int(pl_srvproc, "@vl_numero")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %d de AARP02","@vl_numero");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ numero %d de AARP02","@vl_numero");

	/* 2 eme  champ de  AARP02: @vl_cle de type XDY_Octet : CS_SMALLINT  */
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@vl_cle")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  AARP02","@vl_cle");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ bandeau %s de AARP02","@vl_cle");

	/* 3 eme  champ de  AARP02: @vl_action de type XDY_Entier = CS_INT */
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@vl_action")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  AARP02","@vl_action");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ vitesse %s de AARP02","@vl_action");
	
	/* 4 eme  champ de  AARP02: @vl_valeur de type XDY_Entier = CS_INT */
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@vl_valeur")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  AARP02","@vl_valeur");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ vitesse %s de AARP02","@vl_valeur");
	
	/* Enregistrement des champs de AARP02 */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur AARP02");
		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp AARP02 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp AARP02 a echoue");
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** AARP02 OUT : ***************");
	/* Fin de l enregistrement de AARP02 */

	/*A
	** Enregistrement de la rpc ACRP03
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP03" est le nom de la procedure a enregistrer
	**
	*/
	
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP03", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp03) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp03");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  ACRP03: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP03","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
         /* 
	** 2 eme  champ de  ACRP03: @va_Scenario_in de type XDY_Scenario : char de XDC_Lon_Scenario 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Scenario_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP03","@va_Scenario_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP03: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP03","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 4 eme champ de  ACRP03: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de acrp01", "@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP03
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp03");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp03 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp03 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP03
	*/


	/*A
	** Enregistrement de la rpc ACRP60
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP60" est le nom de la procedure a enregistrer
	**
	*/
	
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP60", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp60) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp60");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  ACRP60: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP60","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
         /* 
	** 2 eme  champ de  ACRP60: @va_PlanDeFeux_in de type XDY_Octet : tinyint
	*/
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_typeAlerte_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP60","@va_PlanDeFeux_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP60: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP60","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 4 eme champ de  ACRP60: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de acrp60", "@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP60
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp60");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp60 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp60 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP60
	*/

	/*A
	** Enregistrement de la rpc ACRP70
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP70" est le nom de la procedure a enregistrer
	**
	*/
	
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP70", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp70) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister ACRP70");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  ACRP70: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70","@va_NumEqt_in");
        	vl_ok = CS_FALSE;
        }
         /* 
	** 2 eme  champ de  ACRP70: @va_ChaineIMU_in de type XDY_ChaineIMU  : char de long XDC_Lon_ChaineIMU
	*/
        if (vl_ok && asrv_regparam_char(pl_srvproc, "@va_ChaineIMU_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP70","@va_ChaineIMU_in");
        	vl_ok = CS_FALSE;
        }

	/*
	** 3 eme  champ de  ACRP70: @va_Intervalle_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_Intervalle_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70","@va_Intervalle_in");
		vl_ok = CS_FALSE;
	}

	/*
	** 4 eme  champ de  ACRP70: @va_Cycles_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_Cycles_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70","@va_Cycles_in");
		vl_ok = CS_FALSE;
	}

	/*
	** 5 eme  champ de  ACRP70: @va_Duree_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_Duree_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70","@va_Duree_in");
		vl_ok = CS_FALSE;
	}

        /* 
	** 6 eme  champ de  ACRP70: @va_NumeroAction_out de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NumeroAction_out")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70","@va_NumeroAction_out");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 7 eme champ de  ACRP70: @@vl_NomMachine  de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@vl_NomMachine")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP70", "@vl_NomMachine ");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP70
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP70");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP70 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP70 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP70
	*/
	
	/*A
	** Enregistrement de la rpc ACRP80
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP80" est le nom de la procedure a enregistrer
	**
	*/
	
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP80", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp80) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister ACRP80");
		vl_ok = CS_FALSE;
	}

         /* 
	** 1 eme  champ de  ACRP80: @va_Message_in de type XDY_Message  : char de long XDC_Lon_MessageSONO
	*/
        if (vl_ok && asrv_regparam_char(pl_srvproc, "@va_Message_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP80","@va_Message_in");
        	vl_ok = CS_FALSE;
        }

	/*
	** 2 eme  champ de  ACRP80: @va_Autoroute_in de type XDY_NomAuto : char de XDC_Lon_NomAuto
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Autoroute_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80","@va_Autoroute_in");
		vl_ok = CS_FALSE;
	}

	/*
	** 3 eme  champ de  ACRP80: @va_Sens_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sens_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80","@va_Sens_in");
		vl_ok = CS_FALSE;
	}

	/*
	** 4 eme  champ de  ACRP80: @va_PR_debut_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_PR_debut_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80","@va_PR_debut_in");
		vl_ok = CS_FALSE;
	}
	
	/*
	** 5 eme  champ de  ACRP80: @va_PR_fin_in de type XDY_Entier
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_PR_fin_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80","@va_PR_fin_in");
		vl_ok = CS_FALSE;
	}

    /* 
	** 6 eme  champ de  ACRP80: @va_NumeroAction_out de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NumeroAction_out")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80","@va_NumeroAction_out");
        	vl_ok = CS_FALSE;
        }
        
    /* 
	** 7 eme champ de  ACRP80: @@vl_NomMachine  de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@vl_NomMachine")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP80", "@vl_NomMachine ");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP80
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP80");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP80 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP80 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP80
	*/

	/*A
	** Enregistrement de la rpc ACRP15
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP15" est le nom de la procedure a enregistrer
	**
	*/
	

	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP15", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp15) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp15");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** acrp15 IN : ***************");
	
		/* 
       	** 1 er champ de  ACRP15: @va_Numero_in de type XDY_Entier = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Numero_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %d de ACRP15","@va_Numero_in");
        	vl_ok = CS_FALSE;
        }
       	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ numero %d de ACRP15","@va_Numero_in");

    /* 
	** 2 eme  champ de  ACRP15: @va_Bandeau_in de type XDY_Bandeau : char de XDC_Lon_Scenario 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Bandeau_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP15","@va_Bandeau_in");
        	vl_ok = CS_FALSE;
        }
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ bandeau %s de ACRP15","@va_Bandeau_in");
	/* 
	** 3 eme  champ de  ACRP15: @va_Vitesse_in de type XDY_VitesseRegul : char de XDC_Lon_Scenario 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Vitesse_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP15","@va_Vitesse_in");
        	vl_ok = CS_FALSE;
        }
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ vitesse %s de ACRP15","@va_Vitesse_in");
	
    /* 
	** 4 eme  champ de  ACRP15: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %d de ACRP15","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
        XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ noCMd %d de ACRP15","@va_NoCMD_in");
    /* 
	** 5 eme champ de  ACRP15: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP15", "@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ nom Machine %s de ACRP15","@va_NomMachine_in");
	/*
        ** Enregistrement des champs de ACRP15
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP15");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP15 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP15 a echoue");
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** acrp15 OUT : ***************");
	/*A
	** Fin de l enregistrement de ACRP15
	*/
	

	/* Enregistrement de la rpc ACRP16
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP16" est le nom de la procedure a enregistrer */
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP16", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp16) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp16");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** acrp16 IN : ***************");

	/* 1 er champ de  acrp16: @va_NumEqt_in de type XDY_Entier = CS_SMALLINT */
	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumEqt_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %d de acrp16","@va_NumEqt_in");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ numero %d de ACRP16","@va_NumEqt_in");

	/* 2 eme  champ de  ACRP16: @va_Bandeau_in de type XDY_Bandeau_PIC : char de XDC_Lon_Scenario  */
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Bandeau_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP16","@va_Bandeau_in");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ bandeau %s de ACRP16","@va_Bandeau_in");

	/* 3 eme  champ de  ACRP16: @va_Flash_in */
	if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@va_Flash_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP16","@va_Flash_in");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ vitesse %s de ACRP16","@va_Flash_in");
	
	/* 4 eme  champ de  ACRP16: @va_Picto_in de type XDY_Picto_PIC */
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Picto_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP16","@va_Picto_in");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ vitesse %s de ACRP16","@va_Picto_in");
	
    /* 5 eme  champ de  ACRP16: @va_NumeroAction_out de type  int */
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NumeroAction_out")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %d de ACRP16","@va_NumeroAction_out");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ noCMd %d de ACRP16","@va_NumeroAction_out");

	/* 6 eme champ de  ACRP16: @vl_NomMachine de type XDY_NomMachine : char de XDC_Lon_NomMachine */
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@vl_NomMachine")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP16", "@vl_NomMachine");
		vl_ok = CS_FALSE;
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"TASRV : le champ nom Machine %s de ACRP16","@vl_NomMachine");

	/* Enregistrement des champs de ACRP16 */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP16");
		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP16 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP16 a echoue");
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ********** ACRP16 OUT : ***************");
	/* Fin de l enregistrement de ACRP16 */

	/*A
        ** Enregistrement de la rpc ACRP99
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "ACRP03" est le nom de la procedure a enregistrer
        **
        */

        if (vl_ok && srv_regdefine(pl_srvproc, "ACRP99", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)acrp99) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp99");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  ACRP99: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP99","@va_NumeroEqt_in");
                vl_ok = CS_FALSE;
        }

	/*
        ** 2 eme  champ de  ACRP99: @va_Scenario_in de type XDY_Scenario : char de XDC_Lon_Scenario
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Scenario_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP99","@va_Scenario_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme  champ de  ACRP99: @va_NoCMD_in de type  int
        */
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP99","@va_NoCMD_in");
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de  ACRP99: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP99", "@va_NomMachine_in");
                vl_ok = CS_FALSE;
        }

	/*
        ** Enregistrement des champs de ACRP99
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP99");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP99 reussi ");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP99 a echoue");
        }
        /*A
        ** Fin de l enregistrement de ACRP99
        */


        /*A
        ** Enregistrement de la rpc AARP11
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "AARP11" est le nom de la procedure a enregistrer
        **
        */
        if (vl_ok && srv_regdefine(pl_srvproc, "AARP11", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)aarp11) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister aarp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  aarp11: va_numAlerte_in de type XDY_Entier = CS_INT
        */
        if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_numAlerte_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_numAlerte_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 2 eme champ : va_DateTime_in : type  = CS_DATETIME
        */
        if (vl_ok &&  asrv_regparam_date (pl_srvproc, "@va_DateTime_in")!= CS_SUCCEED)

        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11", "@va_DateTime_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme champ : va_typeAlerte_in de type  XDY_Octet : tinyint
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_typeAlerte_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_typeAlerte_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 4 eme champ : va_nomType_in type CS_CHAR de long XDC_Lon_NomSysteme
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_nomType_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_nomType_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 5 eme champ : va_typeEqt_in de type  XDY_Octet : tinyint
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_typeEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_typeEqt_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 6 eme champ : va_numEqt_in de type  XDY_Mot : smallint
        */
        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_numEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_numEqt_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 7 eme champ :va_Nom_Eqt_in type CS_CHAR de long XDC_Lon_NomEqt
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Nom_Eqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_Nom_Eqt_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 8 eme champ :va_texte_in type CS_CHAR de long XDC_Lon_TexteAlerte
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_texte_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_texte_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 9 eme champ : va_autoroute_in de type  XDY_Octet : tinyint
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_autoroute_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_autoroute_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 10 eme champ : va_pr_in de type  XDY_PR (long) : CS_INT
        */
        if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_pr_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_pr_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 11 eme champ : va_sens_in de type  XDY_Sens (unsigned char) : CS_TINYINT
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_sens_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_sens_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 12 eme champ : va_creation_in de type  XDY_Booleen (unsigned char) : CS_TINYINT
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_creation_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_creation_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 13 eme champ : va_SiteAlerte_ine type  XDY_Octet : CS_TINYINT
        */
        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_SiteAlerte_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_SiteAlerte_in");
                vl_ok = CS_FALSE;
        }

        /*
        ** 14 eme champ : va_Portion_in type  XDY_Octet : CS_SMALLINT
        */
        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Portion_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_Portion_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 15 eme champ : va_Translation_in type  XDY_Octet : CS_INT
        */
        if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_Translation_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de AARP11","@va_Translation_in");
                vl_ok = CS_FALSE;
        }
        /*A
        ** Enregistrement dans le protocole de communication de la RP des paramaetres decris ci-dessus.
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur aarp11");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp aarp11 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asrv_start_handler : enregistrement de la rp aarp11 a echoue");
        }
        /*A
        ** Fin de l enregistrement de AARP11
        */

	/*A
	** Enregistrement de la rpc AARPDatex2
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "AARPDATEX2" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "AARPDATEX2", CS_NULLTERM,
		        (SRV_EVENTHANDLE_FUNC)aarpDatex2) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister aarpdatex2");
		vl_ok = CS_FALSE;
	}
	/*
	** 1 er champ de  aarpdatex2: va_numAlerte_in de type XDY_Entier = CS_INT
	*/
	if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_NumEvenement_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aarpdatex2","@va_numAlerte_in");
		vl_ok = CS_FALSE;
	}

	/*
	** 13 eme champ : va_SiteAlerte_ine type  XDY_Octet : CS_TINYINT
	*/
	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_CleEvenement_in")!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aarpdatex2","@va_SiteAlerte_in");
		vl_ok = CS_FALSE;
	}        
	/*A
	** Enregistrement dans le protocole de communication de la RP des paramaetres decris ci-dessus.
	*/
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur aarpdatex2");
		vl_ok = CS_FALSE;
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp aarpdatex2 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asrv_start_handler : enregistrement de la rp aarpdatex2 a echoue");
	}
	/*A
	** Fin de l enregistrement de aarpdatex2
	*/


	/*A
	** Enregistrement de la rpc ACRP06
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp06" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP06", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp06) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp06");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  ACRP06: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP06","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP06: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP06","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP06: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP06","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP06
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp06");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp06 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp06 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP06
	*/
	

	/*A
	** Enregistrement de la rpc ACRP40
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp40" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP40", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp40) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp40");
		vl_ok = CS_FALSE;
	}
	
		/* 
       	** 0 champ de  ACRP40: @va_TypEqt_in de type CS_INT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP40","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }

		/* 
       	** 1 er champ de  ACRP40: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP40","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP40: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP40","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP40: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP40","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP40
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp40");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp40 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp40 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP40
	*/
	
	/*A
	** Enregistrement de la rpc ACRP41
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp41" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP41", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp41) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp41");
		vl_ok = CS_FALSE;
	}
		/* 
       	** 0 champ de  ACRP41: @va_TypEqt_in de type CS_TINYINT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
			XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP41","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 1 er champ de  ACRP41: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP41","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP41: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP41","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP41: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP41","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP41
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp41");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp41 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp41 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP41
	*/

	/*A
	** Enregistrement de la rpc ACRP42
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp42" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP42", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp42) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp42");
		vl_ok = CS_FALSE;
	}
		/* 
       	** 0 champ de  ACRP42: @va_TypEqt_in de type CS_INT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
			XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP42","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 1 er champ de  ACRP42: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP42","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP42: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP42","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP42: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP42","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP42
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp42");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp42 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp42 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP42
	*/		
	
	/*A
	** Enregistrement de la rpc ACRP43
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp43" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP43", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp43) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp43");
		vl_ok = CS_FALSE;
	}
		/* 
       	** 0 champ de  ACRP43: @va_TypEqt_in de type CS_INT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
			XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP43","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 1 er champ de  ACRP43: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP43","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP43: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP43","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP43: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP43","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP43
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp43");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp43 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp43 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP43
	*/		
	
	/*A
	** Enregistrement de la rpc ACRP44
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp44" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP44", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp44) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp44");
		vl_ok = CS_FALSE;
	}
		/* 
       	** 0 champ de  ACRP44: @va_TypEqt_in de type CS_INT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
			XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP44","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 1 er champ de  ACRP44: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP44","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP44: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP44","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP44: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP44","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP44
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp44");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp44 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp44 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP44
	*/	
	
	/*A
	** Enregistrement de la rpc ACRP45
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp45" est le nom de la procedure a enregistrer
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP45", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp45) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp45");
		vl_ok = CS_FALSE;
	}
		/* 
       	** 0 champ de  ACRP45: @va_TypEqt_in de type CS_INT
       	*/
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypEqt_in")!= CS_SUCCEED)
        {
			XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP45","@va_TypEqt_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 1 er champ de  ACRP45: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP45","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP45: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP45","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP45: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
      {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP45","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
  	/*
        ** Enregistrement des champs de ACRP45
        */
 	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp45");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp45 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp45 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP45
	*/		

	
	/*A
	** Enregistrement de la rpc ACRP08
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "acrp08" est le nom de la procedure a enregistrer.
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP08", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp08) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp08");
		vl_ok = CS_FALSE;
	}
	/* 
       	** 1 er champ de  ACRP08: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP08","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP08: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Sequence_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP08","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP08: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP08","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP08
        */
  	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp08");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp08 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp08 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP08
	*/



	/*A
	** Enregistrement de la rpc ACRP19
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ACRP19" est le nom de la procedure a enregistrer.
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ACRP19", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)acrp19) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp19 ");
		vl_ok = CS_FALSE;
	}
        /* 
	** 1 er  champ de  ACRP19: commande MTMA de type  tinyint
	*/
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_commande_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP19","@va_commande_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 2 eme  champ de  ACRP19: numero action TFM de type  int
	*/
	if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_numAction_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP19","@va_numAction_in");
        	vl_ok = CS_FALSE;
        }
        /* 
	** 3 eme  champ de  ACRP19: cle (site) action TFM de type  tinyint
	*/
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_siteAction_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP19","@va_siteAction_in");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de ACRP19
        */
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp19");
       		vl_ok = CS_FALSE;  
	}
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp19 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp19 a echoue");
	}
	/*A
	** Fin de l enregistrement de ACRP19
	*/



	/*A
	** Enregistrement de la rpc AMRP05
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "amrp05" est le nom de la procedure a enregistrer.
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "AMRP05", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)amrp05) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister amrp05");
		vl_ok = CS_FALSE;
	}
	/*
        ** Enregistrement des champs de AMRP05
        */
        
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur amrp05");
       		vl_ok = CS_FALSE;  
	}	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp amrp05 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp amrp05 a echoue");
	}	
	/*A
	** Fin de l enregistrement de AMRP05
	*/
	/*A
	** Enregistrement de la rpc AMRP07
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "amrp07"est le nom de la procedure a enregistrer.
	**
	*/
	
	if (vl_ok && srv_regdefine(pl_srvproc, "AMRP07", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)amrp07) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister amrp07");
		vl_ok = CS_FALSE;
	}
	
        /* 
	** 1 er  champ de  amrp07: @va_Sequence_in de type  tinyint
	*/
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_TypeEqt_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  amrp07","va_Sequence_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 2 eme champ de amrp07 : @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de amrp07","@va_NumeroEqt_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 3 eme  champ de  amrp07: @va_NoCMD_in de type  int 
	*/
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_Etat_Eqt_in")!= CS_SUCCEED)
	
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de amrp07","@va_NoCMD_in");
        	vl_ok = CS_FALSE;
        }
	/*
        ** Enregistrement des champs de AMRP07
        */
        
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur amrp07");
       		vl_ok = CS_FALSE;  
	}
	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp amrp07 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp amrp07 a echoue");
	}
	
	/*A
	** Fin de l enregistrement de AMRP07
	*/
	
	
	
	/*A
	** Enregistrement de la rpc APRP22
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "APRP22"est le nom de la procedure a enregistrer.
	**
	*/
	
	
	if (vl_ok && srv_regdefine(pl_srvproc, "APRP22", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)aprp22) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister aprp22");
		vl_ok = CS_FALSE;
	}
	
	
	/* 
       	** 1 er champ de aprp22 : @va_NumTE_in de type = CS_SMALLINT
       	*/
       	
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Enumero_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Enumero_in");
        	vl_ok = CS_FALSE;
        }
        
        
        /* 
	** 2 eme  champ de aprp22 : @va_Ecle_in de type  tinyint
	*/
	
	if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Ecle_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aprp22 ","@va_Ecle_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 3 eme champ de  APRP22: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aprp22", "@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
	** 4 eme champ de  APRP22: @va_Date_in de type CS_DATETIME 
	*/
	if (vl_ok &&  asrv_regparam_date (pl_srvproc, "@va_Date_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aprp22", "@va_Date_in");
        	vl_ok = CS_FALSE;
        }
        /* 
       	** 5 eme champ de aprp22 : @va_NumPA_in de type = CS_SMALLINT
       	*/
       	
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumPA_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_NumPA_in");
        	vl_ok = CS_FALSE;
        }
	/*A
        ** Enregistrement des champs de aprp22
        */
        
	
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur aprp22");
       		vl_ok = CS_FALSE;  
	}
	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp aprp22 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp aprp22 a echoue");
	}	
	
	/*A
	** Fin de l enregistrement de APRP22
	*/	
	/*A
	** Enregistrement de la rpc ARRP03
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "arrp03" est le nom de la procedure a enregistrer.
	**
	*/
	if (vl_ok && srv_regdefine(pl_srvproc, "ARRP03", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)arrp03) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister arrp03");
		vl_ok = CS_FALSE;
	}
	/*A
	** 1er champ de ARRP03 : @va_Numero_in type CS_INT 
	*/
	if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_Numero_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Numero_in");
        	vl_ok = CS_FALSE;
        }
        /*A
	** 2eme champ de ARRP03 : @va_Cle_in type CS_TINYINT 
	*/
	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_Cle_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Cle_in");
        	vl_ok = CS_FALSE;
        }
        /*A
	** 3eme champ de ARRP03 : @va_NomMachine_in type CS_CHAR 
	*/
	if (vl_ok && asrv_regparam_char(pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
        /*A
	** 4eme champ de ARRP03 : @va_NomSite_in type CS_CHAR 
	*/
	if (vl_ok && asrv_regparam_char(pl_srvproc, "@va_NomSite_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_NomSite_in");
        	vl_ok = CS_FALSE;
        }
        /*A
	** 5eme champ de ARRP03 : @va_TexteReveil_in type CS_CHAR 
	*/
	if (vl_ok && asrv_regparam_char(pl_srvproc, "@va_TexteReveil_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_TexteReveil_in");
        	vl_ok = CS_FALSE;
        }
        
	/*A
        ** Enregistrement des champs de AMRP03
        */
        
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur arrp03");
       		vl_ok = CS_FALSE;  
	}
	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp arrp03 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp arrp03 a echoue");
	}
	
	/*A
	** Fin de l enregistrement de ARRP03
	*/
	
	/*A
	** Enregistrement de la rpc ATRP18
	** pl_srvproc est un pointeur sur structure controle de thread interne
	** "ATRP18"est le nom de la procedure a enregistrer.
	**
	*/
	
	
	if (vl_ok && srv_regdefine(pl_srvproc, "ATRP18", CS_NULLTERM,
			(SRV_EVENTHANDLE_FUNC)atrp18) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister atrp18");
		vl_ok = CS_FALSE;
	}
	
	/* 
	** 1 er champ de  atrp18: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de atrp18", "@va_NomMachine_in");
        	vl_ok = CS_FALSE;
        }
	/* 
       	** 2 eme champ de atrp18 : @va_TypeEqt_in de type = CS_TINYINT
       	*/
       	
       	
       	if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypeEqt_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_TypeEqt_in");
        	vl_ok = CS_FALSE;
        }
        
        
        /* 
	** 3 eme  champ de atrp18 : @va_NumEqt_in de type  CS_SMALLINT
	*/
	
	if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@va_NumEqt_in")!= CS_SUCCEED)
        
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aprp22 ","@va_NumEqt_in");
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 4 eme champ de atrp18 : @va_Changement_in de type = CS_SMALLINT
       	*/
       	
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Changement_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Changement_in");
        	vl_ok = CS_FALSE;
        }
        
        
        /* 
       	** 5 eme champ de atrp18 : @va_Etat_in de type = CS_SMALLINT
       	*/
       	
       	
       	if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Etat_in")!= CS_SUCCEED)
        {
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Etat_in");
        	vl_ok = CS_FALSE;
        }
	/*A
        ** Enregistrement des champs de atrp18
        */
        
	
	if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur atrp18");
       		vl_ok = CS_FALSE;  
	}
	
	if(vl_ok)
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp atrp18 reussi");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp atrp18 a echoue");
	}
	
	
	
	/*A
	** Fin de l enregistrement de ATRP18
	*/

       /*A
        ** Enregistrement de la rpc ATRP19
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "ATRP19"est le nom de la procedure a enregistrer.
        **
        */
        if (vl_ok && srv_regdefine(pl_srvproc, "ATRP19", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)atrp19) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister atrp19");
                vl_ok = CS_FALSE;
        }
       /*
        ** 1 er champ de  atrp18: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)

        {
               XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de atrp18", "@va_NomMachine_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 2 eme champ de atrp18 : @va_TypeEqt_in de type = CS_TINYINT
        */


        if (vl_ok && asrv_regparam_tinyint(pl_srvproc, "@va_TypeEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_TypeEqt_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme  champ de atrp18 : @va_NumEqt_in de type  CS_SMALLINT
        */

        if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@va_NumEqt_in")!= CS_SUCCEED)

        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de aprp22 ","@va_NumEqt_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 4 eme champ de atrp18 : @va_Changement_in de type = CS_SMALLINT
        */


        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Changement_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Changement_in");
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme champ de atrp18 : @va_Etat_in de type = CS_SMALLINT
        */


        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_Etat_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Etat_in");
                vl_ok = CS_FALSE;
        }
        /* 6e champ*/
        if (vl_ok && asrv_regparam_int(pl_srvproc, "@va_Animation_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ","@va_Animation_in");
                vl_ok = CS_FALSE;
        }
        /*A
        ** Enregistrement des champs de atrp18
        */


        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur atrp19");
                vl_ok = CS_FALSE; 
        }

        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp atrp19 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp atrp19 a echoue");
        }





        /*PMVA DEBUT*/
        /*A
        ** Enregistrement de la rpc acrp11
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "acrp01" est le nom de la procedure a enregistrer
        **
        */
        if (vl_ok && srv_regdefine(pl_srvproc, "ACRP11", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)acrp11) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
        */

        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@value1")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        /*if (vl_ok && srv_regparam(pl_srvproc, "@value2", CS_NULLTERM, CS_CHAR_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/

         if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value2")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        /*if (vl_ok && srv_regparam(pl_srvproc, "@value3", CS_NULLTERM, CS_TINYINT_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/

        if (vl_ok && asrv_regparam_tinyint (pl_srvproc, "@value3") != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && asrv_regparam_tinyint (pl_srvproc, "@value4") != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value5")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 6 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value6")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 7 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value7")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 8 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value8")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 9 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value9")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value10")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 11 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value11")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 12 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value12")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value13")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 14 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value14")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 15 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value15")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 16 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value16")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 17 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value17")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 18 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value18")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 19 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value19")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 20 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value20")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 21 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value21")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 22 eme champ de XDY_Etat_PMV : FLASH
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value22")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }

        /*
        ** 25 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT seuil
        */


        if (vl_ok && (asrv_regparam_tinyint(pl_srvproc, "@value23")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }

        /*
        ** 23 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt destination un proche
        */
        if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@value24")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 24 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT destination un lointaine
        */
        if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value25")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }

	/*dest deux proche*/
        if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value26")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }

	/*dest deux lointaine*/
        if (vl_ok && (asrv_regparam_smallint(pl_srvproc, "@value27")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }


        /*
        ** 26 eme champ de XDY_Etat_PMV : type XDY_Mot = action
        */


        if (vl_ok && (asrv_regparam_int(pl_srvproc, "@value28")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }
        /*
        ** 27 eme caracterisant la commande type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value29")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }

        /*
        ** Enregistrement des champs acrp01
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp11");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp11 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp11 a echoue");
        }
        /*A
        ** Fin de l enregistrement de acrp01
        */
        /*PMVA FIN*/


        /*SAGA DEBUT*/
        /*A
        ** Enregistrement de la rpc acrp111
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "acrp01" est le nom de la procedure a enregistrer
        **
        */
        if (vl_ok && srv_regdefine(pl_srvproc, "ACRP111", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)acrp111) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT numeqt
        */

        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@value1")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV ligne1
        */
        /*if (vl_ok && srv_regparam(pl_srvproc, "@value2", CS_NULLTERM, CS_CHAR_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/

         if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value2")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT pmvtexte
        */
        /*if (vl_ok && srv_regparam(pl_srvproc, "@value3", CS_NULLTERM, CS_TINYINT_TYPE,
                SRV_NODEFAULT, (CS_BYTE *)NULL) != CS_SUCCEED)*/

        if (vl_ok && asrv_regparam_tinyint (pl_srvproc, "@value3") != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV alternat1
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value4")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 6 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV ligne2
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value5")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 7 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT pmvtexte
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value6")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 9 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV alternat2
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value7")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV ligne3
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value8")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 11 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT pmvtexte
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value9")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV alternat3
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value10")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 14 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV ligne4
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value11")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 15 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt pmvtexte
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value12")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV alternat4
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value13")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 14 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV ligne5
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value14")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 15 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt pmvtexte
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value15")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV alternat5
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value16")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 16 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt cligno
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@value17")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 16 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt lumi
        */
        if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@value18")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 16 eme champ de XDY_Etat_PMV : type char picto
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value19")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 17 eme champ de XDY_Etat_PMV : type XDY_Etat_PMV : type XDY_Ligne_PMV bandeau
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value20")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 19 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt barriere
        */
        if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@value21")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }
        /*
        ** 20 eme champ de XDY_Etat_PMV : type XDY_Octet = TinyInt scenario
        */
        if (vl_ok &&  asrv_regparam_smallint (pl_srvproc, "@value22")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }


        /*
        ** 26 eme champ de XDY_Etat_PMV : type XDY_Mot = action
        */


        if (vl_ok && (asrv_regparam_int(pl_srvproc, "@value23")!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp01");
                vl_ok = CS_FALSE;
        }
        /*
        ** 27 eme caracterisant la commande type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@value24")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister acrp11");
                vl_ok = CS_FALSE;
        }

        /*
        ** Enregistrement des champs acrp111
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp11");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp111 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp111 a echoue");
        }
        /*A
        ** Fin de l enregistrement de acrp111
        */
        /*SAGA FIN*/

        /*A
        ** Enregistrement de la rpc ACRP12
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "acrp12" est le nom de la procedure a enregistrer
        **
        */

        if (vl_ok && srv_regdefine(pl_srvproc, "ACRP12", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)acrp12) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp12");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  acrp12: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de acrp12","@va_NumeroEqt_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module11_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module11_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module12_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module12_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module15_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module15_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module21_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module21_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module22_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module22_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  acrp12: @va_Module25_in
        */
        if (vl_ok &&  asrv_regparam_tinyint (pl_srvproc, "@va_Module25_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  acrp12","@va_Module11_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme  champ de  acrp12: @va_NoCMD_in de type  int
        */
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de acrp12","@va_NoCMD_in");
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de  acrp12: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de acrp12", "@va_NomMachine_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** Enregistrement des champs de acrp12
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur acrp12");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp12 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp acrp12 a echoue");
        }
        /*A
        ** Fin de l enregistrement de acrp12
        */




        /*A
        ** Enregistrement de la rpc ACRP13
        ** pl_srvproc est un pointeur sur structure controle de thread interne
        ** "ACRP13" est le nom de la procedure a enregistrer
        **
        */

        if (vl_ok && srv_regdefine(pl_srvproc, "ACRP13", CS_NULLTERM,
                        (SRV_EVENTHANDLE_FUNC)acrp13) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regdefine n a pu enregister acrp13");
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  ACRP13: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && asrv_regparam_smallint(pl_srvproc, "@va_NumeroEqt_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP13","@va_NumeroEqt_in");
                vl_ok = CS_FALSE;
        }
         /*
        ** 2 eme  champ de  ACRP13: @va_Scenario_in de type XDY_Scenario : char de XDC_Lon_Scenario
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_Scenario_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de  ACRP13","@va_Scenario_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme  champ de  ACRP13: @va_NoCMD_in de type  int
        */
        if (vl_ok &&  asrv_regparam_int (pl_srvproc, "@va_NoCMD_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP13","@va_NoCMD_in");
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de  ACRP13: @va_NomMachine_in de type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        if (vl_ok &&  asrv_regparam_char (pl_srvproc, "@va_NomMachine_in")!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regparam n a pu enregister le champ %s de ACRP13", "@va_NomMachine_in");
                vl_ok = CS_FALSE;
        }
        /*
        ** Enregistrement des champs de ACRP13
        */
        if (vl_ok && srv_regcreate(pl_srvproc, (CS_INT *)NULL) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_regcreate a echoue sur ACRP13");
                vl_ok = CS_FALSE;
        }
        if(vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP13 reussi");
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : enregistrement de la rp ACRP13 a echoue");
        }
        /*A
        ** Fin de l enregistrement de ACRP13
        */







	/*A
        ** Nous n'avons plus besoin du service Thread 
        ** On l'arrete.
        */
        XZST_03EcritureTrace(XZSTC_SGBD,"asrv_start_handler : Arret du service Thread : on en a plus besoin");
        srv_dropproc(pl_srvproc);
        
	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : Fin de l'enregistrement des RPs");
        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_start_handler sort avec %d",(vl_ok ? CS_SUCCEED : CS_FAIL));
        return (vl_ok ? CS_SUCCEED : CS_FAIL);

}


/*^L*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Erreur handler
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asrv_err_handler(CS_CONTEXT      *pa_cp, CS_CLIENTMSG    *va_msg)

/*
* ARGUMENTS EN ENTREE :
*   Pointeur sur le context d ou provient l erreur
*    
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   <Retourne CS_SUCCEED dans tous les cas pour ne pas bloquer le OP Srv >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
** 	La CS-Library appelle CS_ERR_HANDLER quand apparait une erreur
**	On logue une erreur et on revient dans la boucle principale
*
------------------------------------------------------*/

{
        static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_err_handler";
        CS_CHAR         vl_mbuf[CS_MAX_MSG*2];     /* Buffer message. */
        CS_CHAR         sname[CS_MAX_NAME];     /* Le nom du serveur. */
        CS_INT          slen;                   /* La longueur du nom du serveur. */
        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : TASRV : asrv_err_handler est appelee");

        /*A
        ** Recupere le nom du serveur.
        */
        if( srv_props(pa_cp, CS_GET, SRV_S_SERVERNAME, sname, CS_MAX_NAME, &slen)
                == CS_FAIL )
        {
                /*B
                ** Une erreur est apparue.
                */
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_err_handler : srv_props a echoue pour recuperer le nom du serveur\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_err_handler sort avec %d malgre l erreur survenue",CS_SUCCEED);
                return CS_SUCCEED;
        }
        sname[slen] = '\0';

        /*A
        ** Construction du message d'erreur qui doit etre logue.
        */
        (CS_VOID)sprintf(vl_mbuf, "%s: Erreur CS-Library: Couche: %ld Origine:%ld Severite:%ld Numero:%ld Texte: %s\n",
                sname, CS_LAYER(va_msg->msgnumber),
                CS_ORIGIN(va_msg->msgnumber), CS_SEVERITY(va_msg->msgnumber),
                CS_NUMBER(va_msg->msgnumber), va_msg->msgstring);
                
	XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_err_handler logue le message : \n %s\n",vl_mbuf);
        /*
        ** Logue l'erreur.
        */
        (CS_VOID)srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_mbuf, CS_NULLTERM);

        /*
        ** Logue une erreur d'Operating Systeme.
        */
        if( va_msg->osstringlen > 0 )
        {
                (CS_VOID)sprintf(vl_mbuf,
                        "%s: CS-Library Operating system error %ld - %s.\n",
                        sname, va_msg->osnumber, va_msg->osstring);
                        
		XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_err_handler logue le message : \n %s\n",vl_mbuf);
                (CS_VOID)srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_mbuf,CS_NULLTERM);
        }

        /*A
        **  Fin du traitement de l'erreur.
        */
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_err_handler sort avec %d",CS_SUCCEED);
        return CS_SUCCEED;
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* 
*  Erreur handler pour le serveur : Quand une erreur est detectee par
*  l Open Serveur, on passe dans cette fct qui logue l'erreur, la decrit,
*  l'indique dans les traces (exp : mauvais passage de param a une RP).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asrv_server_err_handler(SRV_SERVER   *server,
				SRV_PROC        *sp,
				CS_INT          errornum,
				CS_INT          severity,
				CS_INT          state,
				CS_INT          oserrnum,
				CS_CHAR         *errtext,
				CS_INT          errtextlen,
				CS_CHAR         *oserrtext,
				CS_INT          oserrtextlen)

          

/*
* ARGUMENTS EN ENTREE :
*   < 
**      server          - The Open Server server running.
**      sp              - The Open Server thread that got the error.
**      errornum        - The Open Server error number.
**      severity        - The error severity.
**      state           - The error state.
**      oserrnum        - The operating system error number, if any.
**      errtext         - The text of the error message.
**      errtextlen      - The length of the errtext message
**      oserrtext       - The text of the operating system error message.
**      oserrtextlen    - The length of the errtext message.
*>
*
*
*
* CODE RETOUR : 
*   <
*	CS_CONTINUE     Open Server continue son execution.
*     	CS_FAIL         Fin de l'Open Server.
*
* CONDITION D'UTILISATION
*   < >
*
* FONCTION 
*   < 	L'Open serveur appelle cette routine quand une erreur 
*	interne est detectee, les arguements passes permettent 
*	de decrire le numero de l'erreur, sa severite, 
*	la chaine decrivant l'erreur .
*	On traite les differentes severites d'erreur comme suit :
*	
*	SRV_FATAL_SERVER 	Cette erreur signifie que OPSRV 
				a detecte une erreur definitive
				On essaie de la logue dans le fichier
				et on retourne.
	
	SRV_FATAL_PROCESS	Ce niveau de severite indique qu'un thread
				a detecte une erreur qui tue le thread.
				L'erreur est loguee et on retourne.
				
	SRV_INFO                C'est une erreur informative. Si le thread d'ou
				cette erreur provient est un thread client actif
				on lui envoie l'information via un message
	
>
------------------------------------------------------*/

{
	
        static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_server_err_handler";
        CS_CONTEXT      *vl_cp;                    /* Context structure. */
        CS_CHAR         sname[CS_MAX_NAME];     /* The server name. */
        CS_INT          slen;                   /* The server name length. */
        CS_CHAR         vl_mbuf[CS_MAX_MSG];       /* The message buffer. */
        CS_SERVERMSG    msg;                    /* The message structure. */
        CS_INT          type;                   /* The thread type. */
        CS_BOOL         client;                 /* Is it a client thread? */
        CS_BOOL         iodead;                 /* Is the thread alive? */

        /*A
        ** Initialisation.
        */
        srv_bzero(&msg, sizeof(msg));
        client = CS_FALSE;
        iodead = CS_FALSE;

        /*A
        ** On recupere le contexte utilise par ce serveur.
        */
        if( cs_ctx_global(CS_VERSION_100, &vl_cp) == CS_FAIL )
        {
                /*
                ** Can't raise an error, so just return.
                */
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_server_err_handler retourne %d (cs_ctx_global)",CS_CONTINUE);
                return CS_CONTINUE;
        }

        /*A
        ** On recupere le nom de l'Open Serveur, si on a donne 
        ** une structure SRV_SERVER dans l'appel de la fonction.
        */
        if( server != (SRV_SERVER *)NULL )
        {
                if( srv_props(vl_cp, CS_GET, SRV_S_SERVERNAME, sname,  CS_MAX_NAME, &slen) == CS_FAIL )
                {
                        /*B
                        ** On ne peut faire remonter l erreur: on retourne.
                        */
                        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_server_err_handler retourne %d (srv_props)",CS_CONTINUE);

                        return CS_CONTINUE;
                }
                sname[slen] = '\0';
        }
        else
        {
                (CS_VOID)strcpy(sname, "no server");
        }

        /*A
        ** Est ce une erreur d operating system? Si Oui, on l'a logue.
        */
        if (oserrnum != SRV_ENO_OS_ERR)
        {
                /*B
                ** On logue l'erreur.
                */
                (CS_VOID)sprintf(vl_mbuf, "%s: OPERATING SYSTEM ERROR: %d: %s.\n",  sname,  oserrnum, oserrtext);
		XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_server_err_handler logue le mesg :\n %s",vl_mbuf);
                (CS_VOID)srv_log(server, CS_TRUE, vl_mbuf, CS_NULLTERM);
        }

        /*A
        ** Est ce une erreur fatale pour l'open Serveur?
        */
       if (severity == SRV_FATAL_SERVER)
        {
                /*B
                ** Essai de loguer l'erreur, and retourne.
                */
                (CS_VOID)sprintf(vl_mbuf,"%s: FATAL SERVER ERROR: %d/%d/%d: %s.\n",
                        sname, errornum, severity, state, errtext);
                        
		XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_server_err_handler logue le mesg :\n %s",vl_mbuf);
		
                (CS_VOID)srv_log(server, CS_TRUE, vl_mbuf, CS_NULLTERM);
                
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_server_err_handler retourne %d",SRV_EXIT_PROGRAM);
                return SRV_EXIT_PROGRAM;
        }

        /*A
        ** Est ce un thread qui a eu une erreur fatale?
        */
        if (severity == SRV_FATAL_PROCESS)
        {
                /*B
                ** Essai de loguer l'erreur, and retourne.
                */
                (CS_VOID)sprintf(vl_mbuf,
                        "%s: FATAL CONNECT ERROR: %d/%d/%d: %s.\n",
                        sname, errornum, severity, state, errtext);
                        
                XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_server_err_handler logue le mesg :\n %s",vl_mbuf);
                (CS_VOID)srv_log(server, CS_TRUE, vl_mbuf, CS_NULLTERM);
                
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_server_err_handler retourne %d (severity == SRV_FATAL_PROCESS)",CS_CONTINUE);
                return CS_CONTINUE;
        }

        /*A
        ** L'erreur est non fatale ou un message informatif.
        **
        ** Si l'erreur est apparue sur un thread client, send an
        ** informational message containing the error information
        ** to that client.
        */
       if( sp != (SRV_PROC *)NULL )
        {
                /*B
                ** On recupere le type de thread.
                */
                if( srv_thread_props(sp, CS_GET, SRV_T_TYPE,
                        &type, sizeof(type), (CS_INT *)NULL) == CS_FAIL )
                {
                        /*B
                        ** On ne peut rien faire de plus :retourne CS_FAIL;.
                        */
                        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_server_err_handler retourne %d",CS_FAIL);
                        return CS_FAIL;
                }

                /*B
                ** On regarde si on a un thread client.
                */
                if( (type == SRV_TCLIENT) || (type == SRV_TSUBPROC) )
                {
                        client = CS_TRUE;
                }

                /*B
                ** On verifie que le canal d I/O du thread client
                ** est toujours actif, avant d essayer d'envoyer un message.
                */
                if( srv_thread_props(sp, CS_GET, SRV_T_IODEAD,
                        &iodead, sizeof(iodead), (CS_INT *)NULL) == CS_FAIL )
                {
                        /*B
                        ** On ne peut rien faire de plus :retourne CS_FAIL;.
                        */
                        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_server_err_handler retourne %d",CS_FAIL);
                        return CS_FAIL;
                }

                /*B
                ** Maintenant c'est un thread client actif:  
                ** on envoie le message.
                */
                if( client && !iodead )
                {
                	
                        /*B
                        ** Rempli le message d'erreur a renvoyer.
                        */
                        msg.msgnumber = errornum;
                        msg.state = state;
                        msg.severity = severity;
                        (CS_VOID)strncpy(msg.text, errtext, CS_MAX_MSG);
                        msg.textlen = strlen(msg.text);
                        (CS_VOID)strncpy(msg.svrname, sname, CS_MAX_NAME);
                        msg.svrnlen = strlen(msg.svrname);

                        /*B
                        ** On envoie le message au client.
                        ** On ne regarde pas si cela echoue, parce que
                        ** on logue l'erreur de toute facon.
                        */
                       msg.status = (CS_FIRST_CHUNK|CS_LAST_CHUNK);
                       
                       XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_server_err_handler logue le message :\n %s",msg.text);
                       
                       if (srv_sendinfo(sp, &msg, CS_TRAN_UNDEFINED)!= CS_SUCCEED)                       
                       {
                       		XZST_03EcritureTrace(XZSTC_WARNING,"asrv_server_err_handler : srv_sendinfo a echoue");	
                       }
                       	/*B 
                	** Envoi un status -1 a la requete indiquant une erreur . OM : specifique migrazur
                	*/
                	if (srv_sendstatus(sp,(CS_INT) -1) != CS_SUCCEED)
                        
                 	       	XZST_03EcritureTrace(XZSTC_WARNING," asrv_server_err_handler : srv_sendstatus(-1) a echoue.\n");
                	else 
                		XZST_03EcritureTrace(XZSTC_DEBUG1,"asrv_server_err_handler :  srv_sendstatus() a renvoye -1");      


                }
        }

        /*A
        ** On logue l'erreur.
        */
        sprintf(vl_mbuf,"%s: ERREUR: %d SEVERITE: %d Etat: %d Libele: %s.\n", sname, errornum, severity, state, errtext);
	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_server_err_handler logue le mesg : \n %s",vl_mbuf);
	
        if (srv_log(server, CS_TRUE, vl_mbuf, CS_NULLTERM)!= CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"asrv_server_err_handler : srv_log a echoue");
        }

        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_server_err_handler retourne %d (fin de fonction)",CS_CONTINUE);
        return CS_CONTINUE;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
* asrv_rp_shutdown : Arret de L Open SRV via le isql ou le SQL Srv .
*  On passe un passwd = yes pour pouvoir l arreter.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int asrv_rp_shutdown(SRV_PROC        *pl_srvproc)          
/*
* ARGUMENTS EN ENTREE :
*
*   Pointeur vers une structure de controle thread interne 
*
* ARGUMENTS EN SORTIE :

* CODE RETOUR : 
*   envoie au SQL_serveur appelant un srv_senddone pour completer la requete
*   l Open Serveur s'arrete proprement
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Arret propre du serveur avec verification du passwd... >
*
------------------------------------------------------*/

{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_rp_shutdown";
        CS_BOOL         vl_ok;
        CS_INT          status;
        CS_INT          len;
        CS_INT          numparams;
        CS_SMALLINT     outlen;
        CS_SERVERMSG    msg;
        CS_DATAFMT      vl_datafmt;
        CS_CHAR         buf[CS_MAX_CHAR];
        CS_CONTEXT      *vl_cp;

        /*A 
	** Initialisation.                                              
	*/
        vl_ok = CS_TRUE;
        status = SRV_DONE_FINAL;
        srv_bzero(&vl_datafmt, CS_SIZEOF(CS_DATAFMT));

        /*A 
	** Verification du passwd avant d'effectuer le shut down.                            
	*/
        vl_datafmt.maxlength = CS_MAX_CHAR;
        len = vl_datafmt.maxlength;
        vl_datafmt.datatype = CS_CHAR_TYPE;

        /*A 
	** Verification des parametres passes.                                        
	*/
        if (srv_numparams(pl_srvproc, &numparams) != CS_SUCCEED)
        {
               vl_ok = CS_FALSE;
               XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_rp_shutdown : srv_numparams a echoue");
	}
        /*A 
	** On verifie que l'on a 1 et seulement 1 parametre passe.      
	*/
        if (numparams != 1)
        {
                vl_ok = CS_FALSE;
                XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_rp_shutdown : Le nbre de parametres (%d) passes n est pas bon",numparams);
        }

        /*A 
	** Recuperation du passwd dans le parametre. 
	*/
        if (vl_ok && srv_bind(pl_srvproc, CS_GET, SRV_RPCDATA, 1, &vl_datafmt,
                (CS_BYTE *)buf, &len, &outlen) != CS_SUCCEED)
        {
                /*B 
		** Une erreur est deja apparue.                       
		*/
                vl_ok = CS_FALSE;
                XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_rp_shutdown : srv_bind a echoue ");
        }

        /*A 
	** Lecture des parametres dans des variables allouees.             
	*/
        if (vl_ok && (srv_xferdata(pl_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
        {
        	vl_ok = CS_FALSE;
        	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_rp_shutdown : srv_xferdata pour recuperer les parametres a echoue");
        }

        /*A
        ** Est ce un passwd valide pour arreter le server?
        **
        ** NOTE: La verification du passwd ne marche que si le client travail en
        **       ASCII/ISO_1 clients.
        */
        if (!vl_ok || memcmp(buf, DEFAULT_PASSWD, strlen(DEFAULT_PASSWD)) != 0)
        {
                vl_ok = CS_FALSE;
		XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_rp_shutdown : Probleme est survenu : logue un message");
                /*B
                ** Initialise un message a renvoyer au client lui
                ** indiquant que le passwd est invalid.
                */
                srv_bzero(&msg, CS_SIZEOF(CS_SERVERMSG));
                msg.msgnumber = RPC_FAIL;
                msg.severity = 10;

                /* 
		** Rempli le champ text du message.                      
		*/
                (CS_VOID)sprintf(msg.text, "asrv_rp_shutdown: Invalid password.\n");
                XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : %s",msg.text);
                msg.textlen = strlen(msg.text);

                /* 
		** Alloue la structure de contexte.                      
		*/
                if (cs_ctx_global(CS_VERSION_100, &vl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"cs_ctx_alloc() a echoue.\n");
                }
                else
                {
                        /* 
			** Rempli le champ du nom du serveur.               
			*/
                        (CS_VOID)srv_props(vl_cp, CS_GET, SRV_S_SERVERNAME,
                                (CS_VOID *)msg.svrname, CS_SIZEOF(msg.svrname),
                                &msg.svrnlen);
                }

                /*B 
		** Previent le client qu une erreur est survenue.           
		*/
		msg.status = (CS_FIRST_CHUNK|CS_LAST_CHUNK);
		
                if (srv_sendinfo(pl_srvproc, &msg, (CS_INT)0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : srv_sendinfo() a echoue.\n");
                        
                status |= SRV_DONE_ERROR;

                /*B 
		** Le status de la RPC est mis a -1 indicatant une erreur.  
		*/
                if (srv_sendstatus(pl_srvproc,(CS_INT) -1) != CS_SUCCEED)
                {
                         XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : srv_sendstatus() a echoue.\n");
                }
        }

        /*A 
	** Informe le client du shutdown.                           
	*/
        if (vl_ok)
        {
                /*A 
		** Le status de la RPC est mis a 1 indicatant le succes de la RPC.  
		*/
                if (srv_sendstatus(pl_srvproc,(CS_INT) 1) != CS_SUCCEED)
                {
                        status |= SRV_DONE_ERROR;
               		XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : \nError shutting down server.\n");
                }
                
                
                /*A 
        	**  Envoie du paquet final (done ) au client.                    
        	*/
        	if (srv_senddone(pl_srvproc, status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
        	{
        	        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : srv_senddone() a echoue.\n");
        	}
        	else
        	{
        		XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : Fin de l OPEN serveur");
       	 	}
       	 	
       	 	/*A
                **  Arret de l application 
                */
                
                if (srv_event(pl_srvproc, SRV_STOP, (CS_VOID *) NULL) != SRV_STOP)
                {
                        status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : \nUne erreur est survenue en arretant le serveur.\n");
                }
                else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : \n Arret du serveur");
                }
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV: asrv_rp_shutdown : retourne %d",CS_SUCCEED);
        	return (CS_SUCCEED);
        }
        
        /*A 
        **  Envoie du paquet final (done ) au client.                    
        */
        
        if (srv_senddone(pl_srvproc, status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : srv_senddone() a echoue.\n");
        }
        else
        {
        	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV: asrv_rp_shutdown : srv_senddone : Redonne la main au client ");
       	}
       	 
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV: asrv_rp_shutdown a echoue : retourne %d",CS_FAIL);
        return (CS_FAIL);
}/*Fin de asrv_rp_shutdown*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Parse la ligne de lancement de l Open SRV et recupere les arguments.
*  Le nom du Open SRV le nom du SQL serveur et les traces de la CT lib.
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asrv_proc_args(	CS_CONTEXT      *cp,
				CS_INT          argc,
				CS_CHAR         **argv,
				CS_CHAR         *sname,
				CS_INT          slen,
				CS_CHAR         *sqlname,
				CS_INT          sqllen)
	       
/*
** PROC_ARGS
**
**      This routine parses the command line arguments passed to the
**      Open Server application.
**
**      The syntax expected is as follows:
**
**      sample_name [server name] [-s SQL server name] [-trace flags]
* 
*
* ARGUMENTS EN SORTIE :
*   
*	sname   contiendra le nom de l Open Serveur
*	sqlname contiendra le nom d SQL Serveur au quel on desire se connecte 
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_proc_args";
        CS_INT          flags;                  /* The trace types requested.*/
        CS_CHAR         vl_mbuf[CS_MAX_MSG];       /* Message buffer. */

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : TASRV : asrv_proc_args est appelee");
        /*
        ** Initialization.
        */
        flags = (CS_INT)0;
        *sname = '\0';
        Ctcflags = (CS_INT)0;
        Conflags = (CS_INT)0;

        /*
        ** If we have arguments, the first one should be the
        ** Open Server name.
        */
        if (argc > 1 )
        {
                if( argv[1][0] == '-')
                {
                        /*
                        ** We don't have a server name
                        ** argument, so just jump to the first flag.
                        */
                        argv++;
                }
                else
                {
                        /*
                        ** The first argument is the server name.
                        */
                        (CS_VOID)strncpy(sname, argv[1], slen);

                        /*
                        ** Skip to the second argument, if any.
                        */
                        argv++;
                        argv++;
                }
        }
        else
        {
                /*
                ** We don't have any arguments, so we're all done.
                */
                XZST_03EcritureTrace(XZSTC_FONCTION,"IN : TASRV : asrv_proc_args sort : Pas d'arguments passes en ligne de commande");
                return CS_SUCCEED;
        }

        for (; *argv != (CS_CHAR *)NULL; argv++ )
        {
                /*
                **      Get the SQL Server name, if available.
                */
                if ( strcmp(*argv, "-s") == 0 )
                {
                        argv++;

                        if ( sqlname != (CS_CHAR *)NULL )
                        {
                                (CS_VOID)strncpy(sqlname, *argv, sqllen);
                        }

                        continue;
                }

                if( strcmp(*argv, "-") == 0 )
                {
                        argv++;
                }
                else
                {
                        (*argv)++;
                }

                /*
                ** TDS header tracing.
                */
                if( strcmp(*argv, "h") == 0 )
                {
                        flags |= SRV_TR_TDSHDR;
                }

                /*
                ** TDS data tracing.
                */
                else if( strcmp(*argv, "d") == 0 )
                {
                        flags |= SRV_TR_TDSDATA;
                }
                /*
                ** Attention tracing.
                */
                else if( strcmp(*argv, "a") == 0 )
                {
                        flags |= SRV_TR_ATTN;
                }
                /*
                ** Message queue tracing.
                */
                else if( strcmp(*argv, "m") == 0 )
                {
                        flags |= SRV_TR_MSGQ;
                }
                /*
                ** TDS tvl_oken tracing.
                */
                else if( strcmp(*argv, "t") == 0 )
                {
                        flags |= SRV_TR_TDSTOKEN;
                }
                /*
                ** Event tracing.
                */
                else if( strcmp(*argv, "e") == 0 )
                {
                        flags |= SRV_TR_EVENT;
                }
                /*
                ** Deferred queue tracing.
                */
                else if( strcmp(*argv, "q") == 0 )
                {
                        flags |= SRV_TR_DEFQUEUE;
                }
                /*
                ** Net-Library tracing.
                */
                else if( strcmp(*argv, "n") == 0 )
                {
                        flags |= SRV_TR_NETREQ;
                        flags |= SRV_TR_NETDRIVER;
                }

                /*
                ** CT-Lib: all debug flags.
                */
                else if(strcmp(*argv, "c") == 0 )
                {
                        Conflags |= CS_DBG_ALL;
                }
                /*
                ** CT-Lib API states.
                */
                else if(strcmp(*argv, "f") == 0 )
                {
                        Ctcflags |= CS_DBG_API_STATES;
                }
                /*
                ** CT-Lib asynchronous tracing.
                */
                else if(strcmp(*argv, "y") == 0 )
                {
                        Ctcflags |= CS_DBG_ASYNC;
                }
                /*
                ** CT-Lib diagnostic tracing.
                */
                else if(strcmp(*argv, "g") == 0 )
                {
                        Conflags |= CS_DBG_DIAG;
                }
                /*
                ** CT-Lib error tracing.
                */
                else if(strcmp(*argv, "r") == 0 )
                {
                        Ctcflags |= CS_DBG_ERROR;
                }
                /*
                ** CT-Lib memory tracing.
                */
                else if(strcmp(*argv, "u") == 0)
                {
                        Ctcflags |= CS_DBG_MEM;
                }
                /*
                ** CT-Lib protocol tracing.
                */
                else if(strcmp(*argv, "p") == 0)
                {
                        Conflags |= CS_DBG_PROTOCOL;
                }
                /*
                ** CT-Lib protocol state tracing.
                */

                else if(strcmp(*argv, "v") == 0)
                {
                        Conflags |= CS_DBG_PROTOCOL_STATES;
                }
                /*
                ** CT-Lib network tracing.
                */
                else if(strcmp(*argv, "w") == 0)
                {
                        Ctcflags |= CS_DBG_NETWORK;
                }
                else
                {
                        (CS_VOID)sprintf(vl_mbuf,"Unknown trace flag: '%s'.\n", *argv);
                        asrv_syntax_error(vl_mbuf);
                        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_proc_args sort avec %d", CS_FAIL);
                        return CS_FAIL;
                }
        }

        /*
        ** If there were any trace types set, activate them now.
        */
        if(flags != (CS_INT)(0))
        {
                if( srv_props(cp, CS_SET, SRV_S_TRACEFLAG, &flags,
                        CS_SIZEOF(CS_INT), (CS_INT *)NULL) == CS_FAIL )
                {
                        /*
                        ** An error was already raised.
                        */
                        XZST_03EcritureTrace(XZSTC_FATAL,"asrv_proc_args : srv_props a renvoye l'erreur %d",CS_FAIL);
                        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_proc_args sort avec %d", CS_FAIL);
                        return CS_FAIL;
                }
        }

        /*
        ** If CT-Lib context level tracing was set, we'll
        ** activate it once ct_init() has been called.
        */

        /*
        ** All done.
        */
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_proc_args tout est OK, sort avec %d", CS_SUCCEED);
        return CS_SUCCEED;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette routine alloue la structure contexte
*   et install les CS-Library error handler.
*   La version est aussi positionne pour l Open Serveur et la CT-Lib.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asrv_setversion(CS_CONTEXT      **pa_cpp)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   retourne le contexte dans pa_cpp
*
*
* CODE RETOUR : 
*   CS_SUCCEED      	l information sur la version a ete positionnee
*   CS_FAIL		Une erreur a ete detectee
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Cette routine alloue la structure contexte
*   et install les CS-Library error handler.
*   La version est aussi positionne pour l Open Serveur et la CT-Lib.
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_setversion";
        CS_CONTEXT      *vl_cp;            /* Open Server context.         */

        
        XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : TASRV : asrv_setversion est appelee");
     
        /*A
        ** Allocation de la structure contexte et init de l Open Server
        */

        if(cs_ctx_alloc(CS_VERSION_100, &vl_cp) == CS_FAIL)
        {
                XZST_03EcritureTrace(XZSTC_FATAL," TASRV : cs_ctx_alloc() a echoue.  Exiting.\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_setversion a echoue : %d",CS_FAIL);
                return(CS_FAIL);
        }
	/*A
        **  Installation de la gession des erreurs pour la CS-Library .
        */
        if(cs_config(vl_cp, CS_SET, CS_MESSAGE_CB, (CS_VOID *)asrv_err_handler,
                CS_UNUSED, (CS_INT *)NULL) == CS_FAIL)
        {
                (CS_VOID)cs_ctx_drop(vl_cp);
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : cs_config(CS_MESSAGE_CB) a echoue.  Exiting.\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_setversion a echoue :  %d",CS_FAIL);
                
                return(CS_FAIL);
        }
	XZST_03EcritureTrace(XZSTC_SGBD,"Installation de la CS-Library error handler reussie.");
        
        /*A
        **Definition du comportement de Open Client et du contexte utilise.
        */
        if(srv_version(vl_cp, CS_VERSION_100) == CS_FAIL)
        {
                /*
                ** Desalloue la structure.
                */

                (CS_VOID)cs_ctx_drop(vl_cp);
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_version() a echoue.  Exiting.");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_setversion : Sortie avec %d.",CS_FAIL);
 
                return(CS_FAIL);
        }
	XZST_03EcritureTrace(XZSTC_SGBD,"Definition du comportement de Open Client et du contexte utilise.");
        /*A
        ** Retourn la structure context alouee et initialise.
        */
        *pa_cpp = vl_cp;
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_setversion : Sortie avec %d.",CS_SUCCEED);
        return(CS_SUCCEED);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_syntax_error(CS_CHAR   *pa_msg)

/*
* ARGUMENTS EN ENTREE :
*
*   Recoit la ligne de commande donnee par l utilisateur pour lance l Open Serveur
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Cette routine informa l utilisateur que la ligne de commande pour
*   lance l Open Serveur est incorrect.
*
------------------------------------------------------*/
{	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_syntax_error";
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : TASRV : asrv_syntax_error () est appele avec %s",pa_msg);
        /*A
        ** Affichage du message d erreur.
        */
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : Erreur de synthaxe : %s\n", pa_msg);
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : La syntaxe est:\n\t");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : tasrv.x [Nom de l Open serveur] [-s Nom du SQL ] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-a (Attentions)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-c (CT-Lib all tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-d (TDS data)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-e (Event)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-f (CT-Lib Function States)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-g (CT-Lib Diagnostics Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-h (TDS header)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-i (I/O)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-m (Msg Queue)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-n (Net-Library )] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-p (CT-Lib Protocol Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-q (Deferred Queue)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-r (CT-Lib Error Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-t (TDS tvl_oken)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-u (CT-Lib Memory Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-v (CT-Lib Protocol States Tracing)] \n");
	XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-w (CT-Lib Network Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : asrv_syntax_error : \t\t[-y (CT-Lib Async Tracing)] \n");
        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : asrv_syntax_error : Fin de laffichage d erreur ");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TASRV : asrv_syntax_error () retourne %d",XDC_OK);
        return (XDC_OK);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Erreur handler de la CT_LIB. Toutes connexions des Open Client utilisent cette handler d erreur.
*  L erreur recue est envoyee a l open server client qui est affecte. (Provient de Ctosdemo : asrv_cterr).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_cterr(	CS_CONTEXT      *pa_cp,
			CS_CONNECTION   *pa_chp,
			CS_CLIENTMSG    *pa_emsgp)

/*
* ARGUMENTS EN ENTREE :
*
*   	pa_cp 		Structure de contexte
*	pa_chp		handle de connexion
*	pa_emsgp	pointeur sur la structure CS_CLIENTMSG de l Open Client
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
        static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_cterr";
        SRV_PROC        *pl_spp;           /* pl_spp of client connection.    	*/
        CS_CHAR         vl_msg[1024];      /* Buffer de message.          		*/
        CS_INT          vl_len;            /* ct_con_props retourne la taille.    	*/
        CS_SERVERMSG    vl_smsg;           /* Structure du message du serveur.    	*/

        /* Initialization.                                              */
        pl_spp = (SRV_PROC *)NULL;          /* No SRV_PROC yet.             */
        vl_len = 0;                        /* Clear ct_con_props argument. */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"In : asrv_cterr erreur Handler pour la CT Lib est appele");
        
        /*
        ** If pa_chp is (CS_CONNECTION *)NULL, this error occurred when no
        ** connection handle was active.  Just return cancel.  The error will
        ** be processed by the main-line code.
        */
        if(pa_chp == (CS_CONNECTION *)NULL)
        {
                sprintf(vl_msg,"asrv_cterr: NULLSRVPROC err(%d: %s) oserr(%d: %s)\n",
                        pa_emsgp->msgnumber, pa_emsgp->msgstring, pa_emsgp->osnumber,
                        pa_emsgp->osstring);
		srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_msg, CS_NULLTERM);
                XZST_03EcritureTrace(XZSTC_FONCTION,"Out : asrv_cterr erreur Handler : Sortie avec %d",CS_SUCCEED);
                return(CS_SUCCEED);
        }

        /* Get the current pl_spp from the connection handle.              */
        if(ct_con_props(pa_chp, CS_GET, CS_USERDATA, &pl_spp, CS_SIZEOF(pl_spp), &vl_len)
                != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FONCTION,"Out : asrv_cterr erreur Handler : Sortie avec %d",CS_SUCCEED);
		return(CS_SUCCEED);
        }

        /* Make sure we have a valid pl_spp.                               */
        if(pl_spp == (SRV_PROC *)NULL)
        {
                sprintf(vl_msg,"asrv_cterr: NULLSRVPROC err(%d: %s) oserr(%d: %s)\n",
                        pa_emsgp->msgnumber, pa_emsgp->msgstring, pa_emsgp->osnumber,
                        pa_emsgp->osstring);
                srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_msg, CS_NULLTERM);
                return(CS_SUCCEED);
        }

        /*
        ** Log the error numbers and severity.
        */
        sprintf(vl_msg, c_ERRMSG ,pa_emsgp->msgnumber, pa_emsgp->severity,
                pa_emsgp->osnumber);
        srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_msg, CS_NULLTERM);

        sprintf(vl_msg, "\t\t%s\n", pa_emsgp->msgstring);
        srv_log((SRV_SERVER *)NULL, CS_FALSE, vl_msg, CS_NULLTERM);

        /*
        ** If there was an Operating System error, log this information.
        */
        if((pa_emsgp->osstringlen != 0) && (pa_emsgp->osstringlen != CS_UNUSED))
        {
                sprintf(vl_msg, c_OSERRMSG, pa_emsgp->osnumber);
                srv_log((SRV_SERVER *)NULL, CS_TRUE, vl_msg, CS_NULLTERM);
                sprintf(vl_msg, "%s\n", pa_emsgp->osstring);
                srv_log((SRV_SERVER *)NULL, CS_FALSE, vl_msg, CS_NULLTERM);
        }

        /*
        ** Map the client message structure into the server message
        ** structure so we can send this information on to the client.
        */
        vl_smsg.msgnumber = pa_emsgp->msgnumber;
        vl_smsg.state = (CS_INT)0;
        vl_smsg.severity = pa_emsgp->severity;
        srv_bmove(pa_emsgp->msgstring, vl_smsg.text, CS_SIZEOF(vl_smsg.text));
        vl_smsg.textlen = pa_emsgp->msgstringlen;
        (CS_VOID)srv_props(Context, CS_GET, SRV_S_SERVERNAME,
                (CS_VOID *)vl_smsg.svrname, CS_SIZEOF(vl_smsg.svrname),
                &(vl_smsg.svrnlen));
        vl_smsg.proclen = (CS_INT)0;
        vl_smsg.line = (CS_INT)0;
        vl_smsg.status = pa_emsgp->status;
        vl_smsg.status |= (CS_FIRST_CHUNK|CS_LAST_CHUNK);

        srv_bmove(pa_emsgp->sqlstate, vl_smsg.sqlstate, CS_SIZEOF(vl_smsg.sqlstate));
        vl_smsg.sqlstatelen = pa_emsgp->sqlstatelen;

        /*
        ** Send error string to client.
        */
        (CS_VOID)srv_sendinfo(pl_spp, &vl_smsg, CS_TRAN_UNDEFINED);

        return(CS_SUCCEED);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_regparam pour un type char (tableau)
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_regparam_char(SRV_PROC        *pa_srvproc,char * pa_NomParam)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure a enregistrer
*	pa_NomParam	Chaine contenant le nom du parametre a enregistrer pour la RP
*
* ARGUMENTS EN SORTIE :
*   
*	pa_srvproc	connait le nouveau parametre de type char
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de nom enregistrement du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*   
*	pa_srvproc doit avoir ete aloue par srv_createproc avant l appel.
*
* FONCTION 
*  
*
------------------------------------------------------*/
{	
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_regparam_char";
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : asrv_regparam_char : %s",pa_NomParam); 
	
	vl_retcode = srv_regparam(pa_srvproc,pa_NomParam, CS_NULLTERM, CS_CHAR_TYPE,SRV_NODEFAULT, (CS_BYTE *)NULL);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_regparam_char retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_regparam_in : Encapsule srv_regparam pour un type CS_INT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_regparam_int(SRV_PROC        *pa_srvproc,char * pa_NomParam)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure a enregistrer
*	pa_NomParam	Chaine contenant le nom du parametre a enregistrer pour la RP
*
* ARGUMENTS EN SORTIE :
*   
*	pa_srvproc	connait le nouveau parametre de type char
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non enregistrement du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_srvproc doit avoir ete aloue par srv_createproc avant l appel.
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_regparam_int";
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : asrv_regparam_int : %s",pa_NomParam); 
	
	vl_retcode = srv_regparam(pa_srvproc,pa_NomParam, CS_NULLTERM, CS_INT_TYPE,SRV_NODEFAULT, (CS_BYTE *)NULL);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_regparam_int retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_regparam pour un type CS_TINYINT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_regparam_tinyint(SRV_PROC        *pa_srvproc,char * pa_NomParam)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure a enregistrer
*	pa_NomParam	Chaine contenant le nom du parametre a enregistrer pour la RP
*
* ARGUMENTS EN SORTIE :
*   
*	pa_srvproc	connait le nouveau parametre de type CS_TINYINT
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non enregistrement du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_srvproc doit avoir ete aloue par srv_createproc avant l appel.
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_regparam_tinyint";
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_regparam_tinyint : %s",pa_NomParam); 
	
	vl_retcode = srv_regparam(pa_srvproc,pa_NomParam, CS_NULLTERM, CS_TINYINT_TYPE,SRV_NODEFAULT, (CS_BYTE *)NULL);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_regparam_tinyint retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_regparam pour un type CS_SMALLINT
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_regparam_smallint(SRV_PROC *pa_srvproc,char *pa_NomParam)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure a enregistrer
*	pa_NomParam	Chaine contenant le nom du parametre a enregistrer pour la RP
*
* ARGUMENTS EN SORTIE :
*   
*	pa_srvproc	connait le nouveau parametre de type CS_SMALLINT
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non enregistrement du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_srvproc doit avoir ete aloue par srv_createproc avant l appel.
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_regparam_smallint";
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_regparam_smallint : %s",pa_NomParam); 
	
	vl_retcode=srv_regparam(pa_srvproc,pa_NomParam, CS_NULLTERM, CS_SMALLINT_TYPE,SRV_NODEFAULT,(CS_BYTE *)NULL);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_regparam_smallint retourne %d",vl_retcode );
	return (vl_retcode);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_regparam_date : Encapsule srv_regparam pour un type CS_DATETIME
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_regparam_date(SRV_PROC *pa_srvproc,char *pa_NomParam)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure a enregistrer
*	pa_NomParam	Chaine contenant le nom du parametre a enregistrer pour la RP
*
* ARGUMENTS EN SORTIE :
*   
*	pa_srvproc	connait le nouveau parametre de type CS_DATETIME
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non enregistrement du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_srvproc doit avoir ete aloue par srv_createproc avant l appel.
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_regparam_date";
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_regparam_date : %s",pa_NomParam); 
	
	vl_retcode=srv_regparam(pa_srvproc,pa_NomParam, CS_NULLTERM, CS_DATETIME_TYPE,SRV_NODEFAULT,(CS_BYTE *)NULL);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_regparam_date retourne %d",vl_retcode );
	return (vl_retcode);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_lire_smallint : Encapsule srv_bind pour un type CS_SMALLINT
*	Prepare la lecture d un parametre recu dans une Stored Procedure(XZAO....)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_lire_smallint(SRV_PROC *pa_srvproc,int va_param_no,CS_SMALLINT *pa_Param)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure executee
*	va_param_no	contient la place du parametre dans l appel
*	pa_Param	Pointeur dans lequel on desire renvoyer la valeur lue
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Param	Contient la valeur passee a la RP.
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non recuperation du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_Param doit etre du meme type que le parametre passe a la RP et en bonne position (a verifier)
*	le parametre passe doit etre une adresse.
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_smallint";
	int 		vl_retcode	= CS_SUCCEED;
	CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   */
                                        /* CS_DATAFMT permettant de     */
                                        /* decrire le format des datas  */
                                        /* et les variables du program  */
                                        /* Pour plus de details, voir   */
                                        /* Pour plus de details, voir   */
                                        /* page 2-25 de Open Serv Ref Ma*/
                                        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_smallint : param no %d",va_param_no); 
	srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));
	vl_datafmt.datatype =  CS_SMALLINT_TYPE;
	vl_datafmt.maxlength = (CS_INT)sizeof(CS_SMALLINT);
	
	if (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, va_param_no, &vl_datafmt,(CS_BYTE *)pa_Param,NULL,NULL)!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"asrv_lire_smallint : srv_bind a echoue");
		vl_retcode = CS_FAIL;
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_smallint retourne %d",vl_retcode );	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_smallint retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_lire_tinyint : Encapsule srv_bind pour un type CS_TINYINT
*	Prepare la lecture d un parametre recu dans une Stored Procedure(XZAO....)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_lire_tinyint(SRV_PROC *pa_srvproc,int va_param_no,CS_TINYINT *pa_Param)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure executee
*	va_param_no	contient la place du parametre dans l appel
*	pa_Param	Pointeur dans lequel on desire renvoyer la valeur lue
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Param	Contient la valeur passee a la RP.
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non recuperation du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_Param doit etre du meme type que le parametre passe a la RP et en bonne position (a verifier)
*	le parametre passe doit etre une adresse.
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_tinyint";
	int 		vl_retcode	= CS_SUCCEED;
	CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   */
                                        /* CS_DATAFMT permettant de     */
                                        /* decrire le format des datas  */
                                        /* et les variables du program  */
                                        /* Pour plus de details, voir   */
                                        /* Pour plus de details, voir   */
                                        /* page 2-25 de Open Serv Ref Ma*/
                                        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_tinyint : param no %d",va_param_no); 
	srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));
	vl_datafmt.datatype =  CS_TINYINT_TYPE;
	vl_datafmt.maxlength = (CS_INT)sizeof(CS_TINYINT);
	
	if (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, va_param_no, &vl_datafmt,(CS_BYTE *)pa_Param,NULL,NULL)!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"asrv_lire_tinyint : srv_bind a echoue");
		vl_retcode = CS_FAIL;
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_tinyint retourne %d",vl_retcode );	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_tinyint retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_bind pour un type CS_INT	
*	Prepare la lecture d un parametre recu dans une Stored Procedure(XZAO....)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_lire_int(SRV_PROC *pa_srvproc,int va_param_no,CS_INT *pa_Param)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure executee
*	va_param_no	contient la place du parametre dans l appel
*	pa_Param	Pointeur dans lequel on desire renvoyer la valeur lue
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Param	Contient la valeur passee a la RP.
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non recuperation du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_Param doit etre du meme type que le parametre passe a la RP et en bonne position (a verifier)
*	le parametre passe doit etre une adresse.
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_int";
	int 		vl_retcode	= CS_SUCCEED;
	CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   */
                                        /* CS_DATAFMT permettant de     */
                                        /* decrire le format des datas  */
                                        /* et les variables du program  */
                                        /* Pour plus de details, voir   */
                                        /* Pour plus de details, voir   */
                                        /* page 2-25 de Open Serv Ref Ma*/
                                        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_int : param no %d",va_param_no); 
	
	srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));
	
	vl_datafmt.datatype =  CS_INT_TYPE;
	vl_datafmt.maxlength = (CS_INT)sizeof(CS_INT);
	
	if (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, va_param_no, &vl_datafmt,(CS_BYTE *)pa_Param,NULL,NULL)!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"asrv_lire_int : srv_bind a echoue");
		vl_retcode = CS_FAIL;
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_int retourne %d",vl_retcode );	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_int retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_bind pour un type CS_CHAR
*	Rajoute un parametre dans la definition de l appel a la Register Proc
*	Prepare la lecture d un parametre recu dans une Stored Procedure(XZAO....)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_lire_char(SRV_PROC *pa_srvproc,int va_param_no,CS_CHAR *va_chaine, int va_lon_chaine)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure executee
*	va_param_no	contient la place du parametre dans l appel
*	va_chaine	Pointeur dans lequel on desire renvoyer la valeur lue
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Param	Contient la valeur passee a la RP.
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non recuperation du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_Param doit etre du meme type que le parametre passe a la RP et en bonne position (a verifier)
*	le parametre passe doit etre une adresse.
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_char";
	int 		vl_retcode	= CS_SUCCEED;
	
	CS_SMALLINT	vl_out_len;
	CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   */
                                        /* CS_DATAFMT permettant de     */
                                        /* decrire le format des datas  */
                                        /* et les variables du program  */
                                        /* Pour plus de details, voir   */
                                        /* Pour plus de details, voir   */
                                        /* page 2-25 de Open Serv Ref Ma*/
                                        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_char : param no %d de longueur %d",va_param_no,va_lon_chaine); 
	
	/* 
        ** initialisation des structures a NULL                        
        */
        srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));
        
	vl_datafmt.datatype 	=  CS_CHAR_TYPE;
	vl_datafmt.maxlength 	=  va_lon_chaine;
	vl_datafmt.format 	=  CS_FMT_NULLTERM;
	
	if (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, va_param_no, &vl_datafmt,(CS_BYTE *)va_chaine,(CS_INT *)&va_lon_chaine,&vl_out_len)!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"asrv_lire_char : srv_bind a echoue");
		vl_retcode = CS_FAIL;
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_char retourne %d",vl_retcode );	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_char retourne %d",vl_retcode );
	return (vl_retcode);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	Encapsule srv_bind pour un type CS_DATECHAR
*	Prepare la lecture d un parametre recu dans une Stored Procedure(XZAO....)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_INT asrv_lire_date(SRV_PROC *pa_srvproc,int va_param_no,CS_DATETIME *pa_param)

/*
* ARGUMENTS EN ENTREE :
*
*  	pa_srvproc	Pointeur sur la procedure executee
*	va_param_no	contient la place du parametre dans l appel
*	va_param	Pointeur dans lequel on desire renvoyer la valeur lue
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Param	Contient la valeur passee a la RP.
*
* CODE RETOUR : 
*   	
*	 CS_FAIL 	en cas de non recuperation du parametre
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
*	pa_Param doit etre du meme type que le parametre passe a la RP et en bonne position (a verifier)
*	le parametre passe doit etre une adresse.
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_date";
	int 		vl_retcode	= CS_SUCCEED;
	
	
	CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   */
                                        /* CS_DATAFMT permettant de     */
                                        /* decrire le format des datas  */
                                        /* et les variables du program  */
                                        /* Pour plus de details, voir   */
                                        /* Pour plus de details, voir   */
                                        /* page 2-25 de Open Serv Ref Ma*/
                                        
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_date : param no %d ",va_param_no); 
	
	/* 
        ** initialisation des structures a NULL                        
        */
        srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));
        
	vl_datafmt.datatype 	=  CS_DATETIME_TYPE;
	vl_datafmt.maxlength 	=  sizeof(CS_DATETIME_TYPE);
	/*vl_datafmt.format 	=  CS_FMT_NULLTERM;*/
	
	if (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, va_param_no, &vl_datafmt,(CS_BYTE *)pa_param,NULL,NULL)!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"asrv_lire_date : srv_bind a echoue");
		vl_retcode = CS_FAIL;
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_date retourne %d",vl_retcode );	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_date retourne %d",vl_retcode );
	return (vl_retcode);
}
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_lire_RTSRV_MSG : Lecture des messages du RTServeur afin de declancher les CallBack associees.
*	Cette fonction est appelee a chaque fin d'execution d'une RP.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asrv_lire_RTSRV_MSG()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   
* CODE RETOUR : 
*   		
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_lire_RTSRV_MSG";

	T_IPC_MSG              	pl_Msg ;
        T_INT4                  vl_Fd_RTServer  = 0;
	fd_set                  vl_Lecture;
	int                     vl_typ_evt      = 0;
	T_INT4                  vl_SockRT       = 0;
        struct timeval          vl_timeout;
	double                  vl_Le_TimeOut=0.001;
        double                  vl_Delta;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_lire_RTSRV_MSG "); 

	/* A
	** On vide les messages recus par l Open Serveur a la fin de
	** l execution de la callback OpenServeur
	*/
#ifndef _TIBCO_EMS
	TipcSrvGetSocket(&vl_Fd_RTServer);
        FD_ZERO(&vl_Lecture);
        FD_SET(vl_Fd_RTServer,&vl_Lecture);

    /*A Attendre des messages en mode non bloquant */
	 vl_Delta = floor (vl_Le_TimeOut);
         vl_timeout.tv_sec  = (unsigned long)vl_Delta;
	 vl_timeout.tv_usec = (long)(( vl_Le_TimeOut - vl_Delta)*1000000);


        /*vl_typ_evt = select(vl_Fd_RTServer+1,&vl_Lecture,
                                                        NULL,NULL,NULL);*/
	vl_typ_evt = select(vl_Fd_RTServer+1,&vl_Lecture,NULL,NULL,&vl_timeout );

	XZST_03EcritureTrace(XZSTC_DEBUG1,"errno=%d,vl_typ_evt=%d",errno,vl_typ_evt);

	if ( (vl_typ_evt != 0) && (vl_typ_evt != -1 ))
	{
        	if (FD_ISSET(vl_Fd_RTServer,&vl_Lecture))
        	{
                           /*B Met les interruptions
                            *       SIGALRM en attente      */
#endif
                     sighold(SIGALRM);

/*			while ((pl_Msg = TipcSrvMsgNext(c_ATTENTE_RTMSG))!=NULL)	
*/
//                        pl_Msg  = TipcSrvMsgNext(c_ATTENTE_RTMSG);
		pl_Msg = TipcSrvMsgNext (T_TIMEOUT_FOREVER);
			while (isMessageNotNULL(pl_Msg))	
			{
				XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgNext execute");
				TipcSrvMsgProcess(pl_Msg);
				TipcMsgDestroy(pl_Msg);

                                pl_Msg  = TipcSrvMsgNext(c_ATTENTE_RTMSG);
			}
			XZST_03EcritureTrace(XZSTC_DEBUG1,"sortie TipcSrvMsgNext");
			sigrelse(SIGALRM);

#ifndef _TIBCO_EMS
		}
	}
#endif
/*TipcSrvMainLoop(c_ATTENTE_RTMSG);*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_lire_RTSRV_MSG retourne %d", XDC_OK);
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  
* 	asrv_Cree_ERRMSG : Initialise une structure de type message
*	a renvoyer au SQL Serveur faisant appel a une RP qui a renvoye
*	une erreur.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asrv_Envoie_MSG_ERR(SRV_PROC *pa_srvproc,char *va_fonction,int va_NumErr)

/*
* ARGUMENTS EN ENTREE :
*                     	 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   		
*	 CS_SUCCEED
*
* CONDITION D'UTILISATION
*  
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rpcom.c,v 1.36 2021/06/08 09:50:23 pc2dpdy Exp $ : asrv_Envoie_MSG_ERR";
	CS_SERVERMSG    vl_msg;         /* structure permettant de ren- 	*/
                                        /* voyer un message d erreur au 	*/
                                        /* client.                      	*/
                                        
        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos	*/
                                        /* de localisation du serveur   	*/ 
                                                                       
        XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : asrv_Envoie_MSG_ERR ");   
        
        /*A
        ** Mise a 0 de la structure message
        */                            
        srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));                                
	
	/*A 
        ** indicateur du type de message et de sa gravite       
        */
        	
        vl_msg.msgnumber = RPC_FAIL;
        vl_msg.severity = 10;
	
	(CS_VOID)sprintf(vl_msg.text,"%s : retourne %d.\n",va_fonction,va_NumErr);
        vl_msg.textlen = strlen(vl_msg.text);
	
	/* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING," asrv_Envoie_MSG_ERR : cs_ctx_alloc() a echoue.\n");
        }
        else
        {
              	/* 
              	** Recuperation du nom du serveur               
              	*/
                (CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,(CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
        }
	/*B 
        **  Envoi du message d erreur au client                  
        */
        if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
	
	XZST_03EcritureTrace(XZSTC_WARNING," asrv_Envoie_MSG_ERR : srv_sendinfo() a echoue");
	
	if (cs_ctx_drop(pl_cp) != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING," asrv_Envoie_MSG_ERR : cs_ctx_alloc() a echoue.\n");
        }
        
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : asrv_Envoie_MSG_ERR retourne %d", XDC_OK);
	return (XDC_OK);
}










