/*E*/
/*Fichier : $Id: asrv_crp.c,v 1.30 2021/06/08 11:32:54 pc2dpdy Exp $     Release : $Revision: 1.30 $      Date : $Date: 2021/06/08 11:32:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE CRP * FICHIER asrv_crp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
**********************************
*
*	acrpEXEMPLE : Juste un exemple
*
**********************************
*
*	acrp01 : Recoit les parametres du SQL Serveur et appelle 
*   	XZEP01_Commande_PMV
*	XZEP02_Commande_Picto
*
**********************************
*
*	acrp03 est appele par un SQL serveur et fait appel È xzen03()
*
**********************************
*
*	acrp06 est appele par un SQL serveur et fait appel È xzet03()
*
**********************************
*
*	acrp08 est appele par un SQL serveur et fait appel È xzet04()
*
**********************************
*
*	acrp19 est appele par un SQL serveur et fait appel a XZIT06_Modif_Animateur_TFM()
*
**********************************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	25 Aug 1994	: Creation
------------------------------------------------------
* mercier	29 Sep 1994	: Modification V 1.10
*	Rajoue des flags  static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_mrp*" ;
***************
* MERCIER	16 Dec 1994	: Modification V 1.12
*	remplace XZEN03_Commande_SCEN_FICH en XZEN02_Commande_SCEN.
* VERDIER	02 Sep 1996	: Ajout 4e ligne ACRP01 V1.13
* JMG		27/04/04	: ajout ACRP99 1.15
* JMG		11/10/05	: ajout PMVA BAF BAD 1.16
* AAZ       28/08/2007  : Ajout acrp40, acrp41, acrp42, acrp43, acrp44 DEM 666
* JBL	Mai 2008 :  DEM666
* JPL		03/11/09 : Ajout parametres commande et action TFM a acrp19 (DEM 917) 1.19
* VR		07/11/11 : Ajout DestinationNoeud (DEM/1014)
* VR		05/03/12 : Ajout PRV acrp15(DEM/1016)
* VR		08/06/12 : Ajout destination acrp11 (DEM/1014 PMA)
* JMG		26/04/1- : ajout acrp111 SAGA 1.23
* LCL		02/03/18 : Ajout acrp60 CTRL_FEUX 1.24 DEM1284
* ABE		17/12/20 : Ajout acrp70 pour IMU DEM-SAE155 1.25
* ABE		06/04/21 : Ajout acrp80 pour RAU SONO DEM-SAE244 1.26
* ABE		08/06/21 : Fix autoroute en str pour acrp 80 DEM-SAE244 1.30
* GGY		14/11/23 : Ajout acrp16 pour les PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_crp.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  acrpEXEMPLE : Juste un exemple
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrpEXEMPLE(SRV_PROC        *pa_srvproc)

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
*
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crpEXEMPLE" ;
	int		vl_retcode = XDC_OK;	/* Code de retour			*/
	
        CS_BOOL         vl_ok;
        
        CS_INT          vl_numparams= 0;
              				/* Permet de recuperer le nbre  	*/
                                        /* de param passes a la rpc     	*/
        CS_INT          vl_len;         /* Longueur du vl_buf recuperation 	*/
                                        /* des datas                    	*/
        CS_SMALLINT     vl_outlen;      /* Pointeur sur vl_buffer contenant	*/
                                        /* lindicateur de la valeur NULL	*/
        CS_DATAFMT      vl_datafmt;
					/* Sert a recuperer la struct   	*/
                                        /* CS_DATAFMT permettant de     	*/
                                        /* decrire le format des datas  	*/
                                        /* et les variables du program  	*/
                                        /* Pour plus de details, voir   	*/
                                        /* Pour plus de details, voir   	*/
                                        /* page 2-25 de Open Serv Ref Ma	*/
                                        
        CS_CHAR        vl_buf[CS_MAX_CHAR];/* variable qui va recevoir     	*/
                                        /* le message proprement dit    	*/

        CS_CHAR        vl_buf2[CS_MAX_CHAR]; /* variable qui va recevoir   	*/
                                        /* le message proprement dit    	*/

        CS_SERVERMSG    vl_msg;         /* structure permettant de ren- 	*/
                                        /* voyer un message d erreur au 	*/
                                        /* client.                      	*/

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos	*/
                                        /* de localisation du serveur   	*/

        CS_INT          vl_status;      /* 
        				** Contient de le vl_status du srv 
        				** apres reception de la demande
        				*/

        CS_INT	vl_Eqt;			/* variable pour recuperer le   
        				** param venant du sql serveur
        				*/
	
	CS_TINYINT		vl_nump;
	
	CS_INT		vl_param_no;

        /* Variables specifiques a l appel des services externes	*/
        	
	XZEPT_CMD_PMV   vl_Commande_PMV;	/* Commande a effectuee	*/
	XZEPT_CMD_PICTO vl_Commande_PICTO;	/* Commande a effectuee	*/
	int             vl_NoCMD;		/* Numero de la commande*/
	XDY_NomMachine  pl_NomMachine;		/* nom de la machine	*/


	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrpEXEMPLE : debut d execution ");
	
	/*A 
	**  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /* 
        ** initialisation de vl_ok a TRUE                                  
        */
        vl_ok = CS_TRUE;

        /* 
        ** initialisation des structures a NULL                        
        */
        srv_bzero(&vl_datafmt, sizeof(CS_DATAFMT));

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
        /*if (vl_numparams != ca_acrpEXEMPLE_nb_param)*/
        
	XZST_03EcritureTrace(XZSTC_SGBD,"acrpEXEMPLE : srv_numparams a detecte %d parametres passÅs È la rp",vl_numparams);
	
        if (vl_numparams != 2)  
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
	vl_datafmt.maxlength = CS_MAX_CHAR;
        vl_len = vl_datafmt.maxlength;
        vl_datafmt.datatype = CS_CHAR_TYPE;
	vl_param_no = 1;
        
        /*A 
        ** Recuperation du message du parametre message.
        ** execute la commande CS_GET sur un type SRV_RPCDATA dans
        ** laquelle on a 1 item parametre
        */

	if (vl_ok && (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, vl_param_no, &vl_datafmt,(CS_BYTE *)vl_buf,&vl_len,&vl_outlen)!=CS_SUCCEED))
     	{
         	/* 
         	** Erreur detectee 
         	*/
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}

        vl_datafmt.datatype =  CS_TINYINT_TYPE;
	vl_datafmt.maxlength = (CS_INT)sizeof(CS_TINYINT);
	vl_param_no = 2;

	if (vl_ok && (srv_bind(pa_srvproc, CS_GET, SRV_RPCDATA, vl_param_no, &vl_datafmt,(CS_BYTE *)&vl_nump,NULL,NULL)!=CS_SUCCEED))
	{
		/*
		** Erreur detectee
		*/
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
        /*A 
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

	if (!vl_ok || (strlen(vl_buf)==0))
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrpEXEMPLE : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : srv_sendinfo() a echouÅ.\n");
                /*else printf("Apres srv_sendinfo \n");*/

		/*B Initialisation du vl_status de la requete par le serveur*/
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : srv_sendstatus() a echouÅ.\n");
                        }
        }
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
        /*printf("EQT recu = %d\n\n",vl_Eqt);*/
        
        printf("Buf recu = %s vl_nump = %d\n",vl_buf,vl_nump);
        
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT) 1) != CS_SUCCEED)
                {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : srv_sendstatus() a echoue.\n");
                }
        else
                {
                printf("srv_sendstatus a reussi a envoyer le status du traitement de la requete\n");
                }

	/* Execution de la requete en elle meme : Affichage du vl_msg*/
	/*
	vl_retcode = XZEP01_Commande_PMV(vl_Commande_PMV,vl_NoCMD,pl_NomMachine);
	vl_retcode = XZEP02_Commande_Picto(vl_Commande_PICTO,vl_NoCMD,pl_NomMachine);
	*/
	
 	/*A 
 	** Envoi du paquet final au client.                             
 	*/
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE : srv_senddone(CS_TRAN_COMPLETED) a echouÅ.\n");

 	else XZST_03EcritureTrace(XZSTC_SGBD," acrpEXEMPLE : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrpEXEMPLE : Fin d execution ");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrpEXEMPLE : Fin d execution ");
        return (CS_SUCCEED);
	
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	Recoit les parametres du SQL Serveur et appelle 
*   	XZEP01_Commande_PMV
*	XZEP02_Commande_Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp01(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*   XZEXC_ERR_PICTO     Symbole picto invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*	Appel 	xzep01
*		xzep02
*
*
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
*   Construction du message XDM_ECMD_Picto en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_Picto_<NomMachine> du site concerne.
*
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp01" ;
	int		vl_retcode= XDC_OK;	/* Code de retour		*/
	
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
        	
	XZEPT_CMD_PMV   vl_Commande_PMV;	/* Commande a effectuee	*/
	XZEPT_CMD_PICTO vl_Commande_PICTO;	/* Commande a effectuee	*/
	
	CS_INT          vl_NoCMD_In;		/* Numero de la commande*/
	XDY_NomMachine  vl_NomMachine_In;	/* nom de la machine	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp01 : debut d execution ");
	
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
        
        if (vl_numparams != ca_acrp01_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Numero))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       	

	/* 
	** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte1 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
       	/* 
       	** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage1))!=CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}

   	
       	/* 
       	** 4 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign1))!=CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}

	/* 
	** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat1 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}

	/* 
	** 6 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte2 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
       	** 7 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage2))!=CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
	
	/* 
       	** 8 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign2))!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
	** 9 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat2 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %s errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
	** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte3 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
       	** 11 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
       	*/	     
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage3))!=CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
       	** 12 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/    
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign3))!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
	** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV 
	*/
	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat3 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
       	** 14 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Flash))!=CS_SUCCEED))		
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
     	
       	/*
       	** Structure XDY_Etat_Picto
       	*/
	/* 
       	** 15 eme champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PICTO.Numero))!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
	/* 
	** 16 eme champ de XDY_Etat_PMV : type Texte[XDC_Lon_TxtPicto] 
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PICTO.Texte , XDC_Lon_TxtPicto))!=CS_SUCCEED)	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
	** 17 eme champ de XDY_Etat_PMV : type XDY_Picto  = char XDY_Picto[XDC_Lon_NomPicto] 
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PICTO.Picto , XDC_Lon_NomPicto))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
	/* 
       	** 18 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PICTO.Clign))!=CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
	
	/* 
       	** 19 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       
	/* 
	** 20 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
        /*
	** 21 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte4 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
	{  
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

	/*
	** 22 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
	*/

	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat4 , XDC_Lon_Ligne_PMV))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

	/* 
       	** 23 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Destination))!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

	/* 
       	** 24 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.DestinationAlternat))!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

	/* 
       	** 25 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
       	
	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.DepassSeuil))!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

//EN COURS
	/* 
       	** 26 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.DestinationNoeud))!=CS_SUCCEED))
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrp01 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_sendinfo() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp01 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_sendstatus() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp01 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
        
        	XZST_03EcritureTrace(XZSTC_WARNING," acrp01 : Champs pour la structure XDY_Etat_PMV\n");
     	   	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Numero     \t= %d",vl_Commande_PMV.Numero);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Texte1     \t= %s strlen = %d",vl_Commande_PMV.Texte1,strlen(vl_Commande_PMV.Texte1));
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Affichage1 \t= %d",vl_Commande_PMV.Affichage1);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Clign1     \t= %d",vl_Commande_PMV.Clign1);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Alternat1  \t= %s",vl_Commande_PMV.Alternat1);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Texte2     \t= %s",vl_Commande_PMV.Texte2);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Affichage2 \t= %d",vl_Commande_PMV.Affichage2);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Clign2     \t= %d",vl_Commande_PMV.Clign2);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Alternat2  \t= %s",vl_Commande_PMV.Alternat2);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Texte3     \t= %s",vl_Commande_PMV.Texte3);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Affichage3 \t= %d",vl_Commande_PMV.Affichage3);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Clign3     \t= %d",vl_Commande_PMV.Clign3);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Alternat3  \t= %s",vl_Commande_PMV.Alternat3);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.Flash      \t= %d",vl_Commande_PMV.Flash);
		
		XZST_03EcritureTrace(XZSTC_WARNING," acrp01 : Valeur recue : \t vl_Commande_PMV.Destination      \t= %d",vl_Commande_PMV.Destination);
		XZST_03EcritureTrace(XZSTC_WARNING," acrp01 : Valeur recue : \t vl_Commande_PMV.DestinationAlternat      \t= %d",vl_Commande_PMV.DestinationAlternat);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PMV.DepassSeuil      \t= %d",vl_Commande_PMV.DepassSeuil);
		XZST_03EcritureTrace(XZSTC_WARNING," acrp01 : Valeur recue : \t vl_Commande_PMV.DestinationNoeud       \t= %d",vl_Commande_PMV.DestinationNoeud);
	
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Champs pour la structure XDY_Etat_Picto\n");
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PICTO.Numero   \t= %d",vl_Commande_PICTO.Numero);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PICTO.Texte    \t= %s",vl_Commande_PICTO.Texte);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PICTO.Picto    \t= %s",vl_Commande_PICTO.Picto);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_Commande_PICTO.Clign    \t= %d",vl_Commande_PICTO.Clign);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp01 : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);
	
        
        	/*A
        	** Execution de la requete en elle meme : XZEP01_Commande_PMV
        	*/
		
	
		vl_retcode = XZEP01_Commande_PMV(vl_Commande_PMV,vl_NoCMD_In,vl_NomMachine_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : XZEP01_Commande_PMV() a ÅchouÅ : code =%d",vl_retcode);
		
		}
		
		vl_retcode = XZEP02_Commande_Picto(vl_Commande_PICTO,vl_NoCMD_In,vl_NomMachine_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : XZEP02_Commande_Picto() a ÅchouÅ : code = %d",vl_retcode);
			
		}
	
	
		/*A Initialisation du vl_status a 1 indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            	{
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

			
	
	
 		/* Envoi du paquet final au client.                             */
       		if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_senddone() a echouÅ.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"acrp01 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp01 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp01 : Fin d execution ");
        return (CS_SUCCEED);
	
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp03 est appele par un SQL serveur et fait appel È xzen03()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp03(SRV_PROC        *pa_srvproc)

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
*	Appel 	xzep01
*		xzep02
*
*
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
*   Construction du message XDM_ECMD_Picto en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_Picto_<NomMachine> du site concerne.
*
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp03" ;
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
        	
	XZENT_CMD_SCEN  vl_Commande_SCEN;	/* Commande a effectuee	*/
	
	CS_INT          vl_NoCMD_In;		/* Numero de la commande*/
	XDY_NomMachine  vl_NomMachine_In;	/* nom de la machine	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp03 : debut d execution ");
	
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
        
        if (vl_numparams != ca_acrp03_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrp03: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ de  acrp03: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SCEN.NoEqt))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       	
	/* 
	** 2 eme de type XDY_Scenario : char de XDC_Lon_Scenario
	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_SCEN.Scenar , XDC_Lon_Scenario))!=CS_SUCCEED)	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
 	/* 
       	** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       		
	/* 
	** 4 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine 
	*/
	
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrp03 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : srv_sendinfo() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp03 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : srv_sendstatus() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp03 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
        
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp03 : Champs pour la structure XDY_Etat_PMV\n");
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp03 : Valeur recue : \t vl_Commande_SCEN.NoEqt     \t= %d",vl_Commande_SCEN.NoEqt);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp03 : Valeur recue : \t vl_Commande_SCEN.Scenar    \t= %s strlen = %d",vl_Commande_SCEN.Scenar,strlen(vl_Commande_SCEN.Scenar));
		XZST_03EcritureTrace(XZSTC_SGBD," acrp03 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp03 : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);
	
        
        	/* Execution de la requete en elle meme : Affichage du vl_msg*/
	
		vl_retcode = XZEN02_Commande_SCEN(vl_Commande_SCEN,vl_NoCMD_In,vl_NomMachine_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : XZEN02_Commande_SCEN() a ÅchouÅ : code =%d",vl_retcode);
		}
		
		/*A Initialisation du vl_status a vl_retcode indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp03 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : srv_senddone() a echouÅ.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"acrp03 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp03 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp03 : Fin d execution ");
        return (CS_SUCCEED);
	
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp06 est appele par un SQL serveur et fait appel È xzet03()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp06(SRV_PROC        *pa_srvproc)

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
*	Appel 	xzet03
*		
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp06" ;
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
        	
	XZETT_CMD_SEQ_NIC   vl_Commande_SIG_NIC;	/* Commande a effectuee	*/
	
	CS_INT          vl_NoCMD_In;		/* Numero de la commande*/
	XDY_NomMachine  vl_NomMachine_In;	/* nom de la machine	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp06 : debut d execution ");
	
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
        
        if (vl_numparams != ca_acrp06_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrp06: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ de  acrp06: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SIG_NIC.NumTE))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
       	/* 
       	** 2 eme champ :vl_Commande_SIG_NIC.NumSEQ de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SIG_NIC.NumSEQ))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       	
       
	/* 
       	** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrp06 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : srv_sendinfo() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp06 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : srv_sendstatus() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp06 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
        
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp06 : Champs pour la structure \n");
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp06 : Valeur recue : \t vl_Commande_SIG_NIC.NumTE  \t= %d",vl_Commande_SIG_NIC.NumTE);       
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp06 : Valeur recue : \t vl_Commande_SIG_NIC.NumSEQ \t= %d",vl_Commande_SIG_NIC.NumSEQ);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp06 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
		
        
        	/*A
        	** Execution de la requete en elle meme : XZET03_Commande_SEQT
        	*/
	
		vl_retcode = XZET03_Commande_SEQT (vl_Commande_SIG_NIC,vl_NoCMD_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : XZET03_Commande_SEQT() a ÅchouÅ : code =%d",vl_retcode);
		
		}
		/*A Initialisation du vl_status a 1 indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp06 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"acrp06 : srv_senddone() a echouÅ.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"acrp06 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp06 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp06 : Fin d execution ");
        return (CS_SUCCEED);
	
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp08 est appele par un SQL serveur et fait appel È xzet04()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp08(SRV_PROC        *pa_srvproc)

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

* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*	Appel 	xzet03
*		
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp08" ;
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
        	
	XZETT_CMD_SEQ_NIC   vl_Commande_SIG_NIC;	/* Commande a effectuee	*/
	
	CS_INT          vl_NoCMD_In;		/* Numero de la commande*/
	XDY_NomMachine  vl_NomMachine_In;	/* nom de la machine	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp08 : debut d execution ");
	
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
        
        if (vl_numparams != ca_acrp08_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrp08: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ de  acrp08: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SIG_NIC.NumTE))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
       	/* 
       	** 2 eme champ :vl_Commande_SIG_NIC.NumSEQ de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SIG_NIC.NumSEQ))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       	
       
	/* 
       	** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
        /*A 
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

	/*if (!vl_ok || (strlen(vl_buf1)==0))*/
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrp08 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : srv_sendinfo() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp08 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : srv_sendstatus() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp08 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A 
        **  Envoi au client la confirmation du traitement de la demande  
        */
        if (vl_ok)
        {
        
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp08 : Champs pour la structure \n");
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp08 : Valeur recue : \t vl_Commande_SIG_NIC.NumTE  \t= %d",vl_Commande_SIG_NIC.NumTE);       
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp08 : Valeur recue : \t vl_Commande_SIG_NIC.NumSEQ \t= %d",vl_Commande_SIG_NIC.NumSEQ);
        	XZST_03EcritureTrace(XZSTC_SGBD," acrp08 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
		
        
        	/*A
        	** Execution de la requete en elle meme : XZET04_Commande_SEQE
        	*/
	
		vl_retcode = XZET04_Commande_SEQE (vl_Commande_SIG_NIC,vl_NoCMD_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : XZET04_Commande_SEQE() a ÅchouÅ : code = %d",vl_retcode);
		             	  	
		}
		/*A Initialisation du vl_status a 1 indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp08 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"acrp08 : srv_senddone() a echouÅ.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"acrp08 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp08 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp08 : Fin d execution ");
        return (CS_SUCCEED);
	
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp19 est appele par un SQL serveur et fait appel a XZIT06_Modif_Animateur_TFM()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp19(SRV_PROC        *pa_srvproc)

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

* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*	Appel 	xzet03
*		
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)asrv_crp.c	1.19 11/04/09 : asrv_crp19" ;
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

        CS_INT		vl_numAction=0;	/* variable pour recuperer le   
        				** param venant du sql serveur
        				*/
	
	CS_INT		vl_param_no=1;

	
        /* 
        ** Variables specifiques a l appel des services externes	
        */

	XDY_Octet	vl_commande = 0;		/* Commande */
	XDY_Act		vl_actionTFM = { 0, 0 };	/* Action Trafic FM concernee */


	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp19 : debut d execution ");
	
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
        
        if (vl_numparams != ca_acrp19_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"acrp19: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams, ca_acrp19_nb_param);
        	vl_ok = CS_FALSE;
        }

        /* 
       	** 1 er champ de acrp19 : type d'action/commande de type  XDY_Octet : CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++, &vl_commande) != CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp19: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}

	/* 
       	** 2 eme champ : numero d'action TFM de type : CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++, &vl_numAction) != CS_SUCCEED))
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"acrp19: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}

       	/* 
       	** 3 eme champ : cle (site) de l'action TFM de type  XDY_Octet : CS_TINYINT
       	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++, &vl_actionTFM.cle) != CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp19: parametre %d errone",vl_param_no);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"acrp19 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : srv_sendinfo() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp19 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : srv_sendstatus() a echouÅ.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp19 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A 
        **  Envoi au client la confirmation du traitement de la demande  
        */
        if (vl_ok)
        {
        	/* Execution de la requete en elle meme : XZIT06_Modif_Animateur_TFM*/
	
		vl_actionTFM.numero = vl_numAction;
		vl_retcode = XZIT06_Modif_Animateur_TFM (vl_commande, vl_actionTFM);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : XZIT06_Modif_Animateur_TFM() a echoue : code = %d",vl_retcode);
		             	  	
		}
		/*A Initialisation du vl_status a 1 indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"acrp19 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"acrp19 : srv_senddone() a echoue.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"acrp19 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp19 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp19 : Fin d execution ");
        return (CS_SUCCEED);
	
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*
*  acrp99 est appele par un SQL serveur et fait appel H xzeb01()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp99(SRV_PROC        *pa_srvproc)

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
* FONCTION
*       Appel   xzep01
*               xzep02
*
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
*   Construction du message XDM_ECMD_Picto en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_Picto_<NomMachine> du site concerne.
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c  1.14 05/06/97 : asrv_crp03" ;
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

	char		vl_cmd[6];

        /*
        ** Variables specifiques a l appel des services externes
        */

        XZEBT_CMD_PAL  vl_Commande_SCEN;       /* Commande a effectuee */

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp99 : debut d execution ");

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

        if (vl_numparams != ca_acrp99_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,
			"acrp99: Le nbre de parametres (%d) est pas celui attendu (%d)",
			vl_numparams,
			ca_acrp99_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de  acrp99: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SCEN.Numero)) 
		!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme de type XDY_Scenario : char de XDC_Lon_Scenario
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, 
		vl_cmd , XDC_Lon_Scenario+2))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))

        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , 
			XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrpEXEMPLE: parametre %d errone",vl_param_no);
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

                (CS_VOID)sprintf(vl_msg.text,"acrp99 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp03 : cs_ctx_alloc() a echoue.\n ");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp99 : srv_sendinfo() a echouE.\n ");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,
				"acrp99 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp99 : srv_sendstatus() a echouE.  \n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,
				"acrp99 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
		strcpy(vl_Commande_SCEN.Scenario,vl_cmd);
                XZST_03EcritureTrace(XZSTC_SGBD,
			" acrp99 : Valeur recue : \t vl_Commande_SCEN.Numero     \t= %d",
			vl_Commande_SCEN.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD,
			" acrp99 : Valeur recue : \t vl_Commande_SCEN.Scenario    \t= %s strlen = %d",
			vl_Commande_SCEN.Scenario,strlen(vl_Commande_SCEN.Scenario));
                XZST_03EcritureTrace(XZSTC_SGBD,
			" acrp99 : Valeur recue : \t vl_NoCMD_In \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD,
			" acrp99 : Valeur recue : \t vl_NomMachine_In \t= %s",vl_NomMachine_In);


                /* Execution de la requete en elle meme : Affichage du vl_msg*/
                vl_retcode = XZEB01_Commande_PAL(vl_Commande_SCEN,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,
				"acrp99 : XZEB01_Commande_PAL() a EchouE : code =%d",vl_retcode);
                }

                /*A Initialisation du vl_status a vl_retcode indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp99 : srv_sendstatus() a echoue. \n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,
				"acrp99 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /*A
                ** Envoi du paquet final au client.
                */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp99 : srv_senddone() a echouE.\n ");

                else XZST_03EcritureTrace(XZSTC_SGBD,
			"acrp99 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp99 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp99 : Fin d execution ");
        return (CS_SUCCEED);

}


