		/*E*/
/*Fichier : $Id: asrv_arp.c,v 1.11 2021/04/23 08:17:47 gesconf Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/04/23 08:17:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ARP * FICHIER asrv_arp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	ARP appartient a l open serveur TASRV.x du sous systeme
*  BaseDD.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	28 Oct 1994	: Creation
* gaborit	29 dec 1994	: modif longueur type alerte (1.2)
* torregrossa	09 Jan 1998	: modif aarp01 (1532) 1.3
* jmg		24 mars 2011	: migration 64 bits 1.4
* jmg		01/04/11 : rau virtuel 1.5
* PNI		18/10/17 : pas d'appel de XZIR dans arp01 si alerte XZAAC_APPEL_IPHONE DEM 1.6
* ABE		27/04/20	: ajout arpDatex2 DEM-SAE130 V1.7
* ABE		24/09/20	: revert version précedente (V1.7) DEM-SAE130 V1.8
* ABE		26/11/20	: remise de la version 1.7 pour publication datex2 sur création de fmc au CIT DEM-SAE130 v1.9
* PNI		21/04/21	: init de vl_retcode dans arp01		 SAE-270 1.10
* GGY		15/01/24	: Ajout appel aarp02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_arp.h"
#include "xzsm.h"
#include "xzaac.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_arp.c,v 1.11 2021/04/23 08:17:47 gesconf Exp $ : asrv_arp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  RP affaisant appel a XZIR01 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE aarp01(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*	Passe les parametres È XZIR01.
*	On passe soit NomMachine soit NomSite, l un des deux doit etre NULL.
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_arp.c,v 1.11 2021/04/23 08:17:47 gesconf Exp $ : asrv_arp01" ;
	int		vl_retcode = XDC_OK;	/* Code de retour		*/
	
	
        CS_BOOL         vl_ok;
        
        CS_INT          vl_numparams;
              				/* Permet de recuperer le nbre  */
                                        /* de param passes a la rpc     */
       

        CS_SERVERMSG    vl_msg;        	/* structure permettant de ren- */
                                        /* voyer un message d erreur au */
                                        /* client.                      */

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos*/
                                        /* de localisation du serveur   */

        CS_INT          vl_status;      /* 
        				** Contient de le vl_status du srv 
        				** apres reception de la demande
        				*/

        CS_INT		vl_Eqt;		/* variable pour recuperer le   
        				** param venant du sql serveur
        				*/
	
	CS_TINYINT	vl_nump;
	
	CS_INT		vl_param_no=1;

	
	
	
        /* 
        ** Variables specifiques a l appel des services externes	
        */
      	XDY_Evt		vl_Evt_In;		/*
      						** Numero de la fiche main courante 
        					*/ 
	XDY_Entier        vl_numAlerte;
	
	CS_DATETIME	  vl_DateTime;		/* 
						** Param recu de la base a transcrire 
						** en XDY_Horodate dans vl_date.
						*/
	XDY_Horodate      vl_date;
        XDY_TypeAlerte    vl_typeAlerte;
        XDY_NomType       vl_nomType;
        XDY_TypeEqt       vl_typeEqt;
        XDY_Eqt           vl_numEqt;
        XDY_NomEqt        vl_nom_Eqt;
        XDY_TexteAlerte   vl_texte;
        XDY_Autoroute     vl_autoroute;
        XDY_PR            vl_pr;
        XDY_Sens          vl_sens;
        XDY_Octet       vl_creation;
        XDY_Octet       vl_siteAlerte;
	int	vl_jourSemaine;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);
	
	/*A 
	**  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /* 
        ** initialisation de vl_ok a TRUE                                  
        */
        vl_ok = CS_TRUE;       
        /*A 
        ** verification du contenu du parametre venant du client	
        */

        /*A 
        ** recupere le nombre de parametres et on teste                
        */
        if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
                vl_ok = CS_FALSE;

        /*A 
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc         
        */       
        if (vl_numparams != ca_aarp01_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_aarp01_nb_param);
        	vl_ok = CS_FALSE;
        }      
        /* 
       	** 1 er champ de  aarp01: vl_numAlerte de type XDY_Entier = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_numAlerte))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 2 eme champ : vl_DateTime : type  = CS_DATETIME
       	*/
	if (vl_ok && (asrv_lire_date(pa_srvproc, vl_param_no++,&vl_DateTime)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
     	/* 
       	** 3 eme champ : vl_typeAlerte de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_typeAlerte))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
     	
     	/* 
       	** 4 eme champ : vl_nomType type CS_CHAR de long XDC_Lon_NomSysteme
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_nomType,XDC_Lon_Nom)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
	/* 
       	** 5 eme champ : vl_typeEqt de type  XDY_Octet : tinyint
       	*/
       	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_typeEqt))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 6 eme champ : vl_numEqt de type  XDY_Mot : smallint
       	*/
       	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_numEqt))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 7 eme champ :vl_Nom_Eqt type CS_CHAR de long XDC_Lon_NomEqt
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_nom_Eqt,XDC_Lon_NomEqt)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
       	/* 
       	** 8 eme champ :vl_texte type CS_CHAR de long XDC_Lon_TexteAlerte
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_texte,XDC_Lon_TexteAlerte)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	/* 
       	** 9 eme champ : vl_autoroute de type  XDY_Octet : tinyint
       	*/
       	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_autoroute))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 10 eme champ : vl_pr de type  XDY_PR (long) : CS_INT
       	*/
       	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_pr))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 11 eme champ : vl_sens de type  XDY_Sens (unsigned char) : CS_TINYINT
       	*/
       	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_sens))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 12 eme champ : vl_creation de type  XDY_Booleen (unsigned char) : CS_TINYINT
       	*/
       	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_creation))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
       	** 13 eme champ : vl_siteAlerte de type  XDY_Octet : CS_TINYINT
       	*/
       	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_siteAlerte))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
    	/*A 
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

     	/*A
        **  Transfert de la date du format Sybase au format migrazur
        */

        if (vl_creation<2) //Retour historique des alertes
        {        
                XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                                        &vl_jourSemaine, &vl_date );
        }
        else if (vl_creation==2) //Retour des alertes d'ettacher de la FMC dans MAgistra, on prendr l'horodate de l'alerte et non l'horo courrante
        {
                vl_creation =XDC_VRAI;
                vl_retcode=asql_date_Sybase2Ux (vl_DateTime,&vl_date);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"aarp01 : asql_date_Sybase2Ux() a echoue");	
                        return(vl_retcode);
                }
        }	
	if (!vl_ok )
        {
        	/*A
        	** Initialise le message d erreur a renvoyer si un probleme est
        	** apparu.
        	*/
        	
       		/* 
       		** mise a zero du vl_buffer de message 
       		*/
    		srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));
        	/* 
        	** indicateur du type de message et de sa gravite       
        	*/       	
        	vl_msg.msgnumber = RPC_FAIL;
        	vl_msg.severity = 10;        	
        	/*A 
        	** Initialisation du message pour le client             
        	*/        	
        	(CS_VOID)sprintf(vl_msg.text,"aarp01 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : cs_ctx_alloc() a echoue.\n");
        	}
        	else
        	{
              		/* 
              		** Recuperation du nom du serveur               
              		*/
                	(CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                	(CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
        	}
        	/*B 
        	**  Envoi du message d erreur au client                  
        	*/
        	if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
        	
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendinfo() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendstatus() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : vl_ok");

/*                XZST_03EcritureTrace(XZSTC_DEBUG1,"Valeurs recues :\n\n numAlerte 	= %d\n date 		= %ld\n typeAlerte 	= %d\n nomType 	= %d\n typeEqt		= %d\n numEqt		= %d\n nom_Eqt		= %s\n texte		= %s\n autoroute	= %d\n pr		= %ld\n  sens		= %d\n creation	= %d\n site		= %d\n", vl_numAlerte, vl_date, vl_typeAlerte, vl_nomType, vl_typeEqt, vl_numEqt, vl_nom_Eqt, vl_texte, vl_autoroute, vl_pr, vl_sens, vl_creation, vl_siteAlerte); 	
*/
		/*A 
        	** Execution de la requete en elle meme : 
        	** Permet de creer ou supprimer une alerte dans la liste des alertes
        	** et sur le synoptique.
        	*/
		vl_retcode = XDC_OK;	
		if (vl_typeAlerte!=XZAAC_APPEL_IPHONE)
			vl_retcode = XZIR01_Alerte(	vl_numAlerte,
                      				vl_date,
                      				vl_typeAlerte,
                      				vl_nomType,
                      				vl_typeEqt,
                      				vl_numEqt,
                      				vl_nom_Eqt,
                      				vl_texte,
                      				vl_autoroute,
                      				vl_pr,
                      				vl_sens,
                      				vl_creation,
						vl_siteAlerte
                      				 );
		else
			vl_retcode = XDC_OK; 

		if (vl_retcode != XDC_OK) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : XZIR01_Alerte a ÅchouÅ");
			if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)vl_retcode ) != CS_SUCCEED)
        	
                        	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendinfo() a echoue.\n");
                	else 
                		XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_sendinfo a envoye un message d erreur\n");
		}
		
		
		/*A Initialisation du vl_status a 1 indiquant le succes ou l echec du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_senddone() a echoue.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : apres if vl_ok");

	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_DEBUG1,"OUT %s",version);
        return (CS_SUCCEED);

}

CS_RETCODE aarp02 (SRV_PROC *pa_srvproc)
/*
* ARGUMENTS EN ENTREE :
*
*       Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur modification d'une FMC
*
* FONCTION
*   Envoie un message a Magistra pour notifier la modification
*
*
------------------------------------------------------*/
{
	static char		*version = "asrv_arp.c : aarp02" ;
	int				vl_retcode;		/* Code de retour */
	CS_BOOL         vl_ok;	
	CS_INT          vl_numparams;	/* Permet de recuperer le nbre de param passes a la rpc */
	CS_SERVERMSG    vl_msg;         /* structure permettant de ren voyer un message d erreur au  client.                      */
	CS_CONTEXT      *pl_cp;         /* Structure contenant les infos de localisation du serveur   */
	CS_INT          vl_status;      /* Contient de le vl_status du srv apres reception de la demande */
	CS_INT          vl_Eqt;         /* variable pour recuperer le param venant du sql serveur */
	CS_TINYINT      vl_nump;
	CS_INT          vl_param_no=1;

	XDY_Entier		vl_numero;
	XDY_Octet		vl_cle;
	XDY_Entier		vl_action;
	XDY_Entier		vl_valeur;
	XDY_Datagroup	vl_dg;
	XDY_NomSite		vl_NomSite;
		
	XZST_03EcritureTrace (XZSTC_INTERFACE,"IN %s",version);

	/* initialisation du vl_status a OK pour fin de transaction
		Si on oublie vl_status, on peut bloquer la transaction */
	vl_status = SRV_DONE_FINAL;

	/* initialisation de vl_ok a TRUE */
	vl_ok = CS_TRUE;

	/* verification du contenu du parametre venant du client recupere le nombre de parametres et on teste */
	if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
		vl_ok = CS_FALSE;

	/* On verifie qu'il y a bien le nb de param attendu, passe a la rpc */
	if (vl_numparams != ca_aarp02_nb_param)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "aarp02: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)", vl_numparams, ca_aarp02_nb_param);
		vl_ok = CS_FALSE;
	}

	/* 1er champ : vl_numero de type XDY_Entier = CS_INT */
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_numero))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "aarp02: parametre %d errone", vl_param_no);
		vl_ok = CS_FALSE;
	}   

	/* 2 eme champ : vl_cle de type  XDY_Octet : CS_TINYINT */
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_cle))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "aarp02: parametre %d errone", vl_param_no);
		vl_ok = CS_FALSE;
	}

	/* 3eme champ : vl_action de type XDY_Entier = CS_INT */
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_action))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "aarp02: parametre %d errone", vl_param_no);
		vl_ok = CS_FALSE;
	} 

	/* 4eme champ : vl_valeur de type XDY_Entier = CS_INT */
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_valeur))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "aarp02: parametre %d errone", vl_param_no);
		vl_ok = CS_FALSE;
	} 

	/* Transfert des parametres dans les variables locales au programme */
	if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
		vl_ok = CS_FALSE;

	if (!vl_ok )
	{
		/* Initialise le message d erreur a renvoyer si un probleme est apparu. */
		/* mise a zero du vl_buffer de message */
		srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));

		/* indicateur du type de message et de sa gravite */
		vl_msg.msgnumber = RPC_FAIL;
		vl_msg.severity = 10;

		/* Initialisation du message pour le client */
		(CS_VOID)sprintf(vl_msg.text, "aarp02 : message non correct.\n");
		vl_msg.textlen = strlen(vl_msg.text);

		/* Allocation de la structure de contexte */
		if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"aarp02 : cs_ctx_alloc() a echoue.\n");
		}
		else
		{
			/* Recuperation du nom du serveur */
			(CS_VOID)srv_props (pl_cp, CS_GET, SRV_S_SERVERNAME,
			(CS_VOID *) vl_msg.svrname, sizeof (vl_msg.svrname), &vl_msg.svrnlen);
		}

		/* Envoi du message d erreur au client */
		if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
			XZST_03EcritureTrace (XZSTC_WARNING, "aarp02 : srv_sendinfo() a echoue\n");
		else
			XZST_03EcritureTrace (XZSTC_SGBD, "aarp02 : srv_sendinfo a envoye un message d erreur\n");

		/* Initialisation du vl_status de la requete par le serveur */
		vl_status |= SRV_DONE_ERROR;

		/* Envoi du vl_status de la requete indiquant une erreur. */
		if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
			XZST_03EcritureTrace (XZSTC_WARNING, "aarp02 : srv_sendstatus() a echoue.\n");
		else
			XZST_03EcritureTrace (XZSTC_SGBD, "aarp02 :  srv_sendstatus() a renvoye vl_status = %d ", vl_status);
	}

	/* Recupere le nom du site */
	if( (vl_retcode =XZSC_04NomSite(vl_NomSite))!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "aarp02 : XZSC_07NomSite a echoue : code = %d", vl_retcode );
		exit (XDC_NOK);
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "aarp02 : Recuperation du nom Site a reussi" );
	}

	/* Envoi au client la confirmation du traitement de la demande */
	if (vl_ok)
	{
		XDG_EncodeDG2(vl_dg, XDG_ACT, vl_NomSite);  				

		/* Ecriture du Msg XDM_AS_UPDATE_PA */
		if (!TipcSrvMsgWrite (vl_dg,
			XDM_IdentMsg(XDM_AS_UPDATE_PA),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_INT4, vl_numero,
			T_IPC_FT_INT2, vl_cle,
			T_IPC_FT_INT4, vl_action,				
			T_IPC_FT_INT4, vl_valeur,				
			NULL ))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "aarp02 : Erreur ecriture message XDM_AS_UPDATE_PA: vl_dg=%s, vl_numero=%d, vl_cle=%d, vl_action=%d, vl_valeur=%d", vl_dg, vl_numero, vl_cle, vl_action, vl_valeur);
		} 
		else 
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "aarp02 : Ecriture message XDM_AS_UPDATE_PA success. vl_dg=%s, vl_numero=%d, vl_cle=%d, vl_action=%d, vl_valeur=%d", vl_dg, vl_numero, vl_cle, vl_action, vl_valeur);
		}     

		/* Initialisation du vl_status a 1 indiquant le succes ou l echec du traitement de la requete. */
		if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
		{
			vl_status |= SRV_DONE_ERROR;
			XZST_03EcritureTrace (XZSTC_WARNING,"aarp02 : srv_sendstatus() a echoue.\n");
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_SGBD,"aarp02 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
		}
		/* Envoi du paquet final au client. */
		if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
			XZST_03EcritureTrace(XZSTC_WARNING, "aarp02 : srv_senddone() a echoue.\n");
		else
			XZST_03EcritureTrace(XZSTC_SGBD, "aarp02 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}

	/* Scrute les messages du RTserver afin de declancher les CallBacks */
	asrv_lire_RTSRV_MSG();
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
	return (CS_SUCCEED);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*  RP affaisant appel a XZIR01
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE aarp11(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*       Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
*
* FONCTION
*       Passe les parametres £ XZIR01.
*       On passe soit NomMachine soit NomSite, l un des deux doit etre NULL.
*
*
------------------------------------------------------*/
{
        static char *version = "$Id: asrv_arp.c,v 1.11 2021/04/23 08:17:47 gesconf Exp $ : asrv_arp11" ;
        int             vl_retcode;     /* Code de retour               */


        CS_BOOL         vl_ok;

        CS_INT          vl_numparams;
                                        /* Permet de recuperer le nbre  */
                                        /* de param passes a la rpc     */


        CS_SERVERMSG    vl_msg;         /* structure permettant de ren- */
                                        /* voyer un message d erreur au */
                                        /* client.                      */

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos*/
                                        /* de localisation du serveur   */

        CS_INT          vl_status;      /*
                                        ** Contient de le vl_status du srv
                                        ** apres reception de la demande
                                        */

        CS_INT          vl_Eqt;         /* variable pour recuperer le
                                        ** param venant du sql serveur
                                        */

        CS_TINYINT      vl_nump;

        CS_INT          vl_param_no=1;



        /*
        ** Variables specifiques a l appel des services externes
        */
        XDY_Evt         vl_Evt_In;              /*
                                                ** Numero de la fiche main courante
                                                */
        XDY_Entier        vl_numAlerte;

        CS_DATETIME       vl_DateTime;          /*
                                                ** Param recu de la base a transcrire
                                                ** en XDY_Horodate dans vl_date.
                                                */
        XDY_Horodate      vl_date;
        XDY_TypeAlerte    vl_typeAlerte;
        XDY_NomType       vl_nomType;
        XDY_TypeEqt       vl_typeEqt;
        XDY_Eqt           vl_numEqt;
        XDY_NomEqt        vl_nom_Eqt;
        XDY_TexteAlerte   vl_texte;
        XDY_Autoroute     vl_autoroute;
        XDY_PR            vl_pr;
        XDY_Sens          vl_sens;
        XDY_Booleen       vl_creation;
        XDY_Octet       vl_siteAlerte;
        XDY_Mot vl_portion;
        XDY_Entier      vl_translation;
       XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);

        /*A
        **  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /*
        ** initialisation de vl_ok a TRUE
        */
        vl_ok = CS_TRUE;
        /*A
        ** verification du contenu du parametre venant du client
        */
        /*A
        ** recupere le nombre de parametres et on teste
        */
        if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
                vl_ok = CS_FALSE;

        /*A
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc
        */
        if (vl_numparams != ca_aarp11_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_aarp01_nb_param);
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  aarp01: vl_numAlerte de type XDY_Entier = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_numAlerte))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 2 eme champ : vl_DateTime : type  = CS_DATETIME
        */
        if (vl_ok && (asrv_lire_date(pa_srvproc, vl_param_no++,&vl_DateTime)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme champ : vl_typeAlerte de type  XDY_Octet : tinyint
        */

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_typeAlerte))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 4 eme champ : vl_nomType type CS_CHAR de long XDC_Lon_NomSysteme
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_nomType,XDC_Lon_Nom)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme champ : vl_typeEqt de type  XDY_Octet : tinyint
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_typeEqt))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 6 eme champ : vl_numEqt de type  XDY_Mot : smallint
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_numEqt))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 7 eme champ :vl_Nom_Eqt type CS_CHAR de long XDC_Lon_NomEqt
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_nom_Eqt,XDC_Lon_NomEqt)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 8 eme champ :vl_texte type CS_CHAR de long XDC_Lon_TexteAlerte
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_texte,XDC_Lon_TexteAlerte)!=CS_SUCCEED))

        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 9 eme champ : vl_autoroute de type  XDY_Octet : tinyint
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_autoroute))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 10 eme champ : vl_pr de type  XDY_PR (long) : CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_pr))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 11 eme champ : vl_sens de type  XDY_Sens (unsigned char) : CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_sens))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 12 eme champ : vl_creation de type  XDY_Booleen (unsigned char) : CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_creation))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 13 eme champ : vl_siteAlerte de type  XDY_Octet : CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_siteAlerte))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 14 eme champ : vl_portion de type  XDY_Mot : CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_portion))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 15 eme champ : vl_translation de type  XDY_Entier : CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_translation))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*A
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

        /*A
        **  Transfert de la date du format Sybase au format migrazur
        */

        vl_retcode=asql_date_Sybase2Ux (vl_DateTime,&vl_date);
        if (vl_retcode != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aarp01 : asql_date_Sybase2Ux() a echoue");
                return(vl_retcode);
        }

        if (!vl_ok )
        {
                /*A
                ** Initialise le message d erreur a renvoyer si un probleme est
                ** apparu.
                */

               /*
                ** mise a zero du vl_buffer de message
                */
                srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));
                /*
                ** indicateur du type de message et de sa gravite
                */
                vl_msg.msgnumber = RPC_FAIL;
                vl_msg.severity = 10;
                /*A
                ** Initialisation du message pour le client
                */
                (CS_VOID)sprintf(vl_msg.text,"aarp01 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : cs_ctx_alloc() a echoue.\n");
                }
                else
                {
                        /*
                        ** Recuperation du nom du serveur
                        */
                        (CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                        (CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
                }
                /*B
                **  Envoi du message d erreur au client
                */
                if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendinfo() a echoue.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendstatus() a echoue.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 :  srv_sendstatus() a renvoye vl_status = %d ",vl_status);
        }


        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"Valeurs recues : \nportion %d\ntranslation %d\n",
                                vl_portion,
                                vl_translation);
                /*A
                ** Execution de la requete en elle meme :
                ** Permet de creer ou supprimer une alerte dans la liste des alertes
                ** et sur le synoptique.
                */


	 vl_retcode = XZIR11_Alerte_Localisee(   vl_numAlerte,
                                                vl_date,
                                                vl_typeAlerte,
                                                vl_nomType,
                                                vl_typeEqt,
                                                vl_numEqt,
                                                vl_nom_Eqt,
                                                vl_texte,
                                                vl_autoroute,
                                                vl_pr,
                                                vl_sens,
                                                vl_creation,
                                                vl_siteAlerte,
                                                vl_portion,
                                                vl_translation
                                                 );
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : XZIR01_Alerte a echoue");
                        if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)vl_retcode ) != CS_SUCCEED)

                                XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendinfo() a echoue.\n");
                        else
                                XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_sendinfo a envoye un message d erreur\n");
                }


                /*A Initialisation du vl_status a 1 indiquant le succes ou l echec du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_sendstatus a reussi a envoyer le vl_st atus = %d du traitement de la requete\n",vl_status);
                }
                /*A
                ** Envoi du paquet final au client.
                */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarp01 : srv_senddone() a echoue.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"aarp01 : srv_senddone envoie CS_TRAN_COMPLETED pour co mpleter la transaction\n");
        }
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();


        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return (CS_SUCCEED);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*  RP affaisant appel a XZIR01
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE aarpDatex2(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*       Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation d'une FMC
*
*
*
* FONCTION
*       Sur validation d'une FMC envoie un message à tada2
*
*
------------------------------------------------------*/
{
        static char *version = "$Id: asrv_arp.c,v 1.11 2021/04/23 08:17:47 gesconf Exp $ : asrv_arpDatex2" ;
        int             vl_retcode;     /* Code de retour               */


        CS_BOOL         vl_ok;

        CS_INT          vl_numparams;
                                        /* Permet de recuperer le nbre  */
                                        /* de param passes a la rpc     */


        CS_SERVERMSG    vl_msg;         /* structure permettant de ren- */
                                        /* voyer un message d erreur au */
                                        /* client.                      */

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos*/
                                        /* de localisation du serveur   */

        CS_INT          vl_status;      /*
                                        ** Contient de le vl_status du srv
                                        ** apres reception de la demande
                                        */

        CS_INT          vl_Eqt;         /* variable pour recuperer le
                                        ** param venant du sql serveur
                                        */

        CS_TINYINT      vl_nump;

        CS_INT          vl_param_no=1;



        /*
        ** Variables specifiques a l appel des services externes
        */

        XDY_Entier      vl_numero;
        XDY_Octet       vl_cle;
		
       XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);

        /*A
        **  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /*
        ** initialisation de vl_ok a TRUE
        */
        vl_ok = CS_TRUE;
        /*A
        ** verification du contenu du parametre venant du client
        */
        /*A
        ** recupere le nombre de parametres et on teste
        */
        if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
                vl_ok = CS_FALSE;

        /*A
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc
        */
        if (vl_numparams != ca_aarpDate2_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_aarp01_nb_param);
                vl_ok = CS_FALSE;
        }
        /*
        ** 1 er champ de  aarpDatex2: vl_numero de type XDY_Entier = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }   

        /*
        ** 2 eme champ : vl_cle de type  XDY_Octet : CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_cle))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        
        /*A
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

        if (!vl_ok )
        {
                /*A
                ** Initialise le message d erreur a renvoyer si un probleme est
                ** apparu.
                */

               /*
                ** mise a zero du vl_buffer de message
                */
                srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));
                /*
                ** indicateur du type de message et de sa gravite
                */
                vl_msg.msgnumber = RPC_FAIL;
                vl_msg.severity = 10;
                /*A
                ** Initialisation du message pour le client
                */
                (CS_VOID)sprintf(vl_msg.text,"aarpDatex2 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 : cs_ctx_alloc() a echoue.\n");
                }
                else
                {
                        /*
                        ** Recuperation du nom du serveur
                        */
                        (CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                        (CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
                }
                /*B
                **  Envoi du message d erreur au client
                */
                if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 : srv_sendinfo() a echoue\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"aarpDatex2 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 : srv_sendstatus() a echoue.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"aarpDatex2 :  srv_sendstatus() a renvoye vl_status = %d ",vl_status);
        }


        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
			/* Ecriture du Msg FMC validee pour passerelle DATEX2 XDG_TIMER_DATEX2*/
			vl_retcode = TipcSrvMsgWrite (XDG_TIMER_TAGEN,
				XDM_IdentMsg(XDM_AS_TIMER_SITU_CPC),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, vl_numero,
				T_IPC_FT_INT2, vl_cle,
				T_IPC_FT_INT4, 1,				
				NULL );

			if (vl_retcode != XDC_OK)
			{
										   
				XZST_03EcritureTrace( XZSTC_WARNING, "aarpDatex2 :Erreur ecriture message XDM_AS_TIMER_SITU_CPC. num=%d,cle=%d",vl_numero,vl_cle);
			} 
			else 
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 :Ecriture message XDM_AS_TIMER_SITU_CPC success.num=%d,cle=%d",vl_numero,vl_cle);
			}     				
			
			/*A Initialisation du vl_status a 1 indiquant le succes ou l echec du
			**  traitement de la requete.
			*/
			if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
			{
					vl_status |= SRV_DONE_ERROR;
					XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 : srv_sendstatus() a echoue.\n");
			}
			else
			{
					XZST_03EcritureTrace(XZSTC_SGBD,"aarpDatex2 : srv_sendstatus a reussi a envoyer le vl_st atus = %d du traitement de la requete\n",vl_status);
			}
			/*A
			** Envoi du paquet final au client.
			*/
			if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
					XZST_03EcritureTrace(XZSTC_WARNING,"aarpDatex2 : srv_senddone() a echoue.\n");

			else XZST_03EcritureTrace(XZSTC_SGBD,"aarpDatex2 : srv_senddone envoie CS_TRAN_COMPLETED pour co mpleter la transaction\n");
			
		}
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);

        return (CS_SUCCEED);
}
