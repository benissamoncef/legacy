/*E*/
/*Fichier : $Id: asrv_trp.c,v 1.8 2014/04/29 09:56:32 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2014/04/29 09:56:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE TRP * FICHIER asrv_trp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	Module contenant des registered Procedures.
*	Les tests sont contenus dans BASEDD/tests: 
*	Lorsque l'Open Serveur est lance, on peut lancer atrp18.sh
*	et recuperer dans atrp18.results les resultats renvoyes au SQL Serveur.
*****************************
*
*	atrp18 faisant appel a 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	26 Sep 1994	: Creation
* gaborit	06 jan 1995	: modif trace (1.4)
* Mismer	04 Dec 1996	: modif alerte PAU HS ajout appel de XZEC07_Alarme_Eqt (DEM/1308) (1.5)
* JMG		15/09/09 	FACTIS DEM914
* JPL		17/11/10	: Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_trp.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_trp.c,v 1.8 2014/04/29 09:56:32 pc2dpdy Exp $ : asrv_trp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Pour l instant on ne s en sert pas
*  RP affaisant appel a xzit07
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE atrp18(SRV_PROC        *pa_srvproc)

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
*	Si vl_Changement_In = HS alors on appelle XZEC03_Mise_EnHorsSrv_Eqt()
*	Si vl_Etat_In = HS alors on appelle XZEC04_Activ_Desactiv_Eqt()
*	XZIS02 (ETAT + CHGT).
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_trp.c,v 1.8 2014/04/29 09:56:32 pc2dpdy Exp $ : asrv_trp18" ;
	
	int		vl_retcode	=XDC_OK;	/* Code de retour des fonctions des services	*/
	int		vl_codeBit	=0;	/* 
						** Code renvoye au SQL Serveur : 
						0 -> pas d erreur
						1 -> premier service a retourne une erreur	
						2 -> 2 eme service a retourne une erreur	
						4 -> 3 eme service a retourne une erreur
						*/
	int		vl_Code;	/* Code a passer aux fonctions des equipements*/			
	
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
	
	CS_TINYINT	vl_nump;
	
	CS_INT		vl_param_no=1;

	
	
        /* 
        ** Variables specifiques a l appel des services externes	
        */
        int i;
        XDY_NomSite		vl_NomSite;	
	XDY_NomMachine  	vl_NomMachine_In;	/* nom de la machine	*/
	CS_TINYINT		vl_TypeEqt_In;
	CS_SMALLINT		vl_NumEqt_In;
	CS_SMALLINT		vl_Changement_In;
	CS_SMALLINT		vl_Etat_In;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : atrp18 : debut d execution ");

	/*A
	** Recupere le nom du site afin de le passer aux equipements.
	*/
	if( (vl_retcode =XZSC_04NomSite( vl_NomSite))!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "atrp18 : XZSC_07NomSite a echoue : Pas recuperer le nom du site : code = %d",vl_retcode );
		exit( XDC_NOK );
	}
	
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
        
        if (vl_numparams != ca_atrp18_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_atrp18_nb_param);
        	vl_ok = CS_FALSE;
        }

        /* 
       	** 1 er champ de l envoie de la commande :vl_NomMachine_In type CS_CHAR de long XDC_Lon_NomMachine
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In,XDC_Lon_NomMachine)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
        /* 
       	** 2 eme champ atrp18: vl_TypeEqt de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_TypeEqt_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
        /* 
       	** 3 eme champ de  atrp18: vl_NumEqt de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_NumEqt_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
        
        /* 
       	** 4 eme champ de  atrp18: vl_Changement_In de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_Changement_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       
        /* 
       	** 5 eme champ de  atrp18: vl_Etat_In  = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_Etat_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"atrp18 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_sendinfo() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_sendstatus() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        /*A 
        ** Envoi au client la confirmation du traitement de la demande  
        */
        if (vl_ok)
        {
                for (i=XDC_Lon_NomMachine-1;i>0;i--)
                {
                        if (vl_NomMachine_In[i-1]==' ')
                                vl_NomMachine_In[i-1]='\0';
                        else
                                i=0;
                }
        
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: vl_NomMachine_In = [%s] vl_TypeEqt_In = [%d] vl_NumEqt_In = [%d] vl_Changement_In = [%d] vl_Etat_In = [%d]",
                                        vl_NomMachine_In,
                                        vl_TypeEqt_In,
                                        vl_NumEqt_In,
                                        vl_Changement_In,
                                        vl_Etat_In);

        	/*A
        	** Execution de la requete en elle meme : 
        	*/
        	
		if (vl_Changement_In & XDC_EQT_HS)
		{
			if (vl_Etat_In & XDC_EQT_HS)
			{
				vl_Code = XDC_EQT_HORS_SRV ;
			}
			else
			{
				vl_Code = XDC_EQT_EN_SRV; 
			}
			if((vl_retcode = XZEC03_Mise_EnHorsSrv_Eqt(vl_NomSite,vl_NomMachine_In ,vl_TypeEqt_In,vl_NumEqt_In,vl_Code))!=XDC_OK)			
			{			
				XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : XZEC03_Mise_EnHorsSrv_Eqt a echoue avec nomSite %s nomMachine %s",vl_NomSite,vl_NomMachine_In);;
				vl_codeBit = 1;
			}
			else XZST_03EcritureTrace(XZSTC_DEBUG1,"atrp18 : XZEC03_Mise_EnHorsSrv_Eqt OK");
		}
		if (!(vl_Etat_In & XDC_EQT_HS)) /* si etat = HS on ne fait rien sinon ...*/
		{
			if (vl_Changement_In & XDC_EQT_DESACTIVE) /* si chgt = desactive */
			{
				if (vl_Etat_In & XDC_EQT_DESACTIVE) vl_Code = XDC_EQT_INACTIF;
				else vl_Code = XDC_EQT_ACTIF;
				
				if((vl_retcode = XZEC04_Activ_Desactiv_Eqt (vl_NomSite,vl_NomMachine_In ,vl_TypeEqt_In,vl_NumEqt_In,vl_Code))!=XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : XZEC04_Activ_Desactiv_Eqt a echoue avec nomSite %s nomMachine %s",vl_NomSite,vl_NomMachine_In);;
					vl_codeBit = vl_codeBit+2;
				}
				else XZST_03EcritureTrace(XZSTC_DEBUG1,"atrp18 : XZEC04_Activ_Desactiv_Eqt OK");
			}
			
		}
		
		vl_retcode = XZEC07_Alarme_Eqt (vl_TypeEqt_In,vl_NumEqt_In,vl_Etat_In);

		if((vl_retcode = XZIS02_Raf_Etat_Eqt(vl_TypeEqt_In,vl_NumEqt_In,vl_Etat_In))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : XZIS02_Raf_Etat_Eqt() a echoue");					
			vl_codeBit = vl_codeBit+4;
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1," Execution de XZIS02_Raf_Etat_Eqt() a reussi");
		}
		
		/*A 
		**  Initialisation du vl_status a  vl_retcode indiquant le succes ou l 'echec du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_codeBit ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_senddone() a echou�.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	
	
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : atrp18 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atrp18 : Fin d execution ");
	if (!vl_ok) vl_retcode=XDC_NOK;
	else vl_retcode=XDC_OK;
        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : retourne %d",vl_retcode+vl_codeBit);
        return (vl_retcode+vl_codeBit);
}



/*----------------------------------------------------
* SERVICE RENDU : Pour l instant on ne s en sert pas
*  RP affaisant appel a xzit07
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE atrp19(SRV_PROC        *pa_srvproc)

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
*       Si vl_Changement_In = HS alors on appelle XZEC03_Mise_EnHorsSrv_Eqt()
*       Si vl_Etat_In = HS alors on appelle XZEC04_Activ_Desactiv_Eqt()
*       XZIS02 (ETAT + CHGT).
*
------------------------------------------------------*/
{
        int             vl_retcode      =XDC_OK;        /* Code de retour des fonctions des services    */
        int             vl_codeBit      =0;     /*
                                                ** Code renvoye au SQL Serveur :
                                                0 -> pas d erreur
                                                1 -> premier service a retourne une erreur
                                                2 -> 2 eme service a retourne une erreur
                                                4 -> 3 eme service a retourne une erreur
                                                */
        int             vl_Code;        /* Code a passer aux fonctions des equipements*/

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
        CS_TINYINT      vl_nump;

        CS_INT          vl_param_no=1;

        /*
        ** Variables specifiques a l appel des services externes
        */
        XDY_NomSite             vl_NomSite;
        XDY_NomMachine          vl_NomMachine_In;       /* nom de la machine    */

        CS_TINYINT              vl_TypeEqt_In;
        CS_INT          vl_animation;
        CS_SMALLINT             vl_NumEqt_In;
        CS_SMALLINT             vl_Changement_In;
        CS_SMALLINT             vl_Etat_In;

        XZST_03EcritureTrace(XZSTC_WARNING,"IN : atrp19 : debut d execution ");

        /*A
        ** Recupere le nom du site afin de le passer aux equipements.
        */
        if( (vl_retcode =XZSC_04NomSite( vl_NomSite))!= XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "atrp18 : XZSC_07NomSite a echoue : Pas recuperer le nom du site : code = %d", vl_retcode );
                exit( XDC_NOK );
        }

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

        if (vl_numparams != ca_atrp19_nb_param)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
                vl_ok = CS_FALSE;
        }

        /*
        ** 1 er champ de l envoie de la commande :vl_NomMachine_In type CS_CHAR de long XDC_Lon_NomMachine
        */
        if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In,XDC_Lon_NomMachine)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"APRP22: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 2 eme champ atrp18: vl_TypeEqt de type  XDY_Octet : tinyint
        */

        if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_TypeEqt_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 3 eme champ de  atrp18: vl_NumEqt de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_NumEqt_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 4 eme champ de  atrp18: vl_Changement_In de type XDY_NumEqt = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_Changement_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
        ** 5 eme champ de  atrp18: vl_Etat_In  = CS_SMALLINT
        */
        if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_Etat_In)!=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
                vl_ok = CS_FALSE;
        }

        /*
         6� champ
        */
        if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_animation) !=CS_SUCCEED))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atrp18: parametre %d errone",vl_param_no);
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

                (CS_VOID)sprintf(vl_msg.text,"atrp18 : message non correct.\n");
                vl_msg.textlen = strlen(vl_msg.text);

                /*
                ** Allocation de la structure de contexte
                */
                if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : cs_ctx_alloc() a echoue.\n");
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
                if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT) -1) != CS_SUCCEED)

                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_sendstatus() a echou�.\n");
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);
        }
        /*A
        ** Envoi au client la confirmation du traitement de la demande
        */
        if (vl_ok)
        {

                /*A
                ** Execution de la requete en elle meme :
                */
                /*FACTIS*/
                if (vl_Changement_In & XDC_EQT_CRITIQUE) {
                  XZST_03EcritureTrace(XZSTC_WARNING,"atrp19: appel XZIS01_Positionner_Indicateur_Alarme avec %d",vl_animation);
                  vl_retcode =XZIS01_Positionner_Indicateur_Alarme(vl_animation==1 ? XDC_VRAI : 0);
		  if (vl_animation==1)
		  	vl_retcode =XZIT22_Modif_Liste_Alarmes_Crit();
                }

                /*A
                **  Initialisation du vl_status a  vl_retcode indiquant le succes ou l 'echec du
                **  traitement de la requete.
                */
                if (srv_sendstatus(pa_srvproc,(CS_INT)vl_codeBit ) != CS_SUCCEED)
                {
                        vl_status |= SRV_DONE_ERROR;
                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_sendstatus() a echoue.\n");
                }
                else
                        XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                /*A
                ** Envoi du paquet final au client.
                */
                if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                        XZST_03EcritureTrace(XZSTC_WARNING,"atrp18 : srv_senddone() a echoue.\n");

                else XZST_03EcritureTrace(XZSTC_SGBD,"atrp18 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
        }
        /*A
        **      Scrute les messages du RTserver afin de declancher les CallBacks
        */
        asrv_lire_RTSRV_MSG();


        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : atrp19 : execution reussie");
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atrp19 : Fin d execution ");
        if (!vl_ok) vl_retcode=XDC_NOK;
        else vl_retcode=XDC_OK;
        return (vl_retcode+vl_codeBit);

}