/*PMVA DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP04_Commande_PMVA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp11(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMVA.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_PMVA en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMVA_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp11" ;
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
        XZEPT_CMD_PMVA   vl_Commande_PMV;       /* Commande a effectuee */

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp11 : debut d execution ");

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

        if (vl_numparams != ca_acrp11_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte1 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage1))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign1))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat1 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 6 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte2 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 7 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage2))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }


        /*
        ** 8 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign2))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 9 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat2 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte3 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 11 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage3))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 12 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign3))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat3 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 14 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte4 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 15 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage4))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 16 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign4))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 17 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat4 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 18 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte5 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 19 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage5))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 20 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clign5))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 21 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat5 , XDC_Lon_Ligne_PMVA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 22 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Flash))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 23 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
        */

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.DepassSeuil))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
		

        /*
        ** 23 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */

        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Dest1_proche))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 24 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
        */


        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Dest1_loin))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 25 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */

        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Dest2_proche))!=CS_SUCCEED))
         {
                 XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                 vl_ok = CS_FALSE;
         }

         /*
         ** 26 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
         */


         if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Dest2_loin))!=CS_SUCCEED))
         {
                 XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                 vl_ok = CS_FALSE;
         }
		

        /*
        ** 28 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
		
        /*
        ** 29 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
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

                /*ADepassSeuil
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

                (CS_VOID)sprintf(vl_msg.text,"acrp11 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : srv_sendinfo() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp11 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : srv_sendstatus() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp11 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

                XZST_03EcritureTrace(XZSTC_SGBD," acrp11 : Champs pour la structure XDY_Etat_PMVA\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Numero     \t= %d",vl_Commande_PMV.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte1     \t= %s strlen = %d",vl_Commande_PMV.Texte1,strlen(vl_Commande_PMV.Texte1));
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage1 \t= %d",vl_Commande_PMV.Affichage1);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clign1     \t= %d",vl_Commande_PMV.Clign1);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat1  \t= %s",vl_Commande_PMV.Alternat1);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte2     \t= %s",vl_Commande_PMV.Texte2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage2 \t= %d",vl_Commande_PMV.Affichage2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clign2     \t= %d",vl_Commande_PMV.Clign2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat2  \t= %s",vl_Commande_PMV.Alternat2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte3     \t= %s",vl_Commande_PMV.Texte3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage3 \t= %d",vl_Commande_PMV.Affichage3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clign3     \t= %d",vl_Commande_PMV.Clign3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat3  \t= %s",vl_Commande_PMV.Alternat3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte4     \t= %s",vl_Commande_PMV.Texte4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage4 \t= %d",vl_Commande_PMV.Affichage4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clign4     \t= %d",vl_Commande_PMV.Clign4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat4  \t= %s",vl_Commande_PMV.Alternat4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte5     \t= %s",vl_Commande_PMV.Texte5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage5 \t= %d",vl_Commande_PMV.Affichage5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clign5     \t= %d",vl_Commande_PMV.Clign5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat5  \t= %s",vl_Commande_PMV.Alternat5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Flash      \t= %d",vl_Commande_PMV.Flash);

                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t Destination un proche      \t= %d",vl_Commande_PMV.Dest1_proche);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t Destination un loin        \t= %d",vl_Commande_PMV.Dest1_loin);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t Destination deux proche      \t= %d",vl_Commande_PMV.Dest2_proche);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t Destination deux loin        \t= %d",vl_Commande_PMV.Dest2_loin);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t Depass Seuil               \t= %d",vl_Commande_PMV.DepassSeuil);

                /*A
                ** Execution de la requete en elle meme : XZEP04_Commande_PMVA
                */


                vl_retcode = XZEP04_Commande_PMVA(vl_Commande_PMV,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : XZEP04_Commande_PMVA() a ÅchouÅ : code =%d",vl_retcode);

                }


                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }




                /* Envoi du paquet final au client.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_senddone() a echouÅ.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp01 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }

        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp01 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp01 : Fin d execution ");
        return (CS_SUCCEED);

}
/*PMVA FIN*/

/*PRV DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP04_Commande_PRV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp15(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PRV.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_PRV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp15" ;
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
        XZEPT_CMD_PRV   vl_Commande_PRV;       /* Commande a effectuee */

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */
	char		vl_vitesse[10];
	char		vl_bandeau[20];

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp15 : debut d execution ");

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

        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - recuperation du nombre de parametre \n");
        /*A
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc
        */

        if (vl_numparams != ca_acrp15_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
                vl_ok = CS_FALSE;
        }
        else
        {
        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - le nombre de parametre attendu est ok \n");
        }
        /*
        ** 1 er champ de XDY_Etat_PRV : type XDY_Eqt = CS_SMALLINT
        */
    	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 001\n");
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PRV.Numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 001 OK NUMERO \n");
        }

        /*
        ** 2 eme champ de XDY_Etat_PRV : type XDY_Ligne_PRV
        */
        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 002 BANDEAU : \n");
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_bandeau , 20))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        else
        {

            XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 002 OK BANDEAU\n");

        }

        /*
        ** 3 eme champ de XDY_Etat_PRV : type XDY_Octet = CS_TINYINT
        */
        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 003 VITESSE \n");
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_vitesse, 10)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        else
        {
        	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 003 ok VITESSE\n");
        }

         /*
        ** 4 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
        */
        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 004 NOCMD\n");
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        else
        {
        	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 004 ok NOCMD : %d\n",vl_NoCMD_In);
        }

        /*
        ** 5 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */
        XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 005 NOM MACHINE \n");
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        else
               {
               	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp15 - TRACE 005 ok NOM MACHINE : %s\n",vl_NomMachine_In);
               }




        /*A
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

  	   XZST_03EcritureTrace(XZSTC_INFO ,"Transfert des parametres \n");

        if (!vl_ok )
        {

                /*ADepassSeuil
                ** Initialise le message d erreur a renvoyer si un probleme est
                ** apparu.
                */

                /*
                ** mise a zero du vl_buffer de message
                */
                srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));
          	   XZST_03EcritureTrace(XZSTC_INFO ,"mise a zero du vl_buffer de message \n");

                /*
                ** indicateur du type de message et de sa gravite
                */

                vl_msg.msgnumber = RPC_FAIL;
                vl_msg.severity = 10;

                /*A
                ** Initialisation du message pour le client
                */

                (CS_VOID)sprintf(vl_msg.text,"acrp15 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);
                XZST_03EcritureTrace(XZSTC_INFO ,"Initialisation du message pour le client \n");
                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : cs_ctx_alloc() a echoue.\n");

                }
                else
                {
                        /*
                        ** Recuperation du nom du serveur
                        */
                        (CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                        (CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
                        XZST_03EcritureTrace(XZSTC_INFO ,"Recuperation du nom du serveur \n");
                }
                /*B
                **  Envoi du message d erreur au client
                */
                if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)

                		XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : srv_sendinfo() a echoue.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp15 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : srv_sendstatus() a echoue.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp15 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        XZST_03EcritureTrace(XZSTC_INFO ,"Debut envoi au client la confirmation du traitement de la demande\n");
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
		strncpy(vl_Commande_PRV.Bandeau,vl_bandeau,10);
		strcpy(vl_Commande_PRV.Vitesse,vl_vitesse);
		vl_Commande_PRV.Bandeau[9]='\0';
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Champs pour la structure XDY_Etat_PRV\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Valeur recue : \t vl_Commande_PRV.Numero     \t= %d",vl_Commande_PRV.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Valeur recue : \t vl_Commande_PRV.Bandeau     \t= %s strlen = %d",vl_Commande_PRV.Bandeau,strlen(vl_Commande_PRV.Bandeau));
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Valeur recue : \t vl_Commande_PRV.Vitesse \t= %s",vl_Commande_PRV.Vitesse);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp15  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                /*A
                ** Execution de la requete en elle meme : XZEP04_Commande_PRV
                */

                XZST_03EcritureTrace(XZSTC_INFO ,"Execution de la requete en elle meme : XZEP06_Commande_PRV \n");
                vl_retcode = XZEP06_Commande_PRV(vl_Commande_PRV,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : XZEP06_Commande_PRV() a echoue : code =%d",vl_retcode);

                }
                XZST_03EcritureTrace(XZSTC_INFO ,"Execution requete ok\n");

                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */

                /*transmet l'état au client*/
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }
                XZST_03EcritureTrace(XZSTC_INFO ,"traitement de la requete ok\n");



                /* Envoi du paquet final au client et signal que le traitement est termine.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp15 : srv_senddone() a echoue.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp15 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }
        XZST_03EcritureTrace(XZSTC_INFO ,"Scrute les messages du RTserver \n");
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp15 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp15 : Fin d execution ");
        return (CS_SUCCEED);

}
/*PRV FIN*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*
*  acrp12 est appele par un SQL serveur et fait appel a xzen13()
*  commande BAF par modules
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp12(SRV_PROC        *pa_srvproc)

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
*   Doit etre invoquee sur validation de la fenetre de pilotage BAD.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_BAD en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_BAD_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp13" ;
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

        XZENT_CMD_BAF  vl_Commande_BAF; /* Commande a effectuee */
        XZENT_CMD_BAF  vl_Commande_BAF_globale[6]; /* Commande a effectuee */
        CS_TINYINT      vl_Module11_In;
        CS_TINYINT      vl_Module12_In;
        CS_TINYINT      vl_Module15_In;
        CS_TINYINT      vl_Module21_In;
        CS_TINYINT      vl_Module22_In;
        CS_TINYINT      vl_Module25_In;

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp13 : debut d execution ");

        /*A
        **  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        vl_retcode = XDC_OK;
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

        if (vl_numparams != ca_acrp12_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de  acrp03: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_BAF.NoEqt))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module11_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module12_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module15_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module21_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module22_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_Module25_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp12: parametre %d errone",vl_param_no);
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

                (CS_VOID)sprintf(vl_msg.text,"acrp12 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp12 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp12 : srv_sendinfo() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp12 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp12 : srv_sendstatus() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp12 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }


        /*A Envoi au client la confirmation du traitement de la demande  */

        if (vl_ok)
        {
                /*module 11*/
		vl_Commande_BAF_globale[0].NoEqt = vl_Commande_BAF.NoEqt;
		vl_Commande_BAF_globale[0].Cligno=0;
		vl_Commande_BAF_globale[0].Message=vl_Module11_In;
                vl_Commande_BAF.Module=0;
                vl_Commande_BAF.Cligno=0;
                vl_Commande_BAF.Message=vl_Module11_In;
                /* vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In);*/
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);
                /*module 12*/
                vl_Commande_BAF.Module=1;
                vl_Commande_BAF.Message=vl_Module12_In;
		vl_Commande_BAF_globale[1].Message=vl_Module12_In;
               /* vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In);*/
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);
                /*module 15*/
                vl_Commande_BAF.Module=2;
                vl_Commande_BAF.Message=vl_Module15_In;
		vl_Commande_BAF_globale[2].Message=vl_Module15_In;
                /*vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In);*/
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);
                /*module 21*/
                vl_Commande_BAF.Module=3;
                vl_Commande_BAF.Message=vl_Module21_In;
		vl_Commande_BAF_globale[3].Message=vl_Module21_In;
                /*vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In); */
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);
                /*module 22*/
                vl_Commande_BAF.Module=4;
                vl_Commande_BAF.Message=vl_Module22_In;
		vl_Commande_BAF_globale[4].Message=vl_Module22_In;
                /*vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In);*/
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);
                /*module 25*/
                vl_Commande_BAF.Module=5;
                vl_Commande_BAF.Message=vl_Module25_In;
		vl_Commande_BAF_globale[5].Message=vl_Module25_In;
                /*vl_retcode = XZEN10_Commande_BAF(vl_Commande_BAF,vl_NoCMD_In,vl_NomMachine_In);*/
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"acrp12 : XZEN10_Commande_BAF() a ÅchouÅ : code =%d",vl_retcode);

              
	      vl_retcode = XZEN16_Commande_BAF_globale(vl_Commande_BAF_globale,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,
					"acrp12 : XZEN16_Commande_BAF_globale() a ÅchouÅ : code =%d",vl_retcode);
	


                /*A Initialisation du vl_status a vl_retcode indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp12 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp12 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /*A
                ** Envoi du paquet final au client.
                */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp12 : srv_senddone() a echouÅ.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp12 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp12 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp12 : Fin d execution ");
        return (CS_SUCCEED);

}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*
*  acrp13 est appele par un SQL serveur et fait appel È xzen13()
*  commande BAD par scenario
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp13(SRV_PROC        *pa_srvproc)

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
*   Doit etre invoquee sur validation de la fenetre de pilotage BAD.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_BAD en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_BAD_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp13" ;
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

        XZENT_CMD_SCEN_BAD  vl_Commande_SCEN;       /* Commande a effectuee */

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp13 : debut d execution ");

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

        if (vl_numparams != ca_acrp13_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp13: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de  acrp03: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SCEN.NoEqt))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp13: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme de type XDY_Scenario : char de XDC_Lon_Scenario
        */
if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_SCEN.Scenar , 11))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp13: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp13: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp13: parametre %d errone",vl_param_no);
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

                (CS_VOID)sprintf(vl_msg.text,"acrp13 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : srv_sendinfo() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp13 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : srv_sendstatus() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp13 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

                XZST_03EcritureTrace(XZSTC_SGBD," acrp13 : Champs pour la structure XDY_Etat_BAD\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp13 : Valeur recue : \t vl_Commande_SCEN.NoEqt     \t= %d",vl_Commande_SCEN.NoEqt);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp13 : Valeur recue : \t vl_Commande_SCEN.Scenar    \t= %s strlen = %d",vl_Commande_SCEN.Scenar,strlen(vl_Commande_SCEN.Scenar));
                XZST_03EcritureTrace(XZSTC_SGBD," acrp13 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp13 : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);


                /* Execution de la requete en elle meme : Affichage du vl_msg*/

                vl_retcode = XZEN13_Commande_SCEN_BAD(vl_Commande_SCEN,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : XZEN13_Commande_SCEN_BAD() a ÅchouÅ : code =%d",vl_retcode);
                }

                /*A Initialisation du vl_status a vl_retcode indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp13 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /*A
                ** Envoi du paquet final au client.
                */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp13 : srv_senddone() a echouÅ.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp13 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();


        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp13 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp13 : Fin d execution ");
        return (CS_SUCCEED);

}

/* AAZ 28/08/2007  ajout acrp40, acrp41, acrp42, acrp43, acrp44*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp40 est appele par un SQL serveur et fait appel a xzet40()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp40(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence Signalisation.
*
*
* FONCTION 
*    Appel     xzet40
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp40" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp40 : debut d execution ");
    XZST_03EcritureTrace(XZSTC_WARNING, "IN : acrp40 : debut d execution ");
    XZST_03EcritureTrace(XZSTC_TOUT, "IN : acrp40 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp40_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }
    
    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 1 er champ de  acrp40: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp40 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp40 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp40 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp40 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp40 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp40 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp40 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET40_Commande_SEQSIG
        */

        vl_retcode = XZET40_Commande_SEQSIG (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : XZET40_Commande_SEQSIG() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp40 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp40 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp40 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp40 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp40 : Fin d execution ");

    return (CS_SUCCEED);

}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp41 est appele par un SQL serveur et fait appel a xzet41()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp41(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence Eclairage.
*
*
* FONCTION 
*    Appel     xzet41
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp41" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp41 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp41_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp41: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }
    
    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }
    /* 
    ** 1 er champ de  acrp41: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp41: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp41: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp41: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp41 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp41 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp41 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp41 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp41 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp41 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp41 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET41_Commande_SEQECL
        */

        vl_retcode = XZET41_Commande_SEQECL (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : XZET41_Commande_SEQECL() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp41 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp41 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp41 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp41 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp41 : Fin d execution ");

    return (CS_SUCCEED);

}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp42 est appele par un SQL serveur et fait appel a xzet42()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp42(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence Ventilation.
*
*
* FONCTION 
*    Appel     xzet42
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp42" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp42 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp42_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp42: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }

    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }
    
    /* 
    ** 1 er champ de  acrp42: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp42: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp42: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp42: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp42 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp42 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp42 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp42 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp42 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp42 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp42 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET42_Commande_SEQVEN
        */

        vl_retcode = XZET42_Commande_SEQVEN (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : XZET42_Commande_SEQVEN() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp42 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp42 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp42 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp42 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp42 : Fin d execution ");

    return (CS_SUCCEED);

}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp43 est appele par un SQL serveur et fait appel a xzet43()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp43(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence Energie.
*
*
* FONCTION 
*    Appel     xzet43
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp43" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp43 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp43_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp43: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }
    
    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }
	
    /* 
    ** 1 er champ de  acrp43: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp43: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp43: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp43: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp43 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp43 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp43 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp43 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp43 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp43 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp43 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET43_Commande_SEQENR
        */

        vl_retcode = XZET43_Commande_SEQENR (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : XZET43_Commande_SEQENR() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp43 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp43 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp43 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp43 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp43 : Fin d execution ");

    return (CS_SUCCEED);

}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp44 est appele par un SQL serveur et fait appel a xzet44()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp44(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence Protection incendie.
*
*
* FONCTION 
*    Appel     xzet44
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp44" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : acrp44 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp44_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp44: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }

    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }
    
    /* 
    ** 1 er champ de  acrp44: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp44: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp44: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp44: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp44 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp44 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp44 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp44 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp44 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp44 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp44 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET44_Commande_SEQINC
        */

        vl_retcode = XZET44_Commande_SEQINC (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : XZET44_Commande_SEQINC() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp44 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp44 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp44 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp44 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp44 : Fin d execution ");

    return (CS_SUCCEED);

}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
*  acrp45 est appele par un SQL serveur et fait appel a xzet45()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp45(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*    Pointeur vers une structure de controle thread interne
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
*   Doit etre invoquee sur validation de la fenetre de pilotage sequence exploitation.
*
*
* FONCTION 
*    Appel     xzet45
*        
*     
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	28/08/2007	: Creation
*
------------------------------------------------------*/
{
    static char         *version = "@(#)asrv_crp.c    1.1 28/08/2008 : asrv_crp45" ;
    int                 vl_retcode;    /* Code de retour                  */

    CS_BOOL             vl_ok;

    CS_INT              vl_numparams;  /* Permet de recuperer le nbre     */
                                       /* de param passes a la rpc        */

    CS_SERVERMSG        vl_msg;        /* structure permettant de ren-    */
                                       /* voyer un message d erreur au    */
                                       /* client.                         */

    CS_CONTEXT          *pl_cp;        /* Structure contenant les infos   */
                                       /* de localisation du serveur      */

    CS_INT              vl_status;     /* Contient de le vl_status du srv */ 
                                       /* apres reception de la demande   */

    CS_INT              vl_Eqt;        /* variable pour recuperer le      */
                                       /* param venant du sql serveur     */

    CS_TINYINT          vl_nump;
    
    CS_INT              vl_param_no=1;


    /* 
    ** Variables specifiques a l appel des services externes    
    */
            
    XZETT_CMD_SEQ_NIX   vl_Commande_SEQ_NIX;    /* Commande a effectuer   */
    
    CS_INT              vl_NoCMD_In;            /* Numero de la commande  */
    XDY_NomMachine      vl_NomMachine_In;       /* nom de la machine      */

    XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp45 : debut d execution ");
    
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
        
    if (vl_numparams != ca_acrp45_nb_param)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp45: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        vl_ok = CS_FALSE;
    }

    /* 
	** champ 0 : type d'ouvrage
	*/
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.TypeOuvrage))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp40: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }
    
    /* 
    ** 1 er champ de  acrp45: @va_NumeroEqt_in de type XDY_NumEqt = CS_SMALLINT
    */
    if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumDom))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp45: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 2 eme champ :vl_Commande_SEQ_NIX.NumSEQ de type  XDY_Octet : tinyint
    */

    if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_SEQ_NIX.NumSeq))!=CS_SUCCEED))
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp45: parametre %d errone",vl_param_no);
        vl_ok = CS_FALSE;
    }

    /* 
    ** 3 eme champ de l envoie de la commande :vl_NoCMD_In type  = CS_INT
    */
    if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))    
    {           
        XZST_03EcritureTrace(XZSTC_FATAL,"acrp45: parametre %d errone",vl_param_no);
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
        vl_msg.severity  = 10;
    
        /*A 
        ** Initialisation du message pour le client             
        */

        (CS_VOID)sprintf(vl_msg.text,"acrp45 : message non correct.\n");
        vl_msg.textlen = strlen(vl_msg.text);

        /* 
        ** Allocation de la structure de contexte               
        */

        if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : cs_ctx_alloc() a echoue.\n");
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
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : srv_sendinfo() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp45 : srv_sendinfo a envoye un message d erreur\n");

        /*B Initialisation du vl_status de la requete par le serveur*/

        vl_status |= SRV_DONE_ERROR;

        /*B Envoi du vl_status de la requete indiquant une erreur . */
        if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : srv_sendstatus() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp45 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
    }
        
        
    /*A Envoi au client la confirmation du traitement de la demande  */
    if (vl_ok)
    {

        XZST_03EcritureTrace(XZSTC_SGBD," acrp45 : Champs pour la structure \n");
        XZST_03EcritureTrace(XZSTC_SGBD," acrp45 : Valeur recue : \t vl_Commande_SEQ_NIX.NumDom  \t= %d",vl_Commande_SEQ_NIX.NumDom);       
        XZST_03EcritureTrace(XZSTC_SGBD," acrp45 : Valeur recue : \t vl_Commande_SEQ_NIX.NumSeq \t= %d",vl_Commande_SEQ_NIX.NumSeq);
        XZST_03EcritureTrace(XZSTC_SGBD," acrp45 : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);

        /*A
        ** Execution de la requete en elle meme : XZET45_Commande_SEQEXP
        */

        vl_retcode = XZET45_Commande_SEQEXP (vl_Commande_SEQ_NIX,vl_NoCMD_In);
        if (vl_retcode != XDC_OK)
        {
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : XZET45_Commande_SEQEXP() a echoue : code =%d",vl_retcode);
        }
        /*A Initialisation du vl_status a 1 indiquant le succes du
        **  traitement de la requete.
        */
        if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
            {
                vl_status |= SRV_DONE_ERROR;
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : srv_sendstatus() a echoue.\n");
            }
        else
            {
                XZST_03EcritureTrace(XZSTC_SGBD,"acrp45 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
            }

        /*A 
        ** Envoi du paquet final au client.                             
        */
        if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
            XZST_03EcritureTrace(XZSTC_FATAL,"acrp45 : srv_senddone() a echoue.\n");
        else 
            XZST_03EcritureTrace(XZSTC_SGBD,"acrp45 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
    }
    /*A
    **    Scrute les messages du RTserver afin de declencher les CallBacks
    */    
    asrv_lire_RTSRV_MSG();

    XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp45 : execution reussie");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp45 : Fin d execution ");

    return (CS_SUCCEED);

}


/*PMVA DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP104_Commande_Eqt_SAGA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp111(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMVA.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_PMVA en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMVA_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp111" ;
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
        XZEPT_CMD_EQT_SAGA   vl_Commande_PMV;       /* Commande a effectuee */
	XDY_Mot vl_numero;
	XDY_Mot vl_lumi;
	XDY_Octet vl_cligno;
	XDY_Mot vl_barriere;
	XDY_Mot vl_scenario;
	char vl_picto[20];
	char vl_bandeau[20];
        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp111 : debut d execution ");

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

        if (vl_numparams != ca_acrp111_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp111: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp111_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de XDY_Etat_PMV : type XDY_Eqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte1 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
        /*
        ** 3 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage1))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat1 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte2 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 6 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage2))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }


        /*
        ** 7 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat2 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 8 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte3 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 9 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage3))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 10 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat3 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 11 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte4 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 12 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage4))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 13 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat4 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 14 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Texte5 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 15 eme champ de XDY_Etat_PMV : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Affichage5))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 16 eme champ de XDY_Etat_PMV : type XDY_Ligne_PMV
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Commande_PMV.Alternat5 , XDC_Lon_Ligne_Eqt_SAGA))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 17 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */
        /*if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_PMV.Clignotement))!=CS_SUCCEED))*/
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_cligno)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
	vl_Commande_PMV.Clignotement = vl_cligno;
        /*
        ** 18 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
        */

        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_lumi)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
	vl_Commande_PMV.Luminosite = vl_lumi;	

        /*
        ** 19 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_picto, 11)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
	strcpy(vl_Commande_PMV.Picto, vl_picto);
        /*
        ** 20 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
        */


        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_bandeau,11)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
	strcpy(vl_Commande_PMV.Bandeau, vl_bandeau);
        /*
        ** 21 eme champ de XDY_Etat_PMV : type XDY_Booleen = CS_TINYINT
        */

        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_barriere)!=CS_SUCCEED))
         {
                 XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                 vl_ok = CS_FALSE;
         }
	XZST_03EcritureTrace(XZSTC_FONCTION, "BARRIERE %d",vl_barriere);
	vl_Commande_PMV.Barriere = vl_barriere;
         /*
         ** 22 eme champ de XDY_Etat_PMV : type XDY_Mot = CS_SMALLINT
         */


         if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_scenario)!=CS_SUCCEED))
         {
                 XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                 vl_ok = CS_FALSE;
         }
	XZST_03EcritureTrace(XZSTC_FONCTION, "SCENARIO %d",vl_scenario);
	vl_Commande_PMV.Scenario = vl_scenario;	

        /*
        ** 23 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }
		
        /*
        ** 24 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01: parametre %d errone",vl_param_no);
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

                /*ADepassSeuil
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

                (CS_VOID)sprintf(vl_msg.text,"acrp11 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : srv_sendinfo() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp11 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp11 : srv_sendstatus() a echouÅ.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp11 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

	strcpy(vl_Commande_PMV.Bandeau, vl_bandeau);
	strcpy(vl_Commande_PMV.Picto, vl_picto);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11 : Champs pour la structure XDY_Etat_PMVA\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Numero     \t= %d",vl_Commande_PMV.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte1     \t= %s strlen = %d",vl_Commande_PMV.Texte1,strlen(vl_Commande_PMV.Texte1));
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage1 \t= %d",vl_Commande_PMV.Affichage1);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat1  \t= %s",vl_Commande_PMV.Alternat1);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte2     \t= %s",vl_Commande_PMV.Texte2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage2 \t= %d",vl_Commande_PMV.Affichage2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat2  \t= %s",vl_Commande_PMV.Alternat2);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte3     \t= %s",vl_Commande_PMV.Texte3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage3 \t= %d",vl_Commande_PMV.Affichage3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat3  \t= %s",vl_Commande_PMV.Alternat3);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte4     \t= %s",vl_Commande_PMV.Texte4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage4 \t= %d",vl_Commande_PMV.Affichage4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat4  \t= %s",vl_Commande_PMV.Alternat4);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Texte5     \t= %s",vl_Commande_PMV.Texte5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Affichage5 \t= %d",vl_Commande_PMV.Affichage5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Clignotement     \t= %d",vl_Commande_PMV.Clignotement);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Alternat5  \t= %s",vl_Commande_PMV.Alternat5);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Luminosite      \t= %d",vl_Commande_PMV.Luminosite);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Scenario      \t= %d",vl_Commande_PMV.Scenario);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Barriere      \t= %d",vl_Commande_PMV.Barriere);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Picto      \t= %s",vl_Commande_PMV.Picto);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_Commande_PMV.Bandeau      \t= %s",vl_Commande_PMV.Bandeau);

                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp11  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                /*A
                ** Execution de la requete en elle meme : XZEP104_Commande_Eqt_SAGA
                */


                vl_retcode = XZEP104_Commande_Eqt_SAGA(vl_Commande_PMV,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp111 : XZEP104_Commande_Eqt_SAGA() a echoue : code =%d",vl_retcode);

                }


                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }




                /* Envoi du paquet final au client.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp01 : srv_senddone() a echouÅ.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp01 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }

        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp111 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp111 : Fin d execution ");
        return (CS_SUCCEED);

}

/*CFE DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP07_Commande_CFE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp60(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage CFE.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_CFG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp60" ;
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
        XZEPT_CMD_CFE   vl_Commande_CFE;       /* Commande a effectuer */

        CS_INT          vl_NoCMD_In;            /* Numero de la commande*/
        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp60 : debut d execution ");

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

        if (vl_numparams != ca_acrp60_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp60_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de XDY_Etat_CFE : type XDY_Eqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_CFE.Numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme champ de XDY_Etat_CFE : type XDY_Octet = CS_TINYINT
        */
        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Commande_CFE.NoPlanDeFeux))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

         /*
        ** 3 eme champ de l envoi de la commande :vl_NoCMD type  = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoi de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60: parametre %d errone",vl_param_no);
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

                /*ADepassSeuil
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

                (CS_VOID)sprintf(vl_msg.text,"acrp60 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : srv_sendinfo() a echouÅ\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp60 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : srv_sendstatus() a echouÅ\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp60 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

                XZST_03EcritureTrace(XZSTC_SGBD," acrp60 : Champs pour la structure XDY_Etat_CFE\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp60  : Valeur recue : \t vl_Commande_CFE.Numero     \t= %d",vl_Commande_CFE.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp60  : Valeur recue : \t vl_Commande_CFE.NoPlanDeFeux \t= %d",vl_Commande_CFE.NoPlanDeFeux);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp60  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_NoCMD_In);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp60  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                /*A
                ** Execution de la requete en elle meme : XZEP07_Commande_CFE
                */

                vl_retcode = XZEP07_Commande_CFE(vl_Commande_CFE,vl_NoCMD_In,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : XZEP07_Commande_PRV() a echoue : code =%d",vl_retcode);
		}

                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /* Envoi du paquet final au client.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp60 : srv_senddone() a echoue\n");
                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp60 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }

        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp60 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp60 : Fin d execution ");
        return (CS_SUCCEED);

}
/*CFE FIN*/

/*IMU DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP07_Commande_IMU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp70(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage IMU.
*
*
* FONCTION
*       Appel   xzep01
*               xzep02
*
*
*   Construction du message XDM_ECMD_CFG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp70" ;
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
        XZEPT_CMD_IMU   vl_Commande_IMU;       /* Commande a effectuer */

        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp70 : debut d execution ");

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

        if (vl_numparams != ca_acrp70_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp70_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de XDY_Etat_IMU : type XDY_Eqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_IMU.Numero))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme champ de XDY_Etat_IMU : type Char = char
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_Commande_IMU.ChaineIMU  ,  XDC_Lon_ChaineIMU))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

	/*
        ** 3 eme champ de XDY_Etat_IMU : type int = CS_INT
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Commande_IMU.Intervalle))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoi de la commande : type int = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Commande_IMU.Cycles))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme pour l envoi de la commande : type int = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Commande_IMU.Duree))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 6 eme pour l envoi de la commande : type Xint = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Commande_IMU.NoAction))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 7 eme pour l envoi de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70: parametre %d errone",vl_param_no);
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

                /*ADepassSeuil
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

                (CS_VOID)sprintf(vl_msg.text,"acrp70 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : srv_sendinfo() a echoue\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp70 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : srv_sendstatus() a echoue\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp70 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

                XZST_03EcritureTrace(XZSTC_SGBD," acrp70 : Champs pour la structure XDY_Etat_IMU\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_Commande_IMU.Numero     \t= %d",vl_Commande_IMU.Numero);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_Commande_IMU.ChaineIMU \t= %s",vl_Commande_IMU.ChaineIMU);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_Commande_IMU.Intervalle \t= %d",vl_Commande_IMU.Intervalle);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_Commande_IMU.Cycles \t= %d",vl_Commande_IMU.Cycles);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_Commande_IMU.Duree \t= %d",vl_Commande_IMU.Duree);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_Commande_IMU.NoAction);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp70  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                /*A
                ** Execution de la requete en elle meme : XZEP08_Commande_IMU
                */

                vl_retcode = XZEP08_Commande_IMU(vl_Commande_IMU,vl_Commande_IMU.NoAction,vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : XZEP08_Commande_IMU() a echoue : code =%d",vl_retcode);
		}

                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /* Envoi du paquet final au client.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp70 : srv_senddone() a echoue\n");
                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp70 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }

        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp70 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp70 : Fin d execution ");
        return (CS_SUCCEED);

}
/*IMU FIN*/

/*SONO DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZEP09_Commande_RAU_SONO
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE acrp80(SRV_PROC        *pa_srvproc)

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
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage SONO.
*
*
* FONCTION
*       Appel   xzep09
*
*
*   Construction du message XDM_ECMD_CFG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
*
*
*
------------------------------------------------------*/
{
        static char *version = "@(#)asrv_crp.c	1.19 1.19 11/04/09 : asrv_crp70" ;
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
        XZEPT_CMD_SONO   vl_Commande_SONO;       /* Commande a effectuer */

	char vl_Message[50];
	char vl_Autoroute[50];
	
	XDY_Sens vl_Sens;
	XDY_PR	vl_PR_debut;
	XDY_PR	vl_PR_fin;
	int 	vl_NoAction;

        XDY_NomMachine  vl_NomMachine_In;       /* nom de la machine    */

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp80 : debut d execution ");

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

        if (vl_numparams != ca_acrp80_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp80_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 eme champ de XDY_Etat_SONO : type Char = char
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_Message  ,  XDC_Lon_MessageSono))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

	/*
        ** 2 eme champ de XDY_Etat_SONO : type int = char
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_Autoroute , XDC_Lon_NomAuto))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme pour l envoi de la commande : type int = CS_INT
        */

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Sens))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme pour l envoi de la commande : type int = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_PR_debut))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme pour l envoi de la commande : type int = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_PR_fin))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 6 eme pour l envoi de la commande : type Xint = CS_INT
        */

        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_NoAction))!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 7 eme pour l envoi de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine
        */

        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80: parametre %d errone",vl_param_no);
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

                /*ADepassSeuil
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

                (CS_VOID)sprintf(vl_msg.text,"acrp80 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */

                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : cs_ctx_alloc() a echoue.\n");
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

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : srv_sendinfo() a echoue\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp80 : srv_sendinfo a envoye un message d erreur\n");

                /*B Initialisation du vl_status de la requete par le serveur*/

                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : srv_sendstatus() a echoue\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"acrp80 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }

        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {

		strcpy(vl_Commande_SONO.Message,vl_Message);
		strcpy(vl_Commande_SONO.Autoroute,vl_Autoroute);
		vl_Commande_SONO.Sens = vl_Sens;
		vl_Commande_SONO.PR_debut = vl_PR_debut;
		vl_Commande_SONO.PR_fin = vl_PR_fin;
		vl_Commande_SONO.NoAction = vl_NoAction;

                XZST_03EcritureTrace(XZSTC_SGBD," acrp80 : Champs pour la structure XDY_Etat_SONO\n");
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_Commande_SONO.Message \t= %s",vl_Commande_SONO.Message);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_Commande_SONO.Autoroute \t= %s",vl_Commande_SONO.Autoroute);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_Commande_SONO.Sens \t= %d",vl_Commande_SONO.Sens);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_Commande_SONO.PR_debut \t= %d",vl_Commande_SONO.PR_debut);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_Commande_SONO.PR_fin \t= %d",vl_Commande_SONO.PR_fin);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_NoCMD_In                \t= %d",vl_Commande_SONO.NoAction);
                XZST_03EcritureTrace(XZSTC_SGBD," acrp80  : Valeur recue : \t vl_NomMachine_In           \t= %s",vl_NomMachine_In);

                /*A
                ** Execution de la requete en elle meme : XZEP09_Commande_RAU_SONO
                */

                vl_retcode = XZEP09_Commande_RAU_SONO(vl_Commande_SONO,vl_Commande_SONO.NoAction, vl_NomMachine_In);
                if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : XZEP09_Commande_RAU_SONO() a echoue : code =%d",vl_retcode);
		}

                /*A Initialisation du vl_status a 1 indiquant le succes du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : srv_sendstatus() a echoue.\n");
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

                /* Envoi du paquet final au client.                             */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                XZST_03EcritureTrace(XZSTC_FATAL,"acrp80 : srv_senddone() a echoue\n");
                else XZST_03EcritureTrace(XZSTC_SGBD,"acrp80 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }

        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();

        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : acrp80 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acrp80 : Fin d execution ");
        return (CS_SUCCEED);

}
/*SONO FIN*/
							

/*PIC DEBUT*/
/*----------------------------------------------------
* SERVICE RENDU :
*       Recoit les parametres du SQL Serveur et appelle
*       XZAC5501_Commande_PIC
------------------------------------------------------
*/
CS_RETCODE acrp16(SRV_PROC        *pa_srvproc)
/*
* ARGUMENTS EN ENTREE :
*
*       Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*   XZEXC_ERR_CLIGN     Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PIC.
*
*   Construction du message XDM_ECMD_PIC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDM_ECMD_PIC_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
	static char		*version = "@(#)asrv_crp.c	: asrv_crp16" ;
	int             vl_retcode;     /* Code de retour */
	CS_BOOL         vl_ok;
	CS_INT          vl_numparams;	/* Permet de recuperer le nbre de param passes a la rpc */
	CS_SERVERMSG    vl_msg;         /* structure permettant de renvoyer un message d erreur au client. */
	CS_CONTEXT      *pl_cp;         /* Structure contenant les infos de localisation du serveur */
	CS_INT          vl_status;      /* Contient de le vl_status du srv apres reception de la demande */
	CS_INT          vl_Eqt;         /* variable pour recuperer le param venant du sql serveur */
	CS_TINYINT      vl_nump;
	CS_INT          vl_param_no=1;

	/* Variables specifiques a l appel des services externes */
	XZEPT_CMD_PIC   	vl_Commande_PIC;       /* Commande a effectuee */
	CS_INT          	vl_NoCMD_In;            /* Numero de la commande*/
	XDY_NomMachine  	vl_NomMachine_In;       /* nom de la machine    */
	XDY_Picto_PIC		vl_Picto;
	XDY_Bandeau_PIC		vl_Bandeau;
	XDY_Booleen			vl_Flash;

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acrp16 : debut d execution ");

	/* initialisation du vl_status a OK pour fin de transaction
		Si on oublie vl_status, on peut bloquer la transaction  */
	vl_status = SRV_DONE_FINAL;

	/* initialisation de vl_ok a TRUE */
	vl_ok = CS_TRUE;

	/* verification du contenu du parametre venant du client */
	/* recupere le nombre de parametres et on teste */
	if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
		vl_ok = CS_FALSE;

	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - recuperation du nombre de parametre \n");

	/* On verifie qu'il y a bien le nb de param attendu passe a la rpc */
	if (vl_numparams != ca_acrp16_nb_param)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp16_nb_param);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - le nombre de parametre attendu est ok \n");
	}

	/* 1 er champ de XDY_Etat_PIC : type XDY_Eqt = CS_SMALLINT */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 001\n");
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&(vl_Commande_PIC.Numero))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 001 OK NUMERO \n");
	}

	/* 2 eme champ de XDY_Etat_PIC : type XDY_Bandeau_PIC (char) */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 002 Bandeau : \n");
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++, vl_Bandeau , XDC_LGR_Bandeau_PIC))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 002 OK Bandeau\n");
	}

	/* 3 eme champ de XDY_Etat_PIC : type XDY_Booleen (char) */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 003 Flash \n");
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_Flash, 1)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 003 ok Flash\n");
	}

	/* 4 eme champ de XDY_Etat_PIC : XDY_Picto_PIC (char) */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 004 NOCMD\n");
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_Picto, XDC_LGR_Picto_PIC))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 004 ok NOCMD : %d\n",vl_Picto);
	}

	/* 5 eme champ de l envoie de la commande :vl_NoCMD type  = CS_INT */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 005 NOCMD\n");
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_NoCMD_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 005 ok NOCMD : %d\n",vl_NoCMD_In);
	}

	/* 6 eme pour l envoie de la commande : type XDY_NomMachine : char de XDC_Lon_NomMachine */
	XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 006 NOM MACHINE \n");
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In  , XDC_Lon_NomMachine))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"acrp16: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"acrp16 - TRACE 006 ok NOM MACHINE : %s\n",vl_NomMachine_In);
	}

	/* Transfert des parametres dans les variables locales au programme */
	if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
		vl_ok = CS_FALSE;

	XZST_03EcritureTrace(XZSTC_INFO, "Transfert des parametres \n");

	if (!vl_ok )
	{
		/* DepassSeuil : Initialise le message d erreur a renvoyer si un probleme est apparu. */

		/* mise a zero du vl_buffer de message */
		srv_bzero (&vl_msg, sizeof(CS_SERVERMSG));
		XZST_03EcritureTrace (XZSTC_INFO, "mise a zero du vl_buffer de message \n");

		/* indicateur du type de message et de sa gravite */
		vl_msg.msgnumber = RPC_FAIL;
		vl_msg.severity = 10;

		/* Initialisation du message pour le client */
		(CS_VOID)sprintf(vl_msg.text,"acrp16 : message non correct.\n");
		vl_msg.textlen = strlen(vl_msg.text);
		XZST_03EcritureTrace(XZSTC_INFO ,"Initialisation du message pour le client \n");
		
		/* Allocation de la structure de contexte */
		if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : cs_ctx_alloc() a echoue.\n");
		}
		else
		{
			/* Recuperation du nom du serveur */
			(CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
			(CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
			XZST_03EcritureTrace(XZSTC_INFO, "Recuperation du nom du serveur \n");
		}

		/* Envoi du message d erreur au client */
		if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : srv_sendinfo() a echoue.\n");
		else
			XZST_03EcritureTrace(XZSTC_SGBD,"acrp16 : srv_sendinfo a envoye un message d erreur\n");

		/* Initialisation du vl_status de la requete par le serveur */
		vl_status |= SRV_DONE_ERROR;

		/* Envoi du vl_status de la requete indiquant une erreur. */
		if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : srv_sendstatus() a echoue.\n");
		else
			XZST_03EcritureTrace(XZSTC_SGBD,"acrp16 : srv_sendstatus() a renvoye vl_status = %d", vl_status);
	}

	XZST_03EcritureTrace(XZSTC_INFO ,"Debut envoi au client la confirmation du traitement de la demande\n");
	/*A Envoi au client la confirmation du traitement de la demande  */
	if (vl_ok)
	{
		strncpy(vl_Commande_PIC.Picto, vl_Picto, XDC_LGR_Picto_PIC);
		strncpy(vl_Commande_PIC.Bandeau, vl_Bandeau, XDC_LGR_Bandeau_PIC);
		vl_Commande_PIC.Picto[XDC_LGR_Picto_PIC-1]='\0';
		vl_Commande_PIC.Bandeau[XDC_LGR_Bandeau_PIC-1]='\0';
		vl_Commande_PIC.Flash = vl_Flash;
		
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Champs pour la structure XDY_Etat_PIC\n");
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_Commande_PIC.Numero		\t= %d",vl_Commande_PIC.Numero);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_Commande_PIC.Picto		\t= %s strlen = %d", vl_Commande_PIC.Picto, strlen(vl_Commande_PIC.Picto));
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_Commande_PIC.Bandeau	\t= %s strlen = %d", vl_Commande_PIC.Bandeau, strlen(vl_Commande_PIC.Bandeau));
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_Commande_PIC.Flash 		\t= %s", vl_Commande_PIC.Flash);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_NoCMD_In				\t= %d", vl_NoCMD_In);
		XZST_03EcritureTrace(XZSTC_SGBD," acrp16 : Valeur recue : \t vl_NomMachine_In			\t= %s", vl_NomMachine_In);

		/* Execution de la requete en elle meme : XZEP10_Commande_PIC */
		XZST_03EcritureTrace(XZSTC_INFO ,"Execution de la requete en elle meme : XZEP10_Commande_PIC \n");
		vl_retcode = XZEP10_Commande_PIC (vl_Commande_PIC,vl_NoCMD_In,vl_NomMachine_In);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : XZEP10_Commande_PIC () a echoue : code =%d",vl_retcode);
		}
		XZST_03EcritureTrace(XZSTC_INFO ,"Execution requete ok\n");

		/* Initialisation du vl_status a 1 indiquant le succes du traitement de la requete. */
		/* transmet l'état au client*/
		if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
		{
			vl_status |= SRV_DONE_ERROR;
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : srv_sendstatus() a echoue.\n");
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_SGBD,"srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
		}
		XZST_03EcritureTrace(XZSTC_INFO ,"traitement de la requete ok\n");

		/* Envoi du paquet final au client et signal que le traitement est termine. */
		if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
			XZST_03EcritureTrace(XZSTC_FATAL,"acrp16 : srv_senddone() a echoue.\n");
		else 
			XZST_03EcritureTrace(XZSTC_SGBD,"acrp16 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	XZST_03EcritureTrace(XZSTC_INFO, "Scrute les messages du RTserver \n");

	/* Scrute les messages du RTserver afin de declancher les CallBacks */
	asrv_lire_RTSRV_MSG();

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : acrp16 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acrp16 : Fin d execution ");

	return (CS_SUCCEED);
}
/*PIC FIN*/